#include "pa2m.h"
#include "pruebas.h"
#include "tp1.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LECTURA "r"

void prueba_leer_larpas() {
  const char *buscado = "Larpas";

  struct pokemon *actual = tp1_buscar_pokemon(tp1_leer_archivo("./pruebas/archivos/pokemones.csv"), buscado);
  pa2m_afirmar(strcmp(actual->nombre, buscado) == 0, "El pokemon buscado es %s, el encontrado es %s\n Velocidad: %d\n Peso: %f \n Rareza: %s", buscado, actual->nombre, actual->velocidad, actual->peso, actual->rareza == RAREZA_COMUN ? "Comun" : actual->rareza == RAREZA_RARO ? "Raro" : "Legendario");
}

void prueba_leer_charizard() {
  const char *buscado = "Charizard";

  struct pokemon *actual = tp1_buscar_pokemon(tp1_leer_archivo("./pruebas/archivos/pokemones.csv"), buscado);
  pa2m_afirmar(actual == NULL, "El pokemon buscado es %s y no lo encuentra", buscado);
}

void prueba_buscar_n_valido() {
    size_t n = 2;

    struct pokemon *actual = tp1_buscar_pokemon(tp1_leer_archivo("./pruebas/archivos/pokemones.csv"), n);
    pa2m_afirmar(actual != NULL, "El pokemon buscado es el %zu y su nombre es %s.\n", n, actual->nombre);
}

void prueba_buscar_n_invalido() {
    size_t n = 100;

    struct pokemon *actual = tp1_buscar_pokemon(tp1_leer_archivo("./pruebas/archivos/pokemones.csv"), n);
    pa2m_afirmar(actual == NULL, "El pokemon buscado es el %zu y no lo encuentra.\n", n);
}

void pruebas_buscar() {
  pa2m_nuevo_grupo("Pruebas buscar");
  prueba_buscar_larpas();
  prueba_buscar_charizard();
  prueba_buscar_n_valido();
  prueba_buscar_n_invalido();
}
