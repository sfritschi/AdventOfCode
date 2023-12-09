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
    
    const std::string filename("input.txt");
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
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
