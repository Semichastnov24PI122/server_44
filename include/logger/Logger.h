#ifndef LOGGER_H
#define LOGGER_H

#include <string>

enum class LogLevel {
    INFO,
    ERROR,
    CRITICAL
};

class Logger {
private:
    static Logger* instance;
    std::string logFile;
    
    Logger();
    
public:
    static Logger& getInstance();
    
    bool initialize(const std::string& filename);
    void log(LogLevel level, const std::string& message, 
             const std::string& params = "");
    void reset(); // Для тестов
    
    // Запрет копирования
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

#endif
