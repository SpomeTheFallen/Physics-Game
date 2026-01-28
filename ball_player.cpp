#include "ball_player.hpp"
#include "level_grids.hpp"


// 0 = air, 1 = ball, 2 = ball (texture)

int ball[ballProp::rows][ballProp::cols] = {
    0, 1, 1, 0,
    1, 0, 0, 1,
    1, 0, 0, 1,
    0, 1, 1, 0,
};

int ballR1[ballProp::rows][ballProp::cols] = {
    0, 1, 1, 0,
    1, 0, 0, 1,
    1, 0, 0, 1,
    0, 1, 1, 1,
};

int ballR2[ballProp::rows][ballProp::cols] = {
    0, 1, 1, 0,
    1, 0, 0, 1,
    1, 0, 0, 1,
    1, 1, 1, 0,
};
//move signals
bool signals::rolling_right1 = false;
bool signals::rolling_right2 = false;
int signals::rolling_counter = 0;

//movement
void move_right(){
    if((ballPos::col + ballProp::cols) < (l0Prop::cols)){
        ballPos::col += 1;
        signals::rolling_right1 = true;
    }
}

void move_left(){
    if(ballPos::col > 1){
        ballPos::col -= 1;
    }
}

void move_up(){
    if(ballPos::row > 1){
        ballPos::row -= 1;
    }
}

void move_down(){
    if((ballPos::row + ballProp::rows) < (l0Prop::rows)){
        ballPos::row += 1;
    }
}