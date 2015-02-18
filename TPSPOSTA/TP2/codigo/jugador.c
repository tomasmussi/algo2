#include <stdlib.h>
#include <string.h>

#define SEPARADOR |

#include "jugador.h"

/* Estructura interna del jugador */
struct jugador{
	char *nombre;
	char *equipo;
	int numero;
	int goles;
};

/* Crea un jugador con "nombre", que pertenece a "equipo" y con camiseta = "numero" */
jugador_t* jugador_crear(const char *nombre, const char *equipo, int numero) {
	jugador_t *jugador = malloc(sizeof(jugador_t));
	if (jugador == NULL)
		return NULL;
	char *copia_nombre = malloc((strlen(nombre) + 1) * sizeof(char));
	if (copia_nombre == NULL) {
		free(jugador);
		return NULL;
	}	
	char *copia_equipo = malloc((strlen(equipo) + 1) * sizeof(char));
	if (copia_equipo == NULL){
		free(jugador);
		free(copia_nombre);
		return NULL;
	}
	strcpy(copia_nombre, nombre);
	strcpy(copia_equipo, equipo);
	jugador->nombre = copia_nombre;
	jugador->equipo = copia_equipo;
	jugador->numero = numero;
	jugador->goles = 0;
	return jugador;
}

/* Destruye el jugador creado */
void jugador_destruir(void *jugador){
	jugador_t *jug = (jugador_t*) jugador;
	free(jug->nombre);
	free(jug->equipo);
	free(jug);
}

/* Obtiene una clave unica para guardar al jugador en un hash */
const char* jugador_obtener_clave(const jugador_t *jugador) {
	return jugador->nombre;
}

/* Devuelve el nombre del jugador */
const char* jugador_ver_nombre(const jugador_t *jugador){
	return jugador->nombre;
}

/* Obtiene el nombre del equipo al cual pertenece el jugador */
const char* jugador_ver_equipo(const jugador_t *jugador){
	return jugador->equipo;
}

/* Obtiene el numero de camiseta del jugador */
int jugador_ver_numero(const jugador_t *jugador){
	return jugador->numero;
}

/* Obtiene los goles de un jugador */
int jugador_ver_goles(jugador_t *jugador){
	return jugador->goles;
}

void jugador_agregar_gol(jugador_t *jugador){
	jugador->goles++;
}
