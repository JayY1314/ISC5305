#include "experiment_runner.h"
#include <iostream>

ExperimentRunner::ExperimentRunner(Logger& logger) : logger_(logger) {}

void ExperimentRunner::add_test(std::unique_ptr<Computation> test) {
    tests_.push_back(std::move(test));
}

void ExperimentRunner::run_all() {
    for (auto& test : tests_) {
        try {
            test->run();
            logger_.log_success("ExperimentRunner::run_all", test->name(), "Completed successfully");
        } catch (const std::exception& ex) {
            logger_.log_failure("ExperimentRunner::run_all", test->name(),
                                "Exception caught during run_all()", ex);
        }
    }
}
