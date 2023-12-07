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

// --- Day 7: Camel Cards ---
// Your all-expenses-paid trip turns out to be a one-way, five-minute ride in an airship.
// (At least it's a cool airship!)
// It drops you off at the edge of a vast desert and descends back to Island Island.

// "Did you bring the parts?"

// You turn around to see an Elf completely covered in white clothing, wearing goggles, and riding a large camel.

// "Did you bring the parts?" she asks again, louder this time.
// You aren't sure what parts she's looking for;
// you're here to figure out why the sand stopped.

// "The parts! For the sand, yes! Come with me; I will show you."
// She beckons you onto the camel.

// After riding a bit across the sands of Desert Island,
// you can see what look like very large rocks covering half of the horizon.
// The Elf explains that the rocks are all along the part of Desert Island that is directly above Island Island,
// making it hard to even get there.
// Normally, they use big machines to move the rocks and filter the sand,
// but the machines have broken down because Desert Island recently stopped receiving the parts they need to fix the machines.

// You've already assumed it'll be your job to figure out why the parts stopped when she asks if you can help.
// You agree automatically.

// Because the journey will take a few days, she offers to teach you the game of Camel Cards.
// Camel Cards is sort of similar to poker except it's designed to be easier to play while riding a camel.

// In Camel Cards,
//  you get a list of hands,
//  and your goal is to order them based on the strength of each hand.
// A hand consists of five cards labeled one of
// A, K, Q, J, T, 9, 8, 7, 6, 5, 4, 3, or 2.
// The relative strength of each card follows this order,
//  where A is the highest and 2 is the lowest.

// Every hand is exactly one type.
// From strongest to weakest, they are:

// Five of a kind, where all five cards have the same label: AAAAA
// Four of a kind, where four cards have the same label and one card has a different label: AA8AA
// Full house, where three cards have the same label, and the remaining two cards share a different label: 23332
// Three of a kind, where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
// Two pair, where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
// One pair, where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
// High card, where all cards' labels are distinct: 23456

// Hands are primarily ordered based on type; for example, every full house is stronger than any three of a kind.

// If two hands have the same type, a second ordering rule takes effect.
// Start by comparing the first card in each hand.
// If these cards are different, the hand with the stronger first card is considered stronger.
// If the first card in each hand have the same label, however, then move on to considering the second card in each hand.
// If they differ, the hand with the higher second card wins; otherwise, continue with the third card in each hand, then the fourth, then the fifth.

// So, 33332 and 2AAAA are both four of a kind hands, but 33332 is stronger because its first card is stronger.
// Similarly, 77888 and 77788 are both a full house, but 77888 is stronger because its third card is stronger
//  (and both hands have the same first and second card).

// To play Camel Cards, you are given a list of hands and their corresponding bid (your puzzle input). For example:

// 32T3K 765
// T55J5 684
// KK677 28
// KTJJT 220
// QQQJA 483
// This example shows five hands; each hand is followed by its bid amount.
// Each hand wins an amount equal to its bid multiplied by its rank, where the weakest hand gets rank 1, the second-weakest hand gets rank 2, and so on up to the strongest hand.
// Because there are five hands in this example, the strongest hand will have rank 5 and its bid will be multiplied by 5.

// So, the first step is to put the hands in order of strength:

// 32T3K is the only one pair and the other hands are all a stronger type, so it gets rank 1.
// KK677 and KTJJT are both two pair. Their first cards both have the same label, but the second card of KK677 is stronger (K vs T), so KTJJT gets rank 2 and KK677 gets rank 3.
// T55J5 and QQQJA are both three of a kind. QQQJA has a stronger first card, so it gets rank 5 and T55J5 gets rank 4.

// Now, you can determine the total winnings of this set of hands by 
//  adding up the result of multiplying each hand's bid with its rank (765 * 1 + 220 * 2 + 28 * 3 + 684 * 4 + 483 * 5).
// So the total winnings in this example are 6440.

// Find the rank of every hand in your set. What are the total winnings?

const std::string givenTestData_problem1 = "\
32T3K 765\n\
T55J5 684\n\
KK677 28\n\
KTJJT 220\n\
QQQJA 483\n";

