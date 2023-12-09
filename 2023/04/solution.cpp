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

void part1()
{
    const std::string filename("input.txt");
    
    std::ifstream file (filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    int ans = 0;
    int count = 0;
    
    std::unordered_set<int> winningNumbers;
    
    std::string line;
    while (std::getline(file, line)) {
        // Reset
        winningNumbers.clear();
        
        const size_t listStart = line.find(':'); 
        assert(listStart != std::string::npos);
        const size_t listSep = line.find('|');
        assert(listSep != std::string::npos);
        
        // Parse winning numbers
        for (size_t pos = 0, i = listStart + 1; i < listSep; i += pos) {
            try {
                const int val = std::stoi(&line[i], &pos);
                winningNumbers.insert(val);
            } catch (const std::invalid_argument &) {
                break;  // no more numbers
            }
        }
        
        // Parse numbers that were scratched
        int nMatches = 0;
        for (size_t pos = 0, i = listSep + 1; i < line.size(); i += pos) {
            try {
                const int scratchNumber = std::stoi(&line[i], &pos);
                if (winningNumbers.contains(scratchNumber)) {
                    nMatches += 1;
                }
            } catch (const std::invalid_argument &) {
                break;  // no more numbers
            }
        }
        
        // Note: int (32 bits) allows for max. 32 matches before overflow
        if (nMatches > 0) {
            // Add scratched points of this card to total
            ans += 1 << (nMatches - 1);
        }
        
        ++count;
    }
    file.close();
    
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
    
    std::unordered_set<int> winningNumbers;

    // Find out number of lines in file == number of unique cards
    const size_t lineCount = std::count(std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>(), '\n');
    // Reset file position
    file.clear();  // reset failbit
    file.seekg(0);
    
    std::vector<int> cardCounts(lineCount, 1);  // initially have 1 of each card
    
    std::string line;
    while (std::getline(file, line)) {
        // Reset
        winningNumbers.clear();
        
        const size_t listStart = line.find(':'); 
        assert(listStart != std::string::npos);
        const size_t listSep = line.find('|');
        assert(listSep != std::string::npos);
        
        // Parse winning numbers
        for (size_t pos = 0, i = listStart + 1; i < listSep; i += pos) {
            try {
                const int val = std::stoi(&line[i], &pos);
                winningNumbers.insert(val);
            } catch (const std::invalid_argument &) {
                break;  // no more numbers
            }
        }
        
        // Parse numbers that were scratched to find #matches
        int nMatches = 0;
        for (size_t pos = 0, i = listSep + 1; i < line.size(); i += pos) {
            try {
                const int scratchNumber = std::stoi(&line[i], &pos);
                if (winningNumbers.contains(scratchNumber)) {
                    nMatches += 1;
                }
            } catch (const std::invalid_argument &) {
                break;  // no more numbers
            }
        }
        
        assert(count + nMatches < static_cast<int>(cardCounts.size()));
        for (int i = count + 1; i <= count + nMatches; ++i) {
            cardCounts[i] += cardCounts[count];
        }
        
        ++count;
    }
    file.close();
    
    // Sum over all cardCounts
    ans = std::accumulate(cardCounts.begin(), cardCounts.end(), 0);
    
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
