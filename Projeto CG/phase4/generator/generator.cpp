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

// Vetor que guarda coordenadas dos pontos de controlo
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

    stringstream n; // normal
    stringstream t; // texture

    stringstream res;

    // x_inicial negativo para centralizar o plano na origem dos eixos
    float x_inicial = -(comprimento / 2.0f);
    float comprimento_lado = comprimento / divisoes;
    float x1, x2, z1, z2, y, div;

    // Calculo do nr de pontos
    int total_pontos = divisoes * divisoes * 6;
    str << "# Plane Generator\n";
    str << "# Total de Pontos: " << total_pontos << "\n";

    str << total_pontos << '\n'; 

    // Eixo dos y sempre a 0
    y = 0;

    div = 1 / divisoes;

    for (int i = 0; i < divisoes; i++) {
        for (int j = 0; j < divisoes; j++) {
            x1 = x_inicial + i * comprimento_lado;
            x2 = x_inicial + (i + 1) * comprimento_lado;

            z1 = x_inicial + j * comprimento_lado;
            z2 = x_inicial + (j + 1) * comprimento_lado;

            // triangulo 1
            str << x1 << ' ' << y << ' ' << z1 << '\n';
            n << 0 << ' ' << 1 << ' ' << 0 << '\n';
            t << i * div << ' ' << j * div << '\n';

            str << x1 << ' ' << y << ' ' << z2 << '\n';
            n << 0 << ' ' << 1 << ' ' << 0 << '\n';
            t << i * div << ' ' << (j + 1) * div << '\n';

            str << x2 << ' ' << y << ' ' << z2 << '\n';
            n << 0 << ' ' << 1 << ' ' << 0 << '\n';
            t << (i + 1) * div << ' ' << (j + 1) * div << '\n';

            // triangulo 2
            str << x2 << ' ' << y << ' ' << z1 << '\n';
            n << 0 << ' ' << 1 << ' ' << 0 << '\n';
            t << (i + 1) * div << ' ' << j * div << '\n';

            str << x1 << ' ' << y << ' ' << z1 << '\n';
            n << 0 << ' ' << 1 << ' ' << 0 << '\n';
            t << i * div << ' ' << j * div << '\n';

            str << x2 << ' ' << y << ' ' << z2 << '\n';
            n << 0 << ' ' << 1 << ' ' << 0 << '\n';
            t << (i + 1) * div << ' ' << (j + 1) * div << '\n';
        }
    }
    res << str.str() << n.str() << t.str();
    write_file(file_name, res.str());
}

