#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h> //BORRAR

#include "abb.h"
#include "pila.h"

/* Estructura de los nodos del arbol AVL */
typedef struct nodo_abb {
	char *clave;
	void *dato;
	int altura;
	struct nodo_abb *izq;
	struct nodo_abb *der;
} nodo_abb_t;

/* Estructura interna del arbol AVL */
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

/* ***************************************
 * 		DEFINICION	FUNCIONES AUXILIARES *
 * 										 *
 * ***************************************/


/* Balancea el nodo y sus ramas en caso de haber un desbalance */
nodo_abb_t* ajustar_uno(nodo_abb_t *nodo);

/* Esta función busca un nodo con la clave dada y si lo encuentra devuelve el nodo.
 Caso contrario devuelve NULL. */
nodo_abb_t* nodo_buscar(nodo_abb_t *nodo,const char *clave, abb_comparar_clave_t comparar);

/* Crea un nodo con la clave y dato pasados. */
nodo_abb_t* nodo_abb_crear(const char *clave, void *dato);

/* Destruye el nodo dado. */
void nodo_abb_destruir(nodo_abb_t *nodo, abb_destruir_dato_t destruir);

/* Hace una rotacion hacia la izquierda de los nodos.
 * Devuelve la nueva RAIZ
 * 
 *		  z                                y
 *		 /  \                            /   \ 
 *		T1   y     Left Rotate(z)       z      x
 *			/  \   - - - - - - - ->    / \    / \
 *		   T2   x                     T1  T2 T3  T4
 *			   / \
 *			 T3  T4
 *  */
nodo_abb_t* rotar_izq(nodo_abb_t *raiz);

/* Hace una rotacion hacia la derecha de los nodos.
 * Devuelve la nueva RAIZ
 *        z                                      y 
 *       / \                                   /   \
 *      y   T4      Right Rotate (z)          x      z
 *     / \          - - - - - - - - ->      /  \    /  \ 
 *    x   T3                               T1  T2  T3  T4
 *   / \
 * T1   T2
 * */
nodo_abb_t* rotar_der(nodo_abb_t *raiz);

/* Hace una rotacion primero hacia la izquierda y luego hacia la derecha.
 * Devuelve la nueva RAIZ
 *		 z                               z                           x
 *		/ \                            /   \                        /  \ 
 *	   y   T4  Left Rotate (y)        x    T4  Right Rotate(z)    y      z
 *	  / \      - - - - - - - - ->    /  \      - - - - - - - ->  / \    / \
 *	T1   x                          y    T3                    T1  T2 T3  T4
 *		/ \                        / \
 *	  T2   T3                    T1   T2
 *
 * */
nodo_abb_t* rotar_izq_der(nodo_abb_t *raiz);

/* Hace una rotacion primero a la derecha y luego hacia la izquierda.
 * Devuelve la nueva RAIZ
 *	   z                            z                            x
 *	  / \                          / \                          /  \ 
 *	T1   y   Right Rotate (y)    T1   x      Left Rotate(z)   z      y
 *		/ \  - - - - - - - - ->     /  \   - - - - - - - ->  / \    / \
 *	   x   T4                      T2   y                  T1  T2  T3  T4
 *	  / \                              /  \
 *	T2   T3                           T3   T4
 * */
nodo_abb_t* rotar_der_izq(nodo_abb_t *raiz);

/* Función recursiva para el guardado de un nuevo elemento con los datos pasados. */
int abb_guardar_R(abb_t *arbol, nodo_abb_t **raiz,const char *clave, void *dato);

/* Función recursiva para el borrado de un elemento clave igual a la pasada. */
bool abb_borrar_R(abb_t *arbol, nodo_abb_t **raiz, const char *clave, void **dato);

/* Funcion que determina si un nodo es hoja */
bool es_hoja(nodo_abb_t **nodo);

/* Reemplaza un nodo por su hijo mas conveniente */
int reemplazar_por_hijo(nodo_abb_t **raiz);

/* Función recursiva para recorrer el arbol de manera IN ORDER. */
void abb_in_order_R(nodo_abb_t *nodo, bool visitar(const char *, void *, void *), void *extra,bool *seguir);

/* ***************************************
 * 	FIN	DEFINICION	FUNCIONES AUXILIARES *
 * 										 *
 * ***************************************/



/* ***************************************
 * 			FUNCIONES AUXILIARES		 *
 * 										 *
 * ***************************************/
static int altura(nodo_abb_t *nodo){
	if (!nodo)
		return 0;
	return nodo->altura;
}

size_t maximo(size_t n1, size_t n2){
	return n1 > n2 ? n1 : n2;
}

nodo_abb_t* rotar_izq(nodo_abb_t *raiz){
	if (!raiz)
		return NULL;
	nodo_abb_t *reemplazo = raiz->der;
	raiz->der = reemplazo->izq;
	reemplazo->izq = raiz;
	
	raiz->altura = maximo(  altura(raiz->der),  altura(raiz->izq)  ) + 1;
	reemplazo->altura = maximo( altura(reemplazo->der) , altura(reemplazo->izq) ) + 1;
	return reemplazo;
}

