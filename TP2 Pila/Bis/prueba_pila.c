#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pila.h"
#define MAX_APILADO 100

//Funcion auxiliar para imprimir pruebas

void print_test(char* name, bool result);
void separar();

/*Verifica que se apilan distintas variables y que se guarden
 *Despues verifica que lo que devuelve sea correcto
*/
void prueba_pila_vacia();

void prueba_borde();

void pruebaApilar();

void apilarCantidad();



int main(void)
{
	prueba_pila_vacia();
	prueba_borde();
	pruebaApilar();
	apilarCantidad();
	return 0;
}

void print_test(char* name, bool result)
{
	printf("%s: %s\n", name, result? "OK" : "ERROR");
}

void separar(char *titulo)
{
	printf("\n\n%s", titulo);
}

/* Pruebas con una pila vacia */
void prueba_pila_vacia()
{
	pila_t *pila = pila_crear();
	print_test("Ver el tope de pila vacia", pila_ver_tope(pila) == NULL);
	pila_desapilar(pila);
	pila_destruir(pila);
}


/* Pruebas con una pila que devuelva bien los elementos que estan al borde */
void prueba_borde()
{
	pila_t *pila = pila_crear();
	int vector[10];
	for (int i = 0; i < 10; i++)
	{
		vector[i] = i;
		pila_apilar(pila, vector + i);
	}
	print_test("Verifico que en la pila esta el ultimo numero", *((int*)(pila_ver_tope(pila))) == 9);
	int numero = 22;
	pila_apilar(pila, &numero);
	print_test("Despues de redimensionar", *((int*)(pila_ver_tope(pila))) == 22);
	int *puntero = pila_desapilar(pila);
	print_test("Desapilo el 22", *puntero == numero);
	puntero = pila_desapilar(pila);
	print_test("Desapilo el 9", *puntero == 9);
	/* Apilo dos veces para redimensionar el vector */
	pila_apilar(pila, puntero);
	pila_apilar(pila, puntero);
	puntero = pila_desapilar(pila);
	puntero = pila_desapilar(pila);
	print_test("Desapilo el 9", *puntero == 9);
	print_test("Desapilo el 9", *puntero == 9);
	/* Desapilo todo */
	for (int i = 8; i >= 0; i--)
	{
		puntero = pila_desapilar(pila);
		print_test("Desapilo los consecuentes", *puntero == i);
	}
	pila_destruir(pila);
}


void pruebaApilar()
{
	separar("Prueba apilar\n");
	/* Creo la pila */
	pila_t *pila = pila_crear();
	print_test("Prueba pila vacia: ", pila_esta_vacia(pila));
	
	/* Inicializacion de elementos a apilar */
	int numero = 22;
	char *palabra = "Una cadena de texto cualquiera";
	long *punteroLong = malloc(sizeof(long));
	if (punteroLong != NULL)
	{
		*punteroLong = 10000;
	}
	int vector[10];
	for (int i = 0; i < 10; i++)
	{
		vector[i] = i;
	}

	/* Apilo todos los elementos y me guardo un numero para verificar que puedo ver el elemento sin desapilarlo*/
	print_test("Apilar numero = 22",pila_apilar(pila,&numero));
	int *otroNumero = pila_ver_tope(pila);
	print_test("Apilar string",pila_apilar(pila, palabra));
	print_test("Apilar un puntero a long",pila_apilar(pila, punteroLong));
	print_test("Apilar el vector de enteros", pila_apilar(pila, vector));

	print_test("Verificar que el ver tope funciono", *otroNumero == numero);
	print_test("Verificar que la pila tiene algun elemento", !pila_esta_vacia(pila));

	/* Desapilo todos los elementos */
	int *otroVector = pila_desapilar(pila);
	long *otroPunteroLong = pila_desapilar(pila);
	char *otraPalabra = pila_desapilar(pila);
	otroNumero = pila_desapilar(pila);
	print_test("Prueba pila vacia", pila_esta_vacia(pila));
	
	separar("Desapilado:\n");
	print_test("Numero 22", *otroNumero == numero);
	print_test("Cadena: ", strcmp(otraPalabra, palabra) == 0);		
	print_test("Puntero a long", otroPunteroLong == punteroLong);
	print_test("Puntero a long(valor)", *otroPunteroLong == *punteroLong);

	bool valido = true;	
	for (int i = 0; i < 10; i++)
	{
		if (otroVector[i] != vector[i])
			valido = false;	
	}
	print_test("Vector de enteros", valido);
	
	free(punteroLong);
	pila_destruir(pila);
}


/* Prueba que al apilar muchos elementos, se redimensione bien la pila y devuelva
 * correctamente los elementos apilados
 */
void apilarCantidad()
{
	separar("Apilado en cantidad\n");
	pila_t *pila = pila_crear();	
	int vector[100];
	for (int i = 0; i < MAX_APILADO; i++)
	{
		vector[i] = i;
		pila_apilar(pila, &(vector[i]));		
	}
	bool valido = true;
	
	for (int i = MAX_APILADO-1 ; i >= 0; i--)
	{
		int *numero = pila_desapilar(pila);
		if (numero != NULL)
		{
			if (*numero != i)
			{
				valido = false;
			}
		}
	}	
	for (int i = 0; i < MAX_APILADO; i++)
	{
		vector[i] = i;
		pila_apilar(pila, &(vector[i]));		
	}
	
	for (int i = MAX_APILADO-1 ; i >= 0; i--)
	{
		int *numero = pila_desapilar(pila);
		if (numero != NULL)
		{
			if (*numero != i)
			{
				valido = false;
			}
		}
	}
	for (int i = 0; i < MAX_APILADO; i++)
	{
		vector[i] = i;
		pila_apilar(pila, &(vector[i]));		
	}
	
	
	for (int i = MAX_APILADO-1 ; i >= 0; i--)
	{
		int *numero = pila_desapilar(pila);
		if (numero != NULL)
		{
			if (*numero != i)
			{
				valido = false;
			}
		}
	}	
	print_test("Apilado de enteros", valido);
	print_test("Pila vacia",pila_esta_vacia(pila));
	int prueba = 22;
	pila_apilar(pila, &prueba);
	print_test("Pila no vacia", !pila_esta_vacia(pila));
	print_test("Ver tope", *((int*)pila_ver_tope(pila)) == prueba);
	int *p = pila_desapilar(pila);
	print_test("Iguales", *p == prueba);
	print_test("Desapilar", pila_desapilar(pila) == NULL);
	pila_destruir(pila);
}
