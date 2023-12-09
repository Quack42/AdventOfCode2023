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

// --- Day 9: Mirage Maintenance ---
// You ride the camel through the sandstorm and stop where the ghost's maps told you to stop.
// The sandstorm subsequently subsides, somehow seeing you standing at an oasis!

// The camel goes to get some water and you stretch your neck.
// As you look up, you discover what must be yet another giant floating island, this one made of metal!
// That must be where the parts to fix the sand machines come from.

// There's even a hang glider partially buried in the sand here; once the sun rises and heats up the sand,
// you might be able to use the glider and the hot air to get all the way up to the metal island!

// While you wait for the sun to rise, you admire the oasis hidden here in the middle of Desert Island.
// It must have a delicate ecosystem; you might as well take some ecological readings while you wait.
// Maybe you can report any environmental instabilities you find to someone so the oasis can be around for the next sandstorm-worn traveler.

// You pull out your handy Oasis And Sand Instability Sensor and analyze your surroundings.
// The OASIS produces a report of many values and how they are changing over time (your puzzle input).
// Each line in the report contains the history of a single value. For example:

// 0 3 6 9 12 15
// 1 3 6 10 15 21
// 10 13 16 21 30 45
// To best protect the oasis, your environmental report should include a prediction of the next value in each history.
// To do this, start by making a new sequence from the difference at each step of your history.
// If that sequence is not all zeroes, repeat this process, using the sequence you just generated as the input sequence.
// Once all of the values in your latest sequence are zeroes,
//  you can extrapolate what the next value of the original history should be.

// In the above dataset, the first history is 0 3 6 9 12 15.
// Because the values increase by 3 each step, the first sequence of differences that you generate will be 3 3 3 3 3.
// Note that this sequence has one fewer value than the input sequence because at each step it considers two numbers from the input.
// Since these values aren't all zero, repeat the process: the values differ by 0 at each step,
// so the next sequence is 0 0 0 0.
// This means you have enough information to extrapolate the history!
// Visually, these sequences can be arranged like this:

// 0   3   6   9  12  15
//   3   3   3   3   3
//     0   0   0   0
// To extrapolate, start by adding a new zero to the end of your list of zeroes;
//  because the zeroes represent differences between the two values above them,
// this also means there is now a placeholder in every sequence above it:

// 0   3   6   9  12  15   B
//   3   3   3   3   3   A
//     0   0   0   0   0
// You can then start filling in placeholders from the bottom up.
// A needs to be the result of increasing 3 (the value to its left) by 0 (the value below it); this means A must be 3:

// 0   3   6   9  12  15   B
//   3   3   3   3   3   3
//     0   0   0   0   0
// Finally, you can fill in B, which needs to be the result of increasing 15 (the value to its left) by 3 (the value below it), or 18:

// 0   3   6   9  12  15  18
//   3   3   3   3   3   3
//     0   0   0   0   0
// So, the next value of the first history is 18.

// Finding all-zero differences for the second history requires an additional sequence:

// 1   3   6  10  15  21
//   2   3   4   5   6
//     1   1   1   1
//       0   0   0
// Then, following the same process as before, work out the next value in each sequence from the bottom up:

// 1   3   6  10  15  21  28
//   2   3   4   5   6   7
//     1   1   1   1   1
//       0   0   0   0
// So, the next value of the second history is 28.

// The third history requires even more sequences, but its next value can be found the same way:

// 10  13  16  21  30  45  68
//    3   3   5   9  15  23
//      0   2   4   6   8
//        2   2   2   2
//          0   0   0
// So, the next value of the third history is 68.

// If you find the next value for each history in this example and add them together, you get 114.

// Analyze your OASIS report and extrapolate the next value for each history.
// What is the sum of these extrapolated values?

using puzzleValueType = long long int;

const std::string givenTestData_problem1 = "\
0 3 6 9 12 15\n\
1 3 6 10 15 21\n\
10 13 16 21 30 45\n";
constexpr puzzleValueType expectedSolution_problem1 = 114;

class Sequence {
private:
    std::vector<puzzleValueType> sequence;
public:
    Sequence(const std::string & line) {
        auto lineFractions = StringUtils::split(line, " ");
        for (auto & lineFraction : lineFractions) {
            sequence.push_back(std::stoll(lineFraction));
        }
    }

    Sequence(std::vector<puzzleValueType> && input) {
        std::swap(sequence, input);
    }


    Sequence getDifferenceSequence() const {
        std::vector<puzzleValueType> differenceSequence;
        for (unsigned int i=1; i < sequence.size(); i++) {
            const auto & previousValue = sequence[i-1];
            const auto & value = sequence[i];
            differenceSequence.push_back(value-previousValue);
        }
        return Sequence(std::move(differenceSequence));
    }

    bool isAllZeros() const {
        for (const auto & val : sequence) {
            if (val != 0) {
                return false;
            }
        }
        return true;
    }

    puzzleValueType predictNextValue() const {
        if (isAllZeros()) {
            return 0;
        } else {
            Sequence differenceSequence = getDifferenceSequence();
            auto nextValueDifference = differenceSequence.predictNextValue();
            auto nextValue = sequence[sequence.size()-1] + nextValueDifference;     // add to last
            return nextValue;
        }
    }
};

template<typename T>
puzzleValueType solve1(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    // convert to sequences
    std::vector<Sequence> startingSequences;
    for (const auto & line : lines) {
        startingSequences.push_back(Sequence(line));
    }

    puzzleValueType puzzleValue = 0;
    // Compute puzzel value
    for (const auto & sequence : startingSequences) {
        puzzleValue += sequence.predictNextValue();
    }

    return puzzleValue;
}



// The input for puzzle 2 is the same as for puzzle 1.

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
