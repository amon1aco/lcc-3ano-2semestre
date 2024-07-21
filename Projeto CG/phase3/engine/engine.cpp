#include <stdlib.h>
#include "tinyxml2.h"
#include <string>
#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <iostream>

#define NULL nullptr

using namespace tinyxml2;
using namespace std;

// Altura e Largura da janela
float altura = 0.0, largura = 0.0;

// Camara pos
float px = 0.0, py = 0.0, pz = 0.0;

// LookAt point
float lx = 0.0, ly = 0.0, lz = 0.0;

// Up vector
float ux = 0.0, uy = 1.0, uz = 0.0;				// Valores default

// Camara Settings
float near = 1.0, far = 1000.0, fov = 60.0;		// Valores default

// Fps
int timebase = 0, frame = 0;				

// VBOs
const int objs = 20;
GLuint VBOBuffer[objs], VBOBuffCount[objs];		

// Semelhante ao Generator.cpp
struct Vertices {	// Estrutura que armazena um vetor de floats, para armazenar pontos de controlo
	vector <float> cord;
};

// Struct que armazena as transformações
struct transformacao {
	string nome;
	float x;
	float y;
	float z;
	float angulo;				// para rotates
	float tempo = 0;
	float tempo_atual = 0;
	string orientado;			// Se o objecto deve ser orientado na curva (true ou false), align
	vector <Vertices> coord;	// pontos de control
	float t = 0;				// t de percentagem da reta
};

// Struct que guarda os pontos para o desenho da figura e as transformações que estes vão sofrer
struct figura {
	vector <float> pontos;
	vector <transformacao> transformacoes;
};

// Vetor que guarda todos as figuras
vector <figura> objetos;


void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too 
	if (h == 0)
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
	gluPerspective(fov, ratio, near, far);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void removerLinhasComentario(const std::string& file_in) {
	std::ifstream arquivoEntrada(file_in);
	std::vector<std::string> linhas;

	if (!arquivoEntrada.is_open()) {
		std::cout << "Erro ao abrir o arquivo de entrada." << std::endl;
		return;
	}

	std::string linha;
	while (std::getline(arquivoEntrada, linha)) {
		// Verifica se a linha começa com "#"
		if (linha.empty() || linha[0] != '#') {
			linhas.push_back(linha);
		}
	}
	// Fecha o arquivo de entrada
	arquivoEntrada.close();

	// E volta a abrir para escrever
	std::ofstream arquivoSaida(file_in); // Aberto para escrita, todo o seu conteudo anterior é eliminado
	if (!arquivoSaida.is_open()) {
		std::cout << "Erro ao abrir o arquivo de saída." << std::endl;
		return;
	}

	// Escreve as linhas no arquivo sem as linhas de comentário
	for (const std::string& l : linhas) {
		arquivoSaida << l << std::endl;
	}
	// Fecha o arquivo de entrada reaberto, mas alterado
	arquivoSaida.close();
}


// Agora retorna os pontos respetivos de cada figura
vector <float> readFile(const char* f) {
	// Remove todas as linhas "comentadas" no ficheiro gerado pelo generator.cpp
	removerLinhasComentario(f);

	vector <float> Pontos; // vetor de retorno

	ifstream file(f);
	if (!file.is_open()) {
		std::cout << "Falha ao abrir o arquivo" << std::endl;
		return Pontos;
	}

	std::cout << "Arquivo aberto com sucesso" << std::endl;

	float p1, p2, p3;
	// Lê enquanto tiver dados para ler do arquivo
	while (file >> p1 >> p2 >> p3) {
		// Adiciona no final do vetor
		Pontos.push_back(p1);
		Pontos.push_back(p2);
		Pontos.push_back(p3);
	}

	// Fecha o ficheiro
	file.close();

	return Pontos;
}

