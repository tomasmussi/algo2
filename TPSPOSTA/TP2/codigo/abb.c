#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "abb.h"
#include "pila.h"

/* Estructura de los nodos del arbol */
typedef struct nodo_abb{
	char *clave;
	void *dato;
	struct nodo_abb *izq;
	struct nodo_abb *der;
} nodo_abb_t;

/* Estructura interna del arbol */
struct abb {
	nodo_abb_t *raiz;
	abb_comparar_clave_t comparar;
	abb_destruir_dato_t destruir;
	size_t cantidad;
};


/* Estructura interna del iterador */
struct abb_iter{
	pila_t *pila;
};

/* FUNCIONES INTERNAS */

// Esta función busca un nodo con la clave dada y si lo encuentra devuelve el nodo.
// Caso contrario devuelve NULL.
nodo_abb_t* nodo_abb_buscar(nodo_abb_t *nodo,const char *clave, abb_comparar_clave_t comparar);

// Crea un nodo con la clave y dato pasados.
nodo_abb_t* nodo_abb_crear(const char *clave, void *dato);

// Destruye el nodo dado.
void nodo_abb_destruir(nodo_abb_t *nodo, abb_destruir_dato_t destruir);

// Función recursiva para el guardado de un nuevo elemento con los datos pasados.
bool abb_guardar_R(nodo_abb_t *padre,abb_comparar_clave_t comparar,	abb_destruir_dato_t destruir, const char *clave, void *dato,
	size_t *abb_cantidad);

// Función recursiva para el borrado de un elemento clave igual a la pasada.
nodo_abb_t* abb_borrar_R(nodo_abb_t *hijo, nodo_abb_t *padre, abb_comparar_clave_t comparar, const char *clave);

// Función recursiva para recorrer el arbol de manera IN ORDER.
void abb_in_order_R(nodo_abb_t *nodo, bool visitar(const char *, void *, void *), void *extra,bool *seguir);


/* FIN FUNCIONES INTERNAS */

nodo_abb_t* nodo_abb_buscar(nodo_abb_t *nodo,const char *clave, abb_comparar_clave_t comparar)
{
	if (nodo == NULL)
		return NULL;
	if (comparar(nodo->clave, clave) == 0)
		return nodo;
	return comparar(nodo->clave, clave) < 0 ? nodo_abb_buscar(nodo->der, clave, comparar) : nodo_abb_buscar(nodo->izq, clave, comparar);
}

nodo_abb_t* nodo_abb_crear(const char *clave, void *dato)
{
	nodo_abb_t *nodo = malloc(sizeof(nodo_abb_t));
	if (nodo == NULL)
		return NULL;
	char *nodo_clave = malloc((strlen(clave) + 1) * sizeof(char));
	if (nodo_clave == NULL)
	{
		free(nodo);
		return NULL;
	}
	strcpy(nodo_clave, clave);
	nodo->clave = nodo_clave;
	nodo->dato = dato;
	nodo->izq = NULL;
	nodo->der = NULL;
	return nodo;
}

void nodo_abb_destruir(nodo_abb_t *nodo, abb_destruir_dato_t destruir)
{
	if (destruir != NULL)
		destruir(nodo->dato);
	free(nodo->clave);
	free(nodo);
}

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato)
{
	abb_t *arbol = malloc(sizeof(abb_t));
	if (arbol == NULL)
		return NULL;
	arbol->raiz = NULL;
	arbol->comparar = cmp;
	arbol->destruir = destruir_dato;
	arbol->cantidad = 0;
	return arbol;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
	if (arbol->raiz != NULL) {
		return abb_guardar_R(arbol->raiz,arbol->comparar ,arbol->destruir , clave, dato, &(arbol->cantidad));
	} else {
		nodo_abb_t *nodo = nodo_abb_crear(clave,dato);
		if (nodo == NULL)
			return false;
		arbol->raiz = nodo;
		arbol->cantidad++;
		return true;
	}
}

bool abb_guardar_R(nodo_abb_t *padre,abb_comparar_clave_t comparar,	abb_destruir_dato_t destruir, const char *clave, void *dato,
	size_t *abb_cantidad) {
	int i = comparar(padre->clave, clave);
	if (i == 0)	{
		if (destruir != NULL)
			destruir(padre->dato);
		padre->dato = dato;
		return true;
	}
	nodo_abb_t *hijo = i < 0 ? padre->der : padre->izq;
	if (hijo != NULL)
		return abb_guardar_R(hijo, comparar, destruir, clave,dato,abb_cantidad);
	else {
		nodo_abb_t *nodo = nodo_abb_crear(clave, dato);
		if (nodo == NULL)
			return false;
		if (i < 0)
			padre->der = nodo;
		else
			padre->izq = nodo;
		(*abb_cantidad)++;
		return true;
	}
}

nodo_abb_t* clave_buscar_minimo(nodo_abb_t *hijo, nodo_abb_t *padre, abb_comparar_clave_t comparar) {
	if (hijo->izq != NULL)
		return clave_buscar_minimo(hijo->izq, hijo, comparar);
	return abb_borrar_R(hijo, padre, comparar, hijo->clave);
}

