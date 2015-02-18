#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "abb.h"

#define MAX 10000

#define MAX_CHAR 50


void print_test(char* prueba, char* name, bool result)
{
	printf("%s, %s: %s\n",prueba, name, result ? "OK" : "ERROR");
}

// Esta función analiza el comportamiento de un arbol vacio.
void prueba_arbol_vacio();

// Esta función analiza el comportamiento de un arbol con un elemento.
// Prueba la inserción, obtencion y borrado del dato.
void prueba_arbol_un_elemento();

// Esta función analiza el comportamiento de un arbol con algunos elementos.
// Prueba la inserción, obtencion y borrado de múltiples datos.
void prueba_arbol_algunos_elementos();

// Es una función que probará un caso particular de borrado.
// Se insertarán algunos elementos y se borrará la raiz buscando que el arbol quede
// completo y no se pierda ningun dato, ni deje de ser un ABB.
void prueba_borrado_raiz();

// Es una prueba de volumen del arbol.
// Se insertarán muchos datos y se analizará que la estructura lo pueda manejar sin problema.
void prueba_arbol_muchos_elementos();

// Esta función analiza el comportamiento de un iterador de un arbol vacío.
void prueba_iterar_arbol_vacio();

// Esta función analiza el comportamiento de un iterador de un arbol con un elemento.
void prueba_iterar_un_elemento();

// Esta función analiza el comportamiento de un iterador de un arbol con algunos elementos.
void prueba_iterar_algunos_elementos();

// Esta función analiza el comportamiento de un iterador de un arbol con muchos elementos
void prueba_iterar_muchos_elementos();

// Esta función analiza el iterador interno y comprara que los resultados sean los esperados.
void prueba_iterador_interno();

// Esta función insertará datos y borrara solo algunos, para probar que la destrucción del arbol
// también destruya los datos restantes.
void prueba_arbol_destruccion_datos();



void prueba_arbol_vacio() {
	char prueba[MAX_CHAR] = "Arbol vacio";
	printf("\nPrueba %s\n", prueba);
	abb_t *arbol = abb_crear(NULL, NULL);
	char clave[10] = "clave";
	print_test(prueba, "borrar clave no existente", abb_borrar(arbol, clave) == NULL);
	print_test(prueba, "obtener clave no existente", abb_obtener(arbol, clave) == NULL);
	print_test(prueba, "clave no pertenece", !abb_pertenece(arbol, clave));
	print_test(prueba, "cantidad 0", abb_cantidad(arbol) == 0);
	abb_destruir(arbol);
	printf("El arbol fue destruido\n");
}

void prueba_arbol_un_elemento() {
	char prueba[MAX_CHAR] = "Arbol un elemento";
	printf("\nPrueba %s\n", prueba);
	abb_t *arbol = abb_crear(strcmp, NULL);
	char clave[15] = "Numero";
	int valor1 = 1;
	int valor2 = 2;
	print_test(prueba, "clave obtener NULL", abb_obtener(arbol, clave) == NULL);
	print_test(prueba, "clave no pertenece", !abb_pertenece(arbol, clave));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, clave, &valor1));
	print_test(prueba, "obtener clave", abb_obtener(arbol, clave) == &valor1);
	print_test(prueba, "clave pertenece", abb_pertenece(arbol, clave));
	print_test(prueba, "cantidad 1", abb_cantidad(arbol) == 1);

	print_test(prueba, "guardar nueva clave", abb_guardar(arbol,clave, &valor2));
	print_test(prueba, "cantidad 1", abb_cantidad(arbol) == 1);
	print_test(prueba, "obtener clave", abb_obtener(arbol, clave) == &valor2);
	print_test(prueba, "clave pertenece", abb_pertenece(arbol, clave));


	print_test(prueba, "borrar clave", *((int*)abb_borrar(arbol, clave)) == valor2);
	print_test(prueba, "clave obtener NULL", abb_obtener(arbol, clave) == NULL);
	print_test(prueba, "clave no pertenece", !abb_pertenece(arbol, clave));
	print_test(prueba, "cantidad 0", abb_cantidad(arbol) == 0);
	// Agrego la raiz de vuelta al arbol
	print_test(prueba, "guardar raiz otra vez", abb_guardar(arbol,clave, &valor1));
	print_test(prueba, "cantidad 1", abb_cantidad(arbol) == 1);
	print_test(prueba, "obtener clave", abb_obtener(arbol, clave) == &valor1);
	print_test(prueba, "clave pertenece", abb_pertenece(arbol, clave));
	// La borro otra vez
	print_test(prueba, "borrar clave", *((int*)abb_borrar(arbol, clave)) == valor1);
	print_test(prueba, "clave obtener NULL", abb_obtener(arbol, clave) == NULL);
	print_test(prueba, "clave no pertenece", !abb_pertenece(arbol, clave));
	print_test(prueba, "cantidad 0", abb_cantidad(arbol) == 0);

	abb_destruir(arbol);
	printf("El arbol fue destruido\n");
}

