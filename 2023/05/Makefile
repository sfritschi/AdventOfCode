CXX=clang++
CXXFLAGS=-Wall -Wextra -Wpedantic -std=c++20 -g -gdwarf-4

TARGET=solution
.PHONY: all, clean
all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	$(RM) $(TARGET)
