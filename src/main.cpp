#include <iostream>
#include <string>
#include <cstring>
#include "Server/Server.h"
#include "UserDatabase/UserDatabase.h"
#include "Logger/Logger.h"
#include "DataProcessor/DataProcessor.h"

void printHelp() {
    std::cout << "Usage: server -d <database_file> -l <log_file> -p <port>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -d <file>    User database file (required)" << std::endl;
    std::cout << "  -l <file>    Log file (required)" << std::endl;
    std::cout << "  -p <port>    Port number (required)" << std::endl;
    std::cout << "  -h           Show this help message" << std::endl;
}

bool parseArguments(int argc, char* argv[], std::string& db_file, std::string& log_file, int& port) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            db_file = argv[++i];
        } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            log_file = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            port = std::stoi(argv[++i]);
        } else if (strcmp(argv[i], "-h") == 0) {
            return false;
        }
    }
    
    return !db_file.empty() && !log_file.empty() && port > 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        printHelp();
        return 1;
    }
    
    std::string db_file, log_file;
    int port = 0;
    
    if (!parseArguments(argc, argv, db_file, log_file, port)) {
        printHelp();
        return 1;
    }
    
    // Инициализация компонентов
    UserDatabase user_db;
    Logger logger;
    DataProcessor data_processor;
    
    // Загрузка базы пользователей
    if (!user_db.loadFromFile(db_file)) {
        std::cerr << "Error: Cannot load user database from " << db_file << std::endl;
        return 1;
    }
    
    // Открытие лог-файла
    if (!logger.openLog(log_file)) {
        std::cerr << "Error: Cannot open log file " << log_file << std::endl;
        return 1;
    }
    
    std::cout << "Server starting..." << std::endl;
    std::cout << "Database: " << db_file << std::endl;
    std::cout << "Log file: " << log_file << std::endl;
    std::cout << "Port: " << port << std::endl;
    
    // Создание и запуск сервера
    Server server(user_db, logger, data_processor, port);
    if (!server.run()) {
        std::cerr << "Failed to start server" << std::endl;
        logger.closeLog();
        return 1;
    }
    
    logger.closeLog();
    return 0;
}
