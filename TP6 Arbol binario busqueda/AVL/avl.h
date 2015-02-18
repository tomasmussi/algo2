#include <stdbool.h>

/* Estructura arbol */
typedef struct avl avl_t;

/* Funcion para poder comparar respecto al criterio que se quiera */
//	PRE: La funcion de comparar no puede ser nula
//	POST: Devuelve -1 si el primer parametro es menor que el segundo
//	devuelve 0 si las claves son iguales,
//	devuelve 1 si el segundo parametro es menor que el segundo
typedef int (*avl_comparar_clave_t)(const char*,const char*);

/* Funcion destructora de datos */
typedef void (*avl_destruir_dato_t)(void *);

/* Iterador externo */
typedef struct avl_iter avl_iter_t;

/* Esta funcion crea un arbol binario de busqueda.
 * POST: devulve el arbol creado con las funciones de comparacion y destruccion pasadas.
 * en caso de que el arbol no haya sido creado devuelve NULL.
 */
avl_t* avl_crear(avl_comparar_clave_t cmp, avl_destruir_dato_t destruir_dato);

/* Esta función guarda la clave y el dato en el arbol en el lugar correspondiente.
 * PRE: el arbol fue creado.
 * POST: true o false dependiendo si pudo o no guardar el dato.
 */
bool avl_guardar(avl_t *arbol, const char *clave, void *dato);

/* Esta función borra el nodo que contenga la clave, si existe.
 * PRE: el arbol fue creado.
 * POST: el arbol fue modificado si existía la clave.
 */
void *avl_borrar(avl_t *arbol, const char *clave);

/* Esta función obtiene el valor guardado en el nodo con la clave recibida.
 * PRE: el arbol fue creado.
 * POST: devuelve el dato en dicha posicion, NULL si la clave no existe.
 */
void *avl_obtener(const avl_t *arbol, const char *clave);

/* Esta función sirve para saber si existe la clave en el arbol.
 * PRE: el arbol fue creado.
 * POST: TRUE/FALSE dependiendo si existe o no la clave en el arbol.
 */
bool avl_pertenece(const avl_t *arbol, const char *clave);

/* Devuelve la cantidad de elementos del arbol.
 * PRE: el arbol fue creado.
 * POST: cantidad de elementos del arbol.
 */
size_t avl_cantidad(avl_t *arbol);

/* Esta función destruye el arbol.
 * PRE: el arbol fue creado.
 * POST: el arbol fue destruido.
 */
void avl_destruir(avl_t *arbol);

/* Esta función permite saber si el arbol esta vacío o no.
 * PRE: el arbol fue creado.
 * POST: TRUE/FALSE dependiendo si el arbol tiene datos o no.
 */
bool avl_esta_vacio(const avl_t *arbol);



/* PRIMITIVAS DEL AVL */
//Devuelve la altura del arbol
int avl_altura(const avl_t *arbol);
bool avl_balanceado(const avl_t *arbol);



/* Funcion para iterar in orden el arbol */
//	PRE: El arbol fue creado
//	POST: Itera el arbol in orden
void avl_in_order(avl_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/* Crear un iterador del arbol.
 * PRE: el arbol fue creado.
 * POST: Iterador (IN ORDER) para el arbol.
 */
avl_iter_t *avl_iter_in_crear(const avl_t *arbol);

/* Avanza la posicion del iterador.
 * PRE: el iterador fue creado.
 * POST: TRUE/FALSE si pudo avanzar.
 */
bool avl_iter_in_avanzar(avl_iter_t *iter);

/* Ver clave actual.
 * PRE: el iterador fue creado.
 * POST: devuelve la clave del elemento actual del iterador.
 */
const char *avl_iter_in_ver_actual(const avl_iter_t *iter);

/* Esta función permite saber si el iterador esta al final.
 * PRE: el iterador fue creado.
 * POST: TRUE/FALSE dependiendo si la posición actual del iterador esta en el final o no.
 */
bool avl_iter_in_al_final(const avl_iter_t *iter);

/* Destruir el iterador.
 * PRE: el iterador fue creado.
 * POST: el iterador fue destruido.
 */
void avl_iter_in_destruir(avl_iter_t* iter);
