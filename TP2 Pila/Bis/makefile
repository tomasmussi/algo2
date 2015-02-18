CFLAGS=-g -Wall -pedantic -ansi -std=c99
CC=gcc
EXEC=prueba_pila.exe
OBJ=pila.o

all: pila.o ejecutable

pila.o: pila.c pila.h
	$(CC) $(CFLAGS) -c pila.c

ejecutable: $(OBJ) prueba_pila.c
	$(CC) $(CFLAGS) $(OBJ) prueba_pila.c -o $(EXEC)

clean:
	rm -f *.o $(EXEC) *~

valgrind:
	valgrind --leak-check=full ./$(EXEC)
