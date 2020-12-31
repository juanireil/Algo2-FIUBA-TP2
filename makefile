CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
FLAGS_VALGRIND = --leak-check=full --track-origins=yes --show-reachable=yes 

pruebas_hash: pruebas.c hash.c pa2mm.h
	$(CC) pruebas.c hash.c -o hash $(CFLAGS)

.PHONY : clean
clean : 
	-rm abb

debug_hash : pruebas_hash
	gdb ./hash

valgrind_hash: pruebas_hash
	valgrind $(FLAGS_VALGRIND) ./hash
