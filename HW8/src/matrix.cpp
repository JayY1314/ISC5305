#include "matrix.h"
#include <random>
#include <iostream>

/**
 * These tasks are examples. The exception-throwing behavior should be
 * completed by students. Randomness ensures different outcomes.
 */

static std::mt19937 gen(std::random_device{}());
static std::uniform_real_distribution<> dist(-2.0, 2.0);

// ---------------------------
// TASK 1: Simple computation
// ---------------------------
void task1(int seed) {
    gen.seed(seed);
    double x = dist(gen);
    if (x < -1.0) {
        throw std::runtime_error("task1(): random input < -1.0, value = " + std::to_string(x));
    }
    std::cout << "task1(): success, x = " << x << "\n";
}

// ---------------------------
// TASK 2: Two-level nesting
// ---------------------------
void task2(int seed) {
    try {
        gen.seed(seed);
        double val = dist(gen) + seed * 0.01;
        task2_help(val);
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("task2(): failure -> ") + e.what());
    }
}

void task2_help(double value) {
    if (value > 1.5) {
        throw std::overflow_error("task2_help(): value too large, value = " + std::to_string(value));
    }
    std::cout << "task2_help(): value = " << value << "\n";
}

// ---------------------------
// TASK 3: Three-level nesting
// ---------------------------
void task3(int seed) {
    gen.seed(seed);
    double a = dist(gen);
    double b = dist(gen);
    try {
        task3_help(a, b);
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("task3(): caught in outer level -> ") + e.what());
    }
}

void task3_help(double v1, double v2) {
    DiagonalMatrix M(v1, v2);
    try {
        task3_innermost(M);
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("task3_help(): intermediate -> ") + e.what());
    }
}

void task3_innermost(const Matrix& M) {
    M.validate();
    sqrt_det(M);
    invert_matrix(M);
    std::cout << "task3_innermost(): det = " << M.determinant() << "\n";
}

