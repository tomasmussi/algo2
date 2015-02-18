#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "hash.h"
#include "lista.h"

#define TAMANIO 997
#define MAX_REDIMENSION 4
#define FACTOR 10

/* Estructura del hash ABIERTO */
struct hash {
	lista_t **tabla;
	size_t cantidad;
	size_t tamanio;
	hash_destruir_dato_t destruir;
};

/* Nodos de la tabla de hash */
typedef struct nodo_hash {
	char *clave;
	void *dato;
} nodo_hash_t;

/* Estructura del iterador */
struct hash_iter {
	size_t pos;
	const hash_t *hash;
	lista_iter_t *lista_iter;
};

/* Crea un nodo para guardar el par clave/valor */
//	PRE: La clave es distinta a NULL
//	POST: Devuelve un nodo para insertar en una lista
nodo_hash_t* nodo_hash_crear(const char *clave, void *dato);

/* Destruye un nodo que contiene un par clave/valor */
//	PRE: El nodo fue creado
//	POST: Destruye el nodo y si la funcion destruir es != NULL,
//	tambien destruye el dato almacenado
void nodo_hash_destruir(nodo_hash_t *nodo, hash_destruir_dato_t destruir);

/* Funcion de hash */
//	PRE: La clave nunca puede ser null
//	POST: Devuelve una posicion de la tabla
size_t funcion_hash(const char *clave, size_t tamanio);

/* Busca un nodo en la lista dada */
//	PRE: La lista fue creada
//	POST: Devuelve el nodo con esa clave o, en caso de no existir,
//	devuelve NULL
nodo_hash_t* nodo_buscar(lista_t *lista,const char *clave);

/* Redimensiona el tamaño de la tabla actual de hash */
//	PRE: El hash y la tabla fueron creados.
//	POST: Se copian los datos a una nueva tabla y se destruye la vieja.
void redimensionar(hash_t *hash, size_t tamanio);

/* Destruye la tabla */
//	PRE: La tabla fue creada
//	POST: La tabla fue destruida y no se puede utilizar. Ademas si destruir != NULL
//	tambien destruye los datos
void hash_destruir_tabla(lista_t **tabla, size_t tamanio, hash_destruir_dato_t destruir);

/* Crea un nodo para guardar el par clave/valor */
nodo_hash_t* nodo_hash_crear(const char *clave, void *dato)
{
	char *clave_nodo = malloc((strlen(clave) + 1) * sizeof(char));
	if (clave_nodo == NULL)
		return NULL;
	strcpy(clave_nodo, clave);
	nodo_hash_t *nodo = malloc(sizeof(nodo_hash_t));
	if (nodo == NULL)
	{
		free(clave_nodo);
		return NULL;
	}
	nodo->clave = clave_nodo;
	nodo->dato = dato;
	return nodo;
}

/* Destruye un nodo que contiene un par clave/valor */
void nodo_hash_destruir(nodo_hash_t *nodo, hash_destruir_dato_t destruir)
{
	if (destruir != NULL)
		destruir(nodo->dato);
	free(nodo->clave);
	free(nodo);
}

