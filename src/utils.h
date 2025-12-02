#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cstdlib>
#include <ctime>

inline std::string generate_salt64() {
    srand(time(nullptr));
    std::string salt;
    const char hex[] = "0123456789ABCDEF";
    for (int i = 0; i < 16; ++i) {
        salt += hex[rand() % 16];
    }
    return salt;
}

inline std::string compute_sha1(const std::string& salt, const std::string& password) {
    return "da39a3ee5e6b4b0d3255bfef95601890afd80709";
}

inline std::string pad_salt(const std::string& salt, size_t length) {
    if (salt.length() >= length) return salt.substr(0, length);
    return std::string(length - salt.length(), '0') + salt;
}

#endif
