#include "ball.hpp"
#include "level_grids.hpp"
#include <cmath>
#include <iostream>

using std::sqrt; using std::round; using std::atan; using std::cos; using std::sin;

int g = 2;

int arctan(int y, int x){
    int theta = 0;
    if(x != 0){
        theta = round(atan(y/x) * (180.0/(M_PI)));
    }
    
    if(x < 0){
        theta += 180.0;
    }
    if(y < 0 && x == 0){
        theta = 270;
    }
    else if(y > 0 && x == 0){
        theta = 90;
    }
    else if(y < 0){
        theta += 360;
    }

    return theta;
}


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
//---force---

//2 = compass border, 1 = vector, 0 = space

void resetVector(){
    forceCompass::north = false;
    forceCompass::northe = false;
    forceCompass::east = false;
    forceCompass::southe = false;
    forceCompass::south = false;
    forceCompass::southw = false;
    forceCompass::west = false;
    forceCompass::northw = false;
}

// F = a (since m = 1)
int forceBar::force = 0;
int forceBar::xForce = 0;
int forceBar::yForce = 0;
int forceBar::bar[10] = {0,0,0,0,0,0,0,0,0,0};


void chargeForce(direction dir){
    int temp_xForce = forceBar::xForce;
    int temp_yForce = forceBar::yForce;
  
    switch (dir){
        case direction::left: 
            temp_xForce --;
            break;
        case direction::right: 
            temp_xForce ++;
            break;
        case direction::compress:
            //Simplified hookes law, by compressing the ball/spring, restoring force is +=1 per compression.
            temp_yForce ++;
            break;
        case direction::uncompress:
            if(forceBar::yForce > 0)
                temp_yForce --;
            break;
    }

    int temp_force = round(sqrt(temp_xForce*temp_xForce + temp_yForce*temp_yForce));
    if(temp_force <= 10 && temp_force <= energyBar::internal){
        forceBar::force = temp_force;
        forceBar::xForce = temp_xForce;
        forceBar::yForce = temp_yForce;

        if (forceBar::yForce > 3){
            signals::springed2 = true;
            signals::springed1 = false;
        }
        else if(forceBar::yForce > 0){
            signals::springed1 = true;
            signals::springed2 = false;
        }
        else
            signals::springed1 = false;
        
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
    signals::springed1 = false;
    signals::springed2 = false;

    ballProp::velocityX += forceBar::xForce; 
    ballProp::velocityY += forceBar::yForce;
    transferEnergy(forceBar::force);

    forceBar::xForce = 0;
    forceBar::yForce = 0;

    forceBar::force = 0;
    for(int i = 0 ; i < 10 ; i++){
        forceBar::bar[i] = 0;
    }
}


//move signals
bool signals::rolling_right1 = false;
bool signals::rolling_right2 = false;
bool signals::rolling_left1 = false;
bool signals::rolling_left2 = false;
bool signals::springed1 = false;
bool signals::springed2 = false;
bool signals::grappled = false;
int signals::rolling_counter = 0;

//velocities
int ballProp::velocityX = 0;
int ballProp::velocityY = 0;
int ballProp::accelerationX = 0;
int ballProp::accelerationY = 0;

/*---movement---
For collision checks, ballPos = pos relative to terminal
Level array is shifted to the left and up compared to terminal
1 unit = 1 meter
Acceleration is in unit/millisecond^2;
Velocity is in unit/millisecond; 
ballPos::row or ::col is shifted by -1 to account for level array shift
ballProp::rows or ::cols is shifted by -1 to find proper end posistion
*/

//pass the absolute value of velocity for each collision check
bool checkRightCollisions(int velocity){
    if(!((ballPos::col + ballProp::cols-1 + velocity) < (l0Prop::cols+1))){
        return false;
    }
    for(int i = ballPos::row; i < (ballPos::row + ballProp::rows-1) ; i++){
        for(int j = ballPos::col-1 ; j < ballPos::col-1 + ballProp::cols + velocity ; j++){
            if(level0[i][j] == 1){
                return false;
            }
        }
    }
    return true;
}

bool checkLeftCollisions(int velocity){
    if(!(ballPos::col - velocity > 0)){
        return false;
    }
    for(int i = ballPos::row; i < (ballPos::row + ballProp::rows-1) ; i++){
        for(int j = ballPos::col-1-velocity ; j < ballPos::col ; j++){
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

bool checkUpCollisions(int velocity){
    if(!((ballPos::row - velocity ) > 0)){
        return false;
    }
    for(int i = ballPos::row-1 - velocity; i < ballPos::row; i++){
        for(int j = ballPos::col - 1 ; j < ballPos::col + ballProp::cols - 1 ; j++){
            if(level0[i][j] == 1){
                return false;
            }
        }
    }
    return true;
}

//---grapple---

void launch_grapple(direction dir){
    grapple::theta = 0;
    grapple::velocity = ballProp::velocityX;
    
    signals::grappled = true;
    
    ballProp::velocityX = 0;
    ballProp::velocityY = 0;

    int yRadius = 1;
    while (checkUpCollisions(yRadius+1)){
        yRadius ++;   
    }

    grapple::radius = (yRadius/cos(grapple::theta*M_PI/180.0)); 

    float ratio = ((grapple::velocity*grapple::velocity)/(2*g*grapple::radius));

        
    grapple::thetaMax = acos((1-ratio)) * 180.0/M_PI;

    switch (dir){
        case direction::right:
            grapple::thetaChange = grapple::thetaMax/3;
            break;
    }


    grapple::row = ballPos::row - yRadius;
    grapple::col = ballPos::col + ceil(ballProp::cols/2.0f);

}

//All energy conserved in swing
void simulatePendulumMotion(int ellapsedTime){
    /*
    simplified theta to be a set function of time  
    theta(t) = theta_i + thetaIncrement; 
    */ 
    if(grapple::theta >= grapple::thetaMax)
        grapple::thetaChange = -grapple::thetaMax/3;
    else if(grapple::theta <= -grapple::thetaMax)
        grapple::thetaChange = grapple::thetaMax/3;
    
    
    float theta_i = grapple::theta;
    float theta_f = theta_i + grapple::thetaChange;
    grapple::theta += grapple::thetaChange;

    if(grapple::velocity*grapple::velocity + 2*g*grapple::radius*(cos(theta_f*M_PI/180.0f)-cos(theta_i*M_PI/180.0f)) < 0){
        return;
    }
        


    if(grapple::thetaChange > 0)
        grapple::velocity = sqrt(grapple::velocity*grapple::velocity + 2*g*grapple::radius*(cos(theta_f*M_PI/180)-cos(theta_i*M_PI/180)));
    else if (grapple::thetaChange < 0)
        grapple::velocity = -sqrt(grapple::velocity*grapple::velocity + 2*g*grapple::radius*(cos(theta_f*M_PI/180)-cos(theta_i*M_PI/180)));


    //X movement
    if(( (float)grapple::velocity*cos(grapple::theta*M_PI/180)) > 0)
        ballProp::velocityX = ceil( grapple::velocity*cos(grapple::theta*M_PI/180));
    else
        ballProp::velocityX = floor( grapple::velocity*cos(grapple::theta*M_PI/180));


    if(ballProp::velocityX > 0){
        if(checkRightCollisions(ballProp::velocityX)){        
            ballPos::col += ballProp::velocityX * ellapsedTime;
        }
        else{  
            int reboundVelocity = 0;
            while(ballProp::velocityX > 0 && !checkRightCollisions(ballProp::velocityX)){
                ballProp::velocityX -= 1;
                reboundVelocity += 1;
            }
            ballPos::col += ballProp::velocityX *ellapsedTime;
            //Assume that energy disappation leaves only half velocity remaining
            ballProp::velocityX = -reboundVelocity/2;
        }
    }   
    else if(ballProp::velocityX < 0){
        if(checkLeftCollisions(-ballProp::velocityX)){        
            ballPos::col += ballProp::velocityX * ellapsedTime;
        }
        else{  
            int reboundVelocity = 0;
            while(ballProp::velocityX < 0 && !checkLeftCollisions(-ballProp::velocityX)){
                ballProp::velocityX += 1;
                reboundVelocity += 1;
            }
            ballPos::col += ballProp::velocityX * ellapsedTime;
            //Assume that energy disappation leaves only half velocity remaining
            ballProp::velocityX = reboundVelocity/2;
        }
    }

    //Y movement
    if(( (float)grapple::velocity*sin(grapple::theta*M_PI/180)) > 0)
        ballProp::velocityY = -ceil( grapple::velocity*sin(grapple::theta*M_PI/180));
    else
        ballProp::velocityY = -floor( grapple::velocity*sin(grapple::theta*M_PI/180));

    
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
        if(checkUpCollisions(-ballProp::velocityY)){       
            ballPos::row += ballProp::velocityY * ellapsedTime;
        }
        else{
            int reboundVelocity = 0;
            while(!checkUpCollisions(-ballProp::velocityY)){
                ballProp::velocityY += 1;
                reboundVelocity += 1;
            }
            ballPos::row += ballProp::velocityY *ellapsedTime;
            //Assume that energy disappation leaves only half velocity remaining
            ballProp::velocityY = reboundVelocity/2;
        }
    }
}


//1 unit = 5 m ; g = 10m/s^2; 
void simulateVerticalMovement(int ellapsedTime){
    ballProp::velocityY += ballProp::accelerationY * ellapsedTime;
    
    // gravity
    if(checkDownCollisions(1)){
        ballProp::accelerationY = g; 
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
        if(checkUpCollisions(-ballProp::velocityY)){       
            ballPos::row += ballProp::velocityY * ellapsedTime;
        }
        else{
            int reboundVelocity = 0;
            while(!checkUpCollisions(-ballProp::velocityY)){
                ballProp::velocityY += 1;
                reboundVelocity += 1;
            }
            ballPos::row += ballProp::velocityY *ellapsedTime;
            //Assume that energy disappation leaves only half velocity remaining
            ballProp::velocityY = reboundVelocity/2;
        }
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
        if(checkRightCollisions(ballProp::velocityX)){        
            ballPos::col += ballProp::velocityX * ellapsedTime;
            if(signals::rolling_counter == 0){
                signals::rolling_right1 = true;
                signals::rolling_right2 = false;
            }
            else if(signals::rolling_counter == 3){
                signals::rolling_right1 = false;
                signals::rolling_right2 = true;
                signals::rolling_counter = -3;
            }
            signals::rolling_counter++;
        }
        else{  
            int reboundVelocity = 0;
            while(!checkRightCollisions(ballProp::velocityX)){
                ballProp::velocityX -= 1;
                reboundVelocity += 1;
            }
            ballPos::col += ballProp::velocityX *ellapsedTime;
            if(signals::rolling_counter == 0){
                signals::rolling_right1 = true;
                signals::rolling_right2 = false;
            }
            else if(signals::rolling_counter == 3){
                signals::rolling_right1 = false;
                signals::rolling_right2 = true;
                signals::rolling_counter = -3;
            }
            signals::rolling_counter++;

            //Assume that energy disappation leaves only half velocity remaining
            ballProp::velocityX = -reboundVelocity/2;
        }
    }   
    else if(ballProp::velocityX < 0){
        if(checkLeftCollisions(-ballProp::velocityX)){        
            ballPos::col += ballProp::velocityX * ellapsedTime;
            if(signals::rolling_counter == 0){
                signals::rolling_left1 = true;
                signals::rolling_left2 = false;
            }
            else if(signals::rolling_counter == 3){
                signals::rolling_left1 = false;
                signals::rolling_left2 = true;
                signals::rolling_counter = -3;
            }
            signals::rolling_counter++;
        }
        else{  
            int reboundVelocity = 0;
            while(!checkLeftCollisions(-ballProp::velocityX)){
                ballProp::velocityX += 1;
                reboundVelocity += 1;
            }
            ballPos::col += ballProp::velocityX * ellapsedTime;
            if(signals::rolling_counter == 0){
                signals::rolling_left1 = true;
                signals::rolling_left2 = false;
            }
            else if(signals::rolling_counter == 3){
                signals::rolling_left1 = false;
                signals::rolling_left2 = true;
                signals::rolling_counter = -3;
            }
            signals::rolling_counter++;
            //Assume that energy disappation leaves only half velocity remaining
            ballProp::velocityX = reboundVelocity/2;
        }
    }
    else{
        signals::rolling_left1 = false;
        signals::rolling_left2 = false;
        signals::rolling_right1 = false;
        signals::rolling_right2 = false;
        signals::rolling_counter = 0;
    }
}



float max = 0.0;
float min = 0.0;

void simulateMovement(int ellapsedTime){
    //using unit circle, find the direction in rad or degree of force to then map it on a compass
    
    int theta = arctan(forceBar::yForce, forceBar::xForce);
    
    
    if(ballProp::velocityX > max) max = ballProp::velocityX;
    if(ballProp::velocityX < min) min = ballProp::velocityX;
    
    //force compass
    resetVector();
    switch (theta){
        case 0:
            //check if net force is 0.
            if(forceBar::xForce == 0 && forceBar::yForce == 0){
                break;
            }
            forceCompass::east = true;
            break;
        case 90: 
            forceCompass::north = true;
            break;
        case 180:
            forceCompass::west = true;
            break;
        case 270:
            forceCompass::south = true;
            break;
        default:
            if(theta > 0 && theta < 90){
                forceCompass::northe = true;
            }
            else if(theta < 180){
                forceCompass::northw = true;
            }
            else if(theta < 270 ){
                forceCompass::southw = true;
            }
            else if(theta > 270){
                forceCompass::southe = true;
            }
    }

    //movement functions
    if(signals::grappled){
        simulatePendulumMotion(ellapsedTime);
    }
    else{
        simulateVerticalMovement(ellapsedTime);
        simulateHorizontalMovement(ellapsedTime);
    }
}