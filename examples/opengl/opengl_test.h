#ifndef OPENGL_TEST_H
#define OPENGL_TEST_H

#include <GL/glut.h>
#include "../../sdk/earth/earth_point.h"

using namespace yalgo::earth;

class OpenGLTest {
private:
    static OpenGLTest* instance;
    EarthPoint* earthPoint;
    float rotation;

public:
    OpenGLTest();
    ~OpenGLTest();

    // 初始化OpenGL
    void init(int argc, char** argv);

    // 显示回调函数
    static void display();

    // 空闲回调函数
    static void idle();

    // 键盘回调函数
    static void keyboard(unsigned char key, int x, int y);

    // 运行OpenGL主循环
    void run();

    // 绘制地球坐标点
    void drawEarthPoint();

    // 绘制简单的3D场景
    void drawScene();
};

#endif // OPENGL_TEST_H