// Função que trata do parse XML para os grupos
// Vetor t inicialmente encontra-se vazio
void parser_group(tinyxml2::XMLElement* group, vector <transformacao> t) {

	vector <transformacao> transf_conj = t;	// Cópia do vetor inicial como backup
	transformacao trans;
	figura fig;

	while (group != NULL) {
		t = transf_conj; // t igualado ao vetor inicial a cada iteração

		XMLElement* transform = group->FirstChildElement("transform");
		if (transform) {
			XMLElement* tipo = transform->FirstChildElement(); // sem parametros retorna um qualquer 
			// pode ser "translate", "scale" ou "rotate"
			while (tipo != NULL) {
				string nome = string(tipo->Name());

				if(nome == "translate") {
					// Caso seja translate mas sem Time, temos uma translação normal
					if (tipo->Attribute("time") == nullptr) {
						trans.nome = nome;
						trans.angulo = 0;
						trans.x = atof(tipo->Attribute("x"));
						trans.y = atof(tipo->Attribute("y"));
						trans.z = atof(tipo->Attribute("z"));
					}
					// Caso tenha Time, temos uma "timed_translate"
					else {
						trans.nome = "timed_translate";
						trans.angulo = 0;
						trans.tempo = atof(tipo -> Attribute("time")) * 1000; 
						trans.orientado = tipo -> Attribute("align");	// Verifica se é uma translação orientada ou não, true or false
						
						trans.coord = {}; // Vetor vazio
						for (XMLElement* pc = tipo -> FirstChildElement("point"); pc != NULL; pc = pc -> NextSiblingElement()) {
							Vertices p;
							p.cord.push_back(atof(pc -> Attribute("x")));
							p.cord.push_back(atof(pc -> Attribute("y")));
							p.cord.push_back(atof(pc -> Attribute("z")));
							trans.coord.push_back(p); // Coloca os pontos de controlo no vetor "coord" da transformação
						}
					}

				} 
				else if(nome == "scale"){
					trans.nome = nome;
					trans.angulo = 0;
					trans.x = atof(tipo -> Attribute("x"));
					trans.y = atof(tipo -> Attribute("y"));
					trans.z = atof(tipo -> Attribute("z"));
				}

				else if(nome == "rotate"){
					// Caso se trate de uma rotação sem parâmetro "time" temos uma rotação normal
					if (tipo->Attribute("time") == nullptr) {
						trans.nome = nome;
						trans.angulo = atof(tipo -> Attribute("angle"));
						trans.x = atof(tipo -> Attribute("x"));
						trans.y = atof(tipo -> Attribute("y"));
						trans.z = atof(tipo -> Attribute("z"));
					}
					// Caso haja parâmetro time, temos uma "timed_rotation"
					else {
						trans.nome = "timed_rotation";
						trans.tempo = atof(tipo -> Attribute("time")) * 1000;
						trans.x = atof(tipo -> Attribute("x"));
						trans.y = atof(tipo -> Attribute("y"));
						trans.z = atof(tipo -> Attribute("z"));
					}
				}
				t.push_back(trans); // Armazena a translação lida 
				tipo = tipo->NextSiblingElement(); // avança para a proxima transicao -> NextSibling
			}
		}

		XMLElement* models = group->FirstChildElement("models");
		if(models){
			XMLElement* model = models->FirstChildElement("model");
			while (model != NULL) {
				char* filename = (char*)model->Attribute("file"); // buscar o nome do ficheiro e converter para (char*)
				fig.pontos = readFile(filename);
				model = model->NextSiblingElement(); // NextSibling 
			}
			fig.transformacoes = t; // guarda o vetor das transformacoes na struct
			objetos.push_back(fig); // guarda a figura no vetor das figuras
		}

		// Chamada recursiva para processar os grupos filhos
		parser_group(group->FirstChildElement("group"), t); // passada com as transformacoes atuais pois são comulativas para subgrupos
		group = group->NextSiblingElement("group");
	}
}

// Função que vai ler e intrepretar o ficheiro .XML
void parser_xml(const char* path) {

	// Invoca o tinyxml2 para poder usar livremente as suas funções
	using namespace tinyxml2;

	// abrir o ficheiro .xml
	XMLDocument xml_file;
	xml_file.LoadFile(path);

	XMLElement* world = xml_file.FirstChildElement("world");

	if (world) {
		XMLElement* window = world->FirstChildElement("window");
		if (window) {
			altura = window->IntAttribute("width");
			largura = window->IntAttribute("height");
		}
		else {
			std::cout << "<window> nao encontrado." << std::endl;
		}

		XMLElement* camera = world->FirstChildElement("camera");
		if (camera) {
			XMLElement* position = camera->FirstChildElement("position");
			if (position) {
				px = position->FloatAttribute("x");
				py = position->FloatAttribute("y");
				pz = position->FloatAttribute("z");
			}
			else {
				std::cout << "<position> nao encontrado." << std::endl;
			}

			XMLElement* lookat = camera->FirstChildElement("lookAt");
			if (lookat) {
				lx = lookat->FloatAttribute("x");
				ly = lookat->FloatAttribute("y");
				lz = lookat->FloatAttribute("z");
			}
			else {
				std::cout << "<lookAt> nao encontrado." << std::endl;
			}

			XMLElement* up = camera->FirstChildElement("up");
			if (up) {
				ux = up->FloatAttribute("x");
				uy = up->FloatAttribute("y");
				uz = up->FloatAttribute("z");
			}
			else {
				std::cout << "<up> nao encontrado." << std::endl;
			}

			XMLElement* projection = camera->FirstChildElement("projection");
			if (projection) {
				fov = projection->FloatAttribute("fov");
				near = projection->FloatAttribute("near");
				far = projection->FloatAttribute("far");
			}
			else {
				std::cout << "<projection> nao encontrado." << std::endl;
			}
		}
		else {
			std::cout << "<camera> nao encontrado." << std::endl;
		}

		XMLElement* group = world->FirstChildElement("group");
		if (group) {
			// caso haja elemento <group> então
			vector <transformacao> transf;		// cria um vetor de transicoes vazio
			parser_group(group, transf);		// chama a funcao parser_group com o respetivo grupo e o vetor criado
		}
		else {
			std::cout << "<group> nao encontrado." << std::endl;
		}
	}
	else {
		std::cout << "<world> nao encontrado." << std::endl;
	}
}


