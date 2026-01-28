#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include "ball_player.hpp"
#include "level_grids.hpp"

void clear(){
    system("cls"); 
}

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
            if(level0[i][j] == 1){
                std::cout << 'x';
            }
            //ball posistion
            else if(ballPos::col == j){
                for(int x = 0 ; x < ballProp::cols ; x++){
                    if(ball[i][x] == 1){
                        std::cout << 'H';
                    }
                    else{
                        std::cout << ' ';
                    }
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
    std::cout << "\x1b[?25l";

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

