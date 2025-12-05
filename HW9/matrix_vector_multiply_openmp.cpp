#ifdef OMP
#include <omp.h>
#endif
#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "constants.h"
#include "utils.h"
using namespace std;

using V = std::vector<double>;
using A1D = std::vector<double>;  // Flattened 1D matrix

/**
 * Performs parallel triangular matrix-vector multiplication using OpenMP.
 * Triangular workload: row i only processes columns 0 to k_i, where k_i = i
 *
 * @param y Output vector
 * @param A Input matrix (flattened 1D, row-major)
 * @param x Input vector
 * @param n Matrix dimension
 * @param num_threads Number of threads to request
 * @param schedule_type Scheduling type: "static", "dynamic", or "guided"
 * @param chunk_size Chunk size for dynamic scheduling (ignored for static/guided)
 */
void matvec_triangular(V& y, A1D& A, V& x, int n, int num_threads, 
                       const string& schedule_type, int chunk_size = 1) {
#ifdef OMP
    omp_set_num_threads(num_threads);
#endif

    if (schedule_type == "static") {
#pragma omp parallel for schedule(static)
        for (int i = 0; i < n; ++i) {
            y[i] = 0.0;
            int k_i = i;  // Column limit for row i
            for (int j = 0; j <= k_i; ++j) {  // Triangular: only process columns 0 to k_i
                y[i] += A[i * n + j] * x[j];
            }
        }
    } else if (schedule_type == "dynamic") {
#pragma omp parallel for schedule(dynamic, chunk_size)
        for (int i = 0; i < n; ++i) {
            y[i] = 0.0;
            int k_i = i;  // Column limit for row i
            for (int j = 0; j <= k_i; ++j) {  // Triangular: only process columns 0 to k_i
                y[i] += A[i * n + j] * x[j];
            }
        }
    } else if (schedule_type == "guided") {
#pragma omp parallel for schedule(guided)
        for (int i = 0; i < n; ++i) {
            y[i] = 0.0;
            int k_i = i;  // Column limit for row i
            for (int j = 0; j <= k_i; ++j) {  // Triangular: only process columns 0 to k_i
                y[i] += A[i * n + j] * x[j];
            }
        }
    }
}

/**
 * Main execution entry point.
 */
int main(int argc, char* argv[]) {
    int n = MATRIX_SIZE;
    int num_threads = 4;
    string schedule_type = "static";  // Default: static
    int chunk_size = 1;

    // Parse command line arguments: ./omp_code [N] [Num_Threads] [schedule] [chunk_size]
    if (argc >= 2) {
        n = std::stoi(argv[1]);
    }
    if (argc >= 3) {
        num_threads = std::stoi(argv[2]);
    }
    if (argc >= 4) {
        schedule_type = string(argv[3]);
    }
    if (argc >= 5) {
        chunk_size = std::stoi(argv[4]);
    }

    // Initialize flattened 1D matrix A and vector x
    A1D A;
    V x;
    V y(n, 0.0);

    init_matrix_1d(A, x, n);

    // Warmup
    const int NUM_WARMUP = 2;
    for (int i = 0; i < NUM_WARMUP; ++i) {
        matvec_triangular(y, A, x, n, num_threads, schedule_type, chunk_size);
    }

    // Timing
    double min_time = std::numeric_limits<double>::max();
    double total_time = 0.0;
    std::vector<double> times;
    const int NUM_RUNS = 10;

    for (int i = 0; i < NUM_RUNS; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        matvec_triangular(y, A, x, n, num_threads, schedule_type, chunk_size);
        auto end = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double, std::milli>(end - start).count();

        times.push_back(duration);
        if (duration < min_time) min_time = duration;
        total_time += duration;
    }

    double avg_time = total_time / NUM_RUNS;
    double variance = 0.0;
    for (double t : times) variance += (t - avg_time) * (t - avg_time);
    double std_dev = std::sqrt(variance / NUM_RUNS);

    string metric_name = "OpenMP_" + schedule_type;
    std::cout << metric_name << "," << n << "," << num_threads << "," << min_time << "," << avg_time << ","
              << std_dev << std::endl;

    bool test_passed = verify_triangular(y, n);
    // cout << std::boolalpha << "test passed: " << test_passed << endl;
    return test_passed ? 0 : 1;
}