// Funções estudadas nas aulas práticas
// Cria uma matriz de rotação
void buildRotMatrix(float* x, float* y, float* z, float* M) { // 4X4
	M[0] = x[0]; M[1] = x[1]; M[2] = x[2]; M[3] = 0;
	M[4] = y[0]; M[5] = y[1]; M[6] = y[2]; M[7] = 0;
	M[8] = z[0]; M[9] = z[1]; M[10] = z[2]; M[11] = 0;
	M[12] = 0; M[13] = 0; M[14] = 0; M[15] = 1;
}

// Produto vetorial entre "v" e "u"
void cross(float* v, float* u, float* res) {
	res[0] = v[1] * u[2] - v[2] * u[1];
	res[1] = v[2] * u[0] - v[0] * u[2];
	res[2] = v[0] * u[1] - v[1] * u[0];
}

// Normaliza o vetor "v", torna o seu tamanho ==1
void normalize(float* v) {
	float len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] = v[0] / len;
	v[1] = v[1] / len;
	v[2] = v[2] / len;
}

// multiplica a matriz "M" pelo vetor "v"
void multMatrixVector(float* M, float* v, float* res) {
	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * M[j * 4 + k];
		}
	}
}

// calcula um ponto na curva Catmull-Rom para um parâmetro t (entre 0 e 1)
// recebe 4 pontos de control, p0,p1,p2,p3
void getCatmullRomPoint(float t, vector<float> p0, vector<float> p1, vector<float>  p2, vector<float> p3, float* pos, float* deriv) {
	// matriz Catmull-Rom
	float m[4][4] = {{-0.5f, 1.5f, -1.5f,  0.5f},
					 {1.0f, -2.5f,  2.0f, -0.5f},
					{-0.5f,  0.0f,  0.5f,  0.0f},
					 {0.0f,  1.0f,  0.0f,  0.0f}};

	for (int i = 0; i < 3; i++){ // Para x,y e z
		float A[4];
		float temp[] = {p0[i], p1[i], p2[i], p3[i]};

		// Calcula A = M * P
		multMatrixVector((float*)m, temp, A);

		// Calcula pos = T * A
		pos[i] = pow(t,3) * A[0] + pow(t,2) * A[1] + t * A[2] + A[3];

		// Calcula deriv = T' * A
		deriv[i] = 3 * pow(t,2) * A[0] + 2 * t * A[1] + A[2];
	}
}

// retorna um ponto global na curva Catmull-Rom com base no t global
void getGlobalCatmullRomPoint(float gt, float* pos, float* deriv, vector <Vertices> coord) {
	const int POINT_COUNT = coord.size();

	float t = gt * POINT_COUNT;  // Este é o t global real
	int index = floor(t);		 // Determina em qual segmento estamos
	t = t - index;				 // Determina onde dentro do segmento estamos

	// Índices armazenam os pontos de controle
	int indices[4];
	indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
	indices[1] = (indices[0] + 1) % POINT_COUNT;
	indices[2] = (indices[1] + 1) % POINT_COUNT;
	indices[3] = (indices[2] + 1) % POINT_COUNT;

	// Obtém o ponto Catmull-Rom para o parâmetro local t
	getCatmullRomPoint(t, coord[indices[0]].cord, coord[indices[1]].cord, coord[indices[2]].cord, coord[indices[3]].cord, pos, deriv);
}

