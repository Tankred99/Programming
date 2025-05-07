CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = date_tester
SRCS = main.cpp Date.cpp
HEADERS = Date.h CSVParser.h DateTester.h

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

run_auto: $(TARGET)
	./$(TARGET) date_tests.csv auto

run_manual: $(TARGET)
	./$(TARGET) date_tests.csv manual