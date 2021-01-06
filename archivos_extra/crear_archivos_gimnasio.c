#include <stdio.h>
#include <string.h>

const int MAX_NOMBRE = 30;

typedef struct pokemon{
    char nombre[MAX_NOMBRE];
    int velocidad;
    int ataque;
    int defensa;
}pokemon_t;

typedef struct  entrenador{
    char nombre[MAX_NOMBRE];
    pokemon_t* pokemon;
    int cantidad_pokemon;
}entrenador_t;


int main(){
    char eleccion;
    printf("Desea crear un nuevo archivo o agregar a uno ya existente? Ingrese n si quiere uno nuevo o e si es uno existente\n");
    scanf("%c\n", &eleccion);
    char nombre_archivo[MAX_NOMBRE] = "gimnasio_brock.txt";
    char nombre_gimnasio[MAX_NOMBRE] = "gimnasio_brock";
    int dificultad = 5;
    int id_puntero_a_funcion = 3;

    char nombre_lider[MAX_NOMBRE] = "Brock";
    //ver obtener_pokemones
    char entrenador_1[MAX_NOMBRE] = "Cazabichos Aguustin";
    // ver pokes
    char entrenador_2[MAX_NOMBRE] = "Pescador Pablo";
    //ver pokes
    char entrenador_3[MAX_NOMBRE] = "Montañero Jose";
    //ver pokes
    if(eleccion == 'n'){
       //abro archivo en escritura
       return 0;
    }
    //abro archivo para agregar al final
    return 0;
}