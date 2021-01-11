#include "aventura_pokemon.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool ingreso_inicio_valido(char ingreso){
    return (ingreso == 'E' || ingreso == 'I' || ingreso == 'S' || ingreso == 'A');
}

int main(){
    char ingreso_inicio;
    entrenador_t* personaje = NULL;
    mostrar_menu_inicio();

    scanf(" %c", &ingreso_inicio);
    while (!ingreso_inicio_valido(ingreso_inicio)){
        printf("Ingreso invalido, vea los ingresos posibles.\n");
        scanf(" %c", &ingreso_inicio);
    }
    if (ingreso_inicio == 'E'){
        printf("Obteniendo la informacion de su personaje\n");
        personaje = cargar_personaje("personaje_principal.txt");
        if(!personaje){
            printf("Hubo algun error al cargal los datos de su personaje. Revise y reintente.\n");
            return -1;
        }
    }
    if (ingreso_inicio == 'I'){
        // inocio_partida
    }
    if (ingreso_inicio == 'S'){
        //simular partida
    }
    // ingreso gimnasios
    

    return 0;
}