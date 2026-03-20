namespace homescreen{
    struct signals{
        static bool B1selected;
    };
}

namespace levels{
    const int rows = 35;
    const int cols = 75;
    extern int (*currentLevel)[levels::cols];
    void setLevel0();
    void setLevel1();
    void setLevel2();
    bool levelCompleted();
}





