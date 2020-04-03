#include "Node.h"
#include "Reversi2.h"
#include "algorithim.h"

#include <functional>
#include <iostream>

int main(){
    std::function<Reversi2::action_t(Reversi2& game_in)> random_f = random_h;
    std::function<Reversi2::action_t(Reversi2& game_in)> mobility_f = mobility_h;
    std::function<Reversi2::action_t(Reversi2& game_in)> capture_f = capture_h;
    std::function<Reversi2::action_t(Reversi2& game_in)> corner_f = corner_h;
    //std::function<Reversi2::action_t(Reversi2& game_in)> stability_f = stability_h;
    std::function<Reversi2::action_t(Reversi2& game_in)> blocking_f = blocking_h;

    auto game = Reversi2();
    auto action = Reversi2::action_t{0,0};

    while (!game.goal_test()) {
        game.print();
        std::cout << std::endl;
        game.expand_children();
        if (game.get_head()->children.size() == 0) {
            game.skip_turn();
        } else {
            if (((game.get_head()->turn) % 2) == 0) {
                action = monte_carlo(game, corner_f); //player B
            } else {
                action = monte_carlo(game, random_f); //player W
            }
            game.do_turn(action);
        }
    }
    std::cout << "GAME OVER" << std::endl << game.winner() << " wins!" << std::endl;

    return 0;
}   