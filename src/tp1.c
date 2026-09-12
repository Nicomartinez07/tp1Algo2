#include "tp1.h"
#include "../src/leer_linea.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define LECTURA "r"
#define ESCRITURA "w"
#define FORMATO_LECTURA_SIN_NOMBRE "%d,%f,%c\n"
#define FORMATO_ESCRITURA "%s,%i,%f,%c\n"

#define RARO 'R'
#define COMUN 'C'
#define LEGENDARIO 'L'

struct tp1 {
	struct pokemon *pokemones;
	size_t cantidad;
	size_t capacidad;
};

const size_t CAPACIDAD_INICIAL = 10;
const int ERROR = -1;
const int EXITO = 0;
const int PESO_MINIMO = 0;
const int POKEMON_NO_ENCONTRADO = 0;
const float VELOCIDAD_MINIMA = 1;
const int VELOCIDAD_MAXIMA = 99;
const int CANTIDAD_LEIDOS_POR_LINEA = 3;
const char CARACTER_COMA = ',';
const char CARACTER_SALTO_LINEA = '\n';
const char CARACTER_NULO = '\0';
const int LONGITUD_INICIAL_NOMBRE = 100;

/*
 * Genera una copia del pokemon original en la copia. Se reserva el espacio
 * necesario para el nombre del pokemon.
 *
 * En caso de error devuelve ERROR
 */
int duplicar_pokemon(struct pokemon original, struct pokemon *copia)
{
	size_t longitud_nombre = strlen(original.nombre);
	copia->nombre = malloc((longitud_nombre + 1) * sizeof(char));
	if (!copia->nombre) {
		return ERROR;
	}
	strcpy(copia->nombre, original.nombre);

	copia->peso = original.peso;
	copia->rareza = original.rareza;
	copia->velocidad = original.velocidad;

	return EXITO;
}

size_t buscar_posicion_para_insertar(struct pokemon *pokemones, size_t cantidad,
				     const char *nombre, bool *encontrado)
{
	size_t inicio = 0;
	size_t fin = cantidad;

	while (inicio < fin) {
		size_t centro = inicio + (fin - inicio) / 2;
		int comparacion = strcasecmp(pokemones[centro].nombre, nombre);

		if (comparacion == 0) {
			*encontrado = true;
			return centro;
		}

		if (comparacion < 0) {
			inicio = centro + 1;
		} else {
			fin = centro;
		}
	}

	*encontrado = false;
	return inicio;
}

/*
 * Devuelve true si el pokemon es valido, false en caso contrario.
 */
bool pokemon_valido(struct pokemon actual)
{
	bool peso_valido = actual.peso > PESO_MINIMO;
	bool velocidad_valida = actual.velocidad >= VELOCIDAD_MINIMA &&
				actual.velocidad <= VELOCIDAD_MAXIMA;

	return (peso_valido && velocidad_valida);
}

/*
 * Dado un caracter que representa la rareza, devuelve la rareza correspondiente
 * en el enum.
 *
 * En caso de que sea invalida devuelve false
 */
bool convertir_caracter_a_rareza(char caracter, enum rareza_pokemon *rareza)
{
	switch (caracter) {
	case COMUN:
		*rareza = RAREZA_COMUN;
		return true;

	case LEGENDARIO:
		*rareza = RAREZA_LEGENDARIO;
		return true;

	case RARO:
		*rareza = RAREZA_RARO;
		return true;

	default:
		return false;
	}
}

/*
 * Dado la rareza, devuelve un caracter correspondiente a ese tipo de rareza.
 */
char convertir_rareza_a_caracter(enum rareza_pokemon rareza)
{
	char caracter = ' ';
	switch (rareza) {
	case RAREZA_COMUN:
		caracter = COMUN;
		break;
	case RAREZA_LEGENDARIO:
		caracter = LEGENDARIO;
		break;
	case RAREZA_RARO:
		caracter = RARO;
		break;
	}

	return caracter;
}