// Função que gera o ficheiro com os pontos para uma Box
void boxGeneretor(float comprimento, float divisoes, std::string file_name) {
    // String que vai ser escrita no file
    std::stringstream str;

    stringstream n; // normal
    stringstream t; // texture

    stringstream res;

    float altura = comprimento / 2.0;
    // x_inicial == -altura, para centralizar a boxe na origem dos eixos
    float x_inicial = -altura;
    float comprimento_lado = comprimento / divisoes;
    float x1, x2, y1, y2, z1, z2, div;

    // Calculo do nr de pontos
    int total_pontos = divisoes * divisoes * 3 * 2 * 6;
    str << "# Box Generator\n";
    str << "# Total de Pontos: " << total_pontos << "\n";
    
    str << total_pontos << '\n';

    y1 = altura;
    y2 = -altura;

    div = 1 / divisoes;

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
            n << 0 << ' ' << 1 << ' ' << 0 << '\n';
            t << i * div << ' ' << j * div << '\n';

            str << x1 << ' ' << y1 << ' ' << z2 << '\n';
            n << 0 << ' ' << 1 << ' ' << 0 << '\n';
            t << i * div << ' ' << (j + 1) * div << '\n';

            str << x2 << ' ' << y1 << ' ' << z2 << '\n';
            n << 0 << ' ' << 1 << ' ' << 0 << '\n';
            t << (i + 1) * div << ' ' << (j + 1) * div << '\n';

            // Triangulo 2
            str << x2 << ' ' << y1 << ' ' << z1 << '\n';
            n << 0 << ' ' << 1 << ' ' << 0 << '\n';
            t << (i + 1) * div << ' ' << j * div << '\n';

            str << x1 << ' ' << y1 << ' ' << z1 << '\n';
            n << 0 << ' ' << 1 << ' ' << 0 << '\n';
            t << i * div << ' ' << j * div << '\n';

            str << x2 << ' ' << y1 << ' ' << z2 << '\n';
            n << 0 << ' ' << 1 << ' ' << 0 << '\n';
            t << (i + 1) * div << ' ' << (j + 1) * div << '\n';

            // Face inferior
            // Triangulo 1
            str << x1 << ' ' << y2 << ' ' << z1 << '\n';
            n << 0 << ' ' << -1 << ' ' << 0 << '\n';
            t << i * div << ' ' << j * div << '\n';

            str << x2 << ' ' << y2 << ' ' << z2 << '\n';
            n << 0 << ' ' << -1 << ' ' << 0 << '\n';
            t << (i + 1) * div << ' ' << (j + 1) * div << '\n';

            str << x1 << ' ' << y2 << ' ' << z2 << '\n';
            n << 0 << ' ' << -1 << ' ' << 0 << '\n';
            t << i * div << ' ' << (j + 1) * div << '\n';

            // Triangulo 2
            str << x2 << ' ' << y2 << ' ' << z1 << '\n';
            n << 0 << ' ' << -1 << ' ' << 0 << '\n';
            t << (i + 1) * div << ' ' << j * div << '\n';

            str << x2 << ' ' << y2 << ' ' << z2 << '\n';
            n << 0 << ' ' << -1 << ' ' << 0 << '\n';
            t << (i + 1) * div << ' ' << (j + 1) * div << '\n';

            str << x1 << ' ' << y2 << ' ' << z1 << '\n';
            n << 0 << ' ' << -1 << ' ' << 0 << '\n';
            t << i * div << ' ' << j * div << '\n';
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
            n << 0 << ' ' << 0 << ' ' << 1 << '\n';
            t << i * div << ' ' << (j + 1) * div << '\n';

            str << x1 << ' ' << y1 << ' ' << z1 << '\n';
            n << 0 << ' ' << 0 << ' ' << 1 << '\n';
            t << i * div << ' ' << j * div << '\n';

            str << x2 << ' ' << y1 << ' ' << z1 << '\n';
            n << 0 << ' ' << 0 << ' ' << 1 << '\n';
            t << (i + 1) * div << ' ' << j * div << '\n';

            // triangulo 2
            str << x2 << ' ' << y2 << ' ' << z1 << '\n';
            n << 0 << ' ' << 0 << ' ' << 1 << '\n';
            t << (i + 1) * div << ' ' << (j + 1) * div << '\n';

            str << x1 << ' ' << y2 << ' ' << z1 << '\n';
            n << 0 << ' ' << 0 << ' ' << 1 << '\n';
            t << i * div << ' ' << (j + 1) * div << '\n';

            str << x2 << ' ' << y1 << ' ' << z1 << '\n';
            n << 0 << ' ' << 0 << ' ' << 1 << '\n';
            t << (i + 1) * div << ' ' << j * div << '\n';

            // Face direita
            // triangulo 1
            str << x1 << ' ' << y2 << ' ' << z2 << '\n';
            n << 0 << ' ' << 0 << ' ' << -1 << '\n';
            t << i * div << ' ' << (j + 1) * div << '\n';

            str << x2 << ' ' << y1 << ' ' << z2 << '\n';
            n << 0 << ' ' << 0 << ' ' << -1 << '\n';
            t << (i + 1) * div << ' ' << j * div << '\n';

            str << x1 << ' ' << y1 << ' ' << z2 << '\n';
            n << 0 << ' ' << 0 << ' ' << -1 << '\n';
            t << i * div << ' ' << j * div << '\n';

            // triangulo 2
            str << x2 << ' ' << y2 << ' ' << z2 << '\n';
            n << 0 << ' ' << 0 << ' ' << -1 << '\n';
            t << (i + 1) * div << ' ' << (j + 1) * div << '\n';

            str << x2 << ' ' << y1 << ' ' << z2 << '\n';
            n << 0 << ' ' << 0 << ' ' << -1 << '\n';
            t << (i + 1) * div << ' ' << j * div << '\n';

            str << x1 << ' ' << y2 << ' ' << z2 << '\n';
            n << 0 << ' ' << 0 << ' ' << -1 << '\n';
            t << i * div << ' ' << (j + 1) * div << '\n';
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
            n << 1 << ' ' << 0 << ' ' << 0 << '\n';
            t << i * div << ' ' << j * div << '\n';

            str << x1 << ' ' << y1 << ' ' << z2 << '\n';
            n << 1 << ' ' << 0 << ' ' << 0 << '\n';
            t << (i + 1) * div << ' ' << (j + 1) * div << '\n';

            str << x1 << ' ' << y1 << ' ' << z1 << '\n';
            n << 1 << ' ' << 0 << ' ' << 0 << '\n';
            t << (i + 1) * div << ' ' << j * div << '\n';

            // triangulo 2
            str << x1 << ' ' << y2 << ' ' << z1 << '\n';
            n << 1 << ' ' << 0 << ' ' << 0 << '\n';
            t << i * div << ' ' << j * div << '\n';

            str << x1 << ' ' << y2 << ' ' << z2 << '\n';
            n << 1 << ' ' << 0 << ' ' << 0 << '\n';
            t << i * div << ' ' << (j + 1) * div << '\n';

            str << x1 << ' ' << y1 << ' ' << z1 << '\n';
            n << 1 << ' ' << 0 << ' ' << 0 << '\n';
            t << (i + 1) * div << ' ' << (j + 1) * div << '\n';

            // Face traseira
            // triangulo 1
            str << x2 << ' ' << y2 << ' ' << z2 << '\n';
            n << -1 << ' ' << 0 << ' ' << 0 << '\n';
            t << (i + 1) * div << ' ' << (j + 1) * div << '\n';

            str << x2 << ' ' << y1 << ' ' << z1 << '\n';
            n << -1 << ' ' << 0 << ' ' << 0 << '\n';
            t << i * div << ' ' << j * div << '\n';

            str << x2 << ' ' << y1 << ' ' << z2 << '\n';
            n << -1 << ' ' << 0 << ' ' << 0 << '\n';
            t << (i + 1) * div << ' ' << j * div << '\n';

            // triangulo 2
            str << x2 << ' ' << y2 << ' ' << z1 << '\n';
            n << -1 << ' ' << 0 << ' ' << 0 << '\n';
            t << i * div << ' ' << (j + 1) * div << '\n';

            str << x2 << ' ' << y1 << ' ' << z1 << '\n';
            n << -1 << ' ' << 0 << ' ' << 0 << '\n';
            t << i * div << ' ' << j * div << '\n';

            str << x2 << ' ' << y2 << ' ' << z2 << '\n';
            n << -1 << ' ' << 0 << ' ' << 0 << '\n';
            t << (i + 1) * div << ' ' << (j + 1) * div << '\n';

        }
    }
    res << str.str() << n.str() << t.str();
    write_file(file_name, res.str());
}

