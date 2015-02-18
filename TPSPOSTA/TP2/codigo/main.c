#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"
#include "hash.h"

#define MAX 255
#define MAX_PALABRA 30
#define MAX_PARAMETROS 20
#define EQUIPO_ENTERO 24
#define JUGADORES 23

/* Funcion que se encarga de parsear y ejecutar un comando recibido */
void parsear(sistema_t *sistema, char *comando);

/* Funcion que evalua entre las distintas funciones y ejecuta la que corresponda */
void ejecutar_funcion(sistema_t *sistema, char *funcion, char parametros[][MAX_PALABRA]);

/* Funcion que se encarga de tokenizar los parametros de acuerdo a distintos tokens y cantidad de parametros */
void tokenizar(char parametros[MAX_PARAMETROS][MAX_PALABRA], const char *comando, int i,
		int parametros_esperados, const char token);


void ejecutar_funcion(sistema_t *sistema, char funcion[], char parametros[][MAX_PALABRA]) {
	if (strcmp(funcion, "agregar_resultado") == 0) {
		sistema_agregar_resultado(sistema, parametros[0], parametros[1], parametros[2], parametros[3]);
	} else if (strcmp(funcion, "listar_jugadores") == 0) {
		sistema_listar_jugadores(sistema, parametros[0], parametros[1]);
	} else if (strcmp(funcion, "listar_goleador") == 0) {
		sistema_ver_goleador(sistema);
	} else if (strcmp(funcion, "goles_jugador") == 0) {
		sistema_goles_jugador(sistema, parametros[0]);
	} else if (strcmp(funcion, "mostrar_resultado") == 0) {
		sistema_mostrar_resultado(sistema, parametros[0]);
	}
}

void parsear(sistema_t *sistema, char *comando) {
	char funcion[MAX_PALABRA];
	int i = 0;
	//Parseo la funcion primero
	while (comando[i] != ' ' && comando[i] != '\n' && i < MAX_PALABRA) {
		funcion[i] = comando[i];
		i++;
	}
	funcion[i++] = '\0';
	char token = ',';
	int parametros_esperados = MAX_PARAMETROS;
	if (strcmp(funcion, "agregar_resultado") == 0){
		parametros_esperados = 3;
	} else if (strcmp(funcion, "listar_jugadores") == 0){
		token = ' ';
		parametros_esperados = 1;
	}

	//Tengo que parsear los parametros separados por ',', si los hay
	char parametros[MAX_PARAMETROS][MAX_PALABRA];
	tokenizar(parametros, comando, i, parametros_esperados, token);
	ejecutar_funcion(sistema,funcion, parametros);
}

void tokenizar(char parametros[MAX_PARAMETROS][MAX_PALABRA], const char *comando, int i,
		int parametros_esperados, const char token){
		int cantidad_parametros = 0;
		int subindice = 0;
		while (comando[i] != '\n' && i <= MAX && (cantidad_parametros <= parametros_esperados - 1)) {
			if (comando[i] != token) {
				parametros[cantidad_parametros][subindice++] = comando[i];
			} else {
				parametros[cantidad_parametros++][subindice] = '\0';
				subindice = 0;
			}
			i++;
		}
		while (comando[i] != '\n' && i <= MAX){
			parametros[cantidad_parametros][subindice] = comando[i];
			subindice++;
			i++;
		}
		parametros[cantidad_parametros][subindice] = '\0';
}

/* Lee el archivo pasado por parametro al main */
int leer_archivo_binario(char *path, sistema_t *sistema){
	FILE *archivo = fopen(path, "rb");
	int caracter;
	int indice = 0;
	int jugador = 0;
	char equipo[MAX];
	char jugadores[JUGADORES][MAX];
	int vector[32];
	int i;
	for (i = 0; i < 32; i++)
		vector[i] = i;
	i = 0;
	hash_t* equipos_fixture = hash_crear(NULL);
	while ((caracter = fgetc(archivo)) != EOF) {
		//Mientras tenga caracteres para leer y no sea fin de archivo, leo
		char buffer[MAX];
		while (caracter != EOF && caracter != '\n') {
			buffer[indice++] = caracter;
			caracter = fgetc(archivo);
		}
		buffer[indice] = '\0';
		if (jugador == 0) {
			strcpy(equipo, buffer);
			hash_guardar(equipos_fixture, equipo, vector + i);
			i++;
		}
		else
			strcpy(jugadores[jugador - 1], buffer);
		if (++jugador == EQUIPO_ENTERO) {
			sistema_cargar(sistema, equipo, jugadores);
			jugador = 0;
		}
		indice = 0;
	}
	sistema_cargar_fixture(sistema, equipos_fixture);
	hash_destruir(equipos_fixture);
	return fclose(archivo);
}

/* Main del sistema de futbol */
int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Error\nUso: ./(EXE) <path de archivo que contiene equipos>");
		return 1;
	}
	//Creo el sistema
	sistema_t *sistema = sistema_crear();
	if (leer_archivo_binario(argv[1], sistema) == EOF) {
		printf("Hubo un error leyendo el archivo\n");
		return 2;
	}
	char buffer[MAX];
	while (fgets(buffer, MAX, stdin) != NULL) {
		parsear(sistema, buffer);
	}
	//Destruyo el sistema
	sistema_destruir(sistema);
	return 0;
}
