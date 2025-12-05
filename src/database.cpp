#include "auth/AuthManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

// Функции для работы с базой данных пользователей
// (Дублируют функциональность AuthManager для совместимости)

bool load_user_database(const std::string& filename, 
                        std::map<std::string, std::string>& users) {
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

bool verify_user(const std::map<std::string, std::string>& users,
                 const std::string& user, 
                 const std::string& hash) {
    auto it = users.find(user);
    return it != users.end() && it->second == hash;
}

size_t count_users(const std::map<std::string, std::string>& users) {
    return users.size();
}

void add_user_to_db(std::map<std::string, std::string>& users,
                    const std::string& user, 
                    const std::string& hash) {
    users[user] = hash;
}

// Альтернативно: создаём класс Database для совместимости
class Database {
private:
    std::map<std::string, std::string> users;
    
public:
    bool load(const std::string& filename) {
        return load_user_database(filename, users);
    }
    
    bool verify(const std::string& user, const std::string& hash) {
        return verify_user(users, user, hash);
    }
    
    size_t count() const {
        return users.size();
    }
    
    void add(const std::string& user, const std::string& hash) {
        add_user_to_db(users, user, hash);
    }
};
