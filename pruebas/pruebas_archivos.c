#include "../src/leer_linea.h"
#include "../src/tp1.h"
#include "pa2m.h"
#include "pruebas.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR -1
#define FORMATO_LECTURA "%i,%f,%c\n"
#define CARACTER_COMA ','
#define CARACTER_0 '\0'

const int CANTIDAD_POKEMONES_VALIDOS = 4;
const int CANTIDAD_POKEMONES_VALIDOS_LINEAS_VALIDAS = 5;

void prueba_archivo_lineas_invalidas()
{
	tp1_t *tp1 =
		tp1_leer_archivo("./pruebas/archivos/pokemones(invalido).csv");
	size_t cantidad = tp1_cantidad(tp1);
	pa2m_afirmar(
		cantidad == CANTIDAD_POKEMONES_VALIDOS,
		"La cantidad de pokemones validos es %i, y en el TP1 hay %i pokemones",
		CANTIDAD_POKEMONES_VALIDOS, cantidad);

	tp1_destruir(tp1);
}

void prueba_archivo_lineas_validas()
{
	tp1_t *tp1 = tp1_leer_archivo("./pruebas/archivos/pokemones.csv");
	size_t cantidad = tp1_cantidad(tp1);
	pa2m_afirmar(
		cantidad == CANTIDAD_POKEMONES_VALIDOS_LINEAS_VALIDAS,
		"La cantidad de pokemones validos es %i, y en el TP1 hay %i pokemones",
		CANTIDAD_POKEMONES_VALIDOS_LINEAS_VALIDAS, cantidad);

	tp1_destruir(tp1);
}

void prueba_escribe_en_archivo()
{
	const char *buscado = "Lapras";
	const char *nombre_archivo = "archivo";

	tp1_t *tp1 = tp1_leer_archivo("./pruebas/archivos/pokemones.csv");
	tp1_escribir_archivo(tp1, nombre_archivo);

	FILE *pokemones = fopen(nombre_archivo, "r");
	pa2m_afirmar(pokemones != NULL, "Se pudo abrir el archivo escrito");
	if (pokemones == NULL) {
		tp1_destruir(tp1);
		return;
	}

	bool encontrado = false;
	char *linea = leer_linea(pokemones);

	while (linea != NULL && !encontrado) {
		char *coma = strchr(linea, CARACTER_COMA);

		if (coma != NULL) {
			size_t longitud = (size_t)(coma - linea);

			char *nombre = malloc((longitud + 1) * sizeof(char));

			if (nombre != NULL) {
				strncpy(nombre, linea, longitud);
				nombre[longitud] = CARACTER_0;
				if (strcmp(nombre, buscado) == 0) {
					encontrado = true;
				}
				free(nombre);
			}
		}
		free(linea);
		linea = leer_linea(pokemones);
	}

	free(linea);

	pa2m_afirmar(encontrado,
		     "El pokemon %s fue encontrado en el archivo.\n", buscado);

	fclose(pokemones);
	remove(nombre_archivo);
	tp1_destruir(tp1);
}

void prueba_archivo_con_csv_vacio()
{
	const char *nombre_archivo = "archivo";

	tp1_t *tp1 =
		tp1_leer_archivo("./pruebas/archivos/pokemones(vacio).csv");
	tp1 = tp1_escribir_archivo(tp1, nombre_archivo);
	pa2m_afirmar(tp1 != NULL,
		     "El archivo es esta vacio por carga de csv vacio.\n");

	tp1_destruir(tp1);
	remove(nombre_archivo);
}

void pruebas_archivos()
{
	pa2m_nuevo_grupo("Pruebas con lineas invalidas");
	prueba_archivo_lineas_invalidas();
	prueba_archivo_lineas_validas();

	pa2m_nuevo_grupo("Pruebas escribe en archivo");
	prueba_escribe_en_archivo();

	pa2m_nuevo_grupo("Prueba prueba archivo con csv vacio");
	prueba_archivo_con_csv_vacio();
}
