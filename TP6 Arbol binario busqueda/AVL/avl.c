#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "avl.h"
#include "pila.h"

/* Estructura de los nodos del arbol */
typedef struct nodo_avl{
	char *clave;
	void *dato;
	struct nodo_avl *izq;
	struct nodo_avl *der;
	int factor;
} nodo_avl_t;

/* Estructura interna del arbol */
struct avl {
	nodo_avl_t *raiz;
	avl_comparar_clave_t comparar;
	avl_destruir_dato_t destruir;
	size_t cantidad;
};


/* Estructura interna del iterador */
struct avl_iter{
	pila_t *pila;
};

/* FUNCIONES INTERNAS */

// Esta función busca un nodo con la clave dada y si lo encuentra devuelve el nodo.
// Caso contrario devuelve NULL.
nodo_avl_t* nodo_buscar(nodo_avl_t *nodo,const char *clave, avl_comparar_clave_t comparar);

// Crea un nodo con la clave y dato pasados.
nodo_avl_t* nodo_avl_crear(const char *clave, void *dato);

// Destruye el nodo dado.
void nodo_avl_destruir(nodo_avl_t *nodo, avl_destruir_dato_t destruir);

// Función recursiva para el guardado de un nuevo elemento con los datos pasados.
bool avl_guardar_R(nodo_avl_t *padre,avl_t *arbol, const char *clave, void *dato,bool pertenece, bool *balancear);

// Función recursiva para el borrado de un elemento clave igual a la pasada.
nodo_avl_t* avl_borrar_R(nodo_avl_t *hijo, nodo_avl_t *padre, avl_comparar_clave_t comparar, const char *clave);

// Función recursiva para recorrer el arbol de manera IN ORDER.
void avl_in_order_R(nodo_avl_t *nodo, bool visitar(const char *, void *, void *), void *extra,bool *seguir);


nodo_avl_t* rotar_izq(nodo_avl_t *padre);
nodo_avl_t* rotar_der(nodo_avl_t *padre);

nodo_avl_t* rotar_izq_der(nodo_avl_t *padre);
nodo_avl_t* rotar_der_izq(nodo_avl_t *padre);
void calcular_factor(nodo_avl_t *nodo);


void avl_balancear(avl_t *arbol, pila_t *desbalanceados);
int avl_altura_R(const nodo_avl_t *nodo);
int max(int n1, int n2);
int abs(int factor);

int max(int n1, int n2) {
	return n1 > n2 ? n1 : n2;
}



/* FIN FUNCIONES INTERNAS */


nodo_avl_t* nodo_buscar(nodo_avl_t *nodo,const char *clave, avl_comparar_clave_t comparar)
{
	if (nodo == NULL)
		return NULL;
	if (comparar(nodo->clave, clave) == 0)
		return nodo;
	return comparar(nodo->clave, clave) < 0 ? nodo_buscar(nodo->der, clave, comparar) : nodo_buscar(nodo->izq, clave, comparar);
}

nodo_avl_t* nodo_avl_crear(const char *clave, void *dato)
{
	nodo_avl_t *nodo = malloc(sizeof(nodo_avl_t));
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
	nodo->factor = 0;
	return nodo;
}

void nodo_avl_destruir(nodo_avl_t *nodo, avl_destruir_dato_t destruir)
{
	if (destruir != NULL)
		destruir(nodo->dato);
	free(nodo->clave);
	free(nodo);
}

avl_t* avl_crear(avl_comparar_clave_t cmp, avl_destruir_dato_t destruir_dato)
{
	avl_t *arbol = malloc(sizeof(avl_t));
	if (arbol == NULL)
		return NULL;
	arbol->raiz = NULL;
	arbol->comparar = cmp;
	arbol->destruir = destruir_dato;
	arbol->cantidad = 0;
	return arbol;
}

