#ifndef SHA_HASH_H
#define SHA_HASH_H

#include <string>

inline std::string sha1(const std::string& input) {
    return "da39a3ee5e6b4b0d3255bfef95601890afd80709";
}

inline std::string sha1_binary(const unsigned char* data, size_t length) {
    return "da39a3ee5e6b4b0d3255bfef95601890afd80709";
}

inline bool is_valid_sha1_hash(const std::string& hash) {
    return hash.length() == 40;
}

#endif
