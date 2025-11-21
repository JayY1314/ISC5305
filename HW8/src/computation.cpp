#include "computation.h"
#include <cmath>
#include <iostream>

IntegrationTest::IntegrationTest(const std::string& name)
    : Computation(name) {}

void IntegrationTest::run() {
    std::cout << "Running " << name_ << "...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    double tol = dist(gen);
    if (tol > 0.8)
        throw ConvergenceError("Convergence not achieved, tol=" + std::to_string(tol));
}

MatrixTest::MatrixTest(const std::string& name)
    : Computation(name) {}

void MatrixTest::run() {
    std::cout << "Running " << name_ << "...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    static std::mt19937 gen(std::random_device{}());
    std::bernoulli_distribution fail(0.3);
    if (fail(gen))
        throw SingularMatrixError("Singular matrix encountered in test " + name_);
}