bool avl_guardar(avl_t *arbol, const char *clave, void *dato) {
	bool guardado;
	bool pertenece = nodo_buscar(arbol->raiz, clave, arbol->comparar);
	pila_t *desbalanceados = pila_crear();
	if (arbol->raiz != NULL) {
		guardado = avl_guardar_R(arbol->raiz,arbol, clave, dato, pertenece, desbalanceados);
	} else {
		// No habia nada
		nodo_avl_t *nodo = nodo_avl_crear(clave,dato);
		if (nodo == NULL) {
			guardado = false;
		} else {
			arbol->raiz = nodo;
			arbol->cantidad++;
			guardado = true;
		}
	}
	if (!pila_esta_vacia(desbalanceados))
		avl_balancear(arbol, desbalanceados);
	pila_destruir(desbalanceados);
	return guardado;
}

bool avl_guardar_R(nodo_avl_t *padre,avl_t *arbol, const char *clave, void *dato, bool pertenece, pila_t *desbalanceados) {
	int i = arbol->comparar(padre->clave, clave);

	if (desbalanceados == NULL){
		desbalanceados = pila_crear();
		if (desbalanceados == NULL)
			return false;
	}

	if (i == 0)	{
		if (arbol->destruir != NULL)
			arbol->destruir(padre->dato);
		padre->dato = dato;
		return true;
	}
	if (!pertenece) {
		if (i > 0)
			padre->factor--;
		else
			padre->factor++;

		if (pila_esta_vacia(desbalanceados)) {
			if (abs(padre->factor) > 1)
				pila_apilar(desbalanceados, padre);
		} else {
			pila_apilar(desbalanceados, padre);
		}
	}
	nodo_avl_t *hijo = i < 0 ? padre->der : padre->izq;
	if (hijo != NULL)
		return avl_guardar_R(hijo,arbol,clave,dato, pertenece, desbalanceados);
	else {
		nodo_avl_t *nodo = nodo_avl_crear(clave, dato);
		if (nodo == NULL)
			return false;
		if (i < 0)
			padre->der = nodo;
		else
			padre->izq = nodo;
		arbol->cantidad++;
		return true;
	}
}

nodo_avl_t* clave_buscar_minimo(nodo_avl_t *hijo, nodo_avl_t *padre, avl_comparar_clave_t comparar) {
	if (hijo->izq != NULL)
		return clave_buscar_minimo(hijo->izq, hijo, comparar);
	return avl_borrar_R(hijo, padre, comparar, hijo->clave);
}

void *avl_borrar(avl_t *arbol, const char *clave) {
	if (avl_esta_vacio(arbol))
		return NULL;
	if (arbol->comparar(arbol->raiz->clave, clave) == 0) {
		void *dato = arbol->raiz->dato;
		nodo_avl_t *nueva_raiz = NULL;
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
		nodo_avl_destruir(arbol->raiz, NULL);
		arbol->raiz = nueva_raiz;
		arbol->cantidad--;
		return dato;
	} else {
		nodo_avl_t *nodo = avl_borrar_R(arbol->raiz, NULL, arbol->comparar, clave);
		if (nodo == NULL)
			return NULL;
		void *dato = nodo->dato;
		nodo_avl_destruir(nodo, NULL);
		arbol->cantidad--;
		return dato;
	}
}

nodo_avl_t* avl_borrar_R(nodo_avl_t *hijo, nodo_avl_t *padre, avl_comparar_clave_t comparar, const char *clave){
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
			nodo_avl_t *minimo = clave_buscar_minimo(hijo->der, hijo, comparar);
			minimo->izq = hijo->izq;
			minimo->der = minimo != hijo->der ? hijo->der : NULL;
			if (padre->izq == hijo)
				padre->izq = minimo;
			else
				padre->der = minimo;
		}
		return hijo;
	}
	return avl_borrar_R(i > 0 ? hijo->izq : hijo->der, hijo, comparar, clave);
}

void *avl_obtener(const avl_t *arbol, const char *clave) {
	nodo_avl_t *buscado = nodo_buscar(arbol->raiz, clave, arbol->comparar);
	return buscado != NULL ? buscado->dato : NULL;
}

