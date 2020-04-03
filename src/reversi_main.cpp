#include "Node.h"
#include "Reversi2.h"
#include "algorithim.h"

#include <functional>
#include <iostream>

int main(){
    std::function<Reversi2::action_t(Reversi2& game_in)> random_f = random_h;
    std::function<Reversi2::action_t(Reversi2& game_in)> mobility_h = mobility_h;
    std::function<Reversi2::action_t(Reversi2& game_in)> capture_h = capture_h;
    std::function<Reversi2::action_t(Reversi2& game_in)> corner_h = corner_h;
    std::function<Reversi2::action_t(Reversi2& game_in)> stability_h = stability_h;

    auto game = Reversi2();
    auto action = Reversi2::action_t{0,0};

    while (!game.goal_test()) {
        game.print();
        game.expand_children();
        if (game.get_head()->children.size() == 0) {
            game.skip_turn();
        } else {
            if (((game.get_head()->turn) % 2) == 0) {
                action = monte_carlo(game, random_f); //TODO change to a better one
            } else {
                action = monte_carlo(game, random_f);
            }
            game.do_turn(action);
        }
    }
    std::cout << "GAME OVER" << std::endl << game.winner() << " wins!" << std::endl;

    return 0;
}   