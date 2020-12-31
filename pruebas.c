#include  "heap.h"
#include "pa2mm.h"


void probar_creacion_del_heap(){
    crear_heap();
}

int main(){
    pa2m_nuevo_grupo("PRUEBAS CREACION DE HEAP");
    probar_creacion_del_heap();
    pa2m_mostrar_reporte();
    return 0;
}
