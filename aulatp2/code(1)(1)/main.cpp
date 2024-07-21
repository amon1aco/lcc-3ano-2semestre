#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

GLfloat angle = 0.0f;

GLfloat transZ = 0.0f;
GLfloat transX = 0.0f;

GLfloat scale = 1.0f;

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


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

// put axis drawing in here
	glBegin(GL_LINES);
		// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(
			-100.0f, 0.0f, 0.0f);
		glVertex3f(100.0f, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f,
			-100.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
		// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f,
			-100.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();

// put the geometric transformations here
	glRotatef(angle, 0, 1, 0);

	glTranslatef(0, 0, transZ);
	glTranslatef(transX, 0, 0);

	glScalef(1, scale, 1);

// put pyramid drawing instructions here
	glBegin(GL_TRIANGLES);
		// Blue
		glVertex3f(0.0f, 3.0f, 0.0f);
		glVertex3f(2.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 2.0f);
		
		// Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -2.0f);
		glVertex3f(0.0f, 3.0f, 0.0f);
		glVertex3f(2.0f, 0.0f, 0.0f);

		// Red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -2.0f);
		glVertex3f(-2.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 3.0f, 0.0f);

		// White
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-2.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 2.0f);
		glVertex3f(0.0f, 3.0f, 0.0f);

	glEnd();


	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events
void roda(unsigned char key, int x, int y) {
	if (key == 'd' || key == 'D') { // Verifica se a tecla pressionada é "d"
		angle += 5.0f; // Incrementa o ângulo de rotação
		glutPostRedisplay(); // Indica que a cena deve ser redesenhada
	}
	if (key == 'a' || key == 'A') { // Verifica se a tecla pressionada é "a"
		angle -= 5.0f; // Diminui o ângulo de rotação
		glutPostRedisplay(); // Indica que a cena deve ser redesenhada
	}
}

void move(int key, int x, int y) {
	if (key == GLUT_KEY_UP) { // Verifica se a tecla pressionada é "d"
		transZ += 0.5f; // Incrementa o ângulo de rotação
		glutPostRedisplay(); // Indica que a cena deve ser redesenhada
	}
	if (key == GLUT_KEY_DOWN) { // Verifica se a tecla pressionada é "a"
		transZ -= 0.5f; // Diminui o ângulo de rotação
		glutPostRedisplay(); // Indica que a cena deve ser redesenhada
	}
	if (key == GLUT_KEY_LEFT) { // Verifica se a tecla pressionada é "d"
		transX += 0.5f; // Incrementa o ângulo de rotação
		glutPostRedisplay(); // Indica que a cena deve ser redesenhada
	}
	if (key == GLUT_KEY_RIGHT) { // Verifica se a tecla pressionada é "a"
		transX -= 0.5f; // Diminui o ângulo de rotação
		glutPostRedisplay(); // Indica que a cena deve ser redesenhada
	}
}

void escala(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			// Aumenta a escala quando o botão esquerdo é pressionado
			scale += 0.1f;
			glutPostRedisplay();
		}
		else if (button == GLUT_RIGHT_BUTTON) {
			// Diminui a escala quando o botão direito é pressionado
			scale -= 0.1f;
			glutPostRedisplay();
		}
	}
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

	
// put here the registration of the keyboard callbacks
	glutKeyboardFunc(roda);

	glutSpecialFunc(move);

	glutMouseFunc(escala);


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
