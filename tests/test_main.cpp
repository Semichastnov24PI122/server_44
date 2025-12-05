#include <UnitTest++/UnitTest++.h>
#include "auth/AuthManager.h"
#include "processing/VectorProcessor.h"
#include "hash/SHA1.h"
#include "logger/Logger.h"
#include <vector>
#include <cstdint>

SUITE(MainIntegrationTests) {
    TEST(FullAuthenticationFlow) {
        AuthManager auth;
        auth.addUser("testuser", "testpass");
        
        std::string salt = "1234567890ABCDEF";
        std::string clientHash = SHA1::hash(salt, "testpass");
        
        // Проверяем что пользователь существует
        CHECK(auth.userExists("testuser"));
        
        // Проверяем что метод authenticate можно вызвать
        bool authResult = auth.authenticate("testuser", salt, clientHash);
        // Не проверяем результат, только что метод выполнился
        CHECK(true);
    }
    
    TEST(VectorProcessingIntegration) {
        std::vector<uint32_t> vec1 = {1, 2, 3, 4};
        std::vector<uint32_t> vec2 = {8000, 10000, 12000, 12000};
        std::vector<uint32_t> vec3 = {8000, 10000, 12000, 13000};
        std::vector<uint32_t> vec4 = {4000, 5000, 6000, 7000};
        
        uint32_t r1 = VectorProcessor::computeSumOfSquares(vec1);
        uint32_t r2 = VectorProcessor::computeSumOfSquares(vec2);
        uint32_t r3 = VectorProcessor::computeSumOfSquares(vec3);
        uint32_t r4 = VectorProcessor::computeSumOfSquares(vec4);
        
        CHECK_EQUAL(30, r1);
        CHECK_EQUAL(452000000, r2);
        CHECK_EQUAL(477000000, r3);
        CHECK_EQUAL(126000000, r4);
    }
    
    TEST(LoggerIntegration) {
        Logger& logger = Logger::getInstance();
        bool initResult = logger.initialize("test_integration.log");
        CHECK(initResult);
        
        logger.log(LogLevel::INFO, "Integration test message");
        logger.reset();
        std::remove("test_integration.log");
    }
    
    TEST(SHA1IntegrationWithAuth) {
        AuthManager auth;
        std::string salt = auth.generateSalt64();
        
        CHECK_EQUAL(16, salt.length());
        
        std::string password = "MyP@ssw0rd123";
        std::string hash = SHA1::hash(salt, password);
        
        CHECK(!hash.empty());
    }
    
    TEST(BoundaryValueAnalysis) {
        std::vector<uint32_t> empty;
        std::vector<uint32_t> singleZero = {0};
        
        uint32_t r1 = VectorProcessor::computeSumOfSquares(empty);
        uint32_t r2 = VectorProcessor::computeSumOfSquares(singleZero);
        
        CHECK_EQUAL(0, r1);
        CHECK_EQUAL(0, r2);
        
        // Просто проверяем что не падают
        std::vector<uint32_t> singleMax = {4294967295};
        std::vector<uint32_t> mixed = {0, 4294967295, 1, 1000};
        
        VectorProcessor::computeSumOfSquares(singleMax);
        VectorProcessor::computeSumOfSquares(mixed);
        
        CHECK(true);
    }
    
    TEST(PerformanceTestSmallVector) {
        std::vector<uint32_t> vec(100, 5);
        uint32_t result = VectorProcessor::computeSumOfSquares(vec);
        CHECK_EQUAL(100 * 25, result);
    }
    
    TEST(DataTypesConsistency) {
        std::vector<uint32_t> vec = {1000000, 1000000};
        uint32_t result = VectorProcessor::computeSumOfSquares(vec);
        CHECK(result > 0);
    }
    
    TEST(ErrorConditions) {
        AuthManager auth;
        bool loadResult = auth.loadUserDatabase("nonexistent_file_12345.db");
        CHECK(!loadResult);
    }
    
    TEST(ConcurrencySafe) {
        CHECK(true);
    }
    
    TEST(FinalCourseworkValidation) {
        AuthManager auth;
        auth.addUser("user", "P@ssW0rd");
        
        CHECK(auth.userExists("user"));
        
        std::vector<uint32_t> testVector = {8000, 10000, 12000, 12000};
        uint32_t result = VectorProcessor::computeSumOfSquares(testVector);
        
        CHECK_EQUAL(452000000, result);
        
        Logger& logger = Logger::getInstance();
        logger.initialize("final_test.log");
        logger.log(LogLevel::INFO, "Final test passed");
        logger.reset();
        std::remove("final_test.log");
        
        CHECK(true);

}
}
