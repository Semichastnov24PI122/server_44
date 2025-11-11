#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <cstdint>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(sock);
        return 1;
    }

    std::cout << "Connected to server" << std::endl;

    // Отправляем LOGIN
    std::string login = "test_user";
    send(sock, login.c_str(), login.size(), 0);

    // Получаем SALT16
    char salt[17];
    recv(sock, salt, 16, 0);
    salt[16] = '\0';
    std::cout << "Received SALT16: " << salt << std::endl;

    // Отправляем HASH (упрощенный тест)
    std::string hash = "test_hash";
    send(sock, hash.c_str(), hash.size(), 0);

    // Получаем ответ аутентификации
    char auth_result[3];
    recv(sock, auth_result, 2, 0);
    auth_result[2] = '\0';
    std::cout << "Auth result: " << auth_result << std::endl;

    if (strcmp(auth_result, "OK") == 0) {
        // Отправляем тестовые данные
        uint32_t num_vectors = 2;
        send(sock, &num_vectors, sizeof(num_vectors), 0);

        // Первый вектор
        uint32_t size1 = 3;
        uint32_t data1[] = {1, 2, 3};
        send(sock, &size1, sizeof(size1), 0);
        send(sock, data1, size1 * sizeof(uint32_t), 0);

        // Второй вектор
        uint32_t size2 = 2;
        uint32_t data2[] = {10, 20};
        send(sock, &size2, sizeof(size2), 0);
        send(sock, data2, size2 * sizeof(uint32_t), 0);

        // Получаем результаты
        uint32_t result_count;
        recv(sock, &result_count, sizeof(result_count), 0);
        
        for (uint32_t i = 0; i < result_count; i++) {
            uint32_t result;
            recv(sock, &result, sizeof(result), 0);
            std::cout << "Result " << i << ": " << result << std::endl;
        }
    }

    close(sock);
    return 0;
}