using puzzleValueType = long long int;
constexpr puzzleValueType expectedSolution_problem1 = 6440;

enum HandType_e {
    highCard,
    onePair,
    twoPair,
    threeOfAKind,
    fullHouse,
    fourOfAKind,
    fiveOfAKind,
};

const std::unordered_map<char, int> cardScoreConversionTable = { //A, K, Q, J, T, 9, 8, 7, 6, 5, 4, 3, or 2.
    {'2', 0},
    {'3', 1},
    {'4', 2},
    {'5', 3},
    {'6', 4},
    {'7', 5},
    {'8', 6},
    {'9', 7},
    {'T', 8},
    {'J', 9},
    {'Q', 10},
    {'K', 11},
    {'A', 12},
};

class Hand {
private:
    HandType_e handType;
    std::string cards;
    puzzleValueType bid;

public:
    Hand(const std::string & str){
        auto strs = StringUtils::split(str, " ");
        cards = strs[0];
        bid = std::stoll(strs[1]);
        handType = computeHandType(cards);
        assert('A' > '9');
    }

    bool operator<(const Hand & rhs) const {
        if (handType != rhs.handType) {
            return handType < rhs.handType;
        }
        assert (cards.size() == rhs.cards.size());
        for (unsigned int i=0; i < cards.size(); i++) {
            if (cards[i] != rhs.cards[i]) {
                return cardScoreConversionTable.at(cards[i]) < cardScoreConversionTable.at(rhs.cards[i]);
            }
        }
        assert(false);  // equal; not sure what now.
    }

    puzzleValueType getBid() const {
        return bid;
    }

    void print() const {
        std::cout << "Hand" << std::endl;
        std::cout << "cards:[" << cards << "]" << std::endl;
        computeHandType(cards);
        std::cout << "bid:[" << bid << "]" << std::endl;
    }

private:
    static HandType_e computeHandType(const std::string & cards) {
        std::unordered_map<char, int> cardDivision;
        for (const char & c : cards) {
            cardDivision[c]++;
        }

        if (cardDivision.size()==1) {
            std::cout << "handType:[" << "fiveOfAKind" << "]" << std::endl;
            return HandType_e::fiveOfAKind;
        }
        if (cardDivision.size()==2) {
            if ((cardDivision.begin()->second == 4) || (cardDivision.begin()->second == 1)) {
                std::cout << "handType:[" << "fourOfAKind" << "]" << std::endl;
                return HandType_e::fourOfAKind;
            } else {
                std::cout << "handType:[" << "fullHouse" << "]" << std::endl;
                return HandType_e::fullHouse;
            }
        }
        int highestCount = 0;
        int pairCount = 0;
        for (auto [card, cardCount] : cardDivision) {
            highestCount = std::max(highestCount, cardCount);
            if (cardCount == 2) {
                pairCount++;
            }
        }
        if (highestCount == 3) {
            std::cout << "handType:[" << "threeOfAKind" << "]" << std::endl;
            return HandType_e::threeOfAKind;
        }
        if (pairCount == 2) {
            std::cout << "handType:[" << "twoPair" << "]" << std::endl;
            return HandType_e::twoPair;
        }
        if (pairCount == 1) {
            std::cout << "handType:[" << "onePair" << "]" << std::endl;
            return HandType_e::onePair;
        }

        std::cout << "handType:[" << "highcard" << "]" << std::endl;
        return HandType_e::highCard;
    }
};


template<typename T>
puzzleValueType solve1(T & stream) {
    // convert to lines
    std::vector<std::string> lines;
    for (std::string line; std::getline(stream, line);) {
        lines.push_back(line);
    }

    // convert to hands; and put in ordered set
    std::set<Hand> hands;
    for (auto & line : lines) {
        Hand hand(line);
        hands.insert(hand);
    }

    std::cout << "---" << std::endl;
    // compute value
    puzzleValueType puzzleValue = 0;
    int rank = 1;
    for (const Hand & hand : hands) {
        hand.print();
        puzzleValue += hand.getBid() * rank;
        std::cout << hand.getBid() << " * " << rank << std::endl;
        rank++;
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
