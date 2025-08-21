#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <omp.h>
#include "mapa.h"

// Inicialização dos hospitais com suas coordenadas (x, y)
int hospitais[MAX_HOSPITAIS][2] = {
    {2613, 1608}, {1273, 1876}, {1474, 3082}, {3082, 2814}, {3082, 3015},
    {3015, 3283}, {3015, 3350}, {3082, 3417}, {2881, 3350}, {2747, 3551},
    {3082, 3015}, {1474, 5159}, {3082, 3015}, {2546, 4087}, {3082, 5484},
    {3082, 3621}, {3149, 3621}, {3015, 3484}, {3149, 3484}, {3149, 3819},
    {3149, 4221}, {3752, 3216}, {3953, 2278}, {4690, 4489}, {5628, 2948},
    {6231, 1541}
};// dps da multiplicacao de 67

int numHospitais = sizeof(hospitais) / sizeof(hospitais[0]); // Número total de hospitais

// Inicialização dos pontos de lazer com suas coordenadas e nomes
struct PontoLazer pontosLazer[MAX_PONTOS_LAZER] = {
    {42*67, 40*67, "Allianz Parque"},
    {46*67, 73*67, "Neo Química Arena"},
    {43*67, 44*67, "Pacaembu"}
};
int numPontosLazer = sizeof(pontosLazer) / sizeof(pontosLazer[0]); // Número total de pontos de lazer

// Função para alocar a matriz de distâncias (totalPontos x totalPontos)
double **alocarMatrizDist(int totalPontos) {
    double **matriz = (double **)malloc(totalPontos * sizeof(double *));
    for (int i = 0; i < totalPontos; i++) {
        matriz[i] = (double *)malloc(totalPontos * sizeof(double));
    }
    return matriz;
}

// Função para liberar a memória alocada para a matriz de distâncias
void liberarMatrizDist(double **matriz, int totalPontos) {
    for (int i = 0; i < totalPontos; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

// Função para alocar a memória para o mapa (matriz tridimensional de strings)
char ***alocarMapa(int sizeY, int sizeX) {
    char ***mapa = (char ***)malloc(sizeY * sizeof(char **));
    for (int i = 0; i < sizeY; i++) {
        mapa[i] = (char **)malloc(sizeX * sizeof(char *));
        for (int j = 0; j < sizeX; j++) {
            mapa[i][j] = (char *)malloc(MAX_TEXTO_CELULA * sizeof(char));
        }
    }
    return mapa;
}

// Função para liberar a memória alocada para o mapa
void liberarMapa(char ***mapa, int sizeY, int sizeX) {
    for (int i = 0; i < sizeY; i++) {
        for (int j = 0; j < sizeX; j++) {
            free(mapa[i][j]);
        }
        free(mapa[i]);
    }
    free(mapa);
}

// Função para calcular a distância euclidiana entre dois pontos (x1, y1) e (x2, y2)
// Utiliza os coeficientes DIST_HO e DIST_VE para ajustar as distâncias horizontal e vertical
double calcularDistancia(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) * DIST_HO * DIST_HO + pow(y2 - y1, 2) * DIST_VE * DIST_VE);
}

// Função para inicializar a matriz de distâncias com valores máximos e distâncias específicas
void inicializarDistancias(double **dist, int numHospitais, int numPontosLazer) {
    int totalPontos = numHospitais + numPontosLazer;

    // Inicializa a matriz de distâncias
    for (int i = 0; i < totalPontos; i++) {
        for (int j = 0; j < totalPontos; j++) {
            dist[i][j] = (i == j) ? 0 : DBL_MAX; // Distância 0 para o mesmo ponto e infinito para pontos diferentes
        }
    }

    // Calcula distâncias entre hospitais
    for (int i = 0; i < numHospitais; i++) {
        for (int j = 0; j < numHospitais; j++) {
            dist[i][j] = calcularDistancia(hospitais[i][0], hospitais[i][1], hospitais[j][0], hospitais[j][1]);
        }
    }

    // Calcula distâncias entre pontos de lazer
    for (int i = 0; i < numPontosLazer; i++) {
        for (int j = 0; j < numPontosLazer; j++) {
            dist[numHospitais + i][numHospitais + j] = calcularDistancia(pontosLazer[i].x, pontosLazer[i].y, pontosLazer[j].x, pontosLazer[j].y);
        }
    }

    // Calcula distâncias entre hospitais e pontos de lazer
    for (int i = 0; i < numHospitais; i++) {
        for (int j = 0; j < numPontosLazer; j++) {
            dist[i][numHospitais + j] = calcularDistancia(hospitais[i][0], hospitais[i][1], pontosLazer[j].x, pontosLazer[j].y);
            dist[numHospitais + j][i] = dist[i][numHospitais + j]; // Matriz simétrica
        }
    }
}

