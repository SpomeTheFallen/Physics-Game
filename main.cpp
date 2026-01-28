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
    
    for(int i = 0; i < ballProp::rows + ballPos::row; i++){
        if(i < ballPos::row){
            std::cout << '\n';
            continue;
        }
        for(int j = 0 ; j < ballProp::cols + ballPos::col; j++){
            if(j < ballPos::col){
                std::cout << ' ';
            }
            else if(ball[i-ballPos::row][j-ballPos::col] == 1){
                std::cout << 'H'; 
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

