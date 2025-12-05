#include <UnitTest++/UnitTest++.h>
#include "logger/Logger.h"
#include <fstream>
#include <cstdio>

SUITE(LoggerTests) {
    TEST(LoggerSingletonInstance) {
        Logger& logger1 = Logger::getInstance();
        Logger& logger2 = Logger::getInstance();
        CHECK(&logger1 == &logger2);
    }
    
    TEST(LoggerInitialization) {
        std::string filename = "test_logger_init.log";
        std::remove(filename.c_str());
        
        Logger& logger = Logger::getInstance();
        bool result = logger.initialize(filename);
        
        CHECK(result);
        
        std::ifstream file(filename);
        bool fileExists = file.is_open();
        file.close();
        
        CHECK(fileExists);
        logger.reset();
        std::remove(filename.c_str());
    }
    
    TEST(LoggerWriteMessage) {
        std::string filename = "test_logger_write.log";
        std::remove(filename.c_str());
        
        Logger& logger = Logger::getInstance();
        logger.initialize(filename);
        logger.log(LogLevel::INFO, "Test message");
        
        std::ifstream file(filename);
        std::string content;
        std::getline(file, content);
        file.close();
        
        CHECK(!content.empty());
        logger.reset();
        std::remove(filename.c_str());
    }
    
    TEST(LoggerWriteDifferentLevels) {
        std::string filename = "test_logger_levels.log";
        std::remove(filename.c_str());
        
        Logger& logger = Logger::getInstance();
        logger.initialize(filename);
        logger.log(LogLevel::INFO, "Info message");
        logger.log(LogLevel::ERROR, "Error message");
        
        std::ifstream file(filename);
        std::string line;
        int lineCount = 0;
        while (std::getline(file, line)) {
            lineCount++;
        }
        file.close();
        
        CHECK(lineCount >= 3); // Заголовок + 2 сообщения
        logger.reset();
        std::remove(filename.c_str());
    }
    
    TEST(LoggerReset) {
        std::string filename = "test_logger_reset.log";
        std::remove(filename.c_str());
        
        Logger& logger = Logger::getInstance();
        logger.initialize(filename);
        logger.reset();
        
        // После reset можно снова инициализировать
        bool result = logger.initialize(filename);
        CHECK(result);
        
        logger.reset();
        std::remove(filename.c_str());
    }
}
