#include "CoordinateUtils.hpp"
#include "CoutUtils.hpp"
#include "NumberUtils.hpp"
#include "StringUtils.hpp"
#include "VectorUtils.hpp"

#include <cassert>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>


// --- Day 5: If You Give A Seed A Fertilizer ---
// You take the boat and find the gardener right where you were told he would be:
//  managing a giant "garden" that looks more to you like a farm.

// "A water source? Island Island is the water source!"
// You point out that Snow Island isn't receiving any water.

// "Oh, we had to stop the water because we ran out of sand to filter it with!
//  Can't make snow with dirty water.
//  Don't worry, I'm sure we'll get more sand soon;
//  we only turned off the water a few days... weeks... oh no."
// His face sinks into a look of horrified realization.

// "I've been so busy making sure everyone here has food
//  that I completely forgot to check why we stopped getting more sand!
//  There's a ferry leaving soon that is headed over in that direction 
//  - it's much faster than your boat.
// Could you please go check it out?"

// You barely have time to agree to this request when he brings up another.
// "While you wait for the ferry, maybe you can help us with our food production problem.
//  The latest Island Island Almanac just arrived and we're having trouble making sense of it."

// The almanac (your puzzle input) lists all of the seeds that need to be planted.
// It also lists
//  what type of soil to use with each kind of seed,
//  what type of fertilizer to use with each kind of soil,
//  what type of water to use with each kind of fertilizer,
//  and so on.
// Every type of seed, soil, fertilizer and so on is identified with a number,
//  but numbers are reused by each category
// - that is, soil 123 and fertilizer 123 aren't necessarily related to each other.

// For example:

// seeds: 79 14 55 13

// seed-to-soil map:
// 50 98 2
// 52 50 48

// soil-to-fertilizer map:
// 0 15 37
// 37 52 2
// 39 0 15

// fertilizer-to-water map:
// 49 53 8
// 0 11 42
// 42 0 7
// 57 7 4

// water-to-light map:
// 88 18 7
// 18 25 70

// light-to-temperature map:
// 45 77 23
// 81 45 19
// 68 64 13

// temperature-to-humidity map:
// 0 69 1
// 1 0 69

// humidity-to-location map:
// 60 56 37
// 56 93 4

// The almanac starts by listing which seeds need to be planted:
//  seeds 79, 14, 55, and 13.

// The rest of the almanac contains a list of maps 
//  which describe how to convert numbers from a source category into numbers in a destination category.
// That is, the section that starts with seed-to-soil map:
//  describes how to convert a seed number (the source) to a soil number (the destination).
// This lets the gardener and his team know
//  which soil to use with which seeds,
//  which water to use with which fertilizer,
//  and so on.

// Rather than list every source number and its corresponding destination number one by one,
//  the maps describe entire ranges of numbers that can be converted.
// Each line within a map contains three numbers:
//  the destination range start,
//  the source range start,
//  and the range length.

// Consider again the example seed-to-soil map:
// 50 98 2
// 52 50 48

// The first line has
//  a destination range start of 50,
//  a source range start of 98,
//  and a range length of 2.
// This line means that the source range starts at 98 and contains two values: 98 and 99.
// The destination range is the same length,
//  but it starts at 50,
//  so its two values are 50 and 51.
// With this information, you know that
//  seed number 98 corresponds to soil number 50 and that
//  seed number 99 corresponds to soil number 51.

// The second line means that
//  the source range starts at 50 and contains 48 values: 50, 51, ..., 96, 97.
// This corresponds to a destination range starting at 52 and also containing 48 values: 52, 53, ..., 98, 99.
// So, seed number 53 corresponds to soil number 55.

// Any source numbers that aren't mapped correspond to the same destination number.
// So, seed number 10 corresponds to soil number 10.

// So, the entire list of seed numbers and their corresponding soil numbers looks like this:

// seed  soil
// 0     0
// 1     1
// ...   ...
// 48    48
// 49    49
// 50    52
// 51    53
// ...   ...
// 96    98
// 97    99
// 98    50
// 99    51
// With this map, you can look up the soil number required for each initial seed number:

// Seed number 79 corresponds to soil number 81.
// Seed number 14 corresponds to soil number 14.
// Seed number 55 corresponds to soil number 57.
// Seed number 13 corresponds to soil number 13.
// The gardener and his team want to get started as soon as possible,
//  so they'd like to know the closest location that needs a seed.
// Using these maps, find the lowest location number that corresponds to any of the initial seeds.
// To do this, you'll need to convert each seed number through other categories until you can find its corresponding location number.
// In this example, the corresponding types are:
// Seed 79, soil 81, fertilizer 81, water 81, light 74, temperature 78, humidity 78, location 82.
// Seed 14, soil 14, fertilizer 53, water 49, light 42, temperature 42, humidity 43, location 43.
// Seed 55, soil 57, fertilizer 57, water 53, light 46, temperature 82, humidity 82, location 86.
// Seed 13, soil 13, fertilizer 52, water 41, light 34, temperature 34, humidity 35, location 35.
// So, the lowest location number in this example is 35.

