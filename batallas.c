#include "batallas.h"
#include <string.h>
#include <stdio.h>

#define MAX_MODOS_BATALLA 5
/*
** Gana el pokemon más rapido.
**
*/
int funcion_batalla_1(void* pkm_1, void* pkm_2){
    if(((pokemon_t*)pkm_1)->velocidad > ((pokemon_t*)pkm_2)->velocidad){
        return 1;
    }
    return -1;
}
/*
** Gana el pokemon con mas ataque.
**
*/
int funcion_batalla_2(void* pkm_1, void* pkm_2){
    if(((pokemon_t*)pkm_1)->ataque > ((pokemon_t*)pkm_2)->ataque){
        return 1;
    }
    return -1;
}
/*
** Gana el pokemon de mayor defensa.
**
*/
int funcion_batalla_3(void* pkm_1, void* pkm_2){
    if(((pokemon_t*)pkm_1)->defensa > ((pokemon_t*)pkm_2)->defensa){
        return 1;
    }
    return -1;
}
/*
** Gana el pokemon con el nombre más largo.
**
*/
int funcion_batalla_4(void* pkm_1, void* pkm_2){
    if(strlen(((pokemon_t*)pkm_1)->nombre) > strlen(((pokemon_t*)pkm_2)->nombre)){
        return 1;
    }
    return -1;
}
int suma_estadisticas(void* pkm){
    return ((pokemon_t*)pkm)->velocidad + ((pokemon_t*)pkm)->ataque +((pokemon_t*)pkm)->defensa; 
}
/*
** Gana el pokemon con mayor suma de estadisticas.
**
*/
int funcion_batalla_5(void* pkm_1, void* pkm_2){
    int estadisticas_pkm_1 = 0;
    int estadisticas_pkm_2 = 0;
    estadisticas_pkm_1 = suma_estadisticas(pkm_1);
    estadisticas_pkm_2 = suma_estadisticas(pkm_2);

    if(estadisticas_pkm_1 > estadisticas_pkm_2){
        return 1;
    }
    return -1;
}

int batallas(void* pokemon_1, void* pokemon_2, int id_batalla){
    if(!pokemon_1 || !pokemon_2){
        return 0;
    }
    
    int (*funcion_batalla[MAX_MODOS_BATALLA])(void*, void*);

    funcion_batalla[0] = funcion_batalla_1;
    funcion_batalla[1] = funcion_batalla_2;
    funcion_batalla[2] = funcion_batalla_3;
    funcion_batalla[3] = funcion_batalla_4;
    funcion_batalla[4] = funcion_batalla_5;

    return funcion_batalla[id_batalla](pokemon_1, pokemon_2);
}
