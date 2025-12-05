#include "utils/Interface.h"
#include <iostream>
#include <cstring>

Interface::Interface() : helpRequested(false) {
    // Значения по умолчанию
    params.dbFile = "/etc/vcalc.conf";
    params.logFile = "/var/log/vcalc.log";
    params.port = 33333;
}

bool Interface::parse(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 ||  strcmp(argv[i], "-h") == 0) {
            helpRequested = true;
            return false;
        }
        else if (strcmp(argv[i], "--config") == 0 ||  strcmp(argv[i], "-c") == 0) {
            if (i + 1 < argc) {
                params.dbFile = argv[++i];
            } else {
                std::cerr << "Error: Missing value for --config" << std::endl;
                return false;
            }
        }
        else if (strcmp(argv[i], "--log") == 0 ||  strcmp(argv[i], "-l") == 0) {
            if (i + 1 < argc) {
                params.logFile = argv[++i];
            } else {
                std::cerr << "Error: Missing value for --log" << std::endl;
                return false;
            }
        }
        else if (strcmp(argv[i], "--port") == 0 || strcmp(argv[i], "-p") == 0) {
            if (i + 1 < argc) {
                params.port = std::atoi(argv[++i]);
                if (params.port <= 0 || params.port > 65535) {
                    std::cerr << "Error: Invalid port number" << std::endl;
                    return false;
                }
            } else {
                std::cerr << "Error: Missing value for --port" << std::endl;
                return false;
            }
        }
        else {
            std::cerr << "Error: Unknown option: " << argv[i] << std::endl;
            return false;
        }
    }
    
    return true;
}

Params Interface::getParams() const {
    return params;
}

bool Interface::isHelpRequested() const {
    return helpRequested;
}

void Interface::printHelp() const {
    std::cout << "Usage: server [OPTIONS]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h, --help           Show this help message" << std::endl;
    std::cout << "  -c, --config FILE    User database file (default: /etc/vcalc.conf)" << std::endl;
    std::cout << "  -l, --log FILE       Log file (default: /var/log/vcalc.log)" << std::endl;
    std::cout << "  -p, --port PORT      Server port (default: 33333)" << std::endl;
}
