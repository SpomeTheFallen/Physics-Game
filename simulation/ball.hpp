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

namespace grapple{
    inline float theta = 0;  
    inline float thetaChange = 0;
    inline float thetaMax = 0;
    inline int radius = 0;
    inline float velocity = 0;
    inline float iVelocity = 0;
    inline int row = 0;
    inline int col = 0;
} 


struct signals{
    static int rolling_counter;

    static bool rolling_right1;
    static bool rolling_right2;
    static bool rolling_left1;
    static bool rolling_left2;
    static bool springed1;
    static bool springed2;
    static bool grappled;
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
    inline bool north = false;
    inline bool northe = false;
    inline bool east = false;
    inline bool southe = false;
    inline bool south = false;
    inline bool southw = false;
    inline bool west = false;
    inline bool northw = false;
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

void launch_grapple(direction dir);

void move_up();

void simulateMovement(int ellapsedTime = 1);

extern int ball[ballProp::rows][ballProp::cols];
extern int ballR1[ballProp::rows][ballProp::cols];
extern int ballR2[ballProp::rows][ballProp::cols];

extern int ballSpringed1[ballProp::rows][ballProp::cols];
extern int ballSpringed2[ballProp::rows][ballProp::cols];