// Função que gera o ficheiro com os pontos para um Cone
void coneGenerator(float raio, float altura, float slices, float stacks, std::string file_name) {
    std::stringstream str;

    stringstream n; // normal
    stringstream t; // texture

    stringstream res;

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

    str << total_pontos << '\n';

    // Base do cone
    for (int i = 0; i < slices; i++) {
        x1 = raio * sin(i * (2 * M_PI / slices));
        x2 = raio * sin((i + 1) * (2 * M_PI / slices));

        z1 = raio * cos(i * (2 * M_PI / slices));
        z2 = raio * cos((i + 1) * (2 * M_PI / slices));

        // Centrada na origem e com Y = 0
        str << 0 << ' ' << 0 << ' ' << 0 << '\n';    // Origem
        n << 0 << ' ' << -1 << ' ' << 0 << '\n';
        t << 0.5 + sin((2 * M_PI / slices) * i) << ' ' << 0.5 + cos((2 * M_PI / slices) * i) << '\n';

        str << x2 << ' ' << 0 << ' ' << z2 << '\n';
        n << 0 << ' ' << -1 << ' ' << 0 << '\n';
        t << 0.5 << ' ' << 0.5 << '\n';

        str << x1 << ' ' << 0 << ' ' << z1 << '\n';
        n << 0 << ' ' << -1 << ' ' << 0 << '\n';
        t << 0.5 + sin((2 * M_PI / slices) * (i + 1)) << ' ' << 0.5 + cos((2 * M_PI / slices) * (i + 1)) << '\n';
    }

    // Lateral do cone
    float angulo = 0;
    for (int i = 0; i < stacks; i++) {

        next_y = y + (altura / stacks);
        next_raio = raio_atual - (raio / stacks);

        for (int j = 0; j < slices; j++) {

            // triangulo 1
            str << raio_atual * sin(angulo) << ' ' << y << ' ' << raio_atual * cos(angulo) << '\n';
            n << sin(angulo) << ' ' << altura / stacks << ' ' << cos(angulo) << '\n';
            t << j / (float)slices << ' ' << i / (float)stacks << '\n';

            str << raio_atual * sin(angulo + (2 * M_PI / slices)) << ' ' << y << ' ' << raio_atual * cos(angulo + (2 * M_PI / slices)) << '\n';
            n << sin(angulo + (2 * M_PI / (float)slices)) << ' ' << altura / stacks << ' ' << cos(angulo + (2 * M_PI / (float)slices)) << '\n';
            t << (j + 1) / (float)slices << ' ' << i / (float)stacks << '\n';

            str << next_raio * sin(angulo) << ' ' << next_y << ' ' << next_raio * cos(angulo) << '\n';
            n << sin(angulo) << ' ' << altura / stacks << ' ' << cos(angulo) << '\n';
            t << j / (float)slices << ' ' << (i + 1) / (float)stacks << '\n';

            // triangulo 2
            str << next_raio * sin(angulo) << ' ' << next_y << ' ' << next_raio * cos(angulo) << '\n';
            n << sin(angulo) << ' ' << altura / stacks << ' ' << cos(angulo) << '\n';
            t << j / (float)slices << ' ' << (i + 1) / (float)stacks << '\n';

            str << raio_atual * sin(angulo + (2 * M_PI / slices)) << ' ' << y << ' ' << raio_atual * cos(angulo + (2 * M_PI / slices)) << '\n';
            n << sin(angulo + (2 * M_PI / (float)slices)) << ' ' << altura / stacks << ' ' << cos(angulo + (2 * M_PI / (float)slices)) << '\n';
            t << (j + 1) / (float)slices << ' ' << i / (float)stacks << '\n';

            str << next_raio * sin(angulo + (2 * M_PI / slices)) << ' ' << next_y << ' ' << next_raio * cos(angulo + (2 * M_PI / slices)) << '\n';
            n << sin(angulo + (2 * M_PI / (float)slices)) << ' ' << altura / stacks << ' ' << cos(angulo + (2 * M_PI / (float)slices)) << '\n';
            t << (j + 1) / (float)slices << ' ' << (i + 1) / (float)stacks << '\n';

            angulo += (2 * M_PI / slices);
        }
        y = next_y;
        raio_atual = next_raio;
    }
    res << str.str() << n.str() << t.str();
    write_file(file_name, res.str());
}

