#include "Logger/Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>

bool Logger::openLog(const std::string& file_path) {
    log_file.open(file_path, std::ios::app);
    log_file_path = file_path;
    return log_file.is_open();
}

void Logger::logError(bool critical, const std::string& message) {
    if (!log_file.is_open()) return;
    
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    log_file << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    log_file << " [" << (critical ? "CRITICAL" : "WARNING") << "] ";
    log_file << message << std::endl;
    
    // Для критических ошибок также выводим в консоль
    if (critical) {
        std::cerr << "CRITICAL ERROR: " << message << std::endl;
    }
}

void Logger::closeLog() {
    if (log_file.is_open()) {
        log_file.close();
    }
}
