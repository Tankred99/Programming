CXX = g++
CXXFLAGS = -Wall -g -std=c++11
TARGET = Word.exe

# Source files
SRCS = Word.cpp

# Object files (автоматически генерируются)
OBJS = $(SRCS:.cpp=.o)

# Default rule:  build the executable
all: $(TARGET)

# Rule to link the object files into the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile a .cpp file into a .o file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean up the object files and executable
clean:
	del /f $(OBJS) $(TARGET)