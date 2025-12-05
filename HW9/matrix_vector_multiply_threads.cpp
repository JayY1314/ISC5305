#include <algorithm>
#include <atomic>
#include <cassert>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include <thread>
#include <vector>

#include "constants.h"
#include "utils.h"
using namespace std;

using V = vector<double>;
using A1D = vector<double>;  // Flattened 1D matrix

/**
 * Wrapper class to time function execution
 */
class ThreadTimer {
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
 * Computes triangular dot product for a single row.
 * Triangular workload: row i only processes columns 0 to k_i, where k_i = i
 *
 * @param A Input matrix (flattened 1D, row-major)
 * @param x Input vector
 * @param y Output vector (modified in place)
 * @param row Row index to compute
 * @param n Matrix dimension size
 */
void compute_row_triangular(const A1D& A, const V& x, V& y, int row, int n) {
    y[row] = 0.0;
    int k_i = row;  // Column limit for row i
    for (int j = 0; j <= k_i; ++j) {  // Triangular: only process columns 0 to k_i
        y[row] += A[row * n + j] * x[j];
    }
}

/**
 * Orchestrates parallel triangular matrix-vector multiplication using std::thread
 * with atomic work sharing.
 *
 * @param y Output vector
 * @param A Input matrix (flattened 1D)
 * @param x Input vector
 * @param n Matrix dimension
 * @param num_threads Number of threads to launch
 */
void matvec_triangular_atomic(V& y, A1D& A, V& x, int n, int num_threads) {
    std::atomic<int> next_row(0);
    std::vector<std::thread> threads;

    // Launch worker threads
    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([&, n]() {
            while (true) {
                // Atomically get next row to process
                int row = next_row.fetch_add(1);
                if (row >= n) {
                    break;  // All rows processed
                }
                compute_row_triangular(std::cref(A), std::cref(x), std::ref(y), row, n);
            }
        });
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
}

// Wrapper class to time function execution (for wall time)
class WallTimer {
public:
    template <typename Func, typename... Args>
    void run(Func&& func, Args&&... args) {
        auto start = std::chrono::high_resolution_clock::now();
        func(std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        duration_ms_ = std::chrono::duration<double, std::milli>(end - start).count();
    }

    double get_duration_ms() const {
        return duration_ms_;
    }

private:
    double duration_ms_ = 0.0;
};

/**
 * Main execution entry point.
 */
int main(int argc, char* argv[]) {
    int n = MATRIX_SIZE;
    int num_threads = 4;  // Default

    // Parse command line arguments: ./threads_code [N] [Num_Threads]
    if (argc >= 2) {
        n = std::stoi(argv[1]);
    }
    if (argc >= 3) {
        num_threads = std::stoi(argv[2]);
    }

    // Initialize flattened 1D matrix A and vector x
    A1D A;
    V x;
    V y(n, 0.0);

    init_matrix_1d(A, x, n);

    WallTimer wall_timer;

    // Warm-up phase
    const int NUM_WARMUP = 2;
    for (int i = 0; i < NUM_WARMUP; ++i) {
        matvec_triangular_atomic(y, A, x, n, num_threads);
    }

    double min_duration = std::numeric_limits<double>::max();
    double total_duration = 0.0;
    std::vector<double> durations;
    const int NUM_RUNS = 10;

    for (int i = 0; i < NUM_RUNS; ++i) {
        wall_timer.run(matvec_triangular_atomic, y, A, x, n, num_threads);
        double duration = wall_timer.get_duration_ms();
        durations.push_back(duration);
        if (duration < min_duration) {
            min_duration = duration;
        }
        total_duration += duration;
    }

    double avg_duration = total_duration / NUM_RUNS;
    double variance = 0.0;
    for (double d : durations) variance += (d - avg_duration) * (d - avg_duration);
    double std_dev = std::sqrt(variance / NUM_RUNS);

    std::cout << "Threads," << n << "," << num_threads << "," << min_duration << "," << avg_duration
              << "," << std_dev << std::endl;

    bool test_passed = verify_triangular(y, n);
    // cout << std::boolalpha << "test passed: " << test_passed << endl;
    return test_passed ? 0 : 1;
}
