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
        std::cout << "AvailableMove:[";
        for (const auto & c : line) {
            assert (c == 'L' || c == 'R');
            leftRightDirections.push_back( (c=='L')? Left : Right);
            std::cout << c;
        }
        std::cout << "]" << std::endl;
    }

    const std::vector<direction_e> & getLeftRightDirections() const {
        return leftRightDirections;
    }
};

class Node {
private:
    std::string node;
    std::string left;
    Node * leftNode = nullptr;
    std::string right;
    Node * rightNode = nullptr;
    bool finalNode = false;
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

    const std::string & getLeftNodeString() const {
        return left;
    }

    const std::string & getRightNodeString() const {
        return right;
    }

    Node * getLeftNode() const {
        return leftNode;
    }

    Node * getRightNode() const {
        return rightNode;
    }

    void setLeftNode(Node * leftNode) {
        this->leftNode = leftNode;
    }

    void setRightNode(Node * rightNode) {
        this->rightNode = rightNode;
    }

    void setFinalNode() {
        this->finalNode = true;
    }

    bool isFinalNode() const {
        return finalNode;
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
            currentNode = nodes[currentNode]->getLeftNodeString();
            moveCounter++;
        } else {    //direction == Right
            currentNode = nodes[currentNode]->getRightNodeString();
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


// --- Part Two ---
// The sandstorm is upon you and you aren't any closer to escaping the wasteland.
// You had the camel follow the instructions, but you've barely left your starting position.
// It's going to take significantly more steps to escape!

// What if the map isn't for people - what if the map is for ghosts?
// Are ghosts even bound by the laws of spacetime?
// Only one way to find out.

// After examining the maps a bit longer,
// your attention is drawn to a curious fact: the number of nodes with names ending in A is equal to the number ending in Z!
// If you were a ghost, you'd probably just start at every node that ends with A and follow all of the paths at the same time until they all simultaneously end up at nodes that end with Z.

// For example:

// LR

// 11A = (11B, XXX)
// 11B = (XXX, 11Z)
// 11Z = (11B, XXX)
// 22A = (22B, XXX)
// 22B = (22C, 22C)
// 22C = (22Z, 22Z)
// 22Z = (22B, 22B)
// XXX = (XXX, XXX)
// Here, there are two starting nodes, 11A and 22A (because they both end with A).
// As you follow each left/right instruction, use that instruction to simultaneously navigate away from both nodes you're currently on.
// Repeat this process until all of the nodes you're currently on end with Z.
// (If only some of the nodes you're on end with Z, they act like any other node and you continue as normal.)

// In this example, you would proceed as follows:

// Step 0: You are at 11A and 22A.
// Step 1: You choose all of the left paths, leading you to 11B and 22B.
// Step 2: You choose all of the right paths, leading you to 11Z and 22C.
// Step 3: You choose all of the left paths, leading you to 11B and 22Z.
// Step 4: You choose all of the right paths, leading you to 11Z and 22B.
// Step 5: You choose all of the left paths, leading you to 11B and 22C.
// Step 6: You choose all of the right paths, leading you to 11Z and 22Z.
// So, in this example, you end up entirely on nodes that end in Z after 6 steps.

// Simultaneously start on every node that ends with A. How many steps does it take before you're only on nodes that end with Z?

// The input for puzzle 2 is the same as for puzzle 1.

const std::string givenTestData_problem2 = "\
LR\n\
\n\
11A = (11B, XXX)\n\
11B = (XXX, 11Z)\n\
11Z = (11B, XXX)\n\
22A = (22B, XXX)\n\
22B = (22C, 22C)\n\
22C = (22Z, 22Z)\n\
22Z = (22B, 22B)\n\
XXX = (XXX, XXX)\n";
constexpr puzzleValueType expectedSolution_problem2 = 6;

class Loop {
private:
    std::vector<Node*> nodes;
    size_t initialStepIndex;    //index in the left-right instructions
    puzzleValueType moveCounter;    //which move we're on when the loop started

    bool done = false;

    // -- tracking variables
    puzzleValueType currentMove = 0;

public:
    Loop() {}
    Loop(Node * firstNode, size_t initialStepIndex, puzzleValueType moveCounter)
        : initialStepIndex(initialStepIndex),
        moveCounter(moveCounter)
    {
        nodes.push_back(firstNode);
    }

    void addStep(Node * node, size_t stepIndex) {
        if (done) {
            return;
        }
        if (nodes[0] == node && stepIndex == initialStepIndex) {
            // full cycle
            done = true;
        }
        nodes.push_back(node);
    }

    bool isDone() const {
        return done;
    }

    void print() const {
        PRINT(moveCounter);
        PRINT(nodes[0]->getNodeName());
        PRINT(initialStepIndex);
        PRINT(nodes.size());
        for (size_t i=0; i < nodes.size(); i++) {
            if (nodes[i]->isFinalNode()) {
                std::cout << "final node at nodes index[" << i << "], name:[" << nodes[i]->getNodeName() << "]" << std::endl;
            }
        }

        std::cout << "---" << std::endl;
    }

    // --- tracking functions
    void calculateStartingPosition() {
        std::set<size_t> finalNodes;
        for (size_t i=0; i < nodes.size(); i++) {
            if (nodes[i]->isFinalNode()) {
                finalNodes.insert(i);
                std::cout << "final node at nodes index[" << i << "], name:[" << nodes[i]->getNodeName() << "]" << std::endl;
            }
        }
        assert(finalNodes.size() == 1);

        //start at final node
        currentMove = moveCounter + *finalNodes.begin();
    }

    void incrementCycle() {
        currentMove += nodes.size();
    }

    const puzzleValueType & getCurrentPosition() const {
        return currentMove;
    }

    bool operator<(const Loop & rhs) const {
        if (currentMove != rhs.currentMove) {
            return currentMove < rhs.currentMove;
        }
        return this < &rhs;
    }
    bool operator>(const Loop & rhs) const {
        if (currentMove != rhs.currentMove) {
            return currentMove > rhs.currentMove;
        }
        return this > &rhs;
    }
};

template<typename T>
puzzleValueType solve2(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    // Parse to data types
    std::cout << std::endl;
    AvailableMove move(lines[0]);
    std::unordered_map<std::string, Node*> nodesMap;
    std::vector<Node*> nodesVector;
    for (unsigned int i=2; i < lines.size(); i++) {
        const std::string & line = lines[i];
        if (!line.empty()) {
            Node * node = new Node(line);
            nodesMap[node->getNodeName()] = node;
            nodesVector.push_back(node);
        }
    }

    //post process nodes
    std::cout << "Final nodes:[";
    for (Node * node : nodesVector) {
        node->setLeftNode(nodesMap.at(node->getLeftNodeString()));
        node->setRightNode(nodesMap.at(node->getRightNodeString()));

        if (node->getNodeName()[2] == 'Z') {
            std::cout << node->getNodeName() << ", ";
            node->setFinalNode();
        }
    }
    std::cout << "]" << std::endl;

    // Get starting nodes
    std::vector<Node*> currentNodes;
    std::cout << "Start nodes:[";
    for (Node * node : nodesVector) {
        if (node->getNodeName()[2] == 'A') {
            std::cout << node->getNodeName() << ", ";
            currentNodes.push_back(node);
        }
    }
    std::cout << "]" << std::endl;

    std::queue<direction_e> moveQueue = VectorUtils::convertToQueue(move.getLeftRightDirections());
    std::unordered_map<size_t, std::unordered_map<size_t, std::set<Node*> > > history;
    std::unordered_map<size_t, Loop> loopHistory;
    std::set<size_t> ghostsStuckInALoop;
    unsigned int loopsDone = 0;

    puzzleValueType stepCounter = 0;
    for (unsigned int i=0; i < currentNodes.size(); i++) {
        history[i][moveQueue.size()].insert(currentNodes[i]);
    }
    assert(!moveQueue.empty());
    puzzleValueType moveCounter = 0;
    while(loopsDone < currentNodes.size()) {
        if (moveQueue.empty()) {
            moveQueue = VectorUtils::convertToQueue(move.getLeftRightDirections());
        }
        auto direction = moveQueue.front();
        moveQueue.pop();

        for (unsigned int i=0; i < currentNodes.size(); i++) {
            if (direction == Left) {
                currentNodes[i] = currentNodes[i]->getLeftNode();
            } else {    //direction == Right
                currentNodes[i] = currentNodes[i]->getRightNode();
            }
            if (ghostsStuckInALoop.contains(i)) {
                if (!loopHistory[i].isDone()) {
                    loopHistory[i].addStep(currentNodes[i], move.getLeftRightDirections().size() - moveQueue.size());
                    if (loopHistory[i].isDone()) {
                        loopsDone++;
                    }
                }
            }
            auto [_, inserted] = history[i][moveQueue.size()].insert(currentNodes[i]);
            if (!inserted) {
                if (!ghostsStuckInALoop.contains(i)) {
                    std::cout << "ghost stuck in a loop:[" << i << "] at [" << currentNodes[i]->getNodeName() << "]" << std::endl;
                    ghostsStuckInALoop.insert(i);
                    loopHistory.emplace(i, Loop(currentNodes[i], move.getLeftRightDirections().size() - moveQueue.size(), moveCounter));
                }
            }
        }
        moveCounter++;

        bool success = true;
        // std::cout << "Final nodes check:[";
        for (Node * currentNode : currentNodes) {
            // std::cout << currentNode->getNodeName() << ", ";
            if (!currentNode->isFinalNode()){
                success = false;
            }
        }
        // std::cout << "] [" << success << "]" << std::endl;

        if (success) {
            break;
        }
    }


    PRINT(loopHistory.size());

    std::cout << "--" << std::endl;
    for (const auto & [ghostIndex, loop] : loopHistory) {
        loop.print();
    }
    std::cout << "0" << std::endl;

    // Calculate the initial starting position per loop and put loops in sorting set.
    std::set<Loop> sortedLoops;
    for (auto & [ghostIndex, loop] : loopHistory) {
        loop.calculateStartingPosition();
        sortedLoops.insert(loop);
    }
    std::cout << "1" << std::endl;

    assert(sortedLoops.size() == currentNodes.size());

    // put sorted loops in array
    Loop runningLoops[currentNodes.size()];
    size_t counter = 0;
    for (const Loop & loop : sortedLoops) {
        runningLoops[counter] = loop;
        counter++;
    }
    std::cout << "2" << std::endl;

    // Play out scenario
    constexpr puzzleValueType printBarrierStepSize = 1000000000;
    puzzleValueType printBarrier = printBarrierStepSize;
    while (runningLoops[0].getCurrentPosition() != runningLoops[sizeof(runningLoops)/sizeof(runningLoops[0])-1].getCurrentPosition()) {
        // increment by cycle
        // std::cout << "A" << std::endl;
        runningLoops[0].incrementCycle();

        // std::cout << "B" << std::endl;
        //re-sort
        for (unsigned int i=1; i < sizeof(runningLoops)/sizeof(runningLoops[0]); i++) {
            // PRINT(i);
            if (runningLoops[i-1] > runningLoops[i]) {
                std::swap(runningLoops[i-1], runningLoops[i]);
            }
        }
        // std::cout << "C" << std::endl;

        if (runningLoops[0].getCurrentPosition() > printBarrier) {
            printBarrier += printBarrierStepSize;
            for (unsigned int i=0; i < sizeof(runningLoops)/sizeof(runningLoops[0]); i++) {
                PRINT(runningLoops[i].getCurrentPosition());
            }
            std::cout << std::endl;
        }
    }


    for (unsigned int i=0; i < sizeof(runningLoops)/sizeof(runningLoops[0]); i++) {
        PRINT(runningLoops[i].getCurrentPosition());
    }

    puzzleValueType puzzleValue = runningLoops[0].getCurrentPosition();

    for (Node * node : nodesVector) {
        delete node;
    }
    nodesVector.clear();
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
