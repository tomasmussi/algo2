
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pila.h"

#define MAX 10
#define FACTOR 2
#define TAMANIO_REDIMENSION 2*MAX


/********* DEFINICION DE FUNCIONES ***********/

bool pila_redimensionar(pila_t *pila, size_t nuevoTamanio);

/********* FIN DEFINICION DE FUNCIONES ***********/


struct _pila {
    void** datos;
    size_t tamanio;
    size_t cantidad;
};

// Crea una pila.
// Post: devuelve una nueva pila vacía.
pila_t* pila_crear()
{
	pila_t *pila = malloc(sizeof(pila_t));
	void **datos = malloc(MAX * sizeof(void*));
	if (pila == NULL || datos == NULL)
	{
		return NULL;
	}
	pila->datos = datos;
	pila->tamanio = MAX;
	pila->cantidad = 0;
	return pila;
}

// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.
void pila_destruir(pila_t *pila)
{
	free (pila->datos);
	free (pila);
}

// Devuelve verdadero o falso, según si la pila tiene o no elementos apilados.
// Pre: la pila fue creada.
bool pila_esta_vacia(const pila_t *pila)
{
	return (pila->cantidad == 0);
}

// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void* valor)
{	
	if (!(pila->cantidad < (pila->tamanio - 1)))
	{
		if (!pila_redimensionar(pila, pila->tamanio * FACTOR))
		{
			return false;
		}
	}	
	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}

// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope. Si está vacía devuelve NULL.
// Pre: la pila fue creada.
// Post: se devolvió el valor del tope de la pila, cuando la pila no está
// vacía, NULL en caso contrario.
void* pila_ver_tope(const pila_t *pila)
{
	if (!pila_esta_vacia(pila))
	{
		return pila->datos[pila->cantidad - 1];
	}
	return NULL;	
}

// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve 
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacía, se devuelve el valor del tope anterior 
// y la pila contiene un elemento menos.
void* pila_desapilar(pila_t *pila)
{
	if (!(pila_esta_vacia(pila)))
	{	
		void *datos = pila_ver_tope(pila);
		pila->cantidad--;
		if ((((pila->tamanio-1) / (2*FACTOR) == pila->cantidad)) && pila->cantidad > 2)
		{
			pila_redimensionar(pila, pila->tamanio / FACTOR);		
		}
		return datos;
	}
	return NULL;	
}


//Redimensiona la pila acorde a la cantidad de datos que tengo
//Pre: la pila fue creada
//Post: devuelve true si pudo redimensionar la pila, false en caso contrario
bool pila_redimensionar(pila_t *pila, size_t nuevoTamanio)
{
	void **nuevosDatos = realloc(pila->datos, nuevoTamanio * sizeof(void*));
	// Cuando tam_nuevo es 0, es correcto que devuelva NULL
	// En toda otra situación significa que falló el realloc
	if (nuevoTamanio > 0 && nuevosDatos == NULL)
	{
		return false;
	}
	pila->datos = nuevosDatos;
	pila->tamanio = nuevoTamanio;
	return true;
}


