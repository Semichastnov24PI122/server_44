#include "Server/Server.h"
#include "ClientSession/ClientSession.h"
#include "Logger/Logger.h"
#include "UserDatabase/UserDatabase.h"
#include "DataProcessor/DataProcessor.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

Server::Server(UserDatabase& db, Logger& log, DataProcessor& dp, int p)
    : user_database(db), logger(log), data_processor(dp), port(p), is_running(false), server_socket(-1) {
}

bool Server::setupSocket() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        logger.logError(true, "Failed to create socket");
        return false;
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        logger.logError(true, "Failed to set socket options");
        close(server_socket);
        return false;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        logger.logError(true, "Failed to bind socket to port " + std::to_string(port));
        close(server_socket);
        return false;
    }

    if (listen(server_socket, 5) < 0) {
        logger.logError(true, "Failed to listen on socket");
        close(server_socket);
        return false;
    }

    return true;
}

void Server::handleClient(int client_socket) {
    ClientSession session(client_socket, logger, user_database, data_processor);
    session.start();
}

void Server::mainLoop() {
    logger.logError(false, "Server started on port " + std::to_string(port));
    
    while (is_running) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            if (is_running) {
                logger.logError(false, "Failed to accept client connection");
            }
            continue;
        }

        // В однопоточном режиме обрабатываем клиента сразу
        handleClient(client_socket);
    }
}

bool Server::run() {
    if (!setupSocket()) {
        return false;
    }

    is_running = true;
    logger.logError(false, "Server running main loop");
    mainLoop();
    
    return true;
}

void Server::stop() {
    is_running = false;
    if (server_socket >= 0) {
        close(server_socket);
        server_socket = -1;
    }
    logger.logError(false, "Server stopped");
}
