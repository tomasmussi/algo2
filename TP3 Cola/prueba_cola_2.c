#include <stdio.h>
#include <stdlib.h>
#include "cola.h"

#define MAX 100000


/***************************** PRUEBAS **********************************/
/* Prueba la cola vacia */
void prueba_vacio();

/* Prueba la cola con un elemento */
void prueba_un_elemento();

/* Prueba la cola con un par de elementos */
void prueba_elementos_comun();

/* Prueba que la cola funcione bien en el caso de tener un solo elemento */
void prueba_borde();

/* Prueba que la cola se elimine con elementos aun adentro */
void prueba_cola_con_elementos();

/* Prueba con una cantidad de elementos considerable */
void prueba_muchos_elementos();

/* Prueba que se pueda liberar memoria alocada desde el main */
void prueba_puntero_funcion();

void prueba_puntero_funcion_compleja();

/***************************** FIN PRUEBAS **********************************/

typedef struct _punto{
	int x,y;
	int *puntero;
} punto_t;

punto* crear_punto(int x, int y)
{
	punto_t *punto = malloc(sizeof(punto_t));
	if (punto == NULL)
		return;
	int *puntero = malloc(sizeof(int));
	if(puntero == NULL)
	{
		free(punto);
		return NULL;
	}
	punto->x = x;
	punto->y = y;
	*puntero = 10;
	punto->puntero = puntero;
	return punto;
}

void destruir_punto(void *punto)
{
	punto_t *n = (punto_t*) punto;
	free(n->puntero);
	free(n);
}

/* Implementacion de una funcion que debe liberar memoria alocada previamente */
void destruir_puntero(void* dato)
{
	free(dato);
}

/* Separa las impresiones por stdout de las pruebas */
void separar();

/* No necesita presentacion */
void print_test(char *mensaje, bool estado);

int main(void)
{
	prueba_vacio();
	separar();
	prueba_un_elemento();
	separar();
	prueba_elementos_comun();
	separar();
	prueba_borde();
	separar();
	prueba_cola_con_elementos();
	separar();
	prueba_muchos_elementos();
	separar();
	prueba_puntero_funcion();
	separar();
	prueba_puntero_funcion_compleja();
	return 0;
}

void print_test(char *mensaje, bool estado)
{
	printf("%s: %s\n",mensaje, estado ? "OK" : "ERROR");
}

void separar()
{
	printf("\n");
}


void prueba_vacio()
{
	printf("Prueba cola vacia\n");
	cola_t *cola = cola_crear();
	print_test("Cola vacia", cola_esta_vacia(cola));
	print_test("Cola ver primero NULL", cola_ver_primero(cola) == NULL);
	print_test("Cola desencolar NULL", cola_desencolar(cola) == NULL);
	cola_destruir(cola, NULL);
	printf("La cola fue destruida\n");
}

void prueba_un_elemento()
{
	printf("Prueba cola un elemento\n");
	cola_t *cola = cola_crear();
	int numero = 22;
	print_test("Encolando numero 22",cola_encolar(cola, &numero));
	print_test("Cola no vacia", !cola_esta_vacia(cola));
	int *puntero = cola_ver_primero(cola);
	print_test("Ver primero", *puntero == numero);
	print_test("Sigue con elemento la cola", !cola_esta_vacia(cola));
	puntero = cola_desencolar(cola);
	print_test("Numero desencolado", *puntero == numero);
	print_test("Cola vacia", cola_esta_vacia(cola));
	cola_destruir(cola, NULL);	
	printf("La cola fue destruida\n");
}

void prueba_elementos_comun()
{
	printf("Prueba elementos comun\n");
	cola_t *cola = cola_crear();

	int vector[10];
	bool encolado = true;
	for (int i = 0; i < 10; i++)
	{
		vector[i] = i;
		encolado &= cola_encolar(cola, vector + i);
	}
	print_test("Elementos encolados", encolado);
	print_test("Cola no vacia", !cola_esta_vacia(cola));
	print_test("Cola ver primero", cola_ver_primero(cola) == vector);
	print_test("Cola ver primero bis", *((int*)cola_ver_primero(cola)) == 0);
	int *puntero;
	encolado = true;
	for (int i = 0; i < 10; i++)
	{
		puntero = cola_desencolar(cola);
		encolado = encolado && (*puntero == i);
	}
	print_test("Desencolado", encolado);
	cola_destruir(cola, NULL);
	printf("La cola fue destruida\n");
}

