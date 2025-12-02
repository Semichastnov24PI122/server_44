#ifndef VECTOR_PROCESSOR_H
#define VECTOR_PROCESSOR_H

#include <vector>
#include <cstdint>

class VectorProcessor {
public:
    uint32_t process_vector(const uint32_t* data, size_t size) { 
        uint32_t sum = 0;
        for (size_t i = 0; i < size; ++i) {
            sum += data[i] * data[i];
        }
        return sum;
    }
    
    std::vector<uint32_t> process_multiple_vectors(
        const std::vector<std::vector<uint32_t>>& vectors) {
        std::vector<uint32_t> results;
        for (const auto& vec : vectors) {
            results.push_back(process_vector(vec.data(), vec.size()));
        }
        return results;
    }
};

#endif
