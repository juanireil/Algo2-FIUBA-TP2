CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
FLAGS_VALGRIND = --leak-check=full --track-origins=yes --show-reachable=yes 

pruebas_heap: pruebas.c heap.c pa2mm.h
	$(CC) pruebas.c heap.c -o heap $(CFLAGS)

.PHONY : clean
clean : 
	-rm abb

debug_heap : pruebas_heap
	gdb ./heap

valgrind_heap: pruebas_heap
	valgrind $(FLAGS_VALGRIND) ./heap


pruebas_aventura: abb.c lista.c batallas.c aventura_pokemon.c heap.c pruebas_aventura.c pa2mm.h
	$(CC)   abb.c lista.c batallas.c aventura_pokemon.c heap.c pruebas_aventura.c -o prueba_aventura $(CFLAGS)

valgrind_prueba: prueba_aventura
	valgrind $(FLAGS_VALGRIND) ./prueba_aventura

aventura_pokemon: abb.c lista.c batallas.c aventura_pokemon.c heap.c aventura.c pa2mm.h
	$(CC)   abb.c lista.c batallas.c aventura_pokemon.c heap.c aventura.c -o aventura_pokemon $(CFLAGS)

debug_aventura: aventura_pokemon
	gdb ./aventura_pokemon

aventura: aventura_pokemon
	./aventura_pokemon
