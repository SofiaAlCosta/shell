#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 3  // Altere conforme necessário

int m1[N][N], m2[N][N], mproduto[N][N];

void *calcular_elemento(void *arg) {
    int id = *(int *)arg;
    int linha = id / N;
    int coluna = id % N;

    mproduto[linha][coluna] = 0;
    for (int k = 0; k < N; k++) {
        mproduto[linha][coluna] += m1[linha][k] * m2[k][coluna];
    }

    free(arg); // Liberta memória alocada dinamicamente
    return NULL;
}

int main() {
    pthread_t threads[N*N];

    // Inicializar matrizes com valores simples para teste
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            m1[i][j] = i + j;
            m2[i][j] = i == j ? 1 : 0;  // matriz identidade para teste
        }

    for (int i = 0; i < N*N; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, calcular_elemento, id);
    }

    for (int i = 0; i < N*N; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Matriz Produto:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", mproduto[i][j]);
        }
        printf("\n");
    }

    return 0;
}