void prueba_arbol_algunos_elementos() {
	char prueba[MAX_CHAR] = "Arbol algunos elementos";
	printf("\nPrueba %s\n", prueba);
	abb_t *arbol = abb_crear(strcmp, NULL);
	char claves[][10] = {"0","1","2","3","4","5","6","7","8","9"};
	int valores[] = {0,1,2,3,4,5,6,7,8,9};
	print_test(prueba, "agregar 3 al arbol", abb_guardar(arbol, claves[3], valores + 3));
	print_test(prueba, "cantidad 1", abb_cantidad(arbol) == 1);
	print_test(prueba, "obtener clave 3", abb_obtener(arbol, claves[3]) == (valores + 3));
	print_test(prueba, "obtener clave 6", abb_obtener(arbol, claves[6]) == NULL);
	print_test(prueba, "agregar 4 al arbol", abb_guardar(arbol, claves[4], valores + 4));
	print_test(prueba, "agregar 1 al arbol", abb_guardar(arbol, claves[1], valores + 1));
	print_test(prueba, "agregar 0 arbol", abb_guardar(arbol, claves[0], valores + 0));
	print_test(prueba, "agregar 2 al arbol", abb_guardar(arbol, claves[2], valores + 2));

	// Despues de insertar un par, verifico
	print_test(prueba, "cantidad 5", abb_cantidad(arbol) == 5);
	print_test(prueba, "obtener clave 3", abb_obtener(arbol, claves[3]) == (valores + 3));
	print_test(prueba, "obtener clave 4", abb_obtener(arbol, claves[4]) == (valores + 4));
	print_test(prueba, "clave 1 pertenece", abb_pertenece(arbol, claves[1]));
	print_test(prueba, "clave 0 pertenece", abb_pertenece(arbol, claves[0]));
	print_test(prueba, "borrar clave 0", abb_borrar(arbol, claves[0]) == (valores + 0));
	print_test(prueba, "cantidad 4", abb_cantidad(arbol) == 4);
	print_test(prueba, "obtener clave 0 NULL", abb_obtener(arbol, claves[0]) == NULL);
	print_test(prueba, "clave 0 no pertenece", !abb_pertenece(arbol, claves[0]));
	print_test(prueba, "agregar 0 arbol", abb_guardar(arbol, claves[0], valores + 0));
	print_test(prueba, "clave 1 pertenece", abb_pertenece(arbol, claves[1]));
	print_test(prueba, "agregar 6 al arbol", abb_guardar(arbol, claves[6], valores + 6));
	print_test(prueba, "agregar 5 al arbol", abb_guardar(arbol, claves[5], valores + 5));
	print_test(prueba, "agregar 9 al arbol", abb_guardar(arbol, claves[9], valores + 9));
	print_test(prueba, "agregar 7 al arbol", abb_guardar(arbol, claves[7], valores + 7));
	print_test(prueba, "agregar 8 al arbol", abb_guardar(arbol, claves[8], valores + 8));
	print_test(prueba, "agregar 8 al arbol", abb_guardar(arbol, claves[8], valores + 8));
	print_test(prueba, "cantidad 10", abb_cantidad(arbol) == 10);
	// Prueba de los 3 casos de borrado
	//Caso 1
	print_test(prueba, "borrar clave 0", abb_borrar(arbol, claves[0]) == (valores + 0));
	print_test(prueba, "cantidad 9", abb_cantidad(arbol) == 9);
	print_test(prueba, "obtener clave 0", abb_obtener(arbol, claves[0]) == NULL);
	print_test(prueba, "clave 0 no pertenece", !abb_pertenece(arbol, claves[0]));
	print_test(prueba, "obtener clave 1", abb_obtener(arbol, claves[1]) == valores + 1);
	print_test(prueba, "clave 1 pertenece", abb_pertenece(arbol, claves[1]));
	print_test(prueba, "obtener clave 2", abb_obtener(arbol, claves[2]) == valores + 2);
	print_test(prueba, "clave 2 pertenece", abb_pertenece(arbol, claves[2]));
	//Caso 2
	print_test(prueba, "borrar clave 1", abb_borrar(arbol, claves[1]) == (valores + 1));
	print_test(prueba, "cantidad 8", abb_cantidad(arbol) == 8);
	print_test(prueba, "obtener clave 1", abb_obtener(arbol, claves[1]) == NULL);
	print_test(prueba, "clave 1 no pertenece", !abb_pertenece(arbol, claves[1]));
	print_test(prueba, "obtener clave 3", abb_obtener(arbol, claves[3]) == valores + 3);
	print_test(prueba, "clave 3 pertenece", abb_pertenece(arbol, claves[3]));
	print_test(prueba, "obtener clave 2", abb_obtener(arbol, claves[2]) == valores + 2);
	print_test(prueba, "clave 2 pertenece", abb_pertenece(arbol, claves[2]));
	//Caso 3
	print_test(prueba, "borrar clave 6", abb_borrar(arbol, claves[6]) == (valores + 6));
	print_test(prueba, "cantidad 7", abb_cantidad(arbol) == 7);
	print_test(prueba, "obtener clave 6", abb_obtener(arbol, claves[6]) == NULL);
	print_test(prueba, "clave 6 no pertenece", !abb_pertenece(arbol, claves[6]));
	print_test(prueba, "obtener clave 8", abb_obtener(arbol, claves[8]) == valores + 8);
	print_test(prueba, "clave 8 pertenece", abb_pertenece(arbol, claves[8]));
	print_test(prueba, "obtener clave 7", abb_obtener(arbol, claves[7]) == valores + 7);
	print_test(prueba, "clave 7 pertenece", abb_pertenece(arbol, claves[7]));
	print_test(prueba, "obtener clave 9", abb_obtener(arbol, claves[9]) == valores + 9);
	print_test(prueba, "clave 9 pertenece", abb_pertenece(arbol, claves[9]));
	print_test(prueba, "obtener clave 5", abb_obtener(arbol, claves[5]) == valores + 5);
	print_test(prueba, "clave 5 pertenece", abb_pertenece(arbol, claves[5]));
	print_test(prueba, "obtener clave 4", abb_obtener(arbol, claves[4]) == valores + 4);
	print_test(prueba, "clave 4 pertenece", abb_pertenece(arbol, claves[4]));
	print_test(prueba, "obtener clave 3", abb_obtener(arbol, claves[3]) == valores + 3);
	print_test(prueba, "clave 3 pertenece", abb_pertenece(arbol, claves[3]));

	abb_destruir(arbol);
	printf("El arbol fue destruido\n");
}

