#include <iostream>
#include <vector>
#include <cstdint>
#include <climits>

class VectorProcessor {
public:
    uint32_t process_single(const std::vector<uint32_t>& vec) {
        uint64_t sum = 0;
        for (uint32_t val : vec) {
            sum += static_cast<uint64_t>(val) * val;
        }
        return (sum > UINT32_MAX) ? UINT32_MAX : static_cast<uint32_t>(sum);
    }
    
    std::vector<uint32_t> process_multiple(const std::vector<std::vector<uint32_t>>& vectors) {
        std::vector<uint32_t> results;
        for (const auto& vec : vectors) {
            results.push_back(process_single(vec));
        }
        return results;
    }
};

void run_vector_tests() {
    std::cout << "\n=== ТЕСТЫ ОБРАБОТКИ ВЕКТОРОВ (10 тестов) ===\n";
    int passed = 0;
    
    VectorProcessor vp;
    
    // Тест 1: Обычный вектор
    {
        std::vector<uint32_t> vec = {1, 2, 3, 4};
        uint32_t result = vp.process_single(vec);
        bool ok = (result == 30);
        std::cout << "1. VectorProcessor обычный: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 2: Пустой вектор
    {
        std::vector<uint32_t> vec = {};
        uint32_t result = vp.process_single(vec);
        bool ok = (result == 0);
        std::cout << "2. VectorProcessor пустой: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 3: Один элемент
    {
        std::vector<uint32_t> vec = {42};
        uint32_t result = vp.process_single(vec);
        bool ok = (result == 1764);
        std::cout << "3. VectorProcessor один элемент: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 4: Большой вектор
    {
        std::vector<uint32_t> vec(1000);
        uint64_t expected = 0;
        for (int i = 0; i < 1000; i++) {
            vec[i] = i % 100;
            expected += (i % 100) * (i % 100);
        }
        uint32_t result = vp.process_single(vec);
        bool ok = (result == (uint32_t)expected);
        std::cout << "4. VectorProcessor 1000 элементов: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 5: Переполнение
    {
        std::vector<uint32_t> vec = {65536, 65536};
        uint32_t result = vp.process_single(vec);
        bool ok = (result == UINT32_MAX);
        std::cout << "5. VectorProcessor переполнение: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 6: Граница без переполнения (ИСПРАВЛЕННЫЙ)
    {
        std::vector<uint32_t> vec = {30000, 30000};
        uint32_t result = vp.process_single(vec);
        uint64_t val = 30000ULL;
        uint64_t square = val * val; // 900000000
        uint64_t expected64 = square * 2; // 1800000000
        uint32_t expected = (uint32_t)expected64; // 1800000000 < 4294967295
        bool ok = (result == expected);
        std::cout << "6. VectorProcessor граница без переполнения: " << (ok ? "✅" : "❌") 
                  << " (результат: " << result << ")" << std::endl;
        if (ok) passed++;
    }
    
    // Тест 7: Несколько векторов
    {
        std::vector<std::vector<uint32_t>> vectors = {
            {1, 2},
            {3, 4, 5},
            {10}
        };
        std::vector<uint32_t> results = vp.process_multiple(vectors);
        bool ok = (results.size() == 3) && 
                  (results[0] == 5) && 
                  (results[1] == 50) && 
                  (results[2] == 100);
        std::cout << "7. VectorProcessor несколько векторов: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 8: Все нули
    {
        std::vector<uint32_t> vec(10, 0);
        uint32_t result = vp.process_single(vec);
        bool ok = (result == 0);
        std::cout << "8. VectorProcessor все нули: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 9: Большие числа без переполнения
    {
        std::vector<uint32_t> vec = {10000, 20000};
        uint64_t expected = 10000ULL*10000 + 20000ULL*20000;
        uint32_t result = vp.process_single(vec);

bool ok = (result == (uint32_t)expected);
        std::cout << "9. VectorProcessor большие числа: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    // Тест 10: Обработка пустого списка векторов
    {
        std::vector<std::vector<uint32_t>> vectors;
        std::vector<uint32_t> results = vp.process_multiple(vectors);
        bool ok = results.empty();
        std::cout << "10. VectorProcessor пустой список векторов: " << (ok ? "✅" : "❌") << std::endl;
        if (ok) passed++;
    }
    
    std::cout << "\nПройдено: " << passed << "/10\n";
}

int main() {
    run_vector_tests();
    return 0;
}
