#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>

using namespace std;

struct Vertices {
    vector <float> cord;
};


// Função que recebe uma string de texto e escreve esse "texto" no ficheiro
void write_file(std::string file_name, std::string texto) {
    //std::ofstream file(file_name);
    std::ofstream file("../../../engine/build/" + file_name);
    if (file.is_open()) {
        //std::cout << "Ficheiro Aberto" << std::endl;
        file << texto << std::endl;
        file.close();
    }
    else {
        std::cout << "Nao foi possivel abrir o arquivo: " << file_name << std::endl;
    }
}

// Função que desenha um plano
void planeGenerator(float comprimento, float divisoes, std::string file_name) {
    // string que vai armazenar os pontos em formato string para escrever no file
    std::stringstream str;

    // x_inicial negativo para centralizar o plano na origem dos eixos
    float x_inicial = -(comprimento / 2.0f);
    float comprimento_lado = comprimento / divisoes;
    float x1, x2, z1, z2, y;

    // Calculo do nr de pontos
    int total_pontos = divisoes * divisoes * 6;
    str << "# Plane Generator\n";
    str << "# Total de Pontos: " << total_pontos << "\n";

    // Eixo dos y sempre a 0
    y = 0;

    for (int i = 0; i < divisoes; i++) {
        for (int j = 0; j < divisoes; j++) {
            x1 = x_inicial + i * comprimento_lado;
            x2 = x_inicial + (i + 1) * comprimento_lado;

            z1 = x_inicial + j * comprimento_lado;
            z2 = x_inicial + (j + 1) * comprimento_lado;

            // triangulo 1
            str << x1 << ' ' << y << ' ' << z1 << '\n';
            str << x1 << ' ' << y << ' ' << z2 << '\n';
            str << x2 << ' ' << y << ' ' << z2 << '\n';

            // triangulo 2
            str << x2 << ' ' << y << ' ' << z1 << '\n';
            str << x1 << ' ' << y << ' ' << z1 << '\n';
            str << x2 << ' ' << y << ' ' << z2 << '\n';
        }
    }
    write_file(file_name, str.str());
}

