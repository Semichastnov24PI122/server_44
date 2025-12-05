#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <string>
#include <map>

class AuthManager {
private:
    std::map<std::string, std::string> users; // user:password_hash
    
public:
    AuthManager();
    bool loadUserDatabase(const std::string& filename);
    std::string generateSalt64(); // 64-bit salt (16 hex chars)
    std::string padZeros(const std::string& str, size_t length);
    std::string computeSHA1(const std::string& salt, const std::string& password);
    bool authenticate(const std::string& username, 
                      const std::string& salt, 
                      const std::string& clientHash);
    bool userExists(const std::string& username);
    
    // Для тестов
    void addUser(const std::string& user, const std::string& hash);
};

#endif
