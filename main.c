#include "src/tp1.h"
#include <string.h>
#include <time.h>
#include <stdio.h>

#define MOSTRAR_TODO "mostrar-todo"
#define MOSTRAR_UNO "mostrar-uno"
#define MOSTRAR_RAREZAS "mostrar-rarezas"
#define BUSCAR_NOMBRE "buscar-nombre"
#define ERROR -1

const int CANTIDAD_MINIMA_PARAMETROS = 3;
const int CANTIDAD_BUSCAR_NOMBRE = 4;

char obtener_caracter_rareza(enum rareza_pokemon rareza)
{
	switch (rareza) {
	case RAREZA_COMUN:
		return 'C';
	case RAREZA_RARO:
		return 'R';
	case RAREZA_LEGENDARIO:
		return 'L';
	}

	return '\0';
}

void imprimir_pokemon(struct pokemon *actual, char rareza)
{
	printf("Nombre: %s\n", actual->nombre);
	printf("Velocidad: %i\n", actual->velocidad);
	printf("Peso: %f\n", actual->peso);
	printf("Rareza: %c\n", rareza);
	printf("\n");
}

bool tiene_cierta_rareza(struct pokemon *p, void *extra)
{
	if (!p) {
		return false;
	}

	if (p->rareza == *((enum rareza_pokemon *)extra)) {
		char rareza = obtener_caracter_rareza(p->rareza);
		imprimir_pokemon(p, rareza);
	}
	return true;
}

int main(int argc, char const *argv[])
{
	if (argc < CANTIDAD_MINIMA_PARAMETROS) {
		printf("Se deben ingresar como minimo 3 parametros para el correcto "
		       "funcionamiento del programa.\n Uso: ./ejecutable <archivo_a_leer> "
		       "<funcion_a_ejecutar> <extras>\n");
		return ERROR;
	}

	bool hay_error = false;

	srand((unsigned int)time(NULL));

	tp1_t *pokedex = tp1_leer_archivo(argv[1]);
	if (pokedex == NULL) {
		return ERROR;
	}

	if (strcmp(argv[2], MOSTRAR_UNO) == 0) {
		if (tp1_cantidad(pokedex) > 0) {
			size_t indice_random =
				(size_t)rand() % tp1_cantidad(pokedex);

			struct pokemon *actual =
				tp1_buscar_orden(pokedex, indice_random);
			if (!actual) {
				printf("Hubo un error al imprimir el pokemon\n");
			} else {
				char rareza =
					obtener_caracter_rareza(actual->rareza);
				printf("El pokemon N: %zu es: \n",
				       indice_random);
				imprimir_pokemon(actual, rareza);
			}
		}

	} else if (strcmp(argv[2], MOSTRAR_TODO) == 0) {
		size_t cantidad = tp1_cantidad(pokedex);

		for (size_t i = 0; i < cantidad; i++) {
			struct pokemon *actual = tp1_buscar_orden(pokedex, i);
			if (!actual) {
				printf("Hubo un error al imprimir el pokemon\n");
			} else {
				char rareza =
					obtener_caracter_rareza(actual->rareza);
				printf("El pokemon N: %zu es: \n", i);
				imprimir_pokemon(actual, rareza);
			}
		}

	} else if (strcmp(argv[2], MOSTRAR_RAREZAS) == 0) {
		enum rareza_pokemon rareza_comun = RAREZA_COMUN;
		enum rareza_pokemon rareza_raro = RAREZA_RARO;
		enum rareza_pokemon rareza_legendaria = RAREZA_LEGENDARIO;

		size_t cantidad_rareza_comun =
			tp1_iterar(pokedex, tiene_cierta_rareza, &rareza_comun);

		size_t cantidad_rareza_raro =
			tp1_iterar(pokedex, tiene_cierta_rareza, &rareza_raro);

		size_t cantidad_rareza_legendario = tp1_iterar(
			pokedex, tiene_cierta_rareza, &rareza_legendaria);

		printf("Cantidad de pokemones comunes %zu, de pokemones raros %zu, de "
		       "pokemones legendarios %zu .\n",
		       cantidad_rareza_comun, cantidad_rareza_raro,
		       cantidad_rareza_legendario);

	} else if (strcmp(argv[2], BUSCAR_NOMBRE) == 0 &&
		   argc == CANTIDAD_BUSCAR_NOMBRE) {
		struct pokemon *buscado = tp1_buscar_pokemon(pokedex, argv[3]);
		if (!buscado) {
			printf("Hubo un error al encontrar el pokemon buscado\n");
		} else {
			char rareza = obtener_caracter_rareza(buscado->rareza);
			printf("El pokemon buscado es: \n");
			imprimir_pokemon(buscado, rareza);
		}
	} else {
		printf("Los comandos validos son: 'mostrar-todo', 'mostrar-uno', "
		       "'mostrar-rarezas', 'buscar-nombre' <nombre_a_buscar>.\n Por favor "
		       "ingrese uno valido\n");
		hay_error = true;
	}

	tp1_destruir(pokedex);

	if (hay_error) {
		return ERROR;
	}
	return 0;
}