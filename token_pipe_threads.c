#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4
#define NUM_VOLTAS 10

typedef struct {
    int valor;
    int voltas;
    int thread_atual;
} Token;

int fd[2];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int terminou = 0;

void *thread_func(void *arg) {
    int id = *(int *)arg;
    Token token;

    while (1) {
        pthread_mutex_lock(&mutex);

        if (terminou) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        if (read(fd[0], &token, sizeof(Token)) <= 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        if (token.thread_atual == id) {
            printf("Thread %d recebeu token com valor %d\n", id, token.valor);

            token.valor++;
            if (id == NUM_THREADS - 1) {
                token.voltas++;
            }

            if (token.voltas >= NUM_VOLTAS) {
                printf("Token final chegou ao valor %d após %d voltas\n", token.valor, NUM_VOLTAS);
                terminou = 1;
                pthread_mutex_unlock(&mutex);
                // Fechar pipe de escrita faz com que outras threads saiam naturalmente
                close(fd[1]);
                break;
            }

            token.thread_atual = (id + 1) % NUM_THREADS;

            write(fd[1], &token, sizeof(Token));
        } else {
            write(fd[1], &token, sizeof(Token));
        }

        pthread_mutex_unlock(&mutex);
        usleep(10000);
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    if (pipe(fd) == -1) {
        perror("Erro ao criar o pipe");
        exit(EXIT_FAILURE);
    }

    Token token = {0, 0, 0};
    write(fd[1], &token, sizeof(Token));

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    close(fd[0]);

    printf("Programa a terminar.\n");
    return 0;
}
