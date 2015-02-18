#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"
#include "sesion.h"

#define MAX 255
#define MAX_PALABRA 21
#define MAX_PARAMETROS 5


/* Funcion que se encarga de parsear y ejecutar un comando recibido */
void parsear(sistema_t *sistema, char *comando);

/* Funcion que evalua entre las distintas funciones y ejecuta la que corresponda */
void ejecutar_funcion(sistema_t *sistema, char *funcion, char parametros[][MAX_PALABRA], int cantidad_parametros);


void ejecutar_funcion(sistema_t *sistema, char funcion[], char parametros[][MAX_PALABRA], int cantidad_parametros)
{
	if (strcmp(funcion, "agregar_curso") == 0) {
		size_t vacantes = atoi(parametros[3]);
		agregar_curso(sistema, parametros[0], parametros[1], parametros[2],vacantes);
	} else if (strcmp(funcion, "listar_cursos") == 0){
		listar_cursos(sistema, NULL);
	} else if (strcmp(funcion, "listar_cursos_materia") == 0){
		listar_cursos(sistema, parametros[0]);
	} else if (strcmp(funcion, "inscribir") == 0){
		size_t padron = atoi(parametros[0]);
		inscribir(sistema, padron, parametros[1]);
	} else if (strcmp(funcion, "eliminar_curso") == 0){
		eliminar_curso(sistema, parametros[0]);
	} else if (strcmp(funcion, "desinscribir") == 0){
		size_t padron = atoi(parametros[0]);
		desinscribir(sistema, padron, parametros[1]);
	} else if (strcmp(funcion, "listar_inscriptos") == 0){
		listar_inscriptos(sistema, parametros[0]);
	} else if (strcmp(funcion, "sesion_aplicar") == 0){
		sesion_aplicar_sistema(sistema);
	} else if (strcmp(funcion, "sesion_iniciar") == 0){
		size_t padron = atoi(parametros[0]);
		sesion_iniciar_sistema(sistema, padron);
	} else if (strcmp(funcion, "sesion_inscribir") == 0){
		sesion_inscribir_sistema(sistema, parametros[0]);
	} else if (strcmp(funcion, "sesion_ver") == 0){
		sesion_ver_sistema(sistema);
	} else if (strcmp(funcion, "sesion_deshacer") == 0){
		sesion_deshacer_sistema(sistema);
	} 
}


void parsear(sistema_t *sistema, char *comando)
{
	char funcion[MAX_PALABRA];
	int i = 0;
	while (comando[i] != ' ' && comando[i] != '\n' && i < MAX_PALABRA)
	{
		funcion[i] = comando[i];
		i++;
	}
	bool hay_parametros = true;
	if (comando[i] == '\n')
		hay_parametros = false;
	funcion[i] = '\0';
	i++;

	//Tengo que parsear los parametros separados por ',', si los hay
	int cantidad_parametros = 0;
	int nuevo_indice = 0;
	char parametros[MAX_PARAMETROS][MAX_PALABRA];
	while (comando[i] != '\n' && i <= MAX && hay_parametros)
	{
		if (comando[i] != ',')
		{
			parametros[cantidad_parametros][nuevo_indice] = comando[i];
			nuevo_indice++;
		}
		else
		{
			parametros[cantidad_parametros][nuevo_indice] = '\0';
			nuevo_indice = 0;
			cantidad_parametros++;
		}
		i++;
	}
	parametros[cantidad_parametros][nuevo_indice] = '\0';
	ejecutar_funcion(sistema,funcion, parametros, cantidad_parametros);
}



/* Main del sistema de inscripcion */
int main(void)
{
	//Creo el sistema que se encargara de manejar todos los comandos
	sistema_t *sistema = sistema_crear();

	//En main, lo unico que se hace es recibir el comando de entrada y
	//enviar al sistema la peticion que se hace por comando
	char buffer[MAX];
	//Segun lo especificado en man, fgets devuelve NULL 
	//cuando se termina el flujo de bytes
	while (fgets(buffer, MAX, stdin) != NULL)	
	{
		parsear(sistema, buffer);
	}
	//Libero la memoria usada
	sistema_destruir(sistema);
	return 0;
}
