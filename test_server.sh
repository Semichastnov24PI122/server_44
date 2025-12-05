#!/bin/bash
echo "=== ЗАПУСК СЕРВЕРА ==="
make clean
make
echo "user:P@ssW0rd" > users.db
echo "Сервер запускается..."
echo "Открой второй терминал и выполни:"
echo "cd ~/klient && ./client_uint32_t -H SHA1 -S c"
echo ""
echo "Нажми Enter для продолжения..."
read
./server --config users.db --log server.log --port 33333
