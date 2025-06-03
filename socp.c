#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void ioCopy(int IN, int OUT) {
    char buf[4096];
    ssize_t n;
    while ((n = read(IN, buf, sizeof(buf))) > 0)
        write(OUT, buf, n);
}

void socp(char *fonte, char *destino) {
    int in = open(fonte, O_RDONLY);
    if (in < 0) { perror("Erro ao abrir fonte"); return; }

    int out = open(destino, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) { perror("Erro ao abrir destino"); close(in); return; }

    ioCopy(in, out);
    close(in);
    close(out);
}
