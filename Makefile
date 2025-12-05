CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include
LDFLAGS = -pthread -lssl -lcrypto  # Для SHA1 из OpenSSL

# Для тестов
TEST_CXXFLAGS = $(CXXFLAGS)
TEST_LDFLAGS = $(LDFLAGS) -lunittest++

# Исходные файлы сервера
SRC_DIR = src
SRC_FILES = \
    $(SRC_DIR)/auth/AuthManager.cpp \
    $(SRC_DIR)/database.cpp \
    $(SRC_DIR)/hash/SHA1.cpp \
    $(SRC_DIR)/logger.cpp \
    $(SRC_DIR)/processing/VectorProcessor.cpp \
    $(SRC_DIR)/server.cpp \
    $(SRC_DIR)/utils/Interface.cpp \
    $(SRC_DIR)/main.cpp

OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Тесты
TEST_DIR = tests
TEST_FILES = \
    $(TEST_DIR)/test_auth_unit.cpp \
    $(TEST_DIR)/test_vector_unit.cpp \
    $(TEST_DIR)/test_interface.cpp \
    $(TEST_DIR)/test_network.cpp \
    $(TEST_DIR)/test_sha_salt.cpp \
    $(TEST_DIR)/test_main.cpp

TEST_OBJ_FILES = $(TEST_FILES:.cpp=.o)

# Цели
TARGET = server
TEST_TARGET = run_tests

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

tests: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ_FILES) $(filter-out $(SRC_DIR)/main.o, $(OBJ_FILES))
	$(CXX) $^ -o $@ $(TEST_LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	 	rm -f $(OBJ_FILES) $(TEST_OBJ_FILES) $(TARGET) $(TEST_TARGET)
		rm -f *.log *.db 

test: tests
		./$(TEST_TARGET)

.PHONY: all tests clean test

