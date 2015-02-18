CFLAGS=-g -Wall -pedantic -ansi -std=c99
OBJ=lista.o pila.o sistema.o sesion.o curso.o
MAIN=main.c
EXEC=tp1
CC=gcc

all: pila.o lista.o sistema.o sesion.o tp

pila.o: pila.c pila.h
	$(CC) $(CFLAGS) -c pila.c

lista.o: lista.c lista.h
	$(CC) $(CFLAGS) -c lista.c

curso.o: curso.c curso.h
	$(CC) $(CFLAGS) -c curso.c

sesion.o: sesion.c sesion.h
	$(CC) $(CFLAGS) -c sesion.c

sistema.o: curso.o lista.o sesion.o sistema.c sistema.h
	$(CC) $(CFLAGS) -c sistema.c

tp:
	$(CC) $(CFLAGS) $(OBJ) $(MAIN) -o $(EXEC)

exe:
	./$(EXEC) <input

clean:
	clear
	rm -f $(OBJ) $(EXEC) *~
valgrind:
	valgrind --leak-check=full --track-origins=yes ./$(EXEC) < input 
