#!/bin/bash
echo "=========================================="
echo "ЗАПУСК ВСЕХ 40 МОДУЛЬНЫХ ТЕСТОВ"
echo "=========================================="
echo ""

# Считаем общие результаты
TOTAL_TESTS=0
TOTAL_PASSED=0

echo "1. БАЗОВЫЕ ТЕСТЫ (10 тестов):"
echo "------------------------------------------"
output=$(./build/test_basic)
echo "$output"
passed=$(echo "$output" | grep -o "Пройдено: [0-9]*/10" | cut -d' ' -f2 | cut -d'/' -f1)
TOTAL_PASSED=$((TOTAL_PASSED + passed))
TOTAL_TESTS=$((TOTAL_TESTS + 10))

echo ""
echo "2. ТЕСТЫ АУТЕНТИФИКАЦИИ (10 тестов):"
echo "------------------------------------------"
output=$(./build/test_auth)
echo "$output"
passed=$(echo "$output" | grep -o "Пройдено: [0-9]*/10" | cut -d' ' -f2 | cut -d'/' -f1)
TOTAL_PASSED=$((TOTAL_PASSED + passed))
TOTAL_TESTS=$((TOTAL_TESTS + 10))

echo ""
echo "3. ТЕСТЫ ОБРАБОТКИ ВЕКТОРОВ (10 тестов):"
echo "------------------------------------------"
output=$(./build/test_vectors)
echo "$output"
passed=$(echo "$output" | grep -o "Пройдено: [0-9]*/10" | cut -d' ' -f2 | cut -d'/' -f1)
TOTAL_PASSED=$((TOTAL_PASSED + passed))
TOTAL_TESTS=$((TOTAL_TESTS + 10))

echo ""
echo "4. ТЕСТЫ СЕТЕВОГО ПРОТОКОЛА (10 тестов):"
echo "------------------------------------------"
output=$(./build/test_network)
echo "$output"
passed=$(echo "$output" | grep -o "Пройдено: [0-9]*/10" | cut -d' ' -f2 | cut -d'/' -f1)
TOTAL_PASSED=$((TOTAL_PASSED + passed))
TOTAL_TESTS=$((TOTAL_TESTS + 10))

echo ""
echo "=========================================="
echo "ИТОГОВЫЕ РЕЗУЛЬТАТЫ:"
echo "=========================================="
echo "Всего тестов: $TOTAL_TESTS"
echo "Пройдено: $TOTAL_PASSED"
echo "Провалено: $((TOTAL_TESTS - TOTAL_PASSED))"

if [ $TOTAL_PASSED -eq $TOTAL_TESTS ]; then
    echo "✅ ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО!"
else
    echo "⚠️  ЕСТЬ НЕПРОЙДЕННЫЕ ТЕСТЫ"
fi
echo "=========================================="
