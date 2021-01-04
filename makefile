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
