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

struct Correction {
    int64_t x;
    int64_t offset;
};

void part1()
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
    std::vector<Point> buffer(galaxies);
    
    int64_t yCorr = galaxies[0].y;
    buffer[0].y += yCorr;
    for (size_t i = 1; i < galaxies.size(); ++i) {
        const int64_t dy = galaxies[i].y - galaxies[i-1].y;
        if (dy >= 2) {
            // Expansion occurred
            yCorr += dy - 1;
        }
        // Update y position of next galaxy to account for vertical expansion
        buffer[i].y += yCorr;
    }
    
    // - Horizontal direction (x-axis)
    std::set<int64_t> cols;  // Compare == std::less<int64_t> -> sorted in ascending order
    // Compute all (unique) column indices
    for (const auto &galaxy : galaxies) { cols.insert(galaxy.x); }
    
    std::vector<Correction> xCorrs;
    
    int64_t xCorr = *cols.begin();
    if (xCorr > 0) { xCorrs.push_back({.x = 0, .offset = xCorr}); }
    
    for (auto current = cols.begin(), next = std::next(cols.begin());
        next != cols.end(); ++current, ++next)
    {
        const int64_t dx = *next - *current;
        if (dx >= 2) {
            xCorr += dx - 1;
            xCorrs.push_back({.x = *current + 1, .offset = xCorr});
        }
    }
    
    for (auto &galaxy : buffer) {
        size_t i = 0;
        while (i < xCorrs.size() && galaxy.x > xCorrs[i].x) { ++i; }
        
        if (i > 0) {
            galaxy.x += xCorrs[i-1].offset;
        }
    }
    
    // Swap pointers with buffer
    std::swap(galaxies, buffer);
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
    int64_t ans = 0;
    const int64_t expansion = 1e6 - 1;  // expansion factor
    
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
    std::vector<Point> buffer(galaxies);
    
    int64_t yCorr = galaxies[0].y * expansion;
    buffer[0].y += yCorr;
    for (size_t i = 1; i < galaxies.size(); ++i) {
        const int64_t dy = galaxies[i].y - galaxies[i-1].y;
        if (dy >= 2) {
            // Expansion occurred
            yCorr += (dy - 1) * expansion;
        }
        // Update y position of next galaxy to account for vertical expansion
        buffer[i].y += yCorr;
    }
    
    // - Horizontal direction (x-axis)
    std::set<int64_t> cols;  // Compare == std::less<int64_t> -> sorted in ascending order
    // Compute all (unique) column indices
    for (const auto &galaxy : galaxies) { cols.insert(galaxy.x); }
    
    std::vector<Correction> xCorrs;
    
    int64_t xCorr = *cols.begin() * expansion;
    if (xCorr > 0) { xCorrs.push_back({.x = 0, .offset = xCorr}); }
    
    for (auto current = cols.begin(), next = std::next(cols.begin());
        next != cols.end(); ++current, ++next)
    {
        const int64_t dx = *next - *current;
        if (dx >= 2) {
            xCorr += (dx - 1) * expansion;
            xCorrs.push_back({.x = *current + 1, .offset = xCorr});
        }
    }
    
    for (auto &galaxy : buffer) {
        size_t i = 0;
        while (i < xCorrs.size() && galaxy.x > xCorrs[i].x) { ++i; }
        
        if (i > 0) {
            galaxy.x += xCorrs[i-1].offset;
        }
    }
    
    // Swap pointers with buffer
    std::swap(galaxies, buffer);
    // Compute sum of shortest distances between pairs of galaxies using
    // taxicab metric
    for (size_t i = 0; i < galaxies.size(); ++i) {
        for (size_t j = i + 1; j < galaxies.size(); ++j) {
            ans += galaxies[i].distance(galaxies[j]);
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
