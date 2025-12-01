#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/sha.h>
#include <time.h>
#include <stdint.h>
#include <errno.h>

#include "logger.h"
#include "database.h"

// Функция для вычисления суммы квадратов с проверкой переполнения
uint32_t calculate_sum_of_squares(uint32_t *data, uint32_t size) {
    uint64_t sum = 0;
    
    for (uint32_t i = 0; i < size; i++) {
        uint64_t square = (uint64_t)data[i] * data[i];
        sum += square;
        
        // Проверка переполнения (по ТЗ)
        if (sum > 0xFFFFFFFFULL) {
            return 0xFFFFFFFF; // 2^32 - 1
        }
    }
    
    return (uint32_t)sum;
}

// Функция для вычисления SHA-1 хэша
void compute_sha1_hash(const char *input, char *output) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1((unsigned char*)input, strlen(input), hash);

    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf(output + 2*i, "%02x", hash[i]);
    }
    output[40] = '\0';
}

void print_usage(const char *program_name) {
    printf("Usage: %s -b <database_file> -l <log_file> -p <port>\n", program_name);
    printf("Options:\n");
    printf("  -b <file>    User database file (required)\n");
    printf("  -l <file>    Log file (required)\n");
    printf("  -p <port>    Port number (required)\n");
    printf("  -h           Show this help message\n");
}

int main(int argc, char *argv[]) {
    // Обработка справки
    if (argc == 1 ||  (argc == 2 && strcmp(argv[1], "-h") == 0)) {
        print_usage(argv[0]);
        return 0;
    }
    
    // Парсинг аргументов командной строки
    char *db_filename = NULL;
    char *log_filename = NULL;
    int port = 0;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
            db_filename = argv[++i];
        } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            log_filename = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            port = atoi(argv[++i]);
            if (port <= 0 ||  port > 65535) {
                fprintf(stderr, "Error: Invalid port number %d\n", port);
                return 1;
            }
        }
    }
    
    // Проверка обязательных параметров
    if (!db_filename ||  !log_filename ||  port == 0) {
        fprintf(stderr, "Error: Missing required parameters\n");
        print_usage(argv[0]);
        return 1;
    }
    
    // Инициализация логгера
    init_logger(log_filename);
    log_message(LOG_INFO, "Server starting", NULL);
    
    // Загрузка базы данных пользователей
    UserDatabase *db = load_database(db_filename);
    if (!db) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "Failed to load database from %s", db_filename);
        log_message(LOG_ERROR, error_msg, NULL);
        fprintf(stderr, "Error: %s\n", error_msg);
        close_logger();
        return 1;
    }
    log_message(LOG_INFO, "Database loaded successfully", NULL);
    
    // Создание сокета
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        log_message(LOG_ERROR, "Failed to create socket", strerror(errno));
        fprintf(stderr, "Error: Failed to create socket: %s\n", strerror(errno));
        free_database(db);
        close_logger();
        return 1;
    }
    
    // Настройка опций сокета
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        log_message(LOG_ERROR, "Failed to set socket options", strerror(errno));
    }
    
    // Настройка адреса сервера
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    // Привязка сокета к адресу
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        char error_msg[256];

