#include <memory>
#include <vector>
#include <array>

#ifndef __NODE_HEADER__
#define __NODE_HEADER__


class Node{
    typedef std::array<uint, 64> board_t; 
    
    std::vector<std::unique_ptr<Node>> children{};

    public:
        const board_t board;
        
        Node(board_t board_in) : board(board_in) {}
        ~Node();

        void expand(std::vector<board_t>);
        
    private:
        


};

inline void Node::expand(std::vector<board_t> moves) {
    if (children.size() == 0) {
        for (auto move : moves) {
            children.push_back(std::unique_ptr<Node>(new Node(move)));
        }
    }
}

inline Node::~Node() {
    for (auto &child : children) {
        child.reset(nullptr);
    }
}

#endif
