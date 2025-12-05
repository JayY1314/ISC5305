#include <mpi.h>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "constants.h"
#include "utils.h"
using namespace std;

using ARR = std::vector<std::vector<double>>;
using V = std::vector<double>;
using VI = std::vector<int>;
using A1D = std::vector<double>;  // Flattened 1D matrix

/**
 * Wrapper class to time function execution using std::chrono
 */
class ChronoTimer {
public:
    /**
     * Executes a function and measures its runtime.
     *
     * @param func The function to execute
     * @param args Arguments to pass to the function
     */
    template <typename Func, typename... Args>
    void run(Func&& func, Args&&... args) {
        auto start = std::chrono::high_resolution_clock::now();
        func(std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        duration_ms_ = std::chrono::duration<double, std::milli>(end - start).count();
    }

    /**
     * Returns the duration of the last run in milliseconds.
     *
     * @return double Execution time in ms
     */
    double get_duration_ms() const {
        return duration_ms_;
    }

private:
    double duration_ms_ = 0.0;
};

/**
 * Performs triangular matrix-vector multiplication for local rows: y = A * x
 * Triangular workload: row i only processes columns 0 to k_i, where k_i = i
 *
 * @param A_local Local portion of matrix A (flattened 1D, row-major)
 * @param x Input vector (fully replicated)
 * @param y_local Local portion of output vector
 * @param row_indices Vector of global row indices assigned to this process
 * @param n Matrix dimension size
 */
void compute_rows_triangular(const A1D& A_local, const V& x, V& y_local, 
                             const VI& row_indices, int n) {
    for (size_t idx = 0; idx < row_indices.size(); ++idx) {
        int global_row = row_indices[idx];
        y_local[idx] = 0.0;
        int k_i = global_row;  // Column limit for row i
        for (int j = 0; j <= k_i; ++j) {  // Triangular: only process columns 0 to k_i
            y_local[idx] += A_local[idx * n + j] * x[j];
        }
    }
}

/**
 * Main execution entry point for MPI.
 */
int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = MATRIX_SIZE;  // Default
    string strategy = "contiguous";  // Default: contiguous (unbalanced), options: contiguous, cyclic

    // Parse command line arguments: mpirun -np [P] ./mpi_code [N] [strategy]
    if (argc >= 2) {
        n = std::stoi(argv[1]);
    }
    if (argc >= 3) {
        strategy = string(argv[2]);
    }

    const int MASTER = 0;

    // Master process initializes flattened 1D matrix A and vector x
    A1D A_flat;
    V x(n);

    if (rank == MASTER) {
        init_matrix_1d(A_flat, x, n);
    }

