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

// --- Day 10: Pipe Maze ---
// You use the hang glider to ride the hot air from Desert Island all the way up to the floating metal island.
// This island is surprisingly cold and there definitely aren't any thermals to glide on,
//  so you leave your hang glider behind.

// You wander around for a while, but you don't find any people or animals.
// However, you do occasionally find signposts labeled "Hot Springs" pointing in a seemingly consistent direction;
//  maybe you can find someone at the hot springs and ask them where the desert-machine parts are made.

// The landscape here is alien; even the flowers and trees are made of metal.
// As you stop to admire some metal grass,
//  you notice something metallic scurry away in your peripheral vision and jump into a big pipe!
// It didn't look like any animal you've ever seen;
//  if you want a better look, you'll need to get ahead of it.

// Scanning the area, you discover that the entire field you're standing on is densely packed with pipes;
//  it was hard to tell at first because they're the same metallic silver color as the "ground".
// You make a quick sketch of all of the surface pipes you can see (your puzzle input).

// The pipes are arranged in a two-dimensional grid of tiles:

// | is a vertical pipe connecting north and south.
// - is a horizontal pipe connecting east and west.
// L is a 90-degree bend connecting north and east.
// J is a 90-degree bend connecting north and west.
// 7 is a 90-degree bend connecting south and west.
// F is a 90-degree bend connecting south and east.
// . is ground; there is no pipe in this tile.
// S is the starting position of the animal;
//  there is a pipe on this tile,
//  but your sketch doesn't show what shape the pipe has.
// Based on the acoustics of the animal's scurrying,
//  you're confident the pipe that contains the animal is one large, continuous loop.

// For example, here is a square loop of pipe:
// .....
// .F-7.
// .|.|.
// .L-J.
// .....
// If the animal had entered this loop in the northwest corner, the sketch would instead look like this:

// .....
// .S-7.
// .|.|.
// .L-J.
// .....
// In the above diagram, the S tile is still a 90-degree F bend: you can tell because of how the adjacent pipes connect to it.

// Unfortunately, there are also many pipes that aren't connected to the loop! This sketch shows the same loop as above:

// -L|F7
// 7S-7|
// L|7||
// -L-J|
// L|-JF
// In the above diagram, you can still figure out which pipes form the main loop:
//  they're the ones connected to S,
//  pipes those pipes connect to,
//  pipes those pipes connect to,
//  and so on.
// Every pipe in the main loop connects to its two neighbors
//  (including S, which will have exactly two pipes connecting to it, and which is assumed to connect back to those two pipes).

// Here is a sketch that contains a slightly more complex main loop:
// ..F7.
// .FJ|.
// SJ.L7
// |F--J
// LJ...

// Here's the same example sketch with the extra, non-main-loop pipe tiles also shown:
// 7-F7-
// .FJ|7
// SJLL7
// |F--J
// LJ.LJ

// If you want to get out ahead of the animal, you should find the tile in the loop that is farthest from the starting position.
// Because the animal is in the pipe, it doesn't make sense to measure this by direct distance.
// Instead, you need to find the tile that would take the longest number of steps along the loop to reach from the starting point
// - regardless of which way around the loop the animal went.

// In the first example with the square loop:
// .....
// .S-7.
// .|.|.
// .L-J.
// .....

// You can count the distance each tile in the loop is from the starting point like this:
// .....
// .012.
// .1.3.
// .234.
// .....
// In this example, the farthest point from the start is 4 steps away.

// Here's the more complex loop again:
// ..F7.
// .FJ|.
// SJ.L7
// |F--J
// LJ...

// Here are the distances for each tile on that loop:
// ..45.
// .236.
// 01.78
// 14567
// 23...

// Find the single giant loop starting at S. How many steps along the loop does it take to get from the starting position to the point farthest from the starting position?
const std::string givenTestData_problem1 = "\
..F7.\n\
.FJ|.\n\
SJ.L7\n\
|F--J\n\
LJ...\n";
constexpr puzzleValueType expectedSolution_problem1 = 8;

const CoordinateUtils::ICoordinates up{0,-1};
const CoordinateUtils::ICoordinates down{0,1};
const CoordinateUtils::ICoordinates west{-1,0};
const CoordinateUtils::ICoordinates east{1,0};

class Pipe {
private:
    char c;
public:
    Pipe(const char & c) : c(c)
    {

    }

    const CoordinateUtils::ICoordinates & getFirstCoordinates() const {
        if (hasDownConnection(c)) {
            return down;
        } else if(hasEastConnection(c)) {
            return east;
        } else if(hasUpConnection(c)) {
            return up;
        }
        std::cout << "c:[" << c << "]" << std::endl;
        assert(false);
    }

    const CoordinateUtils::ICoordinates & getSecondCoordinates() const {
        if (hasWestConnection(c)) {
            return west;
        } else if(hasUpConnection(c)) {
            return up;
        } else if(hasEastConnection(c)) {
            return east;
        }
        assert(false);
    }

