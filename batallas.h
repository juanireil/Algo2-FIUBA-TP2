#ifndef __BATALLAS_H__
#define __BATALLAS_H__

#include "aventura_pokemon.h"

#define GANO_PRIMERO 1
#define GANO_SEGUNDO -1

/*
 * Se deberan implementar las 5 funciones de batalla.
 * Las reglas para decidir al ganador las inventan ustedes
 * 
 * Si el ganador es el primer pokemon, se devolvera 1
 * Si el ganador es el segundo pokemon, se devolvera -1
 * 
 * No habran empates.
 */

int funcion_batalla_1(void* pkm_1, void* pkm_2);

int funcion_batalla_2(void* pkm_1, void* pkm_2);

int funcion_batalla_3(void* pkm_1, void* pkm_2);

int funcion_batalla_4(void* pkm_1, void* pkm_2);

int funcion_batalla_5(void* pkm_1, void* pkm_2);

/*
 * Pre: Recibirá los punteros a 2 pokemones y un id de funcion de batalla valido.
 * Pos: Devolverá 1 si el ganador es el primer pokemon y -1 si el ganador es el segundo.
 * Reailizará la batalla entre los 2 pokemones.
 * 
*/
int batallas(void* pokemon_1, void* pokemon_2, int id_batallas);

#endif /* __BATALLAS_H__ */