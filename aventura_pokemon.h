#ifndef __AVENTURA_POKEMON_H__
#define __AVENTURA_POKEMON_H__

#include "abb.h"
#include "lista.h"
#include "heap.h"
#include "batallas.h"

#include <stddef.h>
#include <stdlib.h>

#define MAX_NOMBRE 50
#define MAX_ARCHIVO 50
typedef struct pokemon{
  char nombre[MAX_NOMBRE];
  int velocidad;
  int ataque;
  int defensa;
}pokemon_t;

//decidir si uso las estructuras o punteros a estas
typedef struct entrenador{
  char nombre[MAX_NOMBRE];
  int medallas;
  lista_t* equipo; //lista
  abb_t* pokemones;
}entrenador_t;

typedef struct gimnasio{
  char nombre[MAX_NOMBRE];
  int dificultad;
  int id_puntero_a_funcion;
  lista_t* entrenadores; //implementacion pila
}gimnasio_t;

void cargar_gimnasios();

void* cargar_personaje(char nombre_archivo[MAX_ARCHIVO]);

void jugar_aventura();

void simular_aventura();

void mostrar_menu_inicio();

void mostrar_menu_gimnasio();

void mostrar_menu_batalla();

void mostrar_menu_victoria();

void mostrar_menu_derrota();

#endif /* __AVENTURA_POKEMON_H__ */