// Renderiza a curva Catmull-Rom 
void renderCatmullRomCurve(vector <Vertices> coord) {
	// Desenha a curva usando segmentos de linha com GL_LINE_LOOP
	float pos[3], deriv[3];
	float LINE_SEGMENTS = 100;

	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < LINE_SEGMENTS; i++){
		// Obtém um ponto na curva Catmull-Rom para um determinado t global
		getGlobalCatmullRomPoint(1 / LINE_SEGMENTS * i, pos, deriv, coord);

		// Desenha o ponto na curva
		glVertex3f(pos[0], pos[1], pos[2]);
	}
	glEnd();
}

void renderScene(void) {
	// clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT, GL_LINE);

	// set camera
	glLoadIdentity();
	gluLookAt(px, py, pz,
			  lx, ly, lz,
			  ux, uy, uz);

	// Desenho dos Eixos
	glBegin(GL_LINES);
	// Eixo X é vermelho
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-500.0f, 0.0f, 0.0f); // aumentado de 100 pra 500
	glVertex3f(500.0f, 0.0f, 0.0f);

	// Eixo Y é verde
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -500.0f, 0.0f);
	glVertex3f(0.0f, 500.0f, 0.0f);

	// Eixo Z é azul
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -500.0f);
	glVertex3f(0.0f, 0.0f, 500.0f);

	// Coloca a cor de desenho novamente a branco
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();

	glutPostRedisplay();

	// Desenho das imagens
	for (int i = 0; i < objetos.size(); i++) { // para cada objeto

		glPushMatrix();
		for (int j = 0; j < objetos[i].transformacoes.size(); j++) { // sobre cada transformação
			if (objetos[i].transformacoes[j].nome == "translate") {
				glTranslatef(objetos[i].transformacoes[j].x, objetos[i].transformacoes[j].y, objetos[i].transformacoes[j].z);
			}
			else if (objetos[i].transformacoes[j].nome == "timed_translate") {
				float pos[3], deriv[3];											// Posicao e Derivada na curva
				renderCatmullRomCurve(objetos[i].transformacoes[j].coord);		// desenha a curva
				getGlobalCatmullRomPoint(objetos[i].transformacoes[j].t, pos, deriv, objetos[i].transformacoes[j].coord); // obtem a posicao e a derivada atuais
				glTranslatef(pos[0], pos[1], pos[2]);							// aplica a translação

				// se for orientada, vai calcular a rotação do objeto com base nas derivadas da posicao da curva
				if (objetos[i].transformacoes[j].orientado == "true"){
					// Vetor x é definido como a derivada da curva
					float x[3] = {deriv[0], deriv[1], deriv[2]};
					// Vetores y e z serão calculados a partir do vetor x
					float y[3];
					float z[3];
					float mat[16];				// Matriz de rotação para orientação
					float prev_y[3] = {0,1,0};  // Vetor de orientação y anterior

					// normaliza x e calcula z apartir de x.prev_y
					normalize(x);
					cross(x, prev_y, z);
					// normaliza z e calcula y apartir de z.x
					normalize(z);
					cross(z, x, y);

					// normaliza y
					normalize(y);

					// constroi a matriz de rotação
					buildRotMatrix(x, y, z, mat);
					// multiplica a matriz de gerada pela matriz atual de transformação
					glMultMatrixf(mat);		
				}

				// Atualiza o tempo atual para uso na próxima iteração
				float new_time = glutGet(GLUT_ELAPSED_TIME);
				// Calcula a diferença de tempo desde a última atualização
				float diff = new_time - objetos[i].transformacoes[j].tempo_atual;
				// Atualiza o parâmetro t da curva Catmull-Rom com base na diferença de tempo
				objetos[i].transformacoes[j].t += diff / objetos[i].transformacoes[j].tempo;
				// Atualiza o tempo atual
				objetos[i].transformacoes[j].tempo_atual = new_time;
			}
			else if (objetos[i].transformacoes[j].nome == "scale") {
				glScalef(objetos[i].transformacoes[j].x, objetos[i].transformacoes[j].y, objetos[i].transformacoes[j].z);
			}
			else if (objetos[i].transformacoes[j].nome == "rotate") {
				glRotatef(objetos[i].transformacoes[j].angulo, objetos[i].transformacoes[j].x, objetos[i].transformacoes[j].y, objetos[i].transformacoes[j].z);
			}
			else if (objetos[i].transformacoes[j].nome == "timed_rotation") {
				// calcula o angulo de rotação com base no tempo decorrido desde o inicio da execução do programa
				glRotatef(glutGet(GLUT_ELAPSED_TIME) * 360 / objetos[i].transformacoes[j].tempo, objetos[i].transformacoes[j].x, objetos[i].transformacoes[j].y, objetos[i].transformacoes[j].z);
			} 
		}

		// após as transformacoes de cada objeto, o seu desenho
		// desenho dos triangulos usando VBOs
		glBindBuffer(GL_ARRAY_BUFFER, VBOBuffer[i]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, VBOBuffCount[i]);

		glPopMatrix();
	}
	
	// FPS
	int timer = glutGet(GLUT_ELAPSED_TIME);					// Variável para armazenar o tempo atual em milissegundos
	int fps;
	char str[64];											// String para armazenar o texto a ser exibido na janela

	frame++;
	if(timer - timebase > 1000){								// Se passou mais de 1 segundo desde a última atualização
		// calcula o valor dos fps
		fps = frame * 1000.0 / (timer - timebase);
		timebase = timer;
		frame = 0;
		sprintf(str, "FPS: %d", fps);
		// define o título da janela com o valor do FPS
		glutSetWindowTitle(str);
	}

	glutSwapBuffers();
}

