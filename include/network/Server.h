/**
 * @class Server
 * @brief Основной класс сетевого сервера.
 *
 * Устанавливает TCP-сокет, ожидает подключений клиентов.
 * Создает сессии клиентов, координирует аутентификацию и обработку данных.
 */
#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <vector>
#include <cstdint>
#include "auth/AuthManager.h"
#include "logger/Logger.h"

class Server {
private:
    int port;
    AuthManager& auth;
    Logger& logger;
    bool running;
    
public:
    Server(int port, AuthManager& auth, Logger& logger);
    void run();  // Должен быть публичным!
    void stop();
    
private:
    void handleClient(int clientSocket);
    std::string receiveText(int socket);
    bool sendText(int socket, const std::string& text);
    std::vector<uint32_t> receiveVector(int socket);
    bool sendResult(int socket, uint32_t result);
};

#endif
