#include "CoutUtils.hpp"
#include "NumberUtils.hpp"
#include "StringUtils.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// --- Day 2: Cube Conundrum ---
// You're launched high into the atmosphere!
// The apex of your trajectory just barely reaches the surface of a large island floating in the sky.
// You gently land in a fluffy pile of leaves.
// It's quite cold, but you don't see much snow.
// An Elf runs over to greet you.

// The Elf explains that you've arrived at Snow Island and apologizes for the lack of snow.
// He'll be happy to explain the situation, but it's a bit of a walk, so you have some time.
// They don't get many visitors up here; would you like to play a game in the meantime?

// As you walk, the Elf shows you a small bag and some cubes which are either red, green, or blue.
// Each time you play this game, he will hide a secret number of cubes of each color in the bag,
// and your goal is to figure out information about the number of cubes.

// To get information, once a bag has been loaded with cubes, the Elf will reach into the bag,
 // grab a handful of random cubes, show them to you, and then put them back in the bag.
// He'll do this a few times per game.

// You play several games and record the information from each game (your puzzle input).
// Each game is listed with its ID number (like the 11 in Game 11: ...) followed by a
// semicolon-separated list of subsets of cubes that were revealed from the bag (like 3 red, 5 green, 4 blue).

// For example, the record of a few games might look like this:

// Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
// Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
// Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
// Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
// Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green

// In game 1, three sets of cubes are revealed from the bag (and then put back again).
// The first set is 3 blue cubes and 4 red cubes; the second set is 1 red cube, 2 green cubes, and 6 blue cubes; the third set is only 2 green cubes.

// The Elf would first like to know which games would have been possible if the bag contained only 12 red cubes, 13 green cubes, and 14 blue cubes?

// In the example above, games 1, 2, and 5 would have been possible if the bag had been loaded with that configuration.
// However, game 3 would have been impossible because at one point the Elf showed you 20 red cubes at once;
// similarly, game 4 would also have been impossible because the Elf showed you 15 blue cubes at once.
// If you add up the IDs of the games that would have been possible, you get 8.

// Determine which games would have been possible if the bag had been loaded with only 12 red cubes, 13 green cubes, and 14 blue cubes. What is the sum of the IDs of those games?

constexpr int maximumRedCubes = 12;
constexpr int maximumGreenCubes = 13;
constexpr int maximumBlueCubes = 14;

const std::string givenTestData_problem1 = "\
Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green\n\
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue\n\
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red\n\
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red\n\
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green\n";

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class Game {
private:
    const int gameNumber;
    std::vector<int> redNumbers;
    std::vector<int> greenNumbers;
    std::vector<int> blueNumbers;
public:
    Game(const std::string & line) :
        gameNumber(processGameNumber(line)),
        redNumbers(processRedNumbers(line)),
        greenNumbers(processGreenNumbers(line)),
        blueNumbers(processBlueNumbers(line))
    {
        std::cout << "Game[" << gameNumber << "]" << std::endl;
        std::cout << CoutUtils::convertToString("red  ", redNumbers) << std::endl;
        std::cout << CoutUtils::convertToString("green", greenNumbers) << std::endl;
        std::cout << CoutUtils::convertToString("blue ", blueNumbers) << std::endl;
        assert(redNumbers.size() == greenNumbers.size());
        assert(greenNumbers.size() == blueNumbers.size());
    }

    const int & getGameNumber() const {
        return gameNumber;
    }

    const std::vector<int> & getRedNumbers() const {
        return redNumbers;
    }
    const std::vector<int> & getGreenNumbers() const {
        return greenNumbers;
    }
    const std::vector<int> & getBlueNumbers() const {
        return blueNumbers;
    }
private:
    int processGameNumber(const std::string & line) {
        auto strings = StringUtils::split(line, ": ");
        const std::string gamePart = strings[0];
        const std::string numberPart = StringUtils::remove(gamePart, "Game ");
        const int number = std::stoi(numberPart);
        return number;
    }

    std::vector<int> processColorNumbers(const std::string & line, const std::string & color) {
        std::vector<int> ret;
        auto subsections = StringUtils::split(line, ": ");
        const std::string & gameData = subsections[1];
        auto cubeDraws = StringUtils::split(gameData, "; ");

        for (auto cubeDraw : cubeDraws) {
            auto colorDraws = StringUtils::split(cubeDraw, ",");
            bool found = false;
            for (auto colorDraw : colorDraws) {
                auto colorIndex = colorDraw.find(color);
                if (colorIndex != std::string::npos) {
                    // found the right color
                    found = true;
                    const std::string colorDrawNumberStr = StringUtils::remove(colorDraw, color);
                    const int colorDrawNumber = std::stoi(colorDrawNumberStr);
                    ret.push_back(colorDrawNumber);
                    break;
                }
            }
            if (!found) {
                ret.push_back(0);
            }
        }
        return ret;
    }

    std::vector<int> processRedNumbers(const std::string & line) {
        return processColorNumbers(line, "red");
    }
    std::vector<int> processGreenNumbers(const std::string & line) {
        return processColorNumbers(line, "green");
    }
    std::vector<int> processBlueNumbers(const std::string & line) {
        return processColorNumbers(line, "blue");
    }
};

