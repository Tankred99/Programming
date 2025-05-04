CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -D_UNICODE -DUNICODE
TARGET = date_program
OBJS = Date.o main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

Date.o: Date.cpp Date.h
	$(CXX) $(CXXFLAGS) -c Date.cpp

main.o: main.cpp Date.h
	$(CXX) $(CXXFLAGS) -c main.cpp

clean:
ifeq ($(OS),Windows_NT)
	del $(OBJS) $(TARGET).exe
else
	rm -f $(OBJS) $(TARGET)
endif

.PHONY: all clean