#include <UnitTest++/UnitTest++.h>
#include "utils/Interface.h"
#include <cstring>
#include <fstream>
#include <cstdio>

// Простая функция для создания аргументов
char** createArgs(int argc, const char* argv[]) {
    char** args = new char*[argc + 1];
    for (int i = 0; i < argc; i++) {
        args[i] = new char[strlen(argv[i]) + 1];
        strcpy(args[i], argv[i]);
    }
    args[argc] = nullptr;
    return args;
}

void cleanupArgs(char** args, int argc) {
    for (int i = 0; i < argc; i++) {
        delete[] args[i];
    }
    delete[] args;
}

SUITE(InterfaceTests) {
    // === РАБОЧИЕ ТЕСТЫ (18 штук) ===
    
    TEST(InterfaceCreation) {
        Interface interface;
        CHECK(true);
    }
    
    TEST(InterfaceHelpShort) {
        Interface interface;
        const char* argv[] = {"program", "-h"};
        char** args = createArgs(2, argv);
        
        bool result = interface.parse(2, args);
        // Должен вернуть false при --help
        CHECK(!result);
        
        cleanupArgs(args, 2);
    }
    
    TEST(InterfaceShortParameters) {
        Interface interface;
        const char* argv[] = {
            "program",
            "-c", "short.conf",
            "-l", "short.log",
            "-p", "55555"
        };
        char** args = createArgs(7, argv);
        
        std::ofstream config("short.conf");
        config << "user:pass\n";
        config.close();
        
        std::ofstream log("short.log");
        log.close();
        
        bool result = interface.parse(7, args);
        CHECK(result);
        
        Params params = interface.getParams();
        CHECK_EQUAL("short.conf", params.dbFile);
        CHECK_EQUAL("short.log", params.logFile);
        CHECK_EQUAL(55555, params.port);
        
        cleanupArgs(args, 7);
        remove("short.conf");
        remove("short.log");
    }
    
    TEST(InterfaceInvalidPortZero) {
        Interface interface;
        const char* argv[] = {
            "program",
            "-c", "test.conf",
            "-l", "test.log",
            "-p", "0"
        };
        char** args = createArgs(7, argv);
        
        std::ofstream config("test.conf");
        config << "user:pass\n";
        config.close();
        
        std::ofstream log("test.log");
        log.close();
        
        bool result = interface.parse(7, args);
        CHECK(!result);
        
        cleanupArgs(args, 7);
        remove("test.conf");
        remove("test.log");
    }
    
    TEST(InterfaceInvalidPortTooLarge) {
        Interface interface;
        const char* argv[] = {
            "program",
            "-c", "test.conf",
            "-l", "test.log",
            "-p", "70000"
        };
        char** args = createArgs(7, argv);
        
        std::ofstream config("test.conf");
        config << "user:pass\n";
        config.close();
        
        std::ofstream log("test.log");
        log.close();
        
        bool result = interface.parse(7, args);
        CHECK(!result);
        
        cleanupArgs(args, 7);
        remove("test.conf");
        remove("test.log");
    }
    
    TEST(InterfaceMissingArgument) {
        Interface interface;
        const char* argv[] = {"program", "-c"};
        char** args = createArgs(2, argv);
        
        bool result = interface.parse(2, args);
        CHECK(!result);
        
        cleanupArgs(args, 2);
    }
    
    TEST(InterfaceMixedParameters) {
        Interface interface;
        const char* argv[] = {
            "program",
            "--config", "mixed.conf",
            "-l", "mixed.log",
            "--port", "33333"
        };
        char** args = createArgs(7, argv);
        
        std::ofstream config("mixed.conf");
        config << "user:pass\n";
        config.close();
        
        std::ofstream log("mixed.log");
        log.close();
        
        bool result = interface.parse(7, args);
        CHECK(result);
        
        Params params = interface.getParams();
        CHECK_EQUAL("mixed.conf", params.dbFile);

CHECK_EQUAL("mixed.log", params.logFile);
        CHECK_EQUAL(33333, params.port);
        
        cleanupArgs(args, 7);
        remove("mixed.conf");
        remove("mixed.log");
    }
    
    TEST(InterfaceVersionOption) {
        Interface interface;
        const char* argv[] = {"program", "-v"};
        char** args = createArgs(2, argv);
        
        bool result = interface.parse(2, args);
        // Должен вернуть false при --version
        CHECK(!result);
        
        cleanupArgs(args, 2);
    }
    
    TEST(InterfacePortMinimumValid) {
        Interface interface;
        const char* argv[] = {
            "program",
            "-c", "port.conf",
            "-l", "port.log",
            "-p", "1024"
        };
        char** args = createArgs(7, argv);
        
        std::ofstream config("port.conf");
        config << "user:pass\n";
        config.close();
        
        std::ofstream log("port.log");
        log.close();
        
        bool result = interface.parse(7, args);
        CHECK(result);
        
        cleanupArgs(args, 7);
        remove("port.conf");
        remove("port.log");
    }
    
    TEST(InterfacePortMaximumValid) {
        Interface interface;
        const char* argv[] = {
            "program",
            "-c", "port.conf",
            "-l", "port.log",
            "-p", "65535"
        };
        char** args = createArgs(7, argv);
        
        std::ofstream config("port.conf");
        config << "user:pass\n";
        config.close();
        
        std::ofstream log("port.log");
        log.close();
        
        bool result = interface.parse(7, args);
        CHECK(result);
        
        cleanupArgs(args, 7);
        remove("port.conf");
        remove("port.log");
    }
    
    TEST(InterfacePortBelowMinimum) {
        Interface interface;
        const char* argv[] = {
            "program",
            "-c", "port.conf",
            "-l", "port.log",
            "-p", "1023"
        };
        char** args = createArgs(7, argv);
        
        std::ofstream config("port.conf");
        config << "user:pass\n";
        config.close();
        
        std::ofstream log("port.log");
        log.close();
        
        bool result = interface.parse(7, args);
        CHECK(!result);
        
        cleanupArgs(args, 7);
        remove("port.conf");
        remove("port.log");
    }
    
    TEST(InterfacePortAboveMaximum) {
        Interface interface;
        const char* argv[] = {
            "program",
            "-c", "port.conf",
            "-l", "port.log",
            "-p", "65536"
        };
        char** args = createArgs(7, argv);
        
        std::ofstream config("port.conf");
        config << "user:pass\n";
        config.close();
        
        std::ofstream log("port.log");
        log.close();
        
        bool result = interface.parse(7, args);
        CHECK(!result);
        
        cleanupArgs(args, 7);
        remove("port.conf");
        remove("port.log");
    }
    
    TEST(InterfaceConfigFileWarning) {
        Interface interface;
        const char* argv[] = {
            "program",
            "-c", "nonexistent.conf",
            "-l", "test.log",
            "-p", "30000"
        };
        char** args = createArgs(7, argv);
        
        std::ofstream log("test.log");
        log.close();
        
        bool result = interface.parse(7, args);
        // Должен вернуть true с предупреждением
        CHECK(result);
        
        cleanupArgs(args, 7);
        remove("test.log");
    }
    
    TEST(InterfaceOptionOrder) {
        Interface interface;
        const char* argv[] = {
            "program",
            "-p", "40000",
            "-l", "order.log",
            "-c", "order.conf"
        };
        char** args = createArgs(7, argv);
        
        std::ofstream config("order.conf");
        config << "user:pass\n";
        config.close();
        
        std::ofstream log("order.log");
        log.close();
        
        bool result = interface.parse(7, args);
        CHECK(result);

cleanupArgs(args, 7);
        remove("order.conf");
        remove("order.log");
    }
    
    TEST(InterfaceDuplicateOptions) {
        Interface interface;
        const char* argv[] = {
            "program",
            "-c", "first.conf",
            "-c", "second.conf",
            "-l", "test.log",
            "-p", "30000"
        };
        char** args = createArgs(9, argv);
        
        std::ofstream config1("first.conf");
        config1 << "user:pass\n";
        config1.close();
        
        std::ofstream config2("second.conf");
        config2 << "user:pass\n";
        config2.close();
        
        std::ofstream log("test.log");
        log.close();
        
        bool result = interface.parse(9, args);
        CHECK(result);
        
        Params params = interface.getParams();
        // Последнее значение должно перезаписать первое
        CHECK_EQUAL("second.conf", params.dbFile);
        
        cleanupArgs(args, 9);
        remove("first.conf");
        remove("second.conf");
        remove("test.log");
    }
    
    TEST(InterfaceEmptyValues) {
        Interface interface;
        const char* argv[] = {
            "program",
            "-c", "",
            "-l", "",
            "-p", "30000"
        };
        char** args = createArgs(7, argv);
        
        bool result = interface.parse(7, args);
        CHECK(!result);
        
        cleanupArgs(args, 7);
    }
    
    TEST(InterfaceNoParametersShowsHelp) {
        Interface interface;
        const char* argv[] = {"program"};
        char** args = createArgs(1, argv);
        
        bool result = interface.parse(1, args);
        // Без параметров должен показать справку
        CHECK(!result);
        
        cleanupArgs(args, 1);
    }
    
    TEST(InterfaceUnknownOption) {
        Interface interface;
        const char* argv[] = {"program", "-x"};
        char** args = createArgs(2, argv);
        
        bool result = interface.parse(2, args);
        CHECK(!result);
        
        cleanupArgs(args, 2);
    }
    
    TEST(InterfaceLogFileNotWritable) {
        Interface interface;
        const char* argv[] = {
            "program",
            "-c", "test.conf",
            "-l", "/root/test.log",  // Нельзя писать в /root
            "-p", "30000"
        };
        char** args = createArgs(7, argv);
        
        std::ofstream config("test.conf");
        config << "user:pass\n";
        config.close();
        
        bool result = interface.parse(7, args);
        // Нельзя писать в /root - должно вернуть ошибку
        CHECK(!result);
        
        cleanupArgs(args, 7);
        remove("test.conf");
    }
}
