#ifndef __HEAP_H__
#define __HEAP_H__


#include <stdio.h>
#include <stdlib.h>

/*
 * Comparador de elementos. Recibe dos elementos del heap y devuelve
 * 0 en caso de ser iguales, 1 si el primer elemento es mayor al
 * segundo o -1 si el primer elemento es menor al segundo.
 */
typedef int (*heap_comparador)(void*, void*);

/*
 * Destructor de elementos. Cada vez que un elemento deja el heap
 * se invoca al destructor pasandole
 * el elemento.
 */
typedef void (*heap_liberar_elemento)(void*);

typedef struct nodo_heap {
  void* elemento;
  struct nodo_heap* izquierda;
  struct nodo_heap* derecha;
} nodo_heap_t;

typedef struct heap{
  nodo_heap_t* nodo_heap;
  heap_comparador comparador;
  heap_liberar_elemento destructor;
} heap_t;

/*
** Crea el arbol y reserva la memoria necesaria de la estructura.
** Comparador se utiliza para comparar dos elementos.
** Devuelve el puntero a un heap vacío o NULL en caso de error.
*/
heap_t* crear_heap(heap_comparador comparador, heap_liberar_elemento destructor);

/*
 * Inserta un elemento en el heap.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 * El heap admite elementos con valores repetidos.
 */
int heap_insertar(heap_t* heap, void* elemento);

/*
 * Quita del heap la raiz y lo deja como un heap valido.
 * Adicionalmente, si encuentra el elemento, invoca el destructor con
 * dicho elemento.
 * Devuelve 0 si pudo eliminar el elemento o -1 en caso contrario.
 */
int heap_borrar(heap_t* arbol, void* elemento);

/*
 * Destruye el heap liberando la memoria reservada por el mismo.
 * Adicionalmente invoca el destructor con cada elemento presente en
 * el.
 */
void heap_destruir(heap_t* arbol);

#endif /* __HEAP_H__ */