bool avl_pertenece(const avl_t *arbol, const char *clave) {
	return nodo_buscar(arbol->raiz, clave, arbol->comparar) != NULL;
}

size_t avl_cantidad(avl_t *arbol) {
	return arbol->cantidad;
}

/* PRIMITIVAS DEL AVL */



nodo_avl_t* rotar_izq(nodo_avl_t *padre){
	nodo_avl_t *nuevo_padre = padre->der;
	padre->der = nuevo_padre->izq;
	nuevo_padre->izq = padre;
	calcular_factor(padre);
	calcular_factor(nuevo_padre);
	return nuevo_padre;
}

nodo_avl_t* rotar_der(nodo_avl_t *padre) {
	nodo_avl_t *nuevo_padre = padre->izq;
	padre->izq = nuevo_padre->der;
	nuevo_padre->der = padre;
	calcular_factor(padre);
	calcular_factor(nuevo_padre);
	return nuevo_padre;
}

nodo_avl_t* rotar_izq_der(nodo_avl_t *padre){
	padre->izq = rotar_izq(padre->izq);
	return rotar_der(padre);
}

nodo_avl_t* rotar_der_izq(nodo_avl_t *padre){
	padre->der = rotar_der(padre->der);
	return rotar_izq(padre);
}

void balancear_R(nodo_avl_t *hijo, nodo_avl_t *padre, nodo_avl_t *abuelo) {
	//Si se cumple alguno, es porque todavia tengo que seguir buscando cual es el primer
	//nodo desbalanceado
	if (hijo->factor > 1)
		balancear_R(hijo->der, hijo, padre);
	else if (hijo->factor < -1)
		balancear_R(hijo->izq, hijo, padre);
	else {
		//Primer nodo que esta desbalanceado, evaluo los casos
		if (padre->factor == 2 && hijo->factor == 1) {
			// Rotacion simple a la izquierda
			if (abuelo->izq == padre)
				abuelo->izq = rotar_izq(padre);
			else
				abuelo->der = rotar_izq(padre);
		} else if (padre->factor == -2 && hijo->factor == -1) {
			// Rotacion simple a la derecha
			if (abuelo->izq == padre)
				abuelo->izq = rotar_der(padre);
			else
				abuelo->der = rotar_der(padre);
		} else if (padre->factor == 2 && hijo->factor == -1) {
			//Rotacion doble, primero derecha, despues izquierda
			if (abuelo->izq == padre)
				abuelo->izq = rotar_der_izq(padre);
			else
				abuelo->der = rotar_der_izq(padre);
		} else if (padre->factor == -2 && hijo->factor == 1){
			//Rotacion doble, primero izquierda, despues derecha
			if (abuelo->izq == padre)
				abuelo->izq = rotar_izq_der(padre);
			else
				abuelo->der = rotar_izq_der(padre);
		}
	}
	calcular_factor(abuelo);
}

void avl_balancear(avl_t *arbol, pila_t *desbalanceados) {
	//Si la raiz no es la unica desbalanceada, entonces lo puedo hacer recursivo porque no hay que tocar la raiz
	nodo_avl_t *nodo = pila_desapilar(desbalanceados);
	if (nodo != arbol->raiz) {
		
	}
	int f_izq = nodo->izq != NULL ? abs(nodo->izq->factor) : 0;
	int f_der = nodo->der != NULL ? abs(nodo->der->factor) : 0;
	if (f_izq > 1 || f_der > 1) {
		balancear_R(arbol->raiz, NULL, NULL);
	} else {
		//La raiz esta involucrada en las rotaciones, evaluo casos
		if (arbol->raiz->factor == 2 && arbol->raiz->der->factor == 1) {
			//Caso de rotacion simple a la izquierda
			arbol->raiz = rotar_izq(arbol->raiz);
		} else if (arbol->raiz->factor == -2 && arbol->raiz->izq->factor == -1) {
			//Caso de rotacion simple a la derecha
			arbol->raiz = rotar_der(arbol->raiz);
		} else if (arbol->raiz->factor == 2 && arbol->raiz->der->factor == -1) {
			//Caso de rotacion derecha, despues izquierda
			arbol->raiz = rotar_der_izq(arbol->raiz);
		} else if (arbol->raiz->factor == -2 && arbol->raiz->izq->factor == 1) {
			//Caso de rotacion izquierda, despues derecha
			arbol->raiz = rotar_izq_der(arbol->raiz);
		}
		calcular_factor(arbol->raiz);
	}
}