// Função que gera o ficheiro com os pontos para uma Esfera
void sphereGenerator(int raio, int slices, int stacks, std::string file_name) {
    std::stringstream str;

    stringstream n; // normal
    stringstream t; // texture

    stringstream res;

    float divisao;
    float stack;
    float divisao_size = 2 * M_PI / slices;
    float stack_size = M_PI / stacks;
    float angulo, angulo2, angulo_slice, angulo_slice2;

    float x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4, n_x1, n_y1, n_z1, n_x2, n_y2, n_z2, n_x3, n_y3, n_z3, n_x4, n_y4, n_z4, t_x1, t_y1, t_x2, t_y2, t_x3, t_y3, t_x4, t_y4;

    // Calculo do nr de pontos
    int total_pontos = (2 * slices * 3) + (stacks - 2) * stacks * 6;
    str << "# Sphere Generator\n";
    str << "# Total de Pontos: " << total_pontos << "\n";

    str << total_pontos << '\n';

    for (int i = 0; i < stacks; i++) {

        angulo = M_PI / 2 - i * stack_size;
        angulo2 = M_PI / 2 - (i + 1) * stack_size;

        for (int j = 0; j < slices; j++) {

            angulo_slice = j * divisao_size;
            angulo_slice2 = (j + 1) * divisao_size;

            x1 = raio * cos(angulo) * sin(angulo_slice);
            y1 = raio * sin(angulo);
            z1 = raio * cos(angulo) * cos(angulo_slice);

            x2 = raio * cos(angulo2) * sin(angulo_slice);
            y2 = raio * sin(angulo2);
            z2 = raio * cos(angulo2) * cos(angulo_slice);

            x3 = raio * cos(angulo2) * sin(angulo_slice2);
            y3 = raio * sin(angulo2);
            z3 = raio * cos(angulo2) * cos(angulo_slice2);

            x4 = raio * cos(angulo) * sin(angulo_slice2);
            y4 = raio * sin(angulo);
            z4 = raio * cos(angulo) * cos(angulo_slice2);

            n_x1 = cos(angulo) * sin(angulo_slice);
            n_y1 = sin(angulo);
            n_z1 = cos(angulo) * cos(angulo_slice);

            n_x2 = cos(angulo2) * sin(angulo_slice);
            n_y2 = sin(angulo2);
            n_z2 = cos(angulo2) * cos(angulo_slice);

            n_x3 = cos(angulo2) * sin(angulo_slice2);
            n_y3 = sin(angulo2);
            n_z3 = cos(angulo2) * cos(angulo_slice2);

            n_x4 = cos(angulo) * sin(angulo_slice2);
            n_y4 = sin(angulo);
            n_z4 = cos(angulo) * cos(angulo_slice2);

            t_x1 = (float)j / slices;
            t_y1 = (float)i / stacks;
            t_x2 = (float)j / slices;
            t_y2 = (float)(i + 1) / stacks;
            t_x3 = (float)(j + 1) / slices;
            t_y3 = (float)(i + 1) / stacks;
            t_x4 = (float)(j + 1) / slices;
            t_y4 = (float)i / stacks;

            // 1º triângulo
            if (i != 0) {
                str << x1 << ' ' << y1 << ' ' << z1 << '\n';
                n << n_x1 << ' ' << n_y1 << ' ' << n_z1 << '\n';
                t << t_x1 << ' ' << t_y1 << '\n';

                str << x2 << ' ' << y2 << ' ' << z2 << '\n';
                n << n_x2 << ' ' << n_y2 << ' ' << n_z2 << '\n';
                t << t_x2 << ' ' << t_y2 << '\n';

                str << x4 << ' ' << y4 << ' ' << z4 << '\n';
                n << n_x4 << ' ' << n_y4 << ' ' << n_z4 << '\n';
                t << t_x4 << ' ' << t_y4 << '\n';
            }

            // 2º triângulo
            if (i != stacks - 1) {
                str << x2 << ' ' << y2 << ' ' << z2 << '\n';
                n << n_x2 << ' ' << n_y2 << ' ' << n_z2 << '\n';
                t << t_x2 << ' ' << t_y2 << '\n';

                str << x3 << ' ' << y3 << ' ' << z3 << '\n';
                n << n_x3 << ' ' << n_y3 << ' ' << n_z3 << '\n';
                t << t_x3 << ' ' << t_y3 << '\n';

                str << x4 << ' ' << y4 << ' ' << z4 << '\n';
                n << n_x4 << ' ' << n_y4 << ' ' << n_z4 << '\n';
                t << t_x4 << ' ' << t_y4 << '\n';
            }
        }
    }
    res << str.str() << n.str() << t.str();
    write_file(file_name, res.str());
}

