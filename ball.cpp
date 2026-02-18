#include "ball.hpp"
#include "level_grids.hpp"
#include <math.h>

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

//energy
int energyBar::internal = 100;

int energyBar::bar[10] = {1,1,1,1,1,1,1,1,1,1};

void transferEnergy(int velocityChange){
    int energyChange;
    energyChange =  (velocityChange*velocityChange)/2;
    energyBar::internal -= energyChange;
    
    for(int i = 0 ; i < 100 ; i+=10){
        if(energyBar::internal < (90 - i)){
            energyBar::bar[(i/10)] = 0;
        }
    }

}
//force ; F = a (since m = 1)
int forceBar::force = 0;
int forceBar::xForce = 0;
int forceBar::yForce = 0;
int forceBar::bar[10] = {0,0,0,0,0,0,0,0,0,0};


void chargeForce(direction dir){
    int temp_xForce = 0;
    int temp_yForce = 0;
    switch (dir){
        case direction::left: 
            temp_xForce = forceBar::xForce + -1; 
            break;
        case direction::right: 
            temp_xForce = forceBar::xForce + 1;
            break;
    }
    
    int temp_force = sqrt(temp_xForce*temp_xForce + temp_yForce*temp_yForce);
    if(temp_force <= 10){
        forceBar::force = temp_force;
        forceBar::xForce = temp_xForce;
        forceBar::yForce = temp_yForce;
    }

    for(int i = 0 ; i < 10 ; i++){
        if(forceBar::force > (9 - i)){
            forceBar::bar[i] = 1;
        }
        else{
            forceBar::bar[i] = 0;
        }
    }
}
//we will assume the force gets executed in 1 millisecond, so the resultant change in velocity = acceleration = force.
void executeForce(){
    forceBar::force = 0;
    for(int i = 0 ; i < 10 ; i++){
        forceBar::bar[i] = 0;
    }
    ballProp::velocityX += forceBar::xForce; 
    transferEnergy(forceBar::xForce);
    forceBar::xForce = 0;
}


//move signals
bool signals::rolling_right1 = false;
bool signals::rolling_right2 = false;
bool signals::rolling_left1 = false;
bool signals::rolling_left2 = false;
int signals::rolling_counter = 0;
//velocities
int ballProp::velocityX = 0;
int ballProp::velocityY = 0;
int ballProp::accelerationX = 0;
int ballProp::accelerationY = 0;

/*movement
For collision checks, ballPos = pos relative to terminal
Level array is shifted to the left and up compared to terminal
1 unit = 1 meter
Acceleration is in unit/millisecond^2;
Velocity is in unit/millisecond; 
*/

bool checkRightCollisions(){
    if(!((ballPos::col + ballProp::cols + 1) < (l0Prop::cols))){
        return false;
    }
    for(int i = ballPos::row; i < (ballPos::row + ballProp::rows-1) ; i++){
        for(int j = ballPos::col-1 ; j < ballPos::col-1 + ballProp::cols + 2 ; j++){
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
        for(int j = ballPos::col-1-2 ; j < ballPos::col ; j++){
            if(level0[i][j] == 1){
                return false;
            }
        }
    }
    return true;
}
bool checkGravityCollisions(int velocity){
    if(!((ballPos::row + ballProp::rows-1 + velocity ) < (l0Prop::rows+1))){
        return false;
    }
    for(int i = ballPos::row; i < ballPos::row + ballProp::rows + velocity-1; i++){
        for(int j = ballPos::col - 1 ; j < ballPos::col + ballProp::cols - 1 ; j++){
            if(level0[i][j] == 1){
                return false;
            }
        }
    }
    return true;
}


void move_up(){
    if(ballPos::row > 1){
        ballPos::row -= 1;
    }
}
//1 unit = 5 m ; g = 10m/s^2 ; 
void simulateGravity(){
    if(checkGravityCollisions(ballProp::velocityY)){
        ballPos::row += ballProp::velocityY;
        ballProp::velocityY += 2;
    }
    else{
        //Lower unit movement to account for any leftover space between ball and ground.
        ballProp::velocityY > 0 ? ballProp::velocityY -= 1 : ballProp::velocityY = 0;
    }
}
//time in milliseconds
void simulateHorizontalMovement(int ellapsedTime){
    
    ballProp::velocityX += ballProp::accelerationX * ellapsedTime;
    transferEnergy(ballProp::accelerationX * ellapsedTime);

    //friction
    if(ballProp::velocityX > 0){
        ballProp::accelerationX = -1;
    }
    else if (ballProp::velocityX < 0){
        ballProp::accelerationX = 1;
    }
    else if (ballProp::velocityX == 0){
        ballProp::accelerationX = 0;
    }
    
    if(ballProp::velocityX > 0){
        if(checkRightCollisions()){        
            ballPos::col += ballProp::velocityX * ellapsedTime;
            signals::rolling_right1 = true;
        }
    }   
    else if(ballProp::velocityX < 0){
        if(checkLeftCollisions()){        
            ballPos::col += ballProp::velocityX * ellapsedTime;
            signals::rolling_left1 = true;
        }
    }
};

void simulateMovement(int ellapsedTime){
    simulateGravity();
    simulateHorizontalMovement(ellapsedTime);
}