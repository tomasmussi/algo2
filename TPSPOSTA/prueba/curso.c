#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "curso.h"
#include "lista.h"

/* Constantes para definir la cantidad de caraceteres de la estructura */
#define MAX_IDC 11
#define MAX_DESC 81
#define MAX_MATERIA 11

/* Estructura interna de un curso */
struct _curso {
	char idc[MAX_IDC];
	char descripcion[MAX_DESC];
	char materia[MAX_MATERIA];
	size_t vacantes;
	lista_t *lista_inscriptos;
};



/* Crea un curso con los parametros definidos */
curso_t* curso_crear(char *identificador,char *descripcion,char *materia,size_t vacantes)
{
	curso_t *curso = malloc(sizeof(curso_t));
	if (curso == NULL)
		return NULL;
	lista_t *lista = lista_crear();
	if (lista == NULL)
	{
		free(curso);
		return NULL;
	}
	strcpy(curso->idc, identificador);
	strcpy(curso->descripcion, descripcion);
	strcpy(curso->materia, materia);
	curso->vacantes = vacantes;
	curso->lista_inscriptos = lista;
	return curso;
}

/* Destruye un curso */
void curso_destruir(void *curso)
{
	curso_t *curso_real = (curso_t*) curso;
	lista_destruir(curso_real->lista_inscriptos, free);
	free(curso_real);
}

/* Imprime toda la informacion del curso */
void curso_imprimir(const curso_t *curso)
{
	printf("%s: %s (%s) Vacantes: %zd Inscriptos: %zd\n",curso->idc, curso->descripcion, curso->materia, curso->vacantes, lista_largo(curso->lista_inscriptos) );
}

/* Verifica si el curso es de la materia */
bool curso_verificar_materia(const curso_t *curso,const char *materia)
{
	return strcmp(curso->materia, materia) == 0;
}

/* Verifica si un curso con el codigo ya existe */
bool curso_comprobar(const curso_t *curso,const char *identificador)
{
	return strcmp(curso->idc, identificador) == 0;
}

/* Comprueba si un alumno ya esta inscripto en un curso */
bool curso_inscripto(const curso_t *curso,const size_t padron)
{
	lista_iter_t *it = lista_iter_crear(curso->lista_inscriptos);
	bool encontrado = false;
	while(!lista_iter_al_final(it) && !encontrado)
	{
		size_t *inscripto = lista_iter_ver_actual(it);
		if (padron == *inscripto)
			encontrado = true;
		lista_iter_avanzar(it);
	}
	lista_iter_destruir(it);
	return encontrado;
}

/* Inscribe un alumno a dicho curso */
bool curso_inscribir(curso_t *curso,size_t padron)
{
	size_t *nuevo_padron = malloc(sizeof(size_t));
	*nuevo_padron = padron;
	lista_insertar_ultimo(curso->lista_inscriptos, nuevo_padron);
	return lista_largo(curso->lista_inscriptos) <= curso->vacantes;
}

/* Desinscribe un alumno de un curso */
bool curso_desinscribir(curso_t *curso, size_t padron)
{
	if (curso_inscripto(curso,padron))
	{
		lista_iter_t *iter = lista_iter_crear(curso->lista_inscriptos);
		bool encontrado = false;
		while (!lista_iter_al_final(iter) && !encontrado)
		{
			size_t *actual = lista_iter_ver_actual(iter);
			if (*actual == padron)
			{
				size_t *borrar = lista_borrar(curso->lista_inscriptos, iter);
				free(borrar);
				encontrado = true;
			}
			lista_iter_avanzar(iter);
		}
		lista_iter_destruir(iter);
		return encontrado;
	}
	else
		return false;
}

/* Lista todos los inscriptos de un curso */
void curso_listar_inscriptos(const curso_t *curso)
{
	lista_iter_t *it = lista_iter_crear(curso->lista_inscriptos);
	size_t contador = 0;
	while(!lista_iter_al_final(it))
	{
		size_t *padron = lista_iter_ver_actual(it);
		printf("%zd %s\n", *padron, contador < curso->vacantes ? "regular" : "condicional");
		contador++;
		lista_iter_avanzar(it);
	}
	lista_iter_destruir(it);
}

/* Informa la condicion del siguiente alumno que se inscriba al curso */
bool curso_siguiente_inscripto(const curso_t *curso)
{
	return lista_largo(curso->lista_inscriptos) < curso->vacantes;
}