void calcular_factor(nodo_avl_t *nodo) {
	if (nodo == NULL)
		return;
	nodo->factor = avl_altura_R(nodo->der) - avl_altura_R(nodo->izq);
}

int abs(int factor) {
	return factor >= 0 ? factor : factor * -1;
}

bool avl_balanceado(const avl_t *arbol) {
	calcular_factor(arbol->raiz);
	return arbol->raiz->factor <= 1;
}

int avl_altura(const avl_t *arbol) {
	return avl_altura_R(arbol->raiz);
}

int avl_altura_R(const nodo_avl_t *nodo) {
	if (nodo == NULL)
		return 0;
	return 1 + max(avl_altura_R(nodo->izq), avl_altura_R(nodo->der));
}

/* FIN PRIMITIVAS AVL */

// Esta función se llama recursivamente para destruir todos los nodos del arbol.
void destruir_nodos(nodo_avl_t *nodo, void destruir(nodo_avl_t*, avl_destruir_dato_t), avl_destruir_dato_t destruir_dato) {
	if (nodo != NULL) {
		destruir_nodos(nodo->izq, destruir, destruir_dato);
		destruir_nodos(nodo->der, destruir, destruir_dato);
		destruir(nodo, destruir_dato);
	}
}

void avl_destruir(avl_t *arbol) {
	destruir_nodos(arbol->raiz, nodo_avl_destruir, arbol->destruir);
	free(arbol);
}

bool avl_esta_vacio(const avl_t *arbol) {
	return arbol->cantidad == 0;
}


void avl_in_order(avl_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	bool seguir = true;
	avl_in_order_R(arbol->raiz, visitar, extra, &seguir);
}

void avl_in_order_R(nodo_avl_t *nodo, bool visitar(const char *, void *, void *), void *extra, bool *seguir) {
	if (nodo != NULL && (*seguir)) {
		avl_in_order_R(nodo->izq, visitar, extra, seguir);
		if (*seguir)
			*seguir = visitar(nodo->clave, nodo->dato, extra);
		if (!(*seguir))
			return;
		avl_in_order_R(nodo->der, visitar, extra, seguir);
	}
}

avl_iter_t *avl_iter_in_crear(const avl_t *arbol) {
	avl_iter_t* iter = malloc(sizeof(avl_iter_t));
	if (iter == NULL) return NULL;
	pila_t* pila = pila_crear();
	if (pila == NULL){
		free(iter);
		return NULL;
	}
	nodo_avl_t* nodo = arbol->raiz;
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

bool avl_iter_in_avanzar(avl_iter_t *iter) {
	pila_t* pila = iter->pila;
	if (pila_esta_vacia(pila)) return false;
	nodo_avl_t* nodo = (nodo_avl_t*) pila_desapilar(pila);
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

const char *avl_iter_in_ver_actual(const avl_iter_t *iter) {
	pila_t* pila = iter->pila;
	if (pila_esta_vacia(pila)) return NULL;
	nodo_avl_t* nodo = (nodo_avl_t*) pila_ver_tope(pila);
	return nodo->clave;
}

bool avl_iter_in_al_final(const avl_iter_t *iter) {
	return pila_esta_vacia(iter->pila);
}

void avl_iter_in_destruir(avl_iter_t* iter) {
	pila_destruir(iter->pila);
	free(iter);
}

