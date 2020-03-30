#include <memory>
#include <vector>
#include <array>

#include "Node.h"

#ifndef __GAME_HEADER__
#define __GAME_HEADER__


class Reversi2 {
    const char blank, player1, player2; 
    std::unique_ptr<Node> head;
    

    public:
        typedef uint8_t space_t;
        typedef std::array<space_t, 64> board_t;

        Reversi2();

        void print();

    private:
        // all of these below are just sort of rough guesses
        std::vector<std::pair<int,int>> actions();
        std::vector<std::pair<int,int>> actions(Node);

        bool goal_test(Node);
        bool valid_move(board_t, space_t, char);
        board_t result(board_t, space_t, char);

        space_t coord2space(std::pair<int,int>);
        space_t coord2space(int,int);

};

#endif
