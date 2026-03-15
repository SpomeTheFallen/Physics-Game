#include "Renderer.hpp"
#include "../src/ball.hpp"
#include <iostream>

int main(){
    Renderer renderer;
    int number = 0;
    while(renderer.windowOpen()){
        renderer.render();
        if(ballPos::col < 75) 
            ballPos::col++;
    }
}