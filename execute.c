#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "shell.h"

// Verifica se o comando termina com '&' e remove esse argumento
int ultimo(int *numargs, char **args) {
    if (*numargs > 0 && args[*numargs - 1][0] == '&') {
        (*numargs)--;
        args[*numargs] = NULL;
        return BG;
    }
    return FG;
}

// Verifica se existe pipe e devolve o índice da posição do '|'
int containsPipe(int numArgs, char **args) {
    for (int i = 0; i < numArgs; i++) {
        if (strcmp(args[i], "|") == 0)
            return i;
    }
    return -1;
}

// Executa recursivamente comandos com pipes
void execute_rec(int numargs, char **args, int input_fd) {
    int pipeIndex = containsPipe(numargs, args);

    if (pipeIndex == -1) {
        // Último comando da cadeia
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        args[numargs] = NULL;  
        numargs = redirects(numargs, args);

        execvp(args[0], args);
        perror(args[0]);
        exit(1);
    }

    // Divide os argumentos entre os dois lados do pipe
    args[pipeIndex] = NULL;
    int numargs1 = pipeIndex;
    char **args1 = args;

    int numargs2 = numargs - pipeIndex - 1;
    char **args2_raw = &args[pipeIndex + 1];

    // Criação de novo array para args2 (lado direito do pipe)
    char *args2[64];
    for (int i = 0; i < numargs2; i++) {
        args2[i] = args2_raw[i];
    }
    args2[numargs2] = NULL;

    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // Processo filho: comando à esquerda do pipe
        dup2(fd[1], STDOUT_FILENO);
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        close(fd[0]);
        close(fd[1]);

        numargs1 = redirects(numargs1, args1);
        execvp(args1[0], args1);
        perror(args1[0]);
        exit(1);
    }

    // Processo pai continua recursivamente com o lado direito
    close(fd[1]);
    if (input_fd != STDIN_FILENO)
        close(input_fd);

    execute_rec(numargs2, args2, fd[0]);
}

// Função principal que executa comandos (com ou sem pipes, foreground/background)
void execute(int numargs, char **args) {
    int code = ultimo(&numargs, args);
    pid_t pid = fork();
    int status;

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        execute_rec(numargs, args, STDIN_FILENO);
        exit(1);
    }

    if (code == FG) {
        while (wait(&status) != pid);
    } else {
        printf("[BG] Processo iniciado com PID: %d\n", pid);
    }
}
