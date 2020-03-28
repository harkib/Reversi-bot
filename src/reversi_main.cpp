#include "reversi.h"

int main(){

    reversi game;
    //game.print();    
    game.make_move(5,3);
    game.make_move(3,1);
    game.print();   
    return 0;
}   