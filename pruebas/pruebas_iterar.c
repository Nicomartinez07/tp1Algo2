#include "../src/tp1.h"
#include "pa2m.h"
#include "pruebas.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int CANTIDAD_POKEMONES = 5;
const int CANTIDAD_POKEMONES_COMUNES = 3;

bool contar_pokemon(struct pokemon *pokemon, void *extra)
{
	size_t *contador = extra;
	(*contador)++;

	return true;
}

void prueba_contar_pokemones()
{
	tp1_t *tp1 = tp1_leer_archivo("./pruebas/archivos/pokemones.csv");

	size_t contador = 0;

	tp1_iterar(tp1, contar_pokemon, &contador);
	pa2m_afirmar(contador == CANTIDAD_POKEMONES,
		     "Se leyeron correctamente %i pokemones.",
		     CANTIDAD_POKEMONES);

	tp1_destruir(tp1);
}

void prueba_funcion_nula()
{
	tp1_t *tp1 = tp1_leer_archivo("./pruebas/archivos/pokemones.csv");

	size_t contador_comunes = 0;

	tp1_iterar(tp1, contar_pokemon, &contador_comunes);
	pa2m_afirmar(contador_comunes == CANTIDAD_POKEMONES_COMUNES,
		     "Se leyeron correctamente %i pokemones de rareza COMUN.",
		     contador_comunes);

	tp1_destruir(tp1);
}

void prueba_tp1_nulo()
{
	size_t contador = tp1_iterar(NULL, contar_pokemon, NULL);
	pa2m_afirmar(contador == 0,
		     "El contador es %i debido a que el tp1 es NULL.",
		     contador);
}

void prueba_funcion_null()
{
	tp1_t *tp1 = tp1_leer_archivo("./pruebas/archivos/pokemones.csv");

	size_t contador = tp1_iterar(tp1, NULL, NULL);
	pa2m_afirmar(
		contador == 0,
		"El contador es %i debido a que el la funcion pasada por parametro es NULL.",
		contador);

	tp1_destruir(tp1);
}

void pruebas_iterar()
{
	pa2m_nuevo_grupo("Pruebas contador");
	prueba_contar_pokemones();

	pa2m_nuevo_grupo("Pruebas con parametros NULL");
	prueba_tp1_nulo();
	prueba_funcion_null();
}
