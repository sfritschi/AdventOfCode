#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <numeric>

#include <cassert>
#include <cstdio>
#include <cmath>

void part1()
{
    int64_t ans = 0;
    
    const std::string filename("input.txt");
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    std::vector<int64_t> sequence;
    std::string line;
    while (std::getline(file, line)) {
        // Reset sequence
        sequence.clear();
        for (size_t i = 0, pos = 0; i < line.size(); i += pos) {
            sequence.push_back(std::stoll(&line[i], &pos));
        }
        
        ans += sequence.back();
        for (size_t i = 0; i < sequence.size(); ++i) {
            // Compute differences between consecutive elements
            bool allZeros = true;
            size_t j = 0;
            for (; j < sequence.size() - i - 1; ++j) {
                const int64_t diff = sequence[j+1] - sequence[j];
                sequence[j] = diff;
                allZeros = allZeros && diff == 0;
            }
            
            if (allZeros) { 
                break;  // done
            }
            assert(j >= 1);
            // Add last element of current sub-sequence
            ans += sequence[j-1];
        }
    }
    file.close();
    
    std::cout << "Answer: " << ans << '\n';
}

void part2()
{
    int64_t ans = 0;
    
    const std::string filename("input.txt");
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    std::vector<int64_t> sequence;
    std::string line;
    while (std::getline(file, line)) {
        // Reset sequence
        sequence.clear();
        for (size_t i = 0, pos = 0; i < line.size(); i += pos) {
            sequence.push_back(std::stoll(&line[i], &pos));
        }
        
        for (size_t i = 0; i < sequence.size(); ++i) {
            const int64_t first = sequence.front();
            // Alternate sign of first values of sub-sequences 
            if (i % 2 == 0) { ans += first; } 
            else            { ans -= first; }
            // Compute differences between consecutive elements
            bool allZeros = true;
            for (size_t j = 0; j < sequence.size() - i - 1; ++j) {
                const int64_t diff = sequence[j+1] - sequence[j];
                sequence[j] = diff;
                allZeros = allZeros && diff == 0;
            }
            
            if (allZeros) {
                break;  // done
            }
        }
    }
    file.close();
    
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
