#include "auth/AuthManager.h"
#include "hash/SHA1.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>

AuthManager::AuthManager() {
    srand(time(nullptr));
}

bool AuthManager::loadUserDatabase(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string user = line.substr(0, colon);
            std::string hash = line.substr(colon + 1);
            users[user] = hash;
        }
    }
    file.close();
    return true;
}

std::string AuthManager::generateSalt64() {
    std::stringstream ss;
    for (int i = 0; i < 16; i++) { // 16 hex chars = 64 bits
        ss << std::hex << (rand() % 16);
    }
    std::string salt = ss.str();
    // Приводим к верхнему регистру
    std::transform(salt.begin(), salt.end(), salt.begin(), ::toupper);
    return padZeros(salt, 16);
}

std::string AuthManager::padZeros(const std::string& str, size_t length) {
    if (str.length() >= length) {
        return str;
    }
    return std::string(length - str.length(), '0') + str;
}

std::string AuthManager::computeSHA1(const std::string& salt, 
                                     const std::string& password) {
    return SHA1::hash(salt + password);
}

bool AuthManager::authenticate(const std::string& username, 
                               const std::string& salt, 
                               const std::string& clientHash) {
    auto it = users.find(username);
    if (it == users.end()) {
        return false;
    }
    
    std::string expectedHash = SHA1::hash(salt, it->second);
    return (clientHash == expectedHash);
}

bool AuthManager::userExists(const std::string& username) {
    return users.find(username) != users.end();
}

void AuthManager::addUser(const std::string& user, const std::string& hash) {
    users[user] = hash;
}
