#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

void changeSize(int w, int h) {
    if (h == 0) h = 1;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, (float)w / (float)h, 1.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

float teapotSize = 1.0f;

void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Aumenta e diminui o tamanho do bule de chá usando a função seno
    teapotSize = 1.0f + 0.5f * sin(glutGet(GLUT_ELAPSED_TIME) / 600.0); // O valor 1000 ajusta a velocidade

    // Desenha o bule de chá com o tamanho modificado
    glutWireTeapot(teapotSize);

    glutSwapBuffers();
}

void printInfo(void) {
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1600, 800);
    glutCreateWindow("eduardovski");

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    glutIdleFunc(renderScene);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    glutMainLoop();

    return 0;
}
