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

    void* aux = realloc((*heap).vector ,sizeof(void*) *(size_t)(heap->tope +1));
    if(!aux){
        return NO_SE_PUDO_INSERTAR;
    }
    (*heap).tope ++;
    (*heap).vector = aux;
    heap->vector[heap->tope -1] = elemento;

    sift_up((heap), (*heap).tope -1);
    return SE_PUDO_INSERTAR; 
}
void intercambiar_posiciones(void** vector, int hijo, int padre){
    void* aux = vector[hijo];
    vector[hijo] = vector[padre];
    vector[padre] = aux;
}

int posicion_padre(int posicion_hijo){
    if(posicion_hijo %2 == 0){
        return (posicion_hijo-2)/2;
    }
    return (posicion_hijo-1)/2;
}

void sift_up(heap_t* heap, int posicion){
    if(posicion == 0){
        return;
    }
    int padre = posicion_padre(posicion);

    if((*heap).comparador((*heap).vector[posicion], (*heap).vector[padre]) < 0){
        intercambiar_posiciones((*heap).vector, posicion, padre);
        sift_up(heap, padre);
    }
}

void* heap_extraer_raiz(heap_t* heap){
    if(!heap || !(*heap).vector){
        return NULL;
    }
    
}

void heap_destruir(heap_t* heap){
    free(heap);
}