#ifndef FIXTURE_H

#define FIXTURE_H

#include <stdbool.h>
#include <stddef.h>

#define OK 0
#define JUGADO 1
#define INVALIDO 2

typedef struct fixture fixture_t;

/* Crea un fixture con la cantidad de equipos, de la cual se calcula la cantidad de partidos a jugar */
//	PRE: cantidad_equipos es una potencia de 2 (ej: 2,4,8,16,32..)
//	POST: Fixture listo para agregar los partidos de las llaves inferiores
fixture_t* fixture_crear(size_t cantidad_equipos);

/* Destruye el fixture creado */
//	PRE: El fixture fue creado
//	POST: Fixture destruido
void fixture_destruir(fixture_t *fixture);

/* Agrega un equipo al fixture, siendo "posicion" el subindice en el cual fue leido el equipo */
//	PRE: El fixture fue creado
//	POST: Equipo con nombre "equipo" agregado a la "posicion" que corresponde del fixture
void fixture_agregar_equipo(fixture_t *fixture, const char *equipo, int posicion);

/* Agrega un resultado al fixture */
//	PRE: El fixture fue creado
//	POST: La llave "idr" cargada con goles de local y visitante
//	Si no existe el "idr", devuelve INVALIDO, de ya haberse jugado el partido, devuelve JUGADO
// 	Si el resultado se agrego exitosamente, devuelve OK
int fixture_agregar_resultado(fixture_t *fixture,char *idr,int goles_local, int goles_visitante);

/* Consulta el resultado de un partido jugado */
//	PRE: El fixture fue creado
//	POST: Imprime por pantalla, si existe el idr, el resultado, de no existir, lo informa
void fixture_consultar_partido(fixture_t *fixture, const char *idr);

/* Devuelve el nombre del equipo que esta en ese partido como local o visitante */
//	PRE: El fixture fue creado e "idr" es una llave valida del fixture
//	POST: Devuelve el nombre del equipo en base si el que se consulta es local o visitante
const char* fixture_ver_equipo(fixture_t *fixture, const char *idr, bool local);


#endif //FIXTURE_H
