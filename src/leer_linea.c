#include "leer_linea.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ERROR -1

const int CAPACIDAD_LECTURA = 128;

/*
 * Agranda la capacidad del puntero hasta el tamaño necesario. 
 * 
 * En caso de error devuelve false
 */
bool agrandar_memoria(char **ptr, size_t *capacidad, size_t tamaño_necesario)
{
	if (!ptr || !capacidad)
		return false;

	if (*capacidad >= tamaño_necesario)
		return true;

	size_t nueva_capacidad = *capacidad;

	if (nueva_capacidad == 0)
		nueva_capacidad = CAPACIDAD_LECTURA;

	while (nueva_capacidad < tamaño_necesario)
		nueva_capacidad *= 2;

	char *aux = realloc(*ptr, nueva_capacidad);
	if (!aux)
		return false;

	*ptr = aux;
	*capacidad = nueva_capacidad;

	return true;
}

/**
 * Lee caracteres por entrada estándar hasta encontrar el fin de línea (\n) o no
 * queden mas caracteres para leer.
 *
 * Devuelve un puntero a un string reservado dinámicamente con malloc a liberar
 * por el usuario utilizando free o NULL en caso de error.
 */
char *leer_linea(FILE *archivo)
{
	if (!archivo)
		return NULL;
	size_t tamaño_lectura = CAPACIDAD_LECTURA;
	size_t capacidad = tamaño_lectura;
	size_t uso_actual = 0;
	bool leyo_cosas = false;

	char *string = malloc(sizeof(char) * tamaño_lectura);
	if (!string) {
		return NULL;
	}

	char *linea_actual = malloc(sizeof(char) * tamaño_lectura);
	if (!linea_actual) {
		free(string);
		return NULL;
	}

	while (fgets(linea_actual, (int)tamaño_lectura, archivo) != NULL) {
		size_t leidos = strlen(linea_actual);
		leyo_cosas = true;

		if (!agrandar_memoria(&string, &capacidad,
				      uso_actual + leidos + 1)) {
			free(string);
			free(linea_actual);
			return NULL;
		}

		strcpy(string + uso_actual, linea_actual);
		uso_actual += leidos;

		if (leidos < tamaño_lectura - 1 ||
		    linea_actual[leidos - 1] == '\n') {
			break;
		}
	}
	free(linea_actual);

	if (!leyo_cosas) {
		free(string);
		return NULL;
	}

	return string;
}

/*
 * Lee caracteres por entrada estándar hasta encontrar el fin de línea o no
 * queden mas caracteres por leer. Los caracteres leídos se van insertando en el
 * bloque de memoria apuntada por *ptr de tamaño inicial *tamaño. Dicho bloque
 * debe haber sido reservado por el usuario previa invocación de la función
 * utilizando malloc.
 *
 * Si el tamaño del bloque no es suficiente, se utiliza realloc para agrandar el
 * bloque y se actualiza *tamaño y *ptr.
 *
 * Devuelve la cantidad de caracteres leidos (sin incluír el 0 final) o -1 en
 * caso de error.
 */
int leer_linea_ptr(char **ptr, size_t *tamaño, FILE *archivo)
{
	if (!ptr || !tamaño || !archivo)
		return ERROR;

	size_t uso_actual = 0;
	size_t tamaño_lectura = CAPACIDAD_LECTURA;
	bool leyo_cosas = false;

	char *linea_actual = malloc(sizeof(char) * tamaño_lectura);
	if (!linea_actual)
		return ERROR;

	if (!agrandar_memoria(ptr, tamaño, tamaño_lectura)) {
		free(linea_actual);
		return ERROR;
	}

	while (fgets(linea_actual, (int)tamaño_lectura, archivo) != NULL) {
		size_t leidos = strlen(linea_actual);
		leyo_cosas = true;

		if (!agrandar_memoria(ptr, tamaño, uso_actual + leidos + 1)) {
			free(linea_actual);
			return ERROR;
		}

		strcpy(*ptr + uso_actual, linea_actual);
		uso_actual += leidos;

		if (leidos < tamaño_lectura - 1 ||
		    linea_actual[leidos - 1] == '\n') {
			break;
		}
	}

	free(linea_actual);

	if (!leyo_cosas)
		return ERROR;

	return (int)uso_actual;
}