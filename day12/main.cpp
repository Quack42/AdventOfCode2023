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

// --- Day 12: Hot Springs ---
// You finally reach the hot springs!
// You can see steam rising from secluded areas attached to the primary, ornate building.

// As you turn to enter, the researcher stops you.
// "Wait - I thought you were looking for the hot springs, weren't you?"
// You indicate that this definitely looks like hot springs to you.

// "Oh, sorry, common mistake! This is actually the onsen! The hot springs are next door."

// You look in the direction the researcher is pointing and suddenly notice the massive metal helixes towering overhead.
// "This way!"

// It only takes you a few more steps to reach the main gate of the massive fenced-off area containing the springs.
// You go through the gate and into a small administrative building.

// "Hello! What brings you to the hot springs today?
//  Sorry they're not very hot right now;
//  we're having a lava shortage at the moment."
// You ask about the missing machine parts for Desert Island.

// "Oh, all of Gear Island is currently offline!
// Nothing is being manufactured at the moment,
//  not until we get more lava to heat our forges.
// And our springs.
// The springs aren't very springy unless they're hot!"

// "Say, could you go up and see why the lava stopped flowing?
//  The springs are too cold for normal operation,
//   but we should be able to find one springy enough to launch you up there!"

// There's just one problem
// - many of the springs have fallen into disrepair,
//  so they're not actually sure which springs would even be safe to use!
// Worse yet, their condition records of which springs are damaged (your puzzle input) are also damaged!
// You'll need to help them repair the damaged records.

// In the giant field just outside, the springs are arranged into rows.
// For each row, the condition records show every spring and whether it is operational (.) or damaged (#).
// This is the part of the condition records that is itself damaged;
//  for some springs, it is simply unknown (?) whether the spring is operational or damaged.

// However, the engineer that produced the condition records also duplicated
//  some of this information in a different format!
// After the list of springs for a given row,
//  the size of each contiguous group of damaged springs is listed in the order those groups appear in the row.
// This list always accounts for every damaged spring,
//  and each number is the entire size of its contiguous group
// (that is, groups are always separated by at least one operational spring:
//  #### would always be 4, never 2,2).

// So, condition records with no unknown spring conditions might look like this:

// #.#.### 1,1,3
// .#...#....###. 1,1,3
// .#.###.#.###### 1,3,1,6
// ####.#...#... 4,1,1
// #....######..#####. 1,6,5
// .###.##....# 3,2,1

// However, the condition records are partially damaged; some of the springs' conditions are actually unknown (?). For example:

// ???.### 1,1,3
// .??..??...?##. 1,1,3
// ?#?#?#?#?#?#?#? 1,3,1,6
// ????.#...#... 4,1,1
// ????.######..#####. 1,6,5
// ?###???????? 3,2,1

// Equipped with this information,
//  it is your job to figure out how many different arrangements of operational and broken springs fit the given criteria in each row.

// In the first line (???.### 1,1,3),
//  there is exactly one way separate groups of one, one, and three broken springs (in that order) can appear in that row:
//  the first three unknown springs must be broken,
//  then operational,
//  then broken (#.#),
//  making the whole row #.#.###.

// The second line is more interesting:
//  .??..??...?##. 1,1,3 could be a total of four different arrangements.
// The last ? must always be broken (to satisfy the final contiguous group of three broken springs),
//  and each ?? must hide exactly one of the two broken springs.
// (Neither ?? could be both broken springs or they would form a single contiguous group of two;
//  if that were true, the numbers afterward would have been 2,3 instead.)
// Since each ?? can either be #. or .#, there are four possible arrangements of springs.

// The last line is actually consistent with ten different arrangements!
//  Because the first number is 3, the first and second ? must both be . (if either were #, the first number would have to be 4 or higher).
// However, the remaining run of unknown spring conditions have many different ways they could hold groups of two and one broken springs:

// ?###???????? 3,2,1
// .###.##.#...
// .###.##..#..
// .###.##...#.
// .###.##....#
// .###..##.#..
// .###..##..#.
// .###..##...#
// .###...##.#.
// .###...##..#
// .###....##.#
// In this example, the number of possible arrangements for each row is:

// ???.### 1,1,3 - 1 arrangement
// .??..??...?##. 1,1,3 - 4 arrangements
// ?#?#?#?#?#?#?#? 1,3,1,6 - 1 arrangement
// ????.#...#... 4,1,1 - 1 arrangement
// ????.######..#####. 1,6,5 - 4 arrangements
// ?###???????? 3,2,1 - 10 arrangements
// Adding all of the possible arrangement counts together produces a total of 21 arrangements.

// For each row, count all of the different arrangements of operational and broken springs that meet the given criteria. What is the sum of those counts?

const std::string givenTestData_problem1 = "\
???.### 1,1,3\n\
.??..??...?##. 1,1,3\n\
?#?#?#?#?#?#?#? 1,3,1,6\n\
????.#...#... 4,1,1\n\
????.######..#####. 1,6,5\n\
?###???????? 3,2,1\n";
constexpr puzzleValueType expectedSolution_problem1 = 21;

