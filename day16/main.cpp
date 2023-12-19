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

// --- Day 16: The Floor Will Be Lava ---
// With the beam of light completely focused somewhere,
//  the reindeer leads you deeper still into the Lava Production Facility.
// At some point, you realize that the steel facility walls have been replaced with cave,
//  and the doorways are just cave, and the floor is cave, and you're pretty sure this is actually just a giant cave.

// Finally, as you approach what must be the heart of the mountain,
//  you see a bright light in a cavern up ahead.
// There, you discover that the beam of light you so carefully focused is emerging from the cavern wall closest
//  to the facility and pouring all of its energy into a contraption on the opposite side.

// Upon closer inspection, the contraption appears to be a flat, two-dimensional square grid containing
//  empty space (.),
//  mirrors (/ and \),
//  and splitters (| and -).

// The contraption is aligned so that most of the beam bounces around the grid,
//  but each tile on the grid converts some of the beam's light into heat to melt the rock in the cavern.

// You note the layout of the contraption (your puzzle input). For example:

// .|...\....
// |.-.\.....
// .....|-...
// ........|.
// ..........
// .........\
// ..../.\\..
// .-.-/..|..
// .|....-|.\
// ..//.|....

// The beam enters in the top-left corner from the left and heading to the right.
// Then, its behavior depends on what it encounters as it moves:

// If the beam encounters empty space (.), it continues in the same direction.
// If the beam encounters a mirror (/ or \), the beam is reflected 90 degrees depending on the angle of the mirror.
//  For instance, a rightward-moving beam that encounters a / mirror would continue upward in the mirror's column,
//   while a rightward-moving beam that encounters a \ mirror would continue downward from the mirror's column.
// If the beam encounters the pointy end of a splitter (| or -), the beam passes through the splitter as if the splitter were empty space.
//  For instance, a rightward-moving beam that encounters a - splitter would continue in the same direction.
// If the beam encounters the flat side of a splitter (| or -), the beam is split into two beams going in each of the two directions the splitter's pointy ends are pointing.
//  For instance, a rightward-moving beam that encounters a | splitter would split into two beams:
//   one that continues upward from the splitter's column
//   and one that continues downward from the splitter's column.
// Beams do not interact with other beams; a tile can have many beams passing through it at the same time.
// A tile is energized if that tile has at least one beam pass through it, reflect in it, or split in it.

// In the above example, here is how the beam of light bounces around the contraption:

// >|<<<\....
// |v-.\^....
// .v...|->>>
// .v...v^.|.
// .v...v^...
// .v...v^..\
// .v../2\\..
// <->-/vv|..
// .|<<<2-|.\
// .v//.|.v..

// Beams are only shown on empty tiles; arrows indicate the direction of the beams.
// If a tile contains beams moving in multiple directions, the number of distinct directions is shown instead.
// Here is the same diagram but instead only showing whether a tile is energized (#) or not (.):

// ######....
// .#...#....
// .#...#####
// .#...##...
// .#...##...
// .#...##...
// .#..####..
// ########..
// .#######..
// .#...#.#..

// Ultimately, in this example, 46 tiles become energized.

// The light isn't energizing enough tiles to produce lava;
//  to debug the contraption, you need to start by analyzing the current situation.
// With the beam starting in the top-left heading right, how many tiles end up being energized?

const std::string givenTestData_problem1 = "\
.|...\\....\n\
|.-.\\.....\n\
.....|-...\n\
........|.\n\
..........\n\
.........\\\n\
..../.\\\\..\n\
.-.-/..|..\n\
.|....-|.\\\n\
..//.|....\n";

constexpr puzzleValueType expectedSolution_problem1 = 46;

constexpr char EMPTY_SPACE = '.';
constexpr char VERTICAL_SPLITTER = '|';
constexpr char HORIZONTAL_SPLITTER = '-';
constexpr char MIRROR1 = '\\';
constexpr char MIRROR2 = '/';

enum direction_e {
    e_north,
    e_east,
    e_south,
    e_west
};

using ICoordinates = TCoordinates<int>;

constexpr ICoordinates NORTH{0,-1};
constexpr ICoordinates EAST{1,0};
constexpr ICoordinates SOUTH{0,1};
constexpr ICoordinates WEST{-1,0};


class Beam {
private:
    ICoordinates coordinates;
    direction_e direction;
public:
    Beam(const ICoordinates & coordinates, const direction_e & direction) : coordinates(coordinates), direction(direction) {}

    const ICoordinates & getCoordinates() const {
        return coordinates;
    }

    const direction_e & getDirection() const {
        return direction;
    }
};

class Space {
private:
    const ICoordinates coordinates;
    const char character;
    bool energized = false;

    bool north = false;
    bool east = false;
    bool south = false;
    bool west = false;
public:
    Space(const ICoordinates & coordinates, const char & character) : coordinates(coordinates), character(character) {}

    const char & getCharacter() const {
        return character;
    }

    bool isEnergized() const {
        return energized;
    }

