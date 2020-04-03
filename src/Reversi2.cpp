#include "Reversi2.h"
#include <iostream>

Reversi2::Reversi2() :
    blank(' '), //make it a space for nicer display
    player1('B'),
    player2('W'),
    head(std::unique_ptr<Node>(new Node(board_t{
        blank, blank, blank, blank, blank, blank, blank, blank,
        blank, blank, blank, blank, blank, blank, blank, blank,
        blank, blank, blank, blank, blank, blank, blank, blank,
        blank, blank, blank, player1, player2, blank, blank, blank,
        blank, blank, blank, player2, player1, blank, blank, blank,
        blank, blank, blank, blank, blank, blank, blank, blank,
        blank, blank, blank, blank, blank, blank, blank, blank,
        blank, blank, blank, blank, blank, blank, blank, blank
    }))) //yes, really
{}

void Reversi2::print() const {
    print(*head);
}

void Reversi2::print(const Reversi2::board_t& board) const {
    print(Node(board));
}

void Reversi2::print(const Node& node) const {
    std::cout << "┌───┬───┬───┬───┬───┬───┬───┬───┐" << std::endl;
    //std::cout << "| " << char(head->board[0]) << " ";
    for (uint8_t i = 0; i < node.board.size()-1; i++) {
        std::cout << "| " << char(node.board[i]) << " ";
        if ((i+1) % 8 == 0) {
            std::cout << "|" << std::endl << "├───┼───┼───┼───┼───┼───┼───┼───┤" << std::endl;
        }
    }
    std::cout << "| " << char(node.board.back()) << " |" << std::endl; // it was a pain to get the last or first to print correctly so this is easier
    std::cout << "└───┴───┴───┴───┴───┴───┴───┴───┘" << std::endl;
}

//checks that the x doesn't change too much, indicating a wrap-around. and within the bounds
bool Reversi2::consistent_line(int8_t curr_point, int8_t prev_point) const {
    return ((curr_point >= 0 && curr_point < 64) && (abs((curr_point % 8) - (prev_point % 8)) <= 1));
}

std::vector<Reversi2::action_t> Reversi2::actions() const {
    return actions(*head);
}

std::vector<Reversi2::action_t> Reversi2::actions(const Node& node) const {
    auto moves =std::vector<action_t>{};

    auto player = whos_turn(node);
    
    // this part works sort of like polar coordinates. it first goes through the entire board to find it's own pieces. 
    // then it goes through each cardinal direction and moves outward until it:
    //  - can make a move and then adds it to the moves vector
    //  - hits the edge of the board
    //  - sees it's own type of piece
    // it only moves out along the opponent's type of pieces
    for (uint8_t i = 0; i < node.board.size(); i++) { //it'd be nice if this was range based
        if (node.board.at(i) == player) {
            auto new_moves = find_rows(node.board, blank, i);
            moves.insert(std::begin(moves), std::begin(new_moves), std::end(new_moves));
        }
    }

    return moves; //I'm not sure why the linter is picking this up as an error

    /*  
   -18     -16     -14
        -9  -8  -7
    -2  -1  n   1   2
        7   8   9
    14      16      18

    dn = n + 1*dx + 8*dy
    dn = n + r*theta

    x = (n mod 8) + 1
    y = (n / 8) + 1

    (x >= 3) --> (-1)(dx=-1) left
    (x <= 6) --> (+1)(dx= 1) right
    (y >= 3) --> (-8)(dy=-1) above
    (y <= 6) --> (+8)(dy= 1) below 
    */
}

//find rows from a given space. uses a specified end for different uses.
std::vector<Reversi2::action_t> Reversi2::find_rows(const Reversi2::board_t& board, Reversi2::space_t end_char, int8_t space) const {
    auto rows =std::vector<action_t>{};
    const auto theta_list = std::array<int8_t, 8>{-9, -8, -7, -1, 9, 8, 7, 1};

    space_t player, opponent;
    player = board.at(space);
    if (player == player1) {
        opponent = player2;
    } else {
        opponent = player1;
    }

    for (auto theta : theta_list) {
        for (int8_t r = 1; consistent_line(space + r*theta, space + (r-1)*theta); r++) {
            if ((r > 1) && (board.at(space + r*theta) == end_char)) {
                rows.push_back(action_t{space + r*theta, space});
                break;
            } else if (board.at(space + r*theta) != opponent) { 
                break;
            }
        }
    }

    return rows;
}

bool Reversi2::goal_test() const { 
    return goal_test(*head);
}

bool Reversi2::goal_test(const Node& node) const {
    return ((actions(node).size() == 0) && node.skipped);
}


Reversi2::board_t Reversi2::result(const Reversi2::board_t& old_board, Reversi2::action_t action) const {
    auto new_board = old_board;
    auto symbol = old_board.at(action.old_space);
    int8_t dist, offset;

    new_board.at(action.new_space) = symbol;

    //for each row, it does some math to fill in what's in between
    auto rows = find_rows(new_board, symbol, action.new_space);
    for (auto row : rows) {
        dist = (row.old_space % 8) - (row.new_space % 8);
        if (dist == 0) {
            dist = (row.old_space / 8) - (row.new_space / 8);
        }
        offset = (row.new_space - row.old_space)/abs(dist);
        for (int8_t space = row.old_space; space != row.new_space; space += offset) {
            new_board.at(space) = symbol;
        }
    }
    
    return new_board; //should this be by referance?
} 

void Reversi2::expand_children() {
    expand_children(*head);
}

void Reversi2::expand_children(Node& node) {
    auto new_boards = std::vector<board_t>{};
    auto moves = actions(node);
    auto moves_new = std::vector<int8_t>{}; //what a horrible way to do this but sharing typedefs is just not working.. D:
    auto moves_old = std::vector<int8_t>{};

    for (auto move : moves) {
        new_boards.push_back(result(head->board, move));
        moves_new.push_back(move.new_space);
        moves_old.push_back(move.old_space);
    }

    node.expand(new_boards, moves_new, moves_old);
}

void Reversi2::do_turn(Reversi2::action_t move) {
    head.reset(new Node(result(head->board, move), {move.new_space, move.old_space}, (head->turn+1), false)); //lol the action_t bit is so bad
    //print();
}

void Reversi2::skip_turn() {
    head.reset(new Node(head->board, {-1, -1}, (head->turn+1), true));
    //print();
}

Reversi2::space_t Reversi2::winner() const {
    return winner(*head.get());
}

Reversi2::space_t Reversi2::winner(const Node& node) const {
    if (goal_test(node)) {
        //this is the the only thing that uses <algorithm> and could easiliy be remade without it
        auto player1_score = std::count(std::begin(node.board), std::end(node.board), player1); 
        auto player2_score = std::count(std::begin(node.board), std::end(node.board), player2);
        if (player1_score > player2_score) {
            return player1;
        } else if (player1_score < player2_score) {
            return player2;
        } else {
            return blank; //tie
        }
    } else {
        return NULL; //uh oh
    }
}

const Reversi2::space_t Reversi2::whos_turn() const {
    return whos_turn(*head);
}

Reversi2::space_t Reversi2::whos_turn(const Node& node) const {
    if (node.turn % 2 == 0) {
        return player1;
    } else {
        return player2;
    }
}

Node* Reversi2::get_head() const {
    return head.get();
}


