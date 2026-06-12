#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "modulos.h"

// mod5_busqueda.c - Modulo 5: Busqueda y equivalencias cifradas
// Busca palabra objetivo, cifra sus apariciones y cuenta equivalencias.
// Complejidad: O(T + q)

#define MAX_EQUIV 100  // maximo de equivalentes distintos a rastrear

// Acumula conteos por equivalente cifrado
typedef struct {
    char equivalente[256];
    char clave_id[10];
    int  mensajes_con_palabra;   // cantidad de mensajes distintos
} Equivalencia;

// Retorna puntero a la clave o NULL si no existe
static Clave* buscar_clave_por_id(const char* clave_id, Clave* db_claves, int total_claves) {
    for (int i = 0; i < total_claves; i++) {
        if (strcmp(db_claves[i].id, clave_id) == 0) return &db_claves[i];
    }
    return NULL;
}

// Busca o agrega el equivalente cifrado y retorna su indice
static int buscar_o_agregar_equiv(Equivalencia* equiv, int* total_equiv,
                                   const char* cifrado, const char* clave_id) {
    for (int i = 0; i < *total_equiv; i++) {
        if (strcmp(equiv[i].equivalente, cifrado) == 0) {
            equiv[i].mensajes_con_palabra++;
            return i;
        }
    }
    
    // Nuevo equivalente
    if (*total_equiv < MAX_EQUIV) {
        int idx = *total_equiv;
        strncpy(equiv[idx].equivalente, cifrado,  254); equiv[idx].equivalente[254] = '\0';
        strncpy(equiv[idx].clave_id,    clave_id,   9); equiv[idx].clave_id[9]     = '\0';
        equiv[idx].mensajes_con_palabra = 1;
        (*total_equiv)++;
        return idx;
    }
    return -1;  // Arreglo lleno
}

// Comparador descendente para qsort
static int cmp_equiv(const void* a, const void* b) {
    return ((Equivalencia*)b)->mensajes_con_palabra
         - ((Equivalencia*)a)->mensajes_con_palabra;
}

void m5_buscar_palabra(const char* objetivo, Mensaje* db_mensajes,
                       int total_mensajes, Clave* db_claves, int total_claves) {

    printf("\n--- Modulo 5: Busqueda de \"%s\" ---\n", objetivo);

    Equivalencia equiv[MAX_EQUIV];
    int total_equiv    = 0;
    int mensajes_total = 0;   // mensajes donde aparece la palabra
    char buffer_cifrado[1024];

    for (int i = 0; i < total_mensajes; i++) {
        // Buscar la palabra en el texto original
        if (strstr(db_mensajes[i].texto_original, objetivo) == NULL) continue;

        mensajes_total++;

        // Cifrar con la clave del mensaje
        Clave *c = buscar_clave_por_id(db_mensajes[i].clave_id, db_claves, total_claves);
        if (c) {
            // Arreglo dummy para las posiciones
            int *pos_dummy = calloc(strlen(objetivo), sizeof(int));
            m2_procesar_texto(objetivo, buffer_cifrado, *c, 0, pos_dummy);
            free(pos_dummy); 
        } else {
            // Si no hay clave, pasar tal cual
            strncpy(buffer_cifrado, objetivo, 1023);
        }

        buscar_o_agregar_equiv(equiv, &total_equiv,
                               buffer_cifrado,
                               db_mensajes[i].clave_id);
    }

    if (mensajes_total == 0) {
        printf("La palabra \"%s\" no aparece en ningun mensaje.\n", objetivo);
        return;
    }

    // Ordenar por numero de mensajes (descendente)
    qsort(equiv, total_equiv, sizeof(Equivalencia), cmp_equiv);

    printf("Encontrada en %d mensaje(s). Equivalencias cifradas:\n\n", mensajes_total);
    printf("%-30s %-10s %s\n", "Equivalente cifrado", "Clave", "Mensajes");
    printf("%-30s %-10s %s\n", "-----------------------------", "---------", "--------");

    for (int i = 0; i < total_equiv; i++) {
        printf("%-30s %-10s %d\n",
               equiv[i].equivalente,
               equiv[i].clave_id,
               equiv[i].mensajes_con_palabra);
    }
}