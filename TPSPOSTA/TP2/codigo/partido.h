#ifndef PARTIDO_H
#define PARTIDO_H

#include <stdbool.h>

typedef struct partido partido_t;

/* Crea un partido */
//	POST: devuelve un partido para llenar informacion
partido_t* partido_crear();

/* Destruye un partido */
//	PRE: el partido fue creado
//	POST: partido destruido
void partido_destruir(partido_t* partido);

/* Agrega el nombre de un equipo al partido como local o visitante */
bool partido_agregar_equipo(partido_t* partido,const char *equipo, bool local);

/* Imprime por pantalla el resultado del partido */
//	PRE: el partido fue creado y el idr es valido
//	POST: impresa informacion por pantalla
void partido_resultado(partido_t* partido,const char* idr);

/* Agrega un resultado a un partido */
//	PRE: el partido fue creado
//	POST: si el partido ya se cargo, devuelve false,
//	si el partido ya tiene cargados los equipos, pasar g1 = NULL y g2 = NULL
//	Devuelve true si pudo cargar exitosamente el resultado
bool partido_agregar_resultado(partido_t* partido,const char *g1, int goles_local,const char *g2, int goles_visitante);

/* Consulta si un partido se jugo o no */
//	PRE: el partido fue creado
//	POST: devuelve true si el partido se jugo, false de lo contrario
bool partido_jugado(partido_t *partido);

/* Devuelve el nombre del equipo que gano el partido */
//	PRE: el partido fue creado y se cargo su resultado correspondiente
//	POST: devuelve el nombre del equipo ganador
const char* partido_obtener_ganador(partido_t* partido);

/* Devuelve el nombre del equipo local o visitante */
//	PRE: el partido fue creado
//	POST: si local == true, devuelve nombre del equipo local,
//	sino, devuelve el nombre del equipo vistante
const char* partido_ver_equipo(partido_t* partido, bool local);

#endif //PARTIDO_H
