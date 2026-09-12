usuario ejecuta programa con archivo y X funcion.

El programa leer el archivo, lo baja a un vector de pokemones, y de ahi ejecuta la funcion solicitada.

Proceso de guardado del vector: Abre el archivo en formato lectura, lee linea -> valida linea (se fija si el pokemon no esta repetido) 
-> Si esta, lo ignora, sino lo agrega al vector.

Funciones necesarias:

ordenar vector en orden alfabetico -> Ordenar el vector por nombre



/**
 * Lee un archivo de pokemon según especificado en el enunciado y devuelve un TP1 con dichos pokemon.
 * En caso de error devuelve NULL
 */
tp1_t *tp1_leer_archivo(const char *nombre):



/**
 * Devuelve la cantidad de pokemon leidos en este TP1
 *
 * En caso de error devuelve 0
 *
 */
size_t tp1_cantidad(tp1_t *tp1):





/**
 * Crea un TP1 con la combinación de los pokemon de ambos TPs. En caso de
 * duplicados se toma el pokemon del primer TP1.
 *
 * Devuelve un nuevo TP1 con la combinación de ambos TP1 o NULL en caso de
 * error.
 *
 */
tp1_t *tp1_combinar(tp1_t *tp1_a, tp1_t *tp1_b);

/**
 * Guarda el TP1 a un archivo de texto.
 *
 * Devuelve el mismo TP1 o NULL en caso de error. Si el archivo ya existe se
 * sobreescribe.
 *
 */
tp1_t *tp1_escribir_archivo(tp1_t *tp1, const char *nombre);

/**
 * Busca un pokemon por nombre dentro del TP1 y lo devuelve.
 *
 * En caso de error devuelve NULL
 */
struct pokemon *tp1_buscar_pokemon(tp1_t *tp1, const char *nombre);




/**
 * Busca el n-esimo pokemon del TP1 (ordenados por orden alfabético)
 *
 * En caso de error devuelve NULL
 */
struct pokemon *tp1_buscar_orden(tp1_t *tp1, size_t n): Itera los pokemones 



/**
 * Recorre los pokemon del TP1 aplicando la función f a cada uno. Si f devuelve
 * true, sigue iterando.
 *
 * Devuelve la cantidad de veces que se invocó f.
 */
size_t tp1_iterar(tp1_t *tp1, bool (*f)(struct pokemon *, void *), void *extra): 




/**
 * Destruye el TP1 y toda la memoria asociada
 *
 */
void *tp1_destruir(tp1_t *tp1):

Libera los pokemones, establece todo a 0, y posteriormente libera el TP1







  - **./tp1 pokedex.csv buscar-nombre magikarp**: Busca un pokemon con el nombre magikarp y muestra sus datos por pantalla. -> Llamo a la funcion pokemon buscar pasandole como parametro el 4to parametro enviado en el argv


  - **./tp1 pokedex.csv mostrar-uno**: Busca un pokemon al azar y lo muestra por pantalla con sus datos -> Utilizo srand y rand para generar un numero aleatorio desde 0 hasta la cantidad de elementos que tenga en el tp1, y posteriormente llamo a la funcion buscar_orden (n) 


  - **./tp1 pokedex.csv mostrargv[2]ar-todo**: Muestra todos los pokemon por orden alfabetico. -> Utilizo la funcion para q lea un archivo, y dsp imprimo por pantalla todos los pokemones aprovechando que ya esa funcion me lo devuelve ordenado


  - **./tp1 pokedex.csv mostrar-rarezas**: Muestra todos los pokemon agrupados por rareza. -> Utilizo la funcion que recibe como parametro un puntero a funcion, como condicion hago 3 llamadas como me explicabas vos arriba


