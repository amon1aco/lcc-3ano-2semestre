#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float angAlfa = 0.0f;
float angBeta = 0.0f;
float radius = 5.0f;
float camX, camY, camZ;

void spherical2Cartesian() {
	camX = radius * cos(angBeta) * sin(angAlfa);
	camY = radius * sin(angBeta);
	camZ = radius * cos(angBeta) * cos(angAlfa);
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void drawCylinder(float radius, float height, int slices) {
// put code to draw cylinder in here

	float angulo = 360.0 / slices;

	// pz = r * cos(b) * cos(a)
	// py = r * cos(b) * sin(a)
	// px = r * sin(b)
	glBegin(GL_TRIANGLES);
		// topo
		for (int i = 0; i < slices; i++) {
			glVertex3f(0, height * 0.5, 0);
			glVertex3f(cos(i * angulo * M_PI / 180.0) * radius, height * 0.5, -sin(i * angulo * M_PI / 180.0) * radius);
			glVertex3f(cos((i + 1) * angulo * M_PI / 180.0) * radius, height * 0.5, -sin((i + 1) * angulo * M_PI / 180.0) * radius);
		}

		// topo
		for (int i = 0; i < slices; i++) {
			glVertex3f(0, -height * 0.5, 0);
			glVertex3f(cos((i + 1) * angulo * M_PI / 180.0) * radius, -height * 0.5, -sin((i + 1) * angulo * M_PI / 180.0) * radius);
			glVertex3f(cos(i * angulo * M_PI / 180.0) * radius, -height * 0.5, -sin(i * angulo * M_PI / 180.0) * radius);
		}

		// paredes
		for (int i = 0; i < slices; i++) {

			glVertex3f(cos(i * angulo * M_PI / 180.0) * radius, height * 0.5, -sin(i * angulo * M_PI / 180.0) * radius);
			glVertex3f(cos(i * angulo * M_PI / 180.0) * radius, -height * 0.5, -sin(i * angulo * M_PI / 180.0) * radius);
			glVertex3f(cos((i + 1) * angulo * M_PI / 180.0) * radius, height * 0.5, -sin((i + 1) * angulo * M_PI / 180.0) * radius);

			glVertex3f(cos(i * angulo * M_PI / 180.0) * radius, -height * 0.5, -sin(i * angulo * M_PI / 180.0) * radius);
			glVertex3f(cos((i + 1) * angulo * M_PI / 180.0) * radius, -height * 0.5, -sin((i + 1) * angulo * M_PI / 180.0) * radius);
			glVertex3f(cos((i + 1) * angulo * M_PI / 180.0) * radius, height * 0.5, -sin((i + 1) * angulo * M_PI / 180.0) * radius);
		}
	glEnd();

}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	drawCylinder(1,2,10);

	// End of frame
	glutSwapBuffers();
}


void processSpecialKeys(int key, int xx, int yy) {
	
	switch (key) {
		case GLUT_KEY_RIGHT:
			angAlfa -= 0.1; break;

		case GLUT_KEY_LEFT:
			angAlfa += 0.1; break;

		case GLUT_KEY_UP:
			angBeta += 0.1f;
			break;
		case GLUT_KEY_DOWN:
			angBeta -= 0.1f;
			break;
	}
	spherical2Cartesian();
	glutPostRedisplay();
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard 
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);

	spherical2Cartesian();
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
