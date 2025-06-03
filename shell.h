#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/wait.h>
#include <time.h>

#define MAX 100
#define BG 0
#define FG 1

extern char strings[MAX][130];
extern int k;

typedef struct {
    char msg[100];
    int tempo;
} aviso_t;

typedef struct {
    char fonte[100];
    char destino[100];
    int buffsize;
} copiar_t;

void aviso(char *mesg, int tempo);
void *avisowrapperMAU(void *args);
void *avisowrapper(void *args);
void *copiarwrapper(void *args);

int parse(char *buf, char **args);
void execute(int numargs, char **args);
int builtin(int nargs, char *args[]);
void socp(char *fonte, char *destino); // versão básica
int ultimo(int *numargs, char **args);
int isjpg(int fd);
int fd_is_valid(int fd);
void openfile(char *filename);
void closefd(int fd);
void readfile(char *fdStr, char *nStr);
void fileinfo();
void calc(char *value1, char *op, char *value2);
void bits(char *op1, char *op, char *op2);
int redirects(int numargs, char *args[]);
void cmd_maior(char *file1, char *file2);
void cmd_setx(char *file);
void cmd_removerl(char *file);
void cmd_sols(char *dir);

#endif
