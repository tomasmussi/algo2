#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>


/*********** Lista ***********/
typedef struct lista_t lista_t;
typedef struct _lista_iter_t lista_iter_t;

/* 
	Crea una lista enlazada
	POST: Devuelve una lista vacia 
*/
lista_t *lista_crear();

/*
	Consulta si una lista determinada tiene elementos o no
	PRE: la lista fue creada
	POST: devuelve true si esta vacia, false en otro caso.
*/
bool lista_esta_vacia(const lista_t *lista);

/*
	Inserta un elemento en la primera posicion de la lista
	PRE: la lista fue creada
	POST: devuelve true si se insertó en la primera posición,
	false de lo contrario
*/
bool lista_insertar_primero(lista_t *lista, void *dato);

/*
	Inserta un elemento al final de la lista
	PRE: la lista fue creada
	POST: devuelve true si se pudo insertar el elemento y quedar ultimo en la lista,
	false de lo contrario
*/
bool lista_insertar_ultimo(lista_t *lista, void *dato);

/*
	Borra de la lista el elemento y devuelve un puntero al primer dato almacenado en la lista
	PRE: la lista fue creada
	POST: Devuelve NULL si la lista estaba vacia, sino devuelve un puntero al
	elemento almacenado.
*/
void *lista_borrar_primero(lista_t *lista);

/*
	Devuelve un puntero al primer dato almacenado en la lista
	PRE: la lista fue creada
	POST: Devuelve NULL si la lista estaba vacia, sino devuelve un puntero al
	elemento almacenado
*/
void *lista_ver_primero(const lista_t *lista);

/*
	Devuelve la cantidad de elementos contenidos en la lista
	PRE: la lista fue creada
	POST: Informa la cantidad de elementos que tiene la lista
*/
size_t lista_largo(const lista_t *lista);

/*
	Destruye la lista y todos los elementos contenidos en ella.
	Si se necesita liberar memoria que se haya pedido para los elementos almacenados,
	se deberá pasar un puntero a esa función destructora con el formato indicado en la firma
	de la funcion.
	PRE: la lista fue creada
	POST: la lista se destruye
*/
void lista_destruir(lista_t *lista, void destruir_dato(void *));

/*********** Fin Lista ***********/


/********** Iterador externo ***********/

/*
	Devuelve un iterador para recorrer una lista
	PRE: la lista fue creada
	POST: Devuelve un iterador que apuntará al primer elemento
*/
lista_iter_t *lista_iter_crear(const lista_t *lista);

/*
	Avanza al siguiente elemento de la lista
	PRE: la lista y el iterador fueron creados
	POST: Devuelve true si avanzo, false si no hay nada
*/
bool lista_iter_avanzar(lista_iter_t *iter);

/*
	Muestra el elemento actual de la lista
	PRE: la lista y el iterador fueron creados
	POST: devuelve un puntero al elemento guardado
*/
void *lista_iter_ver_actual(const lista_iter_t *iter);

/*
	Informa si el iterador esta al final de la lista
	PRE: la lista y el iterador fueron creados
	POST: true si no hay ningun elemento mas, false si todavia tiene elementos
*/
bool lista_iter_al_final(const lista_iter_t *iter);

/*
	Destruye el iterador
	PRE: la lista y el iterador fueron creados
	POST: iterador destruido
*/
void lista_iter_destruir(lista_iter_t *iter);

/********** Fin Iterador externo **********/


/********** Iterador interno **********/

void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra);

/********** Fin Iterador interno **********/

/*
	Inserta un elemento en la posicion actual a la que apunta el iterador
	PRE: la lista y el iterador fueron creados
	POST: true si pudo agregar el elemento, false de lo contrario
*/
bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato);

/*
	Borra el elemento al cual esta apuntando el iterador
	PRE: la lista y el iterador fueron creados
	POST: Elemento eliminado de la lista
*/
void *lista_borrar(lista_t *lista, lista_iter_t *iter);



#endif // LISTA_H
