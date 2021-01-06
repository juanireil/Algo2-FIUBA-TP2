#ifndef __AVENTURA_POKEMON_H__
#define __AVENTURA_POKEMON_H__

#include "abb.h"
#include "lista.h"
#include "heap.h"

#include <stddef.h>
#include <stdlib.h>

const int MAX_NOMBRE = 50;

typedef struct pokemon{
  char nombre[MAX_NOMBRE];
  int velocidad;
  int ataque;
  int defensa;
}pokemon_t;

//decidir si uso las estructuras o punteros a estas
typedef struct entrenador{
  char nombre[MAX_NOMBRE];
  lista_t* medallas; //cola
  lista_t* equipo; //lista
  abb_t* pokemones;
}entrenador_t;

typedef struct gimnasio{
  char nombre[MAX_NOMBRE];
  int dificultad;
  int id_puntero_a_funcion;
  lista_t* entrenadores //implementacion pila
}gimnasio_t;


#endif /* __AVENTURA_POKEMON_H__ */
