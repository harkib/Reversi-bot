#include <memory>
#include <vector>

class Node{
    typedef uint board_type[8][8];
    const std::unique_ptr<board_type> board;
    std::vector<std::unique_ptr<Node>> children;

    public:
        Node(board_type &board_in) : board(std::unique_ptr<board_type>(&board_in)) {}
        //~Node();

        void expand(void);
        
    private:
        


};