// Função que gera o ficheiro com os pontos para uma Box
void boxGeneretor(float comprimento, float divisoes, std::string file_name) {
    // String que vai ser escrita no file
    std::stringstream str;

    float altura = comprimento / 2.0;
    // x_inicial == -altura, para centralizar a boxe na origem dos eixos
    float x_inicial = -altura;
    float comprimento_lado = comprimento / divisoes;
    float x1, x2, y1, y2, z1, z2;

    // Calculo do nr de pontos
    int total_pontos = divisoes * divisoes * 12;
    str << "# Box Generator\n";
    str << "# Total de Pontos: " << total_pontos << "\n";

    y1 = altura;
    y2 = -altura;
    // Cadeia de For's para face superior e inferior
    for (int i = 0; i < divisoes; i++) {
        for (int j = 0; j < divisoes; j++) {

            x1 = x_inicial + i * comprimento_lado;
            x2 = x_inicial + (i + 1) * comprimento_lado;

            z1 = x_inicial + j * comprimento_lado;
            z2 = x_inicial + (j + 1) * comprimento_lado;


            // Face superior
            // Triangulo 1
            str << x1 << ' ' << y1 << ' ' << z1 << '\n';
            str << x1 << ' ' << y1 << ' ' << z2 << '\n';
            str << x2 << ' ' << y1 << ' ' << z2 << '\n';

            // Triangulo 2
            str << x2 << ' ' << y1 << ' ' << z1 << '\n';
            str << x1 << ' ' << y1 << ' ' << z1 << '\n';
            str << x2 << ' ' << y1 << ' ' << z2 << '\n';

            // Face inferior
            // Triangulo 1
            str << x1 << ' ' << y2 << ' ' << z1 << '\n';
            str << x2 << ' ' << y2 << ' ' << z2 << '\n';
            str << x1 << ' ' << y2 << ' ' << z2 << '\n';

            // Triangulo 2
            str << x2 << ' ' << y2 << ' ' << z1 << '\n';
            str << x2 << ' ' << y2 << ' ' << z2 << '\n';
            str << x1 << ' ' << y2 << ' ' << z1 << '\n';
        }
    }

    z1 = altura;
    z2 = -altura;
    // Cadeia de For's para face esquerda e direita
    for (int i = 0; i < divisoes; i++) {
        for (int j = 0; j < divisoes; j++) {
            x1 = x_inicial + i * comprimento_lado;
            x2 = x_inicial + (i + 1) * comprimento_lado;

            y1 = x_inicial + j * comprimento_lado;
            y2 = x_inicial + (j + 1) * comprimento_lado;

            // Face esquerda
            // triangulo 1
            str << x1 << ' ' << y2 << ' ' << z1 << '\n';
            str << x1 << ' ' << y1 << ' ' << z1 << '\n';
            str << x2 << ' ' << y1 << ' ' << z1 << '\n';

            // triangulo 2
            str << x2 << ' ' << y2 << ' ' << z1 << '\n';
            str << x1 << ' ' << y2 << ' ' << z1 << '\n';
            str << x2 << ' ' << y1 << ' ' << z1 << '\n';

            // Face direita
            // triangulo 1
            str << x1 << ' ' << y2 << ' ' << z2 << '\n';
            str << x2 << ' ' << y1 << ' ' << z2 << '\n';
            str << x1 << ' ' << y1 << ' ' << z2 << '\n';

            // triangulo 2
            str << x2 << ' ' << y2 << ' ' << z2 << '\n';
            str << x2 << ' ' << y1 << ' ' << z2 << '\n';
            str << x1 << ' ' << y2 << ' ' << z2 << '\n';
        }
    }

    x1 = altura;
    x2 = -altura;
    // Cadeia de For's para face frontal e traseira
    for (int i = 0; i < divisoes; i++) {
        for (int j = 0; j < divisoes; j++) {
            y1 = x_inicial + i * comprimento_lado;
            y2 = x_inicial + (i + 1) * comprimento_lado;

            z1 = x_inicial + j * comprimento_lado;
            z2 = x_inicial + (j + 1) * comprimento_lado;

            // Face frontal
            // triangulo 1
            str << x1 << ' ' << y2 << ' ' << z2 << '\n';
            str << x1 << ' ' << y1 << ' ' << z2 << '\n';
            str << x1 << ' ' << y1 << ' ' << z1 << '\n';

            // triangulo 2
            str << x1 << ' ' << y2 << ' ' << z1 << '\n';
            str << x1 << ' ' << y2 << ' ' << z2 << '\n';
            str << x1 << ' ' << y1 << ' ' << z1 << '\n';

            // Face traseira
            // triangulo 1
            str << x2 << ' ' << y2 << ' ' << z2 << '\n';
            str << x2 << ' ' << y1 << ' ' << z1 << '\n';
            str << x2 << ' ' << y1 << ' ' << z2 << '\n';

            // triangulo 2
            str << x2 << ' ' << y2 << ' ' << z1 << '\n';
            str << x2 << ' ' << y1 << ' ' << z1 << '\n';
            str << x2 << ' ' << y2 << ' ' << z2 << '\n';

        }
    }
    write_file(file_name, str.str());
}

// Função que gera o ficheiro com os pontos para um Cone
void coneGenerator(float raio, float altura, float slices, float stacks, std::string file_name) {
    std::stringstream str;

    float x1, x2, z1, z2;
    float y = 0;
    float next_y;
    float raio_atual = raio;
    float next_raio;

    // Calculo do nr de pontos
    // Pontos = PontosBase + PontosLateral
    int total_pontos = (3 * slices) + (2 * 3 * slices * stacks);
    str << "# Cone Generator\n";
    str << "# Total de Pontos: " << total_pontos << "\n";

    // Base do cone
    for (int i = 0; i < slices; i++) {
        x1 = raio * sin(i * (2 * M_PI / slices));
        x2 = raio * sin((i + 1) * (2 * M_PI / slices));

        z1 = raio * cos(i * (2 * M_PI / slices));
        z2 = raio * cos((i + 1) * (2 * M_PI / slices));

        // Centrada na origem e com Y = 0
        str << 0 << ' ' << 0 << ' ' << 0 << '\n';    // Origem
        str << x2 << ' ' << 0 << ' ' << z2 << '\n';
        str << x1 << ' ' << 0 << ' ' << z1 << '\n';
    }

    // Lateral do cone
    float angulo = 0;
    for (int i = 0; i < stacks; i++) {

        next_y = y + (altura / stacks);
        next_raio = raio_atual - (raio / stacks);

        for (int j = 0; j < slices; j++) {

            // triangulo 1
            str << raio_atual * sin(angulo) << ' ' << y << ' ' << raio_atual * cos(angulo) << '\n';
            str << raio_atual * sin(angulo + (2 * M_PI / slices)) << ' ' << y << ' ' << raio_atual * cos(angulo + (2 * M_PI / slices)) << '\n';
            str << next_raio * sin(angulo) << ' ' << next_y << ' ' << next_raio * cos(angulo) << '\n';

            // triangulo 2
            str << next_raio * sin(angulo) << ' ' << next_y << ' ' << next_raio * cos(angulo) << '\n';
            str << raio_atual * sin(angulo + (2 * M_PI / slices)) << ' ' << y << ' ' << raio_atual * cos(angulo + (2 * M_PI / slices)) << '\n';
            str << next_raio * sin(angulo + (2 * M_PI / slices)) << ' ' << next_y << ' ' << next_raio * cos(angulo + (2 * M_PI / slices)) << '\n';

            angulo += (2 * M_PI / slices);
        }
        y = next_y;
        raio_atual = next_raio;
    }
    write_file(file_name, str.str());
}

