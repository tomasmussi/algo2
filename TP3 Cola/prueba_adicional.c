#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cola.h"

typedef struct _punto{
	int *puntero,x,y;
} punto_t;

void destruir_dato(void *punto);

void sin_memoria(char *mensaje);


int main(void)
{
	punto_t *punto = malloc(sizeof(punto_t));
	if (punto == NULL)
		sin_memoria("el punto");
	int *puntero = malloc(sizeof(int));
	if (puntero == NULL)
	{
		free(punto);
		sin_memoria("el puntero");
	}
	punto->x = 10;
	punto->y = 20;
	punto->puntero = puntero;
	cola_t *cola = cola_crear();
	cola_encolar(cola, punto);
	void (*destruir)(void*) = &destruir_dato;
	cola_destruir(cola, destruir);
	printf("La cola fue destruida\n");
	return 0;
}

void destruir_dato(void *punto)
{
	punto_t *punto_posta;
	punto_posta = ((punto_t*) punto);
	free(punto_posta->puntero);
	free(punto_posta);
}

void sin_memoria(char *mensaje)
{
	printf("Sin memoria para %s\n", mensaje);
	exit(1);
}
