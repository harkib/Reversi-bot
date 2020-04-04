#include <chrono>
#include <functional>

#include <iostream>

#include "algorithim.h"
#include "Reversi2.h"

child_ptr monte_carlo(const Reversi2& game_in, std::function<child_ptr(Reversi2&)> hueristic) {
    typedef std::chrono::system_clock clock;

    const auto choices = game_in.actions();
    const auto num_choices = choices.size();
    const uint max_playouts = 5000 / num_choices;
    const uint max_time = 10; //in seconds
    auto wins = std::vector<uint>{};
    wins.resize(num_choices, 0);
    auto player = game_in.whos_turn();
    uint choice_num = 0;
    uint n = 0;
    
    for (auto curr_time = clock::now(), end_time = curr_time + std::chrono::seconds(max_time); (n < max_playouts) && (curr_time < end_time); n++, curr_time = clock::now()){ 
        choice_num = 0;
        for (auto choice : choices) {
            auto game = Reversi2(game_in); //beware, does not copy children
            game.do_turn(choice);
            while(!game.goal_test()) {
                game.expand_children();
                if (game.get_head()->children.empty()) {
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
    uint max_score = 0;
    uint i_max = 0;
    for(uint i = 0; i < wins.size(); i++){
        if(wins[i] > max_score) {
            max_score = wins[i];
            i_max = i;
        }
    }
    std::cout << "Player " << player << " considered " << (n*num_choices) << " playouts" << std::endl;

    return &(game_in.get_head()->children.at(i_max));
}


/* HEURTISTIC FUNCTIONS
   accepts the current game and returns the best child accoring to it's own ideas
   it is assumed that at least one level of children have been expanded
*/

/* just makes random moves */
child_ptr random_h(Reversi2& game_in) {  
    auto head = game_in.get_head();
    auto rand_i = rand() % head->children.size(); //apparently not a good random element method but w/e
    return &(head->children.at(rand_i));
}

/* finds what move leads to the maximum number of availible moves next turn.
   uses the average from all possible opponent moves. */
child_ptr mobility_h(Reversi2& game_in) {
    auto head = game_in.get_head();
    float max_mobility = 0;
    child_ptr max_node = nullptr;
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
                max_node = &child; 
                child->h_value(30);
            }
        }
    }
    
    if (max_node == nullptr) {
        //no move yields any mobility so just choose a random one
        //odds are there is only one move left anyways
        return random_h(game_in);
    }

    return max_node;
}

/* makes moves that result in the fewest moves for the opponent */
child_ptr blocking_h(Reversi2& game_in) {
    auto head = game_in.get_head();
    uint min_mobility = 64;
    child_ptr min_node = nullptr;

    for (auto& child : head->children) {
        game_in.expand_children(*child);
        if (child->children.size() < min_mobility) {
            min_mobility = child->children.size();
            min_node = &child;
            child->h_value(20);
        }
    }

    if (min_node == nullptr) {
        //no move yields any blocking so just choose a random one
        //odds are there is only one move left anyways
        return random_h(game_in);
    }
    
    return min_node;
}

/* trys to have as many tiles on the board as possible */
child_ptr capture_h(Reversi2& game_in) {
    auto head = game_in.get_head();
    auto player = game_in.whos_turn();
    uint max_total = std::count(std::begin(head->board), std::end(head->board), player);
    uint total = 0;
    child_ptr max_node = nullptr;

    for (auto& child : head->children) {
        total = std::count(std::begin(child->board), std::end(child->board), player);
        if (total > max_total) {
            max_total = child->children.size();
            max_node = &child;
            child->h_value(5);
        }
    }

    if (max_node == nullptr) {
        //somehow went down in number of tiles...
        //so pick a random move
        return random_h(game_in);
    }
    
    return max_node;
}

child_ptr corner_h(Reversi2& game_in) {
    auto head = game_in.get_head();
    auto player = game_in.whos_turn();

    for (auto& child : head->children) {
        for (auto corner_space : {0, 7, 56, 63}) {
            if ((child->board.at(corner_space) == player) && (child->board.at(corner_space) == player)) {
                child->h_value(500);
                return &child;
            }
        }
    }

    //nothing is a corner so pick a random move
    return random_h(game_in);   
}

child_ptr stability_h(Reversi2&);

/* use a bunch of h functions ranked */
child_ptr combo_h(Reversi2& game_in) {
    auto head = game_in.get_head();
    uint max_value = 0;
    uint value = 0;
    child_ptr max_node = nullptr;

    mobility_h(game_in);
    blocking_h(game_in);
    capture_h(game_in);
    corner_h(game_in);
    
    for (auto& child : head->children) {
        value = child->h_value();
        if (value > max_value) {
            max_value = child->children.size();
            max_node = &child;
        }
    }

    if (max_node == nullptr) {
        //nothing had anything of value at all
        //so pick a random move
        return random_h(game_in);
    }
    
    return max_node;
}