    void beamEvent(const Beam & beam, std::queue<Beam> & beamQueue) {
        energized = true;
        switch(character) {
            case EMPTY_SPACE:
                emptySpace_beamEvent(beam, beamQueue);
                break;
            case VERTICAL_SPLITTER:
                verticalSplitter_beamEvent(beam, beamQueue);
                break;
            case HORIZONTAL_SPLITTER:
                horizontalSplitter_beamEvent(beam, beamQueue);
                break;
            case MIRROR1:
                mirror1_beamEvent(beam, beamQueue);
                break;
            case MIRROR2:
                mirror2_beamEvent(beam, beamQueue);
                break;
        }
    }

private:
    void forwardNorth(const Beam & beam, std::queue<Beam> & beamQueue) {
        if (!north) {
            beamQueue.emplace(beam.getCoordinates() + NORTH, direction_e::e_north);
        }
        north = true;
    }
    void forwardEast(const Beam & beam, std::queue<Beam> & beamQueue) {
        if (!east) {
            beamQueue.emplace(beam.getCoordinates() + EAST, direction_e::e_east);
        }
        east = true;
    }
    void forwardSouth(const Beam & beam, std::queue<Beam> & beamQueue) {
        if (!south) {
            beamQueue.emplace(beam.getCoordinates() + SOUTH, direction_e::e_south);
        }
        south = true;
    }
    void forwardWest(const Beam & beam, std::queue<Beam> & beamQueue) {
        if (!west) {
            beamQueue.emplace(beam.getCoordinates() + WEST, direction_e::e_west);
        }
        west = true;
    }

    void emptySpace_beamEvent(const Beam & beam, std::queue<Beam> & beamQueue) {
        // '.'
        switch (beam.getDirection()) {
            case direction_e::e_north:
                forwardNorth(beam, beamQueue);
                break;
            case direction_e::e_east:
                forwardEast(beam, beamQueue);
                break;
            case direction_e::e_south:
                forwardSouth(beam, beamQueue);
                break;
            case direction_e::e_west:
                forwardWest(beam, beamQueue);
                break;
            default:
                assert(false);
        }
    }

    void verticalSplitter_beamEvent(const Beam & beam, std::queue<Beam> & beamQueue) {
        // '|'
        switch (beam.getDirection()) {
            case direction_e::e_north:
                forwardNorth(beam, beamQueue);
                break;
            case direction_e::e_east:
                forwardNorth(beam, beamQueue);
                forwardSouth(beam, beamQueue);
                break;
            case direction_e::e_south:
                forwardSouth(beam, beamQueue);
                break;
            case direction_e::e_west:
                forwardNorth(beam, beamQueue);
                forwardSouth(beam, beamQueue);
                break;
            default:
                assert(false);
        }
    }

    void horizontalSplitter_beamEvent(const Beam & beam, std::queue<Beam> & beamQueue) {
        // '-'
        switch (beam.getDirection()) {
            case direction_e::e_north:
                forwardEast(beam, beamQueue);
                forwardWest(beam, beamQueue);
                break;
            case direction_e::e_east:
                forwardEast(beam, beamQueue);
                break;
            case direction_e::e_south:
                forwardEast(beam, beamQueue);
                forwardWest(beam, beamQueue);
                break;
            case direction_e::e_west:
                forwardWest(beam, beamQueue);
                break;
            default:
                assert(false);
        }
    }

    void mirror1_beamEvent(const Beam & beam, std::queue<Beam> & beamQueue) {
        // '\\'
        switch (beam.getDirection()) {
            case direction_e::e_north:
                forwardWest(beam, beamQueue);
                break;
            case direction_e::e_east:
                forwardSouth(beam, beamQueue);
                break;
            case direction_e::e_south:
                forwardEast(beam, beamQueue);
                break;
            case direction_e::e_west:
                forwardNorth(beam, beamQueue);
                break;
            default:
                assert(false);
        }
    }

    void mirror2_beamEvent(const Beam & beam, std::queue<Beam> & beamQueue) {
        // '/'
        switch (beam.getDirection()) {
            case direction_e::e_north:
                forwardEast(beam, beamQueue);
                break;
            case direction_e::e_east:
                forwardNorth(beam, beamQueue);
                break;
            case direction_e::e_south:
                forwardWest(beam, beamQueue);
                break;
            case direction_e::e_west:
                forwardSouth(beam, beamQueue);
                break;
            default:
                assert(false);
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

    //convert lines into items
    std::vector<std::vector<Space>> map;
    {
        int y=0;
        for (const auto & line : lines) {
            int x=0;
            std::vector<Space> spacesLine;
            for (const char & c : line) {
                spacesLine.push_back(Space({x,y}, c));
                x++;
            }
            map.push_back(spacesLine);
            y++;
        }
    }

    for (const auto & spaceRow : map) {
        for (const auto & space : spaceRow) {
            std::cout << space.getCharacter();
        }
        std::cout << std::endl;
    }

    // play out beam scenario
    std::queue<Beam> beams;
    beams.emplace(ICoordinates{0,0}, direction_e::e_east);
    while (!beams.empty()) {
        const Beam & beam = beams.front();
        const auto & beamCoordinates = beam.getCoordinates();
        if ((beamCoordinates.y >= 0 && beamCoordinates.y < map.size())
            && (beamCoordinates.x >= 0 && beamCoordinates.x < map[0].size()))
        {
            Space & space = map[beamCoordinates.y][beamCoordinates.x];
            space.beamEvent(beam, beams);
        }
        beams.pop();
    }
    std::cout << std::endl;

    // print energized spaces
    for (const auto & spaceRow : map) {
        for (const auto & space : spaceRow) {
            if (space.isEnergized()) {
                std::cout << "#";
            } else {
                std::cout << space.getCharacter();
            }
        }
        std::cout << std::endl;
    }

    // count energized spaces
    puzzleValueType puzzleValue = 0;
    for (const auto & spaceRow : map) {
        for (const auto & space : spaceRow) {
            if (space.isEnergized()) {
                puzzleValue++;
            }
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