nodo_abb_t* rotar_der(nodo_abb_t *raiz){
	nodo_abb_t *reemplazo = raiz->izq;
	raiz->izq = reemplazo->der;
	reemplazo->der = raiz;	
	
	raiz->altura = maximo(  altura(raiz->der),  altura(raiz->izq)  ) + 1;
	reemplazo->altura = maximo( altura(reemplazo->der) , altura(reemplazo->izq) ) + 1;
	return reemplazo;
}

nodo_abb_t* rotar_izq_der(nodo_abb_t *raiz){
	raiz->izq = rotar_izq(raiz->izq);
	return rotar_der(raiz);
}

nodo_abb_t* rotar_der_izq(nodo_abb_t *raiz){
	raiz->der = rotar_der(raiz->der);
	return rotar_izq(raiz);	
}

nodo_abb_t* ajustar_uno(nodo_abb_t *nodo){
	switch (altura(nodo->der) - altura(nodo->izq)){
		case 2:
				// Rama derecha mas alta
				if ( altura(nodo->der->der) - altura(nodo->der->izq) == -1){
					// Requiere rotar primero hacia la derecha
					nodo = rotar_der_izq(nodo);
				} else {
					// Caso simple, solo rotar a la izq
					nodo = rotar_izq(nodo);
				}				
				break;
		case -2:
				// Rama izquiera mas alta
				if ( altura(nodo->izq->der) - altura(nodo->izq->izq) == 1 ){
					// Requiere rotar primero hacia la izquierda
					nodo = rotar_izq_der(nodo);
				} else {
					// Caso simple, solo rotar a la derecha
					nodo = rotar_der(nodo);
				}
				break;
		default:
				nodo->altura = maximo(altura(nodo->izq), altura(nodo->der)) + 1;
				break;
	};	
	return nodo;
}

nodo_abb_t* nodo_buscar(nodo_abb_t *nodo,const char *clave, abb_comparar_clave_t comparar)
{
	if (nodo == NULL)
		return NULL;
	if (comparar(nodo->clave, clave) == 0)
		return nodo;
	return comparar(nodo->clave, clave) < 0 ? nodo_buscar(nodo->der, clave, comparar) : nodo_buscar(nodo->izq, clave, comparar);
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
	nodo->altura = 1;
	return nodo;
}

void nodo_abb_destruir(nodo_abb_t *nodo, abb_destruir_dato_t destruir)
{
	if (destruir != NULL)
		destruir(nodo->dato);
	free(nodo->clave);
	free(nodo);
}


// Devuelve:
//   0 en el caso de reemplazar (no hace falta ajustar)
// 	 1 en el caso de haber insertado correctamente
// 	-1 en el caso de haber ocurrido un error
int abb_guardar_R(abb_t *arbol, nodo_abb_t **raiz,const char *clave, void *dato) {
	// Si el nodo raiz es NULL, inserto aca
	if (! *raiz){
		*raiz = nodo_abb_crear(clave, dato);
		return (*raiz != NULL) ? 1: -1;
	}

	int comp = arbol->comparar((*raiz)->clave, clave);
	if (comp != 0){
		if (comp > 0)
			comp = 1;
		else
			comp = -1;
	}
	nodo_abb_t **rama = NULL;
	switch (comp){
		case 0:
				if (arbol->destruir != NULL)
					arbol->destruir((*raiz)->dato);
				(*raiz)->dato = dato;
				return 0;
				break;
		case 1:
				// (*raiz)->clave > clave => izq
				rama = &((*raiz)->izq);
				break;
		case -1:
				// (*raiz)->clave < clave => derecha
				rama = &((*raiz)->der);
				break;
	};

	int respuesta = abb_guardar_R(arbol, rama, clave, dato);
	if (respuesta == 1){
		*raiz = ajustar_uno(*raiz);
	}
	return respuesta;
}

bool es_hoja(nodo_abb_t **nodo){
	return (*nodo)->izq == NULL && (*nodo)->der == NULL;
}

nodo_abb_t** menor_de_mayores(nodo_abb_t **raiz){
	nodo_abb_t **reemplazo = &((*raiz)->der);
	while ( (*reemplazo)->izq != NULL){
		reemplazo = &(*reemplazo)->izq;
	}
	return reemplazo;
}

nodo_abb_t** mayor_de_menores(nodo_abb_t **raiz){
	nodo_abb_t **reemplazo = &((*raiz)->izq);
	while ( (*reemplazo)->der != NULL){
		reemplazo = &(*reemplazo)->der;
	}
	return reemplazo;
}

/*
 * Esta funcion devuelve:
 * 0 si el nodo es una hoja (listo para borrarse del arbol)
 * 1 si hace un swap con el menor hijo de los mayores (derecha) del nodo raiz
 * -1 si hace un swap con el mayor hijo de los menores (izquierda) del nodo raiz
 * */
