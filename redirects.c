// redirects.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "shell.h"

#define FILE_MODE (S_IRUSR | S_IWUSR) // Permissões: leitura e escrita para o utilizador

// Fecha fdold, duplica fd sobre fdold, fecha fd e anula símbolo de redirecionamento
// Pré-condição: numargs >= 3, e args[numargs - 2] contém o símbolo de redirecionamento
int dupandclose(int fd, int fdold, int numargs, char *args[]) {
    if (fd < 0 || fdold < 0) {
        perror("Descritores inválidos");
        exit(1);
    }
    if (dup2(fd, fdold) < 0) {
        perror("Erro ao duplicar descritor");
        close(fd);
        exit(1);
    }
    close(fd);
    args[numargs - 2] = NULL; // remover símbolo
    return numargs - 2;
}

// Trata redirecionamentos de entrada, saída e erro padrão
int redirects(int numargs, char *args[]) {
    // 1. Redirecionamento de STDERR: 2>
    if (numargs >= 3 && strcmp(args[numargs - 2], "2>") == 0) {
        int fd = creat(args[numargs - 1], FILE_MODE);
        numargs = dupandclose(fd, STDERR_FILENO, numargs, args);
    }

    // 2. Redirecionamento de STDOUT: > ou >>
    else if (numargs >= 3 && strcmp(args[numargs - 2], ">") == 0) {
        int fd = creat(args[numargs - 1], FILE_MODE);
        numargs = dupandclose(fd, STDOUT_FILENO, numargs, args);
    } else if (numargs >= 3 && strcmp(args[numargs - 2], ">>") == 0) {
        int fd = open(args[numargs - 1], O_CREAT | O_APPEND | O_WRONLY, FILE_MODE);
        numargs = dupandclose(fd, STDOUT_FILENO, numargs, args);
    }

    // 3. Redirecionamento de STDIN: <
    else if (numargs >= 3 && strcmp(args[numargs - 2], "<") == 0) {
        int fd = open(args[numargs - 1], O_RDONLY);
        numargs = dupandclose(fd, STDIN_FILENO, numargs, args);
    }

    return numargs;
}