/* Redimensiona el tamaño de la tabla actual de hash */
void redimensionar(hash_t *hash, size_t tamanio)
{
	lista_t **tabla = calloc(tamanio, sizeof(lista_t*));
	if (tabla == NULL)
	{		
		return;
	}

	hash_iter_t *iter = hash_iter_crear(hash);
	while(!hash_iter_al_final(iter))
	{
		const char *clave = hash_iter_ver_actual(iter);
		void *dato = hash_obtener(hash, clave);
		size_t posicion = funcion_hash(clave, tamanio);
		lista_t *lista = tabla[posicion];	
		if (lista == NULL)
		{
			nodo_hash_t *nuevo_nodo = nodo_hash_crear(clave, dato);
			if (nuevo_nodo == NULL)
				return;
			lista = lista_crear();
			if (lista == NULL)
			{
				nodo_hash_destruir(nuevo_nodo, hash->destruir);
				return;
			}
			tabla[posicion] = lista;
			lista_insertar_ultimo(lista, nuevo_nodo);			
		}
		else
		{
			nodo_hash_t *nuevo_nodo = nodo_hash_crear(clave, dato);
			if (nuevo_nodo == NULL)
				return;
			lista_insertar_ultimo(lista, nuevo_nodo);			
		}
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	hash_destruir_tabla(hash->tabla, hash->tamanio, NULL);
	hash->tabla = tabla;
	hash->tamanio = tamanio;
}

/* Funcion de hash */
size_t funcion_hash(const char *clave, size_t tamanio)
{
	unsigned int hash = 0;
	for (unsigned int i = 0; i < strlen(clave); i++)
		hash = 31 * hash + clave[i];
	return hash % tamanio;
}

/* Busca un nodo en la lista dada */
nodo_hash_t* nodo_buscar(lista_t *lista,const char *clave)
{
	lista_iter_t *it = lista_iter_crear(lista);
	bool encontrado = false;
	while (!lista_iter_al_final(it) && !encontrado)
	{
		nodo_hash_t *nodo = lista_iter_ver_actual(it);
		if (strcmp(clave, nodo->clave) == 0)
			encontrado = true;
		else
			lista_iter_avanzar(it);
	}
	//Si se corto la iteracion porque encontrado == true, tengo el nodo que busco
	//sino, lista_ver_actual devuelve null y es el valor que devuelvo en caso de
	//no hallar la clave
	nodo_hash_t *nodo = lista_iter_ver_actual(it);
	lista_iter_destruir(it);
	return nodo;
}

/* Crea una tabla de hash */
hash_t *hash_crear(hash_destruir_dato_t destruir_dato)
{
	hash_t *hash = malloc(sizeof(hash_t));
	if (hash == NULL)
		return NULL;
	lista_t **tabla = calloc(TAMANIO, sizeof(lista_t*));
	if (tabla == NULL)
	{
		free(hash);
		return NULL;
	}
	hash->tabla = tabla;
	hash->cantidad = 0;
	hash->tamanio = TAMANIO;
	hash->destruir = destruir_dato;
	return hash;
}
	
/* Guarda en el hash un par de clave/valor */
bool hash_guardar(hash_t *hash, const char *clave, void *dato)
{	
	size_t posicion = funcion_hash(clave, hash->tamanio);

	lista_t *lista = hash->tabla[posicion];	
	if (lista == NULL)
	{
		nodo_hash_t *nuevo_nodo = nodo_hash_crear(clave, dato);
		if (nuevo_nodo == NULL)
			return false;
		lista = lista_crear();
		if (lista == NULL)
		{
			nodo_hash_destruir(nuevo_nodo, hash->destruir);
			return false;
		}
		hash->tabla[posicion] = lista;
		lista_insertar_ultimo(lista, nuevo_nodo);
		hash->cantidad++;
	}
	else
	{
		nodo_hash_t *nodo = nodo_buscar(lista, clave);
		if (nodo == NULL)
		{
			nodo_hash_t *nuevo_nodo = nodo_hash_crear(clave, dato);
			if (nuevo_nodo == NULL)
				return false;
			lista_insertar_ultimo(lista, nuevo_nodo);
			hash->cantidad++;
		}
		else
		{
			if (hash->destruir != NULL)
				hash->destruir(nodo->dato);			
			nodo->dato = dato;
		}
	}	
	if (lista_largo(lista) > MAX_REDIMENSION)
	{
		redimensionar(hash, hash->tamanio * FACTOR);		
	}
	return true;
}

/* Borra del hash la clave y devuelve el valor asociado */
void *hash_borrar(hash_t *hash, const char *clave)
{
	size_t posicion = funcion_hash(clave, hash->tamanio);
	if (hash->tabla[posicion] != NULL)
	{
		lista_t *lista = hash->tabla[posicion];
		lista_iter_t *it = lista_iter_crear(lista);
		bool encontrado = false;
		void *dato = NULL;
		while (!lista_iter_al_final(it) && !encontrado)
		{
			nodo_hash_t *nodo = lista_iter_ver_actual(it);
			if (strcmp(clave, nodo->clave) == 0)
			{
				dato = nodo->dato;
				lista_borrar(lista, it);
				nodo_hash_destruir(nodo, NULL);
				hash->cantidad--;
				encontrado = true;
			}
			lista_iter_avanzar(it);
		}
		lista_iter_destruir(it);
		return dato;
	}
	return NULL;
}



/* Devuelve el valor asociado a la clave del hash */
void *hash_obtener(const hash_t *hash, const char *clave)
{
	size_t posicion = funcion_hash(clave, hash->tamanio);
	lista_t *lista = hash->tabla[posicion];
	if (lista != NULL)
	{
		nodo_hash_t *nodo = nodo_buscar(lista, clave);
		if (nodo != NULL)
			return nodo->dato;
	}
	return NULL;
}


/* Informa si la clave ya esta en el hash o no */
bool hash_pertenece(const hash_t *hash, const char *clave)
{
	size_t posicion = funcion_hash(clave, hash->tamanio);
	lista_t *lista = hash->tabla[posicion];
	if (lista != NULL)
	{
		nodo_hash_t *nodo = nodo_buscar(lista, clave);
		if (nodo != NULL)
			return true;
	}
	return false;
}


/* Devuelve la cantidad de pares clave/valor contenidos en el hash */
size_t hash_cantidad(const hash_t *hash)
{
	return hash->cantidad;
}

/* Destruye la tabla */
void hash_destruir_tabla(lista_t **tabla, size_t tamanio, hash_destruir_dato_t destruir)
{
	for (size_t i = 0; i < tamanio; i++)
	{
		lista_t *lista = tabla[i];
		if (lista != NULL)
		{
			lista_iter_t *it = lista_iter_crear(lista);
			while (!lista_iter_al_final(it))
			{
				nodo_hash_t *nodo = lista_iter_ver_actual(it);
				nodo_hash_destruir(nodo, destruir);
				lista_iter_avanzar(it);
			}
			lista_iter_destruir(it);
			lista_destruir(lista, NULL);
		}
	}
	free(tabla);
}


/* Destruye el hash */
void hash_destruir(hash_t *hash)
{
	hash_destruir_tabla(hash->tabla, hash->tamanio, hash->destruir);
	free(hash);
}

/****************** ITERADOR *****************/


/* Crea un iterador para recorrer todas las claves del hash */
hash_iter_t *hash_iter_crear(const hash_t *hash)
{
	hash_iter_t *it = malloc(sizeof(hash_iter_t));
	if (it == NULL)
		return NULL;
	it->hash = hash;
	it->pos = 0;
	it->lista_iter = NULL;
	lista_t *lista = it->hash->tabla[it->pos];
	while (lista == NULL && (it->pos < (it->hash->tamanio - 1)))
	{
		it->pos++;
		lista = it->hash->tabla[it->pos];
	}
	if (lista != NULL)
		it->lista_iter = lista_iter_crear(lista);
	return it;
}


/* Avanza a la siguiente clave contenida en el hash */
bool hash_iter_avanzar(hash_iter_t *iter)
{
	if (iter->lista_iter != NULL)
	{		
		lista_iter_avanzar(iter->lista_iter);
		//Si estoy al final de la lista, tengo que pasar al siguiente nodo posible
		if (lista_iter_al_final(iter->lista_iter))
		{
			lista_iter_destruir(iter->lista_iter);
			iter->lista_iter = NULL;
			if (!hash_iter_al_final(iter))
			{
				iter->pos++;
				lista_t *lista = iter->hash->tabla[iter->pos];
				while (lista == NULL && (iter->pos < (iter->hash->tamanio - 1)))
				{
					iter->pos++;
					lista = iter->hash->tabla[iter->pos];
				}
				if (lista != NULL)
					iter->lista_iter = lista_iter_crear(lista);
			}
		}
		//El iterador de hash pudo avanzar, devuelvo true
		return true;		
	}
	//Si el iterador es NULL, entonces no hay nada en la tabla
	return false;
}

/* Devuelve la clave actual de la iteracion */
const char *hash_iter_ver_actual(const hash_iter_t *iter)
{
	if (!hash_iter_al_final(iter))
	{
		nodo_hash_t *nodo = lista_iter_ver_actual(iter->lista_iter);
		return nodo->clave;
	}	
	return NULL;
}


/* Devuelve si el iterador esta al final de la iteracion */
bool hash_iter_al_final(const hash_iter_t *iter)
{
	return (iter->lista_iter == NULL) ? iter->pos == iter->hash->tamanio - 1 : false;
}

/* Destruye el iterador */
void hash_iter_destruir(hash_iter_t* iter)
{
	lista_iter_destruir(iter->lista_iter);
	free(iter);
}
