#include "processing/VectorProcessor.h"
#include <cstdint>
#include <climits>
#include <iostream>

uint32_t VectorProcessor::checkOverflow(uint64_t value) {
    if (value > UINT32_MAX) {
        return UINT32_MAX; // 2^32 - 1
    }
    return static_cast<uint32_t>(value);
}

uint32_t VectorProcessor::computeSumOfSquares(const std::vector<uint32_t>& vector) {
    uint64_t sum = 0;
    
    for (uint32_t val : vector) {
        uint64_t square = static_cast<uint64_t>(val) * val;
        // Проверяем переполнение промежуточного результата
        if (sum > UINT64_MAX - square) {
            return UINT32_MAX;
        }
        sum += square;
    }
    
    return checkOverflow(sum);
}

std::vector<uint32_t> VectorProcessor::processVectors(
    const std::vector<std::vector<uint32_t>>& vectors) {
    
    std::vector<uint32_t> results;
    results.reserve(vectors.size());
    
    for (const auto& vec : vectors) {
        results.push_back(computeSumOfSquares(vec));
    }
    
    return results;
}
