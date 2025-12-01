#include "logger.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

static FILE* log_file = NULL;

void init_logger(const char* filename) {
    if (log_file) {
        fclose(log_file);
    }
    log_file = fopen(filename, "a");
    if (!log_file) {
        fprintf(stderr, "Warning: Cannot open log file %s\n", filename);
    }
}

void log_message(LogLevel level, const char* message, const char* params) {
    if (!log_file) return;
    
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    
    const char* level_str;
    switch (level) {
        case LOG_INFO: level_str = "INFO"; break;
        case LOG_WARNING: level_str = "WARNING"; break;
        case LOG_ERROR: level_str = "ERROR"; break;
        case LOG_CRITICAL: level_str = "CRITICAL"; break;
        default: level_str = "UNKNOWN"; break;
    }
    
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    if (params && strlen(params) > 0) {
        fprintf(log_file, "[%s] %s: %s - %s\n", timestamp, level_str, message, params);
    } else {
        fprintf(log_file, "[%s] %s: %s\n", timestamp, level_str, message);
    }
    fflush(log_file);
}

void close_logger() {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

