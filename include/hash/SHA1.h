/**
 * @namespace SHA1
 * @brief Функции хеширования SHA-1.
 *
 * Вычисление хеша от строки и преобразование в шестнадцатеричный формат.
 * Используется для аутентификации клиентов.
 */
#ifndef SHA1_H
#define SHA1_H

#include <string>

class SHA1 {
public:
    static std::string hash(const std::string& input);
    static std::string hash(const std::string& salt, const std::string& password);
};

#endif
