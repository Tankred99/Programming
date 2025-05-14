CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
TARGET = main
SRCS = main.cpp IntArray.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = IntArray.h

# Detect OS for platform-specific commands
UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
    RM = rm -f
else ifeq ($(UNAME),Darwin)
    RM = rm -f
else ifeq ($(OS),Windows_NT)
    RM = del /Q
    TARGET := $(TARGET).exe
else
    $(error "Unsupported operating system: $(UNAME)")
endif


all: $(TARGET)

$(TARGET): $(OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
