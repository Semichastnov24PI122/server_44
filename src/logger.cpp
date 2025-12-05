#include "logger/Logger.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>

Logger* Logger::instance = nullptr;

Logger::Logger() : logFile("") {}

Logger& Logger::getInstance() {
    if (!instance) {
        instance = new Logger();
    }
    return *instance;
}

bool Logger::initialize(const std::string& filename) {
    logFile = filename;
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        return false;
    }
    file.close();
    return true;
}

void Logger::log(LogLevel level, const std::string& message, 
                 const std::string& params) {
    std::ofstream file(logFile, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Cannot open log file: " << logFile << std::endl;
        return;
    }
    
    // Текущее время
    std::time_t now = std::time(nullptr);
    std::tm* tm = std::localtime(&now);
    file << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    
    // Уровень
    switch (level) {
        case LogLevel::INFO:
            file << " [INFO] ";
            break;
        case LogLevel::ERROR:
            file << " [ERROR] ";
            break;
        case LogLevel::CRITICAL:
            file << " [CRITICAL] ";
            break;
    }
    
    // Сообщение и параметры
    file << message;
    if (!params.empty()) {
        file << " (" << params << ")";
    }
    file << std::endl;
    
    file.close();
}

void Logger::reset() {
    if (!logFile.empty()) {
        std::ofstream file(logFile, std::ios::trunc);
        file.close();
    }
}
