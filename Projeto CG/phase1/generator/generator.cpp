#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

// Fun��o que recebe uma string de texto e escreve esse "texto" no ficheiro
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

// Fun��o que desenha um plano
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

// Fun��o que gera o ficheiro com os pontos para uma Box
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
    for (int i = 0; i < divisoes; i++){
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
        for (int j = 0; j < divisoes; j++){
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



int main(int argc, char const* argv[]) {
    std::cout << "A Rodar!" << std::endl;

    if (argc < 2) {
        std::cout << "Argumentos Insuficientes.\n";
        return 1;
    }

    if (strcmp(argv[1], "plane") == 0) {
        if (argc < 5) {
            return 1;
        }
        planeGenerator(atoi(argv[2]), atoi(argv[3]), argv[4]);
    }
    else if (strcmp(argv[1], "box") == 0) {
        if (argc < 5) {
            return 0;
        }
        boxGeneretor(atoi(argv[2]), atoi(argv[3]), argv[4]);
    }
    else if (strcmp(argv[1], "cone") == 0) {
        if (argc < 7) {
            return 0;
        }
        coneGenerator(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
    }
    else if (strcmp(argv[1], "sphere") == 0) {
        if (argc < 6) {
            return 0;
        }
        sphereGenerator(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
    } 
    else {
        std::cout << "Comando Invalido.\n";
        return 0;
    }
    return 1;
}