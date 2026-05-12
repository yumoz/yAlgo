#include "opengl_test.h"

// 静态成员初始化
OpenGLTest* OpenGLTest::instance = nullptr;

OpenGLTest::OpenGLTest() {
    instance = this;
    earthPoint = new EarthPoint(116.3974, 39.9093, 50.0);
    rotation = 0.0f;
}

OpenGLTest::~OpenGLTest() {
    delete earthPoint;
    instance = nullptr;
}

void OpenGLTest::init(int argc, char** argv) {
    // 初始化GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Earth SDK Test");

    // 设置回调函数
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);

    // 初始化OpenGL
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // 设置透视投影
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0/600.0, 0.1, 1000.0);
}

void OpenGLTest::display() {
    if (instance) {
        instance->drawScene();
    }
}

void OpenGLTest::idle() {
    if (instance) {
        instance->rotation += 0.5f;
        if (instance->rotation > 360.0f) {
            instance->rotation -= 360.0f;
        }
        glutPostRedisplay();
    }
}

void OpenGLTest::keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC键
        exit(0);
    }
}

void OpenGLTest::run() {
    glutMainLoop();
}

void OpenGLTest::drawEarthPoint() {
    // 绘制地球坐标点（简化为一个球体）
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(rotation, 0.0f, 1.0f, 0.0f);
    
    // 绘制地球球体
    glColor3f(0.0f, 0.5f, 1.0f);
    glutWireSphere(1.0, 20, 20);
    
    // 绘制标记点
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    // 简化处理：使用经纬度作为球体上的点
    double lat = earthPoint->latitude() * 3.1415926535 / 180.0;
    double lon = earthPoint->longitude() * 3.1415926535 / 180.0;
    float x = sin(lat) * cos(lon);
    float y = sin(lat) * sin(lon);
    float z = cos(lat);
    glVertex3f(x, y, z);
    glEnd();
    
    glPopMatrix();
}

void OpenGLTest::drawScene() {
    // 清除颜色和深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // 设置模型视图矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // 绘制地球坐标点
    drawEarthPoint();
    
    // 绘制坐标轴
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f);
    
    // X轴（红色）
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(2.0f, 0.0f, 0.0f);
    glEnd();
    
    // Y轴（绿色）
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 2.0f, 0.0f);
    glEnd();
    
    // Z轴（蓝色）
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 2.0f);
    glEnd();
    
    glPopMatrix();
    
    // 交换缓冲区
    glutSwapBuffers();
}
