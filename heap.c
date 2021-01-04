#include "heap.h"

#define SE_PUDO_INSERTAR 0
#define NO_SE_PUDO_INSERTAR -1 

heap_t* crear_heap(heap_comparador comparador, heap_liberar_elemento destructor){
    heap_t* heap = NULL;
    if(!comparador){
        return NULL;
    }
    heap = malloc(sizeof(heap_t));
    if(!heap){
        return heap;
    }
    (*heap).destructor = destructor;
    (*heap).comparador = comparador;
    (*heap).tope = 0;

    return heap;
}

int heap_insertar(heap_t* heap, void* elemento){
    if(!heap || !(*heap).comparador ){
        return NO_SE_PUDO_INSERTAR;
    }
    void* aux = realloc(heap ,sizeof(sizeof(heap_t) * ((*heap).tope+1)));
    if(!aux){
        return NO_SE_PUDO_INSERTAR;
    }
    (*heap).tope ++;
    heap = aux;
    sift_up(heap, (*heap).tope-1);
    return SE_PUDO_INSERTAR; 
}
