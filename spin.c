#include <stdio.h>
#include <pthread.h>

#ifndef NUM_THREADS
#define NUM_THREADS 5
#endif

int x = 1;

void *funcao(void *args) {
    while (x == 1) ; // Spin (não faz nada)
    printf("\nOla\n");
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int i;

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, funcao, NULL);
    }

    printf("Introduza novo Valor de x: ");
    scanf("%d", &x);

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
