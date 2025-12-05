#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>

struct Params {
    std::string dbFile;
    std::string logFile;
    int port;
};

class Interface {
private:
    Params params;
    bool helpRequested;
    
public:
    Interface();
    bool parse(int argc, char* argv[]);
    Params getParams() const;
    bool isHelpRequested() const;
    void printHelp() const;
};

#endif
