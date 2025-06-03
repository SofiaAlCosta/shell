#include <stdio.h>
#include <pthread.h>

int m1[2][2] = {{1, 2}, {3, 4}};
int m2[2][2] = {{3, 4}, {7, 8}};
int mproduto[2][2];

void *calcular_elemento(void *arg) {
    int id = *(int *)arg;
    int linha = id / 2;
    int coluna = id % 2;

    mproduto[linha][coluna] = m1[linha][0] * m2[0][coluna] +
                              m1[linha][1] * m2[1][coluna];

    return NULL;
}

int main() {
    pthread_t threads[4];
    int ids[4];

    for (int i = 0; i < 4; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, calcular_elemento, &ids[i]);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Matriz Produto:\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            printf("%d ", mproduto[i][j]);
        }
        printf("\n");
    }

    return 0;
}