template<typename T>
void solve1(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    // Convert to game-data
    std::vector<Game> games;
    for (const auto & line : lines) {
        games.emplace_back(line);
    }

    // Collect possible games
    std::vector<Game> possibleGames;
    for (const auto & game : games) {
        int highestRedNumber = 0;
        int highestGreenNumber = 0;
        int highestBlueNumber = 0;
        NumberUtils::getHighestElement(game.getRedNumbers(), highestRedNumber);
        NumberUtils::getHighestElement(game.getGreenNumbers(), highestGreenNumber);
        NumberUtils::getHighestElement(game.getBlueNumbers(), highestBlueNumber);

        if (highestRedNumber <= maximumRedCubes 
            && highestGreenNumber <= maximumGreenCubes 
            && highestBlueNumber <= maximumBlueCubes)
        {
            possibleGames.push_back(game);
        }
    }

    // Compute puzzle value
    int puzzleValue = 0;
    for (const auto & possibleGame : possibleGames) {
        puzzleValue += possibleGame.getGameNumber();
    }

    std::cout << "solve1:[" << puzzleValue << "]" << std::endl;
}


// --- Part Two ---
// The Elf says they've stopped producing snow because they aren't getting any water!
// He isn't sure why the water stopped; however, he can show you how to get to the water source to check it out for yourself.
// It's just up ahead!

// As you continue your walk, the Elf poses a second question:
// in each game you played, what is the fewest number of cubes of each color that could have been in the bag to make the game possible?

// Again consider the example games from earlier:

// Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
// Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
// Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
// Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
// Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green

// In game 1, the game could have been played with as few as 4 red, 2 green, and 6 blue cubes. If any color had even one fewer cube, the game would have been impossible.

// Game 2 could have been played with a minimum of 1 red, 3 green, and 4 blue cubes.
// Game 3 must have been played with at least 20 red, 13 green, and 6 blue cubes.
// Game 4 required at least 14 red, 3 green, and 15 blue cubes.
// Game 5 needed no fewer than 6 red, 3 green, and 2 blue cubes in the bag.

// The power of a set of cubes is equal to the numbers of red, green, and blue cubes multiplied together.
// The power of the minimum set of cubes in game 1 is 48.
// In games 2-5 it was 12, 1560, 630, and 36, respectively. Adding up these five powers produces the sum 2286.

// For each game, find the minimum set of cubes that must have been present. What is the sum of the power of these sets?

// The input for puzzle 2 is the same as for puzzle 1.


const std::string & givenTestData_problem2 = givenTestData_problem1;

template<typename T>
void solve2(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    // Convert to game-data
    std::vector<Game> games;
    for (const auto & line : lines) {
        games.emplace_back(line);
    }

    // Collect possible games
    // Compute puzzle value
    int puzzleValue = 0;
    for (const auto & game : games) {
        int highestRedNumber = 0;
        int highestGreenNumber = 0;
        int highestBlueNumber = 0;
        NumberUtils::getHighestElement(game.getRedNumbers(), highestRedNumber);
        NumberUtils::getHighestElement(game.getGreenNumbers(), highestGreenNumber);
        NumberUtils::getHighestElement(game.getBlueNumbers(), highestBlueNumber);

        puzzleValue += highestRedNumber * highestGreenNumber * highestBlueNumber;
    }

    std::cout << "solve2:[" << puzzleValue << "]" << std::endl;
}


int main(int argc, char ** argv) {
    std::stringstream inputStream;
    if (argc == 1) {
        // std::stringstream strStream_problem1(givenTestData_problem1);
        // solve1(strStream_problem1);
        std::stringstream strStream_problem2(givenTestData_problem2);
        solve2(strStream_problem2);
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