#include "CoordinateUtils.hpp"
#include "CoutUtils.hpp"
#include "NumberUtils.hpp"
#include "StringUtils.hpp"
#include "VectorUtils.hpp"
#include "RangeUtils.hpp"

#include <cassert>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include <set>


using puzzleValueType = long long int;

// --- Day 14: Parabolic Reflector Dish ---
// You reach the place where all of the mirrors were pointing:
//  a massive parabolic reflector dish attached to the side of another large mountain.

// The dish is made up of many small mirrors,
//  but while the mirrors themselves are roughly in the shape of a parabolic reflector dish,
//  each individual mirror seems to be pointing in slightly the wrong direction.
// If the dish is meant to focus light, all it's doing right now is sending it in a vague direction.

// This system must be what provides the energy for the lava!
//  If you focus the reflector dish,
//  maybe you can go where it's pointing and use the light to fix the lava production.

// Upon closer inspection, the individual mirrors each appear to be connected
//  via an elaborate system of ropes and pulleys to a large metal platform below the dish.
// The platform is covered in large rocks of various shapes.
//  Depending on their position, the weight of the rocks deforms the platform,
//  and the shape of the platform controls which ropes move and ultimately the focus of the dish.

// In short: if you move the rocks, you can focus the dish.
// The platform even has a control panel on the side that lets you tilt it in one of four directions!
// The rounded rocks (O) will roll when the platform is tilted, while the cube-shaped rocks (#) will stay in place.
// You note the positions of all of the empty spaces (.) and rocks (your puzzle input). For example:

// O....#....
// O.OO#....#
// .....##...
// OO.#O....O
// .O.....O#.
// O.#..O.#.#
// ..O..#O..O
// .......O..
// #....###..
// #OO..#....

// Start by tilting the lever so all of the rocks will slide north as far as they will go:

// OOOO.#.O..
// OO..#....#
// OO..O##..O
// O..#.OO...
// ........#.
// ..#....#.#
// ..O..#.O.O
// ..O.......
// #....###..
// #....#....

// You notice that the support beams along the north side of the platform are damaged;
//  to ensure the platform doesn't collapse, you should calculate the total load on the north support beams.

// The amount of load caused by a single rounded rock (O) is equal to the number of rows from the rock to the south edge of the platform,
//  including the row the rock is on.
// (Cube-shaped rocks (#) don't contribute to load.)
// So, the amount of load caused by each rock in each row is as follows:

// OOOO.#.O.. 10
// OO..#....#  9
// OO..O##..O  8
// O..#.OO...  7
// ........#.  6
// ..#....#.#  5
// ..O..#.O.O  4
// ..O.......  3
// #....###..  2
// #....#....  1

// The total load is the sum of the load caused by all of the rounded rocks. In this example, the total load is 136.

// Tilt the platform so that the rounded rocks all roll north.
// Afterward, what is the total load on the north support beams?

const std::string givenTestData_problem1 = "\
O....#....\n\
O.OO#....#\n\
.....##...\n\
OO.#O....O\n\
.O.....O#.\n\
O.#..O.#.#\n\
..O..#O..O\n\
.......O..\n\
#....###..\n\
#OO..#....\n";

constexpr puzzleValueType expectedSolution_problem1 = 136;

constexpr char ROLLING_ROCK = 'O';
constexpr char CUBE_ROCK = '#';
constexpr char EMPTY = '.';

void copyMapExceptRollingRocks(const std::vector<std::vector<char> > & initialMap, std::vector<std::vector<char> > & mapCopy) {
    assert(!initialMap.empty());
    // Copy the map as if there were no rolling rocks on it.
    for (const auto & initialMapLine : initialMap) {
        std::vector<char> mapCopyLine;
        for (auto & c : initialMapLine) {
            if (c == CUBE_ROCK) {
                mapCopyLine.push_back(CUBE_ROCK);
            } else {
                mapCopyLine.push_back(EMPTY);  // we ignore the rolling rocks for now. re-add them later
            }
        }
        mapCopy.push_back(mapCopyLine);
    }
}

