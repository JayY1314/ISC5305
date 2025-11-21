#include "experiment_runner.h"
#include "computation.h"
#include "logger.h"
#include "matrix.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int main() {
    try {
        Logger logger("experiment_log.csv");
        ExperimentRunner runner(logger);

        // Keep old tests
        runner.add_test(std::make_unique<IntegrationTest>("Integration A"));
        runner.add_test(std::make_unique<MatrixTest>("Matrix B"));
        runner.add_test(std::make_unique<IntegrationTest>("Integration C"));

        // --- New task section ---
        std::vector<int> seeds(15);
        std::iota(seeds.begin(), seeds.end(), 0); // 0..14
        std::shuffle(seeds.begin(), seeds.end(), std::mt19937(std::random_device{}()));

        for (int i = 0; i < 15; ++i) {
            int mod = i % 3;
            try {
                if (mod == 0) task1(seeds[i]);
                else if (mod == 1) task2(seeds[i]);
                else task3(seeds[i]);
                logger.log_success("main", "task" + std::to_string(mod + 1), "completed");
            } catch (const std::exception& ex) {
                logger.log_failure("main", "task" + std::to_string(mod + 1),
                                   "exception caught", ex);
            }
        }

        runner.run_all();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error in main(): " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
