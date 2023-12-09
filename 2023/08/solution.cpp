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

struct Destinations {
    std::string left;
    std::string right;
};

void part1()
{
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        throw std::runtime_error("Failed to open file");
    }
    
    // Parse instructions on first line
    std::string instructions;
    char c;
    while ((c = getc(fp)) != '\n') {
        instructions.push_back(c);
    }
    assert(instructions.size() > 0);
    c = getc(fp); assert(c == '\n');  // ignore blank line
    
    std::unordered_map<std::string, Destinations> lookup;
    
    // Remember first key
    char pos[3+1], left[3+1], right[3+1];  // + 1 for '\0'
    while (fscanf(fp, "%3s = (%3s, %3s)", pos, left, right) != EOF) {
        lookup[std::string(pos)] = {
            .left = std::string(left),
            .right = std::string(right)
        };
    }
    fclose(fp);
    
    // Iterate through instructions & wrap-around if necessary
    size_t index = 0;
    
    const std::string start("AAA");
    const std::string end("ZZZ");
    
    int ans = 0;
    std::string current = start;
    while (true) {
        assert(lookup.contains(current));
        switch (instructions[index]) {
            case 'L':
                // Take left direction
                current = lookup[current].left;
                break;
            
            case 'R':
                // Take right direction
                current = lookup[current].right;
                break;
            
            default:
                throw std::runtime_error("Unrecognized instruction: " + 
                    std::to_string(instructions[index]));
        }
        ++ans;  // moved 1 additional step

        if (current == end) {
            // Reached the end; done
            break;
        }
        index = (index + 1) % instructions.size();  // wrap around
    }
    
    std::cout << "Answer: " << ans << '\n';
}

void part2()
{
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        throw std::runtime_error("Failed to open file");
    }
    
    // Parse instructions on first line
    std::string instructions;
    char c;
    while ((c = getc(fp)) != '\n') {
        instructions.push_back(c);
    }
    assert(instructions.size() > 0);
    c = getc(fp); assert(c == '\n');  // ignore blank line
    
    std::unordered_map<std::string, Destinations> lookup;
    
    std::vector<std::string> positions;
    char pos[3+1], left[3+1], right[3+1];  // + 1 for '\0'
    while (fscanf(fp, "%3s = (%3s, %3s)", pos, left, right) != EOF) {
        const std::string position(pos);
        lookup[position] = {
            .left = std::string(left),
            .right = std::string(right)
        };
        
        if (position.back() == 'A') {
            positions.push_back(position);
        }
    }
    fclose(fp);
    
    // Iterate through instructions & wrap-around if necessary
    size_t index = 0;
    
    // Find number of steps until given position ends with Z.
    // This length is the period of the original position, and is repeated
    // indefinitely (verified computationally)
    std::vector<uint64_t> periods(positions.size(), 0);
    
    uint64_t ans = 0;
    while (std::any_of(periods.begin(), periods.end(), [](uint64_t x) {return x == 0;})) {        
        switch (instructions[index]) {
            case 'L':
                // Take left direction for all positions
                for (auto &pos : positions) { pos = lookup[pos].left; }
                break;
            
            case 'R':
                // Take right direction for all positions
                for (auto &pos : positions) { pos = lookup[pos].right; }
                break;
            
            default:
                throw std::runtime_error("Unrecognized instruction: " + 
                    std::to_string(instructions[index]));
        }
        ++ans;  // moved 1 additional step
                
        for (size_t i = 0; i < positions.size(); ++i) {
            if (positions[i].back() == 'Z') {
                periods[i] = ans;
            }
        }
        index = (index + 1) % instructions.size();  // wrap around
    }
    
    // Number of steps until ALL initial positions end with Z is the
    // least common multiple (lcm) of the respective periods
    ans = 1;
    for (const auto period : periods) {
        ans = std::lcm(period, ans);
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
