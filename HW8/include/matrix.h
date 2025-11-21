#pragma once
#include <cmath>
#include <stdexcept>
#include <string>

/**
 * Matrix hierarchy for testing nested exceptions and polymorphism.
 * Base class: Matrix
 * Derived classes: DiagonalMatrix, UpperTriangularMatrix
 *
 * Exception conditions (to be implemented by students):
 *  - InvalidMatrixStructureError: if diagonal or triangular constraints are violated.
 *  - SingularMatrixError: if determinant is zero during inversion.
 *  - NegativeDeterminantError: if sqrt(det) < 0.
 */

class MatrixError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class InvalidMatrixStructureError : public MatrixError {
public:
    using MatrixError::MatrixError;
};

class NegativeDeterminantError : public MatrixError {
public:
    using MatrixError::MatrixError;
};

class SingularMatrixError : public MatrixError {
public:
    using MatrixError::MatrixError;
};

class Matrix {
protected:
    double a11_, a12_, a21_, a22_;
public:
    Matrix(double a11, double a12, double a21, double a22)
        : a11_(a11), a12_(a12), a21_(a21), a22_(a22) {}
    virtual ~Matrix() = default;

    virtual double determinant() const { return a11_ * a22_ - a12_ * a21_; }
    virtual void validate() const {} // to be overridden
    virtual std::string type() const { return "Generic Matrix"; }
    
    double a11() const { return a11_; }
    double a12() const { return a12_; }
    double a21() const { return a21_; }
    double a22() const { return a22_; }
};

class DiagonalMatrix : public Matrix {
public:
    DiagonalMatrix(double a11, double a22)
        : Matrix(a11, 0.0, 0.0, a22) {}
    void validate() const override {
        if (a12_ != 0.0 || a21_ != 0.0) {
            throw InvalidMatrixStructureError("Non-zero off-diagonal in DiagonalMatrix");
        }
    }
    std::string type() const override { return "DiagonalMatrix"; }
};

class UpperTriangularMatrix : public Matrix {
public:
    UpperTriangularMatrix(double a11, double a12, double a22)
        : Matrix(a11, a12, 0.0, a22) {}
    void validate() const override {
        if (a21_ != 0.0) {
            throw InvalidMatrixStructureError("Non-zero element in lower triangle of UpperTriangularMatrix");
        }
    }
    std::string type() const override { return "UpperTriangularMatrix"; }
};

/**
 * sqrt_det(Matrix) = sqrt(det(Matrix))
 * Throws NegativeDeterminantError if determinant < 0.
 */
inline double sqrt_det(const Matrix& M) {
    double det = M.determinant();
    if (det < 0) {
        throw NegativeDeterminantError("Determinant is negative: " + std::to_string(det));
    }
    return std::sqrt(det);
}

/**
 * Invert a 2x2 matrix. Throws SingularMatrixError if determinant == 0.
 */
inline Matrix invert_matrix(const Matrix& M) {
    double det = M.determinant();
    if (std::abs(det) < 1e-12) {
        throw SingularMatrixError("Cannot invert singular matrix: determinant is " + std::to_string(det));
    }
    return Matrix(M.a22() / det, -M.a12() / det, -M.a21() / det, M.a11() / det);
}

/**
 * Task functions. Each may throw at different call depths.
 */
void task1(int seed);
void task2(int seed);
void task2_help(double value);
void task3(int seed);
void task3_help(double v1, double v2);
void task3_innermost(const Matrix& M);

