#include "ball_player.hpp"
#include "level_grids.hpp"

/*
This is the ball
*/
// 0 = air, 1 = ball, 2 = ball (texture)

int ball[ballProp::rows][ballProp::cols] = {
    0, 0, 1, 1, 2, 1, 1, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 1, 0, 
    1, 0, 0, 0, 0, 0, 0, 0, 2, 
    1, 0, 0, 0, 0, 0, 0, 0, 2, 
    1, 0, 0, 0, 0, 0, 0, 0, 1, 
    0, 2, 0, 0, 0, 0, 0, 1, 0, 
    0, 0, 2, 2, 1, 1, 1, 0, 0, 
};

void move_right(){
    if((ballPos::col + ballProp::cols) < (l0Prop::cols + 1)){
        ballPos::col += 1;
    }
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