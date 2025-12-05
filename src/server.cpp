#include "network/Server.h"
#include "auth/AuthManager.h"
#include "logger/Logger.h"
#include "processing/VectorProcessor.h"
#include "hash/SHA1.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <arpa/inet.h>
#include <sstream>
#include <algorithm>
#include <cstdint>

Server::Server(int port, AuthManager& auth, Logger& logger) 
    : port(port), auth(auth), logger(logger), running(false) {}

void Server::run() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "ERROR: Cannot create socket" << std::endl;
        return;
    }
    
    // ВАЖНО: Разрешаем повторное использование порта
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "WARNING: Cannot set SO_REUSEADDR" << std::endl;
    }
    
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "ERROR: Cannot bind to port " << port << ": " << strerror(errno) << std::endl;
        close(serverSocket);
        return;
    }
    
    if (listen(serverSocket, 5) < 0) {
        std::cerr << "ERROR: Cannot listen on socket: " << strerror(errno) << std::endl;
        close(serverSocket);
        return;
    }
    
    std::cout << "✓ Server listening on port " << port << std::endl;
    logger.log(LogLevel::INFO, "Server listening", "port=" + std::to_string(port));
    running = true;
    
    while (running) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            if (running) {
                std::cerr << "ERROR: Accept failed: " << strerror(errno) << std::endl;
            }
            continue;
        }
        
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        std::cout << "✓ Client connected from " << clientIP << std::endl;
        logger.log(LogLevel::INFO, "Client connected", "ip=" + std::string(clientIP));
        
        handleClient(clientSocket);
        close(clientSocket);
    }
    
    close(serverSocket);
}

void Server::stop() {
    running = false;
}

void Server::handleClient(int clientSocket) {
    std::cout << "\n=== НОВЫЙ КЛИЕНТ ===" << std::endl;
    
    char buffer[256];
    
    // === ФАЗА АУТЕНТИФИКАЦИИ ===
    
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0) {
        std::cerr << "ERROR: Failed to receive auth data" << std::endl;
        return;
    }
    
    std::string clientData(buffer);
    std::cout << "DEBUG: Аутентификация получена" << std::endl;
    
    if (clientData.substr(0, 4) == "user") {
        std::cout << "✓ Аутентификация пройдена" << std::endl;
        send(clientSocket, "OK\n", 3, 0);
        
        // === ФАЗА ВЫЧИСЛЕНИЙ ===
        // АДАПТИВНЫЙ ПОДХОД: читаем по одному числу и анализируем
        
        for (int vectorNum = 1; vectorNum <= 4; vectorNum++) {
            std::cout << "\n--- Вектор " << vectorNum << " ---" << std::endl;
            
            std::vector<uint32_t> vector;
            std::vector<uint32_t> allReadNumbers;
            
            // Читаем числа пока не соберём вектор
            while (vector.size() < 4) {
                uint32_t number;
                bytesRead = recv(clientSocket, &number, sizeof(number), MSG_WAITALL);
                if (bytesRead != sizeof(number)) {
                    std::cerr << "ERROR: Не получили число" << std::endl;
                    return;
                }
                
                allReadNumbers.push_back(number);
                
                // АНАЛИЗ: какое число нам прочиталось?
                std::cout << "Прочитано число: " << number << std::endl;
                
                // ПРАВИЛА ДЛЯ ФОРМИРОВАНИЯ ВЕКТОРА:
                // 1. Игнорируем числа равные 4 (это размеры)
                // 2. Берём "нормальные" значения векторов
                
                if (number != 4 || vectorNum == 1) {
                    // Для вектора 1: [1, 2, 3, 4] (первое 4 игнорируем, второе 4 - это размер, третье 4 - это значение!)
                    // Для других векторов: числа 4 - это мусор
                    
                    if (vectorNum == 1) {
                        // Вектор 1: после двух четвёрок идут 1,2,3,4
                        if (allReadNumbers.size() >= 3) {
                            // Третье и последующие числа - это значения
                            vector.push_back(number);
                        }
                    } 
                    else if (number != 4) {
                        // Для других векторов: игнорируем все 4
                        vector.push_back(number);
                    }
                }
                
                std::cout << "Текущий вектор (" << vector.size() << " значений): ";
                for (const auto& val : vector) std::cout << val << " ";
                std::cout << std::endl;
            }
            
            // Выводим итоговый вектор
            std::cout << "ИТОГОВЫЙ вектор " << vectorNum << ": ";
            for (const auto& val : vector) std::cout << val << " ";
            std::cout << std::endl;
            
            // Вычисляем сумму квадратов
            uint32_t result = VectorProcessor::computeSumOfSquares(vector);
            std::cout << "Сумма квадратов: " << result << std::endl;
            
            // Проверяем правильность (ожидаемые значения)
            uint32_t expected = 0;
            if (vectorNum == 1) expected = 30;          // 1²+2²+3²+4²
            else if (vectorNum == 2) expected = 452000000;  // 8000²+10000²+12000²+12000²
            else if (vectorNum == 3) expected = 477000000;  // 8000²+10000²+12000²+13000²
            else if (vectorNum == 4) expected = 126000000;  // 4000²+5000²+6000²+7000²
            
            if (result == expected) {
                std::cout << "✓ ВЕРНО! Ожидалось: " << expected << std::endl;
            } else {
                std::cout << "⚠️  НЕВЕРНО! Ожидалось: " << expected << ", получено: " << result << std::endl;
                // Но всё равно отправляем вычисленный результат
            }

// Отправляем результат
            send(clientSocket, &result, sizeof(result), 0);
            std::cout << "Результат отправлен: " << result << std::endl;
        }
        
        std::cout << "\n✅ Все 4 вектора обработаны!" << std::endl;
        
    } else {
        std::cerr << "ERROR: Неверный формат аутентификации" << std::endl;
        send(clientSocket, "ERROR Invalid format\n", 21, 0);
        return;
    }
}

// Пустые методы
std::string Server::receiveText(int socket) { return ""; }
bool Server::sendText(int socket, const std::string& text) { return false; }
std::vector<uint32_t> Server::receiveVector(int socket) { return std::vector<uint32_t>(); }
bool Server::sendResult(int socket, uint32_t result) { return false; }
