#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "hash.h"
#include "lista.h"

#define TAMANIO 997
#define MAX_REDIMENSION 0.7
#define FACTOR 10

/* Estructura del hash CERRADO */
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
	char estado;
	/*
		0 Vacio
		1 Ocupado
		2 Borrado
	*/
} nodo_hash_t;

/* Estructura del iterador */
struct hash_iter {
	size_t pos;
	const hash_t *hash;
};

/**/
nodo_hash_t* nodo_hash_crear(const char *clave, void *dato);

/**/
void nodo_hash_destruir(nodo_hash_t *nodo, hash_destruir_dato_t destruir);

/**/
size_t funcion_hash(const char *clave, size_t tamanio);

/**/
nodo_hash_t* nodo_buscar(lista_t *lista,const char *clave);

/**/
void redimensionar(hash_t *hash, size_t tamanio);

/**/
void hash_destruir_tabla(lista_t **tabla, size_t tamanio, hash_destruir_dato_t destruir);


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

void nodo_hash_destruir(nodo_hash_t *nodo, hash_destruir_dato_t destruir)
{
	if (destruir != NULL)
		destruir(nodo->dato);
	free(nodo->clave);
	free(nodo);
}

/**/
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
		/*************************************************/
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
		/****************************************/
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	hash_destruir_tabla(hash->tabla, hash->tamanio, NULL);
	free(hash->tabla);
	hash->tabla = tabla;
	hash->tamanio = tamanio;
}

/* Funcion de hash */
//	PRE: La clave nunca puede ser null
//	POST: Devuelve una posicion de la tabla

size_t funcion_hash(const char *clave, size_t tamanio)
{
	unsigned int hash = 0;
	for (unsigned int i = 0; i < strlen(clave); i++)
		hash = 31 * hash + clave[i];
	return hash % tamanio;
}

/* Busca un nodo en la lista dada */
//	PRE: La lista fue creada
//	POST: Devuelve el nodo con la clave buscada o devuelve NULL
//	en caso de no encontrarla
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
	
/* Guarda el par clave/valor en la tabla de hash */
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
				//TODO: REDIMENSIONAR
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



/**/
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


/**/
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


/* Devuelve la cantidad de elementos cargados en el hash */
//	PRE: El hash fue creado
//	POST: Devuelve la cantidad de elementos del hash
size_t hash_cantidad(const hash_t *hash)
{
	return hash->cantidad;
}

/**/
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
}


/**/
void hash_destruir(hash_t *hash)
{
	hash_destruir_tabla(hash->tabla, hash->tamanio, hash->destruir);
	free(hash->tabla);
	free(hash);
}

/****************** ITERADOR *****************/


/**/
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


/**/
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

/**/
const char *hash_iter_ver_actual(const hash_iter_t *iter)
{
	if (!hash_iter_al_final(iter))
	{
		nodo_hash_t *nodo = lista_iter_ver_actual(iter->lista_iter);
		return nodo->clave;
	}	
	return NULL;
}


/**/
bool hash_iter_al_final(const hash_iter_t *iter)
{
	return (iter->lista_iter == NULL) ? iter->pos == iter->hash->tamanio - 1 : false;
}

/**/
void hash_iter_destruir(hash_iter_t* iter)
{
	lista_iter_destruir(iter->lista_iter);
	free(iter);
}
