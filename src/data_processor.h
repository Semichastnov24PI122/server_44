#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <cstdint>

class DataProcessor {
public:
    virtual ~DataProcessor() = default;
    
    virtual uint32_t compute_sum_of_squares(const uint32_t* data, uint32_t size) { 
        uint32_t sum = 0;
        for (uint32_t i = 0; i < size; ++i) {
            sum += data[i] * data[i];
        }
        return sum;
    }
    
    virtual uint32_t check_overflow(uint64_t value) { 
        return value > 4294967295ULL ? 4294967295 : (uint32_t)value; 
    }
};

#endif
