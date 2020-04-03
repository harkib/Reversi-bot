#include <memory>
#include <vector>
#include <array>

#ifndef __NODE_HEADER__
#define __NODE_HEADER__


//TODO this should be templte based to provide the board_t type
class Node{ 
    typedef std::array<uint8_t, 64> board_t; //needs to be identical to Reversi2::board_t until it's template based
    typedef struct {
        int8_t new_space, old_space;
    } action_t; 
    
    public:
        const board_t board;
        const action_t action{-1, -1};
        const uint8_t turn = 0;
        const bool skipped = false;
        std::vector<std::unique_ptr<Node>> children{};
        
        Node(board_t board_in) : board(board_in) {}
        Node(board_t board_in, action_t action_in, uint8_t turn_in, bool skipped_in) : 
            board(board_in), 
            action(action_in),
            turn(turn_in), 
            skipped(skipped_in) {}
        Node(const Node& node_in) : 
            board(node_in.board), 
            action(node_in.action),
            turn(node_in.turn), 
            skipped(node_in.skipped) {}
        
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
