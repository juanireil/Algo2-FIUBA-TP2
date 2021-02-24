#include "aventura_pokemon.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_ARCHIVO 50
#define FORMATO_POKEMON "%20[^;];%i;%i;%i\n"
#define FORMATO_GIMNASIOS "%50[^;];%i;%i\n"
#define JUGANDO 'J'
#define PERDIDO 'P'
#define GANADO 'G'
#define ERROR -1
const char GIMNASIO = 'G', LIDER = 'L', POKEMON = 'P', ENTRENADOR = 'E';

void* cargar_gimnasios(char nombre_gimnasio[MAX_ARCHIVO]){
    char dato_a_guardar;
    heap_t* heap_gimnasios = crear_heap(comparar_gimnasios, destructor_de_gimnasios);
    if(!heap_gimnasios){
        return NULL;
    }
    gimnasio_t* gimnasio = crear_gimnasio();
    if(!gimnasio){
        heap_destruir(heap_gimnasios);
        return NULL;
    }
    entrenador_t* entrenador = crear_entrenador();
    if(!entrenador){
        heap_destruir(heap_gimnasios);
        destructor_de_gimnasios(gimnasio);
        return NULL;
    }
     entrenador_t* lider = crear_entrenador();
    if(!lider){
        heap_destruir(heap_gimnasios);
        destructor_de_gimnasios(gimnasio);
        return NULL;
    }
    pokemon_t* pokemon = crear_pokemon();

    if(!pokemon){
        heap_destruir(heap_gimnasios);
        destructor_de_gimnasios(gimnasio);
        lista_destruir(entrenador->equipo);
        free(entrenador);
        return NULL;
    }
    FILE* archivo_gimnasio = fopen(nombre_gimnasio, "r");
    if(!archivo_gimnasio){
        printf("No se pudo abrir el archivo. Revise y vuelva a intentar.\n");
        free(pokemon);
        lista_destruir(entrenador->equipo);
        free(entrenador);
        destructor_de_gimnasios(gimnasio);
        heap_destruir(heap_gimnasios);

        return NULL;
    }
    int leidos = fscanf(archivo_gimnasio, "%c;", &dato_a_guardar);
    if (leidos != 1){
        free(pokemon);
        lista_destruir(entrenador->equipo);
        free(entrenador);
        destructor_de_gimnasios(gimnasio);
        heap_destruir(heap_gimnasios);
        fclose(archivo_gimnasio);
        return NULL;
    }
    while(leidos == 1){
        
        while (dato_a_guardar == 'G'){
            fscanf(archivo_gimnasio, FORMATO_GIMNASIOS, gimnasio->nombre, &(gimnasio->dificultad), &(gimnasio)->id_puntero_a_funcion);
            leidos = fscanf(archivo_gimnasio, "%c;", &dato_a_guardar);
            
            while (dato_a_guardar == 'L'){
                fscanf(archivo_gimnasio, "%50[^\n]\n", lider->nombre);
                leidos = fscanf(archivo_gimnasio, "%c;", &dato_a_guardar);
                
                while(dato_a_guardar == 'E'){
                    fscanf(archivo_gimnasio, "%50[^\n]\n", entrenador->nombre);
                    leidos = fscanf(archivo_gimnasio, "%c;", &dato_a_guardar);
                    
                    while(dato_a_guardar == 'P'){
                        fscanf(archivo_gimnasio, FORMATO_POKEMON, (*pokemon).nombre, &((*pokemon).ataque), &((*pokemon).defensa), &((*pokemon).velocidad));
                        lista_insertar(entrenador->equipo,pokemon);
                        pokemon->esta_en_equipo = true;    
            
                        pokemon = crear_pokemon();
                        leidos = fscanf(archivo_gimnasio, "%c;", &dato_a_guardar);
                    }

                    lista_apilar(gimnasio->entrenadores, entrenador);
                    entrenador = crear_entrenador();
                }
                
                lista_apilar(gimnasio->entrenadores, entrenador);
                entrenador = crear_entrenador();
                lista_apilar(gimnasio->entrenadores, lider);
                lider = crear_entrenador();
            }
            
        }

        
        if (dato_a_guardar == GIMNASIO){
            if(strcmp(gimnasio->nombre, " ") != 0){
                lista_apilar(gimnasio->entrenadores, entrenador);
                heap_insertar(heap_gimnasios, gimnasio);
                gimnasio = crear_gimnasio();
                entrenador = crear_entrenador();
            }
            fscanf(archivo_gimnasio, FORMATO_GIMNASIOS, gimnasio->nombre, &(gimnasio->dificultad), &(gimnasio)->id_puntero_a_funcion);
        }
        if(dato_a_guardar == LIDER || dato_a_guardar == ENTRENADOR){
            if(strcmp(entrenador->nombre, " ") != 0){
                lista_apilar(gimnasio->entrenadores, entrenador);
                entrenador = crear_entrenador();
            }
            fscanf(archivo_gimnasio, "%50[^\n]\n", entrenador->nombre);
        }
        if(dato_a_guardar == POKEMON){
            fscanf(archivo_gimnasio, FORMATO_POKEMON, (*pokemon).nombre, &((*pokemon).ataque), &((*pokemon).defensa), &((*pokemon).velocidad));
            lista_insertar(entrenador->equipo,pokemon);
            pokemon->esta_en_equipo = true;    
            
            pokemon = crear_pokemon();
        }

        leidos = fscanf(archivo_gimnasio, "%c;", &dato_a_guardar);
    }
    destructor_pokemones(pokemon);
    lista_apilar(gimnasio->entrenadores, entrenador);
    heap_insertar(heap_gimnasios, gimnasio);

    fclose(archivo_gimnasio);
    return heap_gimnasios;
}