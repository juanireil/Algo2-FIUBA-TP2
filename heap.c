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
/*
Pre: Recibe un vector y la posicion del padre e hijo todos validos.
Pos: Intercambia al padre y al hijo en el vector.
*/
void intercambiar_posiciones(void** vector, int hijo, int padre){
    void* aux = vector[hijo];
    vector[hijo] = vector[padre];
    vector[padre] = aux;
}
/*
Pre: Recibe la posicion de un elemento del heap valida.
Pos: Devuelve la posicion del padre de este elemento valida.
*/
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
/*
Pre: Recibe la posicion de un elemento del heap valido.
Pos: Devuelve la posicion que tendria su hijo izquiedo en caso de existir.
*/
int posicion_hijo_izquierdo(int posicion){
    return(2*posicion)+1;
}
/*
Pre: Recibe la posicion de un elemento del heap valida.
Pos: Devuekve la posicion que tendría su hijo derecho en caso de existir.
*/
int posicion_hijo_derecho(int posicion){
    return (2*posicion)+2;
}
void sift_down(heap_t* heap, int posicion){
    int posicion_menor;
    int posicion_izquierdo = posicion_hijo_izquierdo(posicion);
    int posicion_derecho = posicion_hijo_derecho(posicion);

    if(posicion_izquierdo >= (*heap).tope){
        return;
    }
    if (posicion_derecho >= (*heap).tope){
        posicion_menor = posicion_izquierdo;
    }
    else if((*heap).vector[posicion_derecho] > (*heap).vector[posicion_izquierdo]){
        posicion_menor = posicion_izquierdo;
    }
    else{
        posicion_menor = posicion_derecho;
    }
    if((*heap).vector[posicion_menor] < (*heap).vector[posicion]){
        intercambiar_posiciones((*heap).vector, posicion_menor, posicion);
        sift_down(heap, posicion_menor);
    }
}

void* heap_extraer_raiz(heap_t* heap){
    void* raiz;
    void* aux;
    if(!heap || !(*heap).vector){
        return NULL;
    }
    raiz = (*heap).vector[0];
    (*heap).vector[0] = (*heap).vector[(*heap).tope -1];
    
    aux = realloc((*heap).vector, sizeof(void*)*(size_t) (*heap).tope -1);
    if(!aux){
        return NULL;
    }
    (*heap).tope --;
    (*heap).vector = aux;
    sift_down(heap, 0);
    return raiz;
}

void heap_destruir(heap_t* heap){
    for(int i = 0; i < (*heap).tope; i++){
        if((*heap).destructor){
            (*heap).destructor((*heap).vector[i]);
        }
    }
    free((*heap).vector);
    free(heap);

}