int reemplazar_por_hijo(nodo_abb_t **raiz){
	if (! *raiz || es_hoja(raiz)){
		return 0;
	}
	
	nodo_abb_t **reemplazo = NULL;
	int respuesta = 0;
	if (altura((*raiz)->der) >= altura((*raiz)->izq)){
		reemplazo = menor_de_mayores(raiz);
		respuesta = 1;
	} else {
		reemplazo = mayor_de_menores(raiz);
		respuesta = -1;
	}

	// Swap de clave y valor
	void* aux;
	aux = (*raiz)->clave;
	(*raiz)->clave = (*reemplazo)->clave;
	(*reemplazo)->clave = aux;
	
	aux = (*raiz)->dato;
	(*raiz)->dato = (*reemplazo)->dato;
	(*reemplazo)->dato = aux;
	return respuesta;
}



// Devuelve:
// 	true  en el caso de haber borrado correctamente
// 	false en el caso de haber ocurrido un error (no se encontro la clave a borrar)
bool abb_borrar_R(abb_t *arbol, nodo_abb_t **raiz, const char *clave, void **dato){
	if ((*raiz) == NULL){
		*dato = NULL;
		return false;
	}

	int cmp = arbol->comparar((*raiz)->clave, clave);
	if (cmp != 0)
		cmp = cmp > 0 ? 1 : -1;
	nodo_abb_t **rama = NULL;
	switch (cmp){
		case 1:
				rama = &((*raiz)->izq);
				break;
		case -1:
				rama = &((*raiz)->der);
				break;				
		case 0:
				// El nodo actual es el que hay que borrar
				// La idea a seguir es: si es hoja, problema resuelto, borro y listo
				// Si tiene algun hijo, evaluo por cual reemplazar. Reemplazo el padre con hijo,
				// hasta convertir el nodo que quiero borrar en una hoja
				switch (reemplazar_por_hijo(raiz)){
					case -1:
							rama = &((*raiz)->izq);
							break;
					case 1:
							rama = &((*raiz)->der);
							break;
					case 0:
							*dato = (*raiz)->dato;
							free((*raiz)->clave);
							free((*raiz));
							
							*raiz = NULL;
							return true;							
				};
				break;
	};
	bool respuesta = abb_borrar_R(arbol, rama, clave, dato);
	if (respuesta){
		*raiz = ajustar_uno(*raiz);
	}
	return respuesta;
}


// Esta función se llama recursivamente para destruir todos los nodos del arbol.
void destruir_nodos(nodo_abb_t *nodo, void destruir(nodo_abb_t*, abb_destruir_dato_t), abb_destruir_dato_t destruir_dato) {
	if (nodo != NULL) {
		destruir_nodos(nodo->izq, destruir, destruir_dato);
		destruir_nodos(nodo->der, destruir, destruir_dato);
		destruir(nodo, destruir_dato);
	}
}

/* ***************************************
 * 		FIN	FUNCIONES AUXILIARES		 *
 * 										 *
 * ***************************************/


 /* ***************************************
 * 			PRIMITIVAS AVL				 *
 * 										 *
 * ***************************************/

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

/* Devuelve true si pudo agregar el nodo, false de lo contrario */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {

	int agregado = abb_guardar_R(arbol, &(arbol->raiz), clave, dato);
	if (agregado == 1){
		arbol->cantidad++;
	}
	return agregado >= 0;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
	if (abb_esta_vacio(arbol))
		return NULL;
	
	void* dato;
	bool borrado = abb_borrar_R(arbol, &(arbol->raiz), clave, &(dato));
	if (!borrado){
		return NULL;
	}
	arbol->cantidad--;
	return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
	nodo_abb_t *buscado = nodo_buscar(arbol->raiz, clave, arbol->comparar);
	return buscado != NULL ? buscado->dato : NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
	return nodo_buscar(arbol->raiz, clave, arbol->comparar) != NULL;
}

size_t abb_cantidad(abb_t *arbol) {
	return arbol->cantidad;
}

void abb_destruir(abb_t *arbol) {
	destruir_nodos(arbol->raiz, nodo_abb_destruir, arbol->destruir);
	free(arbol);
}

bool abb_esta_vacio(const abb_t *arbol) {
	return arbol->cantidad == 0;
}

size_t alt_r(nodo_abb_t *raiz){
	if (raiz == NULL){
		return 0;
	}
	return maximo(alt_r(raiz->izq), alt_r(raiz->der)) + 1;
}

size_t alt(abb_t *arbol){
	return alt_r(arbol->raiz);
}

 /* ***************************************
 * 		FIN	PRIMITIVAS AVL				 *
 * 										 *
 * ***************************************/



 /* ***************************************
 * 			PRIMITIVAS ITERADOR			 *
 * 										 *
 * ***************************************/


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

 /* ***************************************
 * 		FIN	PRIMITIVAS ITERADOR			 *
 * 										 *
 * ***************************************/
