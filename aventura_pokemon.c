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
    personaje_t* entrenador = malloc(sizeof(personaje_t));
    if(!entrenador){
        return NULL;
    }
    (*entrenador).equipo = lista_crear(NULL);
    if(!(entrenador->equipo)){
        free(entrenador);
        return NULL;
    }
    strcpy((*entrenador).nombre, " ");
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
    (*pokemon).nivel = 0;
    (*pokemon).esta_en_equipo = false;
    
    return pokemon;
}

void* cargar_personaje(char nombre_archivo[MAX_ARCHIVO]){
    char dato_a_guardar;
    
    personaje_t* personaje = crear_personaje();
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
    int leidos = fscanf(archivo_personaje, "%c;", &dato_a_guardar);
    if(leidos != 1){
        free(pokemon);
        lista_destruir(personaje->equipo);
        arbol_destruir(personaje->pokemones);
        free(personaje);
        fclose(archivo_personaje);
        return NULL;
    }
    while (leidos == 1){
        if(dato_a_guardar == ENTRENADOR){
            fscanf(archivo_personaje, "%50[^\n]\n", personaje->nombre);
        }
        if (dato_a_guardar == POKEMON){
            fscanf(archivo_personaje, FORMATO_POKEMON, (*pokemon).nombre, &((*pokemon).ataque), &((*pokemon).defensa), &((*pokemon).velocidad));
            
            if(personaje->equipo->cantidad < 6){
                lista_insertar(personaje->equipo,pokemon);    
                pokemon->esta_en_equipo = true;
            }
            arbol_insertar(personaje->pokemones, pokemon);
            pokemon = crear_pokemon();
        }
        leidos = fscanf(archivo_personaje, "%c;", &dato_a_guardar);
    }
    free(pokemon);
    fclose(archivo_personaje);
    return personaje;
}
void destructor_entrenadores(void* entrenador){
    if(!entrenador){
        return;
    }
    lista_destruir(((entrenador_t*)entrenador)->equipo);
    free(entrenador);
}
void* crear_gimnasio(){
    gimnasio_t* gimnasio = malloc(sizeof(gimnasio_t));
    if(!gimnasio){
        return NULL;
    }
    strcpy(gimnasio->nombre, " ");
    gimnasio->id_puntero_a_funcion = 0;
    gimnasio->entrenadores = lista_crear(destructor_entrenadores);
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

void* crear_entrenador(){
    entrenador_t* entrenador = malloc(sizeof(entrenador_t));
    if(!entrenador){
        return NULL;
    }

    entrenador->equipo = lista_crear(destructor_pokemones);
    if(!entrenador->equipo){
        free(entrenador);
        return NULL;
    }
    strcpy(entrenador->nombre, "");
    return entrenador;
}
/*
 * ver como mejorar LA FUNCION
 * ES HOOOOOOOOORRIBLE
 * ASI NO LA ENTREGO
 */
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
void mostrar_menu_victoria(char estado_juego){
    system("clear");
    if(estado_juego == JUGANDO){
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
    system("clear");
    printf("Has sido derrotado, tus pokemones han sido llevados al centro pokemon más cercano.\nEligue que quieres hacer a continuación.\n");
    printf("_R Reintentar el combate.                                 _F Finalizar partida.\n");
    printf("_C Cambiar pokemones del equipo.\n");
}

void mostrar_menu_gimnasio(){
    system("clear");
    printf("Estas por empezar una batalla de gimnasio, que deseas hacer a continuacion?\n");
    printf("_E Revisar personaje principal y sus pokemones              _G Ver informacion del gimnasio\n");
    printf("_C Realizar cambios en el equipo                            _B Proceder a la siguiente batalla\n");
}

bool ingreso_valido(char ingreso){
    return (ingreso == 'E' || ingreso == 'C' || ingreso == 'G' || ingreso == 'B');
}
bool mostrar_pokemones_lista(void* elemento, void* contador){
    if(elemento && contador){
        (*(int*)contador)+=1;
        printf("    Pokemon N° %i\n", (*(int*)contador));
        printf("    Nombre: %s\n    Velocidad: %i\n", ((pokemon_t*)elemento)->nombre,((pokemon_t*)elemento)->velocidad);
        printf("    Ataque: %i\n    Defensa: %i\n",  ((pokemon_t*)elemento)->ataque,((pokemon_t*)elemento)->defensa);
        printf("    Nivel: %i\n\n",((pokemon_t*)elemento)->nivel);
    }
    return true;
}

bool mostrar_pokemones_arbol(void* elemento, void* contador){
    if(elemento && contador){
        (*(int*)contador)+=1;
        printf("    Nombre: %s\n    Velocidad: %i\n", ((pokemon_t*)elemento)->nombre,((pokemon_t*)elemento)->velocidad);
        printf("    Ataque: %i\n    Defensa: %i\n\n",  ((pokemon_t*)elemento)->ataque,((pokemon_t*)elemento)->defensa);
        printf("    Nivel: %i\n\n",((pokemon_t*)elemento)->nivel);
    }
    return false;
}

void mostrar_personaje(personaje_t* personaje){
    bool (*funcion)(void*, void*) = mostrar_pokemones_lista;
    int contador = 0;
    char ingreso;

    printf("Nombre: %s\n Cantidad de medallas: %i\n Pokemones:\n", personaje->nombre, personaje->medallas);
    lista_con_cada_elemento(personaje->equipo, funcion, &contador);
    printf("Presione algun boton para avanzar\n");
    scanf(" %c", &ingreso);
}

void mostrar_gimnasio(gimnasio_t* gimnasio){
    char ingreso;
    printf("Informacion del gimnasio a combatir:\n    Nombre: %s\n", gimnasio->nombre);
    printf("    Dificultad: %i\n    Funcion de Combate: %i\n", gimnasio->dificultad, gimnasio->id_puntero_a_funcion);
    printf("Presione algun boton para avanzar\n");
    scanf(" %c", &ingreso);
}

void mostrar_pokemones(personaje_t* personaje){
    bool (*funcion_1)(void*, void*) = mostrar_pokemones_lista;
    bool (*funcion_2)(void*, void*) = mostrar_pokemones_arbol;
    int contador = 0;
    int contador_arbol = 0;
    char ingreso;

    printf("Pokemones del equipo:\n");
    lista_con_cada_elemento(personaje->equipo, funcion_1, &contador);
    printf("Todos tus pokemones:\n");
    abb_con_cada_elemento(personaje->pokemones, 0, funcion_2, &contador_arbol);

    printf("Presione algun boton para avanzar\n");
    scanf(" %c", &ingreso);
}
bool es_pokemon_valido(size_t pokemon_a_cambiar, size_t cantidad_pokemones_equipo){
    return (pokemon_a_cambiar <= cantidad_pokemones_equipo);
}

bool es_ingreso_valido(char ingreso){
    return (ingreso == 's' || ingreso == 'n');
}
void pedir_pokemon_a_sacar(personaje_t* personaje, size_t* pokemon_a_cambiar){
    printf("Ingrese el numero del pokemon del equipo que desea sacar\n");
    scanf("%lui", pokemon_a_cambiar);
    while(!es_pokemon_valido(*pokemon_a_cambiar, personaje->equipo->cantidad)){
        printf("La posicion ingresada no existe. Revise el equipo e ingrese nuevamente\n");
        scanf("%lui", pokemon_a_cambiar);
    }
}

int modificar_equipo(personaje_t* personaje){
    size_t pokemon_a_cambiar = 0;
    char nombre_pokemon[MAX_NOMBRE];
    char ingreso;
    pokemon_t* pokemon_aux = crear_pokemon();
    pokemon_t* pokemon_a_sacar;
    pokemon_t* pokemon;

    
    mostrar_pokemones(personaje);
    pedir_pokemon_a_sacar(personaje, &pokemon_a_cambiar);

    printf("Ingrese el nombre del pokemon por el que lo desea cambiar\n");
    scanf("%s", nombre_pokemon);
    strcpy(pokemon_aux->nombre, nombre_pokemon);
    pokemon = arbol_buscar(personaje->pokemones, pokemon_aux);
    
    while (pokemon == NULL || pokemon->esta_en_equipo){
        printf("Pokemon no encontrado o ya pertenece a tu equipo. Revise y vuelva a intentar\n");
        scanf("%s", nombre_pokemon);
        strcpy(pokemon_aux->nombre, nombre_pokemon);
        pokemon = arbol_buscar(personaje->pokemones ,pokemon_aux);
    }
    pokemon_a_sacar = lista_elemento_en_posicion(personaje->equipo, pokemon_a_cambiar-1);
    if(!pokemon_a_sacar){
        return ERROR;
    }
    pokemon_a_sacar->esta_en_equipo = false;
    pokemon->esta_en_equipo = true;
    if(lista_borrar_de_posicion(personaje->equipo, pokemon_a_cambiar-1) == ERROR){
        return ERROR;
    }
    if(lista_insertar_en_posicion(personaje->equipo, pokemon, pokemon_a_cambiar-1) == ERROR){
        return ERROR;
    } 
    printf("Intercambio realizado correctamente\n Desear seguir cambiando pokemones? Ingrese s si lo desea, n si no\n");
    scanf(" %c", &ingreso);
    while(!es_ingreso_valido(ingreso)){
        printf("Ingreso invalido, por favor vuelva a intentar\n");
        scanf(" %c", &ingreso);
    }
    if(ingreso == 's'){
        modificar_equipo(personaje);
    }
    destructor_pokemones(pokemon_aux);
    return 0;
}
void mostrar_pokemones_combate(pokemon_t* pokemon){

    printf("    Nombre: %s\n    Velocidad: %i\n", pokemon->nombre,pokemon->velocidad);
    printf("    Ataque: %i\n    Defensa: %i\n\n",  pokemon->ataque,pokemon->defensa);
    
}
bool ingreso_combate_valido(char ingreso){
    return ingreso == 'N';
}

void pedir_ingreso(char ingreso){
    printf("Presione N para poceder al siguiente combate\n");
    scanf(" %c", &ingreso);
    while(!ingreso_combate_valido(ingreso)){
        printf("Ingreo invalido, vuelvalo a intentar\n");
        scanf("%c", &ingreso);
    }
}

void mostrar_menu_batalla(pokemon_t* pokemon_personaje, pokemon_t* pokemon_enemigo, int resultado_batalla){
    char ingreso = ' ';
    system("clear");
    printf("Pokemon 1\n");
    mostrar_pokemones_combate(pokemon_personaje);
    printf("Pokemon 2\n");
    mostrar_pokemones_combate(pokemon_enemigo);
    if(resultado_batalla > 0){
        printf("Tu pokemon ha vencido, preparate para el siguiente combate\n\n");
    }
    else{
        printf("Tu pokemon ha sido derrotado, preparate para el siguiente combate\n\n");
    }
    pedir_ingreso(ingreso);
}

void subir_nivel_pokemon(pokemon_t* pokemon){
    if(pokemon->nivel <= 63){
        pokemon->ataque ++;
        pokemon->defensa ++;
        pokemon->velocidad++;
        pokemon->nivel ++;
    }
}

bool quedan_pokemones_en_equipo(lista_t* equipo, size_t posicion_pokemon){
    return (equipo->cantidad > posicion_pokemon);
}


bool ingreso_derrota_valido(char ingreso){
    return (ingreso == 'C' || ingreso == 'R' || ingreso == 'F');
}

void pedir_ingreso_derrota(char* ingreso){
    scanf("%c", ingreso);
    while(!ingreso_derrota_valido(*ingreso)){
        printf("Ingreso invalido, intente nuevamente\n");
        scanf("%c", ingreso);
    }
}

int acciones_derrota(personaje_t* personaje, char* estado_juego){
    char ingreso = ' ';
    mostrar_menu_derrota();
    pedir_ingreso_derrota(&ingreso);

    if(ingreso == 'C'){
        system("clear");
        if(modificar_equipo(personaje) == ERROR){
            printf("Ha ocurrido un error al modificar el equipo, lo sentimos reintentelo\n");
            return ERROR;
        }
    }
    if(ingreso == 'R'){
        printf("Volveras al menu de gimnasio para reintentarlo desde el entrenador que lo dejaste\n");
        sleep(1);
    }
    if(ingreso == 'F'){
        *estado_juego = PERDIDO;
        printf("Muchas gracias por haber jugado a nuestro juego, esperamos que vuelvas a jugar\n");
        sleep(1);
    }
    return 0;
}

bool ingreso_victoria_valido(char ingreso){
    return (ingreso == 'T' || ingreso == 'C' || ingreso == 'N');
}

void pedir_ingreso_victoria(char* ingreso){
    scanf("%c", ingreso);
    while(!ingreso_victoria_valido(*ingreso)){
        printf("Ingreso invalido, intente nuevamente\n");
        scanf("%c", ingreso);
    }
}

int acciones_victoria(personaje_t* personaje, char* estado_juego, entrenador_t* entrenador){
    char ingreso = ' ';
    
    mostrar_menu_victoria(*estado_juego);
    if(*estado_juego == JUGANDO){
        pedir_ingreso_victoria(&ingreso);

        if(ingreso == 'C'){
            system("clear");
            if(modificar_equipo(personaje) == ERROR){
                printf("Ha ocurrido un error al modificar el equipo, lo sentimos reintentelo\n");
                return ERROR;
            }
        }
        if(ingreso == 'T'){
            /*
            *   ver robar a lider
            * 
            */ 
        }
        if(ingreso == 'N'){
            return 1;
        }
    }
    else{
        printf("Presione cualquier boton para terminar su partida\n");
        scanf(" %c", &ingreso);
    }
    return 1;
}

int batalla_gimnasio(personaje_t* personaje, gimnasio_t* gimnasio, char* estado_juego, int cantidad_gimnasios){
    int resultado_batalla = 0;
    size_t posicion_pokemon_personaje = 0;
    size_t posicion_pokemon_enemigo = 0;
    entrenador_t* entrenador;
    pokemon_t* pokemon_personaje;
    pokemon_t* pokemon_enemigo;

    while (!lista_vacia(gimnasio->entrenadores) && quedan_pokemones_en_equipo(personaje->equipo, posicion_pokemon_personaje)){
        posicion_pokemon_personaje = 0;
        posicion_pokemon_enemigo = 0;
        
        entrenador = lista_tope(gimnasio->entrenadores);

        while(quedan_pokemones_en_equipo(entrenador->equipo, posicion_pokemon_enemigo) && quedan_pokemones_en_equipo(personaje->equipo, posicion_pokemon_personaje)){
            pokemon_personaje = lista_elemento_en_posicion(personaje->equipo, posicion_pokemon_personaje);
            pokemon_enemigo = lista_elemento_en_posicion(entrenador->equipo, posicion_pokemon_enemigo);
            
            resultado_batalla = batallas(pokemon_personaje, pokemon_enemigo, (gimnasio->id_puntero_a_funcion)-1);
            mostrar_menu_batalla(pokemon_personaje, pokemon_enemigo, resultado_batalla);

            if(resultado_batalla < 0){
                posicion_pokemon_personaje ++;
            }
            else{
                posicion_pokemon_enemigo++;
                subir_nivel_pokemon(pokemon_personaje);
            }
        }
        if(quedan_pokemones_en_equipo(personaje->equipo, posicion_pokemon_personaje)){
            printf("Has vencido a este entrenador, preparate para el siguiente\n");
            sleep(1);
            lista_desapilar(gimnasio->entrenadores);
        }
    }

    if(!quedan_pokemones_en_equipo(personaje->equipo, posicion_pokemon_personaje)){
        return acciones_derrota(personaje, estado_juego);  
    }
    else{
        personaje->medallas ++;
        if(cantidad_gimnasios <= personaje->medallas){
            *estado_juego = GANADO;
        }
        return acciones_victoria(personaje, estado_juego,entrenador);
    }
    return 1;
}

int jugar_aventura(juego_t* juego){
    char ingreso;
    gimnasio_t* gimnasio = heap_extraer_raiz(juego->gimnasios);
    while (juego->estado_juego == JUGANDO){
        int resultado_batalla_gimnasio = 0;
        mostrar_menu_gimnasio();
        
        scanf(" %c", &ingreso);
        while (!ingreso_valido(ingreso)){
            printf("Ingreso invalido, vea los ingresos posibles.\n");
            scanf(" %c", &ingreso);
        }
        if(ingreso == 'E'){
            system("clear");
            mostrar_personaje(juego->personaje);
        }
        if(ingreso == 'G'){
            system("clear");
            mostrar_gimnasio(gimnasio);
        }
        if(ingreso == 'C'){
            system("clear");
            if(modificar_equipo(juego->personaje) == ERROR){
                printf("Ha ocurrido un error al modificar el equipo, lo sentimos reintentelo\n");
                return ERROR;
            }
        }
        if(ingreso == 'B'){
            if(!gimnasio){
                return ERROR;
            }
            resultado_batalla_gimnasio = batalla_gimnasio(juego->personaje, gimnasio, &(juego->estado_juego), juego->cantidad_gimnasios);
            
            if(resultado_batalla_gimnasio == 1){
                if(juego->estado_juego == JUGANDO){                
                    gimnasio = heap_extraer_raiz(juego->gimnasios);
                }
            }
            if(resultado_batalla_gimnasio == ERROR){
                return ERROR;
            }
        }
        sleep(1);
    }
    return 0;
}

