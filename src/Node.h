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
        int value = 0;
        std::vector<std::unique_ptr<Node>> children{};
        
        Node(board_t board_in) : board(board_in) {}
        Node(board_t board_in, uint8_t turn_in, bool skipped_in) : 
            board(board_in), 
            turn(turn_in), 
            skipped(skipped_in) {}
        Node(const Node& node_in) : 
            board(node_in.board), 
            turn(node_in.turn), 
            skipped(node_in.skipped),
            value(node_in.value) {}
        
        ~Node() {
            for (auto &child : children) {
                child.reset(nullptr);
            }
        }

        void expand(std::vector<board_t> boards) {
            if (children.empty()) {
                for (auto board : boards) {
                    children.push_back(std::unique_ptr<Node>(new Node(board, turn+1, false)));
                }
            }
        }

        auto h_value() {
            return value;
        }

        auto h_value(int value_in) {
            value += value_in;
            return value;
        }
        
    private:
        
};

#endif
