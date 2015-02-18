#ifndef JUGADOR_H
#define JUGADOR_H

#include <stdbool.h>

typedef struct jugador jugador_t;

/* Crea un jugador con "nombre", que pertenece a "equipo" y con camiseta = "numero" */
//	PRE: nombre de equipo valido y numero tambien debe ser valido
//	POST: Devuelve un jugador con caracteristicas mencionadas
jugador_t* jugador_crear(const char *nombre, const char *equipo, int numero);

/* Destruye el jugador creado */
//	PRE: jugador fue creado
//	POST: jugador destruido
void jugador_destruir(void *jugador);

/* Obtiene una clave unica para guardar al jugador en un hash */
//	PRE: El jugador fue creado
//	POST: clave obtenida
const char* jugador_obtener_clave(const jugador_t *jugador);

/* Obtiene el nombre del jugador */
//	PRE: El jugador fue creado
//	POST: nombre obtenido y no se puede modificar
const char* jugador_ver_nombre(const jugador_t *jugador);

/* Obtiene el nombre del equipo al cual pertenece el jugador */
//	PRE: el jugador fue creado
//	POST: equipo obtenido
const char* jugador_ver_equipo(const jugador_t *jugador);

/* Obtiene el numero de camiseta del jugador */
//	PRE: el jugador fue creado
//	POST: camiseta obtenida
int jugador_ver_numero(const jugador_t *jugador);

/* Obtiene los goles de un jugador */
//	PRE: el jugador fue creado
//	POST: cantidad de goles obtenida
int jugador_ver_goles(jugador_t *jugador);

/* Suma un gol al jugador */
//	PRE: El jugador fue creado
//	POST: Se sumo un gol al jugador
void jugador_agregar_gol(jugador_t *jugador);

#endif //JUGADOR_H
