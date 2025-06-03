#include "shell.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int containsPipe(int argc, char **argv) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "|") == 0) {
            return i;
        }
    }
    return -1;
}

void execute(int numargs, char **args) {
    int pid, status;
    int code = ultimo(&numargs, args);
    int pipeIndex = containsPipe(numargs, args);

    if (pipeIndex == -1) {
        // NÃO TEM PIPE: comportamento normal
        if ((pid = fork()) < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            numargs = redirects(numargs, args);
            if (numargs == -1) exit(1);

            execvp(*args, args);
            perror(*args);
            exit(1);
        }

        if (FG == code)
            while (wait(&status) != pid);
        return;
    }

    // TEM PIPE: fazer dois fork()s e pipe()
    args[pipeIndex] = NULL;
    char **comando1 = args;
    char **comando2 = &args[pipeIndex + 1];

    int fd[2];
    pipe(fd);

    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // FILHO
        int pid_neto = fork();
        if (pid_neto == 0) {
            // NETO: executa comando1 → escreve no pipe
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);

            execvp(comando1[0], comando1);
            perror("execvp neto");
            _exit(1);
        } else {
            // FILHO: executa comando2 ← lê do pipe
            wait(NULL);

            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);

            execvp(comando2[0], comando2);
            perror("execvp filho");
            _exit(1);
        }
    }

    if (FG == code)
        while (wait(&status) != pid);
}

int ultimo(int *numargs, char **args) {
    if (args[*numargs - 1][0] == '&') {
        *numargs = *numargs - 1;
        args[*numargs] = NULL;
        return BG;
    }
    return FG; // return FG ou BG definidos no shell.h
}
