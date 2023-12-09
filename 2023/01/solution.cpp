#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <array>

#include <ctype.h>

void part1()
{
    const char *filename = "input.txt";
    
    std::ifstream file (filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + std::string(filename));
    }
    
    // Note: This approach assumes there are no negative numbers in input
    int firstDigit = -1, lastDigit = -1;
    int count = 0;
    int ans = 0;
    char c;
    
    while (file.get(c)) {        
        if (std::isdigit(c)) {
            // Note: Subtract ASCII code for 0 to get digit value
            lastDigit = c - '0';  // Update current digit found
            if (firstDigit == -1) { firstDigit = lastDigit; }
        } else if (c == '\n') {
            // Reached end-of-line -> compute value associated with this line
            if (lastDigit == -1 || firstDigit == -1) {
                throw std::runtime_error("No digit was found on line: " + 
                    std::to_string(count+1)); 
            }
            
            const int val = firstDigit * 10 + lastDigit;
            // Note: Overflow is not possible, since maximum value from
            //       input.txt is #lines * 99 == 1000 * 99 == 99000
            ans += val;
            ++count;  // increment line count
            
            // Reset digits
            firstDigit = lastDigit = -1;
        }
    }
    
    file.close();
    
    std::cout << "Answer: " << ans << '\n';
}

struct Digit {
    std::string digit;
    bool isMatch;
};

class DigitSearch {
public:
    DigitSearch() = default;
    
    void isMatch(const char c, int &value)
    {
        value = 0;  // initialize (invalid)
        
        for (int i = 0; i < static_cast<int>(digits.size()); ++i) {
            auto &el = digits[i];
            
            const int size = static_cast<int>(el.digit.size());
            if (el.isMatch && pos < size && el.digit[pos] == c) {
                // Check if we found a successful match
                if (pos == size - 1) {
                    value = i + 1;
                    reset();
                    return;  // done
                }
            } else {
                // Mark this digit as no longer matching
                el.isMatch = false;
            }
        }
        
        // Check if there are no more matching digits
        bool noMatch = true;
        for (const auto &el : digits) {
            if (el.isMatch) {
                noMatch = false;
                break;
            }
        }
        
        if (noMatch) {
            reset();
        } else {
            pos += 1;  // advance
        }
    }

private:
    std::array<Digit, 9> digits {{
        {"one", true},
        {"two", true},
        {"three", true},
        {"four", true},
        {"five", true},
        {"six", true},
        {"seven", true},
        {"eight", true},
        {"nine", true}
    }};
    int pos = 0;
    
    void reset()
    {
        for (auto &el : digits) {
            el.isMatch = true;
        }
        pos = 0;
    }
};
 
void part2()
{
    std::array<std::string, 9> digits {{
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine"
    }};
    
    const char *filename = "input.txt";
    
    std::ifstream file (filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + std::string(filename));
    }
    
    //DigitSearch digitSearch;
    
    // Note: This approach assumes there are no negative numbers in input
    int firstDigit = -1, lastDigit = -1;
    size_t firstDigitPos = std::string::npos, lastDigitPos = 0;
    int count = 0;
    int ans = 0;
    char c;
        
    std::string line;
    while (file.get(c)) {
        //if (c != '\n') {
        //    int value;
        //    digitSearch.isMatch(c, value);
        //    if (value != 0) {
        //        lastDigit = value;
        //        if (firstDigit == -1) { firstDigit = lastDigit; }
        //    }
        //    std::cout << c;
        //}
        if (c != '\n') { line.push_back(c); }
        
        if (std::isdigit(c)) {
            if (line.size() == 0) { throw std::runtime_error("Empty line encountered"); }
            const size_t pos = line.size() - 1;
            // Note: Subtract ASCII code for 0 to get digit value
            lastDigit = c - '0';  // Update current digit found
            lastDigitPos = pos;
            if (firstDigit == -1) { 
                firstDigit = lastDigit;
                firstDigitPos = pos;
            }
        } else if (c == '\n') {
            // Reached end-of-line -> compute value associated with this line
            if (lastDigit == -1 || firstDigit == -1) {
                throw std::runtime_error("No digit was found on line: " + 
                    std::to_string(count+1)); 
            }
            
            // Find min./max. digit from left/right and update first/last
            // digits if necessary
            size_t lmin = std::string::npos, rmax = 0;
            for (size_t i = 0; i < digits.size(); ++i) {
                const auto &digit = digits[i];
                
                size_t l = line.find(digit);
                if (l != std::string::npos && l < firstDigitPos && l < lmin) {
                    // Found a new candidate from the left
                    firstDigit = static_cast<int>(i + 1);
                    lmin = l;
                }
                
                size_t r = line.rfind(digit);
                if (r != std::string::npos && r > lastDigitPos && r > rmax) {
                    // Found a new candidate from the right
                    lastDigit = static_cast<int>(i + 1);
                    rmax = r;
                }
            }
            
            const int val = firstDigit * 10 + lastDigit;
            // Note: Overflow is not possible, since maximum value from
            //       input.txt is #lines * 99 == 1000 * 99 == 99000
            ans += val;
            ++count;  // increment line count
            // Reset digits
            firstDigit = lastDigit = -1;
            //std::cout << line << ":\t" << val << '\t' << ans << '\n';
            line.clear();
        }
    }
    
    file.close();
    
    std::cout << "Answer: " << ans << '\n';
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "Usage: ./solution <problem num.: 1/2>\n";
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
