# Trabajo Practico Algoritmos y Programacion 2
# La gran aventura pokemon
## Explicacion del Trabajo:
### Implementacion:


### Compilación y ejecución:
Para simplificar la compilación y ejecución del programa opté por armar un makefile. En este, dividí en dos casos diferentes, uno en el que se quiera ejecutar usando las minipruebas y el otro en el que se desee ejecutar usando mis pruebas.

Para el caso de las minipruebas se debe ejecutar primero el comando:

- _make minipruebas_

Posteriormente se ejecutara el comando:

- _make valgrind_minipruebas_

Con estos dos comandos ya se habrá compilado y ejecutado mostrando asi lo esperado por pantalla. En el caso de querer ejecutar mis pruebas las lineas de comando son las mismas reemplazando minipruebas por pruebas. En este caso seria:

- _make pruebas_

- _make valgrind_pruebas_

Además, en el make esta la opcion de realizar un debugging usando gdb si se reemplaza valgrind por debug en la linea de comando. Por ultimo si se desea borrar todo lo generado en las compilaciones anteriores simplemente se ejecuta el comando

- _make clean_

En los casos en que se compila, la linea de compilacion es:

- _gcc pruebas.c abb.c -o abb -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0_

(Aclaración: Los nombres a poner son los de los archivos a compilar si se desean las minipruebas se pone en lugar de pruebas.c)

Para la ejecución con valgrind la linea de comando que se ejecuta es:

- _valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./abb

### Consideraciones tomadas:

