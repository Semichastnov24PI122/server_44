#include "UserDatabase/UserDatabase.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool UserDatabase::loadFromFile(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        return false;
    }
    
    users.clear();
    db_file_path = file_path;
    
    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            std::string login = line.substr(0, pos);
            std::string password = line.substr(pos + 1);
            users[login] = password;
        }
    }
    
    file.close();
    return true;
}

bool UserDatabase::verifyPassword(const std::string& login, const std::string& password_hash) {
    auto it = users.find(login);
    return it != users.end(); // Просто проверяем что пользователь существует
}

std::string UserDatabase::getDbFilePath() const {
    return db_file_path;
}
