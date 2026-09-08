#include "src/tp1.h"

const int CANTIDAD_MINIMA_PARAMETROS = 3;

int main(int argc, char const *argv[]) {
  if (argc > CANTIDAD_MINIMA_ARGUMENTOS) {
    printf("Se deben ingresar como minimo 3 parametros para el correcto "
           "funcionamiento del programa.\n Uso: ./ejecutable <archivo_a_leer> "
           "<funcion_a_ejecutar> <extras>\n");
    return ERROR;
  }

  if (argc == 4) {
    tp1_t *pokedex = tp1_leer_archivo(argv[2]);
    if (pokedex == NULL) {
      return ERROR;
    }

    struct pokemon *buscado = tp1_buscar_pokemon(pokedex, argv[3]);
    if (!buscado) {
      printf("Hubo un error al encontrar el pokemon buscado\n");
    } else {
      printf("El pokemon buscado es: \n");
      printf("Nombre: %s\n", buscado.nombre);
      printf("Velocidad: %i\n", buscado.velocidad);
      printf("Peso: %f\n", buscado.peso);
      printf("Rareza: %s\n", buscado.rareza);
    }
  }
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
-> Se podria utilizar la funcion de puntero a funcion para ir imprimiendo por
pantalla el tp1 que ya viene ordenado desde tp1_leer_archivo()



./tp1 pokedex.csv mostrar-rarezas: Muestra todos los pokemon agrupados por
rareza. ->
*/