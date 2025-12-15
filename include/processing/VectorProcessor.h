/**
 * @class VectorProcessor
 * @brief Обработчик числовых векторов.
 *
 * Вычисляет сумму квадратов элементов вектора.
 * Обрабатывает переполнение (возвращает UINT32_MAX при переполнении).
 */
#ifndef VECTORPROCESSOR_H
#define VECTORPROCESSOR_H

#include <vector>
#include <cstdint>

class VectorProcessor {
public:
    // Сумма квадратов (по ТЗ)
    static uint32_t computeSumOfSquares(const std::vector<uint32_t>& vector);
    
    // Обработка нескольких векторов
    static std::vector<uint32_t> processVectors(
        const std::vector<std::vector<uint32_t>>& vectors);
    
private:
    static uint32_t checkOverflow(uint64_t value);
};

#endif
