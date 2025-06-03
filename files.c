#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "shell.h"

// Verifica se um descritor de ficheiro está válido
int fd_is_valid(int fd) {
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

// Abre ficheiro para leitura e imprime o descritor
void openfile(char *nome) {
    int fd = open(nome, O_RDONLY);
    if (fd >= 0)
        printf("Aberto %s para leitura com descritor fd %d\n", nome, fd);
    else
        perror(nome);
}

// Fecha um descritor de ficheiro
void closefd(int fd) {
    if (close(fd) < 0)
        perror("closefd falhou");
    else
        printf("Descritor %d fechado com sucesso\n", fd);
}

// Lê bytes de um descritor e imprime em ASCII e hexadecimal
void readfile(char *fdstr, char *nbytesstr) {
    int fd = atoi(fdstr);
    int nbytes = atoi(nbytesstr);
    char buffer[2048];

    if (nbytes > 2048) nbytes = 2048;

    int lidos = read(fd, buffer, nbytes);
    if (lidos < 0) {
        perror("Erro ao ler");
        return;
    }

    printf("ASCII: ");
    for (int i = 0; i < lidos; i++) {
        if (buffer[i] >= 32 && buffer[i] <= 126)
            putchar(buffer[i]);
        else
            putchar('.');
    }
    printf("\nHex:   ");
    for (int i = 0; i < lidos; i++) {
        printf("%02x ", (unsigned char)buffer[i]);
    }
    printf("\n");
}

// Mostra informação sobre descritores de ficheiro abertos
void fileinfo() {
    if (stdout) {
        int outfd = fileno(stdout);
        printf("STDOUT está aberto: descritor %d\n", outfd);
    } else {
        printf("STDOUT está fechado.\n");
    }

    struct rlimit lim;
    if (getrlimit(RLIMIT_NOFILE, &lim) == 0) {
        printf("Limite de descritores de ficheiro do processo: %ld\n", lim.rlim_cur);
    }

    printf("Descritores abertos: ");
    int abertos = 0;
    for (int i = 0; i < 64; i++) {
        if (fd_is_valid(i)) {
            printf("%d ", i);
            abertos++;
        }
    }
    printf("\nTotal de ficheiros abertos: %d\n", abertos);
}

// Verifica se um ficheiro é JPEG a partir dos primeiros 4 bytes
int isjpg(int fd) {
    unsigned char b[4];
    int n = read(fd, b, 4);

    if (n < 4) {
        perror("Erro ao ler os bytes iniciais");
        return 0;
    }

    lseek(fd, 0, SEEK_SET); // voltar ao início do ficheiro

    if (b[0] == 0xFF && b[1] == 0xD8 && b[2] == 0xFF &&
        (b[3] == 0xE0 || b[3] == 0xE1 || b[3] == 0xE2 || b[3] == 0xE8)) {
        return 1;
    }

    return 0;
}
