# Trabajo Practico Algoritmos y Programacion 2
# La gran aventura pokemon
## Explicacion del Trabajo:

Este trabajo es una version simplificada del juego de pokemon. En este se cargará a partir de archivos de texto la información de los gimnasios y del personaje principal para jugar. Ya con esta información se desarrollará el juego simulando las batallas. Para esto cada gimnasio posee un id de funcion de batalla. Estas se encuentran en _batallas.c_ y se usarán segun el id que posea el gimnsio. El jugdor podrá elegir jugar el juego el tomando decisiones sobre el equipo entre otras cosas o simularlo simplemente y solo saber si gano o perdió el juego. Para la interacción con el usuario fueron desarrollados diversos menús.
### Implementacion:

Para esta implementación se opto por usar las siguientes estructuras:
{
    
    typedef struct personaje{
        char nombre[MAX_NOMBRE];
        int medallas;
        lista_t* equipo; //Implementado como lista
        abb_t* pokemones;
    }personaje_t;

    typedef struct entrenador{
        char nombre [MAX_NOMBRE];
        lista_t* equipo;
        bool es_lider;
    }entrenador_t;

    typedef struct gimnasio{
        char nombre[MAX_NOMBRE];
        int dificultad;
        int id_puntero_a_funcion;
        lista_t* entrenadores; //Implementado como pila
    }gimnasio_t;

    typedef struct juego{
        personaje_t* personaje;
        heap_t* gimnasios;
        char estado_juego;
        int cantidad_gimnasios;
        char modo_de_juego; 
    }juego_t;
}


Se opto por implementar todos los equipos de pokemones como listas. Esto es debido a que en el caso del personaje principal es necesario poder cambiar cualquier pokemon del equipo y mostrarlos a todos. Estas acciones requieren recorrer la lista cosa que los imposibilita de ser usados como pila o cola.
En el caso de los entrenador restantes tambien se usa la lista por que está la posibilidad de tomar un pokemon del lider. Para esto utilizar una cola o pila nos limitará mucho.

Los demas pokemones que posee el personaje principal fueron guardados en un arbol de busqueda binario. Se eligió este ya que puede ser necesario acceder a cualquiera de estos pokemones y recorrerlo. Esto ya limita a usar abb o lista. Se opto por la lista ya que pueden haber muchos pokemones y en caso de que el arbol no se deforme a lista la complejidad algoritmica de buscar un pokemon en el arbol es menor que en la lista volviendolo así mas eficiente.

En los gimnasios, se posee una pila de los entrenadores que lo componen. Fue elegida la pila debido a que los entrenadores se leen del archivo y el primero en ser leído es el primero en agregarse y el ultimo en enfrentarse. Siendo este el funcionamiento de la pila volviendolo así el ideal en este caso.

Por último se posee una estructura _juego_t con el fin de encapsular mejor el juego.

### Compilación y ejecución:
Para simplificar la compilación y ejecución del programa opté por armar un makefile. Con este puedo correr las pruebas del heap, las pruebas del juego y el juego.

Para correr las pruebas del heap el comando a usar es:

- _make pruebas_heap_

- _make valgrind_heap_

Para correr las pruebas del juego el comando es:

- _make pruebas_aventura_

- _make valgrind_prueba_

Por último para correr el juego el comando es:

- _make aventura_pokemon_

- _make aventura_

Si se desea correrlo con valgrind el comando es:

- _make valgrind_aventura_

Además esta la posibilidad de realizar un debugging con gdb, para esto en el comando se debe reemplazar valgrind por debug.

Por ultimo si se desea borrar todo lo generado en las compilaciones anteriores simplemente se ejecuta el comando

- _make clean_

En los casos en que se compila, la linea de compilacion es:

- _gcc abb.c lista.c batallas.c aventura_pokemon.c heap.c pruebas_aventura.c  -o prueba_aventura -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0_

(Aclaración: Este es un ejemplo de la compilacion para jugar al juego. Los nombres a poner son los de los archivos a compilar, el heap solo requiere heap.c y pruebas.c)

Para la ejecución con valgrind la linea de comando que se ejecuta es:

- _valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./prueba_aventura

### Consideraciones tomadas:
- En las batallas en caso de empate el gandaor es el segundo pokemon que se recibe, en este caso el del enemigo.
- Si se lee un entrenador sin pokemones no se lo cargará y se lo ignorara.
- Si en el archivo de gimnsaio hay un entrenador antes del lider estos son ignorados.
- No se carga un gimnasio sin lider, el cual ademas debe tener al menos 1 pokemon.
- Al cargar un gimnasio si el id de batalla o la dificultad son invalidos se devuelve que hubo un error y no se carga.
- Al cargar un personaje este debe tener por lo menos 1 pokemon, sino devuekve error.
- Cada vez que un pokemon gana una batalla este aumenta su nivel en 1 al igual que todas sus estadísticas hasta llegar al nivel 63.
- Todos los pokemones que se reciben empiezan en el nivel 0.
- En el caso de simular el jugador sala verpa si gano o perdio, no tendrá acceso a ninguno de los menús para modificarn ni siquiera el de derrota.
- En caso de un error mientras se esta jugando se devolvera error, se notificara de este y se abortara el juego.
- Fueron armados los distintos destructores y comparadores con el tipo void* en lugar del especifico para poder usarlos en los TDAS hechos con anterioridad sin perder generalidad.
- Al TDA lista se le agrego un destructor para poder liberar la memorio de lo almacenado dentro de los nodos sin necesidad de recorrerla.
- En el caso del personaje no se pasa destructor a la lista de equipo ya que los pokemones del equipo tambien estan en los obtenidos y ya se liberan allí.
- En caso de que un entrenador de gimnasio posea mas de 6 pokemones solo se cargaran los primeros 6 en leerse, el resto serán ignorados.
