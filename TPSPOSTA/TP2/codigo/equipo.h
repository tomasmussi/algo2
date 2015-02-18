#ifndef EQUIPO_H
#define EQUIPO_H

#include <stdbool.h>
#include "jugador.h"
#include "abb.h"

/* Definicion de la estructura del equipo */
typedef struct equipo equipo_t;

/* Funcion recibida para iterar los jugadores almacenados en el equipo */
typedef bool (*visitar_f)(const char*,void*,void*);

/* Crea un equipo nuevo con nombre */
//	POST: EL equipo fue creado con "nombre"
equipo_t* equipo_crear(const char *nombre);

/* Destruye el equipo creado */
//	PRE: El equipo fue creado
//	POST: Equipo destruido
void equipo_destruir(void *equipo);

/* Agregar un jugador al equipo */
//	PRE: El equipo y el jugador fueron creados
//	POST: Jugador agregado al equipo
void equipo_agregar_jugador(equipo_t *equipo, jugador_t *jugador);

/* Muestra todos los jugadores de un equipo */
//	PRE: El equipo fue creado, el tipo es "dorsal" o "nombre" y visitar es != NULL
//	POST: Itera los jugadores del equipo
void equipo_mostrar_jugadores(equipo_t *equipo, char *tipo, visitar_f);

/* Agrega un gol al jugador con "numero" correspondiente */
//	PRE: El equipo fue creado y numero es una camiseta valida entre 1 y 23
//	POST: Gol agregado
void equipo_agregar_gol(equipo_t *equipo, char *numero);

#endif //EQUIPO_H
