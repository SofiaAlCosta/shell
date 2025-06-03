#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <sys/types.h>

// Compara o tamanho de dois ficheiros
void cmd_maior(char *file1, char *file2) {
    struct stat st1, st2;

    if (stat(file1, &st1) == -1) {
        perror("Erro ao aceder ao primeiro ficheiro");
        return;
    }
    if (stat(file2, &st2) == -1) {
        perror("Erro ao aceder ao segundo ficheiro");
        return;
    }

    double size1 = st1.st_size / 1024.0;
    double size2 = st2.st_size / 1024.0;

    if (st1.st_size >= st2.st_size) {
        printf("Maior ficheiro: %s (%.2f KB)\n", file1, size1);
    } else {
        printf("Maior ficheiro: %s (%.2f KB)\n", file2, size2);
    }
}

// Dá permissão de execução ao dono de um ficheiro
void cmd_setx(char *file) {
    struct stat st;

    if (stat(file, &st) == -1) {
        perror("Erro ao aceder ao ficheiro");
        return;
    }

    mode_t modo_novo = st.st_mode | S_IXUSR;
    if (chmod(file, modo_novo) == -1) {
        perror("Erro ao alterar permissões");
    } else {
        printf("Permissão de execução atribuída ao dono de %s\n", file);
    }
}

// Remove permissões de leitura de grupo e outros
void cmd_removerl(char *file) {
    struct stat st;

    if (stat(file, &st) == -1) {
        perror("Erro ao aceder ao ficheiro");
        return;
    }

    mode_t modo_novo = st.st_mode & ~(S_IRGRP | S_IROTH);
    if (chmod(file, modo_novo) == -1) {
        perror("Erro ao remover permissões de leitura");
    } else {
        printf("Permissões de leitura removidas para grupo e outros em %s\n", file);
    }
}

// Lista ficheiros de um diretório com inode, tamanho e data de modificação
void cmd_sols(char *dir) {
    DIR *d;
    struct dirent *entry;
    struct stat st;
    char path[1024];

    if (dir == NULL)
        dir = ".";

    d = opendir(dir);
    if (!d) {
        perror("Erro ao abrir diretoria");
        return;
    }

    printf("%-25s %-10s %-11s %s\n", "Nome", "Inode", "Tamanho", "Última modificação");

    while ((entry = readdir(d)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);
        if (stat(path, &st) == 0) {
            char *tempo = ctime(&st.st_mtime);
            tempo[strcspn(tempo, "\n")] = '\0';  // remover newline

            printf("%-25s %-10lu %-11ld %s\n",
                   entry->d_name, st.st_ino, st.st_size, tempo);
        }
    }

    closedir(d);
}
