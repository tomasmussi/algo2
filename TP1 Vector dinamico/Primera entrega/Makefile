CFLAGS=-g -Wall -pedantic -ansi -std=c99
CC=gcc
EXEC=prueba_vector
OBJ=vector_dinamico.o

all: vector.o prueba_vector

vector.o: vector_dinamico.c vector_dinamico.h
	$(CC) $(CFLAGS) -c vector_dinamico.c

prueba_vector:
	$(CC) $(CFLAGS) $(OBJ) prueba_vector.c -o $(EXEC)

clean:
	rm -f *.o $(EXEC) *~
