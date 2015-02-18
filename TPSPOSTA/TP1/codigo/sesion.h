#ifndef SESION_H
#define SESION_H

#include <stdio.h>
#include <stdbool.h>
#include "curso.h"
#include "lista.h"


/* Definicion del tipo de la sesion */
typedef struct _sesion sesion_t;

/* Crea una sesion */
//	PRE: no habia ninguna sesion en el sistema
//	POST: se crea la unica instancia de sesion que habra
sesion_t* sesion_crear();

/* Destruye una sesion existente */
//	PRE: la sesion fue creada
//	POST: la sesion fue destruida y no se puede usar mas
void sesion_destruir(sesion_t *sesion);

/* Aplica todos los cambios hechos en la sesion */
//	PRE: La sesion fue creada
//	POST: si habia un alumno en sesion, inscribe al alumno en todos los cursos, 
//	de no haber iniciado sesion se informa por pantalla
void sesion_aplicar(sesion_t *sesion,lista_t *lista_cursos, void* (*curso_buscar)(lista_t*, char*, bool imprimir));


/* Inicia sesion con un padron */
//	PRE: la sesion fue creada
//	POST: devuelve true si se pudo iniciar sesion con el padron actual,
//	false si ya habia una sesion iniciada
bool sesion_iniciar(sesion_t *sesion, size_t padron);

/* Muestra el estado actual de la sesion */
//	PRE: la sesion fue creada
//	POST: si hay un usuario logueado, muestra informacion de sus incripciones,
//	sino muestra un mensaje de error
void sesion_ver(sesion_t *sesion);


/* Deshace la ultima inscripcion a un curso */
//	PRE: la sesion fue creada
//	POST: informa OK si se deshizo la ultima inscripcion,
//	error si no habia ningun curso para desinscribirse o si
//	no habia ninguna sesion iniciada
void sesion_deshacer(sesion_t *sesion);


/* Inscribira a un alumno al curso */
//	PRE: La sesion fue creada
//	POST: Si habia una sesion iniciada, inscribira al alumno
//	e informara en que condicion. De no haber una sesion iniciada
// 	informará el error en pantalla
void sesion_inscribir(sesion_t *sesion,const curso_t *curso,char *idc);

#endif // SESION_H
