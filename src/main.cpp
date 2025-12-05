#include "auth/AuthManager.h"
#include "logger/Logger.h"
#include "utils/Interface.h"
#include "processing/VectorProcessor.h"
#include "network/Server.h"
#include <iostream>
#include <vector>
#include <csignal>
#include <atomic>

std::atomic<bool> stop_requested(false);

void signal_handler(int) {
    stop_requested = true;
}

int main(int argc, char* argv[]) {
    // Настраиваем обработчики сигналов
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    Interface interface;
    if (!interface.parse(argc, argv)) {
        if (interface.isHelpRequested()) {
            interface.printHelp();
        }
        return 1;
    }
    
    Params params = interface.getParams();
    
    // Инициализация логгера
    Logger& logger = Logger::getInstance();
    if (!logger.initialize(params.logFile)) {
        std::cerr << "Cannot open log file: " << params.logFile << std::endl;
        return 1;
    }
    
    // Загрузка базы пользователей
    AuthManager auth;
    if (!auth.loadUserDatabase(params.dbFile)) {
        logger.log(LogLevel::ERROR, "Cannot load user database", 
                  "file=" + params.dbFile);
        std::cerr << "Cannot load user database: " << params.dbFile << std::endl;
        return 1;
    }
    
    logger.log(LogLevel::INFO, "Server started", 
              "port=" + std::to_string(params.port));
    
    std::cout << "Server started on port " << params.port << std::endl;
    std::cout << "User database: " << params.dbFile << std::endl;
    std::cout << "Log file: " << params.logFile << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl;
    
    // СОЗДАЁМ И ЗАПУСКАЕМ СЕРВЕР
    Server server(params.port, auth, logger);
    server.run();
    
    std::cout << "Server stopped." << std::endl;
    return 0;
}
