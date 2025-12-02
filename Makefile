CXX = g++
CXXFLAGS = -std=c++11 -Wall
TESTS = basic auth vectors network interface sha_salt

all:
	for t in $(TESTS); do \
 		mkdir -p tests/build; \
		$(CXX) $(CXXFLAGS) tests/test_$$t.cpp -o tests/build/test_$$t; \
 		tests/build/test_$$t; \
 		echo; \
	done

clean:
	rm -rf tests/build
