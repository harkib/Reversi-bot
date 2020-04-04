//reversi.h
#include <vector>
#include <iostream>
#include <assert.h> 
#include <utility>
using namespace std;

#pragma once 

class reversi {

    //0 = empty, 1 = player 1 (B), 2 = player 2 (W)
    //top left is 0,0  bottom right is 7,7
    std::vector<std::vector<int>> board;
    int turn = 1; //1 or 2

    public:

    reversi();
    //reversi(int);
    
    std::vector<std::pair<int,int>> possible_moves();
    bool game_done();
    void make_move (int,int);
    void skip_turn();
    void print();
    int winner();
    double get_heuristic();
    double get_h_bound();
    private:

    bool valid_move(int,int);
    std::vector<std::pair<std::pair<int,int>,int>> find_rows(int,int);
    double h_parity();
    double h_mobilty();
    double h_corners();
    double h_stabilty();

};

reversi::reversi(){ 

    //8 x 8 matrix of zeros
    vector<int> row = {0,0,0,0,0,0,0,0};
    for(int j = 0; j < 8; j++){
        board.push_back(row);
    }

    //standard starting board
    board[3][3] = 1;
    board[4][4] = 1;
    board[3][4] = 2;
    board[4][3] = 2;

}


// reversi::reversi(int turn_){ 

//     reversi();
//     turn = turn_;

// }

//pritns board
void reversi::print(){

    for (int i = 0; i < 8; i++){
        cout << i << ":";
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
            cout <<"-----------------" <<endl;
        }
    }

    cout << "  ";
    for (int i = 0; i < 8; i++){
        cout << i << " ";
    } cout << endl << endl;
}

//returns vector of possible moves, (y,x)
 std::vector<std::pair<int,int>> reversi::possible_moves(){
    std::vector<std::pair<int,int>> moves;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if(valid_move(i,j)){
                moves.push_back(make_pair(i,j));
            }
        }
    }

    return moves;
 }

//skips turn
//assert: no moves for current player
 void reversi::skip_turn(){
    std::vector<std::pair<int,int>> moves = possible_moves();
    assert(moves.size()==0);

    turn = turn % 2 + 1;

 }

//makes move and updates board
//assert: valid move
 void reversi::make_move (int move_y,int move_x){
    assert(valid_move(move_y,move_x));
    
    
    //udate board
    std::vector<std::pair<std::pair<int,int>,int>> rows = find_rows(move_y,move_x);
    
    //update self
    board[move_y][move_x] = turn;

    //update row
    int dx, dy;
    for (int i = 0; i < rows.size(); i++){
        dy = rows[i].first.first;
        dx = rows[i].first.second;
        
        for (int j = 0; j < rows[i].second; j++){
            board[move_y+dy][move_x+dx] = turn;
            dy = dy + rows[i].first.first;
            dx = dx + rows[i].first.second;
        }
    }

    //change turn
    turn = turn % 2 + 1;
 }

//checks is move is valid by checking what rows would be captured
 bool reversi::valid_move(int move_y,int move_x){
    vector<pair<pair<int,int>,int>> rows = find_rows(move_y,move_x);

    return rows.size() > 0;
 }

//retunrs rows that would be captured with move
//inner pair repersent direction of row outer pair.second is length of row
vector<pair<pair<int,int>,int>> reversi::find_rows(int move_y,int move_x){
    vector<pair<pair<int,int>,int>> rows;

    //check if empty 
    if(board[move_y][move_x] != 0){
        return rows;
    }

    int len, dy, dx;
    bool in_bounds;
    for (int x = -1; x <= 1; x++){
        for (int y = -1; y <= 1; y++){
            if ((x || y) && !(move_y+y > 7 || move_y+y < 0 || move_x+x > 7 || move_x+x < 0)){
                len = 0; 
                dy = y;
                dx = x;
                in_bounds = true;
                while(board[move_y+dy][move_x+dx]== (turn%2 + 1)){
                    len++;
                    dy = dy + y;
                    dx = dx + x;

                    //out of bounds, and has not found same color 
                    if (move_y+dy > 7 || move_y+dy < 0 || move_x+dx > 7 || move_x+dx < 0){
                        in_bounds = false;
                        dx = 0;
                        dy = 0;
                        break;
                    }
                }
                if (board[move_y+dy][move_x+dx]== turn && in_bounds && len != 0){
                    rows.push_back(make_pair(make_pair(y,x), len ));
                }
            }
        }
    }

    return rows;
}


//checks if there is no moves for current player and next player
bool reversi::game_done(){
     std::vector<std::pair<int,int>> moves = possible_moves();
    
     if(moves.size() == 0){
         turn = turn % 2 + 1;
         moves = possible_moves();
         if(moves.size() == 0){
             return true;
         }
         turn = turn % 2 + 1;
     }

     return false; 
}

//checks who won
//assert: game is done
int reversi::winner(){
    assert(game_done());
    int one = 0;
    int two = 0;

    for (int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(board[i][j]==1){
                one ++;
            }else if (board[i][j]==2){
                two ++;
            }
        }
    }

    if (one > two){
        return 1;
    } else if (two > one){
        return 2;
    } else {
        return 0;
    }
}


//returns heuristic value
//1 = player 1 (B) = Maximizing, 2 = player 2 (W) = minimixing
double reversi::get_heuristic(){
//https://kartikkukreja.wordpress.com/2013/03/30/heuristic-function-for-reversiothello/
    int p = 1;
    int c = 10;
    return (p*h_parity() + c*h_corners())/(c+p);
    
}


double reversi::h_parity(){
    
    //count coins
    int one = 0;
    int two = 0;
    for (int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(board[i][j]==1){
                one ++;
            }else if (board[i][j]==2){
                two ++;
            }
        }
    }
    
    return 100*(one - two)/(one + two);
  
}

double reversi::h_mobilty(){return 0;}


double reversi::h_corners(){
    //https://guides.net4tv.com/games/how-win-reversi
    int one = 0;
    int two = 0;
    int true_corner_val = 10;
    vector <int> cord_x = {0,2,5,7};
    vector <int> cord_y = {0,2,5,7};

    for(int i = 0; i < cord_x.size(); i++){
        for(int j = 0; j < cord_x.size(); j++){
            if(board[cord_y[j]][cord_x[i]] == 1){
                if((cord_y[j] == 7 || cord_y[j] == 0) && (cord_x[i] == 7 || cord_x[i] == 0)){
                    one += true_corner_val;
                } else {
                    one ++;
                }
            }else if (board[cord_y[j]][cord_x[i]]==2){
                if((cord_y[j] == 7 || cord_y[j] == 0) && (cord_x[i] == 7 || cord_x[i] == 0)){
                    two += true_corner_val;
                } else {
                    two ++;
                }
            }
        }    
    }
    if (one == 0 && two == 0){
        return 0;
    } else {
        return 100*(one - two)/(one + two);
    }
}

double reversi::h_stabilty(){return 0;}

//returns upper bound of heuristic, lower bound = - upper bound
double reversi::get_h_bound(){
    return 100;
}