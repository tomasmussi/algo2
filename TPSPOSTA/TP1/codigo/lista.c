
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lista.h"

/********* Estructuras de la lista *****************/

typedef struct nodo_lista {
	void *dato;
	struct nodo_lista *siguiente;
} nodo_lista_t;



struct _lista_iter_t {
	nodo_lista_t *actual;
	nodo_lista_t *anterior;
};

struct lista_t {
	nodo_lista_t *primero;
	nodo_lista_t *ultimo;
	size_t cantidad;
};

/******** Operaciones auxiliares de los nodos *************/

nodo_lista_t* nodo_crear(void *dato);

void nodo_destruir(nodo_lista_t *nodo);

/******** Fin Operaciones auxiliares de los nodos *************/


/************ Implementacion de funciones definidas **************/

nodo_lista_t* nodo_crear(void *dato)
{
	nodo_lista_t *nodo = malloc(sizeof(nodo_lista_t));
	if (nodo == NULL)
		return NULL;
	nodo->dato = dato;
	nodo->siguiente = NULL;
	return nodo;
}

void nodo_destruir(nodo_lista_t *nodo)
{
	free(nodo);
}


lista_t* lista_crear()
{
	lista_t *lista = malloc(sizeof(lista_t));
	if (lista == NULL)
		return NULL;
	lista->primero = NULL;
	lista->ultimo = NULL;
	lista->cantidad = 0;
	return lista;
}

bool lista_esta_vacia(const lista_t *lista)
{
	return lista->cantidad == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato)
{
	nodo_lista_t *nuevo_nodo = nodo_crear(dato);
	if (nuevo_nodo == NULL)
		return false;
	if (!lista_esta_vacia(lista))
	{
		nodo_lista_t *primero = lista->primero;
		nuevo_nodo->siguiente = primero;
		lista->primero = nuevo_nodo;
	}
	else
	{
		nuevo_nodo->siguiente = NULL;
		lista->primero = nuevo_nodo;
		lista->ultimo = nuevo_nodo;
	}
	lista->cantidad++;
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato)
{
	nodo_lista_t *nuevo_nodo = nodo_crear(dato);
	if (nuevo_nodo == NULL)
		return false;
	if (!lista_esta_vacia(lista))
	{
		nodo_lista_t *ultimo = lista->ultimo;
		ultimo->siguiente = nuevo_nodo;
		lista->ultimo = nuevo_nodo;
	}
	else
	{
		lista->primero = nuevo_nodo;
		lista->ultimo = nuevo_nodo;
	}
	lista->cantidad++;
	return true;
}

void* lista_borrar_primero(lista_t *lista)
{
	if (lista_esta_vacia(lista))
		return NULL;
	nodo_lista_t *primero = lista->primero;
	nodo_lista_t *segundo = primero->siguiente;
	//Podria poner el lista->primero = segundo porque es lo mismo en ambos casos
	//pero es mas claro para mi gusto de esta forma
	if (segundo == NULL)
	{
		lista->primero = NULL;
		lista->ultimo = NULL;
	}
	else
	{
		lista->primero = segundo;
	}
	void *dato = primero->dato;
	nodo_destruir(primero);
	lista->cantidad--;
	return dato;
}

void* lista_ver_primero(const lista_t *lista)
{
	if (lista_esta_vacia(lista))
		return NULL;
	return lista->primero->dato;
}

size_t lista_largo(const lista_t *lista)
{
	return lista->cantidad;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *))
{
	if (!lista_esta_vacia(lista))
	{
		nodo_lista_t *nodo = lista->primero;
		while(nodo != NULL)
		{
			if (destruir_dato != NULL)
				destruir_dato(nodo->dato);
			nodo_lista_t *siguiente = nodo->siguiente;
			nodo_destruir(nodo);
			nodo = siguiente;
		}
	}
	free(lista);
}

/*********** Fin Lista ***********/


/********** Iterador externo ***********/

lista_iter_t *lista_iter_crear(const lista_t *lista)
{
	lista_iter_t *iterador = malloc(sizeof(lista_iter_t));
	if (iterador == NULL)
		return NULL;
	iterador->actual = lista->primero;
	iterador->anterior = NULL;
	return iterador;
}

bool lista_iter_avanzar(lista_iter_t *iter)
{
	if (!lista_iter_al_final(iter))
	{
		iter->anterior = iter->actual;
		iter->actual = iter->actual->siguiente;
		return true;
	} else {
		return false;
	}
}

void* lista_iter_ver_actual(const lista_iter_t *iter)
{
	return lista_iter_al_final(iter) ? NULL : iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter)
{
	return (iter->actual == NULL);
}

void lista_iter_destruir(lista_iter_t *iter)
{
	free(iter);
}

/********** Fin Iterador externo **********/


/********** Iterador interno **********/

void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra)
{
	if (!lista_esta_vacia(lista))
	{
		nodo_lista_t *nodo = lista->primero;
		bool seguir_visita = true;
		while (nodo != NULL && seguir_visita)
		{
			seguir_visita = visitar(nodo->dato, extra);
			nodo = nodo->siguiente;
		}
	}
}

/********** Fin Iterador interno **********/


/********** Operaciones con iterador externo **********/
bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato)
{
	//Si esta vacia la lista, uso una primitiva existente
	if (lista_esta_vacia(lista))
	{
		bool ok = lista_insertar_primero(lista, dato);
		if (ok)
		{
			iter->actual = lista->primero;
			iter->anterior = NULL;
		}
		return ok;
	}
	else
	{
		//Si esta al principio, utilizo insertar primero
		if (iter->actual == lista->primero)
		{
			bool ok = lista_insertar_primero(lista, dato);
			if (ok)
			{
				iter->actual = lista->primero;
				iter->anterior = NULL;
			}
			return ok;
		}
		//Si esta al final, uso intertar ultimo
		else if (lista_iter_al_final(iter)) {
			bool ok = lista_insertar_ultimo(lista, dato);
			if (ok)
			{
				iter->actual = lista->ultimo;
			}
			return ok;
		} else {
			//Si entre aca es porque el nodo esta en un lugar x
			nodo_lista_t *nodo = nodo_crear(dato);
			if (nodo == NULL)
				return false;
			nodo->siguiente = iter->actual;
			//Si el anterior es null, era el primero de la lista
			iter->anterior->siguiente = nodo;
			iter->actual = nodo;
			lista->cantidad++;
			return true;
		}
	}
}

void* lista_borrar(lista_t *lista, lista_iter_t *iter)
{
	if (!lista_iter_al_final(iter))
	{
		//Hay al menos un nodo		
		void *dato = NULL;
		//Considero si es el primero
		if (iter->actual == lista->primero) {
			//Primero, uso primitivas conocidas
			dato = lista_borrar_primero(lista);
			iter->actual = lista->primero;
		} else {
			//Es del segundo en adelante, el anterior no puede ser NULL
			nodo_lista_t *actual = iter->actual;
			iter->anterior->siguiente = actual->siguiente;
			if (lista->ultimo == actual)
				lista->ultimo = iter->anterior;
			dato = actual->dato;
			iter->actual = actual->siguiente;
			nodo_destruir(actual);
			lista->cantidad--;
		}
		return dato;
	}
	return NULL;
}
