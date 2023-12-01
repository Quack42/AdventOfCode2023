#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <vector>

// Something is wrong with global snow production, and you've been selected to take a look.
// The Elves have even given you a map; on it, they've used stars to mark the top fifty locations that are likely to be having problems.

// You've been doing this long enough to know that to restore snow operations, you need to check all fifty stars by December 25th.

// Collect stars by solving puzzles. Two puzzles will be made available on each day in the Advent calendar;
// the second puzzle is unlocked when you complete the first. Each puzzle grants one star. Good luck!

// You try to ask why they can't just use a weather machine ("not powerful enough") and where they're even sending you ("the sky") and 
//  why your map looks mostly blank ("you sure ask a lot of questions") and 
//  hang on did you just say the sky ("of course, where do you think snow comes from") when you realize that the Elves are already loading 
//  you into a trebuchet ("please hold still, we need to strap you in").

// As they're making the final adjustments, they discover that their calibration document (your puzzle input) has been amended by a very 
// young Elf who was apparently just excited to show off her art skills.
// Consequently, the Elves are having trouble reading the values on the document.

// The newly-improved calibration document consists of lines of text; each line originally contained a specific calibration value that the
//  Elves now need to recover. On each line, the calibration value can be found by combining the first digit and the last digit (in that order) to form a single two-digit number.

// For example:

// 1abc2
// pqr3stu8vwx
// a1b2c3d4e5f
// treb7uchet

// In this example, the calibration values of these four lines are 12, 38, 15, and 77. Adding these together produces 142.

// Consider your entire calibration document. What is the sum of all of the calibration values?

// std::string givenTestData = "\
// 1abc2\n\
// pqr3stu8vwx\n\
// a1b2c3d4e5f\n\
// treb7uchet";

std::string givenTestData = "\
two1nine\n\
eightwothree\n\
abcone2threexyz\n\
xtwone3four\n\
4nineeightseven2\n\
zoneight234\n\
7pqrstsixteen\n";

std::vector<std::pair<int, int>> getPairs(std::vector<std::string> inputLines) {
    std::vector<std::pair<int, int>> ret;
    for(const std::string & line : inputLines) {
        auto iFirst = line.find_first_of("0123456789");
        int first = line[iFirst] - '0';
        auto iLast = line.find_last_of("0123456789");
        int last = line[iLast] - '0';

        ret.push_back({first, last});
    }

    return ret;
}

template<typename T>
void solve1(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    //get pairs
    auto pairs = getPairs(lines);

    // print pairs
    for (auto [first, last] : pairs) {
        std::cout << "first:[" << first << "], last:[" << last << "]" << std::endl;
    }

    // sum
    int sum = 0;
    for (auto [first, last] : pairs) {
        sum += (first*10);
        sum += last;
        // std::cout << sum << std::endl;
    }

    std::cout << sum << std::endl;
}


std::pair<int, int> findValueOfFirstStringDigit(const std::string & str) {
    const std::vector<std::string> numbers = {
        "zero",
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine"
    };
    std::vector<std::pair<int,int>> indices;
    for (unsigned int i=0; i < numbers.size(); i++) {
        auto numberString = numbers[i];
    
        auto index = str.find(numberString);
        if (index != std::string::npos) {
            indices.push_back({index, i});
        }
    }
    std::pair<int,int> ret = {std::string::npos, 0};
    int lowestIndex = str.size();
    for (auto pair : indices) {
        auto index = pair.first;
        if (index < lowestIndex) {
            lowestIndex = index;
            ret = pair;
        }
    }

    return ret;
}

std::pair<int, int> findValueOfLastStringDigit(const std::string & str) {
    const std::vector<std::string> numbers = {
        "zero",
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine"
    };
    std::vector<std::pair<int,int>> indices;
    for (unsigned int i=0; i < numbers.size(); i++) {
        auto numberString = numbers[i];
    
        auto index = str.rfind(numberString);
        if (index != std::string::npos) {
            indices.push_back({index, i});
            std::cout << i << std::endl;
        }
    }

    std::pair<int,int> ret = {std::string::npos, 0};
    int highestIndex = 0;
    for (auto pair : indices) {
        auto index = pair.first;
        if (index > highestIndex) {
            highestIndex = index;
            ret = pair;
        }
    }

    return ret;
}

std::vector<std::pair<int, int>> getPairsWithString(const std::vector<std::string> & inputLines) {
    std::vector<std::pair<int, int>> ret;
    for(const std::string & line : inputLines) {
        std::cout << line << std::endl;

        auto iFirstDigit = line.find_first_of("0123456789");
        auto firstString = findValueOfFirstStringDigit(line);

        int first;
        std::cout << "iFirstDigit: " << iFirstDigit << std::endl;
        std::cout << "firstString.first: " << firstString.first << std::endl;
        if ((iFirstDigit != std::string::npos && iFirstDigit < firstString.first) || firstString.first == std::string::npos){
            first = line[iFirstDigit] - '0';
        } else {
            first = firstString.second;
        }


        auto iLastDigit = line.find_last_of("0123456789");
        auto lastString = findValueOfLastStringDigit(line);

        std::cout << "iLastDigit: " << iLastDigit << std::endl;
        std::cout << "lastString.first: " << lastString.first << std::endl;
        int last;
        if ((iLastDigit != std::string::npos && iLastDigit > lastString.first) || lastString.first == std::string::npos){
            last = line[iLastDigit] - '0';
        } else {
            last = lastString.second;
        }
        std::cout << "first: " << first << std::endl;
        std::cout << "last: " << last << std::endl;

        ret.push_back({first, last});
        std::cout << std::endl;
    }

    return ret;
}

template<typename T>
void solve2(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    //get pairs
    auto pairs = getPairsWithString(lines);

    // print pairs
    for (auto [first, last] : pairs) {
        std::cout << "first:[" << first << "], last:[" << last << "]" << std::endl;
    }

    // sum
    int sum = 0;
    for (auto [first, last] : pairs) {
        sum += (first*10);
        sum += last;
        // std::cout << sum << std::endl;
    }

    std::cout << sum << std::endl;
}

// Your calculation isn't quite right. It looks like some of the digits are actually spelled out with letters:
// one, two, three, four, five, six, seven, eight, and nine also count as valid "digits".

// Equipped with this new information, you now need to find the real first and last digit on each line. For example:

// two1nine
// eightwothree
// abcone2threexyz
// xtwone3four
// 4nineeightseven2
// zoneight234
// 7pqrstsixteen



int main(int argc, char ** argv) {
    std::stringstream inputStream;
    if (argc == 1) {
        // std::stringstream strStream(givenTestData);
        // solve1(strStream);
        std::stringstream strStream(givenTestData);
        solve2(strStream);
    } else if (argc == 2 || argc == 3) {
        std::string url(argv[1]);
        std::cout << url << std::endl;
        std::filesystem::path path(url);

        std::ifstream fileStream;
        fileStream.open(path);
        if (fileStream.is_open()) {
            std::cout << "IsOpen" << std::endl;
        } else {
            std::cout << "Is NOT Open" << std::endl;
        }
        if (argc == 2 || (argc == 3 && argv[2][0] == 'A')) {
            solve1(fileStream);
        } else if(argc == 3 && argv[2][0] == 'B') {
            solve2(fileStream);
            std::cout << "solve2" << std::endl;
        }
    }

    return 0;
}