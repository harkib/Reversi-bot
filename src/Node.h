#include <memory>
#include <vector>
#include <array>
#include <iterator>

//debuggin:
#include <iostream> 
#include <typeinfo>

class Node{
    typedef std::array<uint, 64> board_t; 
    const board_t board;
    std::vector<std::unique_ptr<Node>> children{};

    public:
        Node(board_t board_in) : board(board_in) {}
        ~Node();

        void expand(std::vector<std::pair<int,int>>, int);
        
    private:
        


};

void Node::expand(std::vector<std::pair<int,int>> moves, int symbol) {
    for (auto move : moves) {
        board_t new_board = board;
        //std::copy(std::begin(*board.get()), std::end(*board.get()), std::begin(new_board));
        new_board[move.first + (8*move.second)] = symbol;
        children.push_back(std::unique_ptr<Node>(new Node(new_board)));
    }
}

Node::~Node() {
    for (auto &child : children) {
        child.reset(nullptr);
    }
}

