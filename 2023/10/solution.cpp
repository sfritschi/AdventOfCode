#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <array>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <list>
#include <numeric>

#include <cassert>
#include <cstdio>
#include <cmath>

#define FLATTEN(r, c) (r * nCols + c)
            
enum Direction {
    LEFT = 0,
    RIGHT,
    TOP,
    BOTTOM,
    INVALID
};

struct Position {
    size_t row;
    size_t col;
    Direction dir;
    
    bool operator==(const Position &other) const = default;
};

Direction next(char pipe, Direction incoming) {
    Direction dir = INVALID;
    switch (pipe) {
        case '|':
            if (incoming == TOP)         { dir = BOTTOM; }
            else if (incoming == BOTTOM) { dir = TOP; }
            break;
        case '-':
            if (incoming == LEFT)       { dir = RIGHT; }
            else if (incoming == RIGHT) { dir = LEFT; }
            break;
        case 'L':
            if (incoming == TOP)        { dir = RIGHT; }
            else if (incoming == RIGHT) { dir = TOP; }
            break;
        case 'J':
            if (incoming == TOP)       { dir = LEFT; }
            else if (incoming == LEFT) { dir = TOP; }
            break;
        case '7':
            if (incoming == LEFT)        { dir = BOTTOM; }
            else if (incoming == BOTTOM) { dir = LEFT; }
            break;
        case 'F':
            if (incoming == RIGHT)       { dir = BOTTOM; }
            else if (incoming == BOTTOM) { dir = RIGHT; }
            break;
    }
    
    return dir;
}

void part1()
{
    int ans = 0;
    
    const std::string filename("input.txt");
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    std::string maze;
    char c;

    Position start{};
    
    start.dir = INVALID;
    size_t nCols = 0, nRows = 0;
    while (file.get(c)) {
        if (c == '\n') { ++nRows; }
        
        if (c != '\n') {
            if (c == 'S') {
                // Found starting position
                const size_t i = maze.size();
                if (nCols != 0) { start.row = i / nCols; start.col = i % nCols; }
                else            { start.row = 0; start.col = i; }
            }
            maze.push_back(c);
        } else if (nCols == 0) {
            nCols = maze.size();
        }
    }
    file.close();
    
    assert(maze[FLATTEN(start.row, start.col)] == 'S'); 
    
    
    std::list<Position> search;
    // Check bounds before 
    Position left = start, right = start, top = start, bottom = start;
    if (start.col > 0) { left   = {start.row, start.col - 1, RIGHT}; }
    if (start.col < nCols-1) { right  = {start.row, start.col + 1, LEFT}; }
    if (start.row > 0) { top    = {start.row - 1, start.col, BOTTOM}; }
    if (start.row < nRows-1) { bottom = {start.row + 1, start.col, TOP}; }
    
    if (next(maze[FLATTEN(left.row, left.col)], left.dir) != INVALID) {
        search.push_back(left);
    }
    if (next(maze[FLATTEN(right.row, right.col)], right.dir) != INVALID) {
        search.push_back(right);
    }
    if (next(maze[FLATTEN(top.row, top.col)], top.dir) != INVALID) {
        search.push_back(top);
    }
    if (next(maze[FLATTEN(bottom.row, bottom.col)], bottom.dir) != INVALID) {
        search.push_back(bottom);
    }
    
    const size_t initialSize = search.size();
    
    while (!search.empty()) {
        Position pos = search.front();
        search.pop_front();
        
        Position nextPos{};
        
        ++ans;
        
        char &pipe = maze[FLATTEN(pos.row, pos.col)];
        const Direction nextDir = next(pipe, pos.dir);
        if (nextDir == INVALID) {
            break;  // reached the furthest position
        }
        
        switch (nextDir) {
            case LEFT:
                nextPos = {pos.row, pos.col - 1, RIGHT};
                break;
            case RIGHT:
                nextPos = {pos.row, pos.col + 1, LEFT};
                break;
            case TOP:
                nextPos = {pos.row - 1, pos.col, BOTTOM};
                break;
            case BOTTOM:
                nextPos = {pos.row + 1, pos.col, TOP};
                break;
            
            default:
                throw std::runtime_error("Invalid direction encountered");
        }
        search.push_back(nextPos);
        // Mark this position as already visited
        pipe = 'X';
    }
    
    // Note: This approach could be prone to integer overflow in general
    // Divide by initial search size to avoid 'multiple' counting.
    std::cout << "Answer: " << ans / initialSize << '\n';
}

void part2()
{
    int ans = 0;
    
    const std::string filename("input.txt");
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    std::string maze;
    char c;

    Position start{};
    
    start.dir = INVALID;
    size_t nCols = 0, nRows = 0;
    while (file.get(c)) {
        if (c == '\n') { ++nRows; }
        
        if (c != '\n') {
            if (c == 'S') {
                // Found starting position
                const size_t i = maze.size();
                if (nCols != 0) { start.row = i / nCols; start.col = i % nCols; }
                else            { start.row = 0; start.col = i; }
            }
            maze.push_back(c);
        } else if (nCols == 0) {
            nCols = maze.size();
        }
    }
    file.close();
    
    assert(maze[FLATTEN(start.row, start.col)] == 'S'); 
    
    
    Position current;
    // Check bounds before 
    Position left = start, right = start, top = start, bottom = start;
    if (start.col > 0) { left   = {start.row, start.col - 1, RIGHT}; }
    if (start.col < nCols-1) { right  = {start.row, start.col + 1, LEFT}; }
    if (start.row > 0) { top    = {start.row - 1, start.col, BOTTOM}; }
    if (start.row < nRows-1) { bottom = {start.row + 1, start.col, TOP}; }
    
    if (next(maze[FLATTEN(left.row, left.col)], left.dir) != INVALID) {
        current = left;
    }
    else if (next(maze[FLATTEN(right.row, right.col)], right.dir) != INVALID) {
        current = right;
    }
    else if (next(maze[FLATTEN(top.row, top.col)], top.dir) != INVALID) {
        current = top;
    }
    else if (next(maze[FLATTEN(bottom.row, bottom.col)], bottom.dir) != INVALID) {
        current = bottom;
    }
    
    // Mark start as already visited
    maze[FLATTEN(start.row, start.col)] = 'X';
    
    while (true) {
        char &pipe = maze[FLATTEN(current.row, current.col)];
        const Direction nextDir = next(pipe, current.dir);
        if (nextDir == INVALID) {
            break;  // reached the furthest position
        }
        
        switch (nextDir) {
            case LEFT:
                current = {current.row, current.col - 1, RIGHT};
                break;
            case RIGHT:
                current = {current.row, current.col + 1, LEFT};
                break;
            case TOP:
                current = {current.row - 1, current.col, BOTTOM};
                break;
            case BOTTOM:
                current = {current.row + 1, current.col, TOP};
                break;
            
            default:
                throw std::runtime_error("Invalid direction encountered");
        }
        // Mark this position as already visited
        pipe = 'X';
    }
    
    std::ofstream out("out.txt");
    if (!out) {
        throw std::runtime_error("Failed to open output file");
    }
    
    // Write output line-by-line
    for (size_t i = 0; i < maze.size(); i += nCols) {
        const char n = '\n';
        out.write(&maze[i], nCols);
        out.write(&n, 1);
    }
    out.close();
    
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
