/**
 * @file AuthManager.cpp
 * @brief Реализация менеджера аутентификации.
 *
 * Загрузка базы пользователей, проверка логинов/паролей,
 * генерация и проверка хешей SHA-1.
 */
#include "auth/AuthManager.h"
#include "hash/SHA1.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>
#include <iomanip>
#include <algorithm>

bool AuthManager::loadUserDatabase(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: Cannot open user database: " << filename << std::endl;
        return false;
    }
    
    users.clear();
    std::string line;
    while (std::getline(file, line)) {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string username = line.substr(0, colonPos);
            std::string passwordHash = line.substr(colonPos + 1);
            users[username] = passwordHash;
        }
    }
    
    file.close();
    std::cout << "Loaded " << users.size() << " users from " << filename << std::endl;
    return !users.empty();
}

bool AuthManager::userExists(const std::string& username) {
    return users.find(username) != users.end();
}

bool AuthManager::authenticate(const std::string& username, 
                              const std::string& salt, 
                              const std::string& clientHash) {
    (void)salt; // игнорируем warning
    
    if (!userExists(username)) {
        return false;
    }
    
    std::string storedHash = users[username];
    return clientHash == storedHash;
}

void AuthManager::addUser(const std::string& username, const std::string& passwordHash) {
    users[username] = passwordHash;
}

std::string AuthManager::generateSalt64() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;
    
    uint64_t salt = dis(gen);
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0') << std::setw(16) << salt;
    return ss.str();
}

std::string AuthManager::padZeros(const std::string& str, size_t length) {
    if (str.length() >= length) {
        return str;
    }
    return std::string(length - str.length(), '0') + str;
}

std::string AuthManager::computeSHA1(const std::string& salt, const std::string& password) {
    return SHA1::hash(salt, password);
}
