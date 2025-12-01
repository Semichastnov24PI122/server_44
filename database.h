#ifndef DATABASE_H
#define DATABASE_H

typedef struct {
    char* login;
    char* password;
} User;

typedef struct {
    User* users;
    int count;
} UserDatabase;

UserDatabase* load_database(const char* filename);
void free_database(UserDatabase* db);
char* find_password(UserDatabase* db, const char* login);

#endif
