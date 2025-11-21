#pragma once
#include "computation.h"
#include "logger.h"
#include <memory>
#include <vector>

class ExperimentRunner {
public:
    explicit ExperimentRunner(Logger& logger);
    void add_test(std::unique_ptr<Computation> test);
    void run_all();

private:
    Logger& logger_;
    std::vector<std::unique_ptr<Computation>> tests_;
};
