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
    int ans = 0;
    
    const std::string filename("sample.txt");
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    std::vector<uint32_t> unknownPos;
    std::vector<uint32_t> damagedCounts;
    std::string springs, counts;
    while (file >> springs >> counts) {
        assert(springs.size() < 32);
        
        damagedCounts.clear();
        // + 1 to skip comma
        for (size_t pos = 0, i = 0; i < counts.size(); i += pos + 1) {
            damagedCounts.push_back(std::stoi(&counts[i], &pos));
        }
        
        uint32_t num = 0;
        
        unknownPos.clear();
        for (uint32_t i = 0; i < springs.size(); ++i) {
            if (springs[i] == '?') { unknownPos.push_back(i); }
            else if (springs[i] == '.') { num |= 1 << i; }
        }
        
        for (uint32_t i = 0; i < (1 << unknownPos.size()); ++i) {
            uint32_t unknownNum = num;
            for (uint32_t j = 0; j < unknownPos.size(); ++j) {
                unknownNum |= ((i >> j) & 0x1) << unknownPos[j];
            }
            
            uint32_t damagedIndex = 0;
            // Count sizes of groups of 0s
            for (uint32_t j = 0; j < springs.size(); ++j) {
                uint32_t groupCount = 0;
                while (j < springs.size() && !((unknownNum >> j) & 0x1)) { 
                    ++groupCount;
                    ++j;
                }
                
                if (groupCount > 0) {
                    if (damagedIndex < damagedCounts.size() &&
                        groupCount == damagedCounts[damagedIndex]) {
                        damagedCounts[damagedIndex++] = 0;
                    }
                }
            }
            
            if (std::all_of(damagedCounts.begin(), damagedCounts.end(),
                [](uint32_t x){return x == 0;})) {
                // Found valid example
                ++ans;
            }
        }
        std::cout << num << '\n';
    }
    file.close();
    
    std::cout << "Answer: " << ans << '\n';
}

void part2()
{
    int ans = 0;
    
    const std::string filename("input.txt");
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
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