void prueba_borrado_raiz() {
	char prueba[MAX_CHAR] = "Arbol raiz";
	printf("\nPrueba %s\n", prueba);
	abb_t *arbol = abb_crear(strcmp, NULL);
	char claves[][10] = {"0","1","2","3","4"};
	int valores[] = {0,1,2,3,4};

	print_test(prueba, "borrar clave 1 no existente", abb_borrar(arbol, claves[1]) == NULL);
	print_test(prueba, "cantidad 0", abb_cantidad(arbol) == 0);
	print_test(prueba, "agregar clave 1 al arbol", abb_guardar(arbol, claves[1], valores + 1));
	print_test(prueba, "obtener clave 1", abb_obtener(arbol, claves[1]) == valores + 1);
	print_test(prueba, "clave 1 pertenece", abb_pertenece(arbol, claves[1]));
	//Caso 1: raiz sin nodos hijos
	print_test(prueba, "borrar clave 1 existente", abb_borrar(arbol, claves[1]) == valores + 1);
	print_test(prueba, "obtener clave 1 NULL", abb_obtener(arbol, claves[1]) == NULL);
	print_test(prueba, "clave 1 no pertenece", !abb_pertenece(arbol, claves[1]));

	print_test(prueba, "agregar clave 1 al arbol", abb_guardar(arbol, claves[1], valores + 1));
	print_test(prueba, "obtener clave 1", abb_obtener(arbol, claves[1]) == valores + 1);
	print_test(prueba, "clave 1 pertenece", abb_pertenece(arbol, claves[1]));
	print_test(prueba, "agregar 0 al arbol", abb_guardar(arbol, claves[0], valores + 0));
	print_test(prueba, "obtener clave 0", abb_obtener(arbol, claves[0]) == valores + 0);
	print_test(prueba, "clave 0 pertenece", abb_pertenece(arbol, claves[0]));
	print_test(prueba, "cantidad 2", abb_cantidad(arbol) == 2);
	//Caso 2: raiz con 1 hijo
	print_test(prueba, "borrar clave 1", abb_borrar(arbol, claves[1]) == valores + 1);
	print_test(prueba, "cantidad 1", abb_cantidad(arbol) == 1);
	print_test(prueba, "obtener clave 1 NULL", abb_obtener(arbol, claves[1]) == NULL);
	print_test(prueba, "clave 1 no pertenece", !abb_pertenece(arbol, claves[1]));
	print_test(prueba, "obtener clave 0", abb_obtener(arbol, claves[0]) == valores + 0);
	print_test(prueba, "clave 0 pertenece", abb_pertenece(arbol, claves[0]));
	print_test(prueba, "borrar clave 0", abb_borrar(arbol, claves[0]) == valores + 0);
	print_test(prueba, "cantidad 0", abb_cantidad(arbol) == 0);
	//Arbol vacio, paso a probar:
	//Caso 3: raiz con 2 hijos
	print_test(prueba, "agregar 1 al arbol", abb_guardar(arbol, claves[1], valores + 1));
	print_test(prueba, "obtener clave 1", abb_obtener(arbol, claves[1]) == valores + 1);
	print_test(prueba, "clave 1 pertenece", abb_pertenece(arbol, claves[1]));
	print_test(prueba, "agregar 0 al arbol", abb_guardar(arbol, claves[0], valores + 0));
	print_test(prueba, "obtener clave 0", abb_obtener(arbol, claves[0]) == valores + 0);
	print_test(prueba, "clave 0 pertenece", abb_pertenece(arbol, claves[0]));
	print_test(prueba, "agregar 2 al arbol", abb_guardar(arbol, claves[2], valores + 2));
	print_test(prueba, "obtener clave 2", abb_obtener(arbol, claves[2]) == valores + 2);
	print_test(prueba, "clave 2 pertenece", abb_pertenece(arbol, claves[2]));
	print_test(prueba, "cantidad 3", abb_cantidad(arbol) == 3);
	//Borro la raiz
	print_test(prueba, "borrar clave 1", abb_borrar(arbol, claves[1]) == valores + 1);
	print_test(prueba, "cantidad 2", abb_cantidad(arbol) == 2);
	print_test(prueba, "obtener clave 1", abb_obtener(arbol, claves[1]) == NULL);
	print_test(prueba, "clave 1 pertenece", !abb_pertenece(arbol, claves[1]));
	print_test(prueba, "obtener clave 0", abb_obtener(arbol, claves[0]) == valores + 0);
	print_test(prueba, "clave 0 pertenece", abb_pertenece(arbol, claves[0]));
	print_test(prueba, "obtener clave 2", abb_obtener(arbol, claves[2]) == valores + 2);
	print_test(prueba, "clave 2 pertenece", abb_pertenece(arbol, claves[2]));
	print_test(prueba, "borrar clave 0", abb_borrar(arbol, claves[0]) == valores + 0);
	print_test(prueba, "borrar clave 2", abb_borrar(arbol, claves[2]) == valores + 2);

	//Ultima prueba, si no se pierde el hijo derecho del nodo a ser nueva raiz
	print_test(prueba, "cantidad 0", abb_cantidad(arbol) == 0);
	print_test(prueba, "agregar 1 al arbol", abb_guardar(arbol, claves[1], valores + 1));
	print_test(prueba, "agregar 0 al arbol", abb_guardar(arbol, claves[0], valores + 0));
	print_test(prueba, "agregar 4 al arbol", abb_guardar(arbol, claves[4], valores + 4));
	print_test(prueba, "agregar 2 al arbol", abb_guardar(arbol, claves[2], valores + 2));
	print_test(prueba, "agregar 3 al arbol", abb_guardar(arbol, claves[3], valores + 3));
	print_test(prueba, "borrar clave 1", abb_borrar(arbol, claves[1]) == valores + 1);
	print_test(prueba, "cantidad 4", abb_cantidad(arbol) == 4);
	print_test(prueba, "obtener clave 1 no existente", abb_obtener(arbol, claves[1]) == NULL);
	print_test(prueba, "clave 1 no pertenece", !abb_pertenece(arbol, claves[1]));
	print_test(prueba, "obtener clave 0", abb_obtener(arbol, claves[0]) == valores + 0);
	print_test(prueba, "obtener clave 2", abb_obtener(arbol, claves[2]) == valores + 2);
	print_test(prueba, "obtener clave 3", abb_obtener(arbol, claves[3]) == valores + 3);
	print_test(prueba, "obtener clave 4", abb_obtener(arbol, claves[4]) == valores + 4);
	abb_destruir(arbol);
	printf("El arbol fue destruido\n");

}

