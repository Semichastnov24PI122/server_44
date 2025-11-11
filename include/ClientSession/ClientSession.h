#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include <string>
#include <cstdint>

class Logger;
class UserDatabase;
class DataProcessor;

class ClientSession {
private:
    int client_socket;
    Logger& logger;
    UserDatabase& user_database;
    DataProcessor& data_processor;
    bool authenticated;

    std::string generateSalt();
    std::string computeHash(const std::string& salt, const std::string& password);
    bool authenticateClient();
    void processVectors();

public:
    ClientSession(int socket, Logger& logger_ref, UserDatabase& db_ref, DataProcessor& dp_ref);
    void start();
};

#endif
