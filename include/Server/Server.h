#ifndef SERVER_H
#define SERVER_H

#include <string>

class UserDatabase;
class Logger;
class DataProcessor;

class Server {
private:
    UserDatabase& user_database;
    Logger& logger;
    DataProcessor& data_processor;
    int port;
    bool is_running;
    int server_socket;

    bool setupSocket();
    void mainLoop();
    void handleClient(int client_socket);

public:
    Server(UserDatabase& db, Logger& log, DataProcessor& dp, int p);
    bool run();
    void stop();
};

#endif
