#ifndef LOGGER_H
#define LOGGER_H

#include <string>

enum class LogLevel {
    INFO,
    WARNING,    // Добавляем WARNING
    ERROR,
    CRITICAL
};

class Logger {
private:
    static Logger* instance;  // Добавляем static declaration
    std::string logFile;      // Добавляем переменную
    
    Logger();                  // Приватный конструктор
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
public:
    static Logger& getInstance();
    
    bool initialize(const std::string& filename);
    void log(LogLevel level, const std::string& message, 
             const std::string& params = "");
    void reset();
    
    ~Logger() = default;
};

#endif
