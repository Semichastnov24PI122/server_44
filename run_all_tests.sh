EOF
#!/bin/bash
echo "=== ПОЛНОЕ ФУНКЦИОНАЛЬНОЕ ТЕСТИРОВАНИЕ ==="
echo ""

# Подготовка файлов
echo "testuser:testpassword" > my_vcalc.conf
echo "admin:admin123" >> my_vcalc.conf
touch test1.log test2.log app.log custom.log

# Функция для теста
run_test() {
    local test_num=$1
    local test_name=$2
    local command=$3
    local expected=$4
    
    echo "========================================"
    echo "ТЕСТ $test_num: $test_name"
    echo "Команда: $command"
    echo "Ожидаем: $expected"
    echo "--- РЕЗУЛЬТАТ ---"
    eval "$command" 2>&1
    echo "--- КОНЕЦ ТЕСТА ---"
    echo ""
}

# Тесты
run_test "1/15" "Запуск без параметров" "./server" "Ошибка доступа к файлу журнала"
run_test "2/15" "Справка (-h)" "./server -h" "Вывод справки"
run_test "3/15" "Справка (--help)" "./server --help" "Вывод справки"
run_test "4/15" "Валидный порт 1024" "./server -c my_vcalc.conf -l test1.log -p 1024" "Сервер запускается"
run_test "5/15" "Невалидный порт 0" "./server -c my_vcalc.conf -l test1.log -p 0" "Ошибка валидации"
run_test "6/15" "Системный порт 80" "./server -c my_vcalc.conf -l test1.log -p 80" "Ошибка валидации"
run_test "7/15" "Порт >65535" "./server -c my_vcalc.conf -l test1.log -p 70000" "Ошибка валидации"
run_test "8/15" "Версия программы" "./server -v" "Вывод версии"
run_test "9/15" "Кастомный конфиг" "./server -c my_vcalc.conf -l test1.log -p 20000" "Использование my_vcalc.conf"
run_test "10/15" "Кастомный лог" "./server -c my_vcalc.conf -l custom.log -p 21000" "Логи в custom.log"
run_test "11/15" "Все параметры" "./server --config my_vcalc.conf --log app.log --port 22000" "Все параметры приняты"
run_test "12/15" "Разный порядок" "./server -l test2.log -p 23000 -c my_vcalc.conf" "Аргументы в любом порядке"
run_test "13/15" "Неизвестный аргумент" "./server --unknown" "Ошибка: неизвестная опция"
run_test "14/15" "Нет значения" "./server -p" "Ошибка: нет значения"
run_test "15/15" "Дублирование опций" "./server -p 24000 -p 25000 -c my_vcalc.conf -l test1.log" "Используется последний порт (25000)"

echo "========================================"
echo "ТЕСТИРОВАНИЕ ЗАВЕРШЕНО!"
echo "=========================================="
EOF

chmod +x run_all_tests.sh
./run_all_tests.sh
