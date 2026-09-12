#include "../src/tp1.h"
#include "pa2m.h"
#include "pruebas.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int VELOCIDAD_ESPERADA = 90;
const int CANTIDAD_POKEMONES_ESPERADA = 6;

void prueba_combinar_longitud_6()
{
	tp1_t *tp1 = tp1_leer_archivo("./pruebas/archivos/pokemones.csv");
	tp1_t *tp2 = tp1_leer_archivo("./pruebas/archivos/pokemon.csv");

	tp1_t *tp3 = tp1_combinar(tp1, tp2);
	size_t cantidad = tp1_cantidad(tp3);
	pa2m_afirmar(cantidad == CANTIDAD_POKEMONES_ESPERADA,
		     "La cantidad de pokemones en el TP combinado es %i",
		     cantidad);

	tp1_destruir(tp1);
	tp1_destruir(tp2);

	tp1_destruir(tp3);
}

void prueba_combinar_iguales()
{
	tp1_t *tp1 = tp1_leer_archivo("./pruebas/archivos/pokemones.csv");
	tp1_t *tp2 =
		tp1_leer_archivo("./pruebas/archivos/pokemones(copia).csv");

	tp1_t *tp3 = tp1_combinar(tp1, tp2);

	const char *buscado = "Suicune";
	struct pokemon *actual = tp1_buscar_pokemon(tp3, buscado);
	pa2m_afirmar(
		actual->velocidad == VELOCIDAD_ESPERADA,
		"La velocidad del pokemon %s esperada es: %i, la obtenida es: %i",
		actual->nombre, VELOCIDAD_ESPERADA, actual->velocidad);

	tp1_destruir(tp1);
	tp1_destruir(tp2);

	tp1_destruir(tp3);
}

void prueba_combinar_vacios()
{
	tp1_t *tp1 =
		tp1_leer_archivo("./pruebas/archivos/pokemones(vacio).csv");
	tp1_t *tp2 =
		tp1_leer_archivo("./pruebas/archivos/pokemones(vacio).csv");

	tp1_t *tp3 = tp1_combinar(tp1, tp2);

	pa2m_afirmar(
		tp3 != NULL && tp1_cantidad(tp3) == 0,
		"No se pudieron combinar los TP1, porque ambos estan vacios.\n");

	tp1_destruir(tp1);
	tp1_destruir(tp2);

	tp1_destruir(tp3);
}

void prueba_cantidad_vacio()
{
	tp1_t *tp1 =
		tp1_leer_archivo("./pruebas/archivos/pokemones(vacio).csv");

	size_t cantidad = tp1_cantidad(tp1);

	pa2m_afirmar(
		cantidad == 0,
		"Se entrego un TP1 vacio y la cantidad de elementos es %i.\n",
		cantidad);

	tp1_destruir(tp1);
}

void prueba_cantidad()
{
	tp1_t *tp1 = tp1_leer_archivo("./pruebas/archivos/pokemones.csv");

	size_t cantidad = tp1_cantidad(tp1);

	pa2m_afirmar(
		cantidad == 5,
		"Se entrego un TP1 con 5 elementos y la cantidad de elementos es %i.\n",
		cantidad);

	tp1_destruir(tp1);
}

void pruebas_combinar()
{
	pa2m_nuevo_grupo("Pruebas combinar");
	prueba_combinar_longitud_6();
	prueba_combinar_iguales();
	prueba_combinar_vacios();

	pa2m_nuevo_grupo("Pruebas cantidad");
	prueba_cantidad_vacio();
	prueba_cantidad();
}