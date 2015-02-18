#include <stdlib.h>
#include <stdbool.h>

#include "heap.h"

#define PRIMERO 0
#define TAM_INICIAL 10
#define FACTOR 2

/* Estructura interna del heap */
struct heap {
	void **vector;
	size_t cantidad;
	size_t tamanio;
	cmp_func_t comparar;
};

void swap(void **vector, size_t pos1, size_t pos2){
	void* aux = vector[pos1];
	vector[pos1] = vector[pos2];
	vector[pos2] = aux;
}

/* Redimensiona el heap en caso de necesitar ajustar el tamanio */
bool redimensionar(heap_t *heap, size_t nuevo_tamanio);


bool redimensionar(heap_t *heap, size_t nuevo_tamanio) {
	void **nuevo_vector = realloc(heap->vector, nuevo_tamanio * sizeof(void*));
	if (nuevo_tamanio > 0 && nuevo_vector == NULL)
		return false;
	heap->vector = nuevo_vector;
	heap->tamanio = nuevo_tamanio;
	return true;
}

/* Crea un heap */
heap_t *heap_crear(cmp_func_t cmp) {
	heap_t *heap = malloc(sizeof(heap_t));
	if (heap == NULL)
		return NULL;
	void **vector = malloc(TAM_INICIAL * sizeof(void*));
	if (vector == NULL) {
		free(heap);
		return NULL;
	}
	heap->vector = vector;
	heap->cantidad = 0;
	heap->tamanio = TAM_INICIAL;
	heap->comparar = cmp;
	return heap;
}

/* Elimina el heap */
void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {
	if (destruir_elemento != NULL) {
		for (size_t i = 0; i < heap->cantidad; i++)
			destruir_elemento(heap->vector[i]);
	}
	free(heap->vector);
	free(heap);
}

/* Devuelve la cantidad de elementos que hay en el heap. */
size_t heap_cantidad(const heap_t *heap) {
	return heap->cantidad;
}

/* Devuelve true si la cantidad de elementos que hay en el heap es 0, false en
 * caso contrario. */
bool heap_esta_vacio(const heap_t *heap) {
	return heap->cantidad == 0;
}

// Devulve la posición del padre de la posición dada.
size_t padre_posicion(size_t hijo_pos){
	return (hijo_pos - 1) / 2;
}

void upheap(heap_t* heap, size_t posicion){
	if (posicion > 0){
		size_t padre_pos = padre_posicion(posicion);
		//si el padre es mayor o igual al elemento no hago nada.
		if (heap->comparar(heap->vector[padre_pos], heap->vector[posicion]) < 0){
			swap(heap->vector, posicion, padre_pos);
			upheap(heap, padre_pos);
		}
	}
}

void downheap(heap_t* heap, size_t posicion){
	if (posicion < heap->cantidad){
		int max = -1;
		size_t hijo_izq_pos = 2 * posicion +1;
		size_t hijo_der_pos = 2 * posicion +2;
		void* elem1;
		void* elem2;
		if (hijo_der_pos < heap->cantidad){
			elem1 = heap->vector[hijo_izq_pos];
			elem2 = heap->vector[hijo_der_pos];
			max = heap->comparar(elem1, elem2) > 0 ? hijo_izq_pos : hijo_der_pos;
		} else {
			if (hijo_izq_pos < heap->cantidad) {
				elem1 = heap->vector[hijo_izq_pos];
				max = hijo_izq_pos;
			}
		}
		if (max > -1 && heap->comparar(heap->vector[posicion], heap->vector[max]) < 0){
			swap(heap->vector, posicion, max);
			downheap(heap, max);
		}
	}
}

/* Agrega un elemento al heap. */
bool heap_encolar(heap_t *heap, void *elem) {
	// Primero chequeo si tengo que redimensionar.
	if (heap->cantidad == heap->tamanio && !redimensionar(heap, heap->tamanio * FACTOR))
		return false;
	// Guardo el elemento en la ultima posición y hago upheap para ajustarlo si es necesario.
	heap->vector[heap->cantidad] = elem;
	upheap(heap, heap->cantidad);
	heap->cantidad++;
	return true;
}

/* Devuelve el elemento con máxima prioridad. Si el heap esta vacío, devuelve
 * NULL.
 * Pre: el heap fue creado.
 */
void *heap_ver_max(const heap_t *heap) {
	return (!heap_esta_vacio(heap)) ? heap->vector[PRIMERO] : NULL;
}

/* Elimina el elemento con máxima prioridad, y lo devuelve.
 * Si el heap esta vacío, devuelve NULL.
 * Pre: el heap fue creado.
 * Post: el elemento desencolado ya no se encuentra en el heap.
 */
void *heap_desencolar(heap_t *heap) {
	if (heap_esta_vacio(heap))
		return NULL;
	// Mando el maximo al final, lo saco, y hago downheap del elemento que subi.
	swap(heap->vector, PRIMERO, heap->cantidad -1);
	void* max = heap->vector[heap->cantidad -1];
	heap->cantidad--; //decremento la cantidad para que ya no tenga encuenta el ultimo elemento.
	downheap(heap, PRIMERO);
	return max;
}

/* Restaura el invariante del heap, en caso que se hayan modificado
 * los datos almacenados.
 * Pre: el heap fue creado.
 * Post: Heap en correcto estado
 */
void heap_heapify(heap_t* heap) {
	int primer_padre = (heap->cantidad - 2) / 2;
	while (primer_padre >= 0) {
		downheap(heap, primer_padre);
		primer_padre--;
	}
}

/* Función de heapsort genérica. */
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
	heap_t *heap = malloc(sizeof(heap_t));
	if (heap == NULL)
		return;
	heap->vector = elementos;
	heap->cantidad = cant;
	heap->tamanio = cant + TAM_INICIAL;
	heap->comparar = cmp;
	heap_heapify(heap);
	while (!heap_esta_vacio(heap)) {
		heap_desencolar(heap);
	}
	free(heap);
}

