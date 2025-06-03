#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/resource.h>

int fd_is_valid(int fd) {
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

void openfile(char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd >= 0)
        printf("Aberto %s com fd %d\n", filename, fd);
    else
        perror(filename);
}

void closefd(int fd) {
    if (close(fd) < 0)
        perror("Erro ao fechar fd");
    else
        printf("%d fechado com sucesso\n", fd);
}

void readfile(char *fdStr, char *nStr) {
    int fd = atoi(fdStr);
    int n = atoi(nStr);
    char buf[n];

    int r = read(fd, buf, n);
    if (r <= 0) {
        perror("read");
        return;
    }

    printf("%.*s\n", r, buf);
    for (int i = 0; i < r; i++) printf("%02x ", (unsigned char)buf[i]);
    printf("\n");
}

void fileinfo() {
    struct rlimit rl;
    getrlimit(RLIMIT_NOFILE, &rl);

    FILE *out = stdout;
    int fd_out = fileno(out);
    printf("STDOUT está %s: número %d\n", out ? "aberto" : "fechado", fd_out);
    printf("Limite máximo de descritores: %ld\n", rl.rlim_cur);

    int total = 0;
    printf("Descritores abertos: ");
    for (int i = 0; i < rl.rlim_cur; i++) {
        if (fd_is_valid(i)) {
            printf("%d ", i);
            total++;
        }
    }
    printf("\nTotal abertos: %d\n", total);
}

typedef unsigned short tipo;

void printBits(tipo numero, tipo mascara) {
    while (mascara) {
        putchar((numero & mascara) ? '1' : '0');
        mascara >>= 1;
    }
}

void displayBitOps(tipo a, tipo b) {
    tipo mask = 0x8000;
    printf("A: "); printBits(a, mask); printf(" (%u)\n", a);
    printf("B: "); printBits(b, mask); printf(" (%u)\n", b);
    printf("A&B: "); printBits(a & b, mask); printf(" (%u)\n", a & b);
    printf("A|B: "); printBits(a | b, mask); printf(" (%u)\n", a | b);
    printf("A^B: "); printBits(a ^ b, mask); printf(" (%u)\n", a ^ b);
    printf("A&~B: "); printBits(a & ~b, mask); printf(" (%u)\n", a & ~b);
}

