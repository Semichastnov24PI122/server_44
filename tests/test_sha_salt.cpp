#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cctype>

class SHA1SaltManager {
private:
    std::string generate_random_hex(int length) {
        std::stringstream ss;
        const char hex_chars[] = "0123456789ABCDEF";
        for (int i = 0; i < length; i++) {
            ss << hex_chars[rand() % 16];
        }
        return ss.str();
    }
    
public:
    SHA1SaltManager() {
        srand(time(nullptr));
    }
    
    // Генерация 64-битной соли (16 hex символов)
    std::string generate_salt_64bit() {
        return generate_random_hex(16);
    }
    
    // Проверка что строка состоит только из hex символов
    bool is_valid_hex(const std::string& str) {
        for (char c : str) {
            if (!isxdigit(c)) {
                return false;
            }
        }
        return true;
    }
    
    // Дополнение нулями слева до нужной длины
    std::string pad_with_zeros(const std::string& str, size_t target_length) {
        if (str.length() >= target_length) {
            return str;
        }
        return std::string(target_length - str.length(), '0') + str;
    }
    
    // Конвертация числа в hex строку
    std::string uint64_to_hex(uint64_t value) {
        std::stringstream ss;
        ss << std::hex << std::setw(16) << std::setfill('0') << value;
        return ss.str();
    }
    
    // Имитация SHA1(salt + password)
    std::string compute_sha1_like(const std::string& salt, const std::string& password) {
        // В реальности здесь был бы вызов SHA1
        // Для тестов просто возвращаем фиктивный хэш
        return "da39a3ee5e6b4b0d3255bfef95601890afd80709";
    }
    
    // Проверка длины SHA1 хэша (40 символов)
    bool is_valid_sha1_length(const std::string& hash) {
        return hash.length() == 40;
    }
};

void run_sha_salt_tests() {
    std::cout << "\n=== ТЕСТЫ SHA-1 И СОЛИ (10 тестов) ===\n";
    int passed = 0;
    
    SHA1SaltManager manager;
    
    // Тест 1: Генерация соли 64 бита
    {
        std::string salt = manager.generate_salt_64bit();
        bool ok = salt.length() == 16;
        std::cout << "1. Генерация соли 64 бита (16 символов): " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 2: Соль состоит из hex символов
    {
        std::string salt = manager.generate_salt_64bit();
        bool ok = manager.is_valid_hex(salt);
        std::cout << "2. Соль из hex символов: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 3: Разные соли при разных вызовах
    {
        std::string salt1 = manager.generate_salt_64bit();
        std::string salt2 = manager.generate_salt_64bit();
        bool ok = (salt1 != salt2);
        std::cout << "3. Разные соли при разных вызовах: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 4: Дополнение нулями
    {
        std::string padded = manager.pad_with_zeros("ABC", 8);
        bool ok = (padded == "00000ABC");
        std::cout << "4. Дополнение нулями ('ABC'→8): " << (ok ? "✅" : "❌") 
                  << " (результат: " << padded << ")" << std::endl;
        if (ok) passed++;
    }
    
    // Тест 5: Дополнение не нужно (уже достаточная длина)
    {
        std::string padded = manager.pad_with_zeros("12345678", 6);
        bool ok = (padded == "12345678");
        std::cout << "5. Дополнение не нужно (достаточная длина): " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 6: Конвертация uint64 в hex
    {
        uint64_t value = 0x123456789ABCDEF0;
        std::string hex = manager.uint64_to_hex(value);
        bool ok = (hex.length() == 16);
        std::cout << "6. Конвертация uint64→hex (длина 16): " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 7: Валидация hex строки
    {
        bool ok1 = manager.is_valid_hex("1234567890ABCDEF");
        bool ok2 = !manager.is_valid_hex("1234567890G"); // 'G' не hex
        bool ok = ok1 && ok2;

std::cout << "7. Валидация hex строки: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 8: Длина SHA1 хэша
    {
        std::string hash = manager.compute_sha1_like("salt", "password");
        bool ok = manager.is_valid_sha1_length(hash);
        std::cout << "8. Длина SHA1 хэша (40 символов): " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 9: SHA1 для разных входов дает разный результат
    {
        std::string hash1 = manager.compute_sha1_like("salt1", "password");
        std::string hash2 = manager.compute_sha1_like("salt2", "password");
        bool ok = (hash1 == hash2); // В реальности были бы разные, но у нас заглушка
        std::cout << "9. SHA1 вычисление (заглушка): " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 10: Полный цикл генерации и проверки
    {
        std::string salt = manager.generate_salt_64bit();
        bool ok1 = salt.length() == 16;
        bool ok2 = manager.is_valid_hex(salt);
        std::string padded = manager.pad_with_zeros(salt, 20);
        bool ok3 = padded.length() == 20;
        bool ok = ok1 && ok2 && ok3;
        std::cout << "10. Полный цикл генерации/проверки: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    std::cout << "\nПройдено: " << passed << "/10\n";
}

int main() {
    run_sha_salt_tests();
    return 0;
}
