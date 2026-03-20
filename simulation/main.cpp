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
        if((homescreen::row == 0 && homescreen::col == 0) && renderer.readKey(GLFW_KEY_ENTER)){
            levels::setLevel(levelType::level0);
            renderer.setRender(RenderType::currentLevel);
        }
        else if((homescreen::row == 0 && homescreen::col == 1) && renderer.readKey(GLFW_KEY_ENTER)){
            levels::setLevel(levelType::level1);
            renderer.setRender(RenderType::currentLevel);
        }
        else if((homescreen::row == 1 && homescreen::col == 0) && renderer.readKey(GLFW_KEY_ENTER)){
            levels::setLevel(levelType::level2);
            renderer.setRender(RenderType::currentLevel);
        }

        //Non-transition key checking
        else if(renderer.readKey(GLFW_KEY_S))
            homescreen::row = 1;
        else if(renderer.readKey(GLFW_KEY_W))
            homescreen::row = 0;
        else if(renderer.readKey(GLFW_KEY_D))
            homescreen::col = 1;
        else if(renderer.readKey(GLFW_KEY_A))
            homescreen::col = 0;
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
    if(levels::levelCompleted()){
        renderer.setRender(RenderType::HomeScreen);
    }
        
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
            else if (renderer.readKey(GLFW_KEY_G)) launch_grapple(); 
            else if (renderer.readKey(GLFW_KEY_R)) {ballPos::row = 1 ; ballPos::col = 1; energyBar::internal = 200;}
            else if (renderer.readKey(GLFW_KEY_V)) {energyBar::internal = 200;}
            else{
                keyPress = false;
            }
        }
        else if(signals::grappled){
            if(renderer.readKey(GLFW_KEY_G)) signals::grappled = false; 
            else if(renderer.readKey(GLFW_KEY_D)) chargeForce(direction::right);
            else if (renderer.readKey(GLFW_KEY_A)) chargeForce(direction::left);
            else if (renderer.readKey(GLFW_KEY_W)) chargeForce(direction::uncompress);
            else if (renderer.readKey(GLFW_KEY_S)) chargeForce(direction::compress);
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
        else if(renderer.currentRender() == RenderType::currentLevel)    
            basicGameLoop(renderer);
        
    }
    
}


