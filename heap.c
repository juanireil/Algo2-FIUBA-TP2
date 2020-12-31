#include "heap.h"

heap_t* crear_heap(heap_comparador comparador, heap_liberar_elemento destructor){
    heap_t* heap = NULL;
    if(!comparador){
        return NULL;
    }
    heap = malloc(sizeof(heap_t));
    if(!heap){
        return heap;
    }

}