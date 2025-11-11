CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Iinclude  
TARGET = server

all:
	    $(CXX) $(CXXFLAGS) src/main.cpp src/Server/Server.cpp src/ClientSession/ClientSession.cpp src/UserDatabase/UserDatabase.cpp src/Logger/Logger.cpp src/DataProcessor/DataProcessor.cpp -o $(TARGET)

clean:
	    rm -f $(TARGET)

.PHONY: all clean
