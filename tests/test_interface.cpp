#include <iostream>
#include <string>
#include <vector>
#include <cstring>

class ServerInterface {
private:
    std::string db_file;
    std::string log_file;
    int port;
    bool help_requested;
    
public:
    ServerInterface() : port(0), help_requested(false) {}
    
    bool parse_args(int argc, char* argv[]) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
                db_file = argv[++i];
            } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
                log_file = argv[++i];
            } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
                port = atoi(argv[++i]);
            } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                help_requested = true;
            }
        }
        
        return !db_file.empty() && !log_file.empty() && port > 0 && port <= 65535;
    }
    
    void print_help() {
        std::cout << "Использование: server -d <db_file> -l <log_file> -p <port>\n";
    }
    
    std::string get_db_file() const { return db_file; }
    std::string get_log_file() const { return log_file; }
    int get_port() const { return port; }
    bool is_help_requested() const { return help_requested; }
};

void run_interface_tests() {
    std::cout << "\n=== ТЕСТЫ ИНТЕРФЕЙСА КОМАНДНОЙ СТРОКИ (10 тестов) ===\n";
    int passed = 0;
    
    // Тест 1: Корректные аргументы
    {
        ServerInterface iface;
        char* args[] = {(char*)"server", (char*)"-d", (char*)"db.txt", 
                       (char*)"-l", (char*)"log.txt", (char*)"-p", (char*)"8080"};
        bool result = iface.parse_args(7, args);
        bool ok = result && iface.get_db_file() == "db.txt" && 
                  iface.get_log_file() == "log.txt" && iface.get_port() == 8080;
        std::cout << "1. Корректные аргументы: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 2: Недостаточно аргументов
    {
        ServerInterface iface;
        char* args[] = {(char*)"server", (char*)"-d", (char*)"db.txt"};
        bool result = iface.parse_args(3, args);
        bool ok = !result;
        std::cout << "2. Недостаточно аргументов: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 3: Неверный порт (0)
    {
        ServerInterface iface;
        char* args[] = {(char*)"server", (char*)"-d", (char*)"db.txt",
                       (char*)"-l", (char*)"log.txt", (char*)"-p", (char*)"0"};
        bool result = iface.parse_args(7, args);
        bool ok = !result;
        std::cout << "3. Неверный порт (0): " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 4: Неверный порт (>65535)
    {
        ServerInterface iface;
        char* args[] = {(char*)"server", (char*)"-d", (char*)"db.txt",
                       (char*)"-l", (char*)"log.txt", (char*)"-p", (char*)"70000"};
        bool result = iface.parse_args(7, args);
        bool ok = !result;
        std::cout << "4. Неверный порт (>65535): " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 5: Запрос справки (-h)
    {
        ServerInterface iface;
        char* args[] = {(char*)"server", (char*)"-h"};
        bool result = iface.parse_args(2, args);
        bool ok = iface.is_help_requested();
        std::cout << "5. Запрос справки (-h): " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 6: Запрос справки (--help)
    {
        ServerInterface iface;
        char* args[] = {(char*)"server", (char*)"--help"};
        bool result = iface.parse_args(2, args);
        bool ok = iface.is_help_requested();
        std::cout << "6. Запрос справки (--help): " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 7: Разный порядок аргументов
    {
        ServerInterface iface;
        char* args[] = {(char*)"server", (char*)"-p", (char*)"9090",
                       (char*)"-l", (char*)"log.txt", (char*)"-d", (char*)"db.txt"};

bool result = iface.parse_args(7, args);
        bool ok = result && iface.get_port() == 9090;
        std::cout << "7. Разный порядок аргументов: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 8: Лишние аргументы
    {
        ServerInterface iface;
        char* args[] = {(char*)"server", (char*)"-d", (char*)"db.txt",
                       (char*)"-l", (char*)"log.txt", (char*)"-p", (char*)"8080",
                       (char*)"extra", (char*)"argument"};
        bool result = iface.parse_args(9, args);
        bool ok = result; // Лишние аргументы игнорируются
        std::cout << "8. Лишние аргументы: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 9: Отсутствие обязательного аргумента
    {
        ServerInterface iface;
        char* args[] = {(char*)"server", (char*)"-d", (char*)"db.txt",
                       (char*)"-p", (char*)"8080"}; // Нет -l
        bool result = iface.parse_args(5, args);
        bool ok = !result;
        std::cout << "9. Нет обязательного аргумента (-l): " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 10: Пути с пробелами
    {
        ServerInterface iface;
        char* args[] = {(char*)"server", (char*)"-d", (char*)"/path/to/db file.txt",
                       (char*)"-l", (char*)"/path/to/log file.txt",
                       (char*)"-p", (char*)"8080"};
        bool result = iface.parse_args(7, args);
        bool ok = result && iface.get_db_file() == "/path/to/db file.txt";
        std::cout << "10. Пути с пробелами: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    std::cout << "\nПройдено: " << passed << "/10\n";
}

int main() {
    run_interface_tests();
    return 0;
}
