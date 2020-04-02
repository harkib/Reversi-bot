#include <ctime>

#include "algorithim.h"
#include "Reversi2.h"

Reversi2::action_t monte_carlo(const Reversi2& game_in, const std::vector<Reversi2::action_t>& choices, Reversi2::action_t (*hueristic)(void)) {
    const uint max_playouts = 1000;
    const uint max_time_ms = 5000;
    auto wins = std::vector<uint>{};
    auto best_move = choices.at(0); //TODO this is a placeholder until the best is chosen
    auto player = game_in.whos_turn();
    uint i = 0;

    //perfrom posible move then playout
    for (auto choice : choices) {
        auto game = Reversi2(game_in);
        wins.push_back(0);
        game.do_turn(choice);
        auto next_actions = game.actions();

        for (uint n = 0, curr_time_ms = 0; (n < max_playouts) && (curr_time_ms < max_time_ms); n++){ //TODO make it count time
            while(!game.goal_test()) {
                next_actions = game.actions();
                if (next_actions.size() == 0) {
                    game.skip_turn();
                } else {
                    auto next_action = hueristic(); //TODO make the h functions and thier param type
                    game.do_turn(next_action);
                }
            }
            if (game.winner() == player) {
                wins.at(i) += 1;
            }  
            i++;
        }  
    }
    //TODO choose max scored move
    return best_move;
}


Reversi2::action_t random_h();
Reversi2::action_t mobility_h();
Reversi2::action_t capture_h();
Reversi2::action_t corner_h();
Reversi2::action_t stability_h();
