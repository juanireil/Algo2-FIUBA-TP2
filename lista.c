#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

static const int NO_SE_PUDO_INSERTAR = -1, SE_PUDO_INSERTAR = 0;
static const int SE_PUDO_BORRAR = 0, NO_SE_PUDO_BORRAR = -1;
static const size_t LISTA_VACIA = 0, PRIMERA_POSICION = 0;
static const bool NO_TIENE_SIGUIENTE = false;

lista_t* lista_crear(){
    lista_t* lista = NULL;

    lista = malloc(sizeof(lista_t));
    if(lista == NULL){
        return lista;
    }
    (*lista).nodo_inicio = NULL;
    (*lista).nodo_fin = NULL;
    (*lista).cantidad = 0;
    
    return lista;
}
/*
    Pre: Recibe un elemento valido
    Pos: Devolvera un nodo valido con el elemento recibido y su siguiente en NULL
*/
nodo_t* crear_nodo(void* elemento){
    nodo_t* nodo = NULL;

    nodo = malloc(sizeof(nodo_t));
    if (nodo == NULL){
        return nodo;
    }

    (*nodo).elemento = elemento;
    (*nodo).siguiente = NULL;

    return nodo;
}

int lista_insertar(lista_t* lista, void* elemento){
    nodo_t* nodo = NULL;
    nodo_t* nodo_aux = NULL;

    if(lista == NULL){
        return NO_SE_PUDO_INSERTAR;
    }
    nodo = crear_nodo(elemento);
    nodo_aux = (*lista).nodo_inicio;
    if (nodo == NULL){
        return NO_SE_PUDO_INSERTAR;
    }

    if((*lista).cantidad == LISTA_VACIA){
        (*lista).nodo_inicio = nodo;
    }
    else{
        while((*nodo_aux).siguiente != NULL){
            nodo_aux = (*nodo_aux).siguiente;
        }
        (*nodo_aux).siguiente = nodo;
    }
    (*lista).cantidad ++;
    (*lista).nodo_fin = nodo;
    
    return SE_PUDO_INSERTAR;
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
    nodo_t* nodo = NULL;
    nodo_t* nodo_actual = NULL;
    int insercion = 0;
    
    if(lista == NULL){
        return NO_SE_PUDO_INSERTAR;
    }     
    if ((((posicion+1) > (*lista).cantidad) && (posicion > 0)) || ((*lista).cantidad == 0)){
        insercion = lista_insertar(lista, elemento);
        return insercion;
    }

    nodo_actual = (*lista).nodo_inicio;
    nodo = crear_nodo(elemento);
    if (nodo == NULL){
        return NO_SE_PUDO_INSERTAR;
    }

    while(posicion > 1){
        nodo_actual = (*nodo_actual).siguiente;
        posicion--;
    }
    if(posicion == 0){
        (*nodo).siguiente = nodo_actual;
        (*lista).nodo_inicio = nodo;
    }
    else{
        (*nodo).siguiente = (*nodo_actual).siguiente;
        (*nodo_actual).siguiente = nodo;
    } 
    (*lista).cantidad ++;
    return SE_PUDO_INSERTAR;
}

int lista_borrar(lista_t* lista){
    nodo_t* nodo = NULL;

    if(lista == NULL || (*lista).cantidad == LISTA_VACIA){
        return NO_SE_PUDO_BORRAR;
    }
    if((*lista).cantidad == 1){
        free((*lista).nodo_fin);
        (*lista).nodo_inicio = NULL;
        (*lista).nodo_fin = NULL;
    }
    else{
        nodo = (*lista).nodo_inicio;
        while((*(*nodo).siguiente).siguiente != NULL){
            nodo = (*nodo).siguiente;
        }
        free((*nodo).siguiente);
        (*nodo).siguiente = NULL;
        (*lista).nodo_fin = nodo;
    }
    (*lista).cantidad --;

    return SE_PUDO_BORRAR;
}
/*
    Pre: Recibe un puntero al primer nodo de la lista y la posicion a borrar
    Pos: Borrara el nodo de la posicion recibida dejando la lista funcional.
*/
void borrar_nodo(nodo_t* nodo, size_t posicion){
    nodo_t* nodo_aux = NULL;

    if(posicion == 1){
        nodo_aux = (*nodo).siguiente;
        (*nodo).siguiente = (*(*nodo).siguiente).siguiente;
        free(nodo_aux);
        return;
    }
    borrar_nodo((*nodo).siguiente, posicion-1);
    return;
}
int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
    nodo_t* nodo = NULL;

    if((lista == NULL) || (*lista).cantidad == LISTA_VACIA){
        return NO_SE_PUDO_BORRAR;
    }
    if(posicion >= (*lista).cantidad -1){
        return lista_borrar(lista);
    }
    nodo = (*lista).nodo_inicio;
    if(posicion == 0){
        (*lista).nodo_inicio = (*nodo).siguiente;
        free(nodo);
    }
    else{
        borrar_nodo(nodo, posicion);
    }
    
    
    (*lista).cantidad --;
    return SE_PUDO_BORRAR;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
    nodo_t* nodo = NULL;
    if(lista == NULL || posicion+1 > (*lista).cantidad){
        return NULL;
    }
    nodo = (*lista).nodo_inicio;
    while (posicion != 0){
        nodo = (*nodo).siguiente;
        posicion--;
    }
    return (*nodo).elemento;

}

