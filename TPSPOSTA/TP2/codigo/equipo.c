#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "equipo.h"
#include "abb.h"
#include "jugador.h"

/* Estructura interna del equipo */
struct equipo {
	char *nombre;
	abb_t *jugadores_numero;
	abb_t *jugadores_nombre;
};

/* Funcion para comparar los numeros de 2 jugadores almacenadas como char* */
int comparar_jugadores(const char *s1, const char *s2) {
	return atoi(s1) - atoi(s2);
}


/* Crea un equipo nuevo con nombre */
equipo_t* equipo_crear(const char *nombre){	
	equipo_t *equipo = malloc(sizeof(equipo_t));
	if (equipo == NULL)
		return NULL;
	char *copia = malloc((strlen(nombre) + 1) * sizeof(char));
	if (copia == NULL){
		free(equipo);
		return NULL;
	}
	abb_t *jug_numero = abb_crear(comparar_jugadores, NULL);
	if (jug_numero == NULL) {
		free(copia);
		free(equipo);
		return NULL;
	}
	abb_t *jug_nombre = abb_crear(strcmp, NULL);
	if (jug_nombre == NULL) {
		free(copia);
		free(equipo);
		abb_destruir(jug_nombre);
		return NULL;
	}
	strcpy(copia, nombre);
	equipo->nombre = copia;
	equipo->jugadores_numero = jug_numero;
	equipo->jugadores_nombre = jug_nombre;
	return equipo;
}

/* Destruye el equipo creado */
void equipo_destruir(void *equipo) {
	equipo_t *eq = (equipo_t*) equipo;
	abb_destruir(eq->jugadores_numero);
	abb_destruir(eq->jugadores_nombre);
	free(eq->nombre);
	free(eq);
}

/* Agregar un jugador al equipo */
void equipo_agregar_jugador(equipo_t *equipo, jugador_t *jugador) {
	abb_guardar(equipo->jugadores_nombre, jugador_ver_nombre(jugador), jugador);
	char clave[2];
	sprintf(clave, "%02d",jugador_ver_numero(jugador));
	abb_guardar(equipo->jugadores_numero, clave, jugador);
}

/* Muestra todos los jugadores de un equipo */
void equipo_mostrar_jugadores(equipo_t *equipo, char *tipo, visitar_f visitar) {
	if (strcmp(tipo, "dorsal") == 0) {
		abb_in_order(equipo->jugadores_numero, visitar, NULL);
	} else if (strcmp(tipo, "nombre") == 0) {
		abb_in_order(equipo->jugadores_nombre, visitar, NULL);
	}
}

/* Agrega un gol al jugador con "numero" correspondiente */
void equipo_agregar_gol(equipo_t *equipo, char *numero){
	jugador_t *jugador = abb_obtener(equipo->jugadores_numero, numero);
	jugador_agregar_gol(jugador);
}