/*
 * Ordena el vector de pokemones por nombre.
 * Tope tiene que representar la cantidad de elementos del vector.
 */
void ordenar_vector(struct pokemon *pokemones, size_t tope)
{
	if (tope <= 0 || pokemones == NULL) {
		return;
	}

	for (size_t i = 0; i < tope - 1; i++) {
		for (size_t j = 0; j < tope - 1 - i; j++) {
			struct pokemon aux;

			if (strcasecmp(pokemones[j].nombre,
				       pokemones[j + 1].nombre) > 0) {
				aux = pokemones[j];
				pokemones[j] = pokemones[j + 1];
				pokemones[j + 1] = aux;
			}
		}
	}
}

/*
 *  El archivo tiene que estar abierto en modo lectura. Lee el archivo y guarda
 * los pokemones en la estructura. Devuelve ERROR en caso de error.
 */
int guardar_pokemones(FILE *archivo, tp1_t *estructura)
{
	bool hay_error = false;

	size_t tamaño_inicial = 10;
	char *linea = malloc(tamaño_inicial);
	if (!linea) {
		return ERROR;
	}

	while (leer_linea_ptr(&linea, &tamaño_inicial, archivo) >= 0 &&
	       !hay_error) {
		char *coma = strchr(linea, CARACTER_COMA);

		if (coma != NULL) {
			size_t longitud_nombre = (size_t)(coma - linea);

			if (longitud_nombre != 0) {
				char *nombre_pokemon = malloc(
					(longitud_nombre + 1) * sizeof(char));
				if (!nombre_pokemon) {
					free(linea);
					return ERROR;
				}

				strncpy(nombre_pokemon, linea, longitud_nombre);
				nombre_pokemon[longitud_nombre] = '\0';

				char caracter_rareza = ' ';
				int velocidad = 0;
				float peso = 0;

				int caracteres_leidos = 0;
				int leidos = sscanf(coma + 1, "%d,%f,%c%n",
						    &velocidad, &peso,
						    &caracter_rareza,
						    &caracteres_leidos);

				if (leidos != CANTIDAD_LEIDOS_POR_LINEA ||
				    caracteres_leidos == 0) {
					free(nombre_pokemon);
					continue;
				}

				char fin = (coma + 1)[caracteres_leidos];
				bool sin_basura = (fin == '\0' || fin == '\n' ||
						   fin == '\r');

				enum rareza_pokemon rareza;
				bool rareza_valida =
					convertir_caracter_a_rareza(
						caracter_rareza, &rareza);

				struct pokemon actual;
				actual.nombre = nombre_pokemon;
				actual.peso = peso;
				actual.velocidad = velocidad;
				actual.rareza = rareza;

				if (sin_basura && pokemon_valido(actual) &&
				    rareza_valida) {
					bool encontrado = false;

					size_t posicion =
						buscar_posicion_para_insertar(
							estructura->pokemones,
							estructura->cantidad,
							actual.nombre,
							&encontrado);

					if (!encontrado) {
						if (estructura->cantidad >=
						    estructura->capacidad) {
							size_t nueva_capacidad =
								estructura
									->capacidad *
								2;

							struct pokemon *aux = realloc(
								estructura
									->pokemones,
								sizeof(struct pokemon) *
									nueva_capacidad);

							if (!aux) {
								free(nombre_pokemon);
								hay_error =
									true;
							} else {
								estructura
									->pokemones =
									aux;
								estructura
									->capacidad =
									nueva_capacidad;
							}
						}

						if (!hay_error) {
							for (size_t i =
								     estructura
									     ->cantidad;
							     i > posicion;
							     i--) {
								estructura->pokemones
									[i] =
									estructura
										->pokemones
											[i -
											 1];
							}

							estructura->pokemones
								[posicion] =
								actual;
							estructura->cantidad++;
						}
					} else {
						free(nombre_pokemon);
					}

				} else {
					free(nombre_pokemon);
				}
			}
		}
	}

	free(linea);

	if (hay_error) {
		return ERROR;
	}
	return EXITO;
}

