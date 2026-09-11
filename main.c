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

void imprimir_pokemon(struct pokemon *actual) {
        printf("Nombre: %s\n", actual->nombre);
        printf("Velocidad: %i\n", actual->velocidad);
        printf("Peso: %f\n", actual->peso);
        printf("Rareza: %s\n", actual->rareza);
}

int main(int argc, char const *argv[]) {
  if (argc < CANTIDAD_MINIMA_PARAMETROS) {
    printf("Se deben ingresar como minimo 3 parametros para el correcto "
           "funcionamiento del programa.\n Uso: ./ejecutable <archivo_a_leer> "
           "<funcion_a_ejecutar> <extras>\n");
    return ERROR;
  }

  srand((unsigned int)time(NULL));

  //Validar 3 variables minimas, generar pokedex, if en base a comandos, si es 3, si es 4, tp1_destruir

  tp1_t *pokedex = tp1_leer_archivo(argv[1]);
  if (pokedex == NULL) {
    return ERROR;
  }

  if (argc >= 3) {
    if(strcmp(argv[2], MOSTRAR_UNO) == 0) {
      if(pokedex->cantidad > 0) {
        size_t indice_random = rand() % tp1_cantidad(pokedex);

        struct pokemon *actual = tp1_buscar_orden(pokedex, indice_random);
        if (!actual) {
            printf("Hubo un error al imprimir el pokemon\n");
        } else {
          printf("El pokemon N: %zu es: \n", indice_random);
          imprimir_pokemon(actual);
        }
      }
      
    } else if (strcmp(argv[2], MOSTRAR_TODO) == 0) {
      size_t cantidad = tp1_cantidad(pokedex);

      for(size_t i = 0; i < cantidad; i++) {
        struct pokemon *actual = tp1_buscar_orden(pokedex, i);
        if (!actual) {
          printf("Hubo un error al imprimir el pokemon\n");
        } else {
          printf("El pokemon N: %zu es: \n", i);
          imprimir_pokemon(actual);
        }
      }

    }  else if (strcmp(argv[2], MOSTRAR_RAREZAS) == 0) {

    } else if (strcmp(argv[2], BUSCAR_NOMBRE) == 0 && argc == 4) {
      struct pokemon *buscado = tp1_buscar_pokemon(pokedex, argv[3]);
      if (!buscado) {
        printf("Hubo un error al encontrar el pokemon buscado\n");
      } else {
        printf("El pokemon buscado es: \n");
        imprimir_pokemon(buscado);
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