void moveRollingRocksNorthwards(const std::vector<std::vector<char> > & initialMap, std::vector<std::vector<char> > & _movedMap) {
    assert(!initialMap.empty());
    
    std::vector<std::vector<char> > movedMap;
    copyMapExceptRollingRocks(initialMap, movedMap);
    assert(movedMap.size() == initialMap.size());
    assert(movedMap[0].size() == initialMap[0].size());

    // Copy the rolling rocks as if they moved upwards
    for (size_t x=0; x < initialMap[0].size(); x++) {
        unsigned int rockCount = 0;
        for (size_t y=0; y < initialMap.size(); y++) {
            // calculate read coordinates
            TCoordinates<size_t> rC{x, initialMap.size()-y-1};  //-1 to convert to indices.     [rC]: Read Coordinates
            const char & readCharacter = initialMap[rC.y][rC.x];
            if (readCharacter == ROLLING_ROCK) {
                rockCount++;
            // } else if(readCharacter == EMPTY) {
                // do nothing
            } else if (readCharacter == CUBE_ROCK) {
                for (unsigned int i=0; i < rockCount; i++) {
                    movedMap[rC.y+(i+1)][rC.x] = ROLLING_ROCK;    //+1 to start before the CUBE_ROCK
                }
                rockCount = 0;
            }
        }
        // Make sure the rolling rocks don't roll off the edge.
        for (unsigned int i=0; i < rockCount; i++) {
            movedMap[0+i][x] = ROLLING_ROCK;
        }
    }

    std::swap(movedMap, _movedMap);
}

void moveRollingRocksSouthwards(const std::vector<std::vector<char> > & initialMap, std::vector<std::vector<char> > & _movedMap) {
    assert(!initialMap.empty());
    
    std::vector<std::vector<char> > movedMap;
    copyMapExceptRollingRocks(initialMap, movedMap);
    assert(movedMap.size() == initialMap.size());
    assert(movedMap[0].size() == initialMap[0].size());

    // Copy the rolling rocks as if they moved upwards
    for (size_t x=0; x < initialMap[0].size(); x++) {
        unsigned int rockCount = 0;
        for (size_t y=0; y < initialMap.size(); y++) {
            // calculate read coordinates
            TCoordinates<size_t> rC{x, y};  //-1 to convert to indices.     [rC]: Read Coordinates
            const char & readCharacter = initialMap[rC.y][rC.x];
            if (readCharacter == ROLLING_ROCK) {
                rockCount++;
            // } else if(readCharacter == EMPTY) {
                // do nothing
            } else if (readCharacter == CUBE_ROCK) {
                for (unsigned int i=0; i < rockCount; i++) {
                    movedMap[rC.y-(i+1)][rC.x] = ROLLING_ROCK;    //+1 to start before the CUBE_ROCK
                }
                rockCount = 0;
            }
        }
        // Make sure the rolling rocks don't roll off the edge.
        for (unsigned int i=0; i < rockCount; i++) {
            movedMap[initialMap.size()-1-i][x] = ROLLING_ROCK; //-1 to convert initialMap.size() to indices
        }
    }

    std::swap(movedMap, _movedMap);
}

void moveRollingRocksEastwards(const std::vector<std::vector<char> > & initialMap, std::vector<std::vector<char> > & _movedMap) {
    assert(!initialMap.empty());
    
    std::vector<std::vector<char> > movedMap;
    copyMapExceptRollingRocks(initialMap, movedMap);
    assert(movedMap.size() == initialMap.size());
    assert(movedMap[0].size() == initialMap[0].size());

    // Copy the rolling rocks as if they moved upwards
    for (size_t y=0; y < initialMap.size(); y++) {
        unsigned int rockCount = 0;
        for (size_t x=0; x < initialMap[0].size(); x++) {
            // calculate read coordinates
            TCoordinates<size_t> rC{x, y};  //-1 to convert to indices.     [rC]: Read Coordinates
            const char & readCharacter = initialMap[rC.y][rC.x];
            if (readCharacter == ROLLING_ROCK) {
                rockCount++;
            // } else if(readCharacter == EMPTY) {
                // do nothing
            } else if (readCharacter == CUBE_ROCK) {
                for (unsigned int i=0; i < rockCount; i++) {
                    movedMap[rC.y][rC.x-(i+1)] = ROLLING_ROCK;    //+1 to start before the CUBE_ROCK
                }
                rockCount = 0;
            }
        }
        // Make sure the rolling rocks don't roll off the edge.
        for (unsigned int i=0; i < rockCount; i++) {
            movedMap[y][initialMap[0].size()-(1+i)] = ROLLING_ROCK;      //-1 to convert initialMap.size() to indices
        }
    }

    std::swap(movedMap, _movedMap);
}