/**
 * Lee un archivo de pokemon según especificado en el enunciado y devuelve un
 * TP1 con dichos pokemon.
 *
 * En caso de error devuelve NULL
 */
tp1_t *tp1_leer_archivo(const char *nombre)
{
	if (!nombre) {
		printf("Se debe especificar un nombre de archivo.\n");
		return NULL;
	}

	FILE *archivo = fopen(nombre, LECTURA);
	if (!archivo) {
		printf("Hubo un problema al abrir el archivo.\n");
		return NULL;
	}

	tp1_t *tp1 = malloc(sizeof(tp1_t));
	if (!tp1) {
		fclose(archivo);
		printf("Hubo un error al reservar memoria para la estructura tp1.\n");
		return NULL;
	}

	tp1->cantidad = 0;
	tp1->capacidad = CAPACIDAD_INICIAL;
	tp1->pokemones = malloc(sizeof(struct pokemon) * (tp1->capacidad));
	if (!tp1->pokemones) {
		printf("Hubo un error al reservar memoria para el vector de pokemones.\n");
		fclose(archivo);
		free(tp1);
		return NULL;
	}

	int resultado = guardar_pokemones(archivo, tp1);
	fclose(archivo);

	if (resultado == ERROR) {
		printf("Hubo un error al reservar memoria para el buffer que permite leer "
		       "linea.\n");
		free(tp1);
		return NULL;
	}

	//ordenar_vector(tp1->pokemones, tp1->cantidad);

	return tp1;
}

/**
 * Devuelve la cantidad de pokemon leidos en este TP1
 *
 * En caso de error devuelve 0
 *
 */
size_t tp1_cantidad(tp1_t *tp1)
{
	if (tp1 == NULL) {
		return 0;
	}
	return tp1->cantidad;
}

/**
 * Crea un TP1 con la combinación de los pokemon de ambos TPs. En caso de
 * duplicados se toma el pokemon del primer TP1.
 *
 * Devuelve un nuevo TP1 con la combinación de ambos TP1 o NULL en caso de
 * error.
 *
 */
tp1_t *tp1_combinar(tp1_t *tp1_a, tp1_t *tp1_b)
{
	if (!tp1_a || !tp1_b)
		return NULL;

	size_t i = 0, j = 0, cantidad_actual = 0;
	bool hubo_error = false;

	tp1_t *aux = malloc(sizeof(tp1_t));
	if (!aux) {
		printf("Hubo un error al reservar memoria para la estructura tp1.\n");
		return NULL;
	}

	size_t capacidad_minima = tp1_a->cantidad + tp1_b->cantidad;
	aux->pokemones = malloc(sizeof(struct pokemon) * capacidad_minima);
	if (!aux->pokemones) {
		printf("Hubo un error al reservar memoria para los pokemones del nuevo "
		       "TP1.\n");
		free(aux);
		return NULL;
	}
	aux->capacidad = capacidad_minima;

	while (i < tp1_a->cantidad && j < tp1_b->cantidad && !hubo_error) {
		int comparacion = strcasecmp(tp1_a->pokemones[i].nombre,
					     tp1_b->pokemones[j].nombre);
		int resultado;
		bool sumar_i = false, sumar_j = false;

		if (comparacion == 0) {
			resultado = duplicar_pokemon(
				tp1_a->pokemones[i],
				&aux->pokemones[cantidad_actual]);
			sumar_i = true;
			sumar_j = true;
		} else if (comparacion > 0) {
			resultado = duplicar_pokemon(
				tp1_b->pokemones[j],
				&aux->pokemones[cantidad_actual]);
			sumar_j = true;
		} else if (comparacion < 0) {
			resultado = duplicar_pokemon(
				tp1_a->pokemones[i],
				&aux->pokemones[cantidad_actual]);
			sumar_i = true;
		}

		if (resultado == EXITO) {
			if (sumar_i)
				i++;
			if (sumar_j)
				j++;
			cantidad_actual++;
		} else {
			hubo_error = true;
		}
	}

	while (i < tp1_a->cantidad && !hubo_error) {
		int resultado = duplicar_pokemon(
			tp1_a->pokemones[i], &aux->pokemones[cantidad_actual]);
		if (resultado == EXITO) {
			i++;
			cantidad_actual++;
		} else {
			hubo_error = true;
		}
	}

	while (j < tp1_b->cantidad && !hubo_error) {
		int resultado = duplicar_pokemon(
			tp1_b->pokemones[j], &aux->pokemones[cantidad_actual]);
		if (resultado == EXITO) {
			j++;
			cantidad_actual++;
		} else {
			hubo_error = true;
		}
	}

	aux->cantidad = cantidad_actual;

	if (hubo_error) {
		for (size_t i = 0; i < aux->cantidad; i++) {
			free(aux->pokemones[i].nombre);
		}
		free(aux->pokemones);
		free(aux);
		return NULL;
	}

	return aux;
}

