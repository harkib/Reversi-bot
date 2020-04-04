#include "Node.h"
#include "Reversi2.h"
#include "algorithim.h"

#include <functional>
#include <iostream>

int main(){
    std::function<child_ptr(Reversi2& game_in)> random_f = random_h;
    std::function<child_ptr(Reversi2& game_in)> mobility_f = mobility_h;
    std::function<child_ptr(Reversi2& game_in)> capture_f = capture_h;
    std::function<child_ptr(Reversi2& game_in)> corner_f = corner_h;
    //std::function<child_ptr(Reversi2& game_in)> stability_f = stability_h;
    std::function<child_ptr(Reversi2& game_in)> blocking_f = blocking_h;

    auto game = Reversi2();

    while (!game.goal_test()) {
        game.print();
        std::cout << std::endl;
        game.expand_children();
        if (game.get_head()->children.size() == 0) {
            game.skip_turn();
        } else {
            if (((game.get_head()->turn) % 2) == 0) {
                game.do_turn(monte_carlo(game, corner_f)); //player B
            } else {
                game.do_turn(monte_carlo(game, random_f)); //player W
            }
        }
    }
    std::cout << "GAME OVER" << std::endl << game.winner() << " wins!" << std::endl;

    return 0;
}   