#include "aventura_pokemon.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_NOMBRE_ARCHIVO 50
#define GANADO 'G'
#define JUGANDO 'J'
#define PERDIDO 'P'
#define ERROR -1
static const char JUGAR = 'I', SIMULAR = 'S', ENTRENADOR = 'E', GIMNASIO = 'A';

bool ingreso_inicio_valido(char ingreso){
    return (ingreso == ENTRENADOR || ingreso == JUGAR || ingreso == SIMULAR || ingreso == GIMNASIO);
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

int inicializar_juego(juego_t* juego){
    juego->estado_juego  = JUGANDO;
    
    juego->gimnasios = crear_heap(comparar_gimnasios, destructor_de_gimnasios);
    if(!juego->gimnasios){
        return ERROR;
    }

    juego->personaje = NULL;
    juego->cantidad_gimnasios = 0;
    return 0;

}

int main(){
    juego_t juego;
    char ingreso_inicio;
    char nombre_archivo[MAX_NOMBRE_ARCHIVO];

    if(inicializar_juego(&juego) == ERROR){
        printf("Ha ocurrido un error\n");
        return ERROR;
    }
    while (juego.estado_juego == JUGANDO){    
        mostrar_menu_inicio();
        
        scanf(" %c", &ingreso_inicio);
        while (!ingreso_inicio_valido(ingreso_inicio)){
            printf("Ingreso invalido, vea los ingresos posibles.\n");
            scanf(" %c", &ingreso_inicio);
        }
        if (ingreso_inicio == ENTRENADOR){
            if(!juego.personaje){
                pedir_nombre_archivo(nombre_archivo, ingreso_inicio);
                printf("Obteniendo la informacion de su personaje\n");
                juego.personaje = cargar_personaje(nombre_archivo);

                if(!juego.personaje){
                printf("Hubo algun error al cargar los datos de su personaje. Revise y reintente.\n");
                }
                else{
                    printf("Personaje cargado correctamente\n");
                }
            }
            else{
                printf("Ya cargaste un personaje no se puede cargar otro\n");
            }
            sleep(1);
        }
        if (ingreso_inicio == JUGAR || ingreso_inicio == SIMULAR){
            juego.cantidad_gimnasios = juego.gimnasios->tope;
            juego.modo_de_juego = ingreso_inicio;

            if(!juego.personaje || juego.cantidad_gimnasios < 1){
                printf("No ha cargado o el archivo de gimnasios o el del personaje, sin esa informacion no se puede jugar\n");
                sleep(1);
            }
            else{
                if(jugar_aventura(&juego) == -1){
                    printf("Ha ocurrido un error durante su partida, debemos terminar el juego, vuelva a intentarlo\n");
                    return -1;
                }
            }
        }
        if(ingreso_inicio == GIMNASIO){
            pedir_nombre_archivo(nombre_archivo, ingreso_inicio);
            
            if(cargar_gimnasios(juego.gimnasios, nombre_archivo) == ERROR){
                printf("Hubo algun error en la carga de los datos. Revise y reintente.\n");
            }
            else{
                printf("Informacion de los gimnasios cargada correctamente\n");
            }
            sleep(1);
        }
    }
    system("clear");
    
    return 0;
}