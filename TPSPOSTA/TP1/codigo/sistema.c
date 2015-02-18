#include <stdio.h>
#include <stdlib.h>
#include "sistema.h"
#include "sesion.h"
#include "lista.h"
#include "curso.h"


struct _sistema {
	lista_t *lista_cursos;
	sesion_t *sesion;
};

/* Informa si un curso esta en la lista o no */
void* curso_buscar(lista_t *lista_cursos, char *idc, bool imprimir)
{
	curso_t *curso = NULL;
	lista_iter_t *iter = lista_iter_crear(lista_cursos);
	bool encontrado = false;
	//Busco si el curso ya existe
	while(!lista_iter_al_final(iter) && !encontrado)
	{
		curso_t *siguiente = lista_iter_ver_actual(iter);
		if (curso_comprobar(siguiente,idc))
		{
			curso = siguiente;
			encontrado = true;
		}
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	if (curso == NULL && imprimir)
		printf("Error: el curso con id \"%s\" no existe\n", idc);
	return curso;
}

/* Crea el sistema en el cual se hara ABM de cursos y alumnos */
sistema_t* sistema_crear()
{
	sistema_t *sistema = malloc(sizeof(sistema_t));
	if (sistema == NULL)
		return NULL;
	//Lista para almacenar cursos en memoria
	lista_t *lista_cursos = lista_crear();
	if (lista_cursos == NULL)
	{
		free(sistema);
		return NULL;
	}
	//Sesion para acciones de los usuarios
	sesion_t *sesion = sesion_crear();
	if (sesion == NULL)
	{
		free(sistema);
		lista_destruir(lista_cursos, NULL);
		return NULL;
	}
	sistema->lista_cursos = lista_cursos;
	sistema->sesion = sesion;
	return sistema;
}

/* Da de baja el sistema de alumnos */
void sistema_destruir(sistema_t *sistema)
{
	lista_destruir(sistema->lista_cursos, curso_destruir);
	sesion_destruir(sistema->sesion);
	free(sistema);
}

/* Agrega un curso al sistema siendo administrador */
void agregar_curso(sistema_t *sistema, char *idc,char *descripcion,char *materia,size_t vacantes)
{
	if (curso_buscar(sistema->lista_cursos,idc, false) == NULL)
	{
		curso_t *nuevo = curso_crear(idc, descripcion, materia, vacantes);
		lista_insertar_ultimo(sistema->lista_cursos, nuevo);
		printf("OK\n");
	}
	else
		printf("Error: el curso con id \"%s\" ya existe\n", idc);
}

/* Funcion auxiliar que uso para la iteracion de la lista de cursos */
bool visitar (void *dato, void *extra)
{
	curso_t *curso = (curso_t*) dato;
	if (extra == NULL)
		curso_imprimir(curso);
	else
	{
		if (curso_verificar_materia(curso, (char*)extra))
			curso_imprimir(curso);
	}
	return true;
}

/* Lista todos los cursos disponibles en el sistema */
void listar_cursos(sistema_t *sistema, char *materia)
{
	lista_iterar(sistema->lista_cursos, visitar, materia);
}


/* Inscribe un alumno al curso */
void inscribir(sistema_t *sistema, size_t padron, char *idc)
{
	curso_t *curso = curso_buscar(sistema->lista_cursos, idc, true);
	if (curso != NULL)
	{
		if (!curso_inscripto(curso, padron))
		{
			bool es_regular = curso_inscribir(curso, padron);
			printf("Ok: padron %zd inscripto como %s\n", padron, es_regular ? "regular" : "condicional");
		}
		else
			printf("Error: el padron %zd ya esta inscripto en el curso \"%s\"\n", padron, idc);
	}
}


void eliminar_curso(sistema_t *sistema, char *idc)
{
	curso_t *curso = curso_buscar(sistema->lista_cursos, idc, true);
	if (curso != NULL)
	{
		//Necesito iterar para buscar el curso y eliminarlo
		lista_iter_t *iter = lista_iter_crear(sistema->lista_cursos);
		bool encontrado = false;
		while (!lista_iter_al_final(iter) && !encontrado)
		{
			curso_t *curso = lista_iter_ver_actual(iter);
			if (curso_comprobar(curso,idc))
			{
				lista_borrar(sistema->lista_cursos, iter);
				curso_destruir(curso);
				encontrado = true;
				printf("OK\n");
			}
			lista_iter_avanzar(iter);
		}
		lista_iter_destruir(iter);
	}
}


/* Desinscribe un alumno de un curso */
void desinscribir(sistema_t *sistema, size_t padron, char *idc)
{
	curso_t *curso = curso_buscar(sistema->lista_cursos, idc, true);
	if (curso != NULL)
	{		
		if (curso_desinscribir(curso, padron))
			printf("OK\n");
		else
			printf("Error: el padron %zd no esta inscripto en el curso \"%s\"\n", padron, idc);
	}
}



/* Lista los inscriptos a un curso */
void listar_inscriptos(sistema_t *sistema, char *idc)
{
	curso_t *curso = curso_buscar(sistema->lista_cursos, idc, true);
	if (curso != NULL)
	{
		curso_listar_inscriptos(curso);
	}
}

/* Termina la sesion e inscribe al alumno en todas las materias */
void sesion_aplicar_sistema(sistema_t *sistema)
{
	sesion_aplicar(sistema->sesion, sistema->lista_cursos, curso_buscar);
}

/* Inicia una sesion para un alumno */
void sesion_iniciar_sistema(sistema_t *sistema, size_t padron)
{
	sesion_iniciar(sistema->sesion, padron);
}

/* Inscribe un alumno a un curso identificado por idc */
void sesion_inscribir_sistema(sistema_t *sistema, char *idc)
{
	curso_t *curso = curso_buscar(sistema->lista_cursos, idc, false);
	sesion_inscribir(sistema->sesion, curso, idc);
}


/* Muestra el estado actual de la sesion */
void sesion_ver_sistema(sistema_t *sistema)
{
	sesion_ver(sistema->sesion);
}

/* Muestra el estado actual de la sesion */
void sesion_deshacer_sistema(sistema_t *sistema)
{
	sesion_deshacer(sistema->sesion);
}

