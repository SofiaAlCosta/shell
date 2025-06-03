# Makefile para SoShell

CC = gcc
CFLAGS = -Wall -g
LIBS = -lm -lpthread

OBJS = main.o parse.o execute.o calc.o files.o socp.o redirects.o isjpeg.o threads.o fileutils.o

all: soshell

soshell: $(OBJS)
	$(CC) -o soshell $(OBJS) $(LIBS)

main.o: shell.h main.c
	$(CC) $(CFLAGS) -c main.c

execute.o: shell.h execute.c
	$(CC) $(CFLAGS) -c execute.c

parse.o: shell.h parse.c
	$(CC) $(CFLAGS) -c parse.c

calc.o: shell.h calc.c
	$(CC) $(CFLAGS) -c calc.c

files.o: shell.h files.c
	$(CC) $(CFLAGS) -c files.c

isjpeg.o: shell.h isjpeg.c
	$(CC) $(CFLAGS) -c isjpeg.c

socp.o: shell.h socp.c
	$(CC) $(CFLAGS) -c socp.c

threads.o: shell.h threads.c
	$(CC) $(CFLAGS) -c threads.c

fileutils.o : shell.h fileutils.c
	$(CC) $(FLAGS) -c fileutils.c

clean:
	rm -f *.o soshell *~
	echo "Removidos ficheiros objeto, executáveis e temporários."
