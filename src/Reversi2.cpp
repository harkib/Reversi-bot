#include "Reversi2.h"


Reversi2::Reversi2() :
    blank('0'),
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
    })))
{}