void *abb_borrar(abb_t *arbol, const char *clave) {
	if (abb_esta_vacio(arbol))
		return NULL;
	if (arbol->comparar(arbol->raiz->clave, clave) == 0) {
		void *dato = arbol->raiz->dato;
		nodo_abb_t *nueva_raiz = NULL;
		//Si no tiene hijos, todo bien
		//Si tiene alguno, tengo que elegir por cual reemplazar la nueva raiz
		//Verifico que tenga ambos
		if (arbol->raiz->izq != NULL && arbol->raiz->der != NULL) {
			nueva_raiz = clave_buscar_minimo(arbol->raiz->der, arbol->raiz, arbol->comparar);
			nueva_raiz->izq = arbol->raiz->izq;
			if (nueva_raiz != arbol->raiz->der)
				nueva_raiz->der = arbol->raiz->der;
		} else if (arbol->raiz->der != NULL || arbol->raiz->izq != NULL) {
			//Tiene uno solo de los 2, lo reemplazo por ese
			nueva_raiz = arbol->raiz->der != NULL ? arbol->raiz->der : arbol->raiz->izq;
		}
		nodo_abb_destruir(arbol->raiz, NULL);
		arbol->raiz = nueva_raiz;
		arbol->cantidad--;
		return dato;
	} else {
		nodo_abb_t *nodo = abb_borrar_R(arbol->raiz, NULL, arbol->comparar, clave);
		if (nodo == NULL)
			return NULL;
		void *dato = nodo->dato;
		nodo_abb_destruir(nodo, NULL);
		arbol->cantidad--;
		return dato;
	}
}

nodo_abb_t* abb_borrar_R(nodo_abb_t *hijo, nodo_abb_t *padre, abb_comparar_clave_t comparar, const char *clave){
	if (hijo == NULL)
		return NULL;
	int i = comparar(hijo->clave, clave);
	if (i == 0){
		//Evaluo los 3 casos
		//Caso 1: nodo sin hijos
		if (hijo->der == NULL && hijo->izq == NULL) {
			if (padre->der == hijo)
				padre->der = NULL;
			else
				padre->izq = NULL;
		} else if ((hijo->der == NULL && hijo->izq != NULL) || (hijo->izq == NULL && hijo->der != NULL)) {
			//Caso 2: el nodo tiene un unico hijo
			if (padre->der == hijo)
				padre->der = hijo->izq != NULL ? hijo->izq : hijo->der;
			else
				padre->izq = hijo->izq != NULL ? hijo->izq : hijo->der;
		} else {
			//Caso 3: tiene 2 hijos, buscar minimo de la derecha
			nodo_abb_t *minimo = clave_buscar_minimo(hijo->der, hijo, comparar);
			minimo->izq = hijo->izq;
			minimo->der = minimo != hijo->der ? hijo->der : NULL;
			if (padre->izq == hijo)
				padre->izq = minimo;
			else
				padre->der = minimo;
		}
		return hijo;
	}
	return abb_borrar_R(i > 0 ? hijo->izq : hijo->der, hijo, comparar, clave);
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
	nodo_abb_t *buscado = nodo_abb_buscar(arbol->raiz, clave, arbol->comparar);
	return buscado != NULL ? buscado->dato : NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
	return nodo_abb_buscar(arbol->raiz, clave, arbol->comparar) != NULL;
}

size_t abb_cantidad(abb_t *arbol) {
	return arbol->cantidad;
}

// Esta función se llama recursivamente para destruir todos los nodos del arbol.
void destruir_nodos(nodo_abb_t *nodo, void destruir(nodo_abb_t*, abb_destruir_dato_t), abb_destruir_dato_t destruir_dato) {
	if (nodo != NULL) {
		destruir_nodos(nodo->izq, destruir, destruir_dato);
		destruir_nodos(nodo->der, destruir, destruir_dato);
		destruir(nodo, destruir_dato);
	}
}

void abb_destruir(abb_t *arbol) {
	destruir_nodos(arbol->raiz, nodo_abb_destruir, arbol->destruir);
	free(arbol);
}

bool abb_esta_vacio(const abb_t *arbol) {
	return arbol->cantidad == 0;
}


void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	bool seguir = true;
	abb_in_order_R(arbol->raiz, visitar, extra, &seguir);
}

void abb_in_order_R(nodo_abb_t *nodo, bool visitar(const char *, void *, void *), void *extra, bool *seguir) {
	if (nodo != NULL && (*seguir)) {
		abb_in_order_R(nodo->izq, visitar, extra, seguir);
		if (*seguir)
			*seguir = visitar(nodo->clave, nodo->dato, extra);
		if (!(*seguir))
			return;
		abb_in_order_R(nodo->der, visitar, extra, seguir);
	}
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if (iter == NULL) return NULL;
	pila_t* pila = pila_crear();
	if (pila == NULL){
		free(iter);
		return NULL;
	}
	nodo_abb_t* nodo = arbol->raiz;
	// si la raiz es NULL es porque el arbol esta vacio
	if (nodo != NULL){
		// apilo la raiz primero, y luego si existen los hijos izquierdos
		pila_apilar(pila, nodo);
		while (nodo->izq != NULL){
			nodo = nodo->izq;
			pila_apilar(pila, nodo);
		}
	}
	iter->pila = pila;

	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
	pila_t* pila = iter->pila;
	if (pila_esta_vacia(pila)) return false;
	nodo_abb_t* nodo = (nodo_abb_t*) pila_desapilar(pila);
	if (nodo->der != NULL){
		nodo = nodo->der;
		pila_apilar(pila, nodo);
		while (nodo->izq != NULL){
			nodo = nodo->izq;
			pila_apilar(pila, nodo);
		}
	}
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
	pila_t* pila = iter->pila;
	if (pila_esta_vacia(pila)) return NULL;
	nodo_abb_t* nodo = (nodo_abb_t*) pila_ver_tope(pila);
	return nodo->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
	return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
	pila_destruir(iter->pila);
	free(iter);
}

