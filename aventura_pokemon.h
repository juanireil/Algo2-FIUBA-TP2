#ifndef __AVENTURA_POKEMON_H__
#define __AVENTURA_POKEMON_H__

#include "heap.h"
#include "abb.h"
#include "lista.h"
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
  bool esta_en_equipo;
}pokemon_t;

//decidir si uso las estructuras o punteros a estas
typedef struct personaje{
  char nombre[MAX_NOMBRE];
  int medallas;
  lista_t* equipo; //lista
  abb_t* pokemones;
}personaje_t;
 typedef struct entrenador{
   char nombre [MAX_NOMBRE];
   lista_t* equipo;
 }
 entrenador_t;
typedef struct gimnasio{
  char nombre[MAX_NOMBRE];
  int dificultad;
  int id_puntero_a_funcion;
  lista_t* entrenadores; //implementacion pila
}gimnasio_t;

typedef struct juego{
  personaje_t* personaje;
  heap_t* gimnasios;
  char estado_juego;
}juego_t;

void destructor_de_gimnasios(void* gimnasio);

void* cargar_gimnasios(char nombre_archivo[MAX_ARCHIVO]);

void* cargar_personaje(char nombre_archivo[MAX_ARCHIVO]);

int jugar_aventura(juego_t* juego);

void simular_aventura();

void mostrar_menu_inicio();

void mostrar_menu_gimnasio();

void mostrar_menu_batalla();

void mostrar_menu_victoria(bool juego_terminado);

void mostrar_menu_derrota();

#endif /* __AVENTURA_POKEMON_H__ */