int comparar(const char *s1, const char *s2){
	return atoi(s1) - atoi(s2);
}

void prueba_arbol_muchos_elementos() {

	char prueba[MAX_CHAR] = "Arbol cantidad";
	printf("\nPrueba %s\n", prueba);
	char claves[MAX][6];
	int valores[MAX];
	//Creacion de string de claves y valores
	for (int i = 0; i < MAX; i++) {
		sprintf(claves[i], "%05d", i);
		valores[i] = i;
	}
	//Mezcla de los valores inicializados
	for (int i = 0; i < MAX; i++) {
		int pos = rand() % MAX;
		char aux[6];
		strcpy(aux, claves[pos]);
		int iaux = valores[pos];
		strcpy(claves[pos],claves[i]);
		valores[pos] = valores[i];
		strcpy(claves[i], aux);
		valores[i] = iaux;
	}
	//Creo el arbol
	abb_t *arbol = abb_crear(comparar, NULL);
	bool ok = true;
	for (int i = 0; i < MAX; i++) {
		ok = ok && abb_guardar(arbol, claves[i], valores + i);
	}
	print_test(prueba, "insertados todos los elementos", ok);
	print_test(prueba, "cantidad de elementos MAX", abb_cantidad(arbol) == MAX );

	//Pruebo que esten todas las claves y sus valores
	ok = true;
	for (int i = 0; i < MAX; i++) {
		ok = ok && abb_pertenece(arbol, claves[i]);
	}
	print_test(prueba, "las claves estan guardadas", ok);
	print_test(prueba, "cantidad de elementos MAX", abb_cantidad(arbol) == MAX );
	//Borro los elementos del arbol
	ok = true;
	for (int i = 0; i < MAX; i++) {
		int *valor = abb_borrar(arbol, claves[i]);
		ok = ok && (*valor == valores[i]);
	}
	print_test(prueba, "elementos borrados", ok);
	print_test(prueba, "cantidad de elementos MAX", abb_cantidad(arbol) == 0);
	//Elimino el arbol
	abb_destruir(arbol);
	printf("El arbol fue destruido\n");
}

