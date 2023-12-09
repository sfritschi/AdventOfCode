#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_set>
#include <map>

#include <cassert>

struct Number {
    size_t indexFirst;
    size_t indexLast;  // + 1 of index belonging to last digit
    int value;
};

struct Symbol {
    size_t index;
    char kind;
};

void part1()
{
    const std::string filename("input.txt");
    
    std::ifstream file (filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    int ans = 0;
    int count = 0;
    
    std::vector<size_t> offsets(1, 0);  // initial offset is 0
    std::vector<Number> numbers;
    std::vector<Symbol> symbols;
    
    size_t offset = 0;
    size_t lineSize = 0;
    // Note: This solution assumes ALL lines have the same size
    std::string line;
    while (std::getline(file, line)) {
        lineSize = line.size();
        // Parse current line
        for (size_t i = 0; i < lineSize; ++i) {
            const char c = line[i];
            if (std::isdigit(c)) {
                size_t pos = 0;
                const int value = std::stoi(&line[i], &pos);
                // Append digit
                numbers.push_back({
                    .indexFirst = i + offset,
                    .indexLast = i + offset + pos,
                    .value = value
                });
                // Advance position by #converted characters
                assert(pos >= 1 && "Expected positive #converted characters");
                i += pos - 1;  // - 1, since is incremented immediately afterwards
            } else if (c != '.') {
                // Found a symbol (single character) at index i
                symbols.push_back({
                    .index = i + offset,
                    .kind = c
                });
            }
        }
        offsets.push_back(numbers.size());
        ++count;
        // Update index offset
        offset += lineSize;
    }
    file.close();
    
    std::cout << "#Numbers: " << numbers.size() << '\n';
    std::cout << "#Symbols: " << symbols.size() << '\n';
    std::cout << "#Offsets: " << offsets.size() << '\n';
    
    std::unordered_set<size_t> used;
    // Iterate over all symbols and sum up adjacent numbers if any
    for (const auto &symbol : symbols) {
        used.clear();  // reset found numbers
        // symbol.index = lineNum * lineSize + colNum, colNum < lineSize
        const size_t lineNum = symbol.index / lineSize;
        assert(lineNum > 0 && lineNum + 2 < offsets.size());
        // Check all adjacent fields
        // Note: No need for bounds-checking, since no symbols on border
        for (int iy = -1; iy <= 1; ++iy) {
            const int yOffset = iy * lineSize;
            for (int ix = -1; ix <= 1; ++ix) {
                const size_t nborIndex = symbol.index + ix + yOffset;
                // Find number at nbor index using linear search (potentially slow).
                // Have to only consider numbers from the adjacent three lines
                for (size_t i = offsets[lineNum - 1]; i < offsets[lineNum + 2]; ++i) {
                    const auto &number = numbers[i];
                    // Note: Need to prevent double-counting of numbers
                    //       occupying multiple adjacent positions
                    // Note: Since numbers can't overlap/touch, they ALL have unique
                    //       start and end indices
                    if (!used.contains(number.indexFirst) &&
                            number.indexFirst <= nborIndex &&
                                nborIndex < number.indexLast) {
                        // Add number to total
                        ans += number.value;
                        // Keep track of already found numbers
                        used.insert(number.indexFirst);
                        break;  // at most 1 number at any given position
                    }
                }
            }
        }
    }
    
    std::cout << "Answer: " << ans << '\n';
}

void part2()
{
    const std::string filename("input.txt");
    
    std::ifstream file (filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    int ans = 0;
    int count = 0;
    
    std::vector<size_t> offsets(1, 0);  // initial offset is 0
    std::vector<Number> numbers;
    std::vector<Symbol> symbols;
    
    size_t offset = 0;
    size_t lineSize = 0;
    // Note: This solution assumes ALL lines have the same size
    std::string line;
    while (std::getline(file, line)) {
        lineSize = line.size();
        // Parse current line
        for (size_t i = 0; i < lineSize; ++i) {
            const char c = line[i];
            if (std::isdigit(c)) {
                size_t pos = 0;
                const int value = std::stoi(&line[i], &pos);
                // Append digit
                numbers.push_back({
                    .indexFirst = i + offset,
                    .indexLast = i + offset + pos,
                    .value = value
                });
                // Advance position by #converted characters
                assert(pos >= 1 && "Expected positive #converted characters");
                i += pos - 1;  // - 1, since is incremented immediately afterwards
            } else if (c == '*') {
                // Found a gear symbol (single character) at index i
                symbols.push_back({
                    .index = i + offset,
                    .kind = c
                });
            }
        }
        offsets.push_back(numbers.size());
        ++count;
        // Update index offset
        offset += lineSize;
    }
    file.close();
    
    std::cout << "#Numbers: " << numbers.size() << '\n';
    std::cout << "#Symbols: " << symbols.size() << '\n';
    std::cout << "#Offsets: " << offsets.size() << '\n';
    
    std::map<size_t, int> used;
    // Iterate over all gears '*' and sum up product of exactly two adjacent numbers
    for (const auto &symbol : symbols) {
        used.clear();  // reset found numbers
        // symbol.index = lineNum * lineSize + colNum, colNum < lineSize
        const size_t lineNum = symbol.index / lineSize;
        assert(lineNum > 0 && lineNum + 2 < offsets.size());
        // Check all adjacent fields
        // Note: No need for bounds-checking, since no symbols on border
        for (int iy = -1; iy <= 1; ++iy) {
            const int yOffset = iy * lineSize;
            for (int ix = -1; ix <= 1; ++ix) {
                const size_t nborIndex = symbol.index + ix + yOffset;
                // Find number at nbor index using linear search (potentially slow).
                // Have to only consider numbers from the adjacent three lines
                for (size_t i = offsets[lineNum - 1]; i < offsets[lineNum + 2]; ++i) {
                    const auto &number = numbers[i];
                    // Note: Need to prevent double-counting of numbers
                    //       occupying multiple adjacent positions
                    // Note: Since numbers can't overlap/touch, they ALL have unique
                    //       start and end indices
                    if (!used.contains(number.indexFirst) &&
                            number.indexFirst <= nborIndex &&
                                nborIndex < number.indexLast) {
                        // Keep track of already found numbers
                        used[number.indexFirst] = number.value;
                        break;  // at most 1 number at any given position
                    }
                }
            }
        }
        
        if (used.size() == 2) {
            // Exactly two adjacent part numbers found -> multiply
            int gearRatio = 1;
            for (const auto &v : used) { gearRatio *= v.second; }
            ans += gearRatio;
        }
    }
    
    std::cout << "Answer: " << ans << '\n';
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <problem num.: 1/2>\n";
        return EXIT_FAILURE;
    }
    
    const int problemNum = atoi(argv[1]);
    
    try {
        if (problemNum == 1) {
            std::cout << "Running problem 1:\n";
            part1();
        } else if (problemNum == 2) {
            std::cout << "Running problem 2:\n";
            part2();
        } else {
            std::cerr << "Unrecognized problem number: " << problemNum
                    << ". Expected 1 or 2\n";
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
