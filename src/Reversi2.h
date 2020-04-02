#include <memory>
#include <vector>
#include <array>



#ifndef __GAME_HEADER__
#define __GAME_HEADER__
#include "Node.h"

class Reversi2 {
    const unsigned char blank, player1, player2; 
    std::unique_ptr<Node> head;


    

    public:
        typedef uint8_t space_t;
        typedef std::array<space_t, 64> board_t;
        typedef struct {
            int8_t new_space, old_space;
        } action_t;
        

        Reversi2();

        void print();
        std::vector<action_t> actions();
        
        void do_turn(action_t);
        void skip_turn();
        
        bool goal_test(); //TODO make private after tests

        void expand_children();


    private:
        std::vector<action_t> actions(const Node&);
        bool consistent_line(int8_t, int8_t);
        
        void print(const Node&);
        void print(const board_t&);

        std::vector<action_t> find_rows(const board_t&, space_t, int8_t);
        
        
        bool goal_test(const Node&);
        board_t result(const board_t&, action_t);
        
};

#endif
