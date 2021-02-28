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
   bool es_lider;
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
  char modo_de_juego;
}juego_t;

/*
 * Crea un personaje pricipal reservando la memoria necesaria.
 * Devuelve un puntero al personaje o NULL en caso de error.
*/
personaje_t* crear_personaje();

/*
 * Crea un pokemon reservando la memoria necesaria.
 * Devuelve un puntero al pokemon o NULL en caso de error.
*/
pokemon_t* crear_pokemon();

/*
 * Crea un gimnasio reservando la memoria necesaria.
 * Devuelve um puntero al gimnasio o NULL en caso de error.
*/
gimnasio_t* crear_gimnasio();

/*
 * Crea un entrenador reservando la memoria necesaria.
 * Devuelve un puntero al entrenador o NULL en caso de error.
*/
entrenador_t* crear_entrenador();

/*
 * Recibe los punteros a 2 gimnasios y los compara según su dificultad.
 * Devuelve 1 si el primer elemento es mayor, -1 si el segundo es mayor
 * y 0 si son iguales.
*/
int comparar_gimnasios(void* gimnasio_1, void* gimnasio_2);

/*
 * Recibe los punteros a 2 pokemones y los compara según su nombre por orden alfabetico.
 * Devuelve 1 si el primer elemento es mayor, -1 si el segundo es mayor
 * y 0 si son iguales.
*/
int comparador_pokemones(void* pokemon_1, void* pokemon_2);

/*
 * Libera la memoria reservada para un pokemon.
*/
void destructor_pokemones(void* pokemon);

/*
 * Libera la memoria reservada para un personaje principal.
*/
void destruir_personaje(personaje_t* personaje);

/*
 * Libera la memoria reservada para un entrenador.
*/
void destructor_entrenadores(void* entrenador);

/*
 * Libera la memoria reservada para un gimnasio.
*/
void destructor_de_gimnasios(void* gimnasio);

/*
 * Recibe un puntero a un heap de gimnasios y el nombre del archivo del cual se
 * desean cargar el/los gimnasios. Cargará los gimnasios leidos al heap recibido.
 * Devolverá 1 si se inserto correctamente en el heap o -1 si hubo un error.
*/
int cargar_gimnasios(heap_t* heap_gimnasios ,char nombre_archivo[MAX_ARCHIVO]);

/*
 * Recibe el nombre del archivo del cual se carga el personaje principal.
 * Devolvera un puntero a un personaje creado con esta informacion o NULL en caso de error.
*/
personaje_t* cargar_personaje(char nombre_archivo[MAX_ARCHIVO]);

/*
 * Recibe un juego valido inicializado correctamente.
 * Desarrollará el juego simulandolo sin mostrar nada o permitiendo al jugador jugarlo segun este
 * cargado en el juego. Esto se desarrollará hasta que el jugador haya o ganado el juego o haya 
 * perdido y este desee no seguir intentandolo. 
 * Devolvera 0 en caso de finalizar correctamente o -1 en caso de error.
*/
int jugar_aventura(juego_t* juego);

/*
 * Realizará la batalla contra un gimnasio recibido. En caso de no simularse se mostrará la
 * informacion necesaria.
 * Devolverá 1 si se vencio al gimnasio, 0 si se perdió frente al gimnasio o -1 si hubo algun error.
*/
int batalla_gimnasio(personaje_t* personaje, gimnasio_t* gimnasio, char* estado_juego, int cantidad_gimnasios, char modo_de_juego);

/*
 * Recibe un pokemon valido.
 * Subira un nivel al pokemon generando esto un aumento de todas sus estadísticas en 1 punto. 
*/
void subir_nivel_pokemon(pokemon_t* pokemon);

/*
 * Recibe un personaje valido.
 * El jugador elegirá que pokemon del equipo desea sacar y cual de sus pokemones que 
 * no esta en el equipo desea agregar a este. Devolverá -1 en caso de error y 0 si se
 * pudo realizar correctamente.
*/
int modificar_equipo(personaje_t* personaje);

/*
 * Muestra el menu de inicio del juego.
 * En este el ususario vera que puede ingresar los archivos de personaje o gimnasios
 * y que puede elegir si jugar la partida o simularla.
*/
void mostrar_menu_inicio();

/*
 * Mostrará el menú de los gimnasios.
 * En este el usuario vera la posibilidad de ver informacion del gimnasio o su personaje,
 * realizar cambios en su equipo o empezar la batalla.
*/
void mostrar_menu_gimnasio();

/*
 * Recibirá los punteros a dos pokemones y el resultado de su
 * Mostrará el menú de batalla.
 * En este verá los resultados de la última batalla y los 2 pokemones que la realizaron.
 * 
*/
void mostrar_menu_batalla(pokemon_t* pokemon_personaje, pokemon_t* pokemon_enemigo, int resultado_batalla);

/*
 * Mostrará el menú de victoria.
 * Recibirá el estado del juego siendo este perdido, ganado o jugando.
 * Si el estado es ganado mostrará que termino el juego.
 * Si el estado es jugando informará la victoria en el gimnasio y mostrará
 * distintas posibilidades. Robar un pokemon del lider,cambiar los pokemones del equipo
 * o avanzar a la siguiente batalla de gimnasio.
*/
void mostrar_menu_victoria(char estado_juego, bool saco_pokemon_lider);

/*
 * Mostrará el menú de derrota.
 * En este se vera las posibles decisiones a tomar, cambiar pokemones del equipo, reintentar el gimnasio o
 * finalizar la partida.
*/
void mostrar_menu_derrota();

#endif /* __AVENTURA_POKEMON_H__ */
