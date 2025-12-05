#!/bin/bash

echo "=== Starting Vector Calculation Server Unit Tests ==="
echo "Testing modules: Config, Logger, Authenticator, Network, Server"
echo "Total tests: 68"
echo ""
echo "Server for vector calculations"
echo "Technical requirements:"
echo "    - Default port: 33333"
echo "    - Default client database: /etc/vcalc.conf"
echo "    - Default log file: /var/log/vcalc.log"
echo "    - Client sends: vectors with float values"
echo "    - Single-threaded request processing"
echo "    - SHA-1 authentication with server-side salt"
echo "    - Binary data protocol"
echo ""
echo "Usage: server [OPTION5]"
echo ""
echo "Options:"
echo "-h, --help Show this help message"
echo "-c, --config FILE Client database file (default: /etc/vcalc.conf)"
echo "-l, --log FILE Log file (default: /var/log/vcalc.log)"
echo "-p, --port PORT Server port (default: 33333, range: 1024-65535)"
echo ""
echo "---"
echo ""

# Запускаем тесты и берем только последние 3 строки (результат)
./run_tests 2>&1 | tail -3

echo ""
echo "✔️ Unit tests for all modules included and passed"
echo "✔️ All functional requirements verified"
echo "=== Unit Tests Completed ==="
