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

enum HandType {
    HIGH_CARD = 0,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND
};

struct HandRating {
    HandType type;
    std::vector<int> values;
    int bid;
    
    bool operator<(const HandRating &other) const { 
        if (type < other.type) {
            return true;
        } else if (type > other.type) {
            return false;
        }
        
        size_t i = 0;
        // Find first element at which values differ, or i == values.size()
        // if all values are the same
        while (i < values.size() && values[i] == other.values[i]) { ++i; }
        
        return (i == values.size()) ? false : values[i] < other.values[i];
    }
};

void part1()
{
    const std::string filename("input.txt");
    
    std::ifstream file (filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    int ans = 0;
    
    std::map<char, int> cardValues = {
        {'2', 2},
        {'3', 3},
        {'4', 4},
        {'5', 5},
        {'6', 6},
        {'7', 7},
        {'8', 8},
        {'9', 9},
        {'T', 10},
        {'J', 11},
        {'Q', 12},
        {'K', 13},
        {'A', 14},
    };
    
    std::vector<HandRating> ratings;
    
    std::string hand;
    int bid;
    while (file >> hand >> bid) {
        std::map<char, int> cardFreq;
        for (const char c : hand) {
            ++cardFreq[c];  // int is default-constructed as 0, if c is not contained yet
        }
        
        HandRating rating{};
        rating.bid = bid;
        // Compute values of individual cards
        rating.values.reserve(hand.size());
        for (const char c : hand) {
            rating.values.push_back(cardValues[c]);
        }
        // Determine type of hand
        if (cardFreq.size() == hand.size()) {
            // All card types are unique
            rating.type = HIGH_CARD;
        } else if (cardFreq.size() == 1) {
            // All card types are the same
            rating.type = FIVE_OF_A_KIND;
        } else if (cardFreq.size() == 2) {
            // 2 distinct card types.
            // Could either be four of a kind (1,4) or full house (2,3)
            rating.type = FULL_HOUSE;
            for (const auto &freq : cardFreq) {
                if (freq.second == 1 || freq.second == 4) {
                    rating.type = FOUR_OF_A_KIND;
                    break;
                }
            }
        } else if (cardFreq.size() == 3) {
            // 3 distinct card types.
            // Could either be three of a kind (3,1,1) or two pair (2,2,1)
            rating.type = TWO_PAIR;
            for (const auto &freq : cardFreq) {
                if (freq.second == 3) {
                    rating.type = THREE_OF_A_KIND;
                }
            }
        } else {
            // 4 distinct card types -> one pair, i.e. (2,1,1,1)
            rating.type = ONE_PAIR;
        }
        
        ratings.push_back(rating);
    }
    file.close();
    
    // Sort ratings to find rank numbers
    std::sort(ratings.begin(), ratings.end());
    for (int i = 0; i < static_cast<int>(ratings.size()); ++i) {
        ans += (i + 1) * ratings[i].bid;
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
    
    int ans = 0;
    
    std::map<char, int> cardValues = {
        {'J', 1},
        {'2', 2},
        {'3', 3},
        {'4', 4},
        {'5', 5},
        {'6', 6},
        {'7', 7},
        {'8', 8},
        {'9', 9},
        {'T', 10},
        {'Q', 11},
        {'K', 12},
        {'A', 13}
    };
    
    std::vector<HandRating> ratings;
    
    std::string hand;
    int bid;
    while (file >> hand >> bid) {
        HandRating rating{};
        rating.bid = bid;
        // Compute values of individual cards
        rating.values.reserve(hand.size());
        for (const char c : hand) {
            rating.values.push_back(cardValues[c]);
        }
        
        std::map<char, int> cardFreq;
        for (const char c : hand) {
            if (c != 'J') { ++cardFreq[c]; }  // ignore frequency of jokers
        }
        // Compare only keys (card frequencies) 
        const auto cmp = 
            [](const std::pair<char, int> &a, const std::pair<char, int> &b) {
            return a.second < b.second;
        };
        const auto maxFreq = std::max_element(cardFreq.begin(), cardFreq.end(), cmp);
        
        char replacement = 0;
        if (maxFreq == cardFreq.end()) {
            // Hand consists of only jokers
            replacement = 'A';  // arbitrary replacement
        } else {
            replacement = maxFreq->first;
        }
        // Replace jokers in hand with most frequent card
        std::replace(hand.begin(), hand.end(), 'J', replacement);
        // Recount card frequencies
        cardFreq.clear();
        for (const char c : hand) {
            ++cardFreq[c];  // int is default-constructed as 0, if c is not contained yet
        }
        // Determine type of hand
        if (cardFreq.size() == hand.size()) {
            // All card types are unique
            rating.type = HIGH_CARD;
        } else if (cardFreq.size() == 1) {
            // All card types are the same
            rating.type = FIVE_OF_A_KIND;
        } else if (cardFreq.size() == 2) {
            // 2 distinct card types.
            // Could either be four of a kind (1,4) or full house (2,3)
            rating.type = FULL_HOUSE;
            for (const auto &freq : cardFreq) {
                if (freq.second == 1 || freq.second == 4) {
                    rating.type = FOUR_OF_A_KIND;
                    break;
                }
            }
        } else if (cardFreq.size() == 3) {
            // 3 distinct card types.
            // Could either be three of a kind (3,1,1) or two pair (2,2,1)
            rating.type = TWO_PAIR;
            for (const auto &freq : cardFreq) {
                if (freq.second == 3) {
                    rating.type = THREE_OF_A_KIND;
                }
            }
        } else {
            // 4 distinct card types -> one pair, i.e. (2,1,1,1)
            rating.type = ONE_PAIR;
        }
        
        ratings.push_back(rating);
    }
    file.close();
    
    // Sort ratings to find rank numbers
    std::sort(ratings.begin(), ratings.end());
    for (int i = 0; i < static_cast<int>(ratings.size()); ++i) {
        ans += (i + 1) * ratings[i].bid;
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
