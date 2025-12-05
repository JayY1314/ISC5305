#include "utils.h"

#include <cmath>
#include <iostream>

/**
 * Initializes matrix A and vector x with 1.0s.
 *
 * @param A Reference to the matrix (vector of vectors)
 * @param x Reference to the vector
 */
void init_matrix(ARR& A, V& x) {
    int n = A.size();
    // Initialize matrix A with all 1s for simple computation
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i][j] = 1.0;
        }
    }

    // Initialize vector x with all 1s
    for (int i = 0; i < n; ++i) {
        x[i] = 1.0;
    }
}

/**
 * Initializes flattened 1D matrix A and vector x with 1.0s.
 *
 * @param A Reference to the flattened 1D matrix (size n*n)
 * @param x Reference to the vector
 * @param n Matrix dimension
 */
void init_matrix_1d(A1D& A, V& x, int n) {
    A.resize(n * n);
    // Initialize matrix A with all 1s
    for (int i = 0; i < n * n; ++i) {
        A[i] = 1.0;
    }

    // Initialize vector x with all 1s
    x.resize(n);
    for (int i = 0; i < n; ++i) {
        x[i] = 1.0;
    }
}

/**
 * Generates column limits for each row to create a "Triangular" workload.
 * k_i = i (Row 0 does 0 ops, Row N-1 does N-1 ops).
 *
 * @param N The matrix size (number of rows)
 * @return std::vector<int> A vector where index i contains the column limit for row i
 */
std::vector<int> init_triangular_limits(int N) {
    std::vector<int> limits(N);
    for (int i = 0; i < N; ++i) {
        limits[i] = i;
    }
    return limits;
}

/**
 * Verifies the result of the matrix-vector multiplication.
 * Assumes A and x are initialized with 1.0s.
 *
 * @param y The result vector to check
 * @return true if all elements are close to N, false otherwise
 */
bool verify(V& y) {
    int n = y.size();
    // Clear test: With A[i][j] = 1 and x[j] = 1 for all i,j,
    // each y[i] should equal N
    const double expected_value = static_cast<double>(n);
    bool test_passed = true;
    int error_count = 0;

    // std::cout << "Matrix-Vector Multiplication Test:\n";
    // std::cout << "Expected result: y[i] = " << expected_value << " for all i\n";

    // Verify all elements
    for (int i = 0; i < n; ++i) {
        if (std::abs(y[i] - expected_value) > 1e-9) {  // Float comparison
            if (error_count < 5) {                     // Show first 5 errors
                std::cerr << "ERROR: y[" << i << "] = " << y[i] << ", expected " << expected_value
                          << "\n";
            }
            test_passed = false;
            error_count++;
        }
    }

    if (!test_passed) {
        std::cerr << "✗ TEST FAILED: " << error_count << " out of " << n
                  << " elements are incorrect\n";
    } else {
        // std::cerr << "✓ TEST PASSED\n";
    }

    return test_passed;
}

/**
 * Verifies the result of triangular matrix-vector multiplication.
 * For triangular workload: row i processes columns 0 to i-1 (k_i = i)
 * Expected: y[i] = i (since A[i][j] = 1, x[j] = 1, and we sum i elements)
 *
 * @param y The result vector to check
 * @param n Matrix dimension
 * @return true if all elements are correct, false otherwise
 */
bool verify_triangular(V& y, int n) {
    bool test_passed = true;
    int error_count = 0;

    for (int i = 0; i < n; ++i) {
        double expected_value = static_cast<double>(i);  // Row i sums i elements
        if (std::abs(y[i] - expected_value) > 1e-9) {
            if (error_count < 5) {
                std::cerr << "ERROR: y[" << i << "] = " << y[i] << ", expected " << expected_value
                          << "\n";
            }
            test_passed = false;
            error_count++;
        }
    }

    if (!test_passed) {
        std::cerr << "✗ TRIANGULAR TEST FAILED: " << error_count << " out of " << n
                  << " elements are incorrect\n";
    }

    return test_passed;
}
