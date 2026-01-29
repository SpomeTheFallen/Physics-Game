#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include "ball_player.hpp"
#include "game_graphics.hpp"

auto nextKeyTime = std::chrono::steady_clock::now();


int main(){
    readyTerminal();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    
    //Graphics run
    while(true){
        draw();      
        
        if(std::chrono::steady_clock::now() >= nextKeyTime){
            if (_kbhit()) {           // check if a key was pressed
                char key = _getch();    // read the key without Enter
                if(key == 'd') move_right();
                else if (key == 'a') move_left();
                else if (key == 'w') move_up();
                else if (key == 's') move_down();
                else if (key == 'q') break;  // quit
                nextKeyTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(200);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

