#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sesion.h"
#include "pila.h"
#include "curso.h"

#define MAX_IDC 11
#define MAX_CONDICION 12

/* Definicion de la estructura de la sesion */
struct _sesion{
	pila_t *inscripciones;
	bool sesion_iniciada;
	size_t padron;
};

/* Estructura auxiliar para manejar las inscripciones a los cursos */
typedef struct inscripcion {
	char idc[MAX_IDC];
	char condicion[MAX_CONDICION];
} inscripcion_t;

/* Crea una sesion */
sesion_t* sesion_crear()
{
	sesion_t *sesion = malloc(sizeof(sesion_t));
	if (sesion == NULL)
		return NULL;
	pila_t *inscripciones = pila_crear();
	if (inscripciones == NULL)
	{
		free(sesion);
		return NULL;
	}
	sesion->inscripciones = inscripciones;
	sesion->sesion_iniciada = false;
	return sesion;
}

/* Destruye una sesion existente */
void sesion_destruir(sesion_t *sesion)
{
	pila_destruir(sesion->inscripciones, free);	
	free(sesion);
}

/* Aplica todos los cambios hechos en la sesion */
void sesion_aplicar(sesion_t *sesion,lista_t *lista_cursos, void* (*curso_buscar)(lista_t*, char*, bool imprimir))
{
	if (sesion->sesion_iniciada)
	{
		while (!pila_esta_vacia(sesion->inscripciones))
		{
			inscripcion_t *ins = pila_desapilar(sesion->inscripciones);
			curso_t *curso = curso_buscar(lista_cursos, ins->idc, true);
			curso_inscribir(curso,sesion->padron);
			free(ins);
		}
		sesion->sesion_iniciada = false;
		printf("OK\n");
	}
	else
		printf("Error: no hay una sesion en curso\n");
}



/* Inicia sesion con un padron */
bool sesion_iniciar(sesion_t *sesion, size_t padron)
{
	if (!(sesion->sesion_iniciada))
	{
		sesion->sesion_iniciada = true;
		sesion->padron = padron;
		printf("OK\n");
	}
	else
	{
		printf("Error : ya hay una sesion en curso\n");
	}
	return false;
}



/* Funcion que imprime los cursos a los cuales esta inscripto el alumno */
bool mostrar_todos_cursos(void *dato, void *extra)
{
	inscripcion_t *ins = (inscripcion_t*) dato;
	printf("%s (%s)\n", ins->idc, ins->condicion);
	return true;
}

/* Muestra los cursos a los cuales esta inscripto el alumno */
//	PRE: la sesion fue creada y un usuario se logueo al sistema
//	POST: muestra los cursos que el alumno esta inscripto
void mostrar_cursos(sesion_t *sesion)
{
	if (!pila_esta_vacia(sesion->inscripciones))
		pila_iterar(sesion->inscripciones, mostrar_todos_cursos, NULL);
	else	
		printf("No hay inscripciones\n");
}

/* Muestra el estado actual de la sesion */
void sesion_ver(sesion_t *sesion)
{
	if (sesion->sesion_iniciada)
	{
		printf("Padron: %zd\n", sesion->padron);
		mostrar_cursos(sesion);
	}
	else
		printf("Error : no hay una sesion en curso\n");
}


/* Deshace la ultima inscripcion a un curso */
void sesion_deshacer(sesion_t *sesion)
{
	if (sesion->sesion_iniciada)
	{
		if (!pila_esta_vacia(sesion->inscripciones))
		{
			free(pila_desapilar(sesion->inscripciones));
			printf("OK\n");
		}
		else	
			printf("Error : no hay acciones para deshacer\n");
	}
	else
		printf("Error: no hay una sesion en curso\n");
}


bool comprobar(void *dato, void *extra)
{
	inscripcion_t *ins = (inscripcion_t*)dato;
	char *idc = (char*) extra;
	return !(strcmp(ins->idc, idc) == 0);
}


/* Funcion para saber si el alumno ya se inscribio en la sesion */
bool sesion_ya_inscripto(sesion_t *sesion, char *idc)
{
	return !pila_iterar(sesion->inscripciones, comprobar, idc);
}

/* Inscribira a un alumno al curso "idc" */
void sesion_inscribir(sesion_t *sesion,const curso_t *curso,char *idc)
{
	if (sesion->sesion_iniciada)
	{
		if (curso != NULL)
		{
			if (!sesion_ya_inscripto(sesion, idc))
			{
				inscripcion_t *inscripcion = malloc(sizeof(inscripcion_t));
				if (inscripcion == NULL)
					return;
				bool condicion = curso_siguiente_inscripto(curso);
				strcpy(inscripcion->idc, idc);
				strcpy(inscripcion->condicion, condicion ? "regular" : "condicional");
				if (!pila_apilar(sesion->inscripciones, inscripcion))
					free(inscripcion);
				printf("OK: el padron %zd sera inscripto como %s\n", sesion->padron, condicion ? "regular" : "condicional");
			}
			else
				printf("Error: la sesion ya contiene una inscripcion al curso \"%s\"\n", idc);
		}
		else
			printf("Error: el curso con id \"%s\" no existe\n", idc);
	}
	else
		printf("Error : no hay una sesion en curso\n");
}

