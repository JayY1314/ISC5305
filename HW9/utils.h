#ifndef UTILS_H
#define UTILS_H

#include <vector>

// Type aliases for matrix and vector
using ARR = std::vector<std::vector<double>>;
using V = std::vector<double>;
using A1D = std::vector<double>;  // Flattened 1D matrix

// Function declarations
void init_matrix(ARR& A, V& x);
void init_matrix_1d(A1D& A, V& x, int n);  // Initialize 1D flattened matrix
std::vector<int> init_triangular_limits(int N);
bool verify(V& y);
bool verify_triangular(V& y, int n);  // Verify triangular workload result

#endif // UTILS_H

