namespace ballProp{
    const int rows = 7;
    const int cols = 9;
}

//marks the balls highest and most left points relative to terminal grid.
namespace ballPos{
    inline int col = 1;
    inline int row = 1;
}

void move_right();

void move_left();

void move_up();

void move_down();

extern int ball[ballProp::rows][ballProp::cols];