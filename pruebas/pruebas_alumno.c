#include "pa2m.h"
#include "pruebas.h"

/**
 * Main: Conjunto de grupos de pruebas
 */

int main()
{
	pa2m_nuevo_grupo("============== PRUEBAS DEL TP X ===============");

	pruebas_leer_linea();

	pruebas_buscar();

	pruebas_combinar();

	pa2m_nuevo_grupo("Pruebas de archivos");
	pruebas_archivos();

	pruebas_iterar();
	return pa2m_mostrar_reporte();
}