// Função que gera o ficheiro com os pontos para uma Esfera
void sphereGenerator(int raio, int slices, int stacks, std::string file_name) {
    std::stringstream str;

    float divisao;
    float stack;
    float divisao_size = 2 * M_PI / slices;
    float stack_size = M_PI / stacks;

    // Calculo do nr de pontos
    int total_pontos = slices * stacks * 6;
    str << "# Sphere Generator\n";
    str << "# Total de Pontos: " << total_pontos << "\n";

    for (int i = 0; i < stacks; i++) {
        stack = stack_size * i;

        for (int j = 0; j < slices; j++) {
            divisao = divisao_size * j;

            // triangulo 1
            str << raio * sin(divisao) * sin(stack) << ' ' << raio * cos(stack) << ' ' << raio * sin(stack) * cos(divisao) << "\n";
            str << raio * sin(stack + stack_size) * sin(divisao + divisao_size) << ' ' << raio * cos(stack + stack_size) << ' ' << raio * sin(stack + stack_size) * cos(divisao + divisao_size) << "\n";
            str << raio * sin(stack) * sin(divisao + divisao_size) << ' ' << raio * cos(stack) << ' ' << raio * sin(stack) * cos(divisao + divisao_size) << "\n";

            // triangulo 2
            str << raio * sin(divisao) * sin(stack) << ' ' << raio * cos(stack) << ' ' << raio * sin(stack) * cos(divisao) << "\n";
            str << raio * sin(stack + stack_size) * sin(divisao) << ' ' << raio * cos(stack + stack_size) << ' ' << raio * sin(stack + stack_size) * cos(divisao) << "\n";
            str << raio * sin(stack + stack_size) * sin(divisao + divisao_size) << ' ' << raio * cos(stack + stack_size) << ' ' << raio * sin(stack + stack_size) * cos(divisao + divisao_size) << "\n";
        }
    }
    write_file(file_name, str.str());
}

// Função que gera o ficheiro com os pontos para um Torus (Donut)
void torusGenerator(int raio_menor, int raio_maior, int slices, int stacks, std::string file_name) {
    std::stringstream str;

    raio_menor = (raio_menor + raio_maior) / 2;
    raio_maior = raio_menor - raio_maior;

    float ang1 = (2 * M_PI) / stacks, ang2 = (2 * M_PI) / slices;

    // Calculo do nr de pontos
    int total_pontos = 2 * stacks * slices * 3;
    str << "# Torus Generator\n";
    str << "# Total de Pontos: " << total_pontos << "\n";

    float x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4;

    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {
            x1 = (raio_menor + raio_maior * cos(ang1*i)) * cos(ang2*j);
            x2 = (raio_menor + raio_maior * cos(ang1*(i+1))) * cos(ang2*j);
            x3 = (raio_menor + raio_maior * cos(ang1*(i+1))) * cos(ang2*(j+1));
            x4 = (raio_menor + raio_maior * cos(ang1*i)) * cos(ang2*(j+1));

            y1 = raio_maior * sin(ang1*i);
            y2 = raio_maior * sin(ang1*(i+1));
            y3 = raio_maior * sin(ang1*(i+1));
            y4 = raio_maior * sin(ang1*i);

            z1 = (raio_menor + raio_maior * cos(ang1*i)) * sin(ang2*j);
            z2 = (raio_menor + raio_maior * cos(ang1*(i+1))) * sin(ang2*j);
            z3 = (raio_menor + raio_maior * cos(ang1*(i+1))) * sin(ang2*(j+1));
            z4 = (raio_menor + raio_maior * cos(ang1*i)) * sin(ang2*(j+1));

            // Triangulo 1
            str << x1 << ' ' << y1 << ' ' << z1 << '\n';
            str << x2 << ' ' << y2 << ' ' << z2 << '\n';
            str << x4 << ' ' << y4 << ' ' << z4 << '\n';

            // Triangulo 2
            str << x2 << ' ' << y2 << ' ' << z2 << '\n';
            str << x3 << ' ' << y3 << ' ' << z3 << '\n';
            str << x4 << ' ' << y4 << ' ' << z4 << '\n';
        }
    }
    write_file(file_name, str.str());
}


