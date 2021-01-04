#include  "heap.h"
#include "pa2mm.h"

typedef struct cosa{
    int clave;
    char contenido[10];
}cosa;

void destruir_cosa(cosa* c){
    free(c);
}

int comparar_cosas(void* elemento1, void* elemento2){
    if(!elemento1 || !elemento2)
        return 0;

    if(((cosa*)elemento1)->clave>((cosa*)elemento2)->clave)
        return 1;
    if(((cosa*)elemento1)->clave<((cosa*)elemento2)->clave)
        return -1;
    return 0;
}

void destructor_de_cosas(void* elemento){
    if(!elemento)
        return;
    destruir_cosa((cosa*)elemento);
}

void probar_creacion_del_heap(){

    heap_t* heap = NULL;

    pa2m_afirmar((heap=crear_heap(NULL, NULL)) == NULL, "No puedo crear un heap sin comparador.");
    pa2m_afirmar((heap=crear_heap(comparar_cosas, destructor_de_cosas)) != NULL ,"Heap creado correctamente.");
    pa2m_afirmar(heap == NULL, "Heap creado correctamente con raiz NULL.");


}

int main(){
    pa2m_nuevo_grupo("PRUEBAS CREACION DE HEAP");
    probar_creacion_del_heap();
    pa2m_mostrar_reporte();
    return 0;
}
