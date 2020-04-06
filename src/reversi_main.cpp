#include "reversi.h"
#include <cstdio>
#include <ctime>


//returns max or min heuristic after searching given depth using minmax
double minmax(reversi game_og, int depth, bool maximizing_player){

    //terminatation node
    if(depth == 0 || game_og.game_done()){
        return game_og.get_heuristic();
    }

    //generate all childern (moves)
    std::vector<std::pair<int,int>> p_moves_og = game_og.possible_moves();
    
    //no childern, terminate 
    if ( p_moves_og.size() == 0){
        return game_og.get_heuristic();
    }

    //generate all childern (games)
    reversi game;
    std::vector<reversi> p_games; 
    for(int i = 0; i <p_moves_og.size(); i++){
        game = game_og;
        game.make_move(p_moves_og[i].first,p_moves_og[i].second);
        p_games.push_back(game);
    }

    //get max/min of childern
    double max_h = -game_og.get_h_bound();
    double min_h =  game_og.get_h_bound();
    double h_child;
    if(maximizing_player){
        for(int i = 0; i < p_games.size(); i++){
            h_child = minmax(p_games[i],depth -1, !maximizing_player);
            if(h_child > max_h){
                max_h = h_child;
            }
        }
        return max_h;
    }else {
        for(int i = 0; i < p_games.size(); i++){
            h_child = minmax(p_games[i],depth -1, !maximizing_player);
            if(h_child < min_h){
                min_h = h_child;
            }
        }
        return min_h;
    }
    

}

//returns max or min heuristic after searching given depth using minmax with alphabeta pruning
double minmax_alphabeta(reversi game_og, int depth, bool maximizing_player, double alpha, double beta){

    //terminatation node
    if(depth == 0 || game_og.game_done()){
        return game_og.get_heuristic();
    }

    //generate all childern (moves)
    std::vector<std::pair<int,int>> p_moves_og = game_og.possible_moves();
    
    //no childern, terminate 
    if ( p_moves_og.size() == 0){
        return game_og.get_heuristic();
    }

    //generate all childern (games)
    reversi game;
    std::vector<reversi> p_games; 
    for(int i = 0; i <p_moves_og.size(); i++){
        game = game_og;
        game.make_move(p_moves_og[i].first,p_moves_og[i].second);
        p_games.push_back(game);
    }

    //get max/min of childern
    double max_h = -game_og.get_h_bound();
    double min_h =  game_og.get_h_bound();
    double h_child;
    if(maximizing_player){
        for(int i = 0; i < p_games.size(); i++){
            h_child = minmax_alphabeta(p_games[i],depth -1, !maximizing_player, alpha, beta);
            if(h_child > max_h){
                max_h = h_child;
            }
            if(h_child > alpha){
                alpha = h_child;
            }
            if(alpha >= beta){
                break;
            }
        }
        return max_h;
    }else {
        for(int i = 0; i < p_games.size(); i++){
            h_child = minmax_alphabeta(p_games[i],depth -1, !maximizing_player, alpha, beta);
            if(h_child < min_h){
                min_h = h_child;
            }
            if(h_child < beta){
                beta = h_child;
            }
            if(alpha >= beta){
                break;
            }
        }
        return min_h;
    }
    
}

//Assumes it is player 1
//heuristic assuming 1 = player 1 (B) = Maximizing, 2 = player 2 (W) = minimixing 
std::pair<int,int> betterAI (reversi game_og){
    
    std::vector<std::pair<int,int>> p_moves_og = game_og.possible_moves();

    //no moves
    if ( p_moves_og.size() == 0){
        return std::make_pair(-1,-1);
    }

    //run minmax on all possible moves
    std::vector<double> h_vals;
    reversi game;   
    for(int i = 0; i < p_moves_og.size(); i++){
        game = game_og;
        game.make_move(p_moves_og[i].first, p_moves_og[i].second);
        h_vals.push_back(minmax_alphabeta(game, 5, false,-game.get_h_bound(),game.get_h_bound())); //seems to do better when minmax finishes on agent 1 ie depth is even
    }

    //choose max h  
    int max_h = -game_og.get_h_bound();
    int i_max = 0;
    for(int i = 0; i < p_moves_og.size(); i++){
        if(h_vals[i] > max_h){
            max_h = h_vals[i];
            i_max = i;
        }
    }

    std::cout << "Agent 1 move h: " << h_vals[i_max] << std::endl;
    return p_moves_og[i_max];

}

//Assumes it is player 2
std::pair<int,int> pMCTS (reversi game_og){
    
    std::vector<std::pair<int,int>> p_moves_og = game_og.possible_moves();

    //no moves
    if ( p_moves_og.size() == 0){
        return std::make_pair(-1,-1);
    }

    std::vector<std::pair<int,int>> p_moves;
    int num_playouts = 1000;
    
    std::vector<int> wins;
    int x,y,rand_i;

    reversi game_pm,game;
    //perfrom posible move then playout
    for(int i = 0; i < p_moves_og.size(); i++){
        game_pm = game_og;
        game_pm.make_move(p_moves_og[i].first, p_moves_og[i].second);
        wins.push_back(0);

        //play out game
        for (int n = 0; n < num_playouts; n++){
            game = game_pm;
            while(!game.game_done()){
                p_moves = game.possible_moves();
                if(p_moves.size()==0){
                    game.skip_turn();
                } else {

                    rand_i = rand() % p_moves.size();
                    y = p_moves[rand_i].first;
                    x = p_moves[rand_i].second;

                    game.make_move(y,x);
                }
            }
            if (game.winner() == 2){
                 wins[i] += 1;
            }
        }
       
    }

    //choose max wins
    int max = 0;
    int i_max = 0;
    for(int i = 0; i < p_moves_og.size(); i++){
        if(wins[i] > max){
            max = wins[i];
            i_max = i;
        }
    }

    std::cout << "Agent 2 move wins %: " << 100*(float)wins[i_max]/num_playouts << std::endl;
    return p_moves_og[i_max];

}
 

int main(){
    std::clock_t start;
    double duration;
    double A1_total_time = 0;
    double A2_total_time = 0;
    int A1_total_moves = 0;
    int A2_total_moves = 0;


    reversi game;
    int y,x, rand_i;  
    std::pair<int,int> move_A1; 
    std::pair<int,int> move_A2;
    std::vector<std::pair<int,int>> p_moves;

    //random move vs pMCTS, use negative index to skip if no moves 
    while(!game.game_done()){    
        game.print(); 

        //Agent 1 - betterAI 
        start = std::clock();
        move_A1 = betterAI(game);
        duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        std::cout << "Time taken (s): "<< duration << std::endl;
        y = move_A1.first;
        x = move_A1.second; 
        if (x < 0 || y < 0){
            game.skip_turn();
        }else{
            A1_total_time += duration;
            A1_total_moves ++;
            game.make_move(y,x);
        } 

        //Agent 2 - pMCTS
        start = std::clock();
        move_A2 = pMCTS(game);
        duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        std::cout << "Time taken (s): " << duration << std::endl;
        y = move_A2.first;
        x = move_A2.second; 
        if (x < 0 || y < 0){
            game.skip_turn();
        }else{
            A2_total_time += duration;
            A2_total_moves ++;
            game.make_move(y,x);
        } 
    }   

    game.print(); 
    int winner = game.winner();
    std::cout << "winner: Agent " << winner << std::endl;
    std::cout << "Agnet 1 average time (s): " << A1_total_time/A1_total_moves << std::endl;
    std::cout << "Agnet 2 average time (s): " << A2_total_time/A2_total_moves<< std::endl;
    return 0;
}   