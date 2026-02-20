namespace ballProp{
    const int rows = 4;
    const int cols = 4;
    extern int velocityX;
    extern int velocityY;
    extern int accelerationX;
    extern int accelerationY;
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
    static bool springed1;
    static bool springed2;
};

enum class direction{
    left,
    right,
    compress
};

namespace forceBar{
    const int row = 15;
    const int col = 79;
    extern int xForce;
    extern int yForce;
    extern int force;
    extern int bar[10];
    
}

namespace forceCompass{
    const int row = 30;
    const int col = 81;
    extern int forceUnitVector[5][5];
}

namespace energyBar{
    const int col = 79;
    const int row = 3;
    extern int internal;
    extern int bar[10];
}

void transferEnergy(int velocityChange);

void chargeForce(direction dir);

void executeForce();

void move_up();

void simulateMovement(int ellapsedTime = 1);

extern int ball[ballProp::rows][ballProp::cols];
extern int ballR1[ballProp::rows][ballProp::cols];
extern int ballR2[ballProp::rows][ballProp::cols];

extern int ballSpringed1[ballProp::rows][ballProp::cols];
extern int ballSpringed2[ballProp::rows][ballProp::cols];