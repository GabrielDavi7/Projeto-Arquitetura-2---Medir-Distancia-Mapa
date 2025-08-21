#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "mapa.h"

// Número de threads a serem utilizados pelo OpenMP
#define NUM_THREADS 4

// Função para exibir o menu principal do programa
void menu() {
    printf("\n====================================\n");
    printf("          MENU PRINCIPAL\n");
    printf("====================================\n");
    printf("1. Calcular distância de um ponto (x, y) até hospitais e pontos de lazer\n");
    printf("2. Calcular distâncias entre os pontos de lazer\n");
    printf("3. Calcular distâncias de um ponto específico até todos os hospitais e pontos de lazer\n");
    printf("4. Calcular distâncias mínimas entre todos os pontos (Floyd-Warshall)\n");
    printf("5. Fechar o programa\n");
    printf("====================================\n");
    printf("Escolha uma opção: ");
}

// Função principal do programa
int main() {
    // Configura o número de threads a serem usados pelo OpenMP
    omp_set_num_threads(NUM_THREADS);

    // Aloca memória para o mapa e para a matriz de distâncias
    char ***mapa = alocarMapa(SIZE_Y, SIZE_X);
    double **dist = alocarMatrizDist(MAX_HOSPITAIS + MAX_PONTOS_LAZER);

    // Inicializa o mapa com texto vazio
    for (int i = 0; i < SIZE_Y; i++) {
        for (int j = 0; j < SIZE_X; j++) {
            strcpy(mapa[i][j], ""); // Define cada célula do mapa como uma string vazia
        }
    }

    // Inicializa a matriz de distâncias com valores máximos
    inicializarDistancias(dist, MAX_HOSPITAIS, MAX_PONTOS_LAZER);

    // Calcula as distâncias de todos os pontos vazios no mapa para hospitais e pontos de lazer
    calcularDistanciasPontosVazios(mapa, MAX_HOSPITAIS, MAX_PONTOS_LAZER);

    int opcao;
    while (1) {
        menu(); // Exibe o menu para o usuário
        int result = scanf("%d", &opcao); // Lê a opção escolhida pelo usuário
        if (result != 1) {
            // Verifica se a leitura da opção foi bem-sucedida
            printf("Erro na leitura da opção. Verifique a entrada fornecida.\n");
            while (getchar() != '\n'); // Limpa o buffer de entrada
            continue;
        }

        switch (opcao) {
            case 1: {
                // Calcula a distância de um ponto específico até hospitais e pontos de lazer
                int x, y;
                printf("Digite a coordenada x: ");
                scanf("%d", &x);
                printf("Digite a coordenada y: ");
                scanf("%d", &y);

                // Verifica se as coordenadas fornecidas estão dentro dos limites do mapa
                if (x < 0 || x >= SIZE_Y || y < 0 || y >= SIZE_X) {
                    printf("Coordenadas inválidas. Deve estar entre 0 e (%d, %d).\n", SIZE_Y - 1, SIZE_X - 1);
                    break;
                }

                // Calcula a distância do ponto (x, y) para hospitais e pontos de lazer
                calcularDistanciaEspecifica(x, y, MAX_HOSPITAIS, MAX_PONTOS_LAZER);
                break;
            }
            case 2:
                // Calcula e exibe as distâncias entre todos os pontos de lazer
                calcularDistanciasEntrePontosLazer(MAX_PONTOS_LAZER);
                break;
            case 3: {
                // Calcula a distância de um ponto específico para todos os hospitais e pontos de lazer
                int x, y;
                printf("Digite a coordenada x: ");
                scanf("%d", &x);
                printf("Digite a coordenada y: ");
                scanf("%d", &y);

                // Verifica se as coordenadas fornecidas estão dentro dos limites do mapa
                if (x < 0 || x >= SIZE_Y || y < 0 || y >= SIZE_X) {
                    printf("Coordenadas inválidas. Deve estar entre 0 e (%d, %d).\n", SIZE_Y - 1, SIZE_X - 1);
                    break;
                }

                // Calcula a distância do ponto (x, y) para todos os hospitais e pontos de lazer
                calcularDistanciaTodosPontos(x, y, dist, MAX_HOSPITAIS, MAX_PONTOS_LAZER);
                break;
            }
            case 4:
                // Executa o algoritmo de Floyd-Warshall para encontrar as menores distâncias entre todos os pontos
                floydWarshall(dist, MAX_HOSPITAIS, MAX_PONTOS_LAZER);
                printf("Distâncias mínimas entre todos os pontos foram calculadas.\n");
                break;
            case 5:
                // Libera a memória alocada e encerra o programa
                liberarMapa(mapa, SIZE_Y, SIZE_X);
                liberarMatrizDist(dist, MAX_HOSPITAIS + MAX_PONTOS_LAZER);
                printf("Programa encerrado.\n");
                return 0;
            default:
                // Caso a opção escolhida não seja válida, exibe uma mensagem de erro
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }
}