// What is the lowest location number that corresponds to any of the initial seed numbers?


//////////////////////////////////////

//Interpretation:
// Convert tables:
// [destination, source, range]
// Find the lowest value for table "location"

const std::string givenTestData_problem1 = "\
seeds: 79 14 55 13\n\
\n\
seed-to-soil map:\n\
50 98 2\n\
52 50 48\n\
\n\
soil-to-fertilizer map:\n\
0 15 37\n\
37 52 2\n\
39 0 15\n\
\n\
fertilizer-to-water map:\n\
49 53 8\n\
0 11 42\n\
42 0 7\n\
57 7 4\n\
\n\
water-to-light map:\n\
88 18 7\n\
18 25 70\n\
\n\
light-to-temperature map:\n\
45 77 23\n\
81 45 19\n\
68 64 13\n\
\n\
temperature-to-humidity map:\n\
0 69 1\n\
1 0 69\n\
\n\
humidity-to-location map:\n\
60 56 37\n\
56 93 4\n\
";

using puzzleValueType = long long int;
constexpr puzzleValueType expectedSolution_problem1 = 35;




class ConversionRange {
private:
    puzzleValueType destination;
    puzzleValueType source;
    puzzleValueType range;
public:
    ConversionRange(const std::string & line) {
        auto values = StringUtils::split(line, " ");
        destination = std::stoll(values[0]);
        source = std::stoll(values[1]);
        range = std::stoll(values[2]);
    }

    bool inRange(puzzleValueType sourceValue) const {
        return (sourceValue >= source) && (sourceValue <= source + range);
    }

    puzzleValueType convert(puzzleValueType sourceValue) const {
        return sourceValue + (destination-source);
    }
};

class Object {
private:
    std::string objectName;
    puzzleValueType value;

public:
    Object(const std::string & objectName, puzzleValueType value) :
        objectName(objectName),
        value(value)
    {
        //do nothing
    }

    std::string getObjectName() const {
        return objectName;
    }

    puzzleValueType getValue() const {
        return value;
    }
};

class ConversionMap {
private:
    std::vector<ConversionRange> conversions;
    std::string fromObjectName;
    std::string toObjectName;

public:
    ConversionMap() {}

    ConversionMap(const std::string & line) {
        // line ~= fertilizer-to-water map:
        assert(StringUtils::contains(line, " map:"));
        auto line_temp1 = StringUtils::remove(line, " map:");
        auto mapNames = StringUtils::split(line_temp1, "-to-");
        fromObjectName = mapNames[0];
        toObjectName = mapNames[1];
    }

    void addConversionRange(const ConversionRange & conversionRange) {
        conversions.push_back(conversionRange);
    }

    Object convert(Object & obj) {
        assert(obj.getObjectName() == fromObjectName);
        const auto objectValue = obj.getValue();
        for (auto & conversion : conversions) {
            if (conversion.inRange(objectValue)) {
                auto convertedValue = conversion.convert(objectValue);
                // conversion found, convert and return
                return Object(toObjectName, convertedValue);
            }
        }
        // no conversion found; 1-to-1 conversion
        return Object(toObjectName, objectValue);
    }

    const std::string & getFromObjectName() const {
        return fromObjectName;
    }
};

template<typename T>
puzzleValueType solve1(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    // Collect information
    std::unordered_map<std::string, ConversionMap> conversionMaps;
    std::vector<Object> objects;
    std::string _lastConversionMapKey = "";
    bool collectingSeeds = false;
    for (const auto & line : lines) {
        if (StringUtils::contains(line, " map:")) {
            // conversion map
            ConversionMap map(line);
            conversionMaps[map.getFromObjectName()] = map;
            _lastConversionMapKey = map.getFromObjectName();
        } else if (StringUtils::contains(line, "seeds:")) {
            auto tempLine1 = StringUtils::remove(line, "seeds: ");
            auto seedNumbers = StringUtils::split(tempLine1, " ");
            for (auto seedNumber : seedNumbers) {
                objects.push_back(Object("seed", std::stoll(seedNumber)));
            }
        } else if (!line.empty()) {
            assert(!_lastConversionMapKey.empty());
            conversionMaps[_lastConversionMapKey].addConversionRange(ConversionRange(line));
        }
    }

    assert(!conversionMaps.empty());
    std::cout << "conversionMaps.size():" << conversionMaps.size() << std::endl;

    // Process objects
    std::vector<Object> finishedObjects;    //for when no more steps can be done
    bool change = false;
    do {
        change = false;
        std::vector<Object> newObjects;
        for (auto object : objects) {
            if (conversionMaps.contains(object.getObjectName())) {
                Object newObject = conversionMaps[object.getObjectName()].convert(object);
                newObjects.push_back(newObject);
                change = true;
            } else {
                finishedObjects.push_back(object);
            }
        }
        objects = newObjects;
    } while (change);

    assert(!finishedObjects.empty());

    // Get lowest value
    puzzleValueType puzzleValue = finishedObjects[0].getValue();
    for (auto object : finishedObjects) {
        puzzleValue = std::min(puzzleValue, object.getValue());
    }

    return puzzleValue;
}