/**
 * Guarda el TP1 a un archivo de texto.
 *
 * Devuelve el mismo TP1 o NULL en caso de error. Si el archivo ya existe se
 * sobreescribe.
 *
 */
tp1_t *tp1_escribir_archivo(tp1_t *tp1, const char *nombre)
{
	if (tp1 == NULL || nombre == NULL) {
		printf("Se debe enviar un TP1 y un nombre valido.\n");
		return NULL;
	}

	FILE *archivo = fopen(nombre, ESCRITURA);
	if (!archivo) {
		printf("Hubo un problema al abrir el archivo.\n");
		return NULL;
	}

	for (size_t i = 0; i < tp1->cantidad; i++) {
		struct pokemon actual = tp1->pokemones[i];
		char rareza = convertir_rareza_a_caracter(actual.rareza);

		fprintf(archivo, FORMATO_ESCRITURA, actual.nombre,
			actual.velocidad, actual.peso, rareza);
	}

	fclose(archivo);

	return tp1;
}

/**
 * Busca un pokemon por nombre dentro del TP1 y lo devuelve.
 *
 * En caso de error devuelve NULL
 */
struct pokemon *tp1_buscar_pokemon(tp1_t *tp1, const char *nombre)
{
	if (!tp1 || !nombre)
		return NULL;

	size_t i = 0;
	bool encontrado = false;

	while (i < tp1->cantidad && !encontrado) {
		if (strcasecmp(tp1->pokemones[i].nombre, nombre) == 0) {
			encontrado = true;
		}
		i++;
	}

	if (!encontrado) {
		return NULL;
	}

	return &tp1->pokemones[i - 1];
}

/**
 * Busca el n-esimo pokemon del TP1 (ordenados por orden alfabético)
 *
 * En caso de error devuelve NULL
 */
struct pokemon *tp1_buscar_orden(tp1_t *tp1, size_t n)
{
	if (tp1 == NULL || n >= tp1->cantidad) {
		return NULL;
	}

	return &tp1->pokemones[n];
}

/**
 * Recorre los pokemon del TP1 aplicando la función f a cada uno. Si f devuelve
 * true, sigue iterando.
 *
 * Devuelve la cantidad de veces que se invocó f.
 */
size_t tp1_iterar(tp1_t *tp1, bool (*f)(struct pokemon *, void *), void *extra)
{
	size_t contador = 0;

	bool seguir = true;

	while (contador < tp1->cantidad && seguir) {
		seguir = f(&tp1->pokemones[contador], extra);
		contador++;
	}

	return contador;
}

/**
 * Destruye el TP1 y toda la memoria asociada
 *
 */
void *tp1_destruir(tp1_t *tp1)
{
	if (tp1 == NULL) {
		printf("Se envio un puntero a NULL.\n");
		return NULL;
	}

	for (size_t i = 0; i < tp1->cantidad; i++) {
		free(tp1->pokemones[i].nombre);
	}
	free(tp1->pokemones);
	free(tp1);
	return NULL;
}
