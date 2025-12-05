#include <algorithm>
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

using ARR = std::vector<std::vector<double>>;
using V = std::vector<double>;
using A1D = std::vector<double>;  // Flattened 1D matrix

/**
 * Wrapper class to time function execution using std::chrono
 */
class Timer {
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
 * Performs serial matrix-vector multiplication: y = A * x (Original version with vector of vectors)
 *
 * @param y Output vector (result)
 * @param A Input matrix (NxN) as vector of vectors
 * @param x Input vector (Nx1)
 */
void matvec_original(V& y, ARR& A, V& x) {
    int n = A.size();
    for (int i = 0; i < n; ++i) {
        y[i] = 0.0;
        for (int j = 0; j < n; ++j) {
            y[i] += A[i][j] * x[j];
        }
    }
}

/**
 * Performs serial matrix-vector multiplication: y = A * x (Row-Major: Good)
 * Flattened 1D array with row-major access pattern
 *
 * @param y Output vector (result)
 * @param A Input matrix (NxN) as flattened 1D array
 * @param x Input vector (Nx1)
 * @param n Matrix dimension
 */
void matvec_row_major(V& y, A1D& A, V& x, int n) {
    for (int i = 0; i < n; ++i) {
        y[i] = 0.0;
        for (int j = 0; j < n; ++j) {
            y[i] += A[i * n + j] * x[j];  // Row-major: i*N + j
        }
    }
}

/**
 * Performs serial matrix-vector multiplication: y = A * x (Column-Major: Bad)
 * Flattened 1D array with column-major access pattern (inverted loops)
 *
 * @param y Output vector (result)
 * @param A Input matrix (NxN) as flattened 1D array
 * @param x Input vector (Nx1)
 * @param n Matrix dimension
 */
void matvec_col_major(V& y, A1D& A, V& x, int n) {
    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {
            if (j == 0) {
                y[i] = 0.0;
            }
            y[i] += A[i * n + j] * x[j];  // Column-major: inverted loops
        }
    }
}

/**
 * Initialize flattened 1D matrix from 2D matrix
 */
void flatten_matrix(A1D& A_flat, ARR& A, int n) {
    A_flat.resize(n * n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A_flat[i * n + j] = A[i][j];
        }
    }
}

/**
 * Main execution entry point.
 * Handles benchmarking and data reporting.
 */
int main(int argc, char* argv[]) {
    int n = MATRIX_SIZE;  // Default from constants.h
    string version = "row_major";  // Default: row_major, options: original, row_major, col_major

    // Parse command line arguments
    if (argc >= 2) {
        n = std::stoi(argv[1]);
    }
    if (argc >= 3) {
        version = string(argv[2]);
    }

    // Initialize matrix A (NxN) and vector x (Nx1)
    ARR A(n, std::vector<double>(n));
    V x(n);
    V y(n, 0.0);

    init_matrix(A, x);

    // Create flattened version for row/col major
    A1D A_flat;
    flatten_matrix(A_flat, A, n);

    Timer timer;

    // Warm-up phase
    const int NUM_WARMUP = 2;
    for (int i = 0; i < NUM_WARMUP; ++i) {
        if (version == "original") {
            matvec_original(y, A, x);
        } else if (version == "row_major") {
            matvec_row_major(y, A_flat, x, n);
        } else if (version == "col_major") {
            matvec_col_major(y, A_flat, x, n);
        }
    }

    double min_duration = std::numeric_limits<double>::max();
    double total_duration = 0.0;
    std::vector<double> durations;
    const int NUM_RUNS = 10;

    for (int i = 0; i < NUM_RUNS; ++i) {
        if (version == "original") {
            timer.run(matvec_original, y, A, x);
        } else if (version == "row_major") {
            timer.run(matvec_row_major, y, A_flat, x, n);
        } else if (version == "col_major") {
            timer.run(matvec_col_major, y, A_flat, x, n);
        }
        double duration = timer.get_duration_ms();
        durations.push_back(duration);
        if (duration < min_duration) {
            min_duration = duration;
        }
        total_duration += duration;
    }

    double avg_duration = total_duration / NUM_RUNS;

    // Calculate StdDev
    double variance = 0.0;
    for (double d : durations) {
        variance += (d - avg_duration) * (d - avg_duration);
    }
    double std_dev = std::sqrt(variance / NUM_RUNS);

    // CSV Output for benchmarking
    // Format: Metric, N, Threads, Min, Avg, StdDev
    string metric_name = "Serial_" + version;
    std::cout << metric_name << "," << n << ",1," << min_duration << "," << avg_duration << "," << std_dev
              << std::endl;

    bool test_passed = verify(y);
    // std::cerr << std::boolalpha << "test passed: " << test_passed << endl;
    return test_passed ? 0 : 1;
}
