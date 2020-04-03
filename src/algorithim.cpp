#include <ctime>
#include <functional>

#include <iostream>

#include "algorithim.h"
#include "Reversi2.h"

Reversi2::action_t monte_carlo(const Reversi2& game_in, std::function<Reversi2::action_t(Reversi2&)> hueristic) {
    const auto choices = game_in.actions();
    const auto num_choices = choices.size();
    const uint max_playouts = 200 / num_choices;
    const uint max_time_ms = 5000;
    auto wins = std::vector<uint>{};
    wins.resize(num_choices, 0);
    auto player = game_in.whos_turn();
    uint choice_num = 0;
    
    for (uint n = 0, curr_time_ms = 0; (n < max_playouts) && (curr_time_ms < max_time_ms); n++){ //TODO make it count time
        choice_num = 0;
        for (auto choice : choices) {
            auto game = Reversi2(game_in); //beware, does not copy children
            game.do_turn(choice);
            while(!game.goal_test()) {
                game.expand_children();
                if (game.get_head()->children.size() == 0) {
                    game.skip_turn();
                } else {
                    auto next_action = hueristic(game);
                    game.do_turn(next_action);
                }
            }
            if (game.winner() == player) {
                wins.at(choice_num) += 1;
            }
            choice_num++;
        }
    }
    //choose max wins
    uint max = 0;
    uint i_max = 0;
    for(uint i = 0; i < wins.size(); i++){
        if(wins[i] > max) {
            max = wins[i];
            i_max = i;
        }
    }
    return choices.at(i_max);
}

/* just makes random moves */
Reversi2::action_t random_h(Reversi2& game_in) {  
    auto head = game_in.get_head();
    auto rand_i = rand() % head->children.size(); //apparently not a good random element method but w/e
    auto node_action = head->children.at(rand_i)->action;
    auto action = Reversi2::action_t{node_action.new_space, node_action.old_space};

    return action;
}

/* finds what move leads to the maximum number of availible moves next turn.
   uses the average from all possible opponent moves. */
Reversi2::action_t mobility_h(Reversi2& game_in) {
    auto head = game_in.get_head();
    float max_mobility = 0;
    Node* max_node = nullptr;
    float average = 0;

    
    for (auto& child : head->children) {
        average = 0;
        game_in.expand_children(*child);
        for (auto& grandchild : child->children) { //needs to be two levels to get back to the players turn
            game_in.expand_children(*grandchild);
            average += grandchild->children.size(); //or technically 3 levels
        }
        if (!child->children.empty()) {
            if ((average / float(child->children.size()))  > max_mobility) {
                max_mobility = average / float(child->children.size());
                max_node = child.get(); 
            }
        }
    }
    
    if (max_node == nullptr) {
        //no move yields any mobility so just choose a random one
        //odds are there is only one move left anyways
        return random_h(game_in);
    }

    auto node_action = max_node->action;
    auto action = Reversi2::action_t{node_action.new_space, node_action.old_space};
    return action;
}

/* makes moves that result in the fewest moves for the opponent */
Reversi2::action_t blocking_h(Reversi2& game_in) {
    auto head = game_in.get_head();
    uint min_mobility = 64;
    Node* min_node = nullptr;

    for (auto& child : head->children) {
        game_in.expand_children(*child);
        if (child->children.size() < min_mobility) {
            min_mobility = child->children.size();
            min_node = child.get();
        }
    }
    
    auto node_action = min_node->action;
    auto action = Reversi2::action_t{node_action.new_space, node_action.old_space};
    return action;
}

Reversi2::action_t capture_h(Reversi2& game_in);
Reversi2::action_t corner_h(Reversi2& game_in);



Reversi2::action_t stability_h(Reversi2&);
