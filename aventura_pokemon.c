#include "aventura_pokemon.h"
#include <stdio.h>
#include <string.h>

#define MAX_ARCHIVO 50
#define FORMATO_LECTURA_POKEMON "%c;%20[^;];%i;%i;%i\n"

void destructor_pokemones(void* pokemon){
    free(pokemon);
}
int comparador_pokemones(void* pokemon_1, void* pokemon_2){
    return strcmp(((pokemon_t*)pokemon_1)->nombre, ((pokemon_t*)pokemon_2)->nombre);
}

void mostrar_menu_inicio(){
    system("clear");
    printf("Bienvenido a tu aventura Pokemon!!!\n Preparemos todo para empezar.\n");
    printf("Ingrese la letra del comando que  quiera realizar(la letra figura a la izquierda de la opcion).\n\n\n");
    printf("_E: Ingresar archivo de entrenador              _A: Ingresar un gimnasio\n");
    printf("_I: Empezar la partida                          _S: Simular partida\n");

}
void* crear_personaje(){
    entrenador_t* entrenador = malloc(sizeof(entrenador_t));
    if(!entrenador){
        return NULL;
    }
    (*entrenador).equipo = lista_crear();
    (*entrenador).medallas =  0;
    (*entrenador).pokemones = arbol_crear(comparador_pokemones, destructor_pokemones);
    
    return entrenador;

}
void* crear_pokemon(){
    pokemon_t* pokemon = malloc(sizeof(pokemon_t));
    if(!pokemon){
        return NULL;
    }
    (*pokemon).ataque = 0;
    (*pokemon).defensa = 0;
    (*pokemon).velocidad = 0;
    
    return pokemon;
}

void* cargar_personaje(char nombre_archivo[MAX_ARCHIVO]){
    char dato_a_guardar;
    
    entrenador_t* personaje = crear_personaje();
    pokemon_t* pokemon = crear_pokemon();
    if(!personaje || !pokemon || !(personaje->equipo) || !(personaje->pokemones)){
        return NULL;
    }

    FILE* archivo_personaje = fopen(nombre_archivo, "r");
    if(!archivo_personaje){
        printf("No se pudo abrir el archivo. Revise y vuelva a intentar.\n");
        free(pokemon);
        lista_destruir(personaje->equipo);
        arbol_destruir(personaje->pokemones);
        free(personaje);
        return NULL;
    }
    int leidos = fscanf(archivo_personaje, "%c;%20[^\n]\n", &dato_a_guardar, (*personaje).nombre);
    if(leidos != 2){
        free(pokemon);
        lista_destruir(personaje->equipo);
        arbol_destruir(personaje->pokemones);
        free(personaje);
        fclose(archivo_personaje);
        return NULL;
    }
    leidos = fscanf(archivo_personaje, FORMATO_LECTURA_POKEMON, &dato_a_guardar, (*pokemon).nombre, &((*pokemon).ataque), &((*pokemon).defensa), &((*pokemon).velocidad));
    if(leidos != 5){
        free(pokemon);
        lista_destruir(personaje->equipo);
        arbol_destruir(personaje->pokemones);
        free(personaje);
        fclose(archivo_personaje);
        return NULL;
    }
    while (leidos == 5){
        if(personaje->equipo->cantidad < 6){
            lista_insertar(personaje->equipo,pokemon);    
        }
        arbol_insertar(personaje->pokemones, pokemon);
        pokemon = malloc(sizeof(pokemon_t));
        leidos = fscanf(archivo_personaje, FORMATO_LECTURA_POKEMON, &dato_a_guardar, (*pokemon).nombre, &((*pokemon).ataque), &((*pokemon).defensa), &((*pokemon).velocidad));
    }
    free(pokemon);
    fclose(archivo_personaje);
    return personaje;
}
void jugar_aventura(){
   // batallas();
}