void bezierGenerator(std::string patch, int subdivs, std::string file_name){
    
    //const int subdivs = 10;
    stringstream str;
    fstream f("../../" + patch);    // abrir o arquivo com os "patches

    if(f.is_open()){ // Caso o ficheiro tenha sido aberto com sucesso
        // estrutura do ficheiro patch estudada conforme o ficheiro "pdf" disponibilizado

        string l; // linha de leitura

        int nr_patches;                     // nr de patches, 1a linha
        int nr_control_points;              // nr de control points, (nr_patches+1)a linha

        vector <vector <int>> patch_indices;    // indice dos patch sob a forma de matriz
        vector <Vertices> pontos_de_controlo;   // armazena os pontos de controlo

        if(getline(f, l)){
            sscanf(l.c_str(), "%d\n", &nr_patches); // lê a primeira linha e atribui o valor para "nr_patches"
        }

        for(int i = 0; i < nr_patches; i++){ // Vai ler os indices para cada patch
            vector<int> indices; // vetor auxiliar para armazenar os indices de cada patch

            if(getline(f, l)){
                // cada linha tem 16 valores
                // leitura dos valores
                int i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, i16;
                sscanf(l.c_str(), "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", &i1, &i2, &i3, &i4, &i5, &i6, &i7, &i8, &i9, &i10, &i11, &i12, &i13, &i14, &i15, &i16);

                // passar os valores pro vetor "indices"
                indices.push_back(i1);
                indices.push_back(i2);
                indices.push_back(i3);
                indices.push_back(i4);
                indices.push_back(i5);
                indices.push_back(i6);
                indices.push_back(i7);
                indices.push_back(i8);
                indices.push_back(i9);
                indices.push_back(i10);
                indices.push_back(i11);
                indices.push_back(i12);
                indices.push_back(i13);
                indices.push_back(i14);
                indices.push_back(i15);
                indices.push_back(i16);
            }
            patch_indices.push_back(indices); // adiciona os indices de cada patch ao patch_indices (matriz)
        }

        if(getline(f, l)){    
            sscanf(l.c_str(), "%d\n", &nr_control_points);  // apos os indices dos patches aparece o nr de pontos de controlo
        }

        for(int j = 0; j < nr_control_points; j++){         // lê todos os pontos de controlo
            float x, y, z;

            if (getline(f, l)) {
                sscanf(l.c_str(), "%f, %f, %f\n", &x, &y, &z);
            }

            Vertices ponto;             // armazena sob a estrutura "Vertices"
            ponto.cord.push_back(x);
            ponto.cord.push_back(y);
            ponto.cord.push_back(z);

            pontos_de_controlo.push_back(ponto);
        }
        
        f.close(); // Posta a leitura do arquivo, este é fechado

        // Calculo dos pontos intermédios
        float t;                        // t ao longo da superficie
        int index;                  
        int indices[4];                 // array para armazenar os pontos de controlo

        float step = 1 / ((float)subdivs - 1);  // "passo" ao longo da curva

        //float ctrl[4][subdivs][3], ret[subdivs][subdivs][3];
        vector<vector<vector<float>>> ctrl(4, vector<vector<float>>(subdivs, vector<float>(3)));        // Matriz 'ctrl' para os pontos de controle ao longo da curva
        vector<vector<vector<float>>> ret(subdivs, vector<vector<float>>(subdivs, vector<float>(3)));  // Matriz 'ret' para os pontos da superfície de Bezier

        for(int n = 0; n < nr_patches; n++){
            float p[16][3];                     // Matriz para armazenar os pontos de controle do patch atual
            for (int m = 0; m < 16; m++) {      // preenchimento da matriz
                p[m][0] = pontos_de_controlo[patch_indices[n][m]].cord[0];
                p[m][1] = pontos_de_controlo[patch_indices[n][m]].cord[1];
                p[m][2] = pontos_de_controlo[patch_indices[n][m]].cord[2];
            }
            int j = 0, k = 0;   // Índices para controlar a iteração sobre as matrizes 'ctrl' e 'ret'

            // Iteração sobre os segmentos de curva do patch atual
            for (int i = 0; i < 15; i += 4) {
                // Definição dos índices de controle para o segmento de curva atual
                indices[0] = i;
                indices[1] = i + 1;
                indices[2] = i + 2;
                indices[3] = i + 3;

                // Iteração sobre os pontos de controle ao longo do segmento de curva
                for (int a = 0; a < subdivs - 1; a++) {
                    t = a * step;   // Cálculo do parâmetro 't' para este ponto de controle
                    index = floor(t); 
                    t = t - index;

                    // Cálculo das coordenadas (x, y, z) do ponto de controle atual na direção 'u' da curva de Bezier
                    ctrl[j][k][0] = (-p[indices[0]][0] + 3 * p[indices[1]][0] - 3 * p[indices[2]][0] + p[indices[3]][0]) * pow(t, 3) + (3 * p[indices[0]][0] - 6 * p[indices[1]][0] + 3 * p[indices[2]][0]) * pow(t, 2) + (-3 * p[indices[0]][0] + 3 * p[indices[1]][0]) * t + p[indices[0]][0];
                    ctrl[j][k][1] = (-p[indices[0]][1] + 3 * p[indices[1]][1] - 3 * p[indices[2]][1] + p[indices[3]][1]) * pow(t, 3) + (3 * p[indices[0]][1] - 6 * p[indices[1]][1] + 3 * p[indices[2]][1]) * pow(t, 2) + (-3 * p[indices[0]][1] + 3 * p[indices[1]][1]) * t + p[indices[0]][1];
                    ctrl[j][k][2] = (-p[indices[0]][2] + 3 * p[indices[1]][2] - 3 * p[indices[2]][2] + p[indices[3]][2]) * pow(t, 3) + (3 * p[indices[0]][2] - 6 * p[indices[1]][2] + 3 * p[indices[2]][2]) * pow(t, 2) + (-3 * p[indices[0]][2] + 3 * p[indices[1]][2]) * t + p[indices[0]][2];
                    k++;
                }

                ctrl[j][k][0] = (-p[indices[0]][0] + 3 * p[indices[1]][0] - 3 * p[indices[2]][0] + p[indices[3]][0]) + (3 * p[indices[0]][0] - 6 * p[indices[1]][0] + 3 * p[indices[2]][0]) + (-3 * p[indices[0]][0] + 3 * p[indices[1]][0]) + p[indices[0]][0];
                ctrl[j][k][1] = (-p[indices[0]][1] + 3 * p[indices[1]][1] - 3 * p[indices[2]][1] + p[indices[3]][1]) + (3 * p[indices[0]][1] - 6 * p[indices[1]][1] + 3 * p[indices[2]][1]) + (-3 * p[indices[0]][1] + 3 * p[indices[1]][1]) + p[indices[0]][1];
                ctrl[j][k][2] = (-p[indices[0]][2] + 3 * p[indices[1]][2] - 3 * p[indices[2]][2] + p[indices[3]][2]) + (3 * p[indices[0]][2] - 6 * p[indices[1]][2] + 3 * p[indices[2]][2]) + (-3 * p[indices[0]][2] + 3 * p[indices[1]][2]) + p[indices[0]][2];
                j++;
                k = 0;
            }

            for (int j = 0; j < subdivs; j++) {
                for (int a = 0; a < subdivs - 1; a++) {
                    t = a * step;
                    index = floor(t);
                    t = t - index;

                    ret[j][k][0] = (-ctrl[0][j][0] + 3 * ctrl[1][j][0] - 3 * ctrl[2][j][0] + ctrl[3][j][0]) * pow(t,3) + (3 * ctrl[0][j][0] - 6 * ctrl[1][j][0] + 3 * ctrl[2][j][0]) * pow(t, 2) + (-3 * ctrl[0][j][0] + 3 * ctrl[1][j][0]) * t + ctrl[0][j][0];
                    ret[j][k][1] = (-ctrl[0][j][1] + 3 * ctrl[1][j][1] - 3 * ctrl[2][j][1] + ctrl[3][j][1]) * pow(t, 3) + (3 * ctrl[0][j][1] - 6 * ctrl[1][j][1] + 3 * ctrl[2][j][1]) * pow(t, 2) + (-3 * ctrl[0][j][1] + 3 * ctrl[1][j][1]) * t + ctrl[0][j][1];
                    ret[j][k][2] = (-ctrl[0][j][2] + 3 * ctrl[1][j][2] - 3 * ctrl[2][j][2] + ctrl[3][j][2]) * pow(t, 3) + (3 * ctrl[0][j][2] - 6 * ctrl[1][j][2] + 3 * ctrl[2][j][2]) * pow(t, 2) + (-3 * ctrl[0][j][2] + 3 * ctrl[1][j][2]) * t + ctrl[0][j][2];
                    k++;
                }

                ret[j][k][0] = (-ctrl[0][j][0] + 3 * ctrl[1][j][0] - 3 * ctrl[2][j][0] + ctrl[3][j][0]) + (3 * ctrl[0][j][0] - 6 * ctrl[1][j][0] + 3 * ctrl[2][j][0]) + (-3 * ctrl[0][j][0] + 3 * ctrl[1][j][0]) + ctrl[0][j][0];
                ret[j][k][1] = (-ctrl[0][j][1] + 3 * ctrl[1][j][1] - 3 * ctrl[2][j][1] + ctrl[3][j][1]) + (3 * ctrl[0][j][1] - 6 * ctrl[1][j][1] + 3 * ctrl[2][j][1]) + (-3 * ctrl[0][j][1] + 3 * ctrl[1][j][1]) + ctrl[0][j][1];
                ret[j][k][2] = (-ctrl[0][j][2] + 3 * ctrl[1][j][2] - 3 * ctrl[2][j][2] + ctrl[3][j][2]) + (3 * ctrl[0][j][2] - 6 * ctrl[1][j][2] + 3 * ctrl[2][j][2]) + (-3 * ctrl[0][j][2] + 3 * ctrl[1][j][2]) + ctrl[0][j][2];
                k = 0;
            }

            for (int i = 0; i < subdivs - 1; i++)
                for (int j = 0; j < subdivs - 1; j++) {
                    str << ret[i][j][0] << ' ' << ret[i][j][1] << ' ' << ret[i][j][2] << '\n';
                    str << ret[i+1][j][0] << ' ' << ret[i+1][j][1] << ' ' << ret[i+1][j][2] << '\n';
                    str << ret[i][j+1][0] << ' ' << ret[i][j+1][1] << ' ' << ret[i][j+1][2] << '\n';

                    str << ret[i+1][j][0] << ' ' << ret[i+1][j][1] << ' ' << ret[i+1][j][2] << '\n';
                    str << ret[i+1][j+1][0] << ' ' << ret[i+1][j + 1][1] << ' ' << ret[i+1][j+1][2] << '\n';
                    str << ret[i][j+1][0] << ' ' << ret[i][j+1][1] << ' ' << ret[i][j+1][2] << '\n';
                }
        }
        //wrtfile(s.str(), name);
        write_file(file_name, str.str());

    }
    else {
        std::cout << "Ficheiro \"Patch\" não encontrado";
    }
}


