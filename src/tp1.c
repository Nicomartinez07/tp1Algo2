#include "tp1.h"
#include "../src/leer_linea.h"
#include "../src/lista.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#define LECTURA "r"
#define ESCRITURA "w"
#define FORMATO_LECTURA_SIN_NOMBRE "%d,%f,%c\n"
#define FORMATO_ESCRITURA "%s,%i,%f,%c\n"

#define RARO 'R'
#define COMUN 'C'
#define LEGENDARIO 'L'

struct tp1 {
    lista_t *pokemones;
};

const size_t CAPACIDAD_INICIAL = 10;
const char CARACTER_CERO_LITERAL = '\0';
const char CARACTER_COMA = ',';
const char CARACTER_SALTO_LINEA = '\n';
const char CARACTER_RETORNO = '\r';
const int ERROR = -1;
const int EXITO = 0;
const int PESO_MINIMO = 0;
const float VELOCIDAD_MINIMA = 1;
const int VELOCIDAD_MAXIMA = 99;
const int CANTIDAD_LEIDOS_POR_LINEA = 3;

/*
 * Libera la memoria de cada pokemon y su nombre
 */
void destruir_cada_pokemon(void *elemento)
{
    struct pokemon *pokemon = elemento;

    free(pokemon->nombre);
    free(pokemon);
}

/*
 * Realiza una comparacion entre dos strings sin distinguir si estan escritos en mayuscula o miniscula.
 */
int comparar_textos_sin_distinguir(char *texto1, char *texto2) {
	 while (*texto1 && *texto2 && tolower((unsigned char)*texto1) == tolower((unsigned char)*texto2)) {
        texto1++;
        texto2++;
    }

	return tolower((unsigned char)*texto1) - tolower((unsigned char)*texto2);
}

/*
 * Hace la comparacion entre el nombre buscado y el pokemon.
 */
int comparar_nombres(void *elemento, void *buscado) {
	struct pokemon *pokemon = elemento;
  	char *nombre = buscado;

	return comparar_textos_sin_distinguir(pokemon->nombre, nombre);
}

/*
 * Genera una copia del pokemon original en la copia. Se reserva el espacio
 * necesario para el nombre del pokemon.
 *
 * En caso de error devuelve ERROR
 */
struct pokemon *duplicar_pokemon(struct pokemon original)
{
	struct pokemon *copia = malloc(sizeof(struct pokemon));
	if(!copia) {
		return NULL;
	}

	size_t longitud_nombre = strlen(original.nombre);

	copia->nombre = malloc((longitud_nombre + 1) * sizeof(char));
	if (!copia->nombre) {
		free(copia);
		return NULL;
	}

	strcpy(copia->nombre, original.nombre);

	copia->peso = original.peso;
	copia->rareza = original.rareza;
	copia->velocidad = original.velocidad;

	return copia;
}

/*
 * Devuelve true si puede insertar en la lista, una copia del pokemon original.
 */
bool insertar_copia(lista_t *lista, struct pokemon *original)
{
    struct pokemon *copia = duplicar_pokemon(*original);

    if (!copia)
        return false;

    if (!lista_insertar(lista, copia, lista_cantidad(lista))) {
        free(copia->nombre);
        free(copia);
        return false;
    }

    return true;
}

/*
 * Busca una posicion en pokemones para insertar posteriormente el nuevo pokemon. La comparacion se realiza mediante el nombre.
 * En caso de que la encuentre devuelve la posicion y true en el parametro encontrado.
 * 
 * En caso de no encontrar la posicion, devuelve false en encontrado.
 */
size_t buscar_posicion_para_insertar(lista_t *pokemones, size_t cantidad,
				     char *nombre, bool *encontrado)
{
	size_t inicio = 0;
	size_t fin = cantidad;
	size_t posicion = 0;
	bool encontro_pokemon = false;

	while (inicio < fin && !encontro_pokemon) {
		size_t centro = inicio + (fin - inicio) / 2;
		struct pokemon *actual = lista_obtener(pokemones, centro);	
		int comparacion = comparar_textos_sin_distinguir(actual->nombre, nombre);

		if (comparacion == 0) {
			*encontrado = true;
			encontro_pokemon = true;
			posicion = centro;
		} else if (comparacion < 0) {
			inicio = centro + 1;
		} else {
			fin = centro;
		}
	}

	if(!encontro_pokemon) {
		*encontrado = false;
		posicion = inicio;
	}
	return posicion;
}

