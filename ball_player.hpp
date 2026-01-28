namespace ballProp{
    const int rows = 7;
    const int cols = 9;
}

namespace ballPos{
    inline int col = 0;
    inline int row = 0;
}

void move_right();

void move_left();

void move_up();

void move_down();

extern int ball[ballProp::rows][ballProp::cols];