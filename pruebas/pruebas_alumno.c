#include "pa2m.h"
#include "pruebas.h"

/**
 * Main: Conjunto de grupos de pruebas
 */

int main()
{
	pa2m_nuevo_grupo("============== PRUEBAS DEL TP X ===============");

	pa2m_nuevo_grupo("Pruebas de leer_linea");
	pruebas_leer_linea();

	return pa2m_mostrar_reporte();
}
