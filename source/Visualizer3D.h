#include "Camera.h"

class Visualizer3D
{
private:

public:
    Visualizer3D();
    ~Visualizer3D();
    void run(int arc, char** argv);
    static void DrawObject();
    static void Display();
    static void Reshape(int w, int h);
    static void KeyBoard(unsigned c, int x, int y);
    static Camera camera;
};

