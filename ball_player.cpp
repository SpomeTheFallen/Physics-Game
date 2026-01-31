#include "ball_player.hpp"
#include "level_grids.hpp"

// 0 = air, 1 = ball, 2 = ball (texture)

int ball[ballProp::rows][ballProp::cols] = {
    0, 1, 2, 0,
    1, 0, 0, 1,
    1, 0, 0, 1,
    0, 2, 1, 0,
};

int ballR1[ballProp::rows][ballProp::cols] = {
    0, 1, 1, 0,
    1, 0, 0, 2,
    2, 0, 0, 1,
    0, 1, 1, 0,
};

int ballR2[ballProp::rows][ballProp::cols] = {
    0, 2, 1, 0,
    1, 0, 0, 1,
    1, 0, 0, 2,
    0, 1, 1, 0,
};
//move signals
bool signals::rolling_right1 = false;
bool signals::rolling_right2 = false;
bool signals::rolling_left1 = false;
bool signals::rolling_left2 = false;
int signals::rolling_counter = 0;
//velocities
int ballProp::velocityZ = 0;

/*movement
For collision checks, ballPos = pos relative to terminal
Level array is shifted to the left and up compared to terminal
*/

bool checkRightCollisions(){
    if(!((ballPos::col + ballProp::cols + 1) < (l0Prop::cols))){
        return false;
    }
    for(int i = ballPos::row; i < (ballPos::row + ballProp::rows-1) ; i++){
        for(int j = ballPos::col ; j < ballPos::col + ballProp::cols + 1 ; j++){
            if(level0[i][j] == 1){
                return false;
            }
        }
    }
    return true;
}
bool checkLeftCollisions(){
    if(!(ballPos::col > 2)){
        return false;
    }
    for(int i = ballPos::row; i < (ballPos::row + ballProp::rows-1) ; i++){
        for(int j = ballPos::col-2 ; j < ballPos::col ; j++){
            if(level0[i][j] == 1){
                return false;
            }
        }
    }
    return true;
}
bool checkGravityCollisions(int velocity){
    if(!((ballPos::row + ballProp::rows + velocity-1 ) < (l0Prop::rows))){
        return false;
    }
    for(int i = ballPos::row; i < ballPos::row + ballProp::rows + velocity-1; i++){
        for(int j = ballPos::col ; j < ballPos::col + ballProp::cols - 1 ; j++){
            if(level0[i][j] == 1){
                return false;
            }
        }
    }
    return true;
}


void move_right(){
    if(checkRightCollisions()){
        ballPos::col += 2;
        signals::rolling_right1 = true;
    }
}

void move_left(){
    if(checkLeftCollisions()){
        ballPos::col -= 2;
        signals::rolling_left1 = true; 
    }
}

void move_up(){
    if(ballPos::row > 1){
        ballPos::row -= 1;
    }
}
//1 unit = 5 m ; g = 10m/s^2 ; 
void simulateGravity(){
    if(checkGravityCollisions(ballProp::velocityZ)){
        ballPos::row += ballProp::velocityZ;
        ballProp::velocityZ += 2;
    }
    else{
        //Lower unit movement to account for any leftover space between ball and ground.
        ballProp::velocityZ > 0 ? ballProp::velocityZ -= 1 : ballProp::velocityZ = 0;
    }
}