    const CoordinateUtils::ICoordinates & getUnmatchingCoordinates(const CoordinateUtils::ICoordinates & unmatch) {
        // gets the coordinates that don't match 'unmatch'
        if (hasWestConnection(c) && west != unmatch) {
            return west;
        } else if(hasUpConnection(c) && up != unmatch) {
            return up;
        } else if(hasEastConnection(c) && east != unmatch) {
            return east;
        } else if(hasDownConnection(c) && down != unmatch) {
            return down;
        }
        assert(false);
        return down;
    }

    static bool hasDownConnection(const char & c) {
        return (c == '|' || c == 'F' || c == '7');
    }

    static bool hasUpConnection( const char & c) {
        return (c == '|' || c == 'J' || c == 'L');
    }

    static bool hasWestConnection( const char & c) {
        return (c == '-' || c == 'J' || c == '7');
    }

    static bool hasEastConnection( const char & c) {
        return (c == '-' || c == 'F' || c == 'L');
    }

};


template<typename T>
puzzleValueType solve1(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    std::vector<std::vector<char> > map;
    for (auto & line : lines) {
        std::vector<char> mapRow;
        for (auto & c : line) {
            mapRow.push_back(c);
        }
        map.push_back(mapRow);
    }

    std::vector<std::vector<Pipe> > pipes;
    CoordinateUtils::ICoordinates startingPoint;
    int y=0;
    int x=0;
    for (auto & line : lines) {
        std::vector<Pipe> pipesInThisLine;
        for (auto & c : line) {
            if (c == 'S') {
                startingPoint = CoordinateUtils::ICoordinates{x,y};
                bool up   = (y>0) &&                    Pipe::hasDownConnection(map[y-1][x]);
                bool down = (y < map.size()-1) &&       Pipe::hasUpConnection(map[y+1][x]);
                bool west = (x>0) &&                    Pipe::hasEastConnection(map[y][x-1]);
                bool east = (x < map[0].size()-1) &&    Pipe::hasWestConnection(map[y][x+1]);
                std::cout << "up: [" << up << "]" << std::endl;
                std::cout << "down: [" << down << "]" << std::endl;
                std::cout << "west: [" << west << "]" << std::endl;
                std::cout << "east: [" << east << "]" << std::endl;
                int count = 0;
                if (up) count++;
                if (down) count++;
                if (west) count++;
                if (east) count++;
                assert(count == 2);

                if (up) {
                    if (down) {
                        std::cout << "S, as:[|]" << std::endl;
                        pipesInThisLine.push_back(Pipe('|'));
                    } else if(west) {
                        std::cout << "S, as:[J]" << std::endl;
                        pipesInThisLine.push_back(Pipe('J'));
                    } else {// if(east) {
                        std::cout << "S, as:[L]" << std::endl;
                        pipesInThisLine.push_back(Pipe('L'));
                    }
                } else if (down) {
                    if(west) {
                        std::cout << "S, as:[7]" << std::endl;
                        pipesInThisLine.push_back(Pipe('7'));
                    } else {// if(east) {
                        std::cout << "S, as:[F]" << std::endl;
                        pipesInThisLine.push_back(Pipe('F'));
                    }
                } else {
                        std::cout << "S, as:[-]" << std::endl;
                    pipesInThisLine.push_back(Pipe('-'));
                }
            } else {
                pipesInThisLine.push_back(Pipe(c));
            }
            x++;
        }
        pipes.push_back(pipesInThisLine);
        x=0;
        y++;
    }

    // count pipes
    puzzleValueType puzzleValue = 0;
    std::cout << "startingPoint[" << startingPoint.x << ", " << startingPoint.y << "]" << std::endl;
    std::cout << "startingPoint[" << startingPoint.x << ", " << startingPoint.y << "]" << std::endl;
    CoordinateUtils::ICoordinates pathACoordinates = startingPoint + pipes[startingPoint.y][startingPoint.x].getFirstCoordinates();
    CoordinateUtils::ICoordinates pathBCoordinates = startingPoint + pipes[startingPoint.y][startingPoint.x].getSecondCoordinates();
    puzzleValue++;
    CoordinateUtils::ICoordinates previousOffsetA = pipes[startingPoint.y][startingPoint.x].getFirstCoordinates();
    CoordinateUtils::ICoordinates previousOffsetB = pipes[startingPoint.y][startingPoint.x].getSecondCoordinates();
    std::cout << "pathACoordinates[" << pathACoordinates.x << ", " << pathACoordinates.y << "]" << std::endl;
    std::cout << "pathBCoordinates[" << pathBCoordinates.x << ", " << pathBCoordinates.y << "]" << std::endl;
    while (pathACoordinates != pathBCoordinates) {
        auto offsetA = pipes[pathACoordinates.y][pathACoordinates.x].getUnmatchingCoordinates(CoordinateUtils::invert(previousOffsetA));
        auto offsetB = pipes[pathBCoordinates.y][pathBCoordinates.x].getUnmatchingCoordinates(CoordinateUtils::invert(previousOffsetB));
        previousOffsetA = offsetA;
        previousOffsetB = offsetB;
        pathACoordinates = pathACoordinates + offsetA;
        pathBCoordinates = pathBCoordinates + offsetB;
        puzzleValue++;
    }

    return puzzleValue;
}

const std::string & givenTestData_problem2 = givenTestData_problem1;
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
