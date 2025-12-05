CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include
LDFLAGS = -pthread

TEST_LIBS = -lUnitTest++

SRC_DIR = src
SRC_FILES = \
    $(SRC_DIR)/auth/AuthManager.cpp \
    $(SRC_DIR)/hash/SHA1.cpp \
    $(SRC_DIR)/logger.cpp \
    $(SRC_DIR)/processing/VectorProcessor.cpp \
    $(SRC_DIR)/server.cpp \
    $(SRC_DIR)/utils/Interface.cpp \
    $(SRC_DIR)/main.cpp

OBJ_FILES = $(SRC_FILES:.cpp=.o)

TEST_DIR = tests
TEST_FILES = \
    $(TEST_DIR)/test_auth.cpp \
    $(TEST_DIR)/test_vector.cpp \
    $(TEST_DIR)/test_logger.cpp \
    $(TEST_DIR)/test_interface.cpp \
    $(TEST_DIR)/test_sha.cpp \
    $(TEST_DIR)/test_all_network.cpp \
    $(TEST_DIR)/test_main.cpp

TEST_OBJ_FILES = $(TEST_FILES:.cpp=.o)
TEST_RUNNER = $(TEST_DIR)/test_main_runner.cpp
TEST_RUNNER_OBJ = $(TEST_DIR)/test_main_runner.o

TARGET = server
TEST_TARGET = run_tests

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

tests: $(TEST_TARGET)

# ВАЖНО: Добавляем test_main_runner.o к линковке
$(TEST_TARGET): $(TEST_OBJ_FILES) $(TEST_RUNNER_OBJ) $(filter-out $(SRC_DIR)/main.o, $(OBJ_FILES))
	$(CXX) $^ -o $@ $(LDFLAGS) $(TEST_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(TEST_OBJ_FILES) $(TEST_RUNNER_OBJ) $(TARGET) $(TEST_TARGET)
	rm -f *.log *.db

test: tests
	./$(TEST_TARGET)

.PHONY: all tests clean test
