

#include<stdio.h>
#include<stdlib.h>
#include <iostream>


#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define MAX_TREES 500
#define RAIOCIRCULO 30
#define RAIOTEAPOT 25

float alfa = 0.0f, radius = 5.0f;

float alfa_start = 0;
float beta_start = 0;


float dx = 0;
float dy = 0;
float dz = 0;

float camX = 0, camY = 0, camZ = 0;
int startX, startY, tracking = 0;
bool axis = false;

unsigned int t, tw, th;
unsigned char *imageData;

int alpha = 0, beta = 45, r = 50;

// Estrutura para representar um vértice
struct Vertex {
    float x, y, z;
};

struct TreePosition{
    float x;
    float z;
} ;


Vertex *vertices; // Cria um array de vértices
TreePosition treePositions[MAX_TREES];

//altura do pixel 
float h(int coluna, int linha) {
    return imageData[tw * linha + coluna] / 255.0 * 100; // matriz para array -> linha * tw + coluna
}

//altura do terreno
float hf(float x, float z) {
    // Obtenha a parte inteira de x e z
    int x1 = floor(x);
    int z1 = floor(z);

    // Obtenha a parte fracionária de z
    float fz = z - z1;

    // Calcule as alturas nos quatro cantos da célula da grade
    float h_x1_z1 = h(x1, z1);
    float h_x1_z2 = h(x1, z1 + 1);
    float h_x2_z1 = h(x1 + 1, z1);
    float h_x2_z2 = h(x1 + 1, z1 + 1);

    // Interpole as alturas ao longo do eixo z (interpolação linear)
    float h_x1_z = h_x1_z1 * (1 - fz) + h_x1_z2 * fz;
    float h_x2_z = h_x2_z1 * (1 - fz) + h_x2_z2 * fz;

    // Obtenha a parte fracionária de x
    float fx = x - x1;

    // Interpole as alturas ao longo do eixo x (interpolação linear)
    float height_xz = h_x1_z * (1 - fx) + h_x2_z * fx;

    return height_xz;
}

void generateTreePositions() {
    srand(time(NULL)); // start the random number sequence

    for (int i = 0; i < MAX_TREES; ++i) {
        float x, z;
        do {
            x = (float)(rand() % 200 - 100); // Coordenada x aleatória entre -100 e 100
            z = (float)(rand() % 200 - 100); // Coordenada z aleatória entre -100 e 100
        } while (sqrt(x * x + z * z) <= RAIOCIRCULO); // Verifica se as coordenadas estão dentro do círculo

        treePositions[i].x = x;
        treePositions[i].z = z;
    }
}

void drawTrees() {
    for (int i = 0; i < MAX_TREES; ++i) {
		float x = treePositions[i].x;
		float z = treePositions[i].z;
		
        // Calcula a altura do círculo usando interpolação bilinear dos pixels da imagem
        int imgX = (int)(x + 128); // Ajusta as coordenadas do círculo para o espaço da imagem
        int imgZ = (int)(z + 128);
        float y1 = hf(imgX, imgZ)+ 3.0f; // Usa a função hf para obter a altura
		float y2 = hf(imgX, imgZ); // Usa a função hf para obter a altura

        glPushMatrix();
        glColor3f(0.0f, 0.5f, 0.0f);
        glTranslatef(x, y1, z);
        glRotatef(-90.0f,1.0f,0.0f,0.0f);
        glutSolidCone(3.0f, 10.0f, 5, 20); // Desenha o cone da árvore
        glPopMatrix();

        glColor3f(0.6f, 0.3f, 0.0f);
        glPushMatrix();
        glTranslatef(x, y2, z);
        glRotatef(-90.0f,1.0f,0.0f,0.0f);
        glutSolidCone(1.0f, 5.0f, 15, 20); // Desenha o cone da árvore
        glPopMatrix();
    }
}

void drawTeapot(double raio1, double raio2, int quantidade){

    beta_start+= M_PI / 60 ;
    if ( beta_start == M_PI * 2)
        beta_start = 0;

    float step_beta = M_PI * 2 / quantidade;
    float current_beta = beta_start;

    for( int i = 0; i < quantidade ;++i ){

        float x = sin(current_beta) * raio1;
        float z = cos(current_beta) * raio1;

        // Calcula a altura do círculo usando interpolação bilinear dos pixels da imagem
        int imgX = (int)(x + 128); // Ajusta as coordenadas do círculo para o espaço da imagem
        int imgZ = (int)(z + 128);
        float y = hf(imgX, imgZ)+ 2.0f; // Usa a função hf para obter a altura

        float degrees = current_beta * (180.0/M_PI); // conversão para radianos

        glPushMatrix();
        glColor3f(0.5f, 0.0f, 0.0f);
        glTranslatef(x,y,z);
        glRotatef(degrees,0,1,0);
        glutSolidTeapot(1);
        glPopMatrix();

        current_beta+=step_beta;
    }

    alfa_start+= M_PI / 60 ;
    if ( alfa_start == M_PI * 2)
       alfa_start = 0;

    float step_apha = M_PI * 2 / quantidade;
    float current_alpha = alfa_start;

    for( int i = 0; i<quantidade ;++i ){

        float x = sin(current_alpha) * raio2;
        float z = cos(current_alpha) * raio2;

        // Calcula a altura do círculo usando interpolação bilinear dos pixels da imagem
        int imgX = (int)(x + 128); // Ajusta as coordenadas do círculo para o espaço da imagem
        int imgZ = (int)(z + 128);
        float y = hf(imgX, imgZ)+ 2.0f; // Usa a função hf para obter a altura

        float degrees = current_alpha * (180.0/M_PI); // conversão para radianos

        glPushMatrix();
        glColor3f(0.0f, 0.0f, 0.5f);
        glTranslatef(x,y,z);
        glRotatef(degrees - 90 ,0,1,0);
        glutSolidTeapot(1);
        glPopMatrix();

        current_alpha+=step_apha;
    }
    glutPostRedisplay();
}

