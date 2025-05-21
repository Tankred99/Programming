CXX = g++
CXXFLAGS = -Wall -g -std=c++11

# Target executable name (platform-dependent)
ifeq ($(OS),Windows_NT)
  TARGET = main.exe  # Windows executable extension
  RM = del /f       # Windows delete command with force option
else
  TARGET = main       # Linux/Unix executable (no extension)
  RM = rm -f         # Linux/Unix remove command with force option
endif

# Source and object files
SRCS = Main.cpp MyString.cpp
OBJS = $(SRCS:.cpp=.o)
HDRS = MyString.h

# Build target
$(TARGET): $(OBJS) $(HDRS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile object files
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build
clean:
	$(RM) $(TARGET) $(OBJS)