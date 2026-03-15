#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include "ball.hpp"
#include "game_graphics.hpp"
#include "../Rendering/Renderer.hpp"

auto nextKeyTime = std::chrono::steady_clock::now();


int main(){
    readyTerminal();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    Renderer renderer;
    
    //Graphics run
    while(renderer.windowOpen()){
        renderer.render();
        draw();     
        simulateMovement();
        
        //game loop (synced with key presses)
        if(std::chrono::steady_clock::now() >= nextKeyTime){
            if(_kbhit()) {           // check if a key was pressed
                char key = _getch();  
                if (key == 'q') break;  
                else if(!signals::grappled){
                    if(key == 'd') chargeForce(direction::right);
                    else if (key == 'a') chargeForce(direction::left);
                    else if (key == 'w') move_up();
                    else if (key == 's') chargeForce(direction::compress);
                    else if (key == 'e') executeForce();
                    else if (key == 'f') launch_grapple(direction::right); 
                    else if (key == 'r') {ballPos::row = 1 ; ballPos::col = 1; energyBar::internal = 100;}
                }
                else if(signals::grappled){
                    if(key == 'f') signals::grappled = false; 
                    else if (key == 'r') {ballPos::row = 1 ; ballPos::col = 1;}
                }
                
                nextKeyTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(200);
            }
            else {           
                if (renderer.readKey(GLFW_KEY_Q)) break;  
                else if(!signals::grappled){
                    if(renderer.readKey(GLFW_KEY_D)) chargeForce(direction::right);
                    else if (renderer.readKey(GLFW_KEY_A)) chargeForce(direction::left);
                    else if (renderer.readKey(GLFW_KEY_W)) move_up();
                    else if (renderer.readKey(GLFW_KEY_S)) chargeForce(direction::compress);
                    else if (renderer.readKey(GLFW_KEY_E)) executeForce();
                    else if (renderer.readKey(GLFW_KEY_F)) launch_grapple(direction::right); 
                    else if (renderer.readKey(GLFW_KEY_R)) {ballPos::row = 1 ; ballPos::col = 1; energyBar::internal = 100;}
                }
                else if(signals::grappled){
                    if(renderer.readKey(GLFW_KEY_F)) signals::grappled = false; 
                    else if (renderer.readKey(GLFW_KEY_R)) {ballPos::row = 1 ; ballPos::col = 1;}
                }
                
            }
        }
    }
}

