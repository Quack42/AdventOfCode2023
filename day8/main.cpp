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

// --- Day 8: Haunted Wasteland ---
// You're still riding a camel across Desert Island when you spot a sandstorm quickly approaching.
// When you turn to warn the Elf, she disappears before your eyes!
// To be fair, she had just finished warning you about ghosts a few minutes ago.

// One of the camel's pouches is labeled "maps" - sure enough,
//  it's full of documents (your puzzle input) about how to navigate the desert.
// At least, you're pretty sure that's what they are;
// one of the documents contains a list of left/right instructions,
// and the rest of the documents seem to describe some kind of network of labeled nodes.

// It seems like you're meant to use the left/right instructions to navigate the network.
// Perhaps if you have the camel follow the same instructions,
// you can escape the haunted wasteland!

// After examining the maps for a bit, two nodes stick out:
// AAA and ZZZ.
// You feel like AAA is where you are now,
// and you have to follow the left/right instructions until you reach ZZZ.

// This format defines each node of the network individually. For example:

// RL

// AAA = (BBB, CCC)
// BBB = (DDD, EEE)
// CCC = (ZZZ, GGG)
// DDD = (DDD, DDD)
// EEE = (EEE, EEE)
// GGG = (GGG, GGG)
// ZZZ = (ZZZ, ZZZ)

// Starting with AAA, you need to look up the next element based on the next left/right instruction in your input.
// In this example, start with AAA and go right (R) by choosing the right element of AAA, CCC.
// Then, L means to choose the left element of CCC, ZZZ.
// By following the left/right instructions, you reach ZZZ in 2 steps.

// Of course, you might not find ZZZ right away.
// If you run out of left/right instructions, repeat the whole sequence of instructions as necessary:
// RL really means RLRLRLRLRLRLRLRL... and so on.
// For example, here is a situation that takes 6 steps to reach ZZZ:

// LLR

// AAA = (BBB, BBB)
// BBB = (AAA, ZZZ)
// ZZZ = (ZZZ, ZZZ)

// Starting at AAA, follow the left/right instructions. How many steps are required to reach ZZZ?

const std::string givenTestData_problem1 = "LLR\n\
\n\
AAA = (BBB, BBB)\n\
BBB = (AAA, ZZZ)\n\
ZZZ = (ZZZ, ZZZ)\n";

using puzzleValueType = long long int;
constexpr puzzleValueType expectedSolution_problem1 = 6;


enum direction_e {
    Left,
    Right
};

class AvailableMove {
private:
    std::vector<direction_e> leftRightDirections;

public:
    AvailableMove(const std::string & line) {
        for (const auto & c : line) {
            assert (c == 'L' || c == 'R');
            leftRightDirections.push_back( (c=='L')? Left : Right);
        }
    }

    const std::vector<direction_e> & getLeftRightDirections() const {
        return leftRightDirections;
    }
};

class Node {
private:
    std::string node;
    std::string left;
    std::string right;
public:
    Node(const std::string & line) {
        std::string t1 = StringUtils::remove(line, "(");
        t1 = StringUtils::remove(t1, ")");
        t1 = StringUtils::remove(t1, "= ");
        t1 = StringUtils::remove(t1, ",");
        auto nodeStrings = StringUtils::split(t1, " ");
        node = nodeStrings[0];
        left = nodeStrings[1];
        right = nodeStrings[2];
        std::cout << "line:[" << line << "]" << std::endl;
        std::cout << "t1:[" << t1 << "]" << std::endl;
        assert(!node.empty());
        assert(!left.empty());
        assert(!right.empty());
    }

    const std::string & getNodeName() const {
        return node;
    }

    const std::string & getLeftNode() const {
        return left;
    }

    const std::string & getRightNode() const {
        return right;
    }
};

template<typename T>
puzzleValueType solve1(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    // Parse to data types
    AvailableMove move(lines[0]);
    std::unordered_map<std::string, Node*> nodes;
    std::vector<Node*> nodesVector;
    for (unsigned int i=2; i < lines.size(); i++) {
        const std::string & line = lines[i];
        if (!line.empty()) {
            Node * node = new Node(line);
            nodes[node->getNodeName()] = node;
            nodesVector.push_back(node);
        }
    }
    std::string currentNode = "AAA";
    std::queue<direction_e> moveQueue = VectorUtils::convertToQueue(move.getLeftRightDirections());
    assert(!moveQueue.empty());
    puzzleValueType moveCounter = 0;
    for(;;) {
        if (moveQueue.empty()) {
            moveQueue = VectorUtils::convertToQueue(move.getLeftRightDirections());
        }
        auto direction = moveQueue.front();
        moveQueue.pop();
        // std::cout << "direction:[" << ((direction==Left)? "Left" : "Right") << "]" << std::endl;

        if (direction == Left) {
            currentNode = nodes[currentNode]->getLeftNode();
            moveCounter++;
        } else {    //direction == Right
            currentNode = nodes[currentNode]->getRightNode();
            moveCounter++;
        }
        // std::cout << "currentNode:[" << currentNode << "]" << std::endl;
        if (currentNode == "ZZZ"){
            break;
        }
    }

    puzzleValueType puzzleValue = moveCounter;

    for (Node * node : nodesVector) {
        delete node;
    }
    nodesVector.clear();
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
