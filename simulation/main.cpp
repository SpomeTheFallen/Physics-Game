#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include "ball.hpp"
#include "../Rendering/Renderer.hpp"
#include "level_grids.hpp"

auto nextKeyTime = std::chrono::steady_clock::now();
bool keyPress = false;

void homeScreenLoop(Renderer &renderer){
    renderer.render();
    if(nextKeyTime <= std::chrono::steady_clock::now()){
        keyPress = true; 
        //TRANSITION STATE   
        if(homescreen::signals::B1selected && renderer.readKey(GLFW_KEY_ENTER)){
            renderer.setRender(RenderType::Level0);
        }

        //Non-transition key checking
        else if(renderer.readKey(GLFW_KEY_S))
            homescreen::signals::B1selected = true;
        else if(renderer.readKey(GLFW_KEY_W))
            homescreen::signals::B1selected = false;
        else
            keyPress = false;
        if(keyPress)
            nextKeyTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(100);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(45));
}

void basicGameLoop(Renderer &renderer){
    renderer.render();    
    simulateMovement();
        
    if(nextKeyTime <= std::chrono::steady_clock::now()){
        keyPress = true; 
        //TRANSITION STATE
        if(renderer.readKey(GLFW_KEY_ESCAPE)){
            renderer.setRender(RenderType::HomeScreen);
        }

        //Non-transition key checking
        else if(!signals::grappled){
            if(renderer.readKey(GLFW_KEY_D)) chargeForce(direction::right);
            else if (renderer.readKey(GLFW_KEY_A)) chargeForce(direction::left);
            else if (renderer.readKey(GLFW_KEY_W)) chargeForce(direction::uncompress);
            else if (renderer.readKey(GLFW_KEY_S)) chargeForce(direction::compress);
            else if (renderer.readKey(GLFW_KEY_E)) executeForce();
            else if (renderer.readKey(GLFW_KEY_G)) launch_grapple(direction::right); 
            else if (renderer.readKey(GLFW_KEY_R)) {ballPos::row = 1 ; ballPos::col = 1; energyBar::internal = 200;}
            else if (renderer.readKey(GLFW_KEY_V)) {energyBar::internal = 200;}
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

int main(){
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    Renderer renderer;
    renderer.setRender(RenderType::HomeScreen);
    //Graphics run
    while(renderer.windowOpen()){
        if (renderer.readKey(GLFW_KEY_Q)) 
            break;  
        else if(renderer.currentRender() == RenderType::HomeScreen)
            homeScreenLoop(renderer);
        else if(renderer.currentRender() == RenderType::Level0)    
            basicGameLoop(renderer);
        
    }
    
}


