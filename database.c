#include "database.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Функция для копирования строки (аналог strdup)
char* my_strdup(const char* s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s) + 1;
    char* new_str = malloc(len);
    if (new_str) {
        memcpy(new_str, s, len);
    }
    return new_str;
}

UserDatabase* load_database(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }
    
    UserDatabase* db = malloc(sizeof(UserDatabase));
    db->users = NULL;
    db->count = 0;
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        
        char* colon = strchr(line, ':');
        if (!colon) continue;
        
        *colon = '\0';
        char* login = line;
        char* password = colon + 1;
        
        db->users = realloc(db->users, (db->count + 1) * sizeof(User));
        db->users[db->count].login = my_strdup(login);
        db->users[db->count].password = my_strdup(password);
        db->count++;
    }
    
    fclose(file);
    return db;
}

void free_database(UserDatabase* db) {
    if (!db) return;
    
    for (int i = 0; i < db->count; i++) {
        free(db->users[i].login);
        free(db->users[i].password);
    }
    free(db->users);
    free(db);
}

char* find_password(UserDatabase* db, const char* login) {
    if (!db) return NULL;
    
    for (int i = 0; i < db->count; i++) {
        if (strcmp(db->users[i].login, login) == 0) {
            return db->users[i].password;
        }
    }
    return NULL;
}

