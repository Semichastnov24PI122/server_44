#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#include <string>
#include <arpa/inet.h>

inline std::string parse_login_message(const std::string& msg) {
    if (msg.find("LOGIN ") == 0) {
        return msg.substr(6, msg.find('\n') - 6);
    }
    return "";
}

inline std::string format_salt_response(const std::string& salt) {
    return "SALT16 " + salt + "\n";
}

inline std::string format_ok_response() {
    return "OK\n";
}

inline std::string format_err_response(const std::string& message) {
    return "ERR " + message + "\n";
}

#endif
