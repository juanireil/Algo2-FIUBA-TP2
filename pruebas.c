#include  "heap.h"
#include "pa2mm.h"
#include <stdlib.h>

static const int NO_SE_PUDO_INSERTAR = -1, SE_PUDO_INSERTAR = 0;
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

cosa* crear_cosa(int clave){
    cosa* c = (cosa*)malloc(sizeof(cosa));
    if(c)
        c->clave = clave;
    return c;
}

void probar_creacion_del_heap(){

    heap_t* heap = NULL;
    

    pa2m_afirmar((heap=crear_heap(NULL, NULL)) == NULL, "No puedo crear un heap sin comparador.");
    pa2m_afirmar((heap=crear_heap(comparar_cosas, destructor_de_cosas)) != NULL ,"Heap creado correctamente.");
    pa2m_afirmar(((*heap).vector == NULL), "Heap creado vacio correctamente.");
    
    heap_destruir(heap);
}

void probar_heap_insertar(){
    heap_t* heap = NULL;
    cosa* cosa_1 = crear_cosa(1);
    cosa* cosa_2 = crear_cosa(2);
    cosa* cosa_3 = crear_cosa(3);
    cosa* cosa_4 = crear_cosa(4);
    cosa* cosa_5 = crear_cosa(5);
    cosa* cosa_6 = crear_cosa(6);
    cosa* cosa_7 = crear_cosa(7);
    cosa* cosa_8 = crear_cosa(8);
    cosa* cosa_9 = crear_cosa(9);

    heap = crear_heap(comparar_cosas, destructor_de_cosas);

    pa2m_afirmar(heap_insertar(NULL, cosa_1) == NO_SE_PUDO_INSERTAR, "Heap invalido no se puede insertar");
    pa2m_afirmar(heap_insertar(heap, cosa_6) == SE_PUDO_INSERTAR, "Heap vacio se pudo insertar correctamente");
    pa2m_afirmar(heap_insertar(heap, cosa_8) == SE_PUDO_INSERTAR, "Heap con un elemento se pudo insertar.");
    pa2m_afirmar(heap_insertar(heap, cosa_4) == SE_PUDO_INSERTAR, "Heap con varios elementos inserto nuevo minimo, se pudo insertar correctamente");
    pa2m_afirmar(*(*heap).vector == cosa_4, "Nuevo minimo ubicado correctamente");
    heap_insertar(heap, cosa_2);
    heap_insertar(heap, cosa_5);
    heap_insertar(heap, cosa_7);
    heap_insertar(heap, cosa_9);
    heap_insertar(heap, cosa_3);
    heap_insertar(heap, cosa_1);
    pa2m_afirmar(*(*heap).vector == cosa_1, "Inserte varios elementos minimo ubicado correctamente.");
    heap_destruir(heap);
}
void probar_heap_extraer_raiz(){
    heap_t* heap = NULL;
    cosa* cosa_1 = crear_cosa(1);
    cosa* cosa_2 = crear_cosa(2);
    cosa* cosa_3 = crear_cosa(3);
    cosa* cosa_4 = crear_cosa(4);
    cosa* cosa_5 = crear_cosa(5);
    cosa* cosa_6 = crear_cosa(6);
    cosa* cosa_7 = crear_cosa(7);
    cosa* cosa_8 = crear_cosa(8);
    cosa* cosa_9 = crear_cosa(9);

    heap = crear_heap(comparar_cosas, destructor_de_cosas);
    pa2m_afirmar(heap_extraer_raiz(NULL) == NULL, "Heap invalido no existe raiz");
    pa2m_afirmar(heap_extraer_raiz(heap) == NULL, "Heap vacio no existe raiz");
    heap_insertar(heap, cosa_2);
    heap_insertar(heap, cosa_5);
    heap_insertar(heap, cosa_7);
    heap_insertar(heap, cosa_9);
    heap_insertar(heap, cosa_3);
    heap_insertar(heap, cosa_1);
    heap_insertar(heap,cosa_6);
    heap_insertar(heap,cosa_8);
    heap_insertar(heap,cosa_4);
    pa2m_afirmar(heap_extraer_raiz(heap) == cosa_1, "Raiz eliminada correctamente");
    pa2m_afirmar((*heap).vector[0] == cosa_2, "Nueva raiz es la correcta");
    destruir_cosa(cosa_1);
    heap_destruir(heap);
}
int main(){
    pa2m_nuevo_grupo("PRUEBAS CREACION DE HEAP");
    probar_creacion_del_heap();
    pa2m_nuevo_grupo("PRUEBAS INSERCION DE ELEMENTOS");
    probar_heap_insertar();
    pa2m_nuevo_grupo("PRUEBAS ELIMINACION DE RAIZ");
    probar_heap_extraer_raiz();
    pa2m_mostrar_reporte();
    return 0;
}
