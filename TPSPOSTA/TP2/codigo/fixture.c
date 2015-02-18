#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "partido.h"
#include "fixture.h"

/* Estructura del fixture */
struct fixture {
	partido_t **vector;
	size_t cantidad_partidos;
};

/* Parsea el numero de llave del "idr", es decir "8a" lo convierte a "8" */
//	Devuelve un puntero a char para el cual pidio memoria, hay que liberar esa memoria pedida
char* parsear_id(const char *idr);

/* Parsea de acuerdo a una llave, que partido es, es decir "8a",
 * parsea el 'a' y devuelve 'a' - 'a' (0)*/
int parsear_subindice(const char *idr);

/* A partir de un "idr" y utilizando las 2 funciones anteriores,
 * devuelve una posicion del vector correspondiente a ese idr*/
int obtener_posicion(const char *idr);

/* A partir de una posicion dada y un tamanio,
 * devuelve el subindice del partido de la llave a la izquierda, si se quiere el derecho,
 * hay que sumar 1 a la posicion obtenida */
int obtener_posicion_hijo(int posicion, int tamanio);

int obtener_posicion_hijo(int posicion, int tamanio){
	int pos = posicion * 2 + 1;
	return pos < tamanio ? pos : -1;
}

char* parsear_id(const char *idr){
	char *id = malloc((strlen(idr) + 1) * sizeof(char));
	int i = 0;
	while ('0' <= idr[i] && idr[i] <= '9'){
		id[i] = idr[i];
		i++;
	}
	id[i] = '\0';
	return id;
}

int parsear_subindice(const char *idr){
	int i = 0;
	while ('0' <= idr[i] && idr[i] <= '9'){
		i++;
	}
	return idr[i] - 'a';
}

int obtener_posicion(const char *idr){
	char *id_nuevo = parsear_id(idr);
	int id = atoi(id_nuevo);
	free(id_nuevo);
	return id - 1 + parsear_subindice(idr);
}

/* Crea un fixture con la cantidad de equipos, de la cual se calcula la cantidad de partidos a jugar */
fixture_t* fixture_crear(size_t cantidad_equipos){
	size_t cantidad_partidos = cantidad_equipos - 1;
	fixture_t *fixture = malloc(sizeof(fixture_t));
	if (fixture == NULL)
		return NULL;
	partido_t **vector = calloc (cantidad_partidos, sizeof(partido_t*));
	if (vector == NULL){
		free(fixture);
		return NULL;
	}
	for (int i = 0; i < cantidad_partidos; i++)
		vector[i] = partido_crear();
	fixture->vector = vector;
	fixture->cantidad_partidos = cantidad_partidos;
	return fixture;
}

/* Destruye el fixture creado */
void fixture_destruir(fixture_t *fixture){
	for (int i = 0; i < fixture->cantidad_partidos; i++)
		partido_destruir(fixture->vector[i]);
	free(fixture->vector);
	free(fixture);
}

/* Agrega un equipo al fixture, siendo "posicion" el subindice en el cual fue leido el equipo */
void fixture_agregar_equipo(fixture_t *fixture, const char *equipo, int posicion){
	size_t pos = fixture->cantidad_partidos / 2;
	int subindice = posicion / 2;
	partido_agregar_equipo(fixture->vector[pos + subindice], equipo, (posicion % 2) == 0);
}

/* Agrega un resultado al fixture */
int fixture_agregar_resultado(fixture_t *fixture,char *idr,int goles_local, int goles_visitante){
	int posicion = obtener_posicion(idr);
	int posicion_hijo = obtener_posicion_hijo(posicion, fixture->cantidad_partidos);
	if (posicion >= fixture->cantidad_partidos)
		return INVALIDO;
	if (posicion_hijo == -1){
		//Es uno de los resultados de la primer llave
		bool agregado = partido_agregar_resultado(fixture->vector[posicion],NULL, goles_local,NULL, goles_visitante);
		return agregado ? OK : JUGADO;
	} else {
		partido_t *p1 = fixture->vector[posicion_hijo];
		partido_t *p2 = fixture->vector[posicion_hijo + 1];
		if (!partido_jugado(p1) || !partido_jugado(p2))
			return INVALIDO;
		const char *g1 = partido_obtener_ganador(p1);
		const char *g2 = partido_obtener_ganador(p2);
		bool agregado = partido_agregar_resultado(fixture->vector[posicion],g1, goles_local,g2, goles_visitante);
		return agregado ? OK : JUGADO;
	}
}


/* Consulta el resultado de un partido jugado */
void fixture_consultar_partido(fixture_t *fixture, const char *idr) {
	int pos = obtener_posicion(idr);
	if (pos >= fixture->cantidad_partidos){
		printf("Error : el resultado con id %s no existe\n", idr);
		return;
	}
	partido_resultado(fixture->vector[pos], idr);
}

/* Devuelve el nombre del equipo que esta en ese partido como local o visitante */
const char* fixture_ver_equipo(fixture_t *fixture, const char *idr, bool local){
	int pos = obtener_posicion(idr);
	return partido_ver_equipo(fixture->vector[pos], local);
}

