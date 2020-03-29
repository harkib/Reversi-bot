#include <memory>
#include <vector>
#include <iterator>

//debuggin:
#include <iostream> 
#include <typeinfo>

class Node{
    typedef uint board_t[64];
    const std::unique_ptr<board_t> board;
    std::vector<std::unique_ptr<Node>> children;

    public:
        Node(board_t &board_in) : board(std::unique_ptr<board_t>(&board_in)) {}
        //~Node();

        void expand(std::vector<std::pair<int,int>>, int);
        
    private:
        


};

void Node::expand(std::vector<std::pair<int,int>> moves, int symbol) {
    for (auto move : moves) {
        board_t new_board;
        std::copy(std::begin(*board), std::end(*board), std::begin(new_board));
        new_board[move.first + (8*move.second)] = symbol;
        children.push_back(std::unique_ptr<Node>(new Node(new_board)));
    }
}

