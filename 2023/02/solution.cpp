#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <cassert>

// Note: Do NOT change order
enum Colors {
    RED = 0,
    GREEN,
    BLUE,
    NCOLORS
};

void part1()
{
    const int colorThresh[NCOLORS] = {
        12,  // red
        13,  // green
        14   // blue
    };
    
    const std::string filename("input.txt");
    
    std::ifstream file (filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    const size_t gameOffset = sizeof("Game ") - 1;  // -1 for terminating '\0'
    
    int ans = 0;
    int count = 0;
    std::string line, game, round, draw;
    
    int maxColors[NCOLORS];
    
    while (std::getline(file, line)) {    
        // Parse individual line
        std::stringstream ss (line);
        // Extract game ID
        if (!std::getline(ss, game, ':')) {
            throw std::runtime_error("Failed to parse game ID on line " 
                + std::to_string(count + 1));
        }
        
        assert(gameOffset < game.size());
        const int id = std::atoi(game.c_str() + gameOffset); assert(id != 0);
        
        // Initialize/reset max. color values
        for (int i = 0; i < NCOLORS; ++i) { maxColors[i] = 0; }
        // Check if the current game is valid
        bool isGameValid = true;
        // Extract cube numbers for each draw
        while (isGameValid && std::getline(ss, round, ';')) {
            std::stringstream roundStream (round);
            while (std::getline(roundStream, draw, ',')) {
                size_t index = 0;
                const int num = std::stoi(draw, &index);
                
                Colors color = NCOLORS;
                switch (draw[index + 1]) {
                    case 'r':
                        color = RED;
                        break;
                    case 'g':
                        color = GREEN;
                        break;
                    case 'b':
                        color = BLUE;
                        break;
                    default:
                        assert(false && "Encountered invalid color");
                }
                
                if (num > maxColors[color]) {
                    // Found new max. number of cubes for this color
                    maxColors[color] = num;
                    if (num > colorThresh[color]) {
                        // Threshold for this color has been exceeded.
                        // This game is invalid
                        isGameValid = false;
                        break;
                    }
                }
            }
        }

        if (isGameValid) {
            // Add of valid game to answer
            ans += id;
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
    
    const size_t gameOffset = sizeof("Game ") - 1;  // -1 for terminating '\0'
    
    int ans = 0;
    int count = 0;
    std::string line, game, round, draw;
    
    int maxColors[NCOLORS];
    
    while (std::getline(file, line)) {    
        // Parse individual line
        std::stringstream ss (line);
        // Extract game ID
        if (!std::getline(ss, game, ':')) {
            throw std::runtime_error("Failed to parse game ID on line " 
                + std::to_string(count + 1));
        }
        
        assert(gameOffset < game.size());
        const int id = std::atoi(game.c_str() + gameOffset); assert(id != 0);
        
        // Initialize/reset max. color values
        for (int i = 0; i < NCOLORS; ++i) { maxColors[i] = 0; }
        // Determine min. required cube numbers for game to be valid.
        // This amounts to finding max. number of cubes per color 
        // across entire game
        while (std::getline(ss, round, ';')) {
            std::stringstream roundStream (round);
            while (std::getline(roundStream, draw, ',')) {
                size_t index = 0;
                const int num = std::stoi(draw, &index);
                
                Colors color = NCOLORS;
                switch (draw[index + 1]) {
                    case 'r':
                        color = RED;
                        break;
                    case 'g':
                        color = GREEN;
                        break;
                    case 'b':
                        color = BLUE;
                        break;
                    default:
                        assert(false && "Encountered invalid color");
                }
                
                if (num > maxColors[color]) {
                    // Found new max. number of cubes for this color
                    maxColors[color] = num;
                }
            }
        }

        int power = 1;
        for (int i = 0; i < NCOLORS; ++i) { power *= maxColors[i]; }
        
        ans += power;
        
        ++count;
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
