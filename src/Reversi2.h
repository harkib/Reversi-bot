#include <memory>
#include <vector>
#include <array>



#ifndef __GAME_HEADER__
#define __GAME_HEADER__
#include "Node.h"

class Reversi2 {
    const char blank, player1, player2; 
    std::unique_ptr<Node> head;
    

    public:
        typedef uint8_t space_t;
        typedef std::array<space_t, 64> board_t;
        struct action_t {
            space_t space, destination;
        };

        Reversi2();

        void print();
        
        std::vector<action_t> actions();

    private:
        // all of these below are just sort of rough guesses
        
        std::vector<action_t> actions(Node&);

        bool goal_test(Node);
        bool valid_move(board_t, space_t, char);
        board_t result(board_t, action_t, char);

        space_t coord2space(std::pair<int,int>);
        space_t coord2space(int,int);

};

#endif
