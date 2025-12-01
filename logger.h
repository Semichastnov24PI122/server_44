#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
    LOG_INFO,
    LOG_WARNING, 
    LOG_ERROR,
    LOG_CRITICAL
} LogLevel;

void init_logger(const char* log_file);
void log_message(LogLevel level, const char* message, const char* params);
void close_logger();

#endif
