#ifndef SISTEMA_H
#define SISTEMA_H

#include <stdbool.h>
#include "curso.h"
#include "sesion.h"

typedef struct _sistema sistema_t;


/* Crea el sistema en el cual se hara ABM de cursos y alumnos */
sistema_t* sistema_crear();

/* Da de baja el sistema de alumnos */
void sistema_destruir();


/* Agrega un curso a la lista de cursos del sistema */
//	PRE: El sistema fue creado
//	POST: Si el curso no existia informa que lo pudo agregar,
//	si el curso ya existia, informa el error por pantalla
void agregar_curso(sistema_t *sistema, char *idc,char *descripcion,char *materia,size_t vacantes);

/* Lista todos los cursos disponibles en el sistema */
// 	La funcion toma por parametro una materia como filtro, 
//	o NULL si se quiere imprimir todas las materias
//	PRE: El sistema fue creado
//	POST: Imprime por pantalla la lista de cursos disponibles
void listar_cursos(sistema_t *sistema, char *materia);

/* Inscribe un alumno al curso */
//	PRE: El sistema fue creado
//	POST: Si el curso "idc" existe, inscribe al alumno con el padron,
//	si el curso no existe, informa el error por pantalla
void inscribir(sistema_t *sistema, size_t padron, char *idc);

/* Elimina un curso de la lista de cursos */
//	PRE: El sistema fue creado
//	POST: Si el curso existia, elmina el curso,
//	sino, informa el error por pantalla
void eliminar_curso(sistema_t *sistema, char *idc);

/* Desinscribe un alumno de un curso */
//	PRE: El sistema fue creado
//	POST: Si el alumno ya estaba inscripto, lo desinscribe,
//	si no estaba inscripto, se informa el error por pantalla
void desinscribir(sistema_t *sistema, size_t padron, char *idc);

/* Lista los inscriptos a un curso */
//	PRE: El sistema fue creado
//	POST: Si el curso existe, lista todos los inscriptos y su condicion,
//	si el curso no existe, informa el error
void listar_inscriptos(sistema_t *sistema, char *idc);


/* Termina la sesion e inscribe al alumno en todas las materias */
//	PRE: El sistema fue creado
//	POST: Invoca a la funcion de sesion para que termine una sesion
void sesion_aplicar_sistema(sistema_t *sistema);


/* Inicia una sesion para un alumno */
//	PRE: El sistema fue creado
//	POST: Si no habia una sesion iniciada, inicia sesion con el padron.
//	Si habia una sesion iniciada, informa del error
void sesion_iniciar_sistema(sistema_t *sistema, size_t padron);

/* Inscribe un alumno a un curso identificado por idc */
//	PRE: El sistema fue creado
//	POST: Invoca a la funcion de sesion para inscribir al alumno a un curso
void sesion_inscribir_sistema(sistema_t *sistema, char *idc);


/* Muestra el estado actual de la sesion */
//	PRE: El sistema fue creado
//	POST: Invoca a la funcion de sesion para que muestre el estado
//	actual de la sesion en curso
void sesion_ver_sistema(sistema_t *sistema);


/* Muestra el estado actual de la sesion */
//	PRE: El sistema fue creado
//	POST: Invoca a la funcion de sesion para que deshaga la
//	ultima inscripcion a un curso
void sesion_deshacer_sistema(sistema_t *sistema);


#endif //SISTEMA_H

