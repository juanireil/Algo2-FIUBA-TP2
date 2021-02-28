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
static const char JUGAR = 'I', SIMULAR = 'S';
const char GIMNASIO = 'G', LIDER = 'L', POKEMON = 'P', ENTRENADOR = 'E';


void destructor_pokemones(void* pokemon){
    if(!pokemon){
        return;
    }
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

personaje_t* crear_personaje(){
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

void destruir_personaje(personaje_t* personaje){
    arbol_destruir(personaje->pokemones);
    lista_destruir(personaje->equipo);
    free(personaje);
}

pokemon_t* crear_pokemon(){
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

bool es_leido_valido(char dato_leido){
    return (dato_leido == POKEMON || dato_leido == ENTRENADOR);
}
personaje_t* cargar_personaje(char nombre_archivo[MAX_ARCHIVO]){
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
    while (leidos == 1 && es_leido_valido(dato_a_guardar)){
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
    if(!es_leido_valido(dato_a_guardar)){
        destruir_personaje(personaje);
        printf("Archivo invalido no puede ser cargado");
        sleep(1);
        return NULL;
    }
    if(lista_vacia(personaje->equipo)){
        destruir_personaje(personaje);
        printf("Personaje sin pokemones no se puede cargar");
        sleep(1);
        return NULL;
    }
    return personaje;
}

void destructor_entrenadores(void* entrenador){
    if(!entrenador){
        return;
    }
    lista_destruir(((entrenador_t*)entrenador)->equipo);
    free(entrenador);
}

gimnasio_t* crear_gimnasio(){
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

entrenador_t* crear_entrenador(){
    entrenador_t* entrenador = malloc(sizeof(entrenador_t));
    if(!entrenador){
        return NULL;
    }

    entrenador->equipo = lista_crear(destructor_pokemones);
    if(!entrenador->equipo){
        free(entrenador);
        return NULL;
    }
    strcpy(entrenador->nombre, " ");
    entrenador->es_lider = false;
    return entrenador;
}

/*
 * Recibe el dato leído del archivo.
 * Devólverá True si es valido (L,E,G o Ṕ), False en caso contrario.
 */
bool es_dato_valido(char dato_a_guardar){
    return (dato_a_guardar == LIDER || dato_a_guardar == ENTRENADOR || dato_a_guardar == GIMNASIO || dato_a_guardar == POKEMON);
}
/*
 * Recibe un id de batalla leído del archivo.
 * Devólverá True si es valido (entero del 1 al 5), False en caso contrario.
 */
bool es_id_batalla_valido(int id_batalla){
    return ((id_batalla > 1) && (id_batalla < 6));
}

/*
 * Recibe la dificultad del gimnasio leída del archivo.
 * Devólverá True si es valido (entero positivo), False en caso contrario.
 */
bool es_dificultad_valida(int dificultad){
    return dificultad > 0;
}

/*
 * Recibe una lista de entrenadores.
 * Devólverá true si la lista posee un lider, false en caso contrario.
 */
bool hay_lider(lista_t* entrenadores){
    entrenador_t* entrenador = lista_elemento_en_posicion(entrenadores, 0);
    if(!entrenador){
        return false;
    }
    
    return entrenador->es_lider;
}

int cargar_gimnasios(heap_t* heap_gimnasios, char nombre_gimnasio[MAX_ARCHIVO]){
    char dato_a_guardar;
    bool hay_gimnasio = false;
    if(!heap_gimnasios){
        printf("Heap gimnasios ingresado es invalido");
        return ERROR;
    }
    gimnasio_t* gimnasio = crear_gimnasio();
    if(!gimnasio){
        return ERROR;
    }
    entrenador_t* entrenador = crear_entrenador();
    if(!entrenador){
        destructor_de_gimnasios(gimnasio);
        return ERROR;
    }
    pokemon_t* pokemon = crear_pokemon();

    if(!pokemon){
        destructor_de_gimnasios(gimnasio);
        lista_destruir(entrenador->equipo);
        free(entrenador);
        return ERROR;
    }
    FILE* archivo_gimnasio = fopen(nombre_gimnasio, "r");
    if(!archivo_gimnasio){
        printf("No se pudo abrir el archivo. Revise y vuelva a intentar.\n");
        lista_destruir(entrenador->equipo);
        free(entrenador);
        destructor_de_gimnasios(gimnasio);
        destructor_pokemones(pokemon);

        return ERROR;
    }
    int leidos = fscanf(archivo_gimnasio, "%c;", &dato_a_guardar);
    if (leidos != 1){
        lista_destruir(entrenador->equipo);
        free(entrenador);
        destructor_de_gimnasios(gimnasio);
        destructor_pokemones(pokemon);
        fclose(archivo_gimnasio);
        return ERROR;
    }
    while(leidos == 1 && es_dato_valido(dato_a_guardar)){
        if (dato_a_guardar == GIMNASIO){
            if(lista_elementos(gimnasio->entrenadores) > 0){
                lista_apilar(gimnasio->entrenadores, entrenador);
                heap_insertar(heap_gimnasios, gimnasio);
                gimnasio = crear_gimnasio();
                entrenador = crear_entrenador();
            }
            fscanf(archivo_gimnasio, FORMATO_GIMNASIOS, gimnasio->nombre, &(gimnasio->dificultad), &(gimnasio)->id_puntero_a_funcion);
            if(!es_id_batalla_valido(gimnasio->id_puntero_a_funcion) || !es_dificultad_valida(gimnasio->dificultad)){
                printf("Gimnasio invalido, no puede ser cargado\n");
                sleep(1);
                lista_destruir(entrenador->equipo);
                free(entrenador);
                destructor_de_gimnasios(gimnasio);
                destructor_pokemones(pokemon);
                fclose(archivo_gimnasio);
                return ERROR;
            }
            hay_gimnasio = true;
        }
        if(dato_a_guardar == LIDER && hay_gimnasio){
            if(lista_elementos(entrenador->equipo) > 0){
                lista_apilar(gimnasio->entrenadores, entrenador);
                entrenador = crear_entrenador();
            }
            fscanf(archivo_gimnasio, "%50[^\n]\n", entrenador->nombre);
            entrenador->es_lider = true; 
        }
        if(dato_a_guardar == ENTRENADOR){
            if(lista_elementos(entrenador->equipo) > 0){
                lista_apilar(gimnasio->entrenadores, entrenador);
                entrenador = crear_entrenador();
            }
            if((hay_lider(gimnasio->entrenadores))){
                fscanf(archivo_gimnasio, "%50[^\n]\n", entrenador->nombre);
            }
        }
        if(dato_a_guardar == POKEMON){
            fscanf(archivo_gimnasio, FORMATO_POKEMON, (*pokemon).nombre, &((*pokemon).ataque), &((*pokemon).defensa), &((*pokemon).velocidad));
            if(entrenador->equipo->cantidad < 6){
                lista_insertar(entrenador->equipo,pokemon);    
                pokemon->esta_en_equipo = true;
                pokemon = crear_pokemon();
            }
        }

        leidos = fscanf(archivo_gimnasio, "%c;", &dato_a_guardar);
    }
    if(!es_dato_valido(dato_a_guardar)){
        printf("El archivo ingresado no es valido para esta accion.\n");
        sleep(1);
        lista_destruir(entrenador->equipo);
        free(entrenador);
        destructor_de_gimnasios(gimnasio);
        destructor_pokemones(pokemon);
        fclose(archivo_gimnasio);
        return ERROR;
    }
    destructor_pokemones(pokemon);
    lista_apilar(gimnasio->entrenadores, entrenador);
    heap_insertar(heap_gimnasios, gimnasio);

    fclose(archivo_gimnasio);
    return 1;
}

void mostrar_menu_victoria(char estado_juego, bool saco_pokemon_lider){
    system("clear");
    if(estado_juego == JUGANDO){
        printf("Felicitaciones!!! \n Has vencido este ginasio.\n");
        if(saco_pokemon_lider == false){
            printf("_T Toma un pokemon del lider e incorporalo a tu equipo\n");
        }
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

/*
 * Recibe el dato ingresado por teclado en el menú de gimnasio.
 * Devólverá si True si es valido (C,E,G o B), False en caso contrario.
 */
bool ingreso_valido(char ingreso){
    return (ingreso == 'E' || ingreso == 'C' || ingreso == 'G' || ingreso == 'B');
}

/*
 * Funcion para el iterador interno.
 * Recibe el puntero a un pokemon y un puntero a un contador.
 * Devólverá true siempre (es necesaria su devolucion para el iterador).
 * Mostrará el pokemon y su informacion.
 */
bool mostrar_pokemones_lista(void* pokemon, void* contador){
    if(pokemon && contador){
        (*(int*)contador)+=1;
        printf("    Pokemon N° %i\n", (*(int*)contador));
        printf("    Nombre: %s\n    Velocidad: %i\n", ((pokemon_t*)pokemon)->nombre,((pokemon_t*)pokemon)->velocidad);
        printf("    Ataque: %i\n    Defensa: %i\n",  ((pokemon_t*)pokemon)->ataque,((pokemon_t*)pokemon)->defensa);
        printf("    Nivel: %i\n\n",((pokemon_t*)pokemon)->nivel);
    }
    return true;
}

/*
 * Funcion para el iterador interno.
 * Recibe el puntero a un pokemon y un puntero a un contador.
 * Devólverá false siempre (es necesaria su devolucion para el iterador).
 * Mostrará el pokemon y su informacion siempre que este no este en el equipo.
 */
bool mostrar_pokemones_arbol(void* pokemon, void* contador){
    if(pokemon && contador){
        (*(int*)contador)+=1;
        if(!((pokemon_t*)pokemon)->esta_en_equipo){
            printf("    Nombre: %s\n    Velocidad: %i\n", ((pokemon_t*)pokemon)->nombre,((pokemon_t*)pokemon)->velocidad);
            printf("    Ataque: %i\n    Defensa: %i\n\n",  ((pokemon_t*)pokemon)->ataque,((pokemon_t*)pokemon)->defensa);
            printf("    Nivel: %i\n\n",((pokemon_t*)pokemon)->nivel);
        }
    }
    return false;
}

/*
 * Recibe un personaje principal.
 * Mostrará al personaje, su informacion y los pokemones pertenecientes a su equipo con su información.
 */
void mostrar_personaje(personaje_t* personaje){
    bool (*funcion)(void*, void*) = mostrar_pokemones_lista;
    int contador = 0;
    char ingreso;

    printf("Nombre: %s\n Cantidad de medallas: %i\n Pokemones:\n", personaje->nombre, personaje->medallas);
    lista_con_cada_elemento(personaje->equipo, funcion, &contador);
    printf("Presione algun boton para avanzar\n");
    scanf(" %c", &ingreso);
}

/*
 * Recibe un gimnasio valido.
 * Mostrará el gimnasio y su informacion.
 */
void mostrar_gimnasio(gimnasio_t* gimnasio){
    char ingreso;
    printf("Informacion del gimnasio a combatir:\n    Nombre: %s\n", gimnasio->nombre);
    printf("    Dificultad: %i\n    Funcion de Combate: %i\n", gimnasio->dificultad, gimnasio->id_puntero_a_funcion);
    printf("Cantidad de entrenadores:%lu\n",gimnasio->entrenadores->cantidad);
    printf("Presione algun boton para avanzar\n");
    scanf(" %c", &ingreso);
}

/*
 * Recibe un personaje valido.
 * Mostrará los pokemones pertenecientes al equipo del personaje y todos los obtenidos que no esten en el equipo.
 */
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

/*
 * Recibe la posicion de un pokemon y la cantidad de pokemones del equipo.
 * Devuelve true si la posicion existe en el equipo o false si no.
 */
bool es_pokemon_valido(size_t pokemon_a_cambiar, size_t cantidad_pokemones_equipo){
    return (pokemon_a_cambiar <= cantidad_pokemones_equipo);
}

/*
 * Recibe el ingreso por teclado.
 * Devuelve true si es valido(s o n) o false si no.
 */
bool es_ingreso_valido(char ingreso){
    return (ingreso == 's' || ingreso == 'n');
}

/*
 * Recibe el personaje. Pedirá al usuario la posicion en el equipo del pokemon
 * que se desea sacar. Pedirá la posicion hasta que esta sea valida.
 */
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

/*
 * Recibe el puntero a un pokemon y lo mostrará por pantalla junto a sus estadisticas.
 */
void mostrar_pokemones_combate(pokemon_t* pokemon){

    printf("    Nombre: %s\n    Velocidad: %i\n", pokemon->nombre,pokemon->velocidad);
    printf("    Ataque: %i\n    Defensa: %i\n\n",  pokemon->ataque,pokemon->defensa);
    
}

/*
 * Recibe un ingreso. Devolverá true si es valido (n), o false si no lo es.
 */
bool ingreso_combate_valido(char ingreso){
    return ingreso == 'N';
}

/*
 * Pide al usuario que ingrese ingrese para seguir con el juego. Lo repetirá hasta que el
 * ingreso sea valido.
 */
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

/*
 * Recibe el ingreso por teclado del usuario.
 * Devuelve true si es valido(C, R o F), false en caso contrario.
 */
bool ingreso_derrota_valido(char ingreso){
    return (ingreso == 'C' || ingreso == 'R' || ingreso == 'F');
}

/*
 * Pide al usuario el ingreso por teclado.
 * Repite la accion hasta que el ingreso realizado sea uno valido.
 */
void pedir_ingreso_derrota(char* ingreso){
    scanf("%c", ingreso);
    while(!ingreso_derrota_valido(*ingreso)){
        printf("Ingreso invalido, intente nuevamente\n");
        scanf("%c", ingreso);
    }
}

/*
 * Recibe un puntero al personaje, el estado del juego y el modo en el que se esta jugando.
 * Ofrecerá la informacion correspondiente según la situación del juego. Si el modo es simulacion
 * solo se notificará de la derrota. Si el modo es el de jugar se ofrecerán las opciones del meníú de
 * derrota.
 * Devuelve -1 en caso de error 0 en caso contrario. Si el jugador decide dejar el juego ahí se pone estado juego
 * en perdido.
 */
int acciones_derrota(personaje_t* personaje, char* estado_juego, char modo_de_juego){
    char ingreso = ' ';
    if(modo_de_juego == SIMULAR){
        system("clear");
        *estado_juego = PERDIDO;
        printf("Tu simulacion ha terminado en una derrota.\nMuchas gracias por haber jugado esperamos que vuelvas a jugar\n");
        sleep(1);
        return 0;
    }
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

/*
 * Recibe el ingreso por teclado del usuario.
 * Devuelve true si es valido(C, N o T), false en caso contrario.
 */
bool ingreso_victoria_valido(char ingreso){
    return (ingreso == 'T' || ingreso == 'C' || ingreso == 'N');
}

/*
 * Pide al usuario ingresar una de las opciones de victoria.
 * Esto se repetirá hasta que lo que se ingrese sea valido.
 */
void pedir_ingreso_victoria(char* ingreso){
    scanf("%c", ingreso);
    while(!ingreso_victoria_valido(*ingreso)){
        printf("Ingreso invalido, intente nuevamente\n");
        scanf("%c", ingreso);
    }
}

/*
 * Recibe un puntero al lider del gimnasio.
 * Mostrara los pokemones de su equipo y sus estadísticas.
 */
void mostrar_pokemones_lider(entrenador_t* entrenador){
    bool (*funcion)(void*, void*) = mostrar_pokemones_lista;
    int contador = 0;

    printf("Nombre: %s\n Pokemones:\n", entrenador->nombre);
    lista_con_cada_elemento(entrenador->equipo, funcion, &contador);
} 

/*
 * Recibe un puntero al lider. Pedirá que se ingrese la posicion del pokemon
 * del lider que se desea sacar del equipo.
 * Se repetira hasta que el ingreso sea valido.
 */
void pedir_pokemon_lider(entrenador_t* entrenador, size_t* pokemon_a_cambiar){
    printf("Ingrese el numero del pokemon del lider que desea sacar\n");
    scanf("%lui", pokemon_a_cambiar);
    while(!es_pokemon_valido(*pokemon_a_cambiar, entrenador->equipo->cantidad)){
        printf("La posicion ingresada no existe. Revise el equipo e ingrese nuevamente\n");
        scanf("%lui", pokemon_a_cambiar);
    }
}

/*
 * Recibe un personaje y la posicion de un pokemon de su equipo.
 * Creará una copia del pokemon en esta posicion y delvoverá un puntero a esta copia.
 */
pokemon_t* obtener_pokemon(entrenador_t* entrenador, size_t pokemon_a_cambiar){
    pokemon_t* pokemon = crear_pokemon();
    pokemon_t* pokemon_lider = lista_elemento_en_posicion(entrenador->equipo, pokemon_a_cambiar);

    pokemon->ataque = pokemon_lider->ataque;
    pokemon->defensa = pokemon_lider->defensa;
    pokemon->velocidad = pokemon_lider->velocidad;
    strcpy(pokemon->nombre, pokemon_lider->nombre);
    
    return pokemon;
}

/*
 * Recibe un puntero al personaje, un puntero al lider y el estado del juego y el modo en el que se esta jugando.
 * Ofrecerá la informacion correspondiente según la situación del juego. Si el modo es simulacion
 * solo se notificará de la victoria en caso de haber ganado el juego.
 * Si el modo es el de jugar se ofrecerán las opciones del menu de victoria en caso de no haber ganado el
 * juego todavía. Si ya se ganó se establecerá el estado del juego en ganado.
 * Devuelve -1 en caso de error 1 en caso contrario.
 */
int acciones_victoria(personaje_t* personaje, char* estado_juego, entrenador_t* entrenador, char modo_de_juego){
    bool saco_pokemon_lider = false;
    char ingreso = ' ';
    size_t posicion_pokemon_lider = 0;
    pokemon_t* pokemon;
    if(modo_de_juego == SIMULAR){
        destructor_entrenadores(entrenador);
        if(*estado_juego == GANADO){
            mostrar_menu_victoria(*estado_juego, saco_pokemon_lider);
            printf("Presione cualquier boton para terminar su partida\n");
            scanf(" %c", &ingreso);
        }
    }
    else if(*estado_juego == JUGANDO){
        mostrar_menu_victoria(*estado_juego, saco_pokemon_lider);
        if(modo_de_juego == JUGAR){

            while(ingreso != 'N'){
                mostrar_menu_victoria(*estado_juego, saco_pokemon_lider);
                pedir_ingreso_victoria(&ingreso);
                if(ingreso == 'C'){
                    system("clear");
                    if(modificar_equipo(personaje) == ERROR){
                        destructor_entrenadores(entrenador);
                        printf("Ha ocurrido un error al modificar el equipo, lo sentimos reintentelo\n");
                        return ERROR;
                    }
                }
                if(ingreso == 'T' && saco_pokemon_lider == false){
                    saco_pokemon_lider = true;
                    mostrar_pokemones_lider(entrenador);
                    pedir_pokemon_lider(entrenador, &posicion_pokemon_lider);
                    pokemon = obtener_pokemon(entrenador, posicion_pokemon_lider-1);
                    destructor_entrenadores(entrenador);
                    if(arbol_insertar(personaje->pokemones, pokemon) == ERROR){
                        printf("Ha ocurrido un error al sacar un pokemon del lider, lo sentimos reintentelo\n");
                        return ERROR;
                    }
                    printf("Pokemon agredo a los obtenidos correctamente\n");
                    sleep(1);
                }
                if(ingreso == 'N'){
                    printf("Preparate para combatir contra el siguiente gimnasio\n");
                    sleep(1);
                }
            }
        }
    }
    return 1;
}

int batalla_gimnasio(personaje_t* personaje, gimnasio_t* gimnasio, char* estado_juego, int cantidad_gimnasios, char modo_de_juego){
    int resultado_batalla = 0;
    entrenador_t* entrenador;
    pokemon_t* pokemon_personaje;
    pokemon_t* pokemon_enemigo;

    lista_iterador_t* iterador_personaje = lista_iterador_crear(personaje->equipo);

    while (!lista_vacia(gimnasio->entrenadores) && lista_iterador_tiene_siguiente(iterador_personaje)){
 
        entrenador = lista_tope(gimnasio->entrenadores);

        lista_iterador_t* iterador_enemigo = lista_iterador_crear(entrenador->equipo);

        while(lista_iterador_tiene_siguiente(iterador_personaje) && lista_iterador_tiene_siguiente(iterador_enemigo)){
            pokemon_personaje = lista_iterador_elemento_actual(iterador_personaje);
            pokemon_enemigo = lista_iterador_elemento_actual(iterador_enemigo);
            
            resultado_batalla = batallas(pokemon_personaje, pokemon_enemigo, (gimnasio->id_puntero_a_funcion)-1);
            if(modo_de_juego  == JUGAR){
               mostrar_menu_batalla(pokemon_personaje, pokemon_enemigo, resultado_batalla);
            }

            if(resultado_batalla < 0){
                lista_iterador_avanzar(iterador_personaje);
            }
            else{
                lista_iterador_avanzar(iterador_enemigo);
                subir_nivel_pokemon(pokemon_personaje);
            }
        }
        if(!lista_iterador_tiene_siguiente(iterador_enemigo)){
            if(modo_de_juego == JUGAR){
                printf("Has vencido a este entrenador, preparate para el siguiente\n");
                sleep(1);
            }
            lista_desapilar(gimnasio->entrenadores);
        }
        lista_iterador_destruir(iterador_enemigo);
    }

    if(!lista_iterador_tiene_siguiente(iterador_personaje)){
        lista_iterador_destruir(iterador_personaje);
        return acciones_derrota(personaje, estado_juego, modo_de_juego);  
    }
    else{
        lista_iterador_destruir(iterador_personaje);
        personaje->medallas ++;
        if(cantidad_gimnasios <= personaje->medallas){
            *estado_juego = GANADO;
        }
        return acciones_victoria(personaje, estado_juego,entrenador, modo_de_juego);
    }
    return 1;
}

int jugar_aventura(juego_t* juego){
    char ingreso;
    gimnasio_t* gimnasio = heap_extraer_raiz(juego->gimnasios);
    while (juego->estado_juego == JUGANDO){
        int resultado_batalla_gimnasio = 0;
        if(juego->modo_de_juego == 'I'){
            mostrar_menu_gimnasio();
            
            scanf(" %c", &ingreso);
            while (!ingreso_valido(ingreso)){
                printf("Ingreso invalido, vea los ingresos posibles.\n");
                scanf(" %c", &ingreso);
            }
        }
        else{
            ingreso = 'B';
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
                heap_destruir(juego->gimnasios);
                destruir_personaje(juego->personaje);
                return ERROR;
            }
        }
        if(ingreso == 'B'){
            if(!gimnasio){
                heap_destruir(juego->gimnasios);
                destruir_personaje(juego->personaje);
                return ERROR;
            }
            resultado_batalla_gimnasio = batalla_gimnasio(juego->personaje, gimnasio, &(juego->estado_juego), juego->cantidad_gimnasios, juego->modo_de_juego );
            
            if(resultado_batalla_gimnasio == 1){
                if(juego->estado_juego == JUGANDO){  
                    destructor_de_gimnasios(gimnasio);              
                    gimnasio = heap_extraer_raiz(juego->gimnasios);
                }
            }
            if(resultado_batalla_gimnasio == ERROR){
                heap_destruir(juego->gimnasios);
                destruir_personaje(juego->personaje);
                return ERROR;
            }
        }
        sleep(1);
    }
    destructor_de_gimnasios(gimnasio);
    destruir_personaje(juego->personaje);
    heap_destruir(juego->gimnasios);
    return 0;
}