void prueba_iterar_arbol_vacio() {
	char prueba[MAX_CHAR] = "Arbol iterador vacio";
	printf("\nPrueba %s\n", prueba);
	abb_t *arbol = abb_crear(strcmp, NULL);
	abb_iter_t *iter = abb_iter_in_crear(arbol);
	print_test(prueba, "iterador ver actual es null?", abb_iter_in_ver_actual(iter) == NULL);
	print_test(prueba, "iterador al final?", abb_iter_in_al_final(iter));
	print_test(prueba, "iterador avanzar, no puede avanzar.", !abb_iter_in_avanzar(iter));
	abb_iter_in_destruir(iter);
	abb_destruir(arbol);
	printf("El arbol y el iterador fueron destruidos\n");
}

void prueba_iterar_un_elemento() {
	char prueba[MAX_CHAR] = "Arbol iterador un elemento";
	printf("\nPrueba %s\n", prueba);
	abb_t *arbol = abb_crear(strcmp, NULL);
	char clave[10] = "Clave";
	int valor = 0;
	abb_guardar(arbol, clave, &valor);
	abb_iter_t *iter = abb_iter_in_crear(arbol);
	print_test(prueba, "ver actual", strcmp(((char*)abb_iter_in_ver_actual(iter)), clave) == 0);
	print_test(prueba, "iterador al final?", !abb_iter_in_al_final(iter));
	print_test(prueba, "iterador avanzar", abb_iter_in_avanzar(iter));
	//Avance y estoy al final
	print_test(prueba, "iterador ver actual", abb_iter_in_ver_actual(iter) == NULL);
	print_test(prueba, "iterador al final", abb_iter_in_al_final(iter));
	print_test(prueba, "iterador avanzar", !abb_iter_in_avanzar(iter));
	abb_iter_in_destruir(iter);
	abb_destruir(arbol);
	printf("El arbol y el iterador fueron destruidos\n");
}

