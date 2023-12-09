#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <map>
#include <algorithm>

#include <cassert>

void part1()
{
    const std::string filename("input.txt");
    
    std::ifstream file (filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    int64_t ans = 0;
    
    std::vector<int64_t> seeds;
    
    std::string line;
    std::getline(file, line);
    
    // Parse seeds
    const size_t offset = sizeof("seeds: ") - 1;
    for (size_t pos = 0, i = offset; i < line.size(); i += pos) {
        seeds.push_back(
            static_cast<int64_t>(std::stoll(&line[i], &pos))
        );
    }
    
    std::vector<int64_t> seedsCopy(seeds);
    while (std::getline(file, line)) {
        if (line.back() == ':') {
            // Parse this conversion table
            while (std::getline(file, line) && !line.empty()) { 
                int64_t destStart, sourceStart, rangeLen; 
                
                size_t currPos = 0, width = 0;
                destStart = static_cast<int64_t>(std::stoll(&line[currPos], &width));
                currPos += width;
                sourceStart = static_cast<int64_t>(std::stoll(&line[currPos], &width));
                currPos += width;
                rangeLen = static_cast<int64_t>(std::stoll(&line[currPos], &width));
                
                for (size_t i = 0; i < seeds.size(); ++i) {
                    const int64_t seed = seeds[i];
                    if (seed >= sourceStart && seed < sourceStart + rangeLen) {
                        seedsCopy[i] = seed + destStart - sourceStart;
                    }
                }
            }
            // Copy over converted values
            std::copy(seedsCopy.begin(), seedsCopy.end(), seeds.begin());
        }
    }
    
    const auto minLocation = std::min_element(seeds.begin(), seeds.end());
    assert(minLocation != seeds.end());
    ans = *minLocation;
    
    std::cout << "Answer: " << ans << '\n';
}

struct Range {
    int64_t seed;
    int64_t len;
    
    bool operator<(const Range &other) const { return seed < other.seed; }
};

struct MapRange {
    int64_t destStart;
    int64_t sourceStart;
    int64_t len;    
};

void part2()
{
    const std::string filename("input.txt");
    
    std::ifstream file (filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    int64_t ans = 0;
    std::list<Range> seeds;
    
    std::string line;
    std::getline(file, line);
    
    // Parse seeds
    const size_t offset = sizeof("seeds: ") - 1;
    for (size_t pos = 0, i = offset; i < line.size();) {
        // Read seed number followed by length of range
        const int64_t seed = static_cast<int64_t>(std::stoll(&line[i], &pos));
        i += pos;
        const int64_t len = static_cast<int64_t>(std::stoll(&line[i], &pos));
        i += pos;
        
        seeds.push_back({
            .seed = seed,
            .len = len
        });
    }
    
    while (std::getline(file, line)) {
        if (line.back() == ':') {
            // Parse this conversion table
            std::vector<MapRange> mapRanges;
            while (std::getline(file, line) && !line.empty()) { 
                int64_t destStart, sourceStart, rangeLen; 
                
                size_t currPos = 0, width = 0;
                destStart = static_cast<int64_t>(std::stoll(&line[currPos], &width));
                currPos += width;
                sourceStart = static_cast<int64_t>(std::stoll(&line[currPos], &width));
                currPos += width;
                rangeLen = static_cast<int64_t>(std::stoll(&line[currPos], &width));
                
                mapRanges.push_back({
                    .destStart = destStart,
                    .sourceStart = sourceStart,
                    .len = rangeLen
                });
                
                // Split ranges as necessary
                //std::list<Range> splits;
                for (auto it = seeds.begin(); it != seeds.end();) {
                    const auto range = *it;
                    
                    // Case 1 (outside)
                    if (range.seed + range.len <= sourceStart) { ++it; }
                    // Case 1' (outside)
                    else if (range.seed >= sourceStart + rangeLen) { ++it; }
                    // Case 3 (fully contained)
                    else if (range.seed >= sourceStart && 
                        range.seed + range.len <= sourceStart + rangeLen) {
                        // Update seed number of full range
                        //seedsCopy.push_back({.seed = range.seed + destStart - sourceStart, .len = range.len});
                        //itCopy->seed = range.seed + destStart - sourceStart;
                        ++it;
                    }
                    
                    // Case 2 (partial overlap)
                    else if (range.seed < sourceStart &&
                        range.seed + range.len <= sourceStart + rangeLen) {
                        // Split into left and right range, transform only
                        // right range
                        it = seeds.erase(it);  // remove
                        // Note: increment iterator after insertion to
                        //       move past newly inserted value
                        // Left range:
                        const int64_t leftRange = sourceStart - range.seed;
                        it = seeds.insert(it, {
                            .seed = range.seed,
                            .len = leftRange
                        }); ++it;
                        // Right range:
                        it = seeds.insert(it, {
                            //.seed = destStart,
                            .seed = sourceStart,
                            .len = range.len - leftRange
                        }); ++it;
                    }
                    // Case 2' (partial overlap)
                    else if (range.seed < sourceStart + rangeLen &&
                        range.seed + range.len >= sourceStart + rangeLen) {
                        // Split into left and right range, transform only
                        // left range
                        it = seeds.erase(it);  // remove
                        // Left range:
                        const int64_t rightRange = range.seed + range.len - sourceStart - rangeLen;
                        it = seeds.insert(it, {
                            //.seed = range.seed + destStart - sourceStart,
                            .seed = range.seed,
                            .len = range.len - rightRange
                        }); ++it;
                        // Right range:
                        it = seeds.insert(it, {
                            .seed = sourceStart + rangeLen,
                            .len = rightRange
                        }); ++it;
                    }
                    // Case 4 (full overlap)
                    else if (range.seed < sourceStart &&
                        range.seed + range.len > sourceStart + rangeLen) {
                        // Split into three consecutive ranges,
                        // transform only middle one
                        it = seeds.erase(it);  // remove
                        const int64_t leftRange = sourceStart - range.seed;
                        const int64_t rightRange = range.seed + range.len - sourceStart - rangeLen;
                        const int64_t middleRange = range.len - leftRange - rightRange;
                        // Left range:
                        it = seeds.insert(it, {
                            .seed = range.seed,
                            .len = leftRange
                        }); ++it;
                        // Middle range:
                        it = seeds.insert(it, {
                            //.seed = destStart,
                            .seed = sourceStart,
                            .len = middleRange
                        }); ++it;
                        // Right range:
                        it = seeds.insert(it, {
                            .seed = sourceStart + rangeLen,
                            .len = rightRange
                        }); ++it;
                    }
                    else { assert(false && "Unhandled case encountered"); }
                }
                // Append splits to end of seeds
                //seeds.insert(seeds.end(), splits.begin(), splits.end());
            }
            
            // Iterate over ranges again and update seeds
            for (auto it = seeds.begin(); it != seeds.end(); ++it) {
                for (const auto &mapRange : mapRanges) {
                    if (it->seed >= mapRange.sourceStart && 
                        it->seed < mapRange.sourceStart + mapRange.len) {
                        it->seed += mapRange.destStart - mapRange.sourceStart;
                        break;  // no other range possible
                    }
                }
            }
        }
    }
    
    const auto minLocation = std::min_element(seeds.begin(), seeds.end());
    assert(minLocation != seeds.end());
    ans = minLocation->seed;
    
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
