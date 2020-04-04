#include <memory>
#include <vector>
#include <array>
#include <algorithm>



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
        Reversi2(const Reversi2& game_in) : 
            blank(game_in.blank), 
            player1(game_in.player1), 
            player2(game_in.player2), 
            head(std::unique_ptr<Node>(new Node(*game_in.head.get()))) {}

        void print() const;
        std::vector<action_t> actions() const;
        
        void do_turn(action_t);
        Node* do_turn(std::unique_ptr<Node>*);
        void skip_turn();
        
        bool goal_test() const; //TODO make private after tests

        void expand_children();
        void expand_children(Node&);
        Node* get_head() const;

        space_t winner() const;
        const space_t whos_turn() const;


    private:
        std::vector<action_t> actions(const Node&) const;
        bool consistent_line(int8_t, int8_t) const;
        
        void print(const Node&) const;
        void print(const board_t&) const;

        std::vector<action_t> find_rows(const board_t&, space_t, int8_t) const;
        
        bool goal_test(const Node&) const;
        board_t result(const board_t&, action_t) const;

        space_t winner(const Node&) const;
        space_t whos_turn(const Node&) const;
        
};

#endif
