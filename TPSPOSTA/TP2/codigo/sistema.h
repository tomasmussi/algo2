#ifndef SISTEMA_H
#define SISTEMA_H

#include <stdbool.h>
#include "hash.h"
#define MAX 255
#define JUGADORES 23

typedef struct sistema sistema_t;


/* Crea el sistema en el cual se hara ABM de equipos */
//	POST: sistema creado
sistema_t* sistema_crear();

/* Destruye el sistema */
//	PRE: el sistema fue creado
//	POST: sistema destruido
void sistema_destruir(sistema_t *sistema);

/* Carga un equipo con sus respectivos jugadores al sistema */
//	PRE: el sistema fue creado
//	POST: "equipo", con los 23 jugadores aniadidos
void sistema_cargar(sistema_t *sistema, const char *equipo, char jugadores[JUGADORES][MAX]);

/* Carga al sistema las llaves con las que comienza el toreno, en el orden que fueron
 * leidas del archivo */
//	PRE: el sistema fue creado y el hash contiene los 2^k equipos con sus respectivos ordenes de lectura
//	POST: Fixture cargado y listo para agregar resultados
void sistema_cargar_fixture(sistema_t *sistema, hash_t *equipos_fixture);

/* Agrega un resultado al fixture con id "idr" y actualiza los jugadores y goles de jugadores */
//	PRE: el sistema fue creado
//	POST: valida que el "idr" sea valido y procede a actualizar los goles e informa OK
//	Si el "idr" es invalido o ya se cargo el resultado, se informa por pantalla
void sistema_agregar_resultado(sistema_t *sistema, char *idr, char *goles_local, char *goles_visitante,
		char *goles_jugadores);

/* Muestra un resultado con "idr" */
//	PRE: el sistema fue creado
//	POST: se imprime por pantalla la informacion del partido "idr"
void sistema_mostrar_resultado(sistema_t *sistema,const char *idr);

/* Lista los jugadores ordenados por "tipo" de un equipo */
//	PRE: el sistema fue creado
//	POST: lista los jugadores de un equipo, si el equipo no existe se muestra mensaje de error
void sistema_listar_jugadores(sistema_t *sistema, char *tipo, char *equipo);

/* Muestra el goleador del campeonato */
//	PRE: el sistema fue creado
//	POST: muestra el goleador del campeonato
void sistema_ver_goleador(sistema_t *sistema);

/* Muestra los goles de un jugador de nombre = "nombre" */
//	PRE: el sistema fue creado
//	POST: informa sobre el jugador y en caso de no existir muestra mensaje de error
void sistema_goles_jugador(sistema_t *sistema, char *nombre);

#endif //SISTEMA_H

