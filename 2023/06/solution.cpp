#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <numeric>

#include <cassert>
#include <cmath>

void part1()
{
    const std::string filename("input.txt");
    
    std::ifstream file (filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    int ans = 0;
    
    std::vector<int> times, records;
    
    const size_t offsetTimes = sizeof("Time:");
    const size_t offsetRecords = sizeof("Distance:");
    
    std::string line;
    // Read times
    std::getline(file, line);
    for (size_t pos = 0, i = offsetTimes; i < line.size(); i += pos) {
       times.push_back(
           std::stoi(&line[i], &pos)
       ); 
    }
    // Read record distances
    std::getline(file, line);
    for (size_t pos = 0, i = offsetRecords; i < line.size(); i += pos) {
       records.push_back(
           std::stoi(&line[i], &pos)
       ); 
    }
    file.close();
    
    assert(times.size() == records.size());
    
    ans = 1;
    for (size_t i = 0; i < times.size(); ++i) {
        const double half = static_cast<double>(times[i]) / 2.0;
        const double r = static_cast<double>(records[i]);
        
        double disc = half*half - r;
        if (disc < 0.0) {
            // No solution(s) possible for this time
            ans = 0;
            break;
        }
        disc = std::sqrt(disc);
        // Find minimum/maximum (integer) startup time, s.t. distance
        // travelled is strictly larger than current record
        int xmin = std::ceil(half - disc);
        if (xmin*(times[i] - xmin) == records[i]) { ++xmin; }
        assert(xmin*(times[i] - xmin) > records[i]);
        int xmax = std::floor(half + disc);
        if (xmax*(times[i] - xmax) == records[i]) { --xmax; }
        assert(xmax*(times[i] - xmax) > records[i]);
        // Multiply by size of interval of winning startup times
        ans *= xmax - xmin + 1;
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
    
    int64_t ans = 0;
    
    const auto notdigit = [](const int c){ return !std::isdigit(c); };
    
    std::string line;
    // Read times
    std::getline(file, line);
    // Remove white spaces
    std::remove_if(line.begin(), line.end(), notdigit);
    const int64_t time = std::stoll(line);
    
    // Read record distances
    std::getline(file, line);
    std::remove_if(line.begin(), line.end(), notdigit);
    const int64_t record = std::stoll(line);
    
    file.close();
    
    const double half = static_cast<double>(time) / 2.0;
    const double r = static_cast<double>(record);
    
    double disc = half*half - r;
    if (disc < 0.0) {
        // No solution(s) possible for this time
        ans = 0;
    } else {
        disc = std::sqrt(disc);
        // Find minimum/maximum (integer) startup time, s.t. distance
        // travelled is strictly larger than current record
        int64_t xmin = std::ceil(half - disc);
        if (xmin*(time - xmin) == record) { ++xmin; }
        assert(xmin*(time - xmin) > record);
        int64_t xmax = std::floor(half + disc);
        if (xmax*(time - xmax) == record) { --xmax; }
        assert(xmax*(time - xmax) > record);
        // Multiply by size of interval of winning startup times
        ans = xmax - xmin + 1;
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
