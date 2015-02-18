CFLAGS=-g -Wall -pedantic -ansi -std=c99
OBJ=cola.o
MAIN=prueba_cola.c
EXEC=prueba_cola
CC=gcc

all: cola.o prueba_cola

cola.o: cola.c cola.h
	$(CC) $(CFLAGS) -c cola.c

prueba_cola:
	$(CC) $(CFLAGS) $(OBJ) $(MAIN) -o $(EXEC)

clean:
	rm -f *.o prueba_cola *~

valgrind:
	valgrind --leak-check=full ./$(EXEC)
