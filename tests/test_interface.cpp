#include <UnitTest++/UnitTest++.h>
#include "utils/Interface.h"
#include <cstring>

SUITE(InterfaceTests) {
    TEST(InterfaceCreation) {
        Interface interface;
        CHECK(true); // Просто проверяем что создаётся
    }
    
    TEST(InterfaceDefaultParameters) {
        Interface interface;
        char* args[] = {(char*)"program"};
        bool result = interface.parse(1, args);
        
        CHECK(result);
        
        Params params = interface.getParams();
        CHECK_EQUAL("/etc/vcalc.conf", params.dbFile);
        CHECK_EQUAL("/var/log/vcalc.log", params.logFile);
        CHECK_EQUAL(33333, params.port);
    }
    
    TEST(InterfaceHelpShort) {
        Interface interface;
        char* args[] = {(char*)"program", (char*)"-h"};
        bool result = interface.parse(2, args);
        
        CHECK(!result);
        CHECK(interface.isHelpRequested());
    }
    
    TEST(InterfaceHelpLong) {
        Interface interface;
        char* args[] = {(char*)"program", (char*)"--help"};
        bool result = interface.parse(2, args);
        
        CHECK(!result);
        CHECK(interface.isHelpRequested());
    }
    
    TEST(InterfaceValidCustomParameters) {
        Interface interface;
        char* args[] = {
            (char*)"program",
            (char*)"--config", (char*)"test.conf",
            (char*)"--log", (char*)"test.log",
            (char*)"--port", (char*)"30000"
        };
        
        bool result = interface.parse(7, args);
        CHECK(result);
        
        Params params = interface.getParams();
        CHECK_EQUAL("test.conf", params.dbFile);
        CHECK_EQUAL("test.log", params.logFile);
        CHECK_EQUAL(30000, params.port);
    }
    
    TEST(InterfaceShortParameters) {
        Interface interface;
        char* args[] = {
            (char*)"program",
            (char*)"-c", (char*)"short.conf",
            (char*)"-l", (char*)"short.log",
            (char*)"-p", (char*)"55555"
        };
        
        bool result = interface.parse(7, args);
        CHECK(result);
        
        Params params = interface.getParams();
        CHECK_EQUAL("short.conf", params.dbFile);
        CHECK_EQUAL("short.log", params.logFile);
        CHECK_EQUAL(55555, params.port);
    }
    
    TEST(InterfaceInvalidPortZero) {
        Interface interface;
        char* args[] = {
            (char*)"program",
            (char*)"-c", (char*)"test.conf",
            (char*)"-l", (char*)"test.log",
            (char*)"-p", (char*)"0"
        };
        
        bool result = interface.parse(7, args);
        CHECK(!result);
    }
    
    TEST(InterfaceInvalidPortTooLarge) {
        Interface interface;
        char* args[] = {
            (char*)"program",
            (char*)"-c", (char*)"test.conf",
            (char*)"-l", (char*)"test.log",
            (char*)"-p", (char*)"70000"
        };
        
        bool result = interface.parse(7, args);
        CHECK(!result);
    }
    
    TEST(InterfaceMissingArgument) {
        Interface interface;
        char* args[] = {
            (char*)"program",
            (char*)"-c" // Нет значения
        };
        
        bool result = interface.parse(2, args);
        CHECK(!result);
    }
    
    TEST(InterfaceMixedParameters) {
        Interface interface;
        char* args[] = {
            (char*)"program",
            (char*)"--config", (char*)"mixed.conf",
            (char*)"-l", (char*)"mixed.log",
            (char*)"--port", (char*)"33333"
        };
        
        bool result = interface.parse(7, args);
        CHECK(result);
        
        Params params = interface.getParams();
        CHECK_EQUAL("mixed.conf", params.dbFile);
        CHECK_EQUAL("mixed.log", params.logFile);
        CHECK_EQUAL(33333, params.port);
    }
}
