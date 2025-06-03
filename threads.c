#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "shell.h"

char strings[MAX][130];
int k = 0;

void aviso(char *mesg, int tempo) {
    while (tempo > 0) {
        sleep(1);
        tempo--;
    }
    fprintf(stderr, "Aviso : %s\n", mesg);
}

void *avisowrapperMAU(void *args) {
    char **pargs = (char **)args;
    aviso(pargs[1], atoi(pargs[2]));
    return NULL;
}

void *avisowrapper(void *args) {
    aviso_t *ptr = (aviso_t *)args;
    aviso(ptr->msg, ptr->tempo);
    free(ptr);
    return NULL;
}

void *copiarwrapper(void *args) {
    copiar_t *ptr = (copiar_t *)args;

    socp(ptr->fonte, ptr->destino);

    time_t tempoAtual;
    time(&tempoAtual);
    char *t = ctime(&tempoAtual);
    if (t[strlen(t)-1] == '\n') t[strlen(t)-1] = '\0';

    snprintf(strings[k++ % MAX], 130, "%s %s", t, ptr->destino);

    free(ptr);
    return NULL;
}
