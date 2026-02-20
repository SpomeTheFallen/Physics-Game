#include <iostream>
#include "level_grids.hpp"
#include "ball.hpp"
#include "game_graphics.hpp"

void readyTerminal(){
    std::cout << "\x1b[?25l"; //hide cursor
    std::cout << "\x1b[8;600;600t"; //set size rows ; col
}

/*
In draw we imagine the terminal as a grid
It updates line by line, or every col for each row
We set a barrier to bound the level, so we must shift the level's array by 1 in each index.
To draw the ball, we check if the index we are on is its posistion relative to the terminal grid, then fill the line
It is important to note, cursor posistion affects print and not the index, so they must be set to always match.
*/

void draw(){
    std::cout << "\x1b[H";
    
    for(int i = 0; i < l0Prop::rows+2 ; i++){
        for(int j = 0 ; j < 86; j++){

            //energy bar & force bar
            if(j > l0Prop::cols){
                if(energyBar::row <= i && i < energyBar::row + 10){
                    if(j == energyBar::col-1){
                        std::cout << '[';
                    }
                    else if(j == energyBar::col + 1){
                        std::cout << ']';
                    }
                    else if(j == energyBar::col && energyBar::bar[i-3] == 1){
                        std::cout << '%';
                    }
                    else{
                        std::cout << ' ';
                    }
                }
                if(forceBar::row <= i && i < forceBar::row + 10){
                    if(j == forceBar::col-1){
                        std::cout << '[';
                    }
                    else if(j == forceBar::col + 1){
                        std::cout << ']';
                    }
                    else if(j == forceBar::col && forceBar::bar[i-15] == 1){
                        std::cout << '@';
                    }
                    else{
                        std::cout << ' ';
                    }
                }
                if(forceCompass::row <= i && i < 5 + forceCompass::row){
                    if(j >= forceCompass::col){
                        switch(forceCompass::forceUnitVector[i-forceCompass::row][j-forceCompass::col]){
                            case 2:
                                std::cout << 'O';
                                break;
                            case 1:
                                std::cout << '+';
                                break;
                            case 0:
                                std::cout << ' ';
                        }        
                    }
                }
                
                continue;
            }

            //auto barrier
            if(i == 0 || i == l0Prop::rows+1){
                std::cout << 'x';
                continue;
            }
            if(j == 0 || j == l0Prop::cols+1){
                std::cout << 'x';
                continue;
            }    
            //level design
            if(level0[i-1][j-1] == 1){
                std::cout << 'x';
            }
            //ball posistion + animations
            else if((ballPos::col <= j && j < ballPos::col + ballProp::cols) && (ballPos::row <= i && i < ballPos::row + ballProp::rows) ){
                if(signals::rolling_right1){
                    if(ballR1[i-ballPos::row][j-ballPos::col] == 1){
                        std::cout << 'H';
                    }
                    else if(ballR1[i-ballPos::row][j-ballPos::col] == 2){
                        std::cout << '#';
                    }
                    else{
                        std::cout << ' ';
                    } 
                }
                
                else if(signals::rolling_right2){
                    if(ballR2[i-ballPos::row][j-ballPos::col] == 1){
                        std::cout << 'H';
                    }
                    else if(ballR2[i-ballPos::row][j-ballPos::col] == 2){
                        std::cout << '#';
                    }
                    else{
                        std::cout << ' ';
                    }
                }
                else if(signals::rolling_left1){
                    if(ballR2[i-ballPos::row][j-ballPos::col] == 1){
                        std::cout << 'H';
                    }
                    else if(ballR2[i-ballPos::row][j-ballPos::col] == 2){
                        std::cout << '#';
                    }
                    else{
                        std::cout << ' ';
                    }
                }
                else if(signals::rolling_left2){
                    if(ballR1[i-ballPos::row][j-ballPos::col] == 1){
                        std::cout << 'H';
                    }
                    else if(ballR1[i-ballPos::row][j-ballPos::col] == 2){
                        std::cout << '#';
                    }
                    else{
                        std::cout << ' ';
                    }
                }
                else if(signals::springed1){
                    if(ballSpringed1[i-ballPos::row][j-ballPos::col] == 1){
                        std::cout << 'H';
                    }
                    else if(ballSpringed1[i-ballPos::row][j-ballPos::col] == 2){
                        std::cout << '#';
                    }
                    else{
                        std::cout << ' ';
                    }      
                }
                else if(signals::springed2){
                    if(ballSpringed2[i-ballPos::row][j-ballPos::col] == 1){
                        std::cout << 'H';
                    }
                    else if(ballSpringed2[i-ballPos::row][j-ballPos::col] == 2){
                        std::cout << '#';
                    }
                    else{
                        std::cout << ' ';
                    }  
                }
                
                else{
                    if(ball[i-ballPos::row][j-ballPos::col] == 1){
                        std::cout << 'H';
                    }
                    else if(ball[i-ballPos::row][j-ballPos::col] == 2){
                        std::cout << '#';
                    }
                    else{
                        std::cout << ' ';
                    }                    
                }
                
            }
            else{
                std::cout << ' ';
            }
        }
        //for animations to work row by row.
        if(ballPos::row <= i && i < ballPos::row + ballProp::rows){
            //right animation
            if(signals::rolling_right1){
                if(signals::rolling_counter < 4) signals::rolling_counter++;
                else {      
                    signals::rolling_right1 = false;
                    signals::rolling_counter = 0;
                    signals::rolling_right2 = true;
                }
            }
            else if(signals::rolling_right2){
                if(signals::rolling_counter < 4) signals::rolling_counter++;
                else{ signals::rolling_right2 = false;}
            }
            //left animation
            else if(signals::rolling_left1){
                if(signals::rolling_counter < 4) signals::rolling_counter++;
                else {      
                    signals::rolling_left1 = false;
                    signals::rolling_counter = 0;
                    signals::rolling_left2 = true;
                }
            }
            else if(signals::rolling_left2){
                if(signals::rolling_counter < 4) signals::rolling_counter++;
                else{ signals::rolling_left2 = false;}
            }

        }
        
        std::cout << '\n';
    }
}

