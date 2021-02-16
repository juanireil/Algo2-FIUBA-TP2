#include "aventura_pokemon.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_NOMBRE_ARCHIVO 50
#define GANADO 'G'
#define JUGANDO 'J'
#define PERDIDO 'P'

bool ingreso_inicio_valido(char ingreso){
    return (ingreso == 'E' || ingreso == 'I' || ingreso == 'S' || ingreso == 'A');
}

bool es_nombre_archivo_valido(char nombre_archivo[MAX_NOMBRE_ARCHIVO]){
    FILE* archivo = fopen(nombre_archivo, "r");
    if(!archivo){
        return false;
    }
    fclose(archivo);
    return true;
}
void pedir_nombre_archivo(char nombre_archivo[MAX_NOMBRE_ARCHIVO], char ingreso_inicio){
    
    printf("Ingrese la ruta del archivo deseado\n");
    scanf("%s", nombre_archivo);
    while (!es_nombre_archivo_valido(nombre_archivo)){
        printf("El nombre del archivo ingresado no existe, revise y reintente\n");
        scanf("%s", nombre_archivo);
    }    
}
void inicializar_juego(juego_t* juego, heap_t* gimnasios, personaje_t* personaje, char estado_juego){
    juego->estado_juego = estado_juego;
    juego->gimnasios = gimnasios;
    juego->personaje = personaje;
}
int main(){
    juego_t juego;
    char estado_juego = JUGANDO;
    char ingreso_inicio;
    char nombre_archivo[MAX_NOMBRE_ARCHIVO];

    while (estado_juego == JUGANDO){
        mostrar_menu_inicio();

        scanf(" %c", &ingreso_inicio);
        while (!ingreso_inicio_valido(ingreso_inicio)){
            printf("Ingreso invalido, vea los ingresos posibles.\n");
            scanf(" %c", &ingreso_inicio);
        }
        if (ingreso_inicio == 'E'){
            pedir_nombre_archivo(nombre_archivo, ingreso_inicio);
            printf("Obteniendo la informacion de su personaje\n");
            juego.personaje = cargar_personaje(nombre_archivo);
            if(!juego.personaje){
                printf("Hubo algun error al cargar los datos de su personaje. Revise y reintente.\n");
                pedir_nombre_archivo(nombre_archivo, ingreso_inicio);
            }
            printf("Personaje cargado correctamente\n");
            sleep(1);
        }
        if (ingreso_inicio == 'I'){
            if(!juego.personaje || !juego.gimnasios){
                printf("No ha caregado o el archivo de gimnasios o el del personaje, sin esa informacion no se puede jugar\n");
                sleep(1);
            }
            else{
                inicializar_juego(&juego, juego.gimnasios, juego.personaje, estado_juego);
                if(jugar_aventura(&juego) == -1){
                    printf("Ha ocurrido un error durante su partida, debemos terminar el juego, vuelva a intentarlo\n");
                    return -1;
                }
            }
        }
        if (ingreso_inicio == 'S'){
            inicializar_juego(&juego, juego.gimnasios, juego.personaje, estado_juego);
            //simular partida
        }
        if(ingreso_inicio == 'A'){
            pedir_nombre_archivo(nombre_archivo, ingreso_inicio);
            juego.gimnasios = cargar_gimnasios(nombre_archivo);
            if(!juego.gimnasios){
                printf("Hubo algun error en la carga de los datos. Revise y reintente.\n");
                pedir_nombre_archivo(nombre_archivo, ingreso_inicio);
            }
        }
    }
    system("clear");
    
    return 0;
}