#include "reversi.h"


int main(){
   
    reversi game;
    int y,x,rand_i;   
    std::vector<std::pair<int,int>> p_moves;

    //human vs random move, use negative index to skip if no moves 
    while(!game.game_done()){    
        game.print(); 


        //Agent 1  human
        p_moves = game.possible_moves();
    
        //print possible moves
        cout << "A1 p_moves: "; 
        for (int i = 0; i < p_moves.size(); i++){
            cout << "(" << p_moves[i].first << "," << p_moves[i].second <<") ";
        } cout << endl;

        
        cin >> y;
        cin >> x;
        if (x < 0 || y < 0){
            game.skip_turn();
        }else{
            game.make_move(y,x);
        } 

        //Agent 2 - random move
        p_moves = game.possible_moves();
        if(p_moves.size()==0){
            game.skip_turn();
        } else {

            rand_i = rand() % p_moves.size();
            y = p_moves[rand_i].first;
            x = p_moves[rand_i].second;

            cout <<"A2 move: " << y <<"," << x << endl;
            game.make_move(y,x);
        }
    }   

    game.print(); 
    int winner = game.winner();
    cout << "winner: " << winner << endl;
    return 0;
}   