void prueba_iterar_algunos_elementos() {
	char prueba[MAX_CHAR] = "Arbol iterar algunos elementos";
	printf("\nPrueba %s\n", prueba);
	abb_t *arbol = abb_crear(strcmp, NULL);
	char claves[][10] = {"0","1","2","3","4","5","6","7","8","9"};
	int valores[] = {0,1,2,3,4,5,6,7,8,9};
	print_test(prueba, "agregar 3 al arbol", abb_guardar(arbol, claves[3], valores + 3));
	print_test(prueba, "agregar 4 al arbol", abb_guardar(arbol, claves[4], valores + 4));
	print_test(prueba, "agregar 1 al arbol", abb_guardar(arbol, claves[1], valores + 1));
	print_test(prueba, "agregar 0 arbol", abb_guardar(arbol, claves[0], valores + 0));
	print_test(prueba, "agregar 2 al arbol", abb_guardar(arbol, claves[2], valores + 2));
	print_test(prueba, "agregar 0 arbol", abb_guardar(arbol, claves[0], valores + 0));
	print_test(prueba, "agregar 6 al arbol", abb_guardar(arbol, claves[6], valores + 6));
	print_test(prueba, "agregar 5 al arbol", abb_guardar(arbol, claves[5], valores + 5));
	print_test(prueba, "agregar 9 al arbol", abb_guardar(arbol, claves[9], valores + 9));
	print_test(prueba, "agregar 7 al arbol", abb_guardar(arbol, claves[7], valores + 7));
	print_test(prueba, "agregar 8 al arbol", abb_guardar(arbol, claves[8], valores + 8));
	print_test(prueba, "agregar 8 al arbol", abb_guardar(arbol, claves[8], valores + 8));
	print_test(prueba, "cantidad 10", abb_cantidad(arbol) == 10);
	abb_iter_t *iter = abb_iter_in_crear(arbol);
	int i = 0;
	bool ok = true;
	while (!abb_iter_in_al_final(iter)){
		const char *clave = abb_iter_in_ver_actual(iter);
		int *valor = abb_obtener(arbol, clave);
		ok = ok && (*valor == i);
		i++;
		abb_iter_in_avanzar(iter);
	}
	print_test(prueba, "iteracion de 10 elementos", ok);
	abb_iter_in_destruir(iter);
	abb_destruir(arbol);
	printf("El arbol y el iterador fueron destruidos\n");
}

