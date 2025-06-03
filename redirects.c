#include "shell.h"
#define FILE_MODE (S_IRUSR | S_IWUSR)

int dupandclose(int fd, int fdold, int numargs, char *args[]) {
    if (fd < 0 || fdold < 0) {
        perror("Invalid fds");
        exit(1);
    }
    dup2(fd, fdold);
    close(fd);
    args[numargs - 2] = NULL;
    return numargs - 2;
}

int redirects(int numargs, char *args[]) {
    // 1. STDERR: 2>
    if (numargs >= 3 && strcmp(args[numargs - 2], "2>") == 0) {
        int fd = creat(args[numargs - 1], FILE_MODE);
        numargs = dupandclose(fd, STDERR_FILENO, numargs, args);
    }

    // 2. STDOUT: > ou >>
    if (numargs >= 3 && strcmp(args[numargs - 2], ">") == 0) {
        int fd = creat(args[numargs - 1], FILE_MODE);
        numargs = dupandclose(fd, STDOUT_FILENO, numargs, args);
    } else if (numargs >= 3 && strcmp(args[numargs - 2], ">>") == 0) {
        int fd = open(args[numargs - 1], O_CREAT | O_APPEND | O_WRONLY, FILE_MODE);
        numargs = dupandclose(fd, STDOUT_FILENO, numargs, args);
    }

    // 3. STDIN: <
    if (numargs >= 3 && strcmp(args[numargs - 2], "<") == 0) {
        int fd = open(args[numargs - 1], O_RDONLY);
        numargs = dupandclose(fd, STDIN_FILENO, numargs, args);
    }

    return numargs;
}
