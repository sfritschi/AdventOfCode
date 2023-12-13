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

size_t checkReflection(const std::vector<std::string> &pattern) 
{
    for (size_t i = 1; i < pattern.size(); ++i) {
        bool foundReflection = true;
        const size_t nChecks = std::min(i, pattern.size() - i);
        for (size_t j = i - nChecks; j < i; ++j) {
            if (pattern[j] != pattern[2*i - j - 1]) {
                foundReflection = false;
                break;
            }
        }
        
        if (foundReflection) {
            // We have found a reflection involving the first i rows
            return i;
        }
    }
    
    return 0;
}

void part1()
{
    size_t ans = 0;
    
    const std::string filename("input.txt");
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    std::string line;
    std::vector<std::string> pattern;
    std::vector<std::string> patternTrans;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            pattern.push_back(line);
            if (patternTrans.empty()) { patternTrans.resize(line.size()); }
            
            for (size_t i = 0; i < line.size(); ++i) {
                patternTrans[i].push_back(line[i]);
            }
        } else {
            // Look for horizontal reflections
            ans += checkReflection(patternTrans);
            ans += 100 * checkReflection(pattern);
            // Reset
            pattern.clear();
            patternTrans.clear();
        }
    }
    
    // Final pattern
    ans += checkReflection(patternTrans);
    ans += 100 * checkReflection(pattern);
            
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