void prueba_iterar_muchos_elementos() {
	char prueba[MAX_CHAR] = "Arbol iterar cantidad";
	printf("\nPrueba %s\n", prueba);
	char claves_original[MAX][6];
	char claves[MAX][6];
	int valores[MAX];
	//Creacion de string de claves y valores
	for (int i = 0; i < MAX; i++) {
		sprintf(claves_original[i], "%05d", i);
		sprintf(claves[i], "%05d", i);
		valores[i] = i;
	}
	//Mezcla de los valores inicializados
	for (int i = 0; i < MAX; i++) {
		int pos = rand() % MAX;
		char aux[6];
		strcpy(aux, claves[pos]);
		int iaux = valores[pos];
		strcpy(claves[pos],claves[i]);
		valores[pos] = valores[i];
		strcpy(claves[i], aux);
		valores[i] = iaux;
	}
	//Creo el arbol
	abb_t *arbol = abb_crear(comparar, NULL);
	bool ok = true;
	for (int i = 0; i < MAX; i++) {
		ok = ok && abb_guardar(arbol, claves[i], valores + i);
	}
	print_test(prueba, "insertados todos los elementos", ok);
	print_test(prueba, "cantidad de elementos MAX", abb_cantidad(arbol) == MAX );

	//Creo el iterador
	abb_iter_t *iter = abb_iter_in_crear(arbol);
	ok = true;
	int i = 0;
	while (!abb_iter_in_al_final(iter)){
		const char *clave = abb_iter_in_ver_actual(iter);
		ok = ok && (comparar(clave, claves_original[i]) == 0);
		abb_iter_in_avanzar(iter);
		i++;
	}
	print_test(prueba, "iterados todos los elementos", ok);
	abb_iter_in_destruir(iter);
	//Elimino el arbol
	abb_destruir(arbol);
	printf("El arbol fue destruido\n");
}


bool visitar(const char *clave, void *dato, void *extra) {
	int *elemento = (int*) extra;
	printf("Elemento %d: Clave=%s, Valor=%d\n", *elemento, clave, *((int*)dato));
	(*elemento)++;
	return true;
}

