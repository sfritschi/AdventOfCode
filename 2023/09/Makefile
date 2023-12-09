CXX=clang++
CXXFLAGS=-Wall -Wextra -Wpedantic -Werror -std=c++20 -g -gdwarf-4
#CXXFLAGS=-Wall -Wextra -Wpedantic -Werror -std=c++20 -O3 -march=native -DNDEBUG

TARGET=solution
.PHONY: all, clean
all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	$(RM) $(TARGET)