constexpr char UNKNOWN = '?';
constexpr char BROKEN = '#';
constexpr char WORKING = '.';

class SpringRow {
private:
    std::vector<int> sequences;
    std::string originalRow;

public:
    SpringRow(const std::string & line) {
        auto sections = StringUtils::split(line, " ");
        // sequences
        auto sequenceStrings = StringUtils::split(sections[1], ",");
        for (const auto & sequenceString : sequenceStrings) {
            int sequenceLength = std::stoi(sequenceString);
            if (sequenceLength > 0) {
                sequences.push_back(sequenceLength);
            }
        }
        // original row
        originalRow = sections[0];

        //print to confirm
        std::cout << "originalRow:[" << originalRow << "]" << std::endl;
        std::cout << CoutUtils::convertToString("sequences", sequences) << std::endl;
    }

    puzzleValueType countSolutions() const {
        puzzleValueType ret = 0;
        ret = recursiveSolutionSearch(originalRow, 0, 0, calculateMinimalSequenceLength());
        return ret;
    }

private:
    int calculateMinimalSequenceLength() const {
        int ret = 0;
        for (unsigned int i=0; i < sequences.size(); i++) {
            ret += sequences[i];
        }
        assert(!sequences.empty());
        ret += sequences.size()-1;  //for the holes in between the sequences
        return ret;
    }

    puzzleValueType recursiveSolutionSearch(std::string row, int placementIndex, int sequenceIndex, int minimumSequenceLength) const {
        puzzleValueType ret = 0;
        
        if (sequenceIndex >= sequences.size()) {
            if (validateSolution(row, sequences)) {
                return 1;
            } else {
                return 0;
            }
        }

        int springsLeftToParse = row.size() - (placementIndex + minimumSequenceLength) + 1;     //not sure about the +1 ...

        if (springsLeftToParse < 1) {
            //doesn't fit anymore
            return 0;
        }

        int currentSequenceLength = sequences[sequenceIndex];
        
        for (unsigned int i=0; i < springsLeftToParse; i++) {

            bool placeable = true;
            for (unsigned int i2=0; i2 < currentSequenceLength; i2++) {
                const auto & spring = row[placementIndex+i+i2];
                if ((spring != BROKEN) && (spring != UNKNOWN)) {
                    //i.e. the spring is WORKING
                    placeable = false;
                    break;
                }
            }

            if (placeable) {
                std::string rowCopy = row;
                for (unsigned int i2=0; i2 < currentSequenceLength; i2++) {
                    rowCopy[placementIndex+i+i2] = BROKEN;
                }
                int newMinimumSequenceLength = minimumSequenceLength-currentSequenceLength;
                assert(newMinimumSequenceLength!=1);
                if (newMinimumSequenceLength > 1) {
                    newMinimumSequenceLength--;
                }
                ret += recursiveSolutionSearch(rowCopy, placementIndex+i+currentSequenceLength+1, sequenceIndex+1, newMinimumSequenceLength);
            }
        }

        return ret;
    }

public:
    static bool validateSolution(const std::string & row, const std::vector<int> & sequences) {
        int sequenceIndex = 0;
        for (unsigned int i=0; i < row.size(); i++) {
            // Only interested in BROKEN sections.
            if (row[i] != BROKEN) {
                continue;
            } else if (sequenceIndex >= sequences.size()) {
                // Found a broken section, but there shouldn't be any left.
                return false;
            }

            // Count the length of the broken section.
            int brokenLength = 0;
            while (row[i] == BROKEN && i < row.size()) {
                brokenLength++;
                i++;
            }

            // Match it with what's in the sequence.
            if (brokenLength != sequences[sequenceIndex]) {
                return false;
            }
            sequenceIndex++;
        }
        return true;
    }
};

template<typename T>
puzzleValueType solve1(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    // Convert lines to spring rows
    std::vector<SpringRow> springRows;
    for (const auto & line : lines) {
        springRows.emplace_back(line);
    }

    // SpringRow::validateSolution function validation.
    // std::cout << "1st: " << SpringRow::validateSolution("#.#.###", {1,1,3}) << std::endl;   //1
    // std::cout << "2nd: " << SpringRow::validateSolution("##.#.###", {1,1,3}) << std::endl;  //0
    // std::cout << "3nd: " << SpringRow::validateSolution(".#.#.###", {1,1,3}) << std::endl;  //1
    // std::cout << "4nd: " << SpringRow::validateSolution(".#.#.##.#", {1,1,3}) << std::endl; //0
    // std::cout << "5nd: " << SpringRow::validateSolution(".#.###.#", {1,1,3}) << std::endl; //0
    // std::cout << "6nd: " << SpringRow::validateSolution(".#.#.###.#", {1,1,3}) << std::endl; //0
    // std::cout << "7nd: " << SpringRow::validateSolution(".#.#.###.#", {1,1,3,1}) << std::endl; //1

    // Count solutions
    puzzleValueType puzzleValue = 0;
    for (const auto & springRow : springRows) {
        puzzleValue += springRow.countSolutions();
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
