#ifndef SHA1_H
#define SHA1_H

#include <string>

class SHA1 {
public:
    static std::string hash(const std::string& input);
    static std::string hash(const std::string& salt, const std::string& password);
};

#endif
