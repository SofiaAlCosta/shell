#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

// Função auxiliar para copiar dados entre dois descritores com bloco de tamanho definido
void ioCopy(int IN, int OUT, int blksize) {
    char buffer[blksize];
    ssize_t bytes;

    while ((bytes = read(IN, buffer, blksize)) > 0) {
        if (write(OUT, buffer, bytes) < 0) {
            perror("Erro ao escrever no destino");
            break;
        }
    }

    if (bytes < 0) {
        perror("Erro ao ler do ficheiro fonte");
    }
}

// Função principal do comando socp
void socp(char *fonte, char *destino, int blksize) {
    int fdin = open(fonte, O_RDONLY);
    if (fdin < 0) {
        perror("Erro ao abrir ficheiro fonte");
        return;
    }

    int fdout = open(destino, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fdout < 0) {
        perror("Erro ao criar ficheiro destino");
        close(fdin);
        return;
    }

    ioCopy(fdin, fdout, blksize);

    close(fdin);
    close(fdout);
}
