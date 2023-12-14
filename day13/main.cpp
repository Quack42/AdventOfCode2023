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

// --- Day 13: Point of Incidence ---
// With your help, the hot springs team locates an appropriate spring
//  which launches you neatly and precisely up to the edge of Lava Island.

// There's just one problem:
//  you don't see any lava.

// You do see a lot of ash and igneous rock;
//  there are even what look like gray mountains scattered around.
// After a while, you make your way to a nearby cluster of mountains
//  only to discover that the valley between them is completely full of large mirrors.
// Most of the mirrors seem to be aligned in a consistent way;
//  perhaps you should head in that direction?

// As you move through the valley of mirrors,
//  you find that several of them have fallen from the large metal frames keeping them in place.
// The mirrors are extremely flat and shiny,
//  and many of the fallen mirrors have lodged into the ash at strange angles.
// Because the terrain is all one color,
//  it's hard to tell where it's safe to walk or where you're about to run into a mirror.

// You note down the patterns of ash (.) and rocks (#) that you see as you walk (your puzzle input);
//  perhaps by carefully analyzing these patterns,
//  you can figure out where the mirrors are!

// For example:

// #.##..##.
// ..#.##.#.
// ##......#
// ##......#
// ..#.##.#.
// ..##..##.
// #.#.##.#.

// #...##..#
// #....#..#
// ..##..###
// #####.##.
// #####.##.
// ..##..###
// #....#..#

// To find the reflection in each pattern, you need to find a perfect reflection across either
//  a horizontal line between two rows
//  or across a vertical line between two columns.

// In the first pattern,
//  the reflection is across a vertical line between two columns;
//  arrows on each of the two columns point at the line between the columns:

// 123456789
//     ><   
// #.##..##.
// ..#.##.#.
// ##......#
// ##......#
// ..#.##.#.
// ..##..##.
// #.#.##.#.
//     ><   
// 123456789

// In this pattern, the line of reflection is the vertical line between columns 5 and 6.
// Because the vertical line is not perfectly in the middle of the pattern,
//  part of the pattern (column 1) has nowhere to reflect onto and can be ignored;
//  every other column has a reflected column within the pattern and must match exactly:
//   column 2 matches column 9,
//   column 3 matches 8,
//   4 matches 7,
//   and 5 matches 6.

// The second pattern reflects across a horizontal line instead:

// 1 #...##..# 1
// 2 #....#..# 2
// 3 ..##..### 3
// 4v#####.##.v4
// 5^#####.##.^5
// 6 ..##..### 6
// 7 #....#..# 7

// This pattern reflects across the horizontal line between rows 4 and 5.
// Row 1 would reflect with a hypothetical row 8,
//  but since that's not in the pattern, row 1 doesn't need to match anything.
// The remaining rows match:
//  row 2 matches row 7,
//  row 3 matches row 6,
//  and row 4 matches row 5.

// To summarize your pattern notes,
//  add up the number of columns to the left of each vertical line of reflection;
//  to that, also add 100 multiplied by the number of rows above each horizontal line of reflection.
// In the above example, the first pattern's vertical line has 5 columns to its left and the second pattern's horizontal line has 4 rows above it, a total of 405.

// Find the line of reflection in each of the patterns in your notes. What number do you get after summarizing all of your notes?

const std::string givenTestData_problem1 = "\
#.##..##.\n\
..#.##.#.\n\
##......#\n\
##......#\n\
..#.##.#.\n\
..##..##.\n\
#.#.##.#.\n\
\n\
#...##..#\n\
#....#..#\n\
..##..###\n\
#####.##.\n\
#####.##.\n\
..##..###\n\
#....#..#\n";

constexpr puzzleValueType expectedSolution_problem1 = 405;