void prueba_iterador_interno() {
	char prueba[MAX_CHAR] = "Arbol iterador interno";
	printf("\nPrueba %s\n", prueba);
	abb_t *arbol = abb_crear(strcmp, NULL);
	char claves[][10] = {"0","1","2","3","4","5","6","7","8","9"};
	int valores[] = {0,1,2,3,4,5,6,7,8,9};
	/* Pruebo si anda con el arbol vacio o con un elemento */
	int i = 0;
	abb_in_order(arbol, visitar, (void*)&i);
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[5], valores + 5));
	abb_in_order(arbol, visitar, (void*)&i);
	print_test(prueba, "i con un elemento", i == 1);
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[3], valores + 3));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[8], valores + 8));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[1], valores + 1));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[4], valores + 4));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[7], valores + 7));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[9], valores + 9));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[0], valores + 0));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[2], valores + 2));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[6], valores + 6));
	i = 0;
	abb_in_order(arbol, visitar, (void*)&i);
	print_test(prueba, "i con 10 elementos", i == 10);
	abb_destruir(arbol);
	printf("El arbol fue destruido\n");
}

bool visitar_corte(const char *clave, void *dato, void *extra) {
	int *elemento = (int*) extra;
	printf("Elemento %d: Clave=%s, Valor=%d\n", *elemento, clave, *((int*)dato));
	(*elemento)++;
	return (*elemento) < 5;
}

void prueba_iterador_interno_corte() {
	char prueba[MAX_CHAR] = "Arbol iterador interno corte";
	printf("\nPrueba %s\n", prueba);
	abb_t *arbol = abb_crear(strcmp, NULL);
	char claves[][10] = {"0","1","2","3","4","5","6","7","8","9"};
	int valores[] = {0,1,2,3,4,5,6,7,8,9};
	/* Pruebo si anda con el arbol vacio o con un elemento */
	int i = 0;
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[5], valores + 5));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[3], valores + 3));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[8], valores + 8));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[1], valores + 1));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[4], valores + 4));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[7], valores + 7));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[9], valores + 9));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[0], valores + 0));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[2], valores + 2));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[6], valores + 6));
	i = 0;
	abb_in_order(arbol, visitar_corte, (void*)&i);
	print_test(prueba, "i con 5 elementos", i == 5);
	abb_destruir(arbol);
	printf("El arbol fue destruido\n");
}

void prueba_arbol_destruccion_datos() {
	char prueba[MAX_CHAR] = "Arbol destruccion de datos";
	printf("\nPrueba %s\n", prueba);
	char claves[][2] = {"0","1","2","3","4","5"};
	int* valores[5];
	for (int i = 0; i < 5; i++) {
		int *p = malloc(sizeof(int));
		*p = i;
		valores[i] = p;
	}
	abb_t *arbol = abb_crear(strcmp, free);
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[2], valores[2]));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[3], valores[3]));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[1], valores[1]));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[4], valores[4]));
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[0], valores[0]));
	print_test(prueba, "obtener clave 2", abb_obtener(arbol,claves[2]) == valores[2]);
	int *nuevo_valor = malloc(sizeof(int));
	*nuevo_valor = 30;
	//Reemplazo la clave 0
	print_test(prueba, "agregar un elemento al arbol", abb_guardar(arbol, claves[0], nuevo_valor));
	print_test(prueba, "obtener clave 0", abb_obtener(arbol,claves[0]) == nuevo_valor);
	int *a_borrar = abb_borrar(arbol, claves[0]);
	print_test(prueba, "borrada clave 0", nuevo_valor == a_borrar);
	free(a_borrar);
	abb_destruir(arbol);
	printf("El arbol fue destruido\n");
}


int main(void) {
 	prueba_arbol_vacio();
 	prueba_arbol_un_elemento();
 	prueba_arbol_algunos_elementos();
	prueba_borrado_raiz();
 	prueba_arbol_muchos_elementos();
 	prueba_iterar_arbol_vacio();
 	prueba_iterar_un_elemento();
	prueba_iterar_algunos_elementos();
 	prueba_iterar_muchos_elementos();
	prueba_iterador_interno();
	prueba_arbol_destruccion_datos();
	prueba_iterador_interno_corte();
	return 0;
}

