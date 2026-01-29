namespace ballProp{
    const int rows = 4;
    const int cols = 4;
}

//marks the balls highest and most left points relative to terminal grid.
namespace ballPos{
    inline int col = 1;
    inline int row = 1;
}

struct signals{
    static bool rolling_right1;
    static bool rolling_right2;
    static int rolling_counter;
};

bool checkRightCollisions();

void move_right();

void move_left();

void move_up();

void move_down();

extern int ball[ballProp::rows][ballProp::cols];
extern int ballR1[ballProp::rows][ballProp::cols];
extern int ballR2[ballProp::rows][ballProp::cols];