#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    virtual ~Logger() = default;
    
    virtual bool open_log(const std::string& file_path) { return true; }
    
    // Убираем ... для упрощения
    virtual void log_error(bool critical, const std::string& message) {}
    
    virtual void close_log() {}
};

#endif
