#include <memory>
#include <vector>
#include <array>

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

        void expand(std::vector<board_t>);
        
    private:
        


};

void Node::expand(std::vector<board_t> moves) {
    if (children.size() == 0) {
        for (auto move : moves) {
            children.push_back(std::unique_ptr<Node>(new Node(move)));
        }
    }
}

Node::~Node() {
    for (auto &child : children) {
        child.reset(nullptr);
    }
}

