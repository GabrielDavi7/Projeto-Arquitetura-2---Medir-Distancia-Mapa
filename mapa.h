#ifndef MAPA_H
#define MAPA_H

#define SIZE_X 6700           // Dimensão horizontal
#define SIZE_Y 6700           // Dimensão vertical
#define DIST_VE 633.00/67      // Distância vertical em metros
#define DIST_HO 733.00/67        // Distância horizontal em metros
#define MAX_TEXTO_CELULA 256  // Tamanho máximo do texto na célula
#define MAX_HOSPITAIS 26      // Máximo de hospitais
#define MAX_PONTOS_LAZER 3    // Máximo de pontos de lazer

// Estrutura para armazenar informações de pontos de lazer
struct PontoLazer {
    int x;
    int y;
    char nome[50];
};

// Funções para alocação e liberação de memória
double **alocarMatrizDist(int totalPontos);
void liberarMatrizDist(double **matriz, int totalPontos);
char ***alocarMapa(int sizeY, int sizeX);
void liberarMapa(char ***mapa, int sizeY, int sizeX);

// Funções de cálculo
double calcularDistancia(int x1, int y1, int x2, int y2);
void inicializarDistancias(double **dist, int numHospitais, int numPontosLazer);
void calcularDistanciasPontosVazios(char ***mapa, int numHospitais, int numPontosLazer);
void calcularDistanciaEspecifica(int x, int y, int numHospitais, int numPontosLazer);
void calcularDistanciasEntrePontosLazer(int numPontosLazer);
void calcularDistanciaTodosPontos(int x, int y, double **dist, int numHospitais, int numPontosLazer);
void floydWarshall(double **dist, int numHospitais, int numPontosLazer);

#endif // MAPA_H
