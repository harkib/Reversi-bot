#include <memory>
#include <vector>
#include <array>



#ifndef __GAME_HEADER__
#define __GAME_HEADER__
#include "Node.h"

class Reversi2 {
    const char blank, player1, player2; 
    std::unique_ptr<Node> head;

    uint8_t turn = 0;
    

    public:
        typedef uint8_t space_t;
        typedef std::array<space_t, 64> board_t;
        typedef struct {
            space_t space, destination;
        } action_t;

        Reversi2();

        void print();
        
        
        std::vector<action_t> actions();
        void do_turn(action_t);
        
        bool goal_test(); //TODO make private after tests

        void expand_childred();

    private:
        // all of these below are just sort of rough guesses
        
        std::vector<action_t> actions(const Node&);
        void print(const Node&);
        
        bool goal_test(const Node&);
        bool valid_move(board_t, space_t, char);
        board_t result(const board_t&, action_t);

        space_t coord2space(std::pair<int,int>);
        space_t coord2space(int,int);

};

#endif
