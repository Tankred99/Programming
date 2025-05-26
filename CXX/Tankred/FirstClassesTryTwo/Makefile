CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

all: date_test

date_test: main.o date.o
	$(CXX) $(CXXFLAGS) -o date_test main.o date.o

main.o: main.cpp date.h
	$(CXX) $(CXXFLAGS) -c main.cpp

date.o: date.cpp date.h
	$(CXX) $(CXXFLAGS) -c date.cpp

clean:
	rm -f *.o date_test

.PHONY: clean