void prueba_borde()
{
	printf("Prueba borde\n");
	cola_t *cola = cola_crear();
	print_test("Cola vacia", cola_esta_vacia(cola));
	print_test("Ver primero NULL", cola_ver_primero(cola) == NULL);
	print_test("Desencolar NULL", cola_desencolar(cola) == NULL);
	int num1 = 22, num2 = 208;
	print_test("Encolado num1", cola_encolar(cola, &num1));
	print_test("Cola no vacia", !cola_esta_vacia(cola));
	print_test("Ver num1", *((int*)cola_ver_primero(cola)) == num1);
	print_test("Encolado num2", cola_encolar(cola, &num2));
	print_test("Ver num1",*((int*)cola_ver_primero(cola)) == num1);
	print_test("Desencolar num1", *((int*)cola_desencolar(cola)) == num1);
	print_test("Cola no vacia", !cola_esta_vacia(cola));
	print_test("Ver num2", *((int*)cola_ver_primero(cola)) == num2);
	print_test("Desencolar num2", *((int*)cola_desencolar(cola)) == num2);
	cola_destruir(cola,NULL);
	printf("La cola fue destruida\n");
}

void prueba_cola_con_elementos()
{
	printf("Prueba elementos comun\n");
	cola_t *cola = cola_crear();

	int vector[10];
	bool encolado = true;
	for (int i = 0; i < 10; i++)
	{
		vector[i] = i;
		encolado &= cola_encolar(cola, vector + i);
	}
	print_test("Elementos encolados", encolado);
	cola_destruir(cola, NULL);
	printf("La cola fue destruida\n");
}

void prueba_muchos_elementos()
{
	printf("Prueba con muchos elementos\n");
	cola_t *cola = cola_crear();
	int vector[MAX], i;
	bool encolar = true;
	for(i = 0; i < MAX; i++)
	{
		vector[i] = i;
		encolar = encolar && cola_encolar(cola, vector + i);
	}
	print_test("Encolados", encolar);
	print_test("Ver tope", ((int*)cola_ver_primero(cola)) == vector);
	// Busco la mitad dependiendo si es numero par o impar
	int mitad = (MAX / 2) + ((MAX % 2) != 0 ? 1 : 0);
	encolar = true;
	// Itero hasta la mitad
	for (i = 0; i < mitad; i++)
	{
		encolar = encolar && ((*(int*)cola_desencolar(cola)) == i);
	}
	print_test("Viendo el elemento de la mitad", (int*)cola_ver_primero(cola) == (vector+i));
	print_test("Desencolado hasta la mitad", encolar);
	// Llego hasta el fin
	while (i < MAX)
	{
		encolar = encolar && ((*(int*)cola_desencolar(cola)) == i);
		i++;
	}
	print_test("Desencolado", encolar);
	cola_destruir(cola, NULL);
	printf("La cola fue destruida\n");
}

void prueba_puntero_funcion()
{	
	long *puntero = malloc(sizeof(long));
	if (puntero == NULL)
		return;
	cola_t *cola = cola_crear();
	*puntero = 22;
	print_test("Encolar el long en heap", cola_encolar(cola, puntero));
	print_test("Ver primero", ((long*) cola_ver_primero(cola)) == puntero);
	void (*destruir)(void*) = &destruir_puntero;
	cola_destruir(cola, destruir);
	printf("La cola fue destruida\n");
}

void prueba_puntero_funcion_compleja()
{	
	printf("Prueba funcion compleja\n");
	punto *punto = crear_punto(1,2);
	if (punto == NULL)
		return;
	cola_t *cola = cola_crear();
	if (cola == NULL)
		return;
	void (*destruir)(*punto) = destruir_punto;
	cola_destruir(cola, destruir);
	printf("La cola fue destruida\n");
}
