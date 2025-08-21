Este projeto apresenta um sistema robusto para calcular as menores distâncias entre pontos de interesse em um mapa urbano, como hospitais e locais de lazer. A aplicação foi desenvolvida em C e utiliza o algoritmo de Floyd-Warshall para a determinação de rotas e a Distância Euclidiana para o cálculo das distâncias geográficas. Para otimizar a performance em grandes volumes de dados, o sistema foi paralelizado com OpenMP.

O estudo de caso principal foi realizado na cidade de São Paulo, onde foram mapeados 26 hospitais e 3 estádios de futebol em uma grade que evoluiu de 100x100 para 6500x6500, demonstrando a escalabilidade e eficiência da solução.

Precisão do Algoritmo
Os testes iniciais, comparando as distâncias entre estádios de futebol em São Paulo, demonstraram alta precisão, com taxas de erro baixas, As pequenas discrepâncias são atribuídas a fatores como a resolução da matriz e a precisão na definição das coordenadas dos pontos de interesse.

Performance com OpenMP
A implementação do paralelismo com OpenMP resultou em uma redução drástica no tempo de execução dos cálculos em uma matriz de 6500x6500. Os testes foram realizados em um processador i5 12500H com 16GB de RAM DDR5.

