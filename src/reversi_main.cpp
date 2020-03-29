#include "reversi.h"
#include "Node.h"

//Assumes it is player 1, ie game.winner() = 1 -> it won
// std::pair<int,int> betterAI (reversi game_og){
//     cout << "to be implemented" << endl;

//     return
// }

//Assumes it is player 2
std::pair<int,int> pMCTS (reversi game_og){
    
    std::vector<std::pair<int,int>> p_moves_og = game_og.possible_moves();

    //no moves
    if ( p_moves_og.size() == 0){
        return make_pair(-1,-1);
    }

    std::vector<std::pair<int,int>> p_moves;
    int num_playouts = 1000;
    std::vector<int> wins;
    int x,y,rand_i;

    reversi game;
    //perfrom posible move then playout
    for(int i = 0; i < p_moves_og.size(); i++){
        game = game_og;
        game.make_move(p_moves_og[i].first, p_moves_og[i].second);
        wins.push_back(0);

        //play out game
        for (int n = 0; n < num_playouts; n++){
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

    return p_moves_og[i_max];

}
 

int main(){

    uint test_board[8][8] = {};
    auto test_node = new Node(test_board);
   
    reversi game;
    int y,x, rand_i;  
    pair<int,int> move_A1; 
    pair<int,int> move_A2;
    std::vector<std::pair<int,int>> p_moves;

    //random move vs pMCTS, use negative index to skip if no moves 
    while(!game.game_done()){    
        game.print(); 

        //Agent 1 - random 
        p_moves = game.possible_moves();
        if(p_moves.size()==0){
            game.skip_turn();
        } else {

            rand_i = rand() % p_moves.size();
            y = p_moves[rand_i].first;
            x = p_moves[rand_i].second;

            game.make_move(y,x);
        }

        //Agent 2 - pMCTS
        move_A2 = pMCTS(game);
        y = move_A2.first;
        x = move_A2.second; 
        if (x < 0 || y < 0){
            game.skip_turn();
        }else{
            game.make_move(y,x);
        } 
    }   

    game.print(); 
    int winner = game.winner();
    cout << "winner: Agent " << winner << endl;
    return 0;
}   