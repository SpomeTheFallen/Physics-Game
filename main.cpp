#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include "ball_player.hpp"
#include "level_grids.hpp"

void clear(){
    system("cls"); 
}
/*
In draw we imagine the terminal as a grid
It updates line by line, or every col for each row
We set a barrier to bound the level, so we must shift the level's array by 1 in each index.
To draw the ball, we check if the index we are on is its posistion relative to the terminal grid, then fill the line
It is important to note, cursor posistion affects print and not the index, so they must be set to always match.
*/
void draw(){
    std::cout << "\x1b[H";
    
    for(int i = 0; i < l0Prop::rows+1 ; i++){
        for(int j = 0 ; j < l0Prop::cols+1; j++){
            //auto barrier
            if(i == 0 || i == l0Prop::rows ){
                std::cout << 'x';
                continue;
            }
            if(j == 0 || j == l0Prop::cols){
                std::cout << 'x';
                continue;
            }
            //level design
            if(level0[i-1][j-1] == 1){
                std::cout << 'x';
            }
            //ball posistion
            else if((ballPos::col <= j && j < ballPos::col + ballProp::cols) && (ballPos::row <= i && i < ballPos::row + ballProp::rows) ){
                
                if(ball[i-ballPos::row][j-ballPos::col] == 1){
                    std::cout << 'H';
                }
                else{
                    std::cout << ' ';
                }
                
            }
            else{
                std::cout << ' ';
            }
        }
        std::cout << '\n';
    }
}

int main(){
    std::cout << "\x1b[?25l"; //hide cursor
    //std::cout << "\x1b[8;40;100t"; //set size rows ; col

    //Graphics run
    while(true){
        draw();            
        if (_kbhit()) {           // check if a key was pressed
            char key = _getch();    // read the key without Enter
            if(key == 'd') move_right();
            else if (key == 'a') move_left();
            else if (key == 'w') move_up();
            else if (key == 's') move_down();
            else if (key == 'q') break;  // quit
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

