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

// --- Day 11: Cosmic Expansion ---
// You continue following signs for "Hot Springs" and eventually come across an observatory.
// The Elf within turns out to be a researcher studying cosmic expansion using the giant telescope here.

// He doesn't know anything about the missing machine parts;
//  he's only visiting for this research project.
// However, he confirms that the hot springs are the next-closest area likely to have people;
//  he'll even take you straight there once he's done with today's observation analysis.

// Maybe you can help him with the analysis to speed things up?

// The researcher has collected a bunch of data and compiled the data into a single giant image (your puzzle input).
// The image includes empty space (.) and galaxies (#). For example:

// ...#......
// .......#..
// #.........
// ..........
// ......#...
// .#........
// .........#
// ..........
// .......#..
// #...#.....

// The researcher is trying to figure out
//  the sum of the lengths of the shortest path between every pair of galaxies.
// However, there's a catch:
//  the universe expanded in the time it took the light from those galaxies to reach the observatory.

// Due to something involving gravitational effects, only some space expands.
// In fact, the result is that any rows or columns that contain no galaxies should all actually be twice as big.

// In the above example, three columns and two rows contain no galaxies:

//    v  v  v
//  ...#......
//  .......#..
//  #.........
// >..........<
//  ......#...
//  .#........
//  .........#
// >..........<
//  .......#..
//  #...#.....
//    ^  ^  ^
// These rows and columns need to be twice as big; the result of cosmic expansion therefore looks like this:

// ....#........
// .........#...
// #............
// .............
// .............
// ........#....
// .#...........
// ............#
// .............
// .............
// .........#...
// #....#.......
// Equipped with this expanded universe, the shortest path between every pair of galaxies can be found.
// It can help to assign every galaxy a unique number:

// ....1........
// .........2...
// 3............
// .............
// .............
// ........4....
// .5...........
// ............6
// .............
// .............
// .........7...
// 8....9.......
// In these 9 galaxies, there are 36 pairs.
// Only count each pair once;
//  order within the pair doesn't matter.
// For each pair, find any shortest path between the two galaxies
//  using only steps that move up, down, left, or right exactly one . or # at a time.
// (The shortest path between two galaxies is allowed to pass through another galaxy.)

// For example, here is one of the shortest paths between galaxies 5 and 9:

// ....1........
// .........2...
// 3............
// .............
// .............
// ........4....
// .5...........
// .##.........6
// ..##.........
// ...##........
// ....##...7...
// 8....9.......

// This path has length 9 because it takes a minimum of nine steps to get from galaxy 5 to galaxy 9 
// (the eight locations marked # plus the step onto galaxy 9 itself).
// Here are some other example shortest path lengths:

// Between galaxy 1 and galaxy 7: 15
// Between galaxy 3 and galaxy 6: 17
// Between galaxy 8 and galaxy 9: 5
// In this example, after expanding the universe, the sum of the shortest path between all 36 pairs of galaxies is 374.

// Expand the universe, then find the length of the shortest path between every pair of galaxies. What is the sum of these lengths?

const std::string givenTestData_problem1 = "\
...#......\n\
.......#..\n\
#.........\n\
..........\n\
......#...\n\
.#........\n\
.........#\n\
..........\n\
.......#..\n\
#...#.....\n";
constexpr puzzleValueType expectedSolution_problem1 = 374;

constexpr char GALAXY = '#';

class Galaxy {
private:
    int x;
    int y;
public:
    Galaxy(const int & x, const int & y) : x(x), y(y) {}

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }
};

template<typename T>
puzzleValueType solve1(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    // Convert lines to 2d array
    std::vector<std::vector<char> > map;
    for (const auto & line : lines) {
        std::vector<char> lineVector;
        for (const auto & c : line) {
            lineVector.push_back(c);
        }
        map.push_back(lineVector);
    }

    // Identify columns to double
    std::set<int> columnsToDouble;
    for (unsigned int x=0; x < map[0].size(); x++) {
        bool hasGalaxy = false;
        for (unsigned int y=0; y < map.size(); y++) {
            if (map[y][x] == GALAXY) {
                hasGalaxy = true;
                break;
            }
        }
        if (!hasGalaxy) {
            columnsToDouble.insert(x);
        }
    }

    // Identify rows to double
    std::set<int> rowsToDouble;
    for (unsigned int y=0; y < map.size(); y++) {
        bool hasGalaxy = false;
        for (unsigned int x=0; x < map[0].size(); x++) {
            if (map[y][x] == GALAXY) {
                hasGalaxy = true;
                break;
            }
        }
        if (!hasGalaxy) {
            rowsToDouble.insert(y);
        }
    }

    std::vector<std::vector<char> > expandedMap;
    // Double rows and columns
    for (unsigned int y=0; y < map.size(); y++) {
        std::vector<char> line;
        for (unsigned int x=0; x < map[0].size(); x++) {
            line.push_back(map[y][x]);
            if (columnsToDouble.contains(x)) {
                line.push_back(map[y][x]);
            }
        }
        expandedMap.push_back(line);
        if (rowsToDouble.contains(y)) {
            expandedMap.push_back(line);
        }
    }

    // Make sure we got something shapely.
    for (unsigned int y=0; y < expandedMap.size(); y++) {
        assert(expandedMap[0].size() == expandedMap[y].size());
    }

    // // Print it to make sure
    // for (unsigned int y=0; y < expandedMap.size(); y++) {
    //     for (unsigned int x=0; x < expandedMap[0].size(); x++) {
    //         std::cout << expandedMap[y][x];
    //     }
    //     std::cout << std::endl;
    // }

    // Find all galaxies
    std::vector<Galaxy> galaxies;
    for (unsigned int y=0; y < expandedMap.size(); y++) {
        for (unsigned int x=0; x < expandedMap[0].size(); x++) {
            if (expandedMap[y][x] == GALAXY) {
                galaxies.emplace_back(x, y);
            }
        }
    }

    // Compute distances
    puzzleValueType puzzleValue = 0;
    for (unsigned int iA=0; iA < galaxies.size(); iA++) {
        const Galaxy & galaxyA = galaxies[iA];
        for (unsigned int iB=iA+1; iB < galaxies.size(); iB++) {
            const Galaxy & galaxyB = galaxies[iB];
            puzzleValue += std::abs(galaxyA.getX() - galaxyB.getX());
            puzzleValue += std::abs(galaxyA.getY() - galaxyB.getY());
        }
    }

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
