namespace homescreen{
    struct signals{
        static bool B1selected;
    };
}

namespace levels{
    const int rows = 35;
    const int cols = 75;
    extern int level0[levels::rows][levels::cols];
    extern int level1[levels::rows][levels::cols];
    void setLevel0();
    bool levelCompleted();
}





