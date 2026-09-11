#include "src/tp1.h"
#include <time.h>
#include <string.h>

#define MOSTRAR_TODO "mostrar-todo"
#define MOSTRAR_UNO "mostrar-uno"
#define MOSTRAR_RAREZAS "mostrar-rarezas"
#define BUSCAR_NOMBRE "buscar-nombre"

const int ERROR = -1;
const int CANTIDAD_MINIMA_PARAMETROS = 3;
const int CANTIDAD_BUSCAR_NOMBRE = 4;


void imprimir_pokemon(struct pokemon *actual, char rareza) {
        printf("Nombre: %s\n", actual->nombre);
        printf("Velocidad: %i\n", actual->velocidad);
        printf("Peso: %f\n", actual->peso);
        printf("Rareza: %c\n", rareza);
        printf("\n");
}


bool tiene_cierta_rareza(struct pokemon *p, void *extra) {
  if(!p) {
    return false;
  }

  if (p->rareza == *((enum rareza_pokemon *)extra)){
    char rareza = convertir_rareza_a_caracter(p->rareza);
    imprimir_pokemon(p, rareza);
  }
  return true;
}

int main(int argc, char const *argv[]) {
  if (argc < CANTIDAD_MINIMA_PARAMETROS) {
    printf("Se deben ingresar como minimo 3 parametros para el correcto "
           "funcionamiento del programa.\n Uso: ./ejecutable <archivo_a_leer> "
           "<funcion_a_ejecutar> <extras>\n");
    return ERROR;
  }

  srand((unsigned int)time(NULL));

  tp1_t *pokedex = tp1_leer_archivo(argv[1]);
  if (pokedex == NULL) {
    return ERROR;
  }

  if (argc >= CANTIDAD_MINIMA_PARAMETROS) {
    if(strcmp(argv[2], MOSTRAR_UNO) == 0) {
      if(tp1_cantidad(pokedex) > 0) {
        size_t indice_random = rand() % tp1_cantidad(pokedex);

        struct pokemon *actual = tp1_buscar_orden(pokedex, indice_random);
        if (!actual) {
            printf("Hubo un error al imprimir el pokemon\n");
        } else {
          char rareza = convertir_rareza_a_caracter(actual->rareza);
          printf("El pokemon N: %zu es: \n", indice_random);
          imprimir_pokemon(actual, rareza);
        }
      }
      
    } else if (strcmp(argv[2], MOSTRAR_TODO) == 0) {
      size_t cantidad = tp1_cantidad(pokedex);

      for(size_t i = 0; i < cantidad; i++) {
        struct pokemon *actual = tp1_buscar_orden(pokedex, i);
        if (!actual) {
          printf("Hubo un error al imprimir el pokemon\n");
        } else {
          char rareza = convertir_rareza_a_caracter(actual->rareza);
          printf("El pokemon N: %zu es: \n", i);
          imprimir_pokemon(actual, rareza);
        }
      }

    }  else if (strcmp(argv[2], MOSTRAR_RAREZAS) == 0) {
      enum rareza_pokemon rareza_comun = RAREZA_COMUN;
      enum rareza_pokemon rareza_raro = RAREZA_RARO;
      enum rareza_pokemon rareza_legendaria = RAREZA_LEGENDARIO;

      size_t cantidad_rareza_comun = tp1_iterar(pokedex, tiene_cierta_rareza, &rareza_comun);

      size_t cantidad_rareza_raro = tp1_iterar(pokedex, tiene_cierta_rareza, &rareza_raro);

      size_t cantidad_rareza_legendario = tp1_iterar(pokedex, tiene_cierta_rareza, &rareza_legendaria);


      printf("Cantidad de pokemones comunes %zu, de pokemones raros %zu, de pokemones legendarios %zu .\n", cantidad_rareza_comun, cantidad_rareza_raro, cantidad_rareza_legendario);

    } else if (strcmp(argv[2], BUSCAR_NOMBRE) == 0 && argc == CANTIDAD_BUSCAR_NOMBRE) {
      struct pokemon *buscado = tp1_buscar_pokemon(pokedex, argv[3]);
      if (!buscado) {
        printf("Hubo un error al encontrar el pokemon buscado\n");
      } else {
        char rareza = convertir_rareza_a_caracter(buscado->rareza);
        printf("El pokemon buscado es: \n");
        imprimir_pokemon(buscado, rareza);
      }
    } else {
      printf("Los comandos validos son: 'mostrar-todo', 'mostrar-uno', 'mostrar-rarezas', 'buscar-nombre' <nombre_a_buscar>.\n Por favor ingrese uno valido\n");
      return ERROR;
    }
  }

  tp1_destruir(pokedex);
  return 0;
}

/*
./tp1 pokedex.csv buscar-nombre magikarp: Busca un pokemon con el nombre
magikarp y muestra sus datos por pantalla. -> buscar_nombre()


./tp1 pokedex.csv mostrar-uno: Busca un pokemon al azar y lo muestra por
pantalla con sus datos -> Se puede implementar una funcion que pida la cantidad
total, y desde 0 hasta esa cantidad generar un random y ese numero random
pasarselo al buscar n esimo()


./tp1 pokedex.csv mostrar-todo: Muestra todos los pokemon por orden alfabetico.
-> Se podria utilizar la funcion de buscar_n para ir recorriendo haciendo un bucle por la cantidad de pokemones que tenga 



./tp1 pokedex.csv mostrar-rarezas: Muestra todos los pokemon agrupados por
rareza. -> Funcion iteradora
*/