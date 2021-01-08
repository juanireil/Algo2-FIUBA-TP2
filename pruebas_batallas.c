#include "pa2mm.h"
#include <stdlib.h>

int pruebas_batallas(){
    pa2m_nuevo_grupo("PRUEBAS CREACION DE HEAP");
    probar_creacion_del_heap();
    pa2m_nuevo_grupo("PRUEBAS INSERCION DE ELEMENTOS");
    probar_heap_insertar();
    pa2m_nuevo_grupo("PRUEBAS ELIMINACION DE RAIZ");
    probar_heap_extraer_raiz();
    pa2m_mostrar_reporte();
    return 0;
}