void moveRollingRocksWestwards(const std::vector<std::vector<char> > & initialMap, std::vector<std::vector<char> > & _movedMap) {
    assert(!initialMap.empty());
    
    std::vector<std::vector<char> > movedMap;
    copyMapExceptRollingRocks(initialMap, movedMap);
    assert(movedMap.size() == initialMap.size());
    assert(movedMap[0].size() == initialMap[0].size());

    // Copy the rolling rocks as if they moved upwards
    for (size_t y=0; y < initialMap.size(); y++) {
        unsigned int rockCount = 0;
        for (size_t x=0; x < initialMap[0].size(); x++) {
            // calculate read coordinates
            TCoordinates<size_t> rC{initialMap[0].size()-(1+x), y};  //-1 to convert to indices.     [rC]: Read Coordinates
            const char & readCharacter = initialMap[rC.y][rC.x];
            if (readCharacter == ROLLING_ROCK) {
                rockCount++;
            // } else if(readCharacter == EMPTY) {
                // do nothing
            } else if (readCharacter == CUBE_ROCK) {
                for (unsigned int i=0; i < rockCount; i++) {
                    movedMap[rC.y][rC.x+(i+1)] = ROLLING_ROCK;    //+1 to start before the CUBE_ROCK
                }
                rockCount = 0;
            }
        }
        // Make sure the rolling rocks don't roll off the edge.
        for (unsigned int i=0; i < rockCount; i++) {
            movedMap[y][0+i] = ROLLING_ROCK;
        }
    }

    std::swap(movedMap, _movedMap);
}

template<typename T>
puzzleValueType solve1(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    // convert to map
    std::vector<std::vector<char> > map;
    {
        bool equalLinesInMap = VectorUtils::convertToMap(lines, map);
        assert(equalLinesInMap);
    }

    // move all roling rocks upwards
    // CoutUtils::print2d("initialMap", map);
    // std::cout << std::endl;
    moveRollingRocksNorthwards(map, map);
    // CoutUtils::print2d("movedMap", map);

    // Count puzzle value
    puzzleValueType puzzleValue = 0;
    for (size_t x=0; x < map[0].size(); x++) {
        for (size_t y=0; y < map.size(); y++) {
            TCoordinates<size_t> rC{x, map.size()-y-1};  //-1 to convert to indices.     [rC]: Read Coordinates
            const char & readCharacter = map[rC.y][rC.x];
            if (readCharacter == ROLLING_ROCK) {
                puzzleValue += map.size()-rC.y;    // higher is more weight; so the weight increases in the opposing direction as the (rC.)y index.
            }
        }
    }

    return puzzleValue;
}

// --- Part Two ---
// The parabolic reflector dish deforms, but not in a way that focuses the beam.
// To do that, you'll need to move the rocks to the edges of the platform.
// Fortunately, a button on the side of the control panel labeled "spin cycle" attempts to do just that!

// Each cycle tilts the platform four times so that the rounded rocks roll north, then west, then south, then east.
// After each tilt, the rounded rocks roll as far as they can before the platform tilts in the next direction.
// After one cycle, the platform will have finished rolling the rounded rocks in those four directions in that order.

// Here's what happens in the example above after each of the first few cycles:

// After 1 cycle:
// .....#....
// ....#...O#
// ...OO##...
// .OO#......
// .....OOO#.
// .O#...O#.#
// ....O#....
// ......OOOO
// #...O###..
// #..OO#....

// After 2 cycles:
// .....#....
// ....#...O#
// .....##...
// ..O#......
// .....OOO#.
// .O#...O#.#
// ....O#...O
// .......OOO
// #..OO###..
// #.OOO#...O

// After 3 cycles:
// .....#....
// ....#...O#
// .....##...
// ..O#......
// .....OOO#.
// .O#...O#.#
// ....O#...O
// .......OOO
// #...O###.O
// #.OOO#...O

