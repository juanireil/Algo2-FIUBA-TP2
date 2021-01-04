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
    (*heap).vector = NULL;
    (*heap).destructor = destructor;
    (*heap).comparador = comparador;
    (*heap).tope = 0;

    return heap;
}

int heap_insertar(heap_t* heap, void* elemento){
    if(!heap || !(*heap).comparador ){
        return NO_SE_PUDO_INSERTAR;
    }
    void* aux = realloc((*heap).vector ,sizeof(sizeof(void*) * ((*heap).tope+1)));
    if(!aux){
        return NO_SE_PUDO_INSERTAR;
    }
    (*heap).tope ++;
    (*heap).vector = aux;
    sift_up((heap), (*heap).tope);
    return SE_PUDO_INSERTAR; 
}
void intercambiar_posiciones(void* elemento_1, void* elemento_2){
    void* aux = elemento_1;
    elemento_1 = elemento_2;
    elemento_2 = aux;
}
void sift_up(heap_t* heap, int posicion){
    if(posicion == 1){
        return;
    }
    if(posicion%2 == 0){
        if((*heap).comparador((*heap).vector + posicion/2 -1, (*heap).vector +posicion-1) < 0){
            intercambiar_posiciones((*heap).vector+ posicion/2 -1, (*heap).vector + posicion -1);
            posicion = posicion/2;
        }
    }
    else{
        if((*heap).comparador((*heap).vector + (posicion-1)/2 -1, (*heap).vector +posicion-1) < 0){
            intercambiar_posiciones((*heap).vector+ (posicion-1)/2 -1, (*heap).vector + posicion -1);
            posicion = (posicion-1)/2;
        }
    }
    sift_up(heap, posicion);
}

void heap_destruir(heap_t* heap){
    free(heap);
}