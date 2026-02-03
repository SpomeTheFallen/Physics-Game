namespace ballProp{
    const int rows = 4;
    const int cols = 4;
    extern int velocityX;
    extern int velocityY;
    extern int velocityZ; 
}

//marks the balls highest and most left points relative to terminal grid.
namespace ballPos{
    inline int col = 1;
    inline int row = 1;
}

struct signals{
    static int rolling_counter;

    static bool rolling_right1;
    static bool rolling_right2;
    static bool rolling_left1;
    static bool rolling_left2;
};

namespace energyBar{
    const int col = 79;
    const int row = 3;
    extern int internal;
    extern int bar[10];
}

void transferEnergy(int velocityChange);

bool checkGravityCollisions(int velocity);

void accelerate_right();

void accelerate_left();

void move_up();

void simulateGravity();

void simulateMovement();

extern int ball[ballProp::rows][ballProp::cols];
extern int ballR1[ballProp::rows][ballProp::cols];
extern int ballR2[ballProp::rows][ballProp::cols];