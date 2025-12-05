#include "logger/Logger.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

// Инициализация статической переменной
Logger* Logger::instance = nullptr;

// Приватный конструктор
Logger::Logger() : logFile("") {}

// Получение экземпляра синглтона
Logger& Logger::getInstance() {
    if (!instance) {
        instance = new Logger();
    }
    return *instance;
}

bool Logger::initialize(const std::string& filename) {
    logFile = filename;
    
    // Открываем файл для записи (очищаем старый)
    std::ofstream file(logFile, std::ios::out);
    if (!file.is_open()) {
        std::cerr << "ERROR: Cannot open log file: " << logFile << std::endl;
        return false;
    }
    
    // Записываем заголовок
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    
    file << "=== Log started at: " << std::ctime(&time);
    file.close();
    
    std::cout << "Log file initialized: " << logFile << std::endl;
    return true;
}

void Logger::log(LogLevel level, const std::string& message, 
                 const std::string& params) {
    if (logFile.empty()) {
        std::cerr << "WARNING: Logger not initialized!" << std::endl;
        return;
    }
    
    std::ofstream file(logFile, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "ERROR: Cannot write to log file: " << logFile << std::endl;
        return;
    }
    
    // Время
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    char timeStr[100];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
    
    // Уровень логирования
    std::string levelStr;
    switch (level) {
        case LogLevel::INFO:     levelStr = "INFO"; break;
        case LogLevel::WARNING:  levelStr = "WARNING"; break;
        case LogLevel::ERROR:    levelStr = "ERROR"; break;
        case LogLevel::CRITICAL: levelStr = "CRITICAL"; break;
        default:                 levelStr = "UNKNOWN"; break;
    }
    
    // Запись
    file << "[" << timeStr << "] [" << levelStr << "] " << message;
    if (!params.empty()) {
        file << " {" << params << "}";
    }
    file << std::endl;
    
    file.close();
}

void Logger::reset() {
    if (!logFile.empty()) {
        std::remove(logFile.c_str());
    }
    logFile.clear();
}
