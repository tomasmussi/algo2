#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

#define MAX 200000



/************* Pruebas ******************/
/* Prueba la lista sin elementos */
void prueba_lista_vacia();

/* Prueba que la lista con un elemento no falle Al final probe con 2 elementos */
void prueba_un_elemento();

/* Prueba inserción y extracción de elementos */
void prueba_algunos_elementos();

/* Prueba inserción y extracción de muchos elementos */
void prueba_cantidad();

/* Prueba de que el puntero a funcion destruye bien los elementos */
void destruccion_datos();

/* Prueba el iterador interno */
void prueba_it_interno();

/* Pruebas para el iterador externo */
void prueba_it_externo_vacio();
void prueba_it_un_valor();
void prueba_it_varios();
void prueba_insertar_y_borrar();
void prueba_it_externo();

/************** Fin pruebas ************/

void print_test(char *mensaje, bool estado)
{
	printf("%s: %s\n", mensaje , estado ? "OK" : "ERROR");
}

/****** Separa las impresiones por standard out ********/
void separar()
{
	printf("\n");
}


void prueba_lista_vacia()
{
	lista_t *lista = lista_crear();
	print_test("Lista vacia", lista_esta_vacia(lista));
	print_test("Ver primero", lista_ver_primero(lista) == NULL);
	print_test("Borrar primero", lista_borrar_primero(lista) == NULL);
	print_test("Largo es 0", lista_largo(lista) == 0);
	lista_destruir(lista, NULL);
	printf("La lista fue destruida\n");
}

void prueba_un_elemento()
{
	lista_t *lista = lista_crear();
	int numero = 22;
	print_test("Insertar", lista_insertar_primero(lista, &numero));
	print_test("Lista no vacia", !lista_esta_vacia(lista));
	int *puntero = lista_ver_primero(lista);
	print_test("Valor", *puntero == numero);
	print_test("Largo", lista_largo(lista) == 1);
	puntero = lista_borrar_primero(lista);
	print_test("Elemento obtenido del borrado", *puntero == numero);
	print_test("Lista vacia", lista_esta_vacia(lista));
	print_test("Largo 0", lista_largo(lista) == 0);
	print_test("Borrar", lista_borrar_primero(lista) == NULL);
	int numero_2 = 25;
	print_test("Insertar", lista_insertar_primero(lista, &numero));
	print_test("Insertar", lista_insertar_ultimo(lista, &numero_2));
	print_test("Primero", *((int*)lista_ver_primero(lista)) == numero);
	print_test("Primero borrado", *((int*)lista_borrar_primero(lista)) == numero);
	print_test("Largo 1", lista_largo(lista) == 1);
	lista_destruir(lista, NULL);
	printf("La lista fue destruida\n");
}

void prueba_algunos_elementos()
{
	lista_t *lista = lista_crear();
	//Creo un vector de 5 elementos para jugar con la lista
	int v[] = {0,1,2,3,4};
	// Inserto intercalando y dejo la lista ordenada
	print_test("Insertar", lista_insertar_primero(lista, v + 2));
	print_test("Insertar", lista_insertar_ultimo(lista, v + 3));
	print_test("Insertar", lista_insertar_primero(lista, v + 1));
	print_test("Insertar", lista_insertar_ultimo(lista, v + 4));
	print_test("Insertar", lista_insertar_primero(lista, v));
	print_test("El primero es el 0", *((int*)lista_ver_primero(lista)) == 0);
	print_test("Lista no vacia", !lista_esta_vacia(lista));
	bool estado = true;
	//Meti los elementos ordenados en la lista
	for (int i = 0; i < 5; i++)
	{
		int *puntero;
		puntero = lista_borrar_primero(lista);
		estado &= (*puntero == i);
	}
	print_test("Lista bien usada", estado);
	print_test("Largo 0", lista_largo(lista) == 0);
	int i,j;
	for (i = 0; i < 5; i++)
	{
		print_test("Insertar",lista_insertar_ultimo(lista, v + i));
	}
	estado = true;
	bool cantidad = true;
	for (i = 0, j = 4; i < 5 && j >= 0; i++, j--)
	{
		cantidad &= (j+1 == lista_largo(lista));
		estado &= (*((int*)lista_borrar_primero(lista)) == v[i]);
	}
	print_test("Borrado de lista", estado);
	print_test("Cantidades", cantidad);
	lista_destruir(lista, NULL);
	printf("La lista fue destruida\n");
}

