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

    bool findVerticalMirror(puzzleValueType & out, unsigned int numberOfSmudges = 0) const {    //which mirrors horizontally
        std::cout << "vertical" << std::endl;

        std::set<long int> possibleMirrorIndices;

        for (long int x=0; x < xorMapX[0].size(); x++) {
            unsigned int differenceCount = 0;
            bool noDifference = true;
            for (long int y=0; y < xorMapX.size(); y++) {
                if (xorMapX[y][x]) {
                    //difference found
                    differenceCount++;
                    if (differenceCount > numberOfSmudges) {
                        noDifference = false;
                        break;
                    }
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
            PRINT(possibleMirrorIndex);
            bool isMirror = true;
            unsigned int differenceCount = 0;
            for (long int x=0; x < map[0].size(); x++) {
                if (possibleMirrorIndex - x < 0) {
                    break;
                }
                if (possibleMirrorIndex + x + 1 >= map[0].size()) {
                    break;
                }
                for (long int y=0; y < map.size(); y++) {
                    const auto & leftSide = map[y][possibleMirrorIndex - x];
                    const auto & rightSide = map[y][possibleMirrorIndex + 1 + x];
                    bool difference = leftSide != rightSide;
                    if (difference) {
                        differenceCount++;
                        if (differenceCount > numberOfSmudges) {
                            isMirror = false;
                            break;
                        }
                    }
                }
                if (!isMirror) {
                    break;
                }
            }
            if (isMirror && differenceCount == numberOfSmudges) {
                assert(!solutionFound);
                solutionFound = true;
                out = possibleMirrorIndex;
                break;
            }
        }

        return solutionFound;
    }

    bool findHorizontalMirror(puzzleValueType & out, unsigned int numberOfSmudges = 0) const {    //which mirrors vertically
        std::cout << "horizontal" << std::endl;
        std::set<long int> possibleMirrorIndices;

        for (long int y=0; y < xorMapY.size(); y++) {
            unsigned int differenceCount = 0;
            bool noDifference = true;
            for (long int x=0; x < xorMapY[0].size(); x++) {
                if (xorMapY[y][x]) {
                    //difference found
                    differenceCount++;
                    if (differenceCount > numberOfSmudges) {
                        noDifference = false;
                        break;
                    }
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
            std::cout << std::endl;
            PRINT(possibleMirrorIndex);
            bool isMirror = true;
            unsigned int differenceCount = 0;
            for (long int y=0; y < map.size(); y++) {
                if (possibleMirrorIndex - y < 0) {
                    break;
                }
                if (possibleMirrorIndex + y + 1 >= map.size()) {
                    break;
                }
                for (long int x=0; x < map[0].size(); x++) {
                    const auto & upSide = map[possibleMirrorIndex - y][x];
                    const auto & downSide = map[possibleMirrorIndex + 1 + y][x];
                    bool difference = upSide != downSide;
                    if (difference) {
                        differenceCount++;
                        if (differenceCount > numberOfSmudges) {
                            isMirror = false;
                            break;
                        }
                    }
                }
                if (!isMirror) {
                    break;
                }
            }
            if (isMirror && differenceCount == numberOfSmudges) {
                assert(!solutionFound);
                solutionFound = true;
                out = possibleMirrorIndex;
                break;
            } else {
                std::cout << "is no mirror" << std::endl;
                PRINT(isMirror);
                PRINT(differenceCount);
            }
        }
        return solutionFound;
    }

    void print() const {
        CoutUtils::print2d(map);
        std::cout << std::endl;
        CoutUtils::print2d(xorMapX);
        std::cout << std::endl;
        CoutUtils::print2d(xorMapY);
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
        if (mirrorCount != 1) {
            std::cout << "mirrorCount:" << mirrorCount << std::endl;
        }
        assert(mirrorCount == 1);
    }

    return puzzleValue;
}

// --- Part Two ---
// You resume walking through the valley of mirrors and - SMACK! - run directly into one.
// Hopefully nobody was watching, because that must have been pretty embarrassing.

// Upon closer inspection, you discover that every mirror has exactly one smudge:
//  exactly one . or # should be the opposite type.

// In each pattern, you'll need to locate and fix the smudge that causes a different reflection line to be valid.
//  (The old reflection line won't necessarily continue being valid after the smudge is fixed.)

// Here's the above example again:

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

// The first pattern's smudge is in the top-left corner.
// If the top-left # were instead ., it would have a different, horizontal line of reflection:

// 1 ..##..##. 1
// 2 ..#.##.#. 2
// 3v##......#v3
// 4^##......#^4
// 5 ..#.##.#. 5
// 6 ..##..##. 6
// 7 #.#.##.#. 7

// With the smudge in the top-left corner repaired, a new horizontal line of reflection between rows 3 and 4 now exists.
// Row 7 has no corresponding reflected row and can be ignored, but every other row matches exactly:
// row 1 matches row 6,
// row 2 matches row 5,
// and row 3 matches row 4.

// In the second pattern, the smudge can be fixed by changing the fifth symbol on row 2 from . to #:

// 1v#...##..#v1
// 2^#...##..#^2
// 3 ..##..### 3
// 4 #####.##. 4
// 5 #####.##. 5
// 6 ..##..### 6
// 7 #....#..# 7

// Now, the pattern has a different horizontal line of reflection between rows 1 and 2.

// Summarize your notes as before, but instead use the new different reflection lines.
// In this example, the first pattern's new horizontal line has 3 rows above it and the second pattern's new horizontal line has 1 row above it, summarizing to the value 400.

// In each pattern, fix the smudge and find the different line of reflection.
// What number do you get after summarizing the new reflection line in each pattern in your notes?

// Although it hasn't changed, you can still get your puzzle input.


const std::string givenTestData_problem2 = "\
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

constexpr puzzleValueType expectedSolution_problem2 = 400;

template<typename T>
puzzleValueType solve2(T & stream) {
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
        if (pattern.findVerticalMirror(index, 1)) {
            mirrorCount++;
            puzzleValue += (index + 1);
        }

        if (pattern.findHorizontalMirror(index, 1)) {
            mirrorCount++;
            puzzleValue += (index + 1) * horizontalMultiplier;
        }
        if (mirrorCount != 1) {
            std::cout << "mirrorCount:" << mirrorCount << std::endl;
        }
        assert(mirrorCount == 1);
    }

    return puzzleValue;
}

int main(int argc, char ** argv) {
    std::stringstream inputStream;
    if (argc == 1) {
        std::cout << " --- PUZZLE A --- " << std::endl;
        std::stringstream strStream_problem1(givenTestData_problem1);
        const auto solve1_solution = solve1(strStream_problem1);
        std::cout << " --- PUZZLE B --- " << std::endl;
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