// --- Part Two ---
// Everyone will starve if you only plant such a small number of seeds.
// Re-reading the almanac, it looks like the seeds: line actually describes ranges of seed numbers.

// The values on the initial seeds: line come in pairs.
// Within each pair, the first value is the start of the range and the second value is the length of the range.
// So, in the first line of the example above:

// seeds: 79 14 55 13
// This line describes two ranges of seed numbers to be planted in the garden.
// The first range starts with seed number 79 and contains 14 values: 79, 80, ..., 91, 92.
// The second range starts with seed number 55 and contains 13 values: 55, 56, ..., 66, 67.

// Now, rather than considering four seed numbers, you need to consider a total of 27 seed numbers.

// In the above example, the lowest location number can be obtained from seed number 82,
// which corresponds to soil 84, fertilizer 84, water 84, light 77, temperature 45, humidity 46, and location 46.
// So, the lowest location number is 46.

// Consider all of the initial seed numbers listed in the ranges on the first line of the almanac.
// What is the lowest location number that corresponds to any of the initial seed numbers?

// The input for puzzle 2 is the same as for puzzle 1.

const std::string & givenTestData_problem2 = givenTestData_problem1;
constexpr puzzleValueType expectedSolution_problem2 = 46;

template<typename T>
puzzleValueType solve2(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    // // Collect information
    // std::unordered_map<std::string, ConversionMap> conversionMaps;
    // std::vector<Object> objects;
    // std::string _lastConversionMapKey = "";
    // bool collectingSeeds = false;
    // puzzleValueType totalSeedNumberRange = 0;
    // for (const auto & line : lines) {
    //     if (StringUtils::contains(line, " map:")) {
    //         // conversion map
    //         ConversionMap map(line);
    //         conversionMaps[map.getFromObjectName()] = map;
    //         _lastConversionMapKey = map.getFromObjectName();
    //     } else if (StringUtils::contains(line, "seeds:")) {
    //         auto tempLine1 = StringUtils::remove(line, "seeds: ");
    //         auto seedNumbers = StringUtils::split(tempLine1, " ");
    //         // process in two's
    //         assert((seedNumbers.size() & 1) == 0);  //is even

    //         for (unsigned int i=0; i < seedNumbers.size(); i+=2) {
    //             auto & seedNumberStartStr = seedNumbers[i];
    //             auto & seedNumberRangeStr = seedNumbers[i+1];
    //             puzzleValueType seedNumberStart = std::stoll(seedNumberStartStr);
    //             puzzleValueType seedNumberRange = std::stoll(seedNumberRangeStr);

    //             totalSeedNumberRange += seedNumberRange;
    //             for(puzzleValueType i2=0; i2 < seedNumberRange; i2++) {
    //                 objects.push_back(Object("seed", seedNumberStart+i2));
    //             }

    //         }
    //     } else if (!line.empty()) {
    //         assert(!_lastConversionMapKey.empty());
    //         conversionMaps[_lastConversionMapKey].addConversionRange(ConversionRange(line));
    //     }
    // }
    // std::cout << "totalSeedNumberRange:" << totalSeedNumberRange << std::endl;

    // for (puzzleValueType i=0; i < totalSeedNumberRange; i++) {
    //     totalSeedNumberRange--;
    // }
    // std::cout << "totalSeedNumberRange:" << totalSeedNumberRange << std::endl;

    // assert(!conversionMaps.empty());

    // // Process objects
    // std::vector<Object> finishedObjects;    //for when no more steps can be done
    // std::string lastObjectName = "";
    // bool change = false;
    // do {
    //     change = false;
    //     std::vector<Object> newObjects;
    //     for (auto object : objects) {
    //         if (object.getObjectName() != lastObjectName) {
    //             lastObjectName = object.getObjectName();
    //             std::cout << object.getObjectName() << std::endl;
    //         }
    //         if (conversionMaps.contains(object.getObjectName())) {
    //             Object newObject = conversionMaps[object.getObjectName()].convert(object);
    //             newObjects.push_back(newObject);
    //             change = true;
    //         } else {
    //             finishedObjects.push_back(object);
    //         }
    //     }
    //     objects = newObjects;
    // } while (change);

    // assert(!finishedObjects.empty());

    // // Get lowest value
    // puzzleValueType puzzleValue = finishedObjects[0].getValue();
    // for (auto object : finishedObjects) {
    //     puzzleValue = std::min(puzzleValue, object.getValue());
    // }

    // return puzzleValue;

    return 0;
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