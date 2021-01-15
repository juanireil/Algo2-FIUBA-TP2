#include "aventura_pokemon.h"
#include <stdio.h>
#include <string.h>

#define MAX_ARCHIVO 50
#define FORMATO_LECTURA_POKEMON "%c;%20[^;];%i;%i;%i\n"
#define FORMATO_POKEMON "%20[^;];%i;%i;%i\n"
#define FORMATO_GIMNASIOS "%50[^;];%i;%i\n"
const char GIMNASIO = 'G', LIDER = 'L', POKEMON = 'P', ENTRENADOR = 'E';

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
    if(!(entrenador->equipo)){
        free(entrenador);
        return NULL;
    }
    (*entrenador).medallas =  0;
    (*entrenador).pokemones = arbol_crear(comparador_pokemones, destructor_pokemones);
    if(!(entrenador->pokemones)){
        free(entrenador);
        free(entrenador->equipo);
        return NULL;
    }
    
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
    if(!personaje || !pokemon){
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
        pokemon = crear_pokemon();
        leidos = fscanf(archivo_personaje, FORMATO_LECTURA_POKEMON, &dato_a_guardar, (*pokemon).nombre, &((*pokemon).ataque), &((*pokemon).defensa), &((*pokemon).velocidad));
    }
    free(pokemon);
    fclose(archivo_personaje);
    return personaje;
}

void* crear_gimnasio(){
    gimnasio_t* gimnasio = malloc(sizeof(gimnasio_t));
    if(!gimnasio){
        return NULL;
    }
    gimnasio->id_puntero_a_funcion = 0;
    gimnasio->entrenadores = lista_crear();
    if(!(gimnasio->entrenadores)){
        free(gimnasio);
        return NULL;
    }
    gimnasio->dificultad = 0;
    return gimnasio;
}

void destructor_de_gimnasios(void* gimnasio){
    if(!gimnasio){
        return;
    }
    lista_destruir(((gimnasio_t*)gimnasio)->entrenadores);
    free(gimnasio);
} 
int comparar_gimnasios(void* gym_1, void* gym_2 ){
    if(!gym_1 || !gym_2){
        return 0;
    }
    if(((gimnasio_t*)gym_1)->dificultad > ((gimnasio_t*)gym_2)->dificultad){
        return 1;
    }
    if(((gimnasio_t*)gym_1)->dificultad < ((gimnasio_t*)gym_2)->dificultad){
        return -1;
    }
    return 0;
}
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
    entrenador_t* entrenador = crear_personaje();
    if(!entrenador){
        heap_destruir(heap_gimnasios);
        destructor_de_gimnasios(gimnasio);
        return NULL;
    }
    pokemon_t* pokemon = crear_pokemon();

    if(!pokemon){
        heap_destruir(heap_gimnasios);
        destructor_de_gimnasios(gimnasio);
        lista_destruir(entrenador->equipo);
        arbol_destruir(entrenador->pokemones);
        free(entrenador);
        return NULL;
    }
    FILE* archivo_gimnasio = fopen(nombre_gimnasio, "r");
    if(!archivo_gimnasio){
        printf("No se pudo abrir el archivo. Revise y vuelva a intentar.\n");
        free(pokemon);
        lista_destruir(entrenador->equipo);
        arbol_destruir(entrenador->pokemones);
        free(entrenador);
        destructor_de_gimnasios(gimnasio);
        heap_destruir(heap_gimnasios);

        return NULL;
    }
    int leidos = fscanf(archivo_gimnasio, "%c;", &dato_a_guardar);
    if (leidos != 1){
        free(pokemon);
        lista_destruir(entrenador->equipo);
        arbol_destruir(entrenador->pokemones);
        free(entrenador);
        destructor_de_gimnasios(gimnasio);
        heap_destruir(heap_gimnasios);
        fclose(archivo_gimnasio);
        return NULL;
    }
    /*
    **** VER COMO AGREGO LOS PERSONAJES Y POKEMONES A SUS STRUCTS
    **** VER TMBN CREAR NUEVOS PERSONAJES
    ***
    *****
    *****
    */
    while(leidos == 1){
        if (dato_a_guardar == GIMNASIO){
            fscanf(archivo_gimnasio, FORMATO_GIMNASIOS, gimnasio->nombre, &(gimnasio->dificultad), &(gimnasio)->id_puntero_a_funcion);
        }
        if(dato_a_guardar == LIDER || dato_a_guardar == ENTRENADOR){
            fscanf(archivo_gimnasio, "%50[^\n]\n", entrenador->nombre);
        }
        if(dato_a_guardar == POKEMON){
            if(entrenador->equipo->cantidad < 6){
                lista_insertar(entrenador->equipo,pokemon);    
            }
            arbol_insertar(entrenador->pokemones, pokemon);
            pokemon = crear_pokemon();
            leidos = fscanf(archivo_gimnasio, FORMATO_POKEMON, (*pokemon).nombre, &((*pokemon).ataque), &((*pokemon).defensa), &((*pokemon).velocidad));
        }

        leidos = fscanf(archivo_gimnasio, "%c;", &dato_a_guardar);
    }

    fclose(archivo_gimnasio);
    return heap_gimnasios;
}
void mostrar_menu_victoria(bool juego_terminado){
    if(!juego_terminado){
        printf("Felicitaciones!!! \n Has vencido este ginasio.\n");
        printf("_T Toma un pokemon del lider e incorporalo a tu equipo\n");
        printf("_C Realizar cambios en el equipo\n");
        printf("_N Siguiente Gimnasio\n");
    }
    else{
        printf("Felicitaciones, has logrado vencer y culminar esta ardua aventura.\nA partira de ahora eres un maestro pokemon!!!\n");
    }
    
}
void mostrar_menu_derrota(){
    printf("Has sido derrotado, tus pokemones han sido llevados al centro pokemon más cercano.\nEligue que qquieres hacer a continuación.\n");
    printf("_R Reintentar el combate.                                 _F Finalizar partida.\n");
    printf("_C Cambiar pokemones del equipo.\n");
}

void jugar_aventura(){
   // batallas();
}

