#include "Reversi2.h"
#include <iostream>

Reversi2::Reversi2() :
    blank('0'), //make it a space for nicer display
    player1('B'),
    player2('W'),
    head(std::unique_ptr<Node>(new Node(std::array<uint, 64>{
        blank, blank, blank, blank, blank, blank, blank, blank,
        blank, blank, blank, blank, blank, blank, blank, blank,
        blank, blank, blank, blank, blank, blank, blank, blank,
        blank, blank, blank, player1, player2, blank, blank, blank,
        blank, blank, blank, player2, player1, blank, blank, blank,
        blank, blank, blank, blank, blank, blank, blank, blank,
        blank, blank, blank, blank, blank, blank, blank, blank,
        blank, blank, blank, blank, blank, blank, blank, blank
    }))) //yes, really
{}

void Reversi2::print() {
    std::cout << "┌───┬───┬───┬───┬───┬───┬───┬───┐" << std::endl;
    //std::cout << "| " << char(head->board[0]) << " ";
    for (auto i = 0; i < head->board.size()-1; i++) {
        std::cout << "| " << char(head->board[i]) << " ";
        if ((i+1) % 8 == 0) {
            std::cout << "|" << std::endl << "├───┼───┼───┼───┼───┼───┼───┼───┤" << std::endl;
        }
    }
    std::cout << "| " << char(head->board.back()) << " |" << std::endl; // it was a pain to get the last or first to print correctly so this is easier
    std::cout << "└───┴───┴───┴───┴───┴───┴───┴───┘" << std::endl;
}


std::vector<Reversi2::action_t> Reversi2::actions() {
    return actions(*head);
}

std::vector<Reversi2::action_t> Reversi2::actions(Node& node) {
    std::vector<action_t> moves{};
    std::vector<int8_t> check{};
    for (auto i = 0; i < node.board.size(); i++) { //it'd be nice if this was range based
        if (node.board[i] == player1) { // TODO make it agnostic to which player
            // TODO there has got to be a better way to do this. It's so many lines for so little.
            if                  (i / 8 >= 2) { //N
                check.push_back(-8);
            }
            if ((i % 8 <= 5) && (i / 8 >= 2)) { //NE
                check.push_back(-7);
            }
            if (i % 8 <= 5) {                   //E
                check.push_back(1);
            }
            if ((i % 8 <= 5) && (i / 8 <= 5)) { //SE
                check.push_back(9);
            }
            if                  (i / 8 <= 5) {  //S
                check.push_back(8);
            }
            if ((i % 8 >= 2) && (i / 8 <= 5)) { //SW
                check.push_back(7);
            }
            if (i % 8 >= 2) {                   //W
                check.push_back(-1);
            }
            if ((i % 8 >= 2) && (i / 8 >= 2)) { //NW
                check.push_back(-9);
            }

            for (auto nieghbor : check) {
                if ((node.board[i + nieghbor] == player2) && (node.board[i + 2*nieghbor] == blank)) { // TODO make it agnostic to which player
                    std::cout << "found move: " << i << " to " << (i+2*nieghbor) << std::endl;
                    moves.push_back({i, i+2*nieghbor});
                }
            }

            check.clear();
        }
    }

    /*
    -18     -16     -14
        -9  -8  -7
    -2  -1  n   1   2
        7   8   9
    14      16      18

    dn = n + 1*dx + 8*dy

    x = (n mod 8) + 1
    y = (n / 8) + 1

    (x >= 3) --> (-1)(dx=-1) left
    (x <= 6) --> (+1)(dx= 1) right
    (y >= 3) --> (-8)(dy=-1) above
    (y <= 6) --> (+8)(dy= 1) below
    */
    return moves;
}

bool Reversi2::goal_test() {
    return goal_test(*head);
}

bool Reversi2::goal_test(Node& node) {
    return (actions(node).size() == 0);
}

