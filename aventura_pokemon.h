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
  int nivel;
  bool esta_en_equipo;
}pokemon_t;

typedef struct personaje{
  char nombre[MAX_NOMBRE];
  int medallas;
  lista_t* equipo; //Implementado como lista
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
  lista_t* entrenadores; //Implementado como pila
}gimnasio_t;

typedef struct juego{
  personaje_t* personaje;
  heap_t* gimnasios;
  char estado_juego;
  int cantidad_gimnasios;
}juego_t;

void destructor_de_pokemones(void* pokemon);

int comparador_pokemones(void* pokemon_1, void* pokemon_2);

void* crear_personaje();

void destruir_personaje(personaje_t* personaje);

void* crear_pokemon();

void destructor_entrenadores(void* entrenador);

void* crear_gimnasio();

void destructor_de_gimnasios(void* gimnasio);

int comparar_gimnasios(void* gimnasio_1, void* gimnasio_2);

void* crear_entrenador();

void subir_nivel_pokemon(pokemon_t* pokemon);

void* cargar_gimnasios(char nombre_archivo[MAX_ARCHIVO]);

void* cargar_personaje(char nombre_archivo[MAX_ARCHIVO]);

int batalla_gimnasio(personaje_t* personaje, gimnasio_t* gimnasio, char* estado_juego, int cantidad_gimnasios);

int jugar_aventura(juego_t* juego);

void simular_aventura();

void mostrar_menu_inicio();

void mostrar_menu_gimnasio();

void mostrar_menu_batalla();

void mostrar_menu_victoria(char estado_juego);

void mostrar_menu_derrota();

#endif /* __AVENTURA_POKEMON_H__ */
