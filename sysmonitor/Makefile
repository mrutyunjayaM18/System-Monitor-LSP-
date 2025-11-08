# Makefile
CXX = g++
CXXFLAGS = -std=c++17 -O2
LDFLAGS = -lncurses
TARGET = monitor
SRC = monitor.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

