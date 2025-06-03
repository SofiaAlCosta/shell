#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "shell.h"

#define MAX 100

char strings[MAX][130];
int k = 0;

// Função que espera 'tempo' segundos e depois imprime uma mensagem
void aviso(char *mesg, int tempo) {
    while (tempo > 0) {
        sleep(1);
        tempo--;
    }
    fprintf(stderr, "Aviso : %s\n", mesg);
}

// Wrapper que usa argumentos como vetor de strings (ex: ["avisorepetido", "mensagem", "3"])
void *avisowrapperMAU(void *args) {
    char **pargs = (char **) args;
    aviso(pargs[1], atoi(pargs[2]));
    return NULL;
}

// Wrapper que usa uma struct alocada dinamicamente (msg + tempo)
void *avisowrapper(void *args) {
    aviso_t *ptr = (aviso_t *)args;
    aviso(ptr->msg, ptr->tempo);
    free(ptr);
    return NULL;
}

// Wrapper para socp com log interno num vetor circular
void *socpwrapper(void *args) {
    copiar_t *ptr = (copiar_t *)args;
    socp(ptr->fonte, ptr->destino, ptr->buffsize); 
    
    // Registar no vetor de logs com data e hora
    time_t t;
    time(&t);
    char *tempo = ctime(&t);
    if (tempo[strlen(tempo) - 1] == '\n')
        tempo[strlen(tempo) - 1] = '\0';

    snprintf(strings[k % MAX], sizeof(strings[k % MAX]), "%s %s", tempo, ptr->fonte);
    k++;

    free(ptr);
    return NULL;
}

// Mostra os últimos MAX registos de cópias
void mostrarRelatorio() {
    printf("Registos de cópias efetuadas:\n");
    for (int i = 0; i < k && i < MAX; i++) {
        printf("%s\n", strings[i]);
    }
}
