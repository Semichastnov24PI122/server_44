#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <map>

class Logger {
public:
    bool open_log(const std::string& file) {
        std::ofstream f(file, std::ios::app);
        bool result = f.is_open();
        f.close();
        return result;
    }
    
    void log_error(bool critical, const std::string& msg) {
        std::cout << (critical ? "[CRITICAL] " : "[WARNING] ") << msg << std::endl;
    }
};

class UserDatabase {
private:
    std::map<std::string, std::string> users;
public:
    bool load_from_string(const std::string& data) {
        size_t pos = 0;
        while (pos < data.length()) {
            size_t colon = data.find(':', pos);
            size_t endline = data.find('\n', pos);
            if (colon == std::string::npos) break;
            
            std::string user = data.substr(pos, colon - pos);
            std::string hash = data.substr(colon + 1, endline - colon - 1);
            
            users[user] = hash;
            pos = (endline == std::string::npos) ? data.length() : endline + 1;
        }
        return true;
    }
    
    bool verify(const std::string& user, const std::string& hash) {
        auto it = users.find(user);
        return it != users.end() && it->second == hash;
    }
    
    size_t count() const { return users.size(); }
};

class DataProcessor {
public:
    uint32_t sum_of_squares(const uint32_t* data, size_t size) {
        uint64_t sum = 0;
        for (size_t i = 0; i < size; i++) {
            sum += static_cast<uint64_t>(data[i]) * data[i];
        }
        return (sum > 0xFFFFFFFF) ? 0xFFFFFFFF : static_cast<uint32_t>(sum);
    }
};

void run_basic_tests() {
    std::cout << "=== БАЗОВЫЕ ТЕСТЫ (10 тестов) ===\n";
    int passed = 0;
    
    // Тест 1
    {
        Logger logger;
        bool result = logger.open_log("test1.log");
        std::cout << "1. Logger::open_log: " << (result ? "✅" : "❌") << std::endl;
        if (result) passed++;
    }
    
    // Тест 2
    {
        Logger logger;
        logger.log_error(true, "Тестовая ошибка");
        std::cout << "2. Logger::log_error: ✅" << std::endl;
        passed++;
    }
    
    // Тест 3
    {
        UserDatabase db;
        bool result = db.load_from_string("alice:hash1\nbob:hash2\ncharlie:hash3\n");
        std::cout << "3. UserDatabase::load: " << (result ? "✅" : "❌") << std::endl;
        if (result) passed++;
    }
    
    // Тест 4
    {
        UserDatabase db;
        db.load_from_string("alice:hash1\nbob:hash2\n");
        bool result = db.verify("alice", "hash1");
        std::cout << "4. UserDatabase::verify (правильный): " << (result ? "✅" : "❌") << std::endl;
        if (result) passed++;
    }
    
    // Тест 5
    {
        UserDatabase db;
        db.load_from_string("alice:hash1\n");
        bool result = !db.verify("alice", "wrong");
        std::cout << "5. UserDatabase::verify (неверный пароль): " << (result ? "✅" : "❌") << std::endl;
        if (result) passed++;
    }
    
    // Тест 6
    {
        UserDatabase db;
        db.load_from_string("alice:hash1\n");
        bool result = !db.verify("unknown", "hash");
        std::cout << "6. UserDatabase::verify (несуществующий): " << (result ? "✅" : "❌") << std::endl;
        if (result) passed++;
    }
    
    // Тест 7
    {
        DataProcessor dp;
        uint32_t data[] = {1, 2, 3, 4};
        uint32_t result = dp.sum_of_squares(data, 4);
        bool ok = (result == 30);
        std::cout << "7. DataProcessor::sum_of_squares: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 8
    {
        DataProcessor dp;
        uint32_t data[] = {};
        uint32_t result = dp.sum_of_squares(data, 0);
        bool ok = (result == 0);
        std::cout << "8. DataProcessor::sum_of_squares (пустой): " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 9
    {
        DataProcessor dp;
        uint32_t data[] = {10};
        uint32_t result = dp.sum_of_squares(data, 1);

bool ok = (result == 100);
        std::cout << "9. DataProcessor::sum_of_squares (одно число): " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 10
    {
        DataProcessor dp;
        uint32_t data[] = {1000, 2000};
        uint32_t result = dp.sum_of_squares(data, 2);
        bool ok = (result == 1000000 + 4000000);
        std::cout << "10. DataProcessor::sum_of_squares (большие числа): " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    std::cout << "\nПройдено: " << passed << "/10\n";
}

int main() {
    run_basic_tests();
    return 0;
}