void drawTorus(){
    glPushMatrix();
    glColor3f(0.6f, 0.0f, 0.6f); // Cor roxa (vermelho: 0.6, verde: 0.0, azul: 0.6)
    glutSolidTorus(1.0f, 1.5f, 10,10);
    glPopMatrix();
}

void drawCircle(double radius, int slices) {
    double sliceStep = 2.0 * M_PI / slices;
    double alfa = 0.0f;

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= slices; ++i) {
        float x = cos(alfa) * radius;
        float z = sin(alfa) * radius;

        // Calcula a altura do círculo usando interpolação bilinear dos pixels da imagem
        int imgX = (int)(x + 128); // Ajusta as coordenadas do círculo para o espaço da imagem
        int imgZ = (int)(z + 128);
        float y = hf(imgX, imgZ); // Usa a função hf para obter a altura

        glVertex3f(x, y, z); // Desenha o círculo com a altura correta

        alfa += sliceStep;
    }
    glEnd();
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawTerrain() {
    // Especificar a localização e o formato dos dados dos vértices
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertices);

    // Definindo a cor para a grade
    glColor3f(1.0f, 1.0f, 1.0f);
    // Desenhar o plano
    glPushMatrix();
    glTranslatef(-128,0,-128);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for (int i = 0; i < th - 1 ; i++) {
 		glDrawArrays(GL_TRIANGLE_STRIP, (tw) * 2 * i, (tw) * 2);
 	}
    glPopMatrix();

    // Restaurar o modo de desenho dos polígonos para sólido
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Desabilitar o array de vértices
    glDisableClientState(GL_VERTEX_ARRAY);
}

void renderScene(void) {
	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
    //camY+dy
    int imgX = (int)(camX+dx + 128); // Ajusta as coordenadas do círculo para o espaço da imagem
    int imgZ = (int)(camZ+dz + 128);
    float y1 = hf(imgX, imgZ) + 5.0f; // 5 por causa do olho da pessoa
    
	gluLookAt(camX+dx, y1 , camZ+dz, 
		      dx + sin(alfa), y1,  dz + cos(alfa),
			  0.0f,1.0f,0.0f);

    if (axis){
        glBegin(GL_LINES);
// X axis in red
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(
                -100.0f, 0.0f, 0.0f);
        glVertex3f( 100.0f, 0.0f, 0.0f);
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
    }
	drawTrees();
    drawTeapot(RAIOTEAPOT, RAIOTEAPOT-20, 10);
    drawCircle(RAIOCIRCULO,10);
    drawTorus();
    drawTerrain();


// End of frame
	glutSwapBuffers();
}

void processKeys(unsigned char key, int xx, int yy) {

    switch (key) {
        case 'x':
            axis = !axis;
            break;
		case 'd': dx += 1;
			break;
		case 'a': dx -= 1;
			break;
		case 'w': dy += 1;
			break;
		case 's': dy -= 1;
			break;
		case 'q': dz -= 1;
			break;
		case 'e': dz += 1;
			break;
		default:
        	break;
	}
	glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
		if (radius < 0.1f)
			radius = 0.1f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 0.1f; break;
	}
	glutPostRedisplay();

}

void processMouseButtons(int button, int state, int xx, int yy) {
	
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			
			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
}

void init() {
    ilInit();
    // Índice para rastrear a posição atual no array de vértices
    int index = 0;

    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring)"terreno.jpg");

    ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
    tw = ilGetInteger(IL_IMAGE_WIDTH); // vermelho
    th = ilGetInteger(IL_IMAGE_HEIGHT); // azul

    // Imprimindo as variáveis tw e th
    std::cout << "Largura da imagem: " << tw << std::endl;
    std::cout << "Altura da imagem: " << th << std::endl;

    // imageData is a LINEAR array with the pixel values
    imageData = ilGetData();

    // Aloca memória para o array de vértices e inicializa-o
    vertices = new Vertex[th * tw * 2];
    // Build the vertex arrays
    for (int i = 0; i < th; i++) {
        for (int j = 0; j < tw; j++) {
            // Atribui as coordenadas ao próximo elemento do array
            vertices[index].x = j;
            vertices[index].y = hf(j, i);
            vertices[index].z = i;
            index++;

            vertices[index].x = j;
            vertices[index].y = hf(j, i+1);
            vertices[index].z = i+1;
            index++;
        }
    }
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
}


int main(int argc, char **argv) {
    generateTreePositions(); // Gera as posições das árvores aleatoriamente

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("CG@DI-UM");
		

// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	init();	

// enter GLUT's main cycle
	glutMainLoop();
	
	return 0;
}
