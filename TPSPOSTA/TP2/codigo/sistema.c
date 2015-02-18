#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "hash.h"
#include "abb.h"
#include "heap.h"

#include "sistema.h"
#include "equipo.h"
#include "jugador.h"
#include "fixture.h"

#define MAX_JUGADORES 23

/* Estructura interna del sistema */
struct sistema {
	fixture_t *fixture;
	hash_t *hash_equipos;
	hash_t *hash_jugadores;
	heap_t *heap_jugadores;
};

/* Funcion para comparar jugadores que se insertan en un heap */
int comparar_goles(const void *j1,const void *j2){
	jugador_t *jug1 = (jugador_t*) j1;
	jugador_t *jug2 = (jugador_t*) j2;
	return jugador_ver_goles(jug1) - jugador_ver_goles(jug2);
}

/* Crea el sistema en el cual se hara ABM de equipos */
sistema_t* sistema_crear() {
	sistema_t *sistema = malloc(sizeof(sistema_t));
	if (sistema == NULL)
		return NULL;
	hash_t *hash_equipos = hash_crear(equipo_destruir);
	if (hash_equipos == NULL){
		free(sistema);
		return NULL;
	}
	hash_t *hash_jugadores = hash_crear(jugador_destruir);
	if (hash_jugadores == NULL){
		free(sistema);
		hash_destruir(hash_equipos);
		return NULL;
	}
	heap_t *heap_jugadores = heap_crear(comparar_goles);
	if (heap_jugadores == NULL){
		free(sistema);
		hash_destruir(hash_equipos);
		hash_destruir(hash_jugadores);
		return NULL;
	}
	sistema->hash_equipos = hash_equipos;
	sistema->hash_jugadores = hash_jugadores;
	sistema->heap_jugadores = heap_jugadores;
	return sistema;
}

/* Destruye el sistema */
void sistema_destruir(sistema_t *sistema) {
	heap_destruir(sistema->heap_jugadores, NULL);
	hash_destruir(sistema->hash_equipos);
	hash_destruir(sistema->hash_jugadores);
	fixture_destruir(sistema->fixture);
	free(sistema);	
}

/* Carga un equipo con sus respectivos jugadores al sistema */
void sistema_cargar(sistema_t *sistema, const char *equipo, char jugadores[23][255]){
	equipo_t *nuevo_equipo = equipo_crear(equipo);
	if(nuevo_equipo == NULL) return;
	hash_guardar(sistema->hash_equipos, equipo, nuevo_equipo);
	for (int i = 0; i < MAX_JUGADORES; i++){
		jugador_t *jugador = jugador_crear(jugadores[i], equipo, i + 1);
		hash_guardar(sistema->hash_jugadores, jugador_obtener_clave(jugador), jugador);
		heap_encolar(sistema->heap_jugadores, jugador);
		equipo_agregar_jugador(nuevo_equipo, jugador);
	}
}

/* Carga al sistema las llaves con las que comienza el toreno, en el orden que fueron
 * leidas del archivo */
void sistema_cargar_fixture(sistema_t *sistema, hash_t *equipos_fixture){
	fixture_t *fixture = fixture_crear(hash_cantidad(equipos_fixture));
	if (fixture == NULL)
		return;	
	sistema->fixture = fixture;
	hash_iter_t *it = hash_iter_crear(equipos_fixture);
	while (!hash_iter_al_final(it)) {
		const char *equipo = hash_iter_ver_actual(it);
		int *posicion = hash_obtener(equipos_fixture, equipo);
		fixture_agregar_equipo(sistema->fixture, equipo, *posicion);
		hash_iter_avanzar(it);
	}
	hash_iter_destruir(it);
}

/* Agrega un resultado al fixture con id "idr" y actualiza los jugadores y goles de jugadores */
void sistema_agregar_resultado(sistema_t *sistema, char *idr, char *goles_local, char *goles_visitante,
		char *goles_jugadores){
	int g_local = atoi(goles_local);
	int g_vis = atoi(goles_visitante);
	int res = fixture_agregar_resultado(sistema->fixture,idr, g_local, g_vis);
	if (res == OK){
		const char *local = fixture_ver_equipo(sistema->fixture, idr, true);
		const char *visitante = fixture_ver_equipo(sistema->fixture, idr, false);
		equipo_t *eq_local = hash_obtener(sistema->hash_equipos, local);
		equipo_t *eq_visitante = hash_obtener(sistema->hash_equipos, visitante);
		char camiseta[4];
		int indice = 0;
		int subindice = 0;
		int cantidad_goles = 0;
		while (cantidad_goles < (g_local + g_vis)){
			if (goles_jugadores[indice] != ',' && goles_jugadores[indice] != '\0')
				camiseta[subindice++] = goles_jugadores[indice];
			else {
				camiseta[subindice] = '\0';
				equipo_agregar_gol(cantidad_goles < g_local ? eq_local : eq_visitante, camiseta);
				cantidad_goles++;
				subindice = 0;
			}
			indice++;
		}

		heap_heapify(sistema->heap_jugadores);
		printf("OK\n");
	} else if (res == JUGADO){
		printf("Error : el resultado con id %s ya existe\n", idr);
	} else if (res == INVALIDO){
		printf("Error : el resultado con id %s no existe\n", idr);
	}
}



/* Muestra un resultado con "idr" */
void sistema_mostrar_resultado(sistema_t *sistema,const char *idr){
	fixture_consultar_partido(sistema->fixture, idr);
}


bool visitar(const char *clave, void *dato, void *extra){
	jugador_t *jugador = (jugador_t*) dato;
	printf("%s,%d: Goles: %d\n", jugador_ver_nombre(jugador), jugador_ver_numero(jugador), jugador_ver_goles(jugador));
	return true;
}

/* Lista los jugadores ordenados por "tipo" de un equipo */
void sistema_listar_jugadores(sistema_t *sistema, char *tipo, char *equipo){
	equipo_t *eq = hash_obtener(sistema->hash_equipos, equipo);
	if (eq == NULL){
		printf("Error: el equipo %s no esta inscripto en el fixture\n", equipo);
		return;
	}
	equipo_mostrar_jugadores(eq, tipo, visitar);	
}


/* Muestra el goleador del campeonato */
void sistema_ver_goleador(sistema_t *sistema){
	if (heap_esta_vacio(sistema->heap_jugadores)) return;
	jugador_t* jugador = heap_ver_max(sistema->heap_jugadores);
	printf("%s: %s Goles: %d\n", jugador_ver_nombre(jugador), jugador_ver_equipo(jugador), jugador_ver_goles(jugador));
}

/* Muestra los goles de un jugador de nombre = "nombre" */
void sistema_goles_jugador(sistema_t *sistema, char *nombre){
	jugador_t *jugador = hash_obtener(sistema->hash_jugadores, nombre);
	if (jugador == NULL) {
		printf("Error: el jugador %s no esta inscripto en el fixture\n", nombre);
	} else {
		printf("%s,%d: %s Goles: %d\n", nombre, jugador_ver_numero(jugador), jugador_ver_equipo(jugador),
				jugador_ver_goles(jugador));
	}
}


