#ifndef ABB_H
#define ABB_H

#include <stdbool.h>

/* Estructura arbol */
typedef struct abb abb_t;

/* Funcion para poder comparar respecto al criterio que se quiera */
//	PRE: La funcion de comparar no puede ser nula
//	POST: Devuelve -1 si el primer parametro es menor que el segundo
//	devuelve 0 si las claves son iguales,
//	devuelve 1 si el segundo parametro es menor que el segundo
typedef int (*abb_comparar_clave_t)(const char*,const char*);

/* Funcion destructora de datos */
typedef void (*abb_destruir_dato_t)(void *);

/* Iterador externo */
typedef struct abb_iter abb_iter_t;

/* Esta funcion crea un arbol binario de busqueda.
 * POST: devulve el arbol creado con las funciones de comparacion y destruccion pasadas.
 * en caso de que el arbol no haya sido creado devuelve NULL.
 */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Esta función guarda la clave y el dato en el arbol en el lugar correspondiente.
 * PRE: el arbol fue creado.
 * POST: true o false dependiendo si pudo o no guardar el dato.
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Esta función borra el nodo que contenga la clave, si existe.
 * PRE: el arbol fue creado.
 * POST: el arbol fue modificado si existía la clave.
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Esta función obtiene el valor guardado en el nodo con la clave recibida.
 * PRE: el arbol fue creado.
 * POST: devuelve el dato en dicha posicion, NULL si la clave no existe.
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Esta función sirve para saber si existe la clave en el arbol.
 * PRE: el arbol fue creado.
 * POST: TRUE/FALSE dependiendo si existe o no la clave en el arbol.
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve la cantidad de elementos del arbol.
 * PRE: el arbol fue creado.
 * POST: cantidad de elementos del arbol.
 */
size_t abb_cantidad(abb_t *arbol);

/* Esta función destruye el arbol.
 * PRE: el arbol fue creado.
 * POST: el arbol fue destruido.
 */
void abb_destruir(abb_t *arbol);

/* Esta función permite saber si el arbol esta vacío o no.
 * PRE: el arbol fue creado.
 * POST: TRUE/FALSE dependiendo si el arbol tiene datos o no.
 */
bool abb_esta_vacio(const abb_t *arbol);

/* Funcion para iterar in orden el arbol */
//	PRE: El arbol fue creado
//	POST: Itera el arbol in orden
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/* Crear un iterador del arbol.
 * PRE: el arbol fue creado.
 * POST: Iterador (IN ORDER) para el arbol.
 */
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/* Avanza la posicion del iterador.
 * PRE: el iterador fue creado.
 * POST: TRUE/FALSE si pudo avanzar.
 */
bool abb_iter_in_avanzar(abb_iter_t *iter);

/* Ver clave actual.
 * PRE: el iterador fue creado.
 * POST: devuelve la clave del elemento actual del iterador.
 */
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/* Esta función permite saber si el iterador esta al final.
 * PRE: el iterador fue creado.
 * POST: TRUE/FALSE dependiendo si la posición actual del iterador esta en el final o no.
 */
bool abb_iter_in_al_final(const abb_iter_t *iter);

/* Destruir el iterador.
 * PRE: el iterador fue creado.
 * POST: el iterador fue destruido.
 */
void abb_iter_in_destruir(abb_iter_t* iter);

/*
 * Devuelve la altura del arbol
 * PRE: El arbol fue creado
 * POST: Devuelve la altura del arbol, si no tiene elementos, la altura es 0
 * BORRAR ESTA FUNCION UNA VEZ TERMINADO EL AVL
 * */
size_t alt(abb_t *arbol);

#endif //ABB_H
