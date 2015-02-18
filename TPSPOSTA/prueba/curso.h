#ifndef CURSO_H
#define CURSO_H

#include <stdbool.h>

/* Define la estructura de un curso */
typedef struct _curso curso_t;

/* Crea un curso con los parametros definidos */
//	PRE: que haya memoria en heap
//	POST: devuelve un curso creado con todas sus propiedades
curso_t* curso_crear(char *identificador,char *descripcion,char *materia,size_t vacantes);


/* Destruye un curso creado */
//	PRE: el curso fue creado
//	POST: el curso fue destruido
void curso_destruir(void *curso);

/* Imprime toda la informacion del curso */
//	PRE: El curso fue creado
//	POST: Se imprime por pantalla la informacion del curso
void curso_imprimir(const curso_t *curso);

/* Verifica si el curso es de la materia */
//	PRE: El curso fue creado
//	POST: Devuelve true si el curso tiene el mismo nombre de materia,
//	false de lo contrario
bool curso_verificar_materia(const curso_t *curso,const char *materia);

/* Verifica si el curso y su identificador coinciden */
//	PRE: el curso fue creado
//	POST: devuelve true si el identificador coincide con el identificador del curso
bool curso_comprobar(const curso_t *curso,const char *identificador);


/* Comprueba si un alumno ya esta inscripto en un curso */
//	PRE: El curso fue creado
//	POST: devuelve true si el alumno ya esta inscripto en el curso,
//	false de lo contrario
bool curso_inscripto(const curso_t *curso,const size_t padron);


/* Inscribe un alumno a dicho curso */
//	PRE: El curso fue creado
//	POST: La funcion devuelve true cuando se inscribio al alumno como regular,
//	devuelve false si el alumno se inscribio como condicional
bool curso_inscribir(curso_t *curso,size_t padron);


/* Desinscribe un alumno de un curso */
//	PRE: El curso fue creado
//	POST: Devuelve true si efectivamente habia un alumno y lo desinscribe,
//	devuelve false si el alumno no estaba inscripto en el curso
bool curso_desinscribir(curso_t *curso, size_t padron);


/* Lista todos los inscriptos de un curso */
//	PRE: El curso fue creado
//	POST: Se imprime por pantalla todos los inscriptos al curso y su condicion
void curso_listar_inscriptos(const curso_t *curso);

/* Informa la condicion del siguiente alumno que se inscriba al curso */
//	PRE: El curso fue creado
//	POST: Devuelve true si el alumno sera inscripto como regular,
//	false si sera inscripto como condicional
bool curso_siguiente_inscripto(const curso_t *curso);

#endif // CURSO_H
