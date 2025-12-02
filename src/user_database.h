#ifndef USER_DATABASE_H
#define USER_DATABASE_H

#include <string>

class UserDatabase {
public:
    virtual ~UserDatabase() = default;
    
    virtual bool load_from_file(const std::string& file_path) { return true; }
    
    // Упрощенная сигнатура
    virtual bool verify_password(const std::string& login, const std::string& hash) { 
        return true; 
    }
    
    virtual size_t get_user_count() const { return 0; }
    
    virtual void clear() {}
};

#endif
