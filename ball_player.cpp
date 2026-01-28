#include "ball_player.hpp"

/*
This is the ball
*/
// 0 surronding, 1 is ball

int ball[ballProp::rows][ballProp::cols] = {
    0, 0, 1, 1, 1, 1, 1, 0, 0, 
    0, 1, 1, 1, 1, 1, 1, 1, 0, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 
    0, 1, 1, 1, 1, 1, 1, 1, 0, 
    0, 0, 1, 1, 1, 1, 1, 0, 0, 
};

void move_right(){
    ballPos::col += 1;
}

void move_left(){
    if(ballPos::col > 0){
        ballPos::col -= 1;
    }
}

void move_up(){
    if(ballPos::row > 0){
        ballPos::row -= 1;
    }
}

void move_down(){
    ballPos::row += 1;
}