int main(int argc, char const* argv[]) {
    std::cout << "A Rodar!" << std::endl;

    if(argc < 2) {
        std::cout << "Argumentos Insuficientes.\n";
        return 0;
    }

    if(strcmp(argv[1], "plane") == 0) {
        if(argc < 5) {
            return 0;
        }
        planeGenerator(atoi(argv[2]), atoi(argv[3]), argv[4]);
    }
    else if(strcmp(argv[1], "box") == 0) {
        if(argc < 5) {
            return 0;
        }
        boxGeneretor(atoi(argv[2]), atoi(argv[3]), argv[4]);
    }
    else if(strcmp(argv[1], "cone") == 0) {
        if(argc < 7) {
            return 0;
        }
        coneGenerator(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
    }
    else if(strcmp(argv[1], "sphere") == 0) {
        if(argc < 6) {
            return 0;
        }
        sphereGenerator(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
    }
    else if(strcmp(argv[1], "torus") == 0) {
        if(argc < 7) {
            return 0;
        }
        torusGenerator(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
    }
    else if (strcmp(argv[1], "patch") == 0) { // generator patch teapot.patch 10 bezier_10.3d
        if (argc < 5) {
            return 0;
        }
        bezierGenerator(argv[2], atoi(argv[3]), argv[4]);
    }
    else {
        std::cout << "Comando Invalido.\n";
        return 0;
    }
    return 1;
}

