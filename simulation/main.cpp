#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include "ball.hpp"
#include "../Rendering/Renderer.hpp"

auto nextKeyTime = std::chrono::steady_clock::now();
bool keyPress = false;

int main(){
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    Renderer renderer;
    
    //Graphics run
    while(renderer.windowOpen()){
        renderer.render();    
        simulateMovement();
        
        //game loop (synced with key presses)     
        if(nextKeyTime <= std::chrono::steady_clock::now()){
            keyPress = true; 
            if (renderer.readKey(GLFW_KEY_Q)) break;  
            else if(!signals::grappled){
                if(renderer.readKey(GLFW_KEY_D)) chargeForce(direction::right);
                else if (renderer.readKey(GLFW_KEY_A)) chargeForce(direction::left);
                else if (renderer.readKey(GLFW_KEY_W)) chargeForce(direction::uncompress);
                else if (renderer.readKey(GLFW_KEY_S)) chargeForce(direction::compress);
                else if (renderer.readKey(GLFW_KEY_E)) executeForce();
                else if (renderer.readKey(GLFW_KEY_G)) launch_grapple(direction::right); 
                else if (renderer.readKey(GLFW_KEY_R)) {ballPos::row = 1 ; ballPos::col = 1; energyBar::internal = 100;}
                else{
                    keyPress = false;
                }
            }
            else if(signals::grappled){
                if(renderer.readKey(GLFW_KEY_G)) signals::grappled = false; 
                else if (renderer.readKey(GLFW_KEY_R)) {ballPos::row = 1 ; ballPos::col = 1;}
                else{
                    keyPress = false;
                }
            }
            if(keyPress)
                nextKeyTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(100);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(45));
    }
        
    
}

