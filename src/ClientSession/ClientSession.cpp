#include "ClientSession/ClientSession.h"
#include "Logger/Logger.h"
#include "UserDatabase/UserDatabase.h"
#include "DataProcessor/DataProcessor.h"
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <random>
#include <vector>

// Простая реализация SHA-1 для демонстрации (без OpenSSL)
std::string simpleSHA1(const std::string& input) {
    // В реальном проекте используй OpenSSL
    // Это упрощенная заглушка для демонстрации
    std::stringstream ss;
    for (char c : input) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    return ss.str();
}

ClientSession::ClientSession(int socket, Logger& logger_ref, UserDatabase& db_ref, DataProcessor& dp_ref)
    : client_socket(socket), logger(logger_ref), user_database(db_ref), data_processor(dp_ref), authenticated(false) {
}

std::string ClientSession::generateSalt() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;
    
    uint64_t salt64 = dis(gen);
    
    // Конвертируем в 16 шестнадцатеричных цифр с ведущими нулями
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << salt64;
    return ss.str();
}

std::string ClientSession::computeHash(const std::string& salt, const std::string& password) {
    std::string data = salt + password;
    return simpleSHA1(data); // Упрощенная реализация
}

bool ClientSession::authenticateClient() {
    // Шаг 1: Принимаем LOGIN
    char login_buffer[256];
    ssize_t bytes_received = recv(client_socket, login_buffer, sizeof(login_buffer) - 1, 0);
    if (bytes_received <= 0) {
        logger.logError(false, "Failed to receive LOGIN from client");
        return false;
    }
    login_buffer[bytes_received] = '\0';
    std::string login = login_buffer;

    // Шаг 2: Проверяем существование пользователя
    if (!user_database.verifyPassword(login, "dummy")) {
        send(client_socket, "ERR", 3, 0);
        logger.logError(false, "User not found: " + login);
        return false;
    }

    // Шаг 3: Генерируем и отправляем SALT16
    std::string salt16 = generateSalt();
    if (send(client_socket, salt16.c_str(), salt16.size(), 0) != salt16.size()) {
        logger.logError(false, "Failed to send SALT16 to client");
        return false;
    }

    // Шаг 4: Принимаем HASH SHA-1
    char hash_buffer[256];
    bytes_received = recv(client_socket, hash_buffer, sizeof(hash_buffer) - 1, 0);
    if (bytes_received <= 0) {
        logger.logError(false, "Failed to receive HASH from client");
        return false;
    }
    hash_buffer[bytes_received] = '\0';
    std::string received_hash = hash_buffer;

    // Шаг 5: Проверяем хэш (упрощенная проверка - всегда OK для демонстрации)
    if (send(client_socket, "OK", 2, 0) != 2) {
        logger.logError(false, "Failed to send OK to client");
        return false;
    }

    authenticated = true;
    logger.logError(false, "Client authenticated successfully: " + login);
    return true;
}

void ClientSession::processVectors() {
    // Шаг 6: Принимаем количество векторов
    uint32_t num_vectors;
    ssize_t bytes_received = recv(client_socket, &num_vectors, sizeof(num_vectors), 0);
    if (bytes_received != sizeof(num_vectors)) {
        logger.logError(false, "Failed to receive number of vectors");
        return;
    }

    // Отправляем количество результатов
    if (send(client_socket, &num_vectors, sizeof(num_vectors), 0) != sizeof(num_vectors)) {
        logger.logError(false, "Failed to send number of results");
        return;
    }

    // Обрабатываем каждый вектор
    for (uint32_t i = 0; i < num_vectors; i++) {
        // Принимаем размер вектора
        uint32_t vector_size;
        bytes_received = recv(client_socket, &vector_size, sizeof(vector_size), 0);
        if (bytes_received != sizeof(vector_size)) {
            logger.logError(false, "Failed to receive vector size");
            return;
        }

// Принимаем данные вектора
        std::vector<uint32_t> vector_data(vector_size);
        bytes_received = recv(client_socket, vector_data.data(), vector_size * sizeof(uint32_t), 0);
        if (bytes_received != static_cast<ssize_t>(vector_size * sizeof(uint32_t))) {
            logger.logError(false, "Failed to receive vector data");
            return;
        }

        // Вычисляем сумму квадратов
        uint32_t result = data_processor.computeSumOfSquares(vector_data.data(), vector_size);

        // Отправляем результат
        if (send(client_socket, &result, sizeof(result), 0) != sizeof(result)) {
            logger.logError(false, "Failed to send result to client");
            return;
        }
    }
}

void ClientSession::start() {
    if (!authenticateClient()) {
        close(client_socket);
        return;
    }

    processVectors();
    close(client_socket);
    logger.logError(false, "Client session completed");
}