void prueba_cantidad()
{
	lista_t *lista = lista_crear();
	long i, v[MAX];
	bool estado = true;
	for (i = 0; i < MAX; i++)
	{
		v[i] = i;
		estado &= lista_insertar_ultimo(lista, v + i);
	}
	print_test("Insertados los elementos", estado);
	print_test("Cantidad MAX", lista_largo(lista) == MAX);
	estado = true;
	for (i = 0; i < MAX; i++)
	{
		estado &= (*((int*) lista_borrar_primero(lista)) == i);
	}
	print_test("Elementos borrados", estado);
	estado = true;
	for (i = 0; i < MAX; i++)
	{
		estado &= lista_insertar_primero(lista, v+i);
	}
	print_test("Insertados de vuelta los elementos", estado);
	lista_destruir(lista, NULL);
	printf("La lista fue destruida\n");
}


void destruccion_datos()
{
	lista_t *lista = lista_crear();
	int **vector = malloc(5 * sizeof(int));
	bool estado = true;
	for (int i = 0; i < 5; i++)
	{
		int *puntero = malloc(sizeof(int));
		*puntero = i;
		estado &= lista_insertar_ultimo(lista, puntero);
	}
	print_test("Inserts",estado);
	lista_destruir(lista, free);
	free(vector);
	printf("La lista fue destruida\n");
}

bool visitar(void *dato, void *extra)
{
	char *dato_real = (char*) dato;
	int *cuenta = (int*) extra;
	printf("Iterando %d: %s\n", *cuenta, dato_real);
	(*cuenta)++;
	return true;
}


void prueba_it_interno()
{
	lista_t *lista = lista_crear();
	print_test("Insert",lista_insertar_ultimo(lista, "Primer elemento"));
	print_test("Insert",lista_insertar_ultimo(lista, "Segundo elemento"));
	print_test("Insert",lista_insertar_ultimo(lista, "Tercer elemento"));
	print_test("Insert",lista_insertar_ultimo(lista, "Cuarto elemento"));
	int cuenta = 1;
	lista_iterar(lista, visitar, ((void*)&cuenta));
	lista_destruir(lista, NULL);
	printf("La lista fue destruida\n");
}

void prueba_it_externo_vacio()
{
	lista_t *lista = lista_crear();
	lista_iter_t *iter = lista_iter_crear(lista);
	print_test("Iterador al final", lista_iter_al_final(iter));
	print_test("Iterador avanzar", !lista_iter_avanzar(iter));
	print_test("Iterador ver dato", lista_iter_ver_actual(iter) == NULL);
	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
	printf("La lista y el iterador fueron destruidos\n");
}

void prueba_it_un_valor()
{
	lista_t *lista = lista_crear();
	lista_iter_t *iter = lista_iter_crear(lista);
	int i = 0;
	lista_insertar(lista, iter, &i);
	print_test("Largo 1", lista_largo(lista) == 1);
	print_test("Lista ver primero", ((int*)lista_ver_primero(lista)) == &i);
	print_test("Iterador no esta al final", !lista_iter_al_final(iter));
	print_test("Avanzo", lista_iter_avanzar(iter));
	print_test("Ahora estoy al final", lista_iter_al_final(iter));
	lista_iter_destruir(iter);
	lista_iter_t *nuevo_iter = lista_iter_crear(lista);
	print_test("Iterador nuevo, en el primer elemento", !lista_iter_al_final(nuevo_iter));
	int *puntero = lista_iter_ver_actual(nuevo_iter);
	print_test("Veo el numero actual", *puntero == i);
	lista_iter_destruir(nuevo_iter);
	lista_destruir(lista, NULL);
	printf("La lista y el iterador fueron destruidos\n");
}

void prueba_it_varios()
{
	lista_t *lista = lista_crear();
	int v[MAX];
	bool ok = true;
	for (int i = 0; i < MAX; i++)
	{
		v[i] = i;
		ok &= lista_insertar_ultimo(lista, v + i);
	}
	print_test("Insertados varios elementos", ok);
	lista_iter_t *iter = lista_iter_crear(lista);

	int i = 0;
	ok = true;
	while (!lista_iter_al_final(iter))
	{
		int *puntero = lista_iter_ver_actual(iter);
		ok &= (*puntero == i);
		i++;
		lista_iter_avanzar(iter);
	}
	print_test("Iterados MAX elementos", ok);
	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
	printf("La lista y el iterador fueron destruidos\n");
}