// This process should work if you leave it running long enough,
//  but you're still worried about the north support beams.
// To make sure they'll survive for a while, you need to calculate the total load on the north support beams after 1000000000 cycles.

// In the above example, after 1000000000 cycles, the total load on the north support beams is 64.

// Run the spin cycle for 1000000000 cycles.
// Afterward, what is the total load on the north support beams?

// Although it hasn't changed, you can still get your puzzle input.

const std::string givenTestData_problem2 = "\
.....#....\n\
....#...O#\n\
...OO##...\n\
.OO#......\n\
.....OOO#.\n\
.O#...O#.#\n\
....O#....\n\
......OOOO\n\
#...O###..\n\
#..OO#....\n";

constexpr puzzleValueType expectedSolution_problem2 = 64;

constexpr unsigned long int CYCLES = 1000000000;

template<typename T>
puzzleValueType solve2(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    // convert to map
    std::vector<std::vector<char> > map;
    {
        bool equalLinesInMap = VectorUtils::convertToMap(lines, map);
        assert(equalLinesInMap);
    }

    // move all roling rocks upwards
    CoutUtils::print2d("initialMap", map);
    std::cout << std::endl;
    unsigned long int printCounter = 0;
    for (unsigned long int cycle = 0; cycle < CYCLES; cycle++) {
        if (printCounter == 1000) {
            printCounter = 0;
            PRINT(cycle);
        }
        printCounter++;
        //north ^
        moveRollingRocksNorthwards(map, map);
        //west <-
        moveRollingRocksWestwards(map, map);
        //south \/
        moveRollingRocksSouthwards(map, map);
        // east ->
        moveRollingRocksEastwards(map, map);
    }
    CoutUtils::print2d("movedMap", map);

    // Count puzzle value
    puzzleValueType puzzleValue = 0;
    for (size_t x=0; x < map[0].size(); x++) {
        for (size_t y=0; y < map.size(); y++) {
            TCoordinates<size_t> rC{x, map.size()-y-1};  //-1 to convert to indices.     [rC]: Read Coordinates
            const char & readCharacter = map[rC.y][rC.x];
            if (readCharacter == ROLLING_ROCK) {
                puzzleValue += map.size()-rC.y;    // higher is more weight; so the weight increases in the opposing direction as the (rC.)y index.
            }
        }
    }

    return puzzleValue;
}

int main(int argc, char ** argv) {
    std::stringstream inputStream;
    if (argc == 1) {
        std::stringstream strStream_problem1(givenTestData_problem1);
        const auto solve1_solution = solve1(strStream_problem1);
        std::stringstream strStream_problem2(givenTestData_problem2);
        const auto solve2_solution = solve2(strStream_problem2);

        if (solve1_solution == expectedSolution_problem1) {
            std::cout << "Puzzle A <Success!> :[" << solve1_solution << "]" << std::endl;
        } else {
            std::cout << "Puzzle A <MISMATCH> :[" << solve1_solution << "]" << std::endl;
        }

        if (solve2_solution == expectedSolution_problem2) {
            std::cout << "Puzzle B <Success!> :[" << solve2_solution << "]" << std::endl;
        } else {
            std::cout << "Puzzle B <MISMATCH> :[" << solve2_solution << "]" << std::endl;
        }
    } else if (argc == 2 || argc == 3) {
        std::string url(argv[1]);
        std::cout << "Trying to input file:[" << url << "]" << std::endl;
        std::filesystem::path path(url);

        std::ifstream fileStream;
        fileStream.open(path);
        if (!fileStream.is_open()) {
            std::cout << "fileStream is NOT Open!" << std::endl;
            return -1;
        }

        if (argc == 2 || (argc == 3 && argv[2][0] == 'A')) {
            auto solve1_solution = solve1(fileStream);
            std::cout << "Puzzle A result :[" << solve1_solution << "]" << std::endl;
        } else if(argc == 3 && argv[2][0] == 'B') {
            auto solve2_solution = solve2(fileStream);
            std::cout << "Puzzle B result :[" << solve2_solution << "]" << std::endl;
        }
    }

    return 0;
}
