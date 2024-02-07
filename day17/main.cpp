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

// --- Day 17: Clumsy Crucible ---
// The lava starts flowing rapidly once the Lava Production Facility is operational.
// As you leave, the reindeer offers you a parachute, allowing you to quickly reach Gear Island.

// As you descend, your bird's-eye view of Gear Island reveals why you had trouble finding anyone on your way up:
//  half of Gear Island is empty, but the half below you is a giant factory city!

// You land near the gradually-filling pool of lava at the base of your new lavafall.
// Lavaducts will eventually carry the lava throughout the city, but to make use of it immediately,
//  Elves are loading it into large crucibles on wheels.

// The crucibles are top-heavy and pushed by hand.
// Unfortunately, the crucibles become very difficult to steer at high speeds,
//  and so it can be hard to go in a straight line for very long.

// To get Desert Island the machine parts it needs as soon as possible,
//  you'll need to find the best way to get the crucible from the lava pool to the machine parts factory.
// To do this, you need to minimize heat loss while choosing a route that doesn't require the crucible to go in a straight line for too long.

// Fortunately, the Elves here have a map (your puzzle input) that uses
// traffic patterns,
// ambient temperature,
// and hundreds of other parameters to calculate exactly how much heat loss can be expected for a crucible entering any particular city block.

// For example:

// 2413432311323
// 3215453535623
// 3255245654254
// 3446585845452
// 4546657867536
// 1438598798454
// 4457876987766
// 3637877979653
// 4654967986887
// 4564679986453
// 1224686865563
// 2546548887735
// 4322674655533

// Each city block is marked by a single digit that represents the amount of heat loss if the crucible enters that block.
// The starting point, the lava pool, is the top-left city block;
//  the destination, the machine parts factory, is the bottom-right city block.
// (Because you already start in the top-left block, you don't incur that block's heat loss unless you leave that block and then return to it.)

// Because it is difficult to keep the top-heavy crucible going in a straight line for very long,
//  it can move at most three blocks in a single direction before it must turn 90 degrees left or right.
// The crucible also can't reverse direction;
//  after entering each city block, it may only
//   turn left,
//   continue straight,
//   or turn right.

// One way to minimize heat loss is this path:

// 2>>34^>>>1323
// 32v>>>35v5623
// 32552456v>>54
// 3446585845v52
// 4546657867v>6
// 14385987984v4
// 44578769877v6
// 36378779796v>
// 465496798688v
// 456467998645v
// 12246868655<v
// 25465488877v5
// 43226746555v>

// This path never moves more than three consecutive blocks in the same direction and incurs a heat loss of only 102.

// Directing the crucible from the lava pool to the machine parts factory,
//  but not moving more than three consecutive blocks in the same direction, what is the least heat loss it can incur?

const std::string givenTestData_problem1 = "\
2413432311323\n\
3215453535623\n\
3255245654254\n\
3446585845452\n\
4546657867536\n\
1438598798454\n\
4457876987766\n\
3637877979653\n\
4654967986887\n\
4564679986453\n\
1224686865563\n\
2546548887735\n\
4322674655533\n";

constexpr puzzleValueType expectedSolution_problem1 = 102;

constexpr unsigned int MAXIMUM_STRAIGHT_MOVES = 3;

enum direction_e {
    e_north,
    e_east,
    e_south,
    e_west
};

class Crucible {
private:
    ICoordinates coordinates;

    direction_e direction;
    unsigned int stepCount = 0;     //steps since turn
    //---
    unsigned int heatLoss;
    
public:
    Crucible(const ICoordinates & coordinates, const direction_e & direction, const unsigned int & stepCount, const unsigned int & heatLoss)
        : coordinates(coordinates),
        direction(direction),
        stepCount(stepCount),
        heatLoss(heatLoss)
    {
    }

    const unsigned int & getStepCount() const {
        return stepCount;
    }

    const unsigned int & getHeatLoss() const {
        return heatLoss;
    }
};

class Block {
private:
    const unsigned int heatLoss;
    std::array<unsigned int, MAXIMUM_STRAIGHT_MOVES> bestHistory;
public:
    Block(const unsigned int & heatLoss)
        : heatLoss(heatLoss)
    {
        for (unsigned int i=0; i < bestHistory.size(); i++) {
            bestHistory[i] = std::numeric_limits<unsigned int>::max;
        }
    }

    const unsigned int & getHeatLoss() const {
        return heatLoss;
    }

    bool checkIfBetterCrucibleWasHereBefore(const Crucible & crucible) const {
        assert(crucible.getStepCount() < MAXIMUM_STRAIGHT_MOVES);
        return bestHistory[crucible.getStepCount] < crucible.getHeatLoss();
    }

    //TODO: record move and put in bestHistory.
};

template<typename T>
puzzleValueType solve1(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    // convert to map
    std::vector<std::vector<Block>> map;
    for (const auto & line : lines) {
        std::vector<Block> mapLine;
        for (const auto & c : line) {
            assert(c >= '0' && c <= '9');
            mapLine.emplace(c-'0');
        }
        map.push_back(mapLine);
    }

    // TODO: add Crucibles from starting line.

    puzzleValueType puzzleValue = 0;

    return puzzleValue;
}

const std::string givenTestData_problem2 = "";

constexpr puzzleValueType expectedSolution_problem2 = -1;

template<typename T>
puzzleValueType solve2(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    puzzleValueType puzzleValue = 0;

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
