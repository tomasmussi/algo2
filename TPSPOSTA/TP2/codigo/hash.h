#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct hash hash_t;

typedef struct hash_iter hash_iter_t;

typedef void (*hash_destruir_dato_t)(void*);

/**************** FUNCIONES HASH *****************************/
/* Crea una tabla de hash */
//	POST: Devuelve una tabla de hash creada
hash_t *hash_crear(hash_destruir_dato_t destruir_dato);

/* Guarda en el hash un par de clave/valor */
//	PRE: El hash fue creado
//	POST: Devuelve true si pudo guardar el par clave/valor,
//	sino devuelve false. Si la clave ya existia, reemplaza el dato
bool hash_guardar(hash_t *hash, const char *clave, void *dato);

/* Borra del hash la clave y devuelve el valor asociado */
//	PRE: El hash fue creado
//	POST: Si la clave existia, devuelve el dato,
//	sino devuelve NULL
void *hash_borrar(hash_t *hash, const char *clave);

/* Devuelve el valor asociado a la clave del hash */
//	PRE: El hash fue creado
//	POST: Si la clave existe, devuelve el dato asociado,
//	sino devuelve NULL
void *hash_obtener(const hash_t *hash, const char *clave);

/* Informa si la clave ya esta en el hash o no */
//	PRE: El hash fue creado
//	POST: Devuelve true si la clave esta guardada en el hash,
//	false de lo contrario
bool hash_pertenece(const hash_t *hash, const char *clave);

/* Devuelve la cantidad de pares clave/valor contenidos en el hash */
//	PRE: El hash fue creado
//	POST: Devuelve la cantidad de elementos
size_t hash_cantidad(const hash_t *hash);

/* Destruye el hash */
//	PRE: El hash fue creado
//	POST: El hash fue destruido y no se puede usar
void hash_destruir(hash_t *hash);

/**************** FIN FUNCIONES HASH ************************/


/**************** FUNCIONES DEL ITERADOR ********************/

/* Crea un iterador para recorrer todas las claves del hash */
//	PRE: El hash fue creado
//	POST: Devuelve un iterador posicionado en el primer elemento
//	y si el hash no tiene elementos, estará al final del hash
hash_iter_t *hash_iter_crear(const hash_t *hash);

/* Avanza a la siguiente clave contenida en el hash */
//	PRE: El iterador fue creado
//	POST: Devuelve true en caso de que haya podido avanzar a la
//	siguiente clave, false en caso contrario
bool hash_iter_avanzar(hash_iter_t *iter);

/* Devuelve la clave actual de la iteracion */
//	PRE: El iterador fue creado
//	POST: Devuelve la clave actual de iteracion en caso de haber,
//	si el iterador esta al final, devuelve NULL
const char *hash_iter_ver_actual(const hash_iter_t *iter);

/* Devuelve si el iterador esta al final de la iteracion */
//	PRE: El iterador fue creado
//	POST: Devuelve true en caso de que no haya mas claves para iterar,
//	devuelve false si todavia esta situado en una clave
bool hash_iter_al_final(const hash_iter_t *iter);

/* Destruye el iterador */
//	PRE: El iterador fue creado
//	POST: El iterador se destruye y no se puede utilizar mas
void hash_iter_destruir(hash_iter_t* iter);

/**************** FIN FUNCIONES DEL ITERADOR *****************/


#endif // HASH_H
