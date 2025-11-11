#ifndef USERDATABASE_H
#define USERDATABASE_H

#include <string>
#include <unordered_map>

class UserDatabase {
private:
    std::unordered_map<std::string, std::string> users;
    std::string db_file_path;

public:
    bool loadFromFile(const std::string& file_path);
    bool verifyPassword(const std::string& login, const std::string& password_hash);
    std::string getDbFilePath() const;
};

#endif
