#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <climits>
#include <iomanip>

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

std::pair<int,std::pair<int, int>> setBoardScore(int player, int enemy, bool maximize);

std::pair<int,std::pair<int, int>> minimax(int depth, int player, int enemy, bool maximize){
    if(depth == 0){
        std::pair<int,std::pair<int, int>> score = setBoardScore(player, enemy, !maximize);
        return score;
    }
    if(maximize == true){
        int maxval = INT_MIN;
        std::pair<int, std::pair<int,int>> maxpos;
        for(int i=0; i<SIZE; i++){
            for(int j=0; j<SIZE; j++){
                if(board[i][j] == EMPTY){
                    board[i][j] = player;
                    std::pair<int, std::pair<int,int>> temp;
                    temp = minimax(depth-1, player, enemy, false);
                    if(temp.first > maxval){
                        maxval = temp.first;
                        maxpos = std::make_pair(maxval, std::make_pair(i,j));
                        //std::cout <<"Place: " << i << " " << j << " " << maxval << "\n";
                    }
                    board[i][j] = EMPTY;
                    //std::cout << "leaving maximizing\n";
                }
            }
        }
        return maxpos;
    }else{
        int minval = INT_MAX;
        std::pair<int, std::pair<int,int>> minpos;
        //std::cout <<"Start minimizing\n";
        for(int i=0; i<SIZE; i++){
            for(int j=0; j<SIZE; j++){
                if(board[i][j] == EMPTY){
                    board[i][j] = enemy;
                    std::pair<int, std::pair<int,int>> temp;
                    temp = minimax(depth-1, player, enemy, true);
                    //std::cout << "temp.first is " << temp.first << "\n";
                    // if(minval != INT_MAX && temp.first > minval){
                    //     minval = temp.first;
                    //     minpos = std::make_pair(minval, std::make_pair(i,j));
                    //     std::cout <<"Place: " << i << " " << j << " " << minval << "\n";
                    // }
                    if(temp.first < minval){
                        minval = temp.first;
                        minpos = std::make_pair(minval, std::make_pair(i,j));
                        //std::cout <<"Place: " << i << " " << j << " " << minval << "\n";
                    }
                    board[i][j] = EMPTY;
                    //std::cout << "leaving minimizing\n";
                }
            }
        }
        return minpos;
    }
}
std::pair<int, std::pair<int,int>> setBoardScore(int player, int enemy, bool maximize){
    // we want to check the score for every spot on the board
    // for each position, check for pieces which are connected in a row,
    // with the directions of going up, down, left, right, and diagonally
    // std::ios_base::sync_with_stdio(false);
    // std::cin.tie(NULL);
    int boardscore[SIZE][SIZE];
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            boardscore[i][j] = 0;
        }
    }

    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            if(board[i][j] == EMPTY){
                std::vector<int> player_data;
                std::vector<int> enemy_data;
                int up = 0;
                for(int k = i-1; k >= 0; k--){
                    if(board[k][j] == player){
                        up++;
                    }
                    else{
                        break;
                    }
                }
                player_data.push_back(up);
                int enemyup = 0;
                for(int k = i-1; k >= 0; k--){
                    if(board[k][j] == enemy){
                        enemyup++;
                    }
                    else{
                        break;
                    }
                }
                enemy_data.push_back(enemyup);

                //check down
                int down = 0;
                for(int k = i+1; k < SIZE; k++){
                    if(board[k][j] == player){
                        down++;
                    }
                    else{
                        break;
                    }
                }
                player_data.push_back(down);
                //block enemies
                //down
                int enemydown = 0;
                for(int k = i+1; k < SIZE; k++){
                    if(board[k][j] == enemy){
                        enemydown++;
                    }
                    else{
                        break;
                    }
                }
                enemy_data.push_back(enemydown);

                //check left
                int left = 0;
                for(int k = j-1; k >= 0; k--){
                    if(board[i][k] == 1){
                        left++;
                    }
                    else{
                        break;
                    }
                }
                player_data.push_back(left);

                //block enemies
                //left
                int enemyleft = 0;
                for(int k = j-1; k >= 0; k--){
                    if(board[i][k] == enemy){
                        enemyleft++;
                    }
                    else{
                        break;
                    }
                }
                enemy_data.push_back(enemyleft);
                //check right
                int right = 0;
                for(int k = j+1; k < SIZE; k++){
                    if(board[i][k] == player){
                        right++;
                    }
                    else{
                        break;
                    }
                }
                player_data.push_back(right);
                //block enemies
                //right
                int enemyright = 0;
                for(int k = j+1; k < SIZE; k++){
                    if(board[i][k] == enemy){
                        enemyright++;
                    }
                    else{
                        break;
                    }
                }
                enemy_data.push_back(enemyright);
                int up_left = 0;
                for(int k = i-1, l = j-1; k >= 0 && l >= 0; k--, l--){
                    if(board[k][l] == player){
                        up_left++;
                    }
                    else{
                        break;
                    }
                }
                player_data.push_back(up_left);
                //block enemies
                //up_left
                int enemyup_left = 0;
                for(int k = i-1, l = j-1; k >= 0 && l >= 0; k--, l--){
                    if(board[k][l] == enemy){
                        enemyup_left++;
                    }
                    else{
                        break;
                    }
                }
                enemy_data.push_back(enemyup_left);
                //check up-right
                int up_right = 0;
                for(int k = i-1, l = j+1; k >= 0 && l < SIZE; k--, l++){
                    if(board[k][l] == player){
                        up_right++;
                    }
                    else{
                        break;
                    }
                }
                player_data.push_back(up_right);
                //block enemies
                //up_right
                int enemyup_right = 0;
                for(int k = i-1, l = j+1; k >= 0 && l < SIZE; k--, l++){
                    if(board[k][l] == enemy){
                        enemyup_right++;
                    }
                    else{
                        break;
                    }
                }
                enemy_data.push_back(enemyup_right);

                //check down-left
                int down_left = 0;
                for(int k = i+1, l = j-1; k < SIZE && l >= 0; k++, l--){
                    if(board[k][l] == player){
                        down_left++;
                    }
                    else{
                        break;
                    }
                }
                player_data.push_back(down_left);
                //block enemies
                //down_left
                int enemydown_left = 0;
                for(int k = i+1, l = j-1; k < SIZE && l >= 0; k++, l--){
                    if(board[k][l] == enemy){
                        enemydown_left++;
                    }
                    else{
                        break;
                    }
                }
                enemy_data.push_back(enemydown_left);

                //check down-right
                int down_right = 0;
                for(int k = i+1, l = j+1; k < SIZE && l < SIZE; k++, l++){
                    if(board[k][l] == player){
                        down_right++;
                    }
                    else{
                        break;
                    }
                }
                player_data.push_back(down_right);
                //block enemies
                //down_right
                int enemydown_right = 0;
                for(int k = i+1, l = j+1; k < SIZE && l < SIZE; k++, l++){
                    if(board[k][l] == enemy){
                        enemydown_right++;
                    }
                    else{
                        break;
                    }
                }
                enemy_data.push_back(enemydown_right);
                //std::cout <<"Player data: ";
                int size = player_data.size();
                for(int k = 0; k < size; k++){
                    //std::cout << player_data[k] << " ";
                    if(player_data[k] == 5){
                        boardscore[i][j] += 3000;//win
                    }
                    else if(player_data[k] == 4){
                        boardscore[i][j] += 120;
                    }
                    else if(player_data[k] == 3){
                        boardscore[i][j] += 80;
                    }
                    else if(player_data[k] == 2){
                        boardscore[i][j] += 5;
                    }
                    // else if(player_data[k] == 1){
                    //     boardscore[i][j] += 2;
                    // }
                }
                //std::cout <<"\nEnemy data: ";
                size = enemy_data.size();
                for(int k = 0; k < size; k++){
                    //std::cout << enemy_data[k] << " ";
                    if(enemy_data[k] == 5){
                        boardscore[i][j] -= 3000; //lose
                    }
                    else if(enemy_data[k] == 4){
                        boardscore[i][j] -= 120;
                    }
                    else if(enemy_data[k] == 3){
                        boardscore[i][j] -= 80;
                    }
                    else if(enemy_data[k] == 2){
                        boardscore[i][j] -= 5;
                    }
                    // else if(enemy_data[k] == 1){
                    //     boardscore[i][j] -= 2;
                    // }
                }
                // std::cout <<"\n";
                // std::cout <<"Current position: " << i << " " << j << "\n";
                // std::cout<<"board score"<<"\n";
                // for(int i = 0; i < SIZE; i++){
                //     for(int j = 0; j < SIZE; j++){
                //         std::cout<< std::setw(3)<<boardscore[i][j]<<" ";
                //     }
                //     std::cout<<"\n";
                // }
            }
        }
    }
    // std::cout<<"board score"<<"\n";
    // for(int i = 0; i < SIZE; i++){
    //     for(int j = 0; j < SIZE; j++){
    //         std::cout<< std::setw(3)<<boardscore[i][j]<<" ";
    //     }
    //     std::cout<<"\n";
    // }
    if(maximize){
        int maxval = INT_MIN;
        std::pair<int, std::pair<int,int>> maxpos;
        for(int i=0; i<SIZE; i++){
            for(int j=0; j<SIZE; j++){
                if(boardscore[i][j] > maxval && board[i][j] == EMPTY){
                    maxval = boardscore[i][j];
                    maxpos = std::make_pair(maxval, std::make_pair(i,j));
                }
            }
        }
        return maxpos;
    }else{
        int minval = INT_MAX;
        std::pair<int, std::pair<int,int>> minpos;
        for(int i=0; i<SIZE; i++){
            for(int j=0; j<SIZE; j++){
                if(boardscore[i][j] < minval && board[i][j] == EMPTY){
                    minval = boardscore[i][j];
                    minpos = std::make_pair(minval, std::make_pair(i,j));
                }
            }
        }
        return minpos;
    }
}

