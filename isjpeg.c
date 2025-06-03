#include <unistd.h>
#include <fcntl.h>

int isjpg(int fd) {
    unsigned char b[4];
    int n = read(fd, b, 4);
    lseek(fd, 0, SEEK_SET);  // Voltar ao início do ficheiro

    if (n == 4 && b[0] == 0xff && b[1] == 0xd8 && b[2] == 0xff &&
        (b[3] == 0xe0 || b[3] == 0xe1 || b[3] == 0xe2 || b[3] == 0xe8))
        return 1;

    return 0;
}
