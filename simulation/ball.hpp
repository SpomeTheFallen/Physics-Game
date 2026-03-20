namespace ballProp{
    const int rows = 4;
    const int cols = 4;
    extern float velocityX;
    extern float velocityY;
    extern float accelerationX;
    extern float accelerationY;
    inline float theta = 0.0f;
}

//marks the balls highest and most left points relative to rendering grid.
namespace ballPos{
    inline float col = 1.0f;
    inline float row = 1.0f;
  
}

//theta in degrees
namespace grapple{
    inline float theta = 0;  
    inline float thetaChange = 0;
    inline float thetaMax = 0;
    inline int radius = 0;
    inline float velocity = 0;
    inline int row = 0;
    inline int col = 0;
} 


struct signals{
    static bool springed1;
    static bool springed2;
    static bool grappled;
};

enum class direction{
    left,
    right,
    compress,
    uncompress,
};

namespace forceBar{
    const int row = 15;
    const int col = 79;
    extern float xForce;
    extern float yForce;
    extern float force;
    extern int bar[10];
    
}

namespace forceCompass{
    const int row = 30;
    const int col = 81;
    inline float theta = 0.0f;
}

namespace energyBar{
    const int col = 79;
    const int row = 3;
    extern float internal;
    extern int bar[10];
}

void transferEnergy(float velocityChange);

void chargeForce(direction dir);

void executeForce();

void launch_grapple();

void simulateMovement(float ellapsedTime = 1);