void write_valid_spot(std::ofstream& fout) {

    int x;
    int y;

    int enemy;
    if(player == BLACK)
        enemy = WHITE;
    else
        enemy = BLACK;

    std::pair<int, std::pair<int,int>> best_move;
    // if(player == BLACK)
    //     best_move = setBoardScore(player, enemy, true);
    // else if(player == WHITE)
    //     best_move = setBoardScore(enemy, player, true);
    // x = best_move.second.first;
    // y = best_move.second.second;
    best_move = minimax(1, player, enemy, false);
    x = best_move.second.first;
    y = best_move.second.second;
    // int best_score = INT_MIN;
    // for(int i = 0; i < SIZE; i++){
    //     for(int j = 0; j < SIZE; j++){
    //         if(board[i][j] == EMPTY){
    //             board[i][j] = player;
    //             best_move = minimax(1, player, enemy, true);
    //             board[i][j] = EMPTY;
    //             if(best_move.first > best_score){
    //                 best_score = best_move.first;
    //                 x = i;
    //                 y = j;
    //             }
    //         }
    //     }
    // }

    bool flag = true;
    while(flag){
        if(board[x][y] == EMPTY){
            fout << x << " " << y << std::endl;
            fout.flush();
            flag = false;
        }
        else{
            x++;
            y++;
        }
    }
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
