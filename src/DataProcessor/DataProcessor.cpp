#include "DataProcessor/DataProcessor.h"
#include <cstdint>

uint32_t DataProcessor::computeSumOfSquares(const uint32_t* data, uint32_t size) {
    uint64_t sum = 0;  // Используем 64-бит для избежания переполнения
    
    for (uint32_t i = 0; i < size; ++i) {
        sum += static_cast<uint64_t>(data[i]) * data[i];
        
        // Проверка переполнения (максимальное значение uint32_t)
        if (sum > 0xFFFFFFFF) {
            return 0xFFFFFFFF;  // Возвращаем максимальное значение при переполнении
        }
    }
    
    return static_cast<uint32_t>(sum);
}
