#include "leer_linea.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define ERROR -1

const int CAPACIDAD_LECTURA = 10;
/**
 * Lee caracteres por entrada estándar hasta encontrar el fin de línea (\n) o no
 * queden mas caracteres para leer.
 *
 * Devuelve un puntero a un string reservado dinámicamente con malloc a liberar
 * por el usuario utilizando free o NULL en caso de error.
 */
char *leer_linea(FILE *archivo) {
  size_t tamaño_lectura = CAPACIDAD_LECTURA;
  size_t capacidad = tamaño_lectura;
  size_t uso_actual = 0;
  bool leyo_cosas = false;

  char *string = malloc(sizeof(char) * tamaño_lectura);
  if (!string) {
    printf("Hubo un error al reservar el string \n");
    return NULL;
  }

  char *linea_actual = malloc(sizeof(char) * tamaño_lectura);
  if (!linea_actual) {
    printf("Hubo un error al reservar la linea \n");
    free(string);
    return NULL;
  }

  while (fgets(linea_actual, (int)tamaño_lectura, archivo) != NULL) {
    size_t leidos = strlen(linea_actual);
    leyo_cosas = true;

    if (uso_actual + leidos + 1 > capacidad) {
      capacidad *= 2;
      char *aux = realloc(string, capacidad);
      if (!aux) {
        printf("Hubo un error al reservar el nuevo string \n");
        free(string);
        free(linea_actual);
        return NULL;
      }

      string = aux;
    }

    strcpy(string + uso_actual, linea_actual);
    uso_actual += leidos;

    if (leidos < tamaño_lectura - 1 || linea_actual[leidos - 1] == '\n') {
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
int leer_linea_ptr(char **ptr, size_t *tamaño, FILE *archivo) {
  size_t uso_actual = 0;
  size_t tamaño_lectura = CAPACIDAD_LECTURA;
  bool leyo_cosas = false;

  char *linea_actual = malloc(sizeof(char) * tamaño_lectura);
  if (!linea_actual) {
    printf("Hubo un error al reservar la linea \n");
    return ERROR;
  }

  if (*tamaño <= 0) {
    size_t nuevo_tamaño = tamaño_lectura;

    char *aux = realloc(*ptr, nuevo_tamaño);
    if (!aux) {
      printf("Hubo un error al reservar el nuevo puntero \n");
      free(linea_actual);
      return ERROR;
    }

    *ptr = aux;
    *tamaño = nuevo_tamaño;
  }

  while (fgets(linea_actual, (int)tamaño_lectura, archivo) != NULL) {
    size_t leidos = strlen(linea_actual);
    leyo_cosas = true;

    if (uso_actual + leidos + 1 > (*tamaño)) {
      size_t nuevo_tamaño = *tamaño;

      while (uso_actual + leidos + 1 > nuevo_tamaño) {
        nuevo_tamaño *= 2;
      }

      char *aux = realloc(*ptr, nuevo_tamaño);
      if (!aux) {
        printf("Hubo un error al reservar el nuevo puntero \n");
        free(linea_actual);
        return ERROR;
      }

      *ptr = aux;
      *tamaño = nuevo_tamaño;
    }
    strcpy(*ptr + uso_actual, linea_actual);
    uso_actual += leidos;

    if (leidos < tamaño_lectura - 1 || linea_actual[leidos - 1] == '\n') {
      break;
    }
  }
  free(linea_actual);

  if (!leyo_cosas) {
    return ERROR;
  }
  return (int)(uso_actual);
}