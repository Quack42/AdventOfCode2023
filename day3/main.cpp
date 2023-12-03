#include "CoordinateUtils.hpp"
#include "CoutUtils.hpp"
#include "NumberUtils.hpp"
#include "StringUtils.hpp"
#include "VectorUtils.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

// --- Day 3: Gear Ratios ---
// You and the Elf eventually reach a gondola lift station;
//  he says the gondola lift will take you up to the water source,
//  but this is as far as he can bring you.
// You go inside.

// It doesn't take long to find the gondolas,
//  but there seems to be a problem:
//  they're not moving.

// "Aaah!"

// You turn around to see a slightly-greasy Elf with a wrench and a look of surprise.
// "Sorry, I wasn't expecting anyone!
//  The gondola lift isn't working right now;
//  it'll still be a while before I can fix it."
// You offer to help.

// The engineer explains that an engine part seems to be missing from the engine,
//  but nobody can figure out which one.
// If you can add up all the part numbers in the engine schematic,
//  it should be easy to work out which part is missing.

// The engine schematic (your puzzle input) consists of a visual representation of the engine.
// There are lots of numbers and symbols you don't really understand,
//  but apparently any number adjacent to a symbol,
//  even diagonally, is a "part number" and should be included in your sum.
// (Periods (.) do not count as a symbol.)

// Here is an example engine schematic:

// 467..114..
// ...*......
// ..35..633.
// ......#...
// 617*......
// .....+.58.
// ..592.....
// ......755.
// ...$.*....
// .664.598..

// In this schematic, two numbers are not part numbers because they are not adjacent to a symbol:
//  114 (top right)
//  and 58 (middle right).
// Every other number is adjacent to a symbol and so is a part number; their sum is 4361.

// Of course, the actual engine schematic is much larger.
// What is the sum of all of the part numbers in the engine schematic?

const std::string givenTestData_problem1 = "\
467..114..\n\
...*......\n\
..35..633.\n\
......#...\n\
617*......\n\
.....+.58.\n\
..592.....\n\
......755.\n\
...$.*....\n\
.664.598..\n";

constexpr int expectedSolution_problem1 = 4361;


class Symbol {
private:
    char symbol;
    int row;
    int column;
public:
    Symbol(char symbol, int row, int column)
        : symbol(symbol),
        row(row),
        column(column)
    {
        // CoutUtils::print("symbol", symbol);
        // CoutUtils::print("row", row);
        // CoutUtils::print("column", column);
        // std::cout << std::endl;
    }

    char getSymbol() const {
        return symbol;
    }

    CoordinateUtils::ICoordinates getCoordinates() const {
        return {column, row};
    }
};

class Number {
private:
    int numberValue;
    int row;                    //y
    int columnLeftMostDigit;    //x
    int width;                  //x_width
public:
    Number(int numberValue, int row, int columnLeftMostDigit, int width)
        : numberValue(numberValue),
        row(row),
        columnLeftMostDigit(columnLeftMostDigit),
        width(width)
    {
        // CoutUtils::print("numberValue", numberValue);
        // CoutUtils::print("row", row);
        // CoutUtils::print("columnLeftMostDigit", columnLeftMostDigit);
        // CoutUtils::print("width", width);
        // std::cout << std::endl;
    }

    bool isAdjacent(const Symbol & symbol) const {
        for (int i=0; i < width; i++) {
            if (CoordinateUtils::isAdjacent({columnLeftMostDigit + i, row}, symbol.getCoordinates())) {
                return true;
            }
        }
        return false;
    }

    int getNumberValue() const {
        return numberValue;
    }
};

std::string collectDigits(const std::string & line, const int index) {
    std::string ret;
    if (index >= line.size()) {
        return {};
    }

    if (isdigit(line[index])) {
        ret += line[index];
        auto nextDigits = collectDigits(line, index+1);
        ret += nextDigits;
    }

    return ret;
}

std::vector<Number> parseLineForNumbers(const std::string & line, const int & row) {
    std::vector<Number> ret;

    for (unsigned int i=0; i < line.size(); i++) {
        const char & c = line[i];
        if (isdigit(c)) {
            const std::string digits = collectDigits(line, i);
            assert(!digits.empty());

            // convert digits into a number
            const int number = std::stoi(digits);
            
            // Create Number object
            ret.push_back(Number(number, row, i, digits.size()));

            // adjust index appropriately to skip the digits we already processed.
            i += (digits.size() - 1);   //strictly we can leave out the -1, as the next character would either be a non-digit, or the line would run out.
        }
    }

    return ret;
}

