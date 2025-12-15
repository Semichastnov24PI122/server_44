#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <vector>

// Структура для хранения параметров командной строки
struct Params {
    std::string dbFile;      // Файл базы данных пользователей
    std::string logFile;     // Файл журнала
    int port;                // Порт сервера
    
    Params() : dbFile("/etc/vcalc.conf"), logFile("/var/log/vcalc.log"), port(33333) {}
};

// Класс для разбора и валидации параметров командной строки
class Interface {
private:
    Params params;
    bool helpRequested;
    bool versionRequested;
    std::vector<std::string> parseErrors;
    
    // Парсинг опций командной строки с использованием getopt_long
    void parseOptions(int argc, char* argv[]);
    
    // Валидация разобранных параметров
    bool validateParams();
    
    // Проверка валидности номера порта
    bool validatePort(int port) const;
    
    // Проверка существования файла
    bool checkFileExists(const std::string& filename) const;
    
    // Проверка возможности записи в файл
    bool checkFileWritable(const std::string& filename) const;

public:
    // Конструктор по умолчанию
    Interface();
    
    // Разбор аргументов командной строки
    bool parse(int argc, char* argv[]);
    
    // Получение разобранных параметров
    Params getParams() const;
    
    // Проверка запроса справки
    bool isHelpRequested() const;
    
    // Проверка запроса версии
    bool isVersionRequested() const;
    
    // Получение ошибок парсинга
    std::vector<std::string> getParseErrors() const;
    
    // Вывод справки по использованию программы
    void printHelp() const;
    
    // Вывод информации о версии программы
    void printVersion() const;
};

#endif // INTERFACE_H
