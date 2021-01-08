#include "pa2mm.h"
#include "aventura_pokemon.h"
#include <stdlib.h>
#include <string.h>

void probar_funciones_batalla(){
    pokemon_t* pkm_1 = malloc(sizeof(pokemon_t));
    pokemon_t* pkm_2 = malloc (sizeof(pokemon_t));
    
    strcpy(pkm_1->nombre, "Charizard");
    pkm_1->ataque = 90;
    pkm_1->defensa = 90;
    pkm_1->velocidad = 90;

    strcpy(pkm_2->nombre, "Eeve");
    pkm_2->ataque = 20;
    pkm_2->defensa = 20;
    pkm_2->velocidad = 20;

    pa2m_afirmar(funcion_batalla_1(pkm_1, pkm_2) == 1, "Funcion batalla 1 devolvió correctamente");
    pa2m_afirmar(funcion_batalla_2(pkm_1, pkm_2) == 1, "Funcion batalla 2 devolvió correctamente");
    pa2m_afirmar(funcion_batalla_3(pkm_1, pkm_2) == 1, "Funcion batalla 3 devolvió correctamente");
    pa2m_afirmar(funcion_batalla_4(pkm_1, pkm_2) == 1, "Funcion batalla 4 devolvió correctamente");
    pa2m_afirmar(funcion_batalla_5(pkm_1, pkm_2) == 1, "Funcion batalla 5 devolvió correctamente"); 
    
}

int main(){
    pa2m_nuevo_grupo("PRUEBAS CREACION DE HEAP");
    probar_funciones_batalla();
    pa2m_mostrar_reporte();
    return 0;
}
