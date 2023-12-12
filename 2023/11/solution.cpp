#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <functional>

#include <cassert>
#include <cstdio>
#include <cmath>

struct Point {
    int64_t x;
    int64_t y;
    
    int64_t distance(const Point &p) const {
        return std::abs(x - p.x) + std::abs(y - p.y);
    }
};

// Note: Cannot make galaxies const without using separate functor for it
template <typename Func>
void expandUniverse(std::vector<Point> &galaxies, Func &&dir, int64_t expansion)
{
    expansion -= 1;  // subtract original line that is being expanded
    
    int64_t corr = dir(galaxies[0]);
    dir(galaxies[0]) += corr;
    for (size_t i = 1; i < galaxies.size(); ++i) {
        // Add correction from previous iteration to cancel out 
        const int64_t diff = dir(galaxies[i]) - dir(galaxies[i-1]) + corr;
        if (diff >= 2) {
            // Expansion occurred
            corr += (diff - 1) * expansion;
        }
        // Update y position of next galaxy to account for vertical expansion
        dir(galaxies[i]) += corr;
    }
}

void part1(int64_t expansion=2)
{
    int64_t ans = 0;
    
    const std::string filename("input.txt");
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    int64_t x = 0, y = 0;
    char c;
    std::vector<Point> galaxies;
    while (file.get(c)) {
        if (c == '#') {
            galaxies.push_back({.x = x, .y = y});
        }
        if (c != '\n') {
            ++x;
        } else {
            // Move on to next line
            ++y;
            x = 0;
        }
    }
    file.close();
    
    assert(galaxies.size() >= 2);
    // Account for expansion of universe
    // - Vertical direction (y-axis)    
    expandUniverse(galaxies, [](Point &p) -> int64_t &{ return p.y; }, expansion);
    
    // - Horizontal direction (x-axis)
    // Sort w.r.t. x-component first
    std::sort(galaxies.begin(), galaxies.end(), 
        [](const Point &a, const Point &b) { return a.x < b.x; });
    
    expandUniverse(galaxies, [](Point &p) -> int64_t &{ return p.x; }, expansion);
    
    // Compute sum of shortest distances between pairs of galaxies using
    // taxicab metric
    for (size_t i = 0; i < galaxies.size(); ++i) {
        for (size_t j = i + 1; j < galaxies.size(); ++j) {
            ans += galaxies[i].distance(galaxies[j]);
        }
    }
    std::cout << "Answer: " << ans << '\n';
}

void part2()
{
    const int64_t expansion = 1e6;  // expansion factor
    part1(expansion);
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
