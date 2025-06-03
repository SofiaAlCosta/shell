#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

// Função para detetar o pipe
int containsPipe (int numArgs, char **args) {
    for (int index = 0; index < numArgs; index++){
        if ('|' == args[index][0]) {
            return index;
        }
    }
    return -1;
}

int main () {
    // Pode testar qualquer um dos vetores:
    char *myargs1[] = {"ls", "-l", "-a", NULL};                  // 3 argumentos
    char *myargs2[] = {"ls", "-l", "-a", "|", "wc", "-c", NULL}; // 6 argumentos + '|'
    char *myargs3[] = {"cat", "-t", "/etc/passwd", "|", "grep", "-v", "root", NULL}; // 7

    char **args = myargs3;       // Escolher o vetor a testar
    int numArgs = 7;             // Atualizar consoante o vetor

    int indice = containsPipe(numArgs, args);

    if (indice > 0) {
        int fd[2];
        printf("pipe detected at index %d\n", indice);
        printf("Remove PIPE symbol. Create Pipe. Fork(). Exec in 2 Processes\n");

        args[indice] = NULL;  // Remove o símbolo do pipe
        pipe(fd);

        int pidFilho = fork();
        if (0 == pidFilho) {
            // Processo filho: escreve no pipe (comando antes do '|')
            numArgs = indice; // Número de argumentos antes do pipe
            fprintf(stderr, "cmd write to pipe: <%s> nºargs=%d\n", args[0], numArgs);

            dup2(fd[1], STDOUT_FILENO); // Redireciona STDOUT para escrita no pipe
            close(fd[0]); close(fd[1]);

            execvp(args[0], args);
            perror("execvp filho");
        } else {
            // Processo pai: lê do pipe (comando depois do '|')
            wait(NULL); // Espera que o filho termine

            args = args + indice + 1;
            numArgs = numArgs - indice - 1;
            fprintf(stderr, "cmd read from pipe: <%s> nºArgs=%d\n", args[0], numArgs);

            dup2(fd[0], STDIN_FILENO); // Redireciona STDIN para ler do pipe
            close(fd[0]); close(fd[1]);

            execvp(args[0], args);
            perror("execvp pai");
        }
    } else {
        execvp(args[0], args);
        perror("execvp sem pipe");
    }

    return 0;
}
