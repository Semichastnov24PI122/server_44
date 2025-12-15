/**
 * @file main.cpp
 * @brief Главная функция сервера.
 *
 * Точка входа в программу. Парсит аргументы командной строки,
 * инициализирует и запускает сервер. Обрабатывает исключения.
 */
#include <iostream>
#include "utils/Interface.h"
#include "logger/Logger.h"
#include "auth/AuthManager.h"
#include "network/Server.h"  

int main(int argc, char* argv[]) {
    // Инициализация интерфейса командной строки
    Interface interface;
    
    // Парсинг аргументов
    if (!interface.parse(argc, argv)) {
        // Пользователь запросил справку или произошла ошибка
        return interface.isHelpRequested() || interface.isVersionRequested() ? 0 : 1;
    }
    
    // Получение параметров
    Params params = interface.getParams();
    
    try {
        // Инициализация логгера
        Logger& logger = Logger::getInstance();
        if (!logger.initialize(params.logFile)) {
            std::cerr << "Ошибка: не удалось инициализировать логгер" << std::endl;
            return 1;
        }
        
        // Загрузка базы пользователей
        AuthManager authManager;
        if (!authManager.loadUserDatabase(params.dbFile)) {
            // Создаем строку с параметрами
            std::string errorParams = "file=" + params.dbFile;
            logger.log(LogLevel::ERROR, "Не удалось загрузить базу пользователей", errorParams);
            
            std::cerr << "Ошибка: не удалось загрузить базу пользователей из файла '" 
                      << params.dbFile << "'" << std::endl;
            return 1;
        }
        
        // Создание и запуск сервера
        Server server(params.port, authManager, logger);
        
        // Создаем строку с параметрами для лога
        std::string infoParams = "port=" + std::to_string(params.port) + 
                                 ",config=" + params.dbFile + 
                                 ",log=" + params.logFile;
        logger.log(LogLevel::INFO, "Сервер запускается", infoParams);
        
        std::cout << "Сервер запущен на порту " << params.port << std::endl;
        std::cout << "База пользователей: " << params.dbFile << std::endl;
        std::cout << "Файл журнала: " << params.logFile << std::endl;
        std::cout << "Для остановки нажмите Ctrl+C\n";
        
        // Запуск основного цикла сервера
        server.run();
        
        logger.log(LogLevel::INFO, "Сервер остановлен", "");
        
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