/*
 * Devuelve true si el pokemon tiene un peso mayor a PESO_MINIMO, y su velocidad esta entre VELOCIDAD_MINIMA y VELOCIDAD_MAXIMA.
 *
 * Devuelve false en caso contrario.
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

/**
 * Realiza una busqueda del pokemon, en caso de no encontrarlo procede a insertarlo. Si lo encuentra libera la memoria asociada
 * Devuelve false en caso de error, true en caso contrario. 
 */
bool insertar_pokemon_si_no_existe(lista_t *lista,
                                   struct pokemon *pokemon)
{
    bool encontro = false;

    size_t posicion = buscar_posicion_para_insertar(
        lista,
        lista_cantidad(lista),
        pokemon->nombre,
        &encontro
    );

    if (encontro) {
        free(pokemon->nombre);
        free(pokemon);
        return true;
    }

    if (!lista_insertar(lista, pokemon, posicion)) {
        free(pokemon->nombre);
        free(pokemon);
        return false;
    }

    return true;
}

/**
 * Parsea la linea obteniendo el nombre del pokemon. En el puntero **coma, devuelve un puntero a la posicion donde esta
 * posicionada la coma en la linea.
 */
char *obtener_nombre_pokemon(char *linea, char **coma) {
	*coma = strchr(linea, CARACTER_COMA);
		
 	if (!*coma) return NULL;
	size_t longitud_nombre = (size_t)(*coma - linea);

	if (longitud_nombre == 0) return NULL;

	char *nombre_pokemon = malloc((longitud_nombre + 1) * sizeof(char));
	if (!nombre_pokemon) return NULL;

	memcpy(nombre_pokemon, linea, longitud_nombre);
	nombre_pokemon[longitud_nombre] = CARACTER_CERO_LITERAL;

	return nombre_pokemon;
}

/**
 * Obtiene el nombre del pokemon al parsear la linea, y verifica que el resto de los datos del pokemon sean validos.
 * Devuelve el pokemon obtenido de esa linea, NULL en caso de error.
 */
struct pokemon *generar_pokemon_a_partir_de_linea(char* linea) {
	if(!linea) return NULL;

	char *coma;
	char* nombre_pokemon = obtener_nombre_pokemon(linea, &coma);

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
		return NULL;
	}

	char fin = (coma + 1)[caracteres_leidos];
    bool sin_basura = (fin == CARACTER_CERO_LITERAL || fin == CARACTER_SALTO_LINEA || fin == CARACTER_RETORNO);

	enum rareza_pokemon rareza = 0;
	bool rareza_valida = convertir_caracter_a_rareza(caracter_rareza, &rareza);
	if(!rareza_valida || !sin_basura) {
		free(nombre_pokemon);
		return NULL;
	}

	struct pokemon *actual = malloc(sizeof(struct pokemon));
	if(!actual) {
		free(nombre_pokemon);
		return NULL;
	}

	actual->nombre = nombre_pokemon;
	actual->peso = peso;
	actual->velocidad = velocidad;
	actual->rareza = rareza;

	if(!pokemon_valido(*actual)) {
		free(nombre_pokemon);
		free(actual);
		return NULL;
	}

	return actual;
}

/*
 * El archivo tiene que estar abierto en modo lectura. Lee el archivo y guarda
 * los pokemones en la estructura. 
 * 
 * Devuelve ERROR en caso de error.
 */
int guardar_pokemones(FILE *archivo, tp1_t *estructura)
{
	 bool hay_error = false;

    size_t tamaño_inicial = CAPACIDAD_INICIAL;
    char *linea = malloc(tamaño_inicial);

    if (!linea)
        return ERROR;

    while (leer_linea_ptr(&linea, &tamaño_inicial, archivo) >= 0 &&
           !hay_error) {
        struct pokemon *pokemon =
            generar_pokemon_a_partir_de_linea(linea);

        if (pokemon != NULL) {
            if (!insertar_pokemon_si_no_existe(
                    estructura->pokemones, pokemon)) {
                hay_error = true;
            }
        }
    }
	free(linea);

	if (hay_error) return ERROR;
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
		return NULL;
	}

	FILE *archivo = fopen(nombre, LECTURA);
	if (!archivo) {
		return NULL;
	}

	tp1_t *tp1 = malloc(sizeof(tp1_t));
	if (!tp1) {
		fclose(archivo);
		return NULL;
	}

	tp1->pokemones = lista_crear();
	if (!tp1->pokemones) {
		fclose(archivo);
		free(tp1);
		return NULL;
	}

	int resultado = guardar_pokemones(archivo, tp1);
	fclose(archivo);

	if (resultado == ERROR) {
		lista_destruir_todo(tp1->pokemones, destruir_cada_pokemon);
		free(tp1);
		return NULL;
	}

	return tp1;
}

