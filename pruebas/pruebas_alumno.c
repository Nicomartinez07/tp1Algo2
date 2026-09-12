#include "pa2m.h"
#include "pruebas.h"

/**
 * Main: Conjunto de grupos de pruebas
 */

int main() {
  pa2m_nuevo_grupo("============== PRUEBAS DEL TP X ===============");

  pa2m_nuevo_grupo("Pruebas de leer_linea");
  pruebas_leer_linea();

  pa2m_nuevo_grupo("Pruebas de buscar");
  pruebas_buscar();

  pa2m_nuevo_grupo("Pruebas de combinar");
  pruebas_combinar();

  pa2m_nuevo_grupo("Pruebas de archivos");
  pruebas_archivos();

  return pa2m_mostrar_reporte();
}
