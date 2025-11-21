
#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

class Logger {
public:
    explicit Logger(const std::string& filename);
    ~Logger();

    void log_success(const std::string& function_name,
                     const std::string& test_name,
                     const std::string& message);

    void log_failure(const std::string& function_name,
                     const std::string& test_name,
                     const std::string& message,
                     const std::exception& ex);

private:
    std::ofstream file_;
    std::string current_time() const;
};
