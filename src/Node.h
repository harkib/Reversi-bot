#include <memory>
#include <vector>
#include <array>

#ifndef __NODE_HEADER__
#define __NODE_HEADER__


//TODO this should be templte based to provide the board_t type
class Node{ 
    typedef std::array<uint8_t, 64> board_t; //needs to be identical to Reversi2::board_t until it's template based
    
    public:
        const board_t board;
        const uint8_t turn = 0;
        const bool skipped = false;
        std::vector<std::unique_ptr<Node>> children{};
        
        Node(board_t board_in) : board(board_in) {}
        Node(board_t board_in, uint8_t turn_in, bool skipped_in) : board(board_in), turn(turn_in), skipped(skipped_in) {}
        ~Node() {
            for (auto &child : children) {
                child.reset(nullptr);
            }
        }

        void expand(std::vector<board_t> moves) {
            if (children.size() == 0) {
                for (auto move : moves) {
                    children.push_back(std::unique_ptr<Node>(new Node(move)));
                }
            }
        }
        
    private:
        
};

#endif
