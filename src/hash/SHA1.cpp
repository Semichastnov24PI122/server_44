#include "hash/SHA1.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <algorithm>

std::string SHA1::hash(const std::string& input) {
    unsigned char digest[SHA_DIGEST_LENGTH];
    
   :: SHA1(reinterpret_cast<const unsigned char*>(input.c_str()),         input.length(),  digest);
    
    std::stringstream ss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') 
           << static_cast<int>(digest[i]);
    }
    
    std::string result = ss.str();
    // Приводим к верхнему регистру как в ТЗ
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string SHA1::hash(const std::string& salt, const std::string& password) {
    return hash(salt + password);
}
