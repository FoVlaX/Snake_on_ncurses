CC=gcc

CFLAGS=-lncurses

all:
	$(CC) snake.c $(CFLAGS) -o snake.o 
	./snake.o

