#include <stdlib.h>
#include "tinyxml2.h"
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
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
float ux = 0.0, uy = 1.0, uz = 0.0;

// Camara Settings
float near = 0.0, far = 0.0, fov = 0.0;

// Struct que armazena as transformações
struct transformacao {
	string nome;
	float x;
	float y;
	float z;
	float angulo; // para rotates
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

	while(group != NULL){
		t = transf_conj; // t igualado ao vetor inicial a cada iteração

		XMLElement* transform = group->FirstChildElement("transform");
		if(transform){
			XMLElement* tipo = transform->FirstChildElement(); // sem parametros retorna um qualquer 
															   // pode ser "translate", "scale" ou "rotate"
			while(tipo != NULL){
				string nome = string(tipo->Name());
				trans.nome = nome;
				trans.x = atof(tipo->Attribute("x"));
				trans.y = atof(tipo->Attribute("y"));
				trans.z = atof(tipo->Attribute("z"));

				if (nome == "translate" || nome == "scale") {
					trans.angulo = 0;	// translate e scale não recebem parametro angulo
				}
				else if (nome == "rotate") {
					trans.angulo = atof(tipo->Attribute("angle")); // atof -> string para float
				}

				t.push_back(trans); // Armazena a translação lida 
				tipo = tipo->NextSiblingElement(); // avança para a proxima transicao -> NextSibling
			}
		} 

		XMLElement* models = group->FirstChildElement("models");
		if(models){
			XMLElement* model = models->FirstChildElement("model");
			while(model != NULL){
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
void parser_xml(const char* path){

	// Invoca o tinyxml2 para poder usar livremente as suas funções
	using namespace tinyxml2;

	// abrir o ficheiro .xml
	XMLDocument xml_file;
	xml_file.LoadFile(path);

	XMLElement* world = xml_file.FirstChildElement("world");

	if(world){
		XMLElement* window = world->FirstChildElement("window");
		if(window){
			altura = window->IntAttribute("width");
			largura = window->IntAttribute("height");
		}
		else {
			std::cout << "<window> nao encontrado." << std::endl;
		}

		XMLElement* camera = world->FirstChildElement("camera");
		if(camera){
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
			if(up){
				ux = up->FloatAttribute("x");
				uy = up->FloatAttribute("y");
				uz = up->FloatAttribute("z");
			}
			else {
				std::cout << "<up> nao encontrado." << std::endl;
			}

			XMLElement* projection = camera->FirstChildElement("projection");
			if(projection){
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
		if(group){
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

void renderScene(void) {
	// clear buffers
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
			if (objetos[i].transformacoes[j].nome == "scale") {
				glScalef(objetos[i].transformacoes[j].x, objetos[i].transformacoes[j].y, objetos[i].transformacoes[j].z);
			}
			if (objetos[i].transformacoes[j].nome == "rotate") {
				glRotatef(objetos[i].transformacoes[j].angulo, objetos[i].transformacoes[j].x, objetos[i].transformacoes[j].y, objetos[i].transformacoes[j].z);
			}
		}

		// após as transformacoes de cada objeto, o seu desenho
		glBegin(GL_TRIANGLES);

			for (int k = 0; k < objetos[i].pontos.size(); k += 9) {
				glColor3f(1.0f, 1.0f, 1.0f); // branco
				glVertex3f(objetos[i].pontos[k], objetos[i].pontos[k + 1], objetos[i].pontos[k + 2]);
				glVertex3f(objetos[i].pontos[k + 3], objetos[i].pontos[k + 4], objetos[i].pontos[k + 5]);
				glVertex3f(objetos[i].pontos[k + 6], objetos[i].pontos[k + 7], objetos[i].pontos[k + 8]);
			}

		glEnd();

	glPopMatrix();
	}
	glutSwapBuffers();
}

// funcao q move a pos da camara
void movimento(unsigned char key, int x, int y){
	if (key == 'w') {
		px += 1;
	}
	else if(key == 's'){
		px -= 1;
	}
	else if(key == 'n'){
		py += 1;
	}
	else if(key == 'm'){
		py -= 1;
	}
	else if(key == 'd'){
		pz += 1;
	}
	else if(key == 'a'){
		pz -= 1;
	}
}

// funcao que altera o ponto que a camara esta a olhar
void movimentoSpecialKeys(int key, int x, int y) {
	if(key == GLUT_KEY_UP){
		ly += 1;
	}
	else if(key == GLUT_KEY_DOWN){
		ly -= 1;
	}
	else if (key == GLUT_KEY_RIGHT) {
		lx += 1;
	}
	else if(key == GLUT_KEY_LEFT){
		lx -= 1;
	}
}


int main(int argc, char** argv) {

	int in;
	const char* file = nullptr;

	std::cout << "Selecione o ficheiro a correr:\n";
	std::cout << "1. test_2_1.xml\n";
	std::cout << "2. test_2_2.xml\n";
	std::cout << "3. test_2_3.xml\n";
	std::cout << "4. test_2_4.xml\n";
	std::cout << "5. sistema_solar.xml\n";
	std::cout << "Opcao: ";
	std::cin >> in;

	if (in == 1)
		file = "../test_files/test_files_phase_2/test_2_1.xml";
	else if (in == 2)
		file = "../test_files/test_files_phase_2/test_2_2.xml";
	else if (in == 3)
		file = "../test_files/test_files_phase_2/test_2_3.xml";
	else if (in == 4)
		file = "../test_files/test_files_phase_2/test_2_4.xml";
	else if (in == 5)
		file = "../test_files/test_files_phase_2/sistema_solar.xml";

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
	*/

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

	// enter GLUT's main cycle
	glutMainLoop();

	return 0;
}