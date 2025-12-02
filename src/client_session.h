#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H

#include <cstdint>
#include <vector>
#include <string>
#include "logger.h"
#include "user_database.h"
#include "data_processor.h"

class ClientSession {
private:
    int socket_fd;
    Logger* logger;
    UserDatabase* user_db;
    DataProcessor* data_processor;
    bool authenticated;
    
public:
    ClientSession(int socket, Logger* log, UserDatabase* db, DataProcessor* processor)
        : socket_fd(socket), logger(log), user_db(db), 
          data_processor(processor), authenticated(false) {}
    
    bool is_authenticated() const { return authenticated; }
    
    // Аутентификация
    bool authenticate_user(const std::string& login, const std::string& salt, 
                          const std::string& hash) {
        if (!user_db) {
            if (logger) logger->log_error(true, "User database not initialized");
            return false;
        }
        
        bool result = user_db->verify_password(login, hash);
        authenticated = result;
        
        if (logger) {
            if (result) {
                logger->log_error(false, "User " + login + " authenticated");
            } else {
                logger->log_error(false, "Authentication failed for " + login);
            }
        }
        
        return result;
    }
    
    // Обработка данных
    uint32_t process_vector(const std::vector<uint32_t>& data) {
        if (!authenticated) {
            if (logger) logger->log_error(true, "No authentication");
            return 0;
        }
        
        if (!data_processor) {
            if (logger) logger->log_error(true, "No data processor");
            return 0;
        }
        
        return data_processor->compute_sum_of_squares(data.data(), data.size());
    }
    
    std::vector<uint32_t> process_vectors(const std::vector<std::vector<uint32_t>>& vectors) {
        std::vector<uint32_t> results;
        if (!authenticated) {
            if (logger) logger->log_error(true, "No authentication for multiple vectors");
            return results;
        }
        
        for (const auto& vec : vectors) {
            results.push_back(process_vector(vec));
        }
        
        return results;
    }
    
    void handle_session() {
        if (logger) logger->log_error(false, "Handling session");
    }
    
    void close_session() {
        if (logger) logger->log_error(false, "Closing session");
        authenticated = false;
    }
};

#endif