    // Broadcast vector x to all processes
    MPI_Bcast(x.data(), n, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

    VI row_indices;  // Global row indices assigned to this process
    A1D A_local;
    V y_local;

    if (strategy == "contiguous") {
        // Baseline: Contiguous block distribution (unbalanced for triangular workload)
        const int rows_per_process = n / size;
        int start_row = rank * rows_per_process;
        int end_row = (rank == size - 1) ? n : (rank + 1) * rows_per_process;
        int num_rows = end_row - start_row;

        // Build row indices
        for (int i = start_row; i < end_row; ++i) {
            row_indices.push_back(i);
        }

        // Prepare scatter parameters
        VI sendcounts(size);
        VI displs(size);
        for (int i = 0; i < size; ++i) {
            int i_start = i * rows_per_process;
            int i_end = (i == size - 1) ? n : (i + 1) * rows_per_process;
            sendcounts[i] = (i_end - i_start) * n;  // Number of doubles per process
            displs[i] = i_start * n;  // Displacement in flattened array
        }

        // Scatter matrix A using MPI_Scatterv
        A_local.resize(num_rows * n);
        MPI_Scatterv(rank == MASTER ? A_flat.data() : nullptr, sendcounts.data(), displs.data(),
                     MPI_DOUBLE, A_local.data(), num_rows * n, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

        y_local.resize(num_rows);
    } else if (strategy == "cyclic") {
        // Load balancing: Cyclic distribution
        // Rank r receives rows: r, r+P, r+2P, ...
        for (int i = rank; i < n; i += size) {
            row_indices.push_back(i);
        }
        int num_rows = row_indices.size();

        // For cyclic distribution, we need to manually pack and send data
        // since MPI_Scatterv with cyclic pattern is complex
        if (rank == MASTER) {
            // Master sends rows to each process
            for (int dest = 0; dest < size; ++dest) {
                VI dest_rows;
                for (int i = dest; i < n; i += size) {
                    dest_rows.push_back(i);
                }
                int dest_num_rows = dest_rows.size();

                if (dest == MASTER) {
                    // Copy data for master
                    A_local.resize(num_rows * n);
                    for (size_t idx = 0; idx < row_indices.size(); ++idx) {
                        int global_row = row_indices[idx];
                        for (int j = 0; j < n; ++j) {
                            A_local[idx * n + j] = A_flat[global_row * n + j];
                        }
                    }
                } else {
                    // Send rows to other processes
                    A1D send_buffer(dest_num_rows * n);
                    for (size_t idx = 0; idx < dest_rows.size(); ++idx) {
                        int global_row = dest_rows[idx];
                        for (int j = 0; j < n; ++j) {
                            send_buffer[idx * n + j] = A_flat[global_row * n + j];
                        }
                    }
                    MPI_Send(&dest_num_rows, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
                    MPI_Send(send_buffer.data(), dest_num_rows * n, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
                }
            }
        } else {
            // Receive rows from master
            int num_rows;
            MPI_Recv(&num_rows, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            A_local.resize(num_rows * n);
            MPI_Recv(A_local.data(), num_rows * n, MPI_DOUBLE, MASTER, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        y_local.resize(row_indices.size());
    }

    // Benchmarking Loop
    const int NUM_WARMUP = 2;
    for (int i = 0; i < NUM_WARMUP; ++i) {
        compute_rows_triangular(A_local, x, y_local, row_indices, n);
    }

    double min_time = std::numeric_limits<double>::max();
    double total_time = 0.0;
    std::vector<double> times;
    const int NUM_RUNS = 10;

    for (int i = 0; i < NUM_RUNS; ++i) {
        MPI_Barrier(MPI_COMM_WORLD);  // Sync before timing
        double start = MPI_Wtime();

        compute_rows_triangular(A_local, x, y_local, row_indices, n);

        double end = MPI_Wtime();
        double duration = (end - start) * 1000.0;  // ms

        // We want the MAX time across all ranks for the "parallel execution time"
        double max_duration;
        MPI_Reduce(&duration, &max_duration, 1, MPI_DOUBLE, MPI_MAX, MASTER, MPI_COMM_WORLD);

        if (rank == MASTER) {
            times.push_back(max_duration);
            if (max_duration < min_time) min_time = max_duration;
            total_time += max_duration;
        }
    }

    // Gather results from all processes to master
    V y(n);
    if (strategy == "contiguous") {
        // Use MPI_Gatherv for contiguous case
        VI recvcounts(size);
        VI displs(size);
        const int rows_per_process = n / size;
        for (int i = 0; i < size; ++i) {
            int i_start = i * rows_per_process;
            int i_end = (i == size - 1) ? n : (i + 1) * rows_per_process;
            recvcounts[i] = i_end - i_start;
            displs[i] = i_start;
        }
        MPI_Gatherv(y_local.data(), y_local.size(), MPI_DOUBLE, y.data(), recvcounts.data(), 
                    displs.data(), MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    } else if (strategy == "cyclic") {
        // For cyclic, gather results manually
        if (rank == MASTER) {
            // Initialize result vector
            y.assign(n, 0.0);
            // Master's own results
            for (size_t idx = 0; idx < row_indices.size(); ++idx) {
                y[row_indices[idx]] = y_local[idx];
            }
            // Receive from other processes
            for (int src = 1; src < size; ++src) {
                int num_rows;
                MPI_Recv(&num_rows, 1, MPI_INT, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                VI src_rows(num_rows);
                V src_y(num_rows);
                MPI_Recv(src_rows.data(), num_rows, MPI_INT, src, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(src_y.data(), num_rows, MPI_DOUBLE, src, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                for (int i = 0; i < num_rows; ++i) {
                    y[src_rows[i]] = src_y[i];
                }
            }
        } else {
            // Send results to master
            int num_rows = row_indices.size();
            MPI_Send(&num_rows, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
            MPI_Send(row_indices.data(), num_rows, MPI_INT, MASTER, 1, MPI_COMM_WORLD);
            MPI_Send(y_local.data(), num_rows, MPI_DOUBLE, MASTER, 2, MPI_COMM_WORLD);
        }
    }

    if (rank == MASTER) {
        double avg_time = total_time / NUM_RUNS;
        double variance = 0.0;
        for (double t : times) variance += (t - avg_time) * (t - avg_time);
        double std_dev = std::sqrt(variance / NUM_RUNS);

        string metric_name = "MPI_" + strategy;
        std::cout << metric_name << "," << n << "," << size << "," << min_time << "," << avg_time << ","
                  << std_dev << std::endl;

        bool test_passed = verify_triangular(y, n);
        // std::cerr << "test passed: " << test_passed << endl;
    }

    MPI_Finalize();
    return 0;
}
