#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

class Logger {
private:
    std::ofstream log_file;
    std::string log_file_path;

public:
    bool openLog(const std::string& file_path);
    void logError(bool critical, const std::string& message);
    void closeLog();
};

#endif
