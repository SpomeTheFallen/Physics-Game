namespace homescreen{
    inline int row = 0;
    inline int col = 0;
}

enum class levelType{
    level0,
    level1,
    level2
};

namespace levels{
    const int rows = 35;
    const int cols = 75;
    extern int (*currentLevel)[levels::cols];
    void setLevel(levelType type);
    bool levelCompleted();
}





