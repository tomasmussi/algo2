#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "partido.h"

/* Estructura interna del partido */
struct partido {
	char *local;
	int goles_local;
	char *visitante;
	int goles_visitante;
	bool jugado;
};

/* Crea un partido */
partido_t* partido_crear(){
	partido_t *partido = malloc(sizeof(partido_t));
	if (partido == NULL) return NULL;
	partido->local = NULL;
	partido->visitante = NULL;
	partido->goles_local = 0;
	partido->goles_local = 0;
	partido->jugado = false;
	return partido;
}

/* Destruye un partido */
void partido_destruir(partido_t* partido){
	free(partido->local);
	free(partido->visitante);
	free(partido);
}

/* Agrega el nombre de un equipo al partido como local o visitante */
bool partido_agregar_equipo(partido_t* partido,const char *equipo, bool local){
	if (partido->local && partido->visitante)
		return false;
	char *eq = malloc((strlen(equipo) +1) * sizeof(char));
	strcpy(eq, equipo);
	if (local){
		partido->local = eq;
	} else {
		partido->visitante = eq;
	}
	return true;
}

/* Imprime por pantalla el resultado del partido */
void partido_resultado(partido_t* partido,const char* idr){
	if (!partido || (partido && !partido->jugado)) {
		printf("Error: el resultado con id %s no existe\n", idr);
	} else {
		printf("resultado: %s:%d vs %s:%d\n", partido->local, partido->goles_local,
				partido->visitante, partido->goles_visitante);
	}
}

/* Agrega un resultado a un partido */
bool partido_agregar_resultado(partido_t* partido,const char *g1, int goles_local,const char *g2, int goles_visitante){
	if (partido->jugado)
		return false;
	if (g1 != NULL && g2 != NULL){
		char *local = malloc((strlen(g1) + 1) * sizeof(char));
		char *visitante = malloc((strlen(g2) + 1) * sizeof(char));
		strcpy(local,g1);
		strcpy(visitante,g2);
		partido->local = local;
		partido->visitante = visitante;
	}
	partido->goles_local = goles_local;
	partido->goles_visitante = goles_visitante;
	partido->jugado = true;
	return true;
}

/* Consulta si un partido se jugo o no */
bool partido_jugado(partido_t *partido){
	return partido->jugado;
}

/* Devuelve el nombre del equipo que gano el partido */
const char* partido_obtener_ganador(partido_t* partido){
	return partido->goles_local > partido->goles_visitante ? partido->local : partido->visitante;
}

/* Devuelve el nombre del equipo local o visitante */
const char* partido_ver_equipo(partido_t* partido, bool local){
	return local ? partido->local : partido->visitante;
}
