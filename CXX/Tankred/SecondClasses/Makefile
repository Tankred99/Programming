# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

# Source files
SRCS = main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable
TARGET = main

# Default rule
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compiling source files
%.o: %.cpp IntArray.h
	$(CXX) $(CXXFLAGS) -c $<

# Cleaning the build files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
