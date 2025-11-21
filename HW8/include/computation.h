
#pragma once
#include "matrix.h"
#include <string>
#include <stdexcept>
#include <random>
#include <thread>
#include <chrono>

class Computation {
protected:
    std::string name_;

public:
    explicit Computation(std::string name) : name_(std::move(name)) {}
    virtual ~Computation() = default;
    virtual void run() = 0;
    std::string name() const { return name_; }
};

class ConvergenceError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class IntegrationTest : public Computation {
public:
    explicit IntegrationTest(const std::string& name);
    void run() override;
};

class MatrixTest : public Computation {
public:
    explicit MatrixTest(const std::string& name);
    void run() override;
};
