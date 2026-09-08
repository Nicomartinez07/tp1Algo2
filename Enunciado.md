<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP1: Archivos, strings y punteros

Se pide implementar la funcionalidad del archivo **.h** provisto según la descripción presente en el mismo y lo charlado en clase.

Adicionalmente se pide la implementación de ***main.c*** que dado un archivo de pokedex y otros parámetros, cargue los datos y ejecute la operación pedida.

Por último, es **obligatorio** implementar ***pruebas_alumno.c*** con pruebas de la funcionalidad de ***tp1.h***.

## Estructura del archivo

El archivo a leer tiene un formato como se muestra a continuación:

```csv
Magikarp,10,5.2,C
Starmie,40,4.7,C
Lapras,30,50.3,R
Azumarill,50,10,C
Suicune,70,80,L
```

Donde la primer columna es el nombre del pokemon, luego le sigue la velocidad (entero del 1 al 99), el peso (float positivo mayor a 0) y la rareza.

Las rarezas posibles son las siguientes:

| Código | Tipo       |
|--------|------------|
| C      | Común      |
| R      | Raro       |
| L      | Legendario |

## Respecto de los pokemones repetidos

Al cargar un archivo, si se encuentran pokemones repetidos, se deben conservar solamente uno de los pokemones (el primero encontrado en el archivo). La detección de repetidos se realiza en base al nombre del pokemon.

En lo que respecta a este trabajo, los nombres de los pokemon no distinguen diferencias entre mayúsculas y minúsculas (pikachu, Pikachu o PIKACHU son el mismo pokemon).

## Ejemplos de uso del main a implementar

  - **./tp1 pokedex.csv buscar-nombre magikarp**: Busca un pokemon con el nombre magikarp y muestra sus datos por pantalla.
  - **./tp1 pokedex.csv mostrar-uno**: Busca un pokemon al azar y lo muestra por pantalla con sus datos
  - **./tp1 pokedex.csv mostrar-todo**: Muestra todos los pokemon por orden alfabetico.
  - **./tp1 pokedex.csv mostrar-rarezas**: Muestra todos los pokemon agrupados por rareza.

---

## Consejos para la elaboración del trabajo

**LEER EL ENUNCIADO** primero y luego **la descripción de las funciones**. Importantísimo. 

**LEER EL ENUNCIADO** primero y luego **la descripción de las funciones**. Importantísimo. 

**LEER EL ENUNCIADO** primero y luego **la descripción de las funciones**. Importantísimo. 

**LEER EL ENUNCIADO** primero y luego **la descripción de las funciones**. Importantísimo. 

**LEER EL ENUNCIADO** primero y luego **la descripción de las funciones**. Importantísimo. 

**LEER EL ENUNCIADO** primero y luego **la descripción de las funciones**. Importantísimo. 

**LEER EL ENUNCIADO** primero y luego **la descripción de las funciones**. Importantísimo. 

**LEER EL ENUNCIADO** primero y luego **la descripción de las funciones**. Importantísimo. 

**LEER EL ENUNCIADO** primero y luego **la descripción de las funciones**. Importantísimo. 

**LEER EL ENUNCIADO** primero y luego **la descripción de las funciones**. Importantísimo. 

**LEER EL ENUNCIADO** primero y luego **la descripción de las funciones**. Importantísimo. 

**LEER EL ENUNCIADO** primero y luego **la descripción de las funciones**. Importantísimo. 

**LEER EL ENUNCIADO** primero y luego **la descripción de las funciones**. Importantísimo. 


**NO** escribas código a lo loco sin compilar cada tanto. Implementá la solución de a poco y compilando a cada paso. Dejar la compilación para el final es uno de los peores errores que podés cometer.

**NO** avances en la implementación si quedan errores sin resolver en alguna prueba. Cada vez que escribas una prueba implementá toda la funcionalidad necesaria para que funcione correctamente. Esto incluye liberar memoria y accesos inválidos a la misma. Solamente una vez que hayas logrado que la prueba pase exitosamente es que podés comenzar a escribir la próxima prueba para continuar el trabajo.

**NO** está permitido modificar los archivos **.h** provistos con el enunciado.

---

## Puntos teóricos

 - Explicar la elección de la estructura para implementar la funcionalida pedida. Justifique el uso de cada uno de los campos de la estructura.
 - Dar una definición de complejidad computacional y explique cómo se calcula.
 - Explicar con diagramas cómo quedan dispuestas las estructuras y elementos en memoria.
 - Justificar la complejidad computacional temporal de **cada una** de las funciones que se piden implementar.
 - Explique qué dificultades tuvo para implementar las funcionalidades pedidas en el main (si tuvo alguna) y explique si alguna de estas dificultades se podría haber evitado modificando la definición del .h

## Restricciones

Para este TP **NO** se permiten utilizar las funciones de la biblioteca estándar de ***C*** **qsort**, **bsearch** y **strtok**. Si se requieren dichas funcionalidades deben ser implementadas por el alumno.

Tampoco se permite presuponer un longitúd máxima de línea, nombres, vectores, etc. Debe utilizarse memoria dinámica.

Al leer un archivo con líneas inválidas, dichas líneas se deben saltear e ignorar para continuar leyendo el resto de las líneas.