/**
 * Devuelve la cantidad de pokemon leidos en este TP1
 *
 * En caso de error devuelve 0
 */
size_t tp1_cantidad(tp1_t *tp1)
{
	if (!tp1)
		return 0;
	return lista_cantidad(tp1->pokemones);
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

	tp1_t *aux = malloc(sizeof(tp1_t));
	if (!aux) {
		return NULL;
	}

	aux->pokemones = lista_crear();
	if (!aux->pokemones) {
		free(aux);
		return NULL;
	}

	size_t i = 0, j = 0;
	bool hubo_error = false;

	while (i < lista_cantidad(tp1_a->pokemones) && j < lista_cantidad(tp1_b->pokemones) && !hubo_error) {
		struct pokemon *pokemon_a = lista_obtener(tp1_a->pokemones, i);	
		struct pokemon *pokemon_b = lista_obtener(tp1_b->pokemones, j);
		int comparacion = comparar_textos_sin_distinguir(pokemon_a->nombre,
					     			 pokemon_b->nombre);

		if (comparacion == 0) {
			hubo_error = !insertar_copia(aux->pokemones, pokemon_a);
            j++;
			i++;
		}
		else if (comparacion < 0) {
            hubo_error = !insertar_copia(aux->pokemones, pokemon_a);
            i++;
        } else {
            hubo_error = !insertar_copia(aux->pokemones, pokemon_b);
            j++;
        }
    }

    while (i < lista_cantidad(tp1_a->pokemones) && !hubo_error) {
        struct pokemon *actual = lista_obtener(tp1_a->pokemones, i);
        hubo_error = !insertar_copia(aux->pokemones, actual);
        i++;
    }

    while (j < lista_cantidad(tp1_b->pokemones) && !hubo_error) {
        struct pokemon *actual = lista_obtener(tp1_b->pokemones, j);
        hubo_error = !insertar_copia(aux->pokemones, actual);
        j++;
    }

	if (hubo_error) {
		lista_destruir_todo(aux->pokemones, destruir_cada_pokemon);
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
	if (!tp1 || !nombre) {
		return NULL;
	}

	FILE *archivo = fopen(nombre, ESCRITURA);
	if (!archivo) {
		return NULL;
	}

	for (size_t i = 0; i < lista_cantidad(tp1->pokemones); i++) {
		struct pokemon *actual = lista_obtener(tp1->pokemones, i);	
		char rareza = convertir_rareza_a_caracter(actual->rareza);

		fprintf(archivo, FORMATO_ESCRITURA, actual->nombre,
			actual->velocidad, actual->peso, rareza);
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

	struct pokemon *encontrado = NULL;

	int resultado = lista_buscar(tp1->pokemones, (void*)nombre, comparar_nombres, (void**)&encontrado);

	if (resultado == ERROR) {
		return NULL;
	}

	return encontrado;
}

/**
 * Busca el n-esimo pokemon del TP1 (ordenados por orden alfabético)
 *
 * En caso de error devuelve NULL
 */
struct pokemon *tp1_buscar_orden(tp1_t *tp1, size_t n)
{
	if (tp1 == NULL) {
		return NULL;
	}

	return lista_obtener(tp1->pokemones, n);
}

/*
 * Recorre los pokemon del TP1 aplicando la función f a cada uno. Si f devuelve
 * true, sigue iterando.
 *
 * Devuelve la cantidad de veces que se invocó f. 
 */
size_t tp1_iterar(tp1_t *tp1, bool (*f)(struct pokemon *, void *), void *extra)
{
	if (!tp1 || !f)
		return 0;
	size_t contador = 0;

	bool seguir = true;

	while (contador < lista_cantidad(tp1->pokemones) && seguir) {
		seguir = f(lista_obtener(tp1->pokemones, contador), extra);
		contador++;
	}

	return contador;
}

/*
 * Destruye el TP1 y toda la memoria asociada
 *
 */
void *tp1_destruir(tp1_t *tp1)
{
	if (!tp1) 
		return NULL;

	lista_destruir_todo(tp1->pokemones, destruir_cada_pokemon);
	free(tp1);
	return NULL;
}