// Função que gera o ficheiro com os pontos para um Torus (Donut)
void torusGenerator(int raio_menor, int raio_maior, int slices, int stacks, std::string file_name) {
    std::stringstream str;

    stringstream n; // normal
    stringstream t; // texture

    stringstream res;

    int meio_diff = (raio_menor + raio_maior) / 2; 
    int diff_raios = raio_menor - raio_maior;           // raio_menor -> meio_diff

    float ang1 = (2 * M_PI) / stacks, ang2 = (2 * M_PI) / slices;

    // Calculo do nr de pontos
    int total_pontos = 2 * stacks * slices * 3;
    str << "# Torus Generator\n";
    str << "# Total de Pontos: " << total_pontos << "\n";

    str << total_pontos << '\n';

    float x1, x2, x3, x4, y1, y2, y3, z1, z2, z3, z4, n_x1, n_x2, n_x3, n_x4, n_z1, n_z2, n_z3, n_z4;
    float alpha = (2 * M_PI) / stacks, beta = (2 * M_PI) / slices;

    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {
            x1 = (meio_diff + diff_raios * cos(alpha * i)) * cos(beta * j);
            x2 = (meio_diff + diff_raios * cos(alpha * (i + 1))) * cos(beta * j);
            x3 = (meio_diff + diff_raios * cos(alpha * (i + 1))) * cos(beta * (j + 1));
            x4 = (meio_diff + diff_raios * cos(alpha * i)) * cos(beta * (j + 1));

            n_x1 = (diff_raios * cos(alpha * i)) * cos(beta * j);
            n_x2 = (diff_raios * cos(alpha * (i + 1))) * cos(beta * j);
            n_x3 = (diff_raios * cos(alpha * (i + 1))) * cos(beta * (j + 1));
            n_x4 = (diff_raios * cos(alpha * i)) * cos(beta * (j + 1));

            y1 = diff_raios * sin(alpha * i);
            y2 = diff_raios * sin(alpha * (i + 1));
            y3 = diff_raios * sin(alpha * i);

            z1 = (meio_diff + diff_raios * cos(alpha * i)) * sin(beta * j);
            z2 = (meio_diff + diff_raios * cos(alpha * (i + 1))) * sin(beta * j);
            z3 = (meio_diff + diff_raios * cos(alpha * (i + 1))) * sin(beta * (j + 1));
            z4 = (meio_diff + diff_raios * cos(alpha * i)) * sin(beta * (j + 1));

            n_z1 = (diff_raios * cos(alpha * i)) * sin(beta * j);
            n_z2 = (diff_raios * cos(alpha * (i + 1))) * sin(beta * j);
            n_z3 = (diff_raios * cos(alpha * (i + 1))) * sin(beta * (j + 1));
            n_z4 = (diff_raios * cos(alpha * i)) * sin(beta * (j + 1));

            str << x1 << ' ' << y1 << ' ' << z1 << '\n';
            // normal normalizada
            n << n_x1 / sqrt(n_x1 * n_x1 + y1 * y1 + n_z1 * n_z1) << ' ' << y1 / sqrt(n_x1 * n_x1 + y1 * y1 + n_z1 * n_z1) << ' ' << n_z1 / sqrt(n_x1 * n_x1 + y1 * y1 + n_z1 * n_z1) << '\n';
            t << (float)(j) / slices << ' ' << (float)(i) / stacks << '\n';
            
            str << x2 << ' ' << y2 << ' ' << z2 << '\n';
            // normal normalizada
            n << n_x2 / sqrt(n_x2 * n_x2 + y2 * y2 + n_z2 * n_z2) << ' ' << y2 / sqrt(n_x2 * n_x2 + y2 * y2 + n_z2 * n_z2) << ' ' << n_z2 / sqrt(n_x2 * n_x2 + y2 * y2 + n_z2 * n_z2) << '\n';
            t << (float)(j) / slices << ' ' << (float)(i + 1) / stacks << '\n';
            
            str << x4 << ' ' << y3 << ' ' << z4 << '\n';
            // normal normalizada
            n << n_x4 / sqrt(n_x4 * n_x4 + y3 * y3 + n_z4 * n_z4) << ' ' << y3 / sqrt(n_x4 * n_x4 + y3 * y3 + n_z4 * n_z4) << ' ' << n_z4 / sqrt(n_x4 * n_x4 + y3 * y3 + n_z4 * n_z4) << '\n';
            t << (float)(j + 1) / slices << ' ' << (float)(i + 1) / stacks << '\n';

            str << x2 << ' ' << y2 << ' ' << z2 << '\n';
            // normal normalizada
            n << n_x2 / sqrt(n_x2 * n_x2 + y2 * y2 + n_z2 * n_z2) << ' ' << y2 / sqrt(n_x2 * n_x2 + y2 * y2 + n_z2 * n_z2) << ' ' << n_z2 / sqrt(n_x2 * n_x2 + y2 * y2 + n_z2 * n_z2) << '\n';
            t << (float)(j + 1) / slices << ' ' << (float)(i + 1) / stacks << '\n';
            
            str << x3 << ' ' << y2 << ' ' << z3 << '\n';
            // normal normalizada
            n << n_x3 / sqrt(n_x3 * n_x3 + y2 * y2 + n_z3 * n_z3) << ' ' << y2 / sqrt(n_x3 * n_x3 + y2 * y2 + n_z3 * n_z3) << ' ' << n_z3 / sqrt(n_x3 * n_x3 + y2 * y2 + n_z3 * n_z3) << '\n';
            t << (float)(j + 1) / slices << ' ' << (float)(i) / stacks << '\n';
            
            str << x4 << ' ' << y3 << ' ' << z4 << '\n';
            // normal normalizada
            n << n_x4 / sqrt(n_x4 * n_x4 + y3 * y3 + n_z4 * n_z4) << ' ' << y3 / sqrt(n_x4 * n_x4 + y3 * y3 + n_z4 * n_z4) << ' ' << n_z4 / sqrt(n_x4 * n_x4 + y3 * y3 + n_z4 * n_z4) << '\n';
            t << (float)(j) / slices << ' ' << (float)(i) / stacks << '\n';
        }
    }
    res << str.str() << n.str() << t.str();
    write_file(file_name, res.str());
}


void bezierGenerator(std::string patch, int subdivs, std::string file_name){
    
    //const int subdivs = 10;
    stringstream str;

    // nr Pontos == 0 para o engine saber
    str << '0' << '\n';

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
                p[m][0] = pontos_de_controlo[patch_indices[n][m]].cord[0];  //x
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

