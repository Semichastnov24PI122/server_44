/**
 * @class AuthManager
 * @brief Менеджер аутентификации пользователей.
 *
 * Загружает базу пользователей из файла, проверяет логины и хеши паролей.
 * Использует схему аутентификации SHA-1 с 64-битной солью от клиента.
 */
#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <string>
#include <map>

class AuthManager {
private:
    std::map<std::string, std::string> users;
    
    std::string padZeros(const std::string& str, size_t length);
    std::string computeSHA1(const std::string& salt, const std::string& password);
    
public:
    // Только то, что используется в тестах и сервере
    AuthManager() = default;
    ~AuthManager() = default;
    
    bool loadUserDatabase(const std::string& filename);
    bool userExists(const std::string& username);
    bool authenticate(const std::string& username, 
                     const std::string& salt, 
                     const std::string& clientHash);
    void addUser(const std::string& username, const std::string& passwordHash);
    std::string generateSalt64();
};

#endif