void* lista_ultimo(lista_t* lista){
    if((lista == NULL) || (*lista).cantidad <= LISTA_VACIA){
        return NULL;
    }
    return (*lista).nodo_fin->elemento;
}

bool lista_vacia(lista_t* lista){
    if(lista == NULL){
        return true;
    }
    return ((*lista).cantidad == LISTA_VACIA); 
}

size_t lista_elementos(lista_t* lista){
    if (lista == NULL){
        return LISTA_VACIA;
    }
    
    return (*lista).cantidad;
}

int lista_apilar(lista_t* lista, void* elemento){
    return lista_insertar(lista, elemento);
}

int lista_desapilar(lista_t* lista){
    return lista_borrar(lista);
}

void* lista_tope(lista_t* lista){
    return lista_ultimo(lista);
}

int lista_encolar(lista_t* lista, void* elemento){
    return lista_insertar(lista, elemento);
}

int lista_desencolar(lista_t* lista){
    return lista_borrar_de_posicion(lista, PRIMERA_POSICION);
}

void* lista_primero(lista_t* lista){
    return lista_elemento_en_posicion(lista, PRIMERA_POSICION);
}
/*
    Pre: Recibe un puntero al primer nodo de una lista.
    Pos: Recursivamente borarrara todos los nodos de la lista.
*/
void destruir_nodos(nodo_t* nodo){
    if(nodo != NULL){
        destruir_nodos((*nodo).siguiente);
        free(nodo);
    }
}

void lista_destruir(lista_t* lista){
    if(lista_elementos(lista) > 0){
        destruir_nodos((*lista).nodo_inicio);
    }
    free(lista);
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){
    lista_iterador_t* iterador = NULL;

    if(lista == NULL){
        return NULL;
    }
    iterador = malloc(sizeof(lista_iterador_t));
    if (iterador == NULL){
        return NULL;
    }
    
    (*iterador).lista = lista;
    (*iterador).corriente = (*lista).nodo_inicio;
    return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
    if(iterador == NULL){
        return NO_TIENE_SIGUIENTE;
    }
    return (*iterador).corriente != NULL;
}

bool lista_iterador_avanzar(lista_iterador_t* iterador){
    if(iterador == NULL){
        return NO_TIENE_SIGUIENTE;
    }
    if(lista_iterador_tiene_siguiente(iterador)){
        (*iterador).corriente = (*(*iterador).corriente).siguiente;
        return (*iterador).corriente;
    }
    return NO_TIENE_SIGUIENTE;
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador){
    if ((iterador == NULL) || (*iterador).corriente == NULL){
        return NULL;
    }
    return (*(*iterador).corriente).elemento;
}

void lista_iterador_destruir(lista_iterador_t* iterador){
    free(iterador);
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto){
    nodo_t* nodo;
    size_t contador = 1;

    if(lista == NULL || contexto == NULL || funcion == NULL || (*lista).cantidad == 0){
        return 0;
    }
    nodo = (*lista).nodo_inicio;
    while(funcion((*nodo).elemento, contexto) && (*nodo).siguiente != NULL){
        nodo = (*nodo).siguiente;
        contador ++;
    }

    return contador;
}