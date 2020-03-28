//reversi.h
#include <vector>
#include <iostream>
#include <assert.h> 
#include <utility>
using namespace std;

class reversi {

    //0 = empty, 1 = player 1 (B), 2 = player 2 (W)
    //top left is 0,0  bottom right is 7,7
    std::vector<std::vector<int>> board;
    int turn = 1; //1 or 2

    public:

    reversi();
    reversi(int);
    void print();
    std::vector<std::pair<int,int>> possible_moves();
    void make_move (int,int);

    private:
    bool valid_move(int,int);

};

reversi::reversi(){ 

        vector<int> row = {0,0,0,0,0,0,0,0};
        for(int j = 0; j < 8; j++){
            board.push_back(row);
        }

}


reversi::reversi(int turn_){ 

    reversi();
    turn = turn_;

}

void reversi::print(){

    for (int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){

            if (board[i][j] == 0){
                cout << " ";
            } else if  (board[i][j] == 1){
                cout << "B";
            } else if  (board[i][j] == 2){
                cout << "W";
            }

            if( j < 7){
                cout << "|";
            }

        }

        cout << endl;

        if(i < 7){
            cout <<"---------------" <<endl;
        }
    }
}

 std::vector<std::pair<int,int>> reversi::possible_moves(){
    std::vector<std::pair<int,int>> moves;

    return moves;
 }


 void reversi::make_move (int move_y,int move_x){
    assert(valid_move(move_y,move_x));

    if (turn == 1){
        board[move_y][move_x] = 1;
        turn = 2;
    }else {
        board[move_y][move_x] = 2;
        turn = 1;
    }

    //udate board

 }

 bool reversi::valid_move(int move_y,int move_x){

     return true;
 }