snprintf(error_msg, sizeof(error_msg), "Failed to bind to port %d", port);
        log_message(LOG_ERROR, error_msg, strerror(errno));
        fprintf(stderr, "Error: %s - %s\n", error_msg, strerror(errno));
        close(server_socket);
        free_database(db);
        close_logger();
        return 1;
    }
    
    // Прослушивание входящих соединений
    if (listen(server_socket, 5) < 0) {
        log_message(LOG_ERROR, "Failed to listen on socket", strerror(errno));
        fprintf(stderr, "Error: Failed to listen on socket: %s\n", strerror(errno));
        close(server_socket);
        free_database(db);
        close_logger();
        return 1;
    }
    
    printf("✅ Server started successfully on port %d\n", port);
    log_message(LOG_INFO, "Server started and listening", NULL);
    
    // Основной цикл сервера
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            log_message(LOG_ERROR, "Failed to accept client connection", strerror(errno));
            continue;
        }
        
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        
        printf("Client connected from %s\n", client_ip);
        log_message(LOG_INFO, "Client connected", client_ip);
        
        // ================= АУТЕНТИФИКАЦИЯ =================
        // Клиент отправляет 60 байт: login(4) + salt(16) + hash(40)
        char auth_data[64];
        int bytes_received = recv(client_socket, auth_data, 60, MSG_WAITALL);
        
        if (bytes_received != 60) {
            log_message(LOG_ERROR, "Invalid auth data length", NULL);
            send(client_socket, "ERR", 3, 0);
            close(client_socket);
            continue;
        }
        
        // Извлекаем логин
        char login[5] = {0};
        strncpy(login, auth_data, 4);
        login[4] = '\0';
        
        printf("DEBUG: Login: %s\n", login);
        
        // Проверяем пользователя
        char *password = find_password(db, login);
        if (!password) {
            log_message(LOG_ERROR, "User not found", login);
            send(client_socket, "ERR", 3, 0);
            close(client_socket);
            continue;
        }
        
        // Извлекаем соль и хэш
        char salt_client[17] = {0};
        strncpy(salt_client, auth_data + 4, 16);
        salt_client[16] = '\0';
        
        char client_hash[41] = {0};
        strncpy(client_hash, auth_data + 20, 40);
        client_hash[40] = '\0';
        
        // Проверяем хэш
        char combined[256];
        snprintf(combined, sizeof(combined), "%s%s", salt_client, password);
        char expected_hash[41];
        compute_sha1_hash(combined, expected_hash);
        
        if (strcasecmp(client_hash, expected_hash) != 0) {
            log_message(LOG_ERROR, "Authentication failed", login);
            send(client_socket, "ERR", 3, 0);
            close(client_socket);
            continue;
        }
        
        // Успешная аутентификация
        send(client_socket, "OK", 2, 0);
        log_message(LOG_INFO, "Client authenticated successfully", login);
        printf("✅ Client %s authenticated successfully\n", login);
        
        // ================= ОБРАБОТКА ВЕКТОРОВ =================
        printf("\n--- Processing vectors ---\n");
        
        // 1. Количество векторов
        uint32_t num_vectors;
        bytes_received = recv(client_socket, &num_vectors, 4, MSG_WAITALL);
        
        if (bytes_received != 4) {
            log_message(LOG_ERROR, "Failed to receive number of vectors", NULL);
            close(client_socket);
            continue;
        }
        
        printf("DEBUG: Number of vectors: %u\n", num_vectors);
        
        // 2. Обрабатываем каждый вектор
        for (uint32_t i = 0; i < num_vectors; i++) {
            // Размер вектора
            uint32_t vector_size;

bytes_received = recv(client_socket, &vector_size, 4, MSG_WAITALL);
            
            if (bytes_received != 4) {
                log_message(LOG_ERROR, "Failed to receive vector size", NULL);
                break;
            }
            
            printf("DEBUG: Vector %u size: %u\n", i + 1, vector_size);
            
            // Данные вектора
            uint32_t vector_data[4];
            bytes_received = recv(client_socket, vector_data, 16, MSG_WAITALL);
            
            if (bytes_received != 16) {
                log_message(LOG_ERROR, "Failed to receive vector data", NULL);
                break;
            }
            
            // ВАЖНО: Данные уже в правильном формате!
            // Не нужно делать ntohl() - debug показал что данные верные
            printf("DEBUG: Vector %u values: %u %u %u %u\n", 
                   i + 1, vector_data[0], vector_data[1], 
                   vector_data[2], vector_data[3]);
            
            // Вычисляем сумму квадратов
            uint32_t result = calculate_sum_of_squares(vector_data, 4);
            printf("DEBUG: Result: %u\n", result);
            
            // Отправляем результат
            send(client_socket, &result, 4, 0);
        }
        
        printf("--- Vectors processed ---\n\n");
        
        close(client_socket);
        log_message(LOG_INFO, "Client disconnected", login);
        printf("Client %s disconnected\n\n", login);
    }
    
    // Очистка ресурсов
    close(server_socket);
    free_database(db);
    close_logger();
    
    return 0;
}