class Pattern {
private:
    std::vector<std::vector<char> > map;
    std::vector<std::vector<bool> > xorMapX;
    std::vector<std::vector<bool> > xorMapY;
public:
    Pattern(const std::vector<std::vector<char>> & map)
        : map(map)
    {
        for (unsigned int y=0; y < map.size(); y++) {
            std::vector<bool> xorLine;
            for (unsigned int x=1; x < map[0].size(); x++) {
                bool difference = map[y][x-1] != map[y][x];
                xorLine.push_back(difference);
                if (difference) {
                    std::cout << "x";
                } else {
                    std::cout << ".";
                }
            }
            xorMapX.push_back(xorLine);
            std::cout << std::endl;
        }
        std::cout << std::endl;

        for (unsigned int y=1; y < map.size(); y++) {
            std::vector<bool> xorLine;
            for (unsigned int x=0; x < map[0].size(); x++) {
                bool difference = map[y-1][x] != map[y][x];
                xorLine.push_back(difference);
                if (difference) {
                    std::cout << "x";
                } else {
                    std::cout << ".";
                }
            }
            xorMapY.push_back(xorLine);
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    bool findVerticalMirror(puzzleValueType & out) const {    //which mirrors horizontally
        std::set<long int> possibleMirrorIndices;

        for (long int x=0; x < xorMapX[0].size(); x++) {
            bool noDifference = true;
            for (long int y=0; y < xorMapX.size(); y++) {
                if (xorMapX[y][x]) {
                    //difference found
                    noDifference = false;
                    break;
                }
            }
            if (noDifference) {
                possibleMirrorIndices.insert(x);
            }
        }

        if (possibleMirrorIndices.empty()) {
            return false;
        }

        // Find mirror
        bool solutionFound = false;
        for (auto & possibleMirrorIndex : possibleMirrorIndices) {
            bool isMirror = true;
            for (long int x=0; x < xorMapX[0].size(); x++) {
                if (possibleMirrorIndex - x < 0) {
                    break;
                }
                if (possibleMirrorIndex + x >= xorMapX[0].size()) {
                    break;
                }
                for (long int y=0; y < xorMapX.size(); y++) {
                    bool leftSide = xorMapX[y][possibleMirrorIndex - x];
                    bool rightSide = xorMapX[y][possibleMirrorIndex + x];
                    bool difference = leftSide != rightSide;
                    if (difference) {
                        isMirror = false;
                        break;
                    }
                }
                if (!isMirror) {
                    break;
                }
            }
            if (isMirror) {
                assert(!solutionFound);
                solutionFound = true;
                out = possibleMirrorIndex;
                break;
            }
        }

        return solutionFound;
    }

    bool findHorizontalMirror(puzzleValueType & out) const {    //which mirrors vertically
        std::set<long int> possibleMirrorIndices;

        for (long int y=0; y < xorMapY.size(); y++) {
            bool noDifference = true;
            for (long int x=0; x < xorMapY[0].size(); x++) {
                if (xorMapY[y][x]) {
                    //difference found
                    noDifference = false;
                    break;
                }
            }
            if (noDifference) {
                possibleMirrorIndices.insert(y);
            }
        }

        if (possibleMirrorIndices.empty()) {
            return false;
        }

        // Find mirror
        bool solutionFound = false;
        for (auto & possibleMirrorIndex : possibleMirrorIndices) {
            bool isMirror = true;
            for (long int y=0; y < xorMapY.size(); y++) {
                if (possibleMirrorIndex - y < 0) {
                    break;
                }
                if (possibleMirrorIndex + y >= xorMapY.size()) {
                    break;
                }
                for (long int x=0; x < xorMapY[0].size(); x++) {
                    bool upSide = xorMapY[possibleMirrorIndex - y][x];
                    bool downSide = xorMapY[possibleMirrorIndex + y][x];
                    bool difference = upSide != downSide;
                    if (difference) {
                        isMirror = false;
                        break;
                    }
                }
                if (!isMirror) {
                    break;
                }
            }
            if (isMirror) {
                assert(!solutionFound);
                solutionFound = true;
                out = possibleMirrorIndex;
                break;
            }
        }
        return solutionFound;
    }

    void print() const {
        CoutUtils::print(map);
        std::cout << std::endl;
        CoutUtils::print(xorMapX);
        std::cout << std::endl;
        CoutUtils::print(xorMapY);
    }
};

template<typename T>
puzzleValueType solve1(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    std::vector<Pattern> patterns;
    std::vector<std::vector<char> > map;
    for (const auto & line : lines) {
        if (line.empty()) {
            if (!map.empty()) {
                patterns.emplace_back(map);
                map.clear();
            }
        } else {
            std::vector<char> lineAsVector(line.begin(), line.end());
            map.push_back(lineAsVector);
        }
    }

    if (!map.empty()) {
        patterns.emplace_back(map);
        map.clear();
    }

    // Find mirror positions
    puzzleValueType puzzleValue = 0;
    constexpr puzzleValueType horizontalMultiplier = 100;
    for (const auto & pattern : patterns) {
        std::cout << "-" << std::endl;
        pattern.print();
        puzzleValueType index = 0;
        int mirrorCount = 0;
        if (pattern.findVerticalMirror(index)) {
            mirrorCount++;
            puzzleValue += (index + 1);
        }

        if (pattern.findHorizontalMirror(index)) {
            mirrorCount++;
            puzzleValue += (index + 1) * horizontalMultiplier;
        }
        assert(mirrorCount == 1);
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
