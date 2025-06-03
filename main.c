#include "shell.h"
#include <pthread.h>     
#include <stdlib.h>      
#include <string.h>     
#include <stdio.h>
#include <unistd.h>
#include <time.h>

extern char strings[MAX][130];
extern int k;

char prompt[100];

int main() {
    int len;
    char linha[1024];
    char *args[64];
    int nargs = 0;

    strcpy(prompt, "SOSHELL: Introduza um comando : prompt>");
    while (1) {
        printf("%s", prompt);
        if (fgets(linha, 1023, stdin) == NULL) {
            printf("\n");
            exit(0);
        }
        len = strlen(linha);
        if (1 == len)
            continue;
        if (linha[len - 1] == '\n')
            linha[len - 1] = '\0';

        nargs = parse(linha, args);
        if (!builtin(nargs, args))
            execute(nargs, args);
    }
    return 0;
}

int builtin(int nargs, char *args[]) {
    if (strcmp(args[0], "sair") == 0) {
        exit(0);
        return 1;
    }

    if (strncmp(args[0], "42", 2) == 0) {
        printf("42 is the answer to life the universe and everything\n");
        printf("https://pt.wikipedia.org/wiki/The_Hitchhiker's_Guide_to_the_Galaxy\n");
        return 1;
    }

    if (strcmp(args[0], "obterinfo") == 0) {
        printf("SO Shell 2025 versão 1.0\n");
        return 1;
    }

    if (strlen(args[0]) > 4 && strncmp(args[0], "PS1=", 4) == 0) {
        strcpy(prompt, args[0] + 4);
        return 1;
    }

    if (strcmp(args[0], "quemsoueu") == 0) {
        system("id");
        return 1;
    }

    if (strcmp(args[0], "cd") == 0) {
        int err;
        if (args[1] == NULL || strcmp(args[1], "~") == 0 || strcmp(args[1], "$HOME") == 0) {
            err = chdir(getenv("HOME"));
        } else {
            err = chdir(args[1]);
        }
        if (err < 0) perror(args[1]);
        return 1;
    }

    if (strcmp(args[0], "socp") == 0) {
        if (args[1] != NULL && args[2] != NULL) {
            socp(args[1], args[2]);
        } else {
            printf("Sintaxe incorreta: Uso: socp fonte destino\n");
        }
        return 1;
    }

    if (strcmp(args[0], "calc") == 0 && nargs == 4) {
        calc(args[1], args[2], args[3]);
        return 1;
    }

    if (strcmp(args[0], "bits") == 0 && nargs == 4) {
        bits(args[1], args[2], args[3]);
        return 1;
    }

    if (strcmp(args[0], "isjpeg") == 0 && args[1] != NULL) {
        int fd = atoi(args[1]);
        if (isjpg(fd))
            printf("Ficheiro com fd %d É um JPEG válido\n", fd);
        else
            printf("Ficheiro com fd %d NÃO é JPEG\n", fd);
        return 1;
    }

    if (strcmp(args[0], "isValid") == 0 && args[1] != NULL) {
        int fd = atoi(args[1]);
        printf("FD %d é %s válido\n", fd, fd_is_valid(fd) ? "" : "não");
        return 1;
    }

    if (strcmp(args[0], "openfile") == 0 && args[1] != NULL) {
        openfile(args[1]);
        return 1;
    }

    if (strcmp(args[0], "closefd") == 0 && args[1] != NULL) {
        closefd(atoi(args[1]));
        return 1;
    }

    if (strcmp(args[0], "read") == 0 && args[1] != NULL && args[2] != NULL) {
        readfile(args[1], args[2]);
        return 1;
    }

    if (strcmp(args[0], "fileinfo") == 0) {
        fileinfo();
        return 1;
    }

    if (strcmp(args[0], "avisoTeste") == 0) {
        if (args[1] == NULL || args[2] == NULL) {
            fprintf(stderr, "Uso: avisoTeste <mensagem> <tempo>\n");
            return 1;
        }
        aviso(args[1], atoi(args[2]));
        return 1;
    }

    if (strcmp(args[0], "avisoMAU") == 0) {
        pthread_t th;
        pthread_create(&th, NULL, avisowrapperMAU, (void *)args);
        pthread_detach(th);
        return 1;
    }

    if (strcmp(args[0], "aviso") == 0) {
        if (args[1] == NULL || args[2] == NULL) {
            fprintf(stderr, "Uso: aviso <mensagem> <tempo>\n");
            return 1;
        }
        aviso_t *ptr = malloc(sizeof(aviso_t));
        strcpy(ptr->msg, args[1]);
        ptr->tempo = atoi(args[2]);

        pthread_t th;
        pthread_create(&th, NULL, avisowrapper, (void *)ptr);
        pthread_detach(th);
        return 1;
    }

    if (strcmp(args[0], "socpthread") == 0) {
        if (args[1] == NULL || args[2] == NULL) {
            fprintf(stderr, "Uso: socpthread <fonte> <destino> [blksize]\n");
            return 1;
        }

        copiar_t *ptr = malloc(sizeof(copiar_t));
        strcpy(ptr->fonte, args[1]);
        strcpy(ptr->destino, args[2]);
        ptr->buffsize = (args[3] != NULL) ? atoi(args[3]) : 512;

        pthread_t th;
        pthread_create(&th, NULL, copiarwrapper, (void *)ptr);
        pthread_detach(th);
        return 1;
    }

    if (strcmp(args[0], "lnfoCopias") == 0) {
        for (int i = 0; i < k && i < MAX; i++) {
            printf("%s\n", strings[i]);
        }
        return 1;
    }

    if (strcmp(args[0], "maior") == 0 && args[1] && args[2]) {
        cmd_maior(args[1], args[2]);
        return 1;
    }
    
    if (strcmp(args[0], "setx") == 0 && args[1]) {
        cmd_setx(args[1]);
        return 1;
    }
    
    if (strcmp(args[0], "removerl") == 0 && args[1]) {
        cmd_removerl(args[1]);
        return 1;
    }
    
    if (strcmp(args[0], "sols") == 0) {
        cmd_sols(args[1]); // args[1] pode ser NULL
        return 1;
    }

    return 0; 
}
