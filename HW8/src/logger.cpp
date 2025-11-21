#include "logger.h"
#include <ctime>
#include <stdexcept>

// Logger class implementations
Logger::Logger(const std::string& filename) {
    file_.open(filename);
    if (!file_.is_open()) {
        throw std::runtime_error("Failed to open log file: " + filename);
    }
    // Write CSV header
    file_ << "timestamp,function_name,test_name,status,message,exception\n";
    file_.flush();
}

Logger::~Logger() {
    if (file_.is_open()) {
        file_.close();
    }
}

void Logger::log_success(const std::string& function_name,
                         const std::string& test_name,
                         const std::string& message) {
    file_ << current_time() << ","
          << function_name << ","
          << test_name << ","
          << "SUCCESS" << ","
          << "\"" << message << "\"" << ","
          << "\n";
    file_.flush();
}

void Logger::log_failure(const std::string& function_name,
                         const std::string& test_name,
                         const std::string& message,
                         const std::exception& ex) {
    file_ << current_time() << ","
          << function_name << ","
          << test_name << ","
          << "FAILURE" << ","
          << "\"" << message << "\"" << ","
          << "\"" << ex.what() << "\"" << "\n";
    file_.flush();
}

std::string Logger::current_time() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}
