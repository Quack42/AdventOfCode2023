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

    char getCharacter() const {
        return c;
    }

    bool hasUp() const {
        return hasUpConnection(c);
    }

    bool hasEast() const {
        return hasEastConnection(c);
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

    const CoordinateUtils::ICoordinates & getUnmatchingCoordinates(const CoordinateUtils::ICoordinates & unmatch) const {
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

// --- Part Two ---
// You quickly reach the farthest point of the loop, but the animal never emerges.
// Maybe its nest is within the area enclosed by the loop?

// To determine whether it's even worth taking the time to search for such a nest,
//  you should calculate how many tiles are contained within the loop. For example:

// ...........
// .S-------7.
// .|F-----7|.
// .||.....||.
// .||.....||.
// .|L-7.F-J|.
// .|..|.|..|.
// .L--J.L--J.
// ...........

// The above loop encloses merely four tiles - the two pairs of . in the southwest and southeast (marked I below).
// The middle . tiles (marked O below) are not in the loop. Here is the same loop again with those regions marked:

// ...........
// .S-------7.
// .|F-----7|.
// .||OOOOO||.
// .||OOOOO||.
// .|L-7OF-J|.
// .|II|O|II|.
// .L--JOL--J.
// .....O.....
// In fact, there doesn't even need to be a full tile path to the outside for tiles to count as outside the loop
// - squeezing between pipes is also allowed!
// Here, I is still within the loop and O is still outside the loop:

// ..........
// .S------7.
// .|F----7|.
// .||OOOO||.
// .||OOOO||.
// .|L-7F-J|.
// .|II||II|.
// .L--JL--J.
// ..........

// In both of the above examples, 4 tiles are enclosed by the loop.

// Here's a larger example:
// .F----7F7F7F7F-7....
// .|F--7||||||||FJ....
// .||.FJ||||||||L7....
// FJL7L7LJLJ||LJ.L-7..
// L--J.L7...LJS7F-7L7.
// ....F-J..F7FJ|L7L7L7
// ....L7.F7||L7|.L7L7|
// .....|FJLJ|FJ|F7|.LJ
// ....FJL-7.||.||||...
// ....L---J.LJ.LJLJ...

// The above sketch has many random bits of ground, some of which are in the loop (I) and some of which are outside it (O):

// OF----7F7F7F7F-7OOOO
// O|F--7||||||||FJOOOO
// O||OFJ||||||||L7OOOO
// FJL7L7LJLJ||LJIL-7OO
// L--JOL7IIILJS7F-7L7O
// OOOOF-JIIF7FJ|L7L7L7
// OOOOL7IF7||L7|IL7L7|
// OOOOO|FJLJ|FJ|F7|OLJ
// OOOOFJL-7O||O||||OOO
// OOOOL---JOLJOLJLJOOO
// In this larger example, 8 tiles are enclosed by the loop.

// Any tile that isn't part of the main loop can count as being enclosed by the loop.
// Here's another example with many bits of junk pipe lying around that aren't connected to the main loop at all:

// FF7FSF7F7F7F7F7F---7
// L|LJ||||||||||||F--J
// FL-7LJLJ||||||LJL-77
// F--JF--7||LJLJ7F7FJ-
// L---JF-JLJ.||-FJLJJ7
// |F|F-JF---7F7-L7L|7|
// |FFJF7L7F-JF7|JL---7
// 7-L-JL7||F7|L7F-7F7|
// L.L7LFJ|||||FJL7||LJ
// L7JLJL-JLJLJL--JLJ.L
// Here are just the tiles that are enclosed by the loop marked with I:

// FF7FSF7F7F7F7F7F---7
// L|LJ||||||||||||F--J
// FL-7LJLJ||||||LJL-77
// F--JF--7||LJLJIF7FJ-
// L---JF-JLJIIIIFJLJJ7
// |F|F-JF---7IIIL7L|7|
// |FFJF7L7F-JF7IIL---7
// 7-L-JL7||F7|L7F-7F7|
// L.L7LFJ|||||FJL7||LJ
// L7JLJL-JLJLJL--JLJ.L
// In this last example, 10 tiles are enclosed by the loop.

// Figure out whether you have time to search for the nest by calculating the area within the loop.
// How many tiles are enclosed by the loop?

// The input for puzzle 2 is the same as for puzzle 1.


// const std::string givenTestData_problem2 = "\
// .F----7F7F7F7F-7....\n\
// .|F--7||||||||FJ....\n\
// .||.FJ||||||||L7....\n\
// FJL7L7LJLJ||LJ.L-7..\n\
// L--J.L7...LJS7F-7L7.\n\
// ....F-J..F7FJ|L7L7L7\n\
// ....L7.F7||L7|.L7L7|\n\
// .....|FJLJ|FJ|F7|.LJ\n\
// ....FJL-7.||.||||...\n\
// ....L---J.LJ.LJLJ...\n";

// const std::string givenTestData_problem2 = "\
// ....\n\
// .F7.\n\
// .SJ.\n\
// ....\n";

const std::string givenTestData_problem2 = "\
FF7FSF7F7F7F7F7F---7\n\
L|LJ||||||||||||F--J\n\
FL-7LJLJ||||||LJL-77\n\
F--JF--7||LJLJ7F7FJ-\n\
L---JF-JLJ.||-FJLJJ7\n\
|F|F-JF---7F7-L7L|7|\n\
|FFJF7L7F-JF7|JL---7\n\
7-L-JL7||F7|L7F-7F7|\n\
L.L7LFJ|||||FJL7||LJ\n\
L7JLJL-JLJLJL--JLJ.L\n";

constexpr puzzleValueType expectedSolution_problem2 = 10;


void addIfAbleFunc(std::queue<CoordinateUtils::ICoordinates> & coordinatesToProcess,
                   const std::vector<std::vector<char> > & map2,
                   const CoordinateUtils::ICoordinates & coordsToTest)
{
    // std::cout << "coordsToTest:[" << coordsToTest.x << ", " << coordsToTest.y << "]" << std::endl;
    if (coordsToTest.x >= 0 && coordsToTest.x < map2[0].size()) {
        if (coordsToTest.y >= 0 && coordsToTest.y < map2.size()) {
            coordinatesToProcess.push(coordsToTest);
        }
    }
}

template<typename T>
puzzleValueType solve2(T & stream) {
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



    //fill with blanks and grounds
    const char ground = '.';
    const char filler = ' ';
    std::vector<std::vector<char> > map2;
    std::vector<char> bottomLine;
    bottomLine.push_back(filler);    //+1 for x
    for (size_t x=0; x < pipes[0].size(); x++) {
        bottomLine.push_back(filler);
        bottomLine.push_back(filler);//*2 for x
    }
    map2.push_back(bottomLine); // +1 for y

    for (size_t y=0; y < pipes.size(); y++) {
        std::vector<char> line1;
        line1.push_back(filler); //+1 for x
        for (size_t x=0; x < pipes[0].size(); x++) {
            line1.push_back(ground);
            line1.push_back(filler); //*2 for x
        }
        map2.push_back(line1);

        std::vector<char> line2;    //*2 for y
        line2.push_back(filler); //+1 for x
        for (size_t x=0; x < pipes[0].size(); x++) {
            line2.push_back(filler);
            line2.push_back(filler); //*2 for x
        }
        map2.push_back(line2);
        assert(line1.size() == line2.size());
    }

    // Fill in connected pipes; and count them
    // x*2+1
    // y*2+1
    // ....
    // .F7.
    // .SJ.
    // ....
    puzzleValueType pipeCount = 0;
    const Pipe & startingPipe = pipes[startingPoint.y][startingPoint.x];
    map2[startingPoint.y*2+1][startingPoint.x*2+1] = startingPipe.getCharacter();   //mark as impassible pipe
    pipeCount++;
    if (startingPipe.hasUp()) {
        map2[startingPoint.y*2+1-1][startingPoint.x*2+1] = 'y';   //mark as impassible pipe
    }
    if (startingPipe.hasEast()) {
        map2[startingPoint.y*2+1][startingPoint.x*2+1+1] = 'x';   //mark as impassible pipe
    }
    CoordinateUtils::ICoordinates pathACoordinates = startingPoint + startingPipe.getFirstCoordinates();
    {
        const Pipe & pipeA = pipes[pathACoordinates.y][pathACoordinates.x];
        if (pipeA.hasUp()) {
            map2[pathACoordinates.y*2+1-1][pathACoordinates.x*2+1] = 'y';   //mark as impassible pipe
        }
        if (pipeA.hasEast()) {
            map2[pathACoordinates.y*2+1][pathACoordinates.x*2+1+1] = 'x';   //mark as impassible pipe
        }
        map2[pathACoordinates.y*2+1][pathACoordinates.x*2+1] = pipeA.getCharacter();   //mark as impassible pipe
        pipeCount++;
    }
    CoordinateUtils::ICoordinates pathBCoordinates = startingPoint + startingPipe.getSecondCoordinates();
    {
        const Pipe & pipeB = pipes[pathBCoordinates.y][pathBCoordinates.x];
        if (pipeB.hasUp()) {
            map2[pathBCoordinates.y*2+1-1][pathBCoordinates.x*2+1] = 'y';   //mark as impassible pipe
        }
        if (pipeB.hasEast()) {
            map2[pathBCoordinates.y*2+1][pathBCoordinates.x*2+1+1] = 'x';   //mark as impassible pipe
        }
        map2[pathBCoordinates.y*2+1][pathBCoordinates.x*2+1] = pipeB.getCharacter();   //mark as impassible pipe
        // pipeCount++;     //this one is redundant; will be counted in the main loop; so this line is commented, otherwise it will be counted twice
    }
    CoordinateUtils::ICoordinates previousOffsetA = pipes[startingPoint.y][startingPoint.x].getFirstCoordinates();
    CoordinateUtils::ICoordinates previousOffsetB = pipes[startingPoint.y][startingPoint.x].getSecondCoordinates();
    while (pathACoordinates != pathBCoordinates) {
        const Pipe & pipeA = pipes[pathACoordinates.y][pathACoordinates.x];
        auto offsetA = pipeA.getUnmatchingCoordinates(CoordinateUtils::invert(previousOffsetA));
        previousOffsetA = offsetA;
        pathACoordinates = pathACoordinates + offsetA;
        {
            const Pipe & newPipeA = pipes[pathACoordinates.y][pathACoordinates.x];
            map2[pathACoordinates.y*2+1][pathACoordinates.x*2+1] = newPipeA.getCharacter();   //mark as impassible pipe
            pipeCount++;
            if (newPipeA.hasUp()) {
                map2[pathACoordinates.y*2+1-1][pathACoordinates.x*2+1] = 'y';   //mark as impassible pipe
            }
            if (newPipeA.hasEast()) {
                map2[pathACoordinates.y*2+1][pathACoordinates.x*2+1+1] = 'x';   //mark as impassible pipe
            }
        }
    }

    // for (auto & row : map2) {
    //     for (auto & c : row) {
    //         std::cout << c;
    //     }
    //     std::cout << std::endl;
    // }

    // crawl through map, count all grounds OUTSIDE loop
    std::queue<CoordinateUtils::ICoordinates> coordinatesToProcess;
    coordinatesToProcess.push(CoordinateUtils::ICoordinates{0,0});
    puzzleValueType groundsOutsideLoop = 0;
    while (!coordinatesToProcess.empty()) {
        auto coords = coordinatesToProcess.front();
        coordinatesToProcess.pop();
        char c = map2[coords.y][coords.x];
        // std::cout << "processing:[" << coords.x << ", " << coords.y << "]:[" << c << "]" << std::endl;
        map2[coords.y][coords.x] = ';';     //processed; don't do again
        if (c == ground) {
            groundsOutsideLoop++;
            // std::cout << "groundsOutsideLoop:[" << groundsOutsideLoop << "]" << std::endl;
        }
        if ((c == filler) || (c == ground)) {
            auto upwards = coords + up;
            addIfAbleFunc(coordinatesToProcess, map2, upwards);
            auto downwards = coords + down;
            addIfAbleFunc(coordinatesToProcess, map2, downwards);
            auto westwards = coords + west;
            addIfAbleFunc(coordinatesToProcess, map2, westwards);
            auto eastwards = coords + east;
            addIfAbleFunc(coordinatesToProcess, map2, eastwards);
        }
    }
    // std::cout << std::endl;
    // std::cout << std::endl;

    // for (auto & row : map2) {
    //     for (auto & c : row) {
    //         std::cout << c;
    //     }
    //     std::cout << std::endl;
    // }

    puzzleValueType totalSpaces = map.size() * map[0].size();
    puzzleValueType puzzleValue = totalSpaces - groundsOutsideLoop - pipeCount;

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
