/**
 * @file Interface.cpp
 * @brief Реализация парсера командной строки.
 *
 * Разбор аргументов с использованием getopt_long.
 * Валидация портов, проверка существования файлов.
 */
#include "utils/Interface.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>

// Версия программы
#define PROGRAM_VERSION "1.0.0"
#define PROGRAM_NAME "vcalc_server"

// Структура для длинных опций getopt_long
static struct option long_options[] = {
    {"help",    no_argument,       0, 'h'},
    {"version", no_argument,       0, 'v'},
    {"config",  required_argument, 0, 'c'},
    {"log",     required_argument, 0, 'l'},
    {"port",    required_argument, 0, 'p'},
    {0, 0, 0, 0}  // Конец массива
};

// Короткие опции для getopt
const char* short_options = "hvc:l:p:";

Interface::Interface() : helpRequested(false), versionRequested(false) {}

bool Interface::parse(int argc, char* argv[]) {
    // Сброс предыдущих ошибок
    parseErrors.clear();
    
    // Разбор опций
    parseOptions(argc, argv);
    
    // Если запрошена справка или версия
    if (helpRequested) {
        printHelp();
        return false;
    }
    
    if (versionRequested) {
        printVersion();
        return false;
    }
    
    // Валидация параметров
    if (!validateParams()) {
        // Вывод ошибок
        for (const auto& error : parseErrors) {
            std::cerr << "Ошибка: " << error << std::endl;
        }
        std::cerr << "\nИспользуйте '" << PROGRAM_NAME << " --help' для справки.\n";
        return false;
    }
    
    return true;
}

void Interface::parseOptions(int argc, char* argv[]) {
    int option_index = 0;
    int c;
    
    // Сброс состояния getopt для повторного использования
    optind = 1;
    
    while ((c = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (c) {
            case 'h':
                helpRequested = true;
                break;
                
            case 'v':
                versionRequested = true;
                break;
                
            case 'c':
                params.dbFile = optarg;
                break;
                
            case 'l':
                params.logFile = optarg;
                break;
                
            case 'p':
                try {
                    params.port = std::stoi(optarg);
                } catch (const std::exception& e) {
                    parseErrors.push_back("Неверный формат порта: '" + std::string(optarg) + "'");
                }
                break;
                
            case '?':
                // Неизвестная опция - getopt уже вывел сообщение об ошибке
                parseErrors.push_back("Неизвестная опция или отсутствует значение");
                break;
                
            default:
                break;
        }
    }
    
    // Проверка на лишние аргументы
    if (optind < argc) {
        parseErrors.push_back("Неизвестные аргументы: " + std::string(argv[optind]));
    }
}

bool Interface::validateParams() {
    bool isValid = true;
    
    // Проверка обязательных параметров
    if (params.dbFile.empty()) {
        parseErrors.push_back("Не указан файл базы данных (--config)");
        isValid = false;
    }
    
    if (params.logFile.empty()) {
        parseErrors.push_back("Не указан файл журнала (--log)");
        isValid = false;
    }
    
    // Валидация порта
    if (!validatePort(params.port)) {
        parseErrors.push_back("Порт должен быть в диапазоне 1024-65535");
        isValid = false;
    }
    
    // Проверка существования файла конфигурации (предупреждение)
    if (!checkFileExists(params.dbFile)) {
        std::cerr << "Предупреждение: файл базы данных '" << params.dbFile 
                  << "' не существует. Он будет создан при необходимости.\n";
    }
    
    // Проверка возможности записи в лог-файл
    if (!checkFileWritable(params.logFile)) {
        parseErrors.push_back("Невозможно записать в файл журнала: '" + params.logFile + "'");
        isValid = false;
    }
    
    return isValid;
}

bool Interface::validatePort(int port) const {
    return (port >= 1024 && port <= 65535);
}

bool Interface::checkFileExists(const std::string& filename) const {
    std::ifstream file(filename);
    return file.good();
}

bool Interface::checkFileWritable(const std::string& filename) const {
    // Если файл существует, проверяем права на запись
    if (checkFileExists(filename)) {
        std::ofstream file(filename, std::ios::app);
        return file.is_open();
    }
    
    // Если файл не существует, проверяем можем ли создать его в этой директории
    // Проверяем существование директории и права на запись
    size_t pos = filename.find_last_of("/\\");
    if (pos != std::string::npos) {
        std::string dir = filename.substr(0, pos);
        if (dir.empty()) dir = ".";
        
        // Простая проверка - пытаемся создать временный файл
        std::string testFile = dir + "/.vcalc_test_" + std::to_string(getpid());
        std::ofstream test(testFile);
        if (test.is_open()) {
            test.close();
            remove(testFile.c_str());
            return true;
        }
        return false;
    }
    
    // Файл в текущей директории
    return true;
}

Params Interface::getParams() const {
    return params;
}

bool Interface::isHelpRequested() const {
    return helpRequested;
}

bool Interface::isVersionRequested() const {
    return versionRequested;
}

std::vector<std::string> Interface::getParseErrors() const {
    return parseErrors;
}

void Interface::printHelp() const {
    std::cout << "Использование: " << PROGRAM_NAME << " [ОПЦИИ]\n\n"
              << "Обязательные опции:\n"
              << "  -c, --config ФАЙЛ    Файл базы данных пользователей\n"
              << "  -l, --log ФАЙЛ       Файл журнала для записи ошибок\n"
              << "  -p, --port ПОРТ      Порт сервера (1024-65535)\n\n"
              << "Дополнительные опции:\n"
              << "  -h, --help           Показать эту справку\n"
              << "  -v, --version        Показать информацию о версии\n\n"
              << "Значения по умолчанию:\n"
              << "  --config /etc/vcalc.conf\n"
              << "  --log    /var/log/vcalc.log\n"
              << "  --port   33333\n\n"
              << "Примеры:\n"
              << "  " << PROGRAM_NAME << " -c /etc/my.conf -l /var/log/my.log -p 30000\n"
              << "  " << PROGRAM_NAME << " --config ~/server.conf --log /tmp/server.log --port 33333\n"
              << "  " << PROGRAM_NAME << " --help\n";
}

void Interface::printVersion() const {
    std::cout << PROGRAM_NAME << " версия " << PROGRAM_VERSION << "\n"
              << "Сервер для векторных вычислений\n"
              << "Автор: Семичастнов Н.П., группа 24ПИ1\n"
              << "Пензенский государственный университет, 2025\n";
}
