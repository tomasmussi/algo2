#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "heap.h"

#define MAX 60000
#define MAX_CHAR 50

void print_test(char* prueba, char* name, bool result) {
	printf("%s, %s: %s\n",prueba, name, result ? "OK" : "ERROR");
}

/* Funcion para comparar las claves numericas que almacenamos en char[]*/
int comparar(const void *s1, const void *s2);

/* Funcion para desordenar un vector y poder heapificarlo o usarlo para pruebas.
 * La funcion lo carga con enteros del 0 a tamanio - 1 y los desordena
*/
void inicializar_vector(int *vector, int *vector_original, size_t tamanio);

/* Prueba con un heap vacio */
void prueba_heap_vacio();

/* Prueba del heap con algunos elementos */
void prueba_heap_elementos();

/* Prueba del heap cuando guardo elementos de memoria dinamica */
void prueba_heap_destruccion();

/* Prueba la destruccion de elementos del feed */
void prueba_heap_destruccion();

/* Prueba desordenar los elementos guardados en el heap y reestablecer el orden del heap*/
void prueba_heap_heapificar();

/* Prueba el ordenamiento de un vector mediante el metodo heapsort */
void prueba_heap_sort();

/* Funcion para comparar las claves numericas que almacenamos en char[]*/
int comparar(const void *s1, const void *s2){
	int *n1 = (int*) s1;
	int *n2 = (int*) s2;
	return *n1 - *n2;
}

void vector_desordenar(int *vector,size_t tamanio) {
	for (size_t i = 0; i < tamanio; i++) {
		size_t pos = rand() % tamanio;
		size_t aux = vector[pos];
		vector[pos] = vector[i];
		vector[i] = aux;
	}
	if (vector[tamanio-1] == tamanio - 1){
		vector[tamanio-1] = vector[0];
		vector[0] = tamanio - 1;
	}
}

void inicializar_vector(int *vector, int *vector_original, size_t tamanio) {
	for (size_t i = 0; i < tamanio; i++) {
		vector[i] = i;
		vector_original[i] = i;
	}
	vector_desordenar(vector, tamanio);
}

void prueba_heap_vacio(){
	char prueba[MAX_CHAR] = "Heap vacio";
	printf("\nPrueba %s\n", prueba);
	heap_t *heap = heap_crear(comparar);
	print_test(prueba, "cantidad heap 0", heap_cantidad(heap) == 0);
	print_test(prueba, "heap vacio", heap_esta_vacio(heap));
	print_test(prueba, "heap obtener maximo", heap_ver_max(heap) == NULL);
	print_test(prueba, "heap desencolar", heap_desencolar(heap) == NULL);
	heap_destruir(heap, NULL);
	printf("El heap fue destruido\n");
}

void prueba_heap_elementos(){
	char prueba[MAX_CHAR] = "Heap algunos elementos";
	printf("\nPrueba %s\n", prueba);
	heap_t *heap = heap_crear(comparar);
	size_t tam = 10;
	int vector[tam];
	int vector_original[tam];
	inicializar_vector(vector, vector_original,tam);
	print_test(prueba, "heap encolar 0",heap_encolar(heap, vector));
	print_test(prueba, "cantidad heap 1", heap_cantidad(heap) == 1);
	print_test(prueba, "heap no vacio", !heap_esta_vacio(heap));
	print_test(prueba, "heap obtener maximo", *(int*)heap_ver_max(heap) == vector[0]);
	print_test(prueba, "heap desencolar 0", *(int*)heap_desencolar(heap) == vector[0]);
	print_test(prueba, "cantidad heap 0", heap_cantidad(heap) == 0);
	print_test(prueba, "heap vacio", heap_esta_vacio(heap));
	print_test(prueba, "heap obtener maximo", heap_ver_max(heap) == NULL);
	print_test(prueba, "heap desencolar", heap_desencolar(heap) == NULL);
	bool ok = true;
	for (int i = 0; i < tam; i++) {
		ok = ok && heap_encolar(heap, vector + i);
	}
	print_test(prueba, "elementos encolados", ok);
	print_test(prueba, "cantidad heap 10", heap_cantidad(heap) == tam);
	print_test(prueba, "heap no vacio", !heap_esta_vacio(heap));
	ok = true;
	for (int i = tam - 1; i >= 0; i--) {
		int *pos = heap_desencolar(heap);
		ok = ok && (*pos == vector_original[i]);
	}
	print_test(prueba, "elementos desencolados", ok);
	print_test(prueba, "cantidad heap 0", heap_cantidad(heap) == 0);
	print_test(prueba, "heap vacio", heap_esta_vacio(heap));
	heap_destruir(heap, NULL);
	printf("El heap fue destruido\n");
}

