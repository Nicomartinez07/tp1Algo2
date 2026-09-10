#ifndef PRUEBAS_H_
#define PRUEBAS_H_

/**
 * Interfaz de las pruebas: declara la función de entrada de las pruebas para 
 * cada TDA. Cada pruebas_tda_x.c la implementa y se compila por separado; 
 * pruebas_alumno.c las invoca. El enlazador une todo.
 *
 * Para agregar un TDA: crear pruebas/pruebas_tda_x.c que incluya este header
 * e implemente pruebas_tda_x(), declararla abajo e invocarla desde main().
 */

void pruebas_leer_linea();

void pruebas_buscar();

void pruebas_combinar();

#endif // PRUEBAS_H_