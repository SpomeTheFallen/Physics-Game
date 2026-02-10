#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include "ball.hpp"
#include "game_graphics.hpp"

auto nextKeyTime = std::chrono::steady_clock::now();


int main(){
    readyTerminal();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    
    //Graphics run
    while(true){
        draw();     
        simulateMovement(1);
        //game loop (synced with key presses)
        if(std::chrono::steady_clock::now() >= nextKeyTime){
            if(_kbhit()) {           // check if a key was pressed
                char key = _getch();    
                if(key == 'd') chargeForce(direction::right);
                else if (key == 'a') chargeForce(direction::left);
                else if (key == 'w') move_up();
                else if (key == 's') continue;
                else if (key == 'e') executeForce();
                else if (key == 'r') {ballPos::row = 1 ; ballPos::col = 1;}
                else if (key == 'q') break;  
                nextKeyTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(200);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

