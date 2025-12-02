#include <iostream>
#include <string>
#include <map>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

class AuthManager {
private:
    std::map<std::string, std::string> users;
    
public:
    AuthManager() { srand(time(nullptr)); }
    
    void add_user(const std::string& user, const std::string& hash) {
        users[user] = hash;
    }
    
    bool identify(const std::string& user) {
        return users.find(user) != users.end();
    }
    
    bool authenticate(const std::string& user, const std::string& salt, const std::string& hash) {
        auto it = users.find(user);
        if (it == users.end()) return false;
        
        std::string expected = salt + it->second;
        return hash == expected;
    }
    
    std::string generate_salt() {
        std::stringstream ss;
        for (int i = 0; i < 16; i++) {
            ss << std::hex << (rand() % 16);
        }
        return ss.str();
    }
    
    std::string pad_zeros(const std::string& s, size_t len) {
        if (s.length() >= len) return s;
        return std::string(len - s.length(), '0') + s;
    }
};

void run_auth_tests() {
    std::cout << "\n=== ТЕСТЫ АУТЕНТИФИКАЦИИ (10 тестов) ===\n";
    int passed = 0;
    
    AuthManager auth;
    auth.add_user("admin", "admin_hash");
    auth.add_user("user1", "hash1");
    auth.add_user("user2", "hash2");
    
    // Тест 1
    {
        bool result = auth.identify("admin");
        std::cout << "1. AuthManager::identify (существующий): " << (result ? "✅" : "❌") << std::endl;
        if (result) passed++;
    }
    
    // Тест 2
    {
        bool result = !auth.identify("unknown");
        std::cout << "2. AuthManager::identify (несуществующий): " << (result ? "✅" : "❌") << std::endl;
        if (result) passed++;
    }
    
    // Тест 3
    {
        std::string salt = "SALT";
        std::string hash = "SALT" + std::string("admin_hash");
        bool result = auth.authenticate("admin", salt, hash);
        std::cout << "3. AuthManager::authenticate (успешная): " << (result ? "✅" : "❌") << std::endl;
        if (result) passed++;
    }
    
    // Тест 4
    {
        bool result = !auth.authenticate("admin", "SALT", "wrong_hash");
        std::cout << "4. AuthManager::authenticate (неверный пароль): " << (result ? "✅" : "❌") << std::endl;
        if (result) passed++;
    }
    
    // Тест 5
    {
        bool result = !auth.authenticate("unknown", "SALT", "hash");
        std::cout << "5. AuthManager::authenticate (несуществующий): " << (result ? "✅" : "❌") << std::endl;
        if (result) passed++;
    }
    
    // Тест 6
    {
        std::string salt = auth.generate_salt();
        bool result = (salt.length() == 16);
        std::cout << "6. AuthManager::generate_salt (длина 16): " << (result ? "✅" : "❌") << std::endl;
        if (result) passed++;
    }
    
    // Тест 7
    {
        std::string salt = auth.generate_salt();
        bool is_hex = true;
        for (char c : salt) {
            if (!((c >= '0' && c <= '9') ||  (c >= 'a' && c <= 'f') ||  (c >= 'A' && c <= 'F'))) {
                is_hex = false;
                break;
            }
        }
        std::cout << "7. AuthManager::generate_salt (hex формат): " << (is_hex ? "✅" : "❌") << std::endl;
        if (is_hex) passed++;
    }
    
    // Тест 8
    {
        std::string padded = auth.pad_zeros("ABC", 8);
        bool result = (padded == "00000ABC");
        std::cout << "8. AuthManager::pad_zeros: " << (result ? "✅" : "❌") << std::endl;
        if (result) passed++;
    }
    
    // Тест 9
    {
        std::string padded = auth.pad_zeros("12345678", 6);
        bool result = (padded == "12345678");
        std::cout << "9. AuthManager::pad_zeros (достаточная длина): " << (result ? "✅" : "❌") << std::endl;
        if (result) passed++;
    }
    
    // Тест 10
    {
        std::string salt1 = "SALT1";
        std::string salt2 = "SALT2";
        std::string hash1 = salt1 + "admin_hash";
        std::string hash2 = salt2 + "admin_hash";

bool result1 = auth.authenticate("admin", salt1, hash1);
        bool result2 = auth.authenticate("admin", salt2, hash2);
        bool ok = result1 && result2 && (hash1 != hash2);
        std::cout << "10. Разные соли -> разные хэши: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    std::cout << "\nПройдено: " << passed << "/10\n";
}

int main() {
    run_auth_tests();
    return 0;
}