// funcao q move a pos da camara
void movimento(unsigned char key, int x, int y) {
	if (key == 'w') {
		px += 1;
	}
	else if (key == 's') {
		px -= 1;
	}
	else if (key == 'n') {
		py += 1;
	}
	else if (key == 'm') {
		py -= 1;
	}
	else if (key == 'd') {
		pz += 1;
	}
	else if (key == 'a') {
		pz -= 1;
	}
}

// funcao que altera o ponto que a camara esta a olhar
void movimentoSpecialKeys(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		ly += 1;
	}
	else if (key == GLUT_KEY_DOWN) {
		ly -= 1;
	}
	else if (key == GLUT_KEY_RIGHT) {
		lx += 1;
	}
	else if (key == GLUT_KEY_LEFT) {
		lx -= 1;
	}
}


int main(int argc, char** argv) {

	int in;
	const char* file = nullptr;

	std::cout << "Selecione o ficheiro a correr:\n";
	std::cout << "1. test_3_1.xml\n";
	std::cout << "2. test_3_2.xml\n";
	std::cout << "3. sistema_solar.xml\n";
	std::cout << "Opcao: ";
	std::cin >> in;

	if (in == 1)
		file = "../test_files/test_files_phase_3/test_3_1.xml";
	else if (in == 2)
		file = "../test_files/test_files_phase_3/test_3_2.xml";
	else if (in == 3)
		file = "../test_files/test_files_phase_3/sistema_solar.xml";	

	/*
	else if (in == 6)
		file = "../test_files/test_files_phase_1/test_1_1.xml";
	else if (in == 7)
		file = "../test_files/test_files_phase_1/test_1_2.xml";
	else if (in == 8)
		file = "../test_files/test_files_phase_1/test_1_3.xml";
	else if (in == 9)
		file = "../test_files/test_files_phase_1/test_1_4.xml";
	else if (in == 10)
		file = "../test_files/test_files_phase_1/test_1_5.xml"; // abre 2 ficheiros no mesmo grupo, apenas desenha o ultimo..
	
	else if (in == 11)
		file = "../test_files/test_files_phase_2/test_2_1.xml";
	else if (in == 12)
		file = "../test_files/test_files_phase_2/test_2_2.xml";
	else if (in == 13)
		file = "../test_files/test_files_phase_2/test_2_3.xml";
	else if (in == 14)
		file = "../test_files/test_files_phase_2/test_2_4.xml";
	else if (in == 15)
		file = "../test_files/test_files_phase_2/sistema_solar.xml"; */
	
	else {
		std::cout << "Opcao Invalida\n";
		return 1;
	}
	parser_xml(file);

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(altura, largura);
	glutCreateWindow("ProjetoCG");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	// put here the registration of the keyboard callbacks
	glutKeyboardFunc(movimento);
	glutSpecialFunc(movimentoSpecialKeys);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnableClientState(GL_VERTEX_ARRAY);	

	glewInit();

	glGenBuffers(objetos.size(), VBOBuffer);

	for (int i = 0; i < objetos.size(); i++) {
		VBOBuffCount[i] = objetos[i].pontos.size() / 3;
		glBindBuffer(GL_ARRAY_BUFFER, VBOBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, objetos[i].pontos.size() * sizeof(float), objetos[i].pontos.data(), GL_STATIC_DRAW);
	}

	// enter GLUT's main cycle
	glutMainLoop();

	return 0;
}