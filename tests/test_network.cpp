#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <arpa/inet.h>

class NetworkProtocol {
public:
    static std::string parse_login(const std::string& msg) {
        if (msg.find("LOGIN ") == 0) {
            size_t space = msg.find(' ');
            size_t end = msg.find('\n');
            if (end == std::string::npos) end = msg.length();
            return msg.substr(space + 1, end - space - 1);
        }
        return "";
    }
    
    static std::string format_salt(const std::string& salt) {
        return "SALT16 " + salt + "\n";
    }
    
    static std::string format_ok() {
        return "OK\n";
    }
    
    static std::string format_err(const std::string& msg) {
        return "ERR " + msg + "\n";
    }
    
    static uint32_t host_to_net(uint32_t val) {
        return htonl(val);
    }
    
    static uint32_t net_to_host(uint32_t val) {
        return ntohl(val);
    }
};

void run_network_tests() {
    std::cout << "\n=== ТЕСТЫ СЕТЕВОГО ПРОТОКОЛА (10 тестов) ===\n";
    int passed = 0;
    
    // Тест 1: Парсинг LOGIN сообщения
    {
        std::string result = NetworkProtocol::parse_login("LOGIN admin\n");
        bool ok = (result == "admin");
        std::cout << "1. Парсинг LOGIN: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 2: Парсинг некорректного LOGIN
    {
        std::string result = NetworkProtocol::parse_login("INVALID admin\n");
        bool ok = result.empty();
        std::cout << "2. Парсинг некорректного LOGIN: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 3: Формирование SALT16
    {
        std::string result = NetworkProtocol::format_salt("1234567890ABCDEF");
        bool ok = (result == "SALT16 1234567890ABCDEF\n");
        std::cout << "3. Формирование SALT16: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 4: Формирование OK
    {
        std::string result = NetworkProtocol::format_ok();
        bool ok = (result == "OK\n");
        std::cout << "4. Формирование OK: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 5: Формирование ERR
    {
        std::string result = NetworkProtocol::format_err("Auth failed");
        bool ok = (result == "ERR Auth failed\n");
        std::cout << "5. Формирование ERR: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 6: Конвертация host to network
    {
        uint32_t original = 0x12345678;
        uint32_t converted = NetworkProtocol::host_to_net(original);
        uint32_t back = NetworkProtocol::net_to_host(converted);
        bool ok = (original == back);
        std::cout << "6. Конвертация host->network->host: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 7: Конвертация маленького числа
    {
        uint32_t original = 100;
        uint32_t converted = NetworkProtocol::host_to_net(original);
        uint32_t back = NetworkProtocol::net_to_host(converted);
        bool ok = (original == back);
        std::cout << "7. Конвертация маленького числа: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 8: Конвертация максимального числа
    {
        uint32_t original = 0xFFFFFFFF;
        uint32_t converted = NetworkProtocol::host_to_net(original);
        uint32_t back = NetworkProtocol::net_to_host(converted);
        bool ok = (original == back);
        std::cout << "8. Конвертация максимального числа: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 9: Парсинг LOGIN без перевода строки
    {
        std::string result = NetworkProtocol::parse_login("LOGIN admin");
        bool ok = (result == "admin");
        std::cout << "9. Парсинг LOGIN без \\n: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 10: Формирование с пустым сообщением ERR
    {
        std::string result = NetworkProtocol::format_err("");
        bool ok = (result == "ERR \n");

std::cout << "10. Формирование ERR с пустым сообщением: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    std::cout << "\nПройдено: " << passed << "/10\n";
}

int main() {
    run_network_tests();
    return 0;
}
