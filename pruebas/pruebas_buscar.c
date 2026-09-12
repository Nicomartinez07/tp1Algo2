#include "../src/tp1.h"
#include "pa2m.h"
#include "pruebas.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LECTURA "r"

void prueba_buscar_larpas()
{
	const char *buscado = "Lapras";

	tp1_t *tp1 = tp1_leer_archivo("./pruebas/archivos/pokemones.csv");
	struct pokemon *actual = tp1_buscar_pokemon(tp1, buscado);
	pa2m_afirmar(
		actual != NULL && strcmp(actual->nombre, buscado) == 0,
		"El pokemon buscado es %s, el encontrado es %s\n Velocidad: "
		"%d\n Peso: %f \n Rareza: %s",
		buscado, actual->nombre, actual->velocidad, actual->peso,
		actual->rareza == RAREZA_COMUN ? "Comun" :
		actual->rareza == RAREZA_RARO  ? "Raro" :
						 "Legendario");

	tp1_destruir(tp1);
}

void prueba_buscar_charizard()
{
	const char *buscado = "Charizard";

	tp1_t *tp1 = tp1_leer_archivo("./pruebas/archivos/pokemones.csv");
	struct pokemon *actual = tp1_buscar_pokemon(tp1, buscado);
	pa2m_afirmar(actual == NULL,
		     "El pokemon buscado es %s y no lo encuentra", buscado);

	tp1_destruir(tp1);
}

void prueba_buscar_n_valido()
{
	size_t n = 2;

	tp1_t *tp1 = tp1_leer_archivo("./pruebas/archivos/pokemones.csv");
	struct pokemon *actual = tp1_buscar_orden(tp1, n);
	pa2m_afirmar(actual != NULL,
		     "El pokemon buscado es el %zu y su nombre es %s.\n", n,
		     actual->nombre);

	tp1_destruir(tp1);
}

void prueba_buscar_n_invalido()
{
	size_t n = 100;

	tp1_t *tp1 = tp1_leer_archivo("./pruebas/archivos/pokemones.csv");
	struct pokemon *actual = tp1_buscar_orden(tp1, n);
	pa2m_afirmar(actual == NULL,
		     "El pokemon buscado es el %zu y no lo encuentra.\n", n);

	tp1_destruir(tp1);
}

void pruebas_buscar()
{
	pa2m_nuevo_grupo("Pruebas buscar");
	prueba_buscar_larpas();
	prueba_buscar_charizard();

	pa2m_nuevo_grupo("Pruebas buscar orden");
	prueba_buscar_n_valido();
	prueba_buscar_n_invalido();
}
