#include "ball.hpp"
#include "level_grids.hpp"
#include <math.h>
#include <iostream>

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
//force 

//2 = compass border, 1 = vector, 0 = space
int forceCompass::forceUnitVector[5][5] = {
    0, 2, 2, 2, 0,
    2, 0, 0, 0, 2,
    2, 0, 1, 0, 2,
    2, 0, 0, 0, 2,
    0, 2, 2, 2, 0,
};

void resetVector(){
    forceCompass::forceUnitVector[1][1] = 0;
    forceCompass::forceUnitVector[1][2] = 0;
    forceCompass::forceUnitVector[1][3] = 0;
    forceCompass::forceUnitVector[2][1] = 0;
    forceCompass::forceUnitVector[2][3] = 0;
    forceCompass::forceUnitVector[3][1] = 0;
    forceCompass::forceUnitVector[3][2] = 0;
    forceCompass::forceUnitVector[3][3] = 0;
}

// F = a (since m = 1)
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
bool checkDownCollisions(int velocity){
    if(!((ballPos::row + ballProp::rows-1 + velocity ) < (l0Prop::rows+1))){
        return false;
    }
    for(int i = ballPos::row; i < ballPos::row + ballProp::rows-1 + velocity; i++){
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


//1 unit = 5 m ; g = 10m/s^2; 
void simulateVerticalMovement(int ellapsedTime){
    ballProp::velocityY += ballProp::accelerationY * ellapsedTime;
    
    // gravity
    if(checkDownCollisions(1)){
        ballProp::accelerationY = 2; 
    }
    else{
        ballProp::velocityY > 0 ? ballProp::accelerationY -= 1 : ballProp::accelerationY = 0;
    }

    if(ballProp::velocityY > 0){
        if(checkDownCollisions(ballProp::velocityY)){       
            ballPos::row += ballProp::velocityY * ellapsedTime;
        }
        else{
            int reboundVelocity = 0;
            while(!checkDownCollisions(ballProp::velocityY)){
                ballProp::velocityY -= 1;
                reboundVelocity += 1;
            }
            ballPos::row += ballProp::velocityY *ellapsedTime;
            //Assume that energy disappation leaves only half velocity remaining
            ballProp::velocityY = -reboundVelocity/2;
        }
    }
    else if(ballProp::velocityY < 0){
        ballPos::row += -1 * ellapsedTime;
        return;
    }
      
}

//time in milliseconds
void simulateHorizontalMovement(int ellapsedTime){
    
    ballProp::velocityX += ballProp::accelerationX * ellapsedTime;

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

int arctan(int y, int x){
    int theta = 0;
    if(x != 0){
        int theta = std::atan(y/x) * (180/(2*M_PI));
    }
   
    if(x < 0){
        theta += 180;
    }
    if(y < 0){
        theta += 360;
    }
    return theta;
}


void simulateMovement(int ellapsedTime){
    //using unit circle, find the direction in rad or degree of force to then map it on a compass
    int theta = arctan(ballProp::accelerationY, ballProp::accelerationX);

    resetVector();
    switch (theta){
        case 0:
            forceCompass::forceUnitVector[2][3] = 1;
            break;
        case 90: 
            forceCompass::forceUnitVector[1][2] = 1;
            break;
        case 180:
            forceCompass::forceUnitVector[2][1] = 1;
            break;
        case 270:
            forceCompass::forceUnitVector[3][2] = 1;
            break;
        default:
            if(theta > 0 && theta < 90)
                forceCompass::forceUnitVector[1][3] = 1;
            else if(theta < 180)
                forceCompass::forceUnitVector[1][1] = 1;
            else if(theta < 270 )
                forceCompass::forceUnitVector[3][1] = 1;
            else if(theta > 270)
                forceCompass::forceUnitVector[3][3] = 1;
    }
    simulateVerticalMovement(ellapsedTime);
    simulateHorizontalMovement(ellapsedTime);
}