std::vector<Symbol> parseLineForSymbols(const std::string & line, const int & row) {
    std::vector<Symbol> ret;

    for (unsigned int i=0; i < line.size(); i++) {
        const char & c = line[i];
        if (!isdigit(c) && c != '.') {
            // Create Symbol object
            ret.push_back(Symbol(c, row, i));
        }
    }

    return ret;
}


template<typename T>
int solve1(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    // parse lines
    std::vector<Number> numbers;
    std::vector<Symbol> symbols;
    for (unsigned int i=0; i < lines.size(); i++) {
        const std::string & line = lines[i];
        auto newNumbers = parseLineForNumbers(line, i);
        numbers = VectorUtils::concatenate(numbers, newNumbers);
        auto newSymbols = parseLineForSymbols(line, i);
        symbols = VectorUtils::concatenate(symbols, newSymbols);
    }

    // Find numbers with a symbol
    std::vector<Number> numbersWithSymbol;
    for (const Number & number : numbers) {
        for (const Symbol & symbol : symbols) {
            if (number.isAdjacent(symbol)) {
                numbersWithSymbol.push_back(number);
            }
        }
    }

    // Calculate sum of part numbers
    int puzzleResult = 0;
    for (const Number & numberWithSymbol : numbersWithSymbol) {
        puzzleResult += numberWithSymbol.getNumberValue();
    }

    // std::cout << CoutUtils::convertToString(
    //         std::string("numbersWithSymbol"),
    //         numbersWithSymbol, 
    //         std::function<std::string(const Number &)>([](const Number & number){
    //             return std::to_string(number.getNumberValue());
    //         })) << std::endl;

    return puzzleResult;
}

// --- Part Two ---
// The engineer finds the missing part and installs it in the engine!
// As the engine springs to life, you jump in the closest gondola,
//  finally ready to ascend to the water source.

// You don't seem to be going very fast, though.
// Maybe something is still wrong?
// Fortunately, the gondola has a phone labeled "help",
//  so you pick it up and the engineer answers.

// Before you can explain the situation, she suggests that you look out the window.
// There stands the engineer, holding a phone in one hand and waving with the other.
// You're going so slowly that you haven't even left the station.
// You exit the gondola.

// The missing part wasn't the only issue - one of the gears in the engine is wrong.
// A gear is any * symbol that is adjacent to exactly two part numbers.
// Its gear ratio is the result of multiplying those two numbers together.

// This time, you need to find the gear ratio of every gear and add them all up
//  so that the engineer can figure out which gear needs to be replaced.

// Consider the same engine schematic again:

// 467..114..
// ...*......
// ..35..633.
// ......#...
// 617*......
// .....+.58.
// ..592.....
// ......755.
// ...$.*....
// .664.598..

// In this schematic, there are two gears.
// The first is in the top left; it has part numbers 467 and 35, so its gear ratio is 16345.
// The second gear is in the lower right; its gear ratio is 451490.
// (The * adjacent to 617 is not a gear because it is only adjacent to one part number.)
// Adding up all of the gear ratios produces 467835.

// What is the sum of all of the gear ratios in your engine schematic?

constexpr int expectedSolution_problem2 = 467835;
const std::string & givenTestData_problem2 = givenTestData_problem1;

template<typename T>
int solve2(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    // parse lines
    std::vector<Number> numbers;
    std::vector<Symbol> symbols;
    for (unsigned int i=0; i < lines.size(); i++) {
        const std::string & line = lines[i];
        auto newNumbers = parseLineForNumbers(line, i);
        numbers = VectorUtils::concatenate(numbers, newNumbers);
        auto newSymbols = parseLineForSymbols(line, i);
        symbols = VectorUtils::concatenate(symbols, newSymbols);
    }

    // Find gears, calculate their gear ratio and sum it. Might need a bigger datatype than int for this?
    int puzzleResult = 0;
    for (const Symbol & symbol : symbols) {
        if (symbol.getSymbol() != '*') {
            continue;
        }

        // find all adjacent numbers
        std::vector<Number> numbersAdjacentToPotentialGear;
        for (const Number & number : numbers) {
            if (number.isAdjacent(symbol)) {
                numbersAdjacentToPotentialGear.push_back(number);
            }
        }

        // check if it's a gear
        if (numbersAdjacentToPotentialGear.size() == 2) {
            // It's a gear!
            puzzleResult += (numbersAdjacentToPotentialGear[0].getNumberValue() * numbersAdjacentToPotentialGear[1].getNumberValue());
        }
    }

    return puzzleResult;
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