// Função para calcular as distâncias de todos os pontos vazios no mapa até hospitais e pontos de lazer
void calcularDistanciasPontosVazios(char ***mapa, int numHospitais, int numPontosLazer) {
    #pragma omp parallel for collapse(2) // Paraleliza o loop para melhorar a performance
    for (int i = 0; i < SIZE_Y; i++) {
        for (int j = 0; j < SIZE_X; j++) {
            if (strcmp(mapa[i][j], "") == 0) { // Verifica se a célula do mapa está vazia
                // Calcula distâncias para hospitais
                for (int h = 0; h < numHospitais; h++) {
                    double distancia = calcularDistancia(i, j, hospitais[h][0], hospitais[h][1]);
                    // Cálculo realizado, mas não impresso
                }
                // Calcula distâncias para pontos de lazer
                for (int p = 0; p < numPontosLazer; p++) {
                    double distancia = calcularDistancia(i, j, pontosLazer[p].x, pontosLazer[p].y);
                    // Cálculo realizado, mas não impresso
                }
            }
        }
    }
}

// Função para calcular a distância de um ponto específico até todos os hospitais e pontos de lazer
void calcularDistanciaEspecifica(int x, int y, int numHospitais, int numPontosLazer) {
    for (int h = 0; h < numHospitais; h++) {
        double distancia = calcularDistancia(x, y, hospitais[h][0], hospitais[h][1]);
        printf("Distância até o hospital %d: %.2f metros\n", h + 1, distancia);
    }

    for (int p = 0; p < numPontosLazer; p++) {
        double distancia = calcularDistancia(x, y, pontosLazer[p].x, pontosLazer[p].y);
        printf("Distância até o ponto de lazer %s: %.2f metros\n", pontosLazer[p].nome, distancia);
    }
}

// Função para calcular e exibir as distâncias entre todos os pontos de lazer
void calcularDistanciasEntrePontosLazer(int numPontosLazer) {
    for (int i = 0; i < numPontosLazer; i++) {
        for (int j = i + 1; j < numPontosLazer; j++) {
            double distancia = calcularDistancia(pontosLazer[i].x, pontosLazer[i].y, pontosLazer[j].x, pontosLazer[j].y);
            printf("Distância entre %s e %s: %.2f metros\n", pontosLazer[i].nome, pontosLazer[j].nome, distancia);
        }
    }
}

// Função para calcular a distância de um ponto específico até todos os hospitais e pontos de lazer usando o algoritmo de Floyd-Warshall
void calcularDistanciaTodosPontos(int x, int y, double **dist, int numHospitais, int numPontosLazer) {
    int totalPontos = numHospitais + numPontosLazer;

    // Calcula a distância do ponto (x, y) a todos os pontos
    for (int i = 0; i < totalPontos; i++) {
        double distancia = calcularDistancia(x, y, i < numHospitais ? hospitais[i][0] : pontosLazer[i - numHospitais].x,
                                             i < numHospitais ? hospitais[i][1] : pontosLazer[i - numHospitais].y);
        printf("Distância até %s: %.2f metros\n",
               i < numHospitais ? (i < numHospitais ? "Hospital" : pontosLazer[i - numHospitais].nome) : pontosLazer[i - numHospitais].nome, distancia);
    }
}

// Implementação do algoritmo de Floyd-Warshall para encontrar as menores distâncias entre todos os pontos
void floydWarshall(double **dist, int numHospitais, int numPontosLazer) {
    int totalPontos = numHospitais + numPontosLazer;

    // Calcula a menor distância entre todos os pares de pontos
    for (int k = 0; k < totalPontos; k++) {
        #pragma omp parallel for collapse(2) // Paraleliza os loops sobre i e j
        for (int i = 0; i < totalPontos; i++) {
            for (int j = 0; j < totalPontos; j++) {
                if (dist[i][k] < DBL_MAX && dist[k][j] < DBL_MAX) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}