void prueba_heap_volumen() {
	char prueba[MAX_CHAR] = "Heap volumen";
	printf("\nPrueba %s\n", prueba);
	heap_t *heap = heap_crear(comparar);
	int vector[MAX];
	int vector_original[MAX];
	inicializar_vector(vector,vector_original, MAX);
	//Vector inicializado, meto los elementos
	bool ok = true;
	for (int i = 0; i < MAX; i++) {
		ok = ok && heap_encolar(heap, vector + i);
	}
	print_test(prueba, "elementos encolados", ok);
	print_test(prueba, "cantidad heap MAX", heap_cantidad(heap) == MAX);
	print_test(prueba, "heap obtener maximo", (*((int*)heap_ver_max(heap))) == vector_original[MAX - 1]);
	ok = true;
	for (int i = MAX - 1; i >= 0; i--)
		ok = ok && (*((int*)heap_desencolar(heap)) == vector_original[i]);
	print_test(prueba, "elementos desencolados", ok);
	print_test(prueba, "cantidad heap 0", heap_cantidad(heap) == 0);
	print_test(prueba, "heap vacio", heap_esta_vacio(heap));
	heap_destruir(heap, NULL);
	printf("El heap fue destruido\n");
}

void prueba_heap_destruccion() {
	char prueba[MAX_CHAR] = "Heap destruccion elementos";
	printf("\nPrueba %s\n", prueba);
	heap_t *heap = heap_crear(comparar);
	size_t tamanio = 10;
	int *vector[tamanio],i;
	for (i = 0; i < tamanio; i++) {
		int *numero = malloc(sizeof(int));
		*numero = i;
		vector[i] = numero;
	}
	for (i = 0; i < tamanio; i++) {
		size_t pos = rand() % tamanio;
		int *aux = vector[pos];
		vector[pos] = vector[i];
		vector[i] = aux;
	}
	bool ok = true;
	for (i = 0; i < tamanio; i++){
		ok = ok && heap_encolar(heap, vector[i]);
	}
	print_test(prueba, "elementos encolados", ok);
	print_test(prueba, "cantidad heap 10", heap_cantidad(heap) == tamanio);
	print_test(prueba, "heap no vacio", !heap_esta_vacio(heap));
	print_test(prueba, "heap ver maximo 9", (*((int*)heap_ver_max(heap)) == tamanio - 1));
	int *max = heap_desencolar(heap);
	print_test(prueba, "heap obtenido el maximo", *max == tamanio - 1);
	free(max);
	print_test(prueba, "heap ver maximo 8", (*((int*)heap_ver_max(heap)) == tamanio - 2));
	heap_destruir(heap, free);
	printf("El heap fue destruido\n");
}

void prueba_heap_heapificar(){
	char prueba[MAX_CHAR] = "Heap heapificar";
	printf("\nPrueba %s\n", prueba);
	heap_t *heap = heap_crear(comparar);
	size_t tam = 100, i;
	int vector[tam];
	int vector_original[tam];
	inicializar_vector(vector, vector_original,tam);
	bool ok = true;

	for (i = 0; i < tam; i++)
		ok = ok && heap_encolar(heap, vector + i);
	print_test(prueba, "heap ver maximo 99", (*((int*)heap_ver_max(heap)) == (tam - 1)));
	vector_desordenar(vector, tam);
	print_test(prueba, "vector desordenado, maximo != 99", (*((int*)heap_ver_max(heap)) != (tam - 1)));
	//Reestablezco el orden con heapify
	heap_heapify(heap);
	print_test(prueba, "heap ver maximo 99", (*((int*)heap_ver_max(heap)) == (tam - 1)));
	ok = true;
	for (int i = tam - 1; i >= 0; i--)
		ok = ok && (*((int*)heap_desencolar(heap)) == i);
	print_test(prueba, "elementos desencolados", ok);
	print_test(prueba, "cantidad heap 0", heap_cantidad(heap) == 0);
	print_test(prueba, "heap vacio", heap_esta_vacio(heap));
	heap_destruir(heap, NULL);
	printf("El heap fue destruido\n");
}

void prueba_heap_sort(){
	char prueba[MAX_CHAR] = "Heap heapsort";
	printf("\nPrueba %s\n", prueba);
	size_t tam = 1000;
	int *vector[tam],i;
	for (i = 0; i < tam; i++) {
		int *num = malloc(sizeof(int));
		*num = i;
		vector[i] = num;
	}
	for (i = 0; i < tam; i++) {
		size_t pos = rand() % tam;
		int *aux = vector[pos];
		vector[pos] = vector[i];
		vector[i] = aux;
	}
	heap_sort((void**)vector, tam, comparar);
	bool ok = true;
	for (i = 0; i < tam; i++) {
		ok = ok && (*(vector[i]) == i);
		free(vector[i]);
	}
	print_test(prueba, "vector ordenado mediante heapsort", ok);
}


int main(void) {
	// Obtiene la fecha y hora actual desde 1970, en segundos.
    time_t t = time(NULL);
    // Inicializa el generador de números aleatorios.
	srand(t);
	prueba_heap_vacio();
	prueba_heap_elementos();
	prueba_heap_volumen();
	prueba_heap_destruccion();
	prueba_heap_heapificar();
	prueba_heap_sort();
	return 0;
}

