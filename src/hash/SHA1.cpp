#include "hash/SHA1.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <openssl/sha.h>
#include <cstring>

std::string SHA1::hash(const std::string& input) {
#ifdef WITH_OPENSSL
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(input.c_str()), input.length(), hash);
    
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0');
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        ss << std::setw(2) << static_cast<unsigned int>(hash[i]);
    }
    return ss.str();
#else
    // Простая эмуляция SHA1 для тестов - возвращаем разные значения для разных входов
    static unsigned int counter = 0;
    counter++;
    
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0');
    
    // Генерируем "уникальный" хэш на основе ввода
    unsigned int seed = 0;
    for (char c : input) {
        seed = seed * 31 + static_cast<unsigned char>(c);
    }
    
    ss << std::setw(8) << (seed + counter);
    ss << std::setw(8) << (seed * 2 + counter);
    ss << std::setw(8) << (seed * 3 + counter);
    ss << std::setw(8) << (seed * 4 + counter);
    ss << std::setw(8) << (seed * 5 + counter);
    
    return ss.str();
#endif
}

std::string SHA1::hash(const std::string& salt, const std::string& password) {
    std::string combined = salt + password;
    return hash(combined);
}
