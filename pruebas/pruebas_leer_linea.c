#include "../src/leer_linea.h"
#include "pa2m.h"
#include "pruebas.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LECTURA "r"

void prueba_leer_linea_basica()
{
	FILE *archivo_prueba = fopen("./pruebas/archivos/prueba1.txt", LECTURA);
	if (!archivo_prueba) {
		pa2m_afirmar(
			false,
			"No se pudo abrir el archivo para ejecutar la prueba");
		return;
	}

	char *linea = leer_linea(archivo_prueba);
	const char *esperado = "hola nico";
	pa2m_afirmar(strcmp(linea, esperado) == 0,
		     "La linea leida es \"hola nico\"");

	free(linea);
	fclose(archivo_prueba);
}

void prueba_archivo_vacio()
{
	FILE *archivo_prueba = fopen("./pruebas/archivos/vacia.txt", LECTURA);
	if (!archivo_prueba) {
		pa2m_afirmar(
			false,
			"No se pudo abrir el archivo para ejecutar la prueba");
		return;
	}

	char *linea = leer_linea(archivo_prueba);
	pa2m_afirmar(
		linea == NULL,
		"La longitud de la linea es 0, por ende el el puntero queda en NULL");

	free(linea);
	fclose(archivo_prueba);
}

void prueba_linea_larga_leida_correctamente()
{
	FILE *archivo_prueba = fopen("./pruebas/archivos/larga.txt", LECTURA);
	if (!archivo_prueba) {
		pa2m_afirmar(
			false,
			"No se pudo abrir el archivo para ejecutar la prueba");
		return;
	}

	char *linea = leer_linea(archivo_prueba);
	pa2m_afirmar(strlen(linea) > 50,
		     "Lee correctamente una linea mas larga que "
		     "la capacidad de lectura arbitraria");

	free(linea);
	fclose(archivo_prueba);
}

void prueba_leer_linea_ptr()
{
	FILE *archivo_prueba = fopen("./pruebas/archivos/larga.txt", LECTURA);
	if (!archivo_prueba) {
		pa2m_afirmar(
			false,
			"No se pudo abrir el archivo para ejecutar la prueba");
		return;
	}

	size_t tamaño_inicial = 2;
	char *buffer = malloc(tamaño_inicial);

	int leidos = leer_linea_ptr(&buffer, &tamaño_inicial, archivo_prueba);
	pa2m_afirmar(tamaño_inicial >= (size_t)leidos + 1,
		     "El tamaño crecio adecuadamente");

	free(buffer);
	fclose(archivo_prueba);
}

void pruebas_leer_linea()
{
	pa2m_nuevo_grupo("Pruebas leer_linea");
	prueba_leer_linea_basica();
	prueba_linea_larga_leida_correctamente();
	prueba_archivo_vacio();

	pa2m_nuevo_grupo("Prueba leer_linea_ptr");
	prueba_leer_linea_ptr();
}
