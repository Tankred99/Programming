CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = date_tester
SRCS = main.cpp Date.cpp DateTester.cpp
HEADERS = Date.h CSVParser.h DateTester.h

# Detect OS for platform-specific commands
ifeq ($(OS),Windows_NT)
    RM = del /Q
    TARGET := $(TARGET).exe
else
    RM = rm -f
endif

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	$(RM) $(TARGET)

run: $(TARGET)
	./$(TARGET)

run_auto: $(TARGET)
	./$(TARGET) date_tests.csv auto

run_manual: $(TARGET)
	./$(TARGET) date_tests.csv manual
