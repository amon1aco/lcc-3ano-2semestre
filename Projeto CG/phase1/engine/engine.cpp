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

#define NULL nullptr

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

// Array (Vetor) que vai armazenar todos os pontos que precisam de serem criados
std::vector <float> Pontos;

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

void readFile(const char* f) {
	// Remove todas as linhas "comentadas" no ficheiro gerado pelo generator.cpp
	removerLinhasComentario(f);

	std::ifstream file(f);
	if (!file.is_open()) {
		std::cout << "Falha ao abrir o arquivo" << std::endl;
		return;
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
}


void renderScene(void){

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT, GL_LINE);

	// set camera
	glLoadIdentity();

	// LookAt da camara
	gluLookAt(px,py,pz,
			  lx, ly,lz,
			  ux, uy, uz);
	
	// Desenho dos Eixos
    glBegin(GL_LINES);
		// Eixo X é vermelho
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f(100.0f, 0.0f, 0.0f);

		// Eixo Y é verde
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);

		// Eixo Z é azul
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);

		// Coloca a cor de desenho novamente a branco
		glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();

	// Desenho da Figura, acedendo ao vetor Pontos[]
	glBegin(GL_TRIANGLES);
		for (int i = 0; i < Pontos.size(); i += 9) {
			glVertex3f(Pontos[i], Pontos[i+1], Pontos[i+2]);
			glVertex3f(Pontos[i+3], Pontos[i+4], Pontos[i+5]);
			glVertex3f(Pontos[i+6], Pontos[i+7], Pontos[i+8]);
		} 
	glEnd(); 

	glutSwapBuffers();
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
		} else {
			std::cout << "<window> nao encontrado." << std::endl;
		}

		XMLElement* camera = world->FirstChildElement("camera");
		if (camera) {
			XMLElement* position = camera->FirstChildElement("position");
			if (position) {
				px = position->FloatAttribute("x");
				py = position->FloatAttribute("y");
				pz = position->FloatAttribute("z");
			} else {
				std::cout << "<position> nao encontrado." << std::endl;
			}

			XMLElement* lookat = camera->FirstChildElement("lookAt");
			if (lookat) {
				lx = lookat->FloatAttribute("x");
				ly = lookat->FloatAttribute("y");
				lz = lookat->FloatAttribute("z");
			} else {
				std::cout << "<lookAt> nao encontrado." << std::endl;
			}

			XMLElement* up = camera->FirstChildElement("up");
			if (up) {
				ux = up->FloatAttribute("x");
				uy = up->FloatAttribute("y");
				uz = up->FloatAttribute("z");
			} else {
				std::cout << "<up> nao encontrado." << std::endl;
			}

			XMLElement* projection = camera->FirstChildElement("projection");
			if (projection) {
				fov = projection->FloatAttribute("fov");
				near = projection->FloatAttribute("near");
				far = projection->FloatAttribute("far");
			} else {
				std::cout << "<projection> nao encontrado." << std::endl;
			}
		} else {
			std::cout << "<camera> nao encontrado." << std::endl;
		}

		XMLElement* group = world->FirstChildElement("group");
		if (group) {
			XMLElement* models = group->FirstChildElement("models");
			if (models) {
				XMLElement* model = models->FirstChildElement("model");

				while (model != nullptr) {
					const char* filename = model->Attribute("file");
					if (filename) {
						//std::cout << "dentro\n" << filename << endl;
						readFile(filename);
					}
					else {
						std::cout << "Filename nao encontrado." << std::endl;
					}
					model = model->NextSiblingElement();
				}
			} else {
				std::cout << "<models> nao encontrado." << std::endl;
			}
		} else {
			std::cout << "<group> nao encontrado." << std::endl;
		}
	} else {
		std::cout << "<world> nao encontrado." << std::endl;
	}
}


int main(int argc, char** argv) {

	int in;
	const char* file = nullptr;

	std::cout << "Selecione o ficheiro a correr:\n";
	std::cout << "1. test_1_1.xml\n";
	std::cout << "2. test_1_2.xml\n";
	std::cout << "3. test_1_3.xml\n";
	std::cout << "4. test_1_4.xml\n";
	std::cout << "5. test_1_5.xml\n";
	std::cout << "Opcao: ";
	std::cin >> in;

	if (in == 1)
		file = "../test_files/test_files_phase_1/test_1_1.xml";
	else if (in == 2)
		file = "../test_files/test_files_phase_1/test_1_2.xml";
	else if (in == 3)
		file = "../test_files/test_files_phase_1/test_1_3.xml";
	else if (in == 4)
		file = "../test_files/test_files_phase_1/test_1_4.xml";
	else if (in == 5)
		file = "../test_files/test_files_phase_1/test_1_5.xml";
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

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// enter GLUT's main cycle
	glutMainLoop();

	return 0;
}