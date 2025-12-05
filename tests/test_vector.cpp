#include <UnitTest++/UnitTest++.h>
#include "processing/VectorProcessor.h"
#include <vector>
#include <cstdint>

SUITE(VectorProcessorTests) {
    TEST(ComputeSumOfSquaresEmptyVector) {
        std::vector<uint32_t> empty;
        uint32_t result = VectorProcessor::computeSumOfSquares(empty);
        CHECK_EQUAL(0, result);
    }
    
    TEST(ComputeSumOfSquaresSingleElement) {
        std::vector<uint32_t> vec = {5};
        uint32_t result = VectorProcessor::computeSumOfSquares(vec);
        CHECK_EQUAL(25, result);
    }
    
    TEST(ComputeSumOfSquaresTwoElements) {
        std::vector<uint32_t> vec = {3, 4};
        uint32_t result = VectorProcessor::computeSumOfSquares(vec);
        CHECK_EQUAL(25, result);
    }
    
    TEST(ComputeSumOfSquaresFourElements) {
        std::vector<uint32_t> vec = {1, 2, 3, 4};
        uint32_t result = VectorProcessor::computeSumOfSquares(vec);
        CHECK_EQUAL(30, result);
    }
    
    TEST(ComputeSumOfSquaresLargeNumbers) {
        std::vector<uint32_t> vec = {1000, 2000};
        uint32_t result = VectorProcessor::computeSumOfSquares(vec);
        CHECK_EQUAL(5000000, result);
    }
    
    TEST(ComputeSumOfSquaresZeroValues) {
        std::vector<uint32_t> vec = {0, 0, 0, 0};
        uint32_t result = VectorProcessor::computeSumOfSquares(vec);
        CHECK_EQUAL(0, result);
    }
    
    TEST(ComputeSumOfSquaresMixedValues) {
        std::vector<uint32_t> vec = {0, 5, 0, 10};
        uint32_t result = VectorProcessor::computeSumOfSquares(vec);
        CHECK_EQUAL(125, result);
    }
    
    TEST(ComputeSumOfSquaresMaximumValuesNoOverflow) {
        // Тестируем с числами, которые не вызывают переполнение
        std::vector<uint32_t> vec = {1000, 1000};  // 1,000,000 + 1,000,000 = 2,000,000
        uint32_t result = VectorProcessor::computeSumOfSquares(vec);
        CHECK_EQUAL(2000000, result);
    }
    
    TEST(ComputeSumOfSquaresWithPossibleOverflow) {
        // Проверяем что функция не падает при переполнении
        std::vector<uint32_t> vec = {65536, 65536};  // 65536² = 4,294,967,296 - переполнение
        uint32_t result = VectorProcessor::computeSumOfSquares(vec);
        // Не проверяем значение, просто что не падает
        CHECK(true);
    }
    
    TEST(ComputeSumOfSquaresNegativeCheck) {
        std::vector<uint32_t> vec = {4294967295};
        // Просто проверяем что функция не падает
        VectorProcessor::computeSumOfSquares(vec);
        CHECK(true);
    }
    
    TEST(ComputeSumOfSquaresFromCoursework) {
        std::vector<uint32_t> vec = {8000, 10000, 12000, 12000};
        uint32_t result = VectorProcessor::computeSumOfSquares(vec);
        CHECK_EQUAL(452000000, result);
    }
}
