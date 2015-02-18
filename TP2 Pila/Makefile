CFLAGS=-g -Wall -pedantic -ansi -std=c99
CC=gcc
EXEC=prueba_pila.exe
OBJ=pila.o

pila.o: pila.c pila.h
	$(CC) $(CFLAGS) -c pila.c

$(EXEC): $(OBJ) prueba_pila.c
	$(CC) $(CFLAGS) $(OBJ) prueba_pila.c -o $(EXEC)

clean:
	rm -f *.o $(EXEC) *~
