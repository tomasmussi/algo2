CFLAGS=-g -Wall -pedantic -ansi -std=c99
OBJ=lista.o
MAIN=prueba_lista.c
EXEC=prueba_lista
CC=gcc

all: lista.o prueba_lista

lista.o: lista.c lista.h
	$(CC) $(CFLAGS) -c lista.c

prueba_lista:
	$(CC) $(CFLAGS) $(OBJ) $(MAIN) -o $(EXEC)

clean:
	clear
	rm -f *.o prueba_lista *~
valgrind:
	valgrind --leak-check=full ./$(EXEC)