void prueba_insertar_y_borrar_vacio()
{
	lista_t *lista = lista_crear();
	lista_iter_t *iter = lista_iter_crear(lista);
	int v[] = {0,1};
	print_test("Largo de lista 0", lista_largo(lista) == 0);
	print_test("Insertado 0", lista_insertar(lista, iter, v));
	print_test("Largo de lista 1", lista_largo(lista) == 1);
	print_test("Actual 0", (int*)lista_iter_ver_actual(iter) == v);
	int *puntero = lista_borrar(lista, iter);
	print_test("Borrado 0", *puntero == v[0]);
	print_test("Largo de lista 0", lista_largo(lista) == 0);
	print_test("Insertado 1",lista_insertar(lista, iter, v + 1));
	print_test("Largo de lista 1", lista_largo(lista) == 1);
	print_test("Actual 1", (int*)lista_iter_ver_actual(iter) == (v + 1));
	puntero = lista_borrar(lista, iter);
	print_test("Borrado 1", *puntero == v[1]);

	print_test("Iterador al final", lista_iter_al_final(iter));
	print_test("Insertado 0", lista_insertar(lista, iter, v));
	print_test("Iterador no al final", !lista_iter_al_final(iter));
	print_test("Actual 0", (int*)lista_iter_ver_actual(iter) == v);
	print_test("Insertado 1",lista_insertar(lista, iter, v + 1));
	print_test("Actual 1", (int*)lista_iter_ver_actual(iter) == (v + 1));
	print_test("Largo de lista 2", lista_largo(lista) == 2);
	puntero = lista_borrar(lista, iter);
	print_test("Borrado 1", *puntero == v[1]);
	print_test("Largo de lista 1", lista_largo(lista) == 1);
	print_test("Actual 0", (int*)lista_iter_ver_actual(iter) == v);
	puntero = lista_borrar(lista, iter);
	print_test("Borrado 0", *puntero == v[0]);
	print_test("Largo de lista 0", lista_largo(lista) == 0);

	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
	printf("La lista y el iterador fueron destruidos\n");
}

void prueba_insertar_y_borrar_elemento()
{
	lista_t *lista = lista_crear();
	int v[] = {0,1,2,3,4};
	print_test("Inserto un elemento", lista_insertar_ultimo(lista, v));
	print_test("Inserto un elemento", lista_insertar_ultimo(lista, v + 2));
	lista_iter_t *iter = lista_iter_crear(lista);
	// Voy a iterar e insertar (v+1) antes de (v+2), asi queda en orden
	bool encontrado = false;
	while(!lista_iter_al_final(iter))
	{
		int *puntero = lista_iter_ver_actual(iter);
		if ((*puntero == v[2]) && !encontrado)
		{
			lista_insertar(lista,iter, v+1);
			print_test("El actual debe ser el 1", ((int*)lista_iter_ver_actual(iter)) == v+1);
			encontrado = true;
		}
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);

	lista_iter_t *iter_2 = lista_iter_crear(lista);
	bool ok = true;
	int i = 0;
	while (!lista_iter_al_final(iter_2))
	{
		int *puntero = lista_iter_ver_actual(iter_2);
		ok &= (*puntero == i);
		i++;
		lista_iter_avanzar(iter_2);
	}
	print_test("La lista quedo bien", ok);
	lista_iter_destruir(iter_2);

	lista_destruir(lista, NULL);
	printf("La lista y el iterador fueron destruidos\n");
}

void prueba_insertar_y_borrar_varios()
{
}

void prueba_iterar()
{
	printf("\nPrueba iterador vacio\n");
	prueba_it_externo_vacio();
	printf("\nPrueba iterador un valor\n");
	prueba_it_un_valor();
	printf("\nPrueba iterador varios\n");
	prueba_it_varios();
}

void prueba_insertar_borrar()
{
	printf("\nPrueba iterador insertar y borrar vacio\n");
	prueba_insertar_y_borrar_vacio();
	printf("\nPrueba iterador insertar y borrar pocos elementos\n");
	prueba_insertar_y_borrar_elemento();
	printf("\nPrueba iterador insertar y borrar varios\n");
	//prueba_insertar_y_borrar_varios();
}

void prueba_it_externo()
{
	prueba_iterar();
	prueba_insertar_borrar();
}

int main(void)
{
	printf("Prueba lista vacia\n");
	prueba_lista_vacia();
	printf("\nPrueba un elemento\n");
	prueba_un_elemento();
	printf("\nPrueba algunos elementos\n");
	prueba_algunos_elementos();
	printf("\nPrueba cantidad\n");
	prueba_cantidad();
	printf("\nDestruir datos\n");
 	destruccion_datos();
	printf("\nPrueba iterador interno\n");
	prueba_it_interno();
	printf("\nPrueba iterador externo\n");
	prueba_it_externo();
	return 0;
}
