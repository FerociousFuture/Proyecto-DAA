#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "modulos.h"

/* ============================================================
 * mod5_busqueda.c  –  Módulo 5: Búsqueda y equivalencias cifradas
 *
 * Dado una palabra objetivo, el módulo:
 * 1. Recorre todos los mensajes buscando la palabra en texto original.  O(T)
 * 2. Para cada mensaje donde aparece, cifra la palabra con la clave
 * correspondiente.                                                   O(q)
 * 3. Agrupa y cuenta apariciones por equivalente cifrado.
 * 4. Muestra un resumen con: equivalente cifrado, clave usada y
 * cuántos mensajes la contienen.
 *
 * Complejidad total: O(T + q)
 * T = total de caracteres en todos los textos originales
 * q = longitud de la palabra buscada
 * ============================================================ */

#define MAX_EQUIV 100  /* máximo de equivalentes cifrados distintos a rastrear */

/* Registro para acumular conteos por equivalente cifrado */
typedef struct {
    char equivalente[256];
    char clave_id[10];
    int  mensajes_con_palabra;   /* cantidad de mensajes distintos */
} Equivalencia;

/* ----------------------------------------------------------
 * buscar_clave_por_id  –  O(número de claves)
 * Retorna puntero a la clave o NULL si no existe.
 * ---------------------------------------------------------- */
static Clave* buscar_clave_por_id(const char* clave_id, Clave* db_claves, int total_claves) {
    for (int i = 0; i < total_claves; i++) {
        if (strcmp(db_claves[i].id, clave_id) == 0) return &db_claves[i];
    }
    return NULL;
}

/* ----------------------------------------------------------
 * buscar_o_agregar_equiv
 * Busca el equivalente cifrado en el arreglo de equivalencias.
 * Si existe, incrementa su contador; si no, lo agrega.
 * Retorna el índice del slot usado.
 * ---------------------------------------------------------- */
static int buscar_o_agregar_equiv(Equivalencia* equiv, int* total_equiv,
                                   const char* cifrado, const char* clave_id) {
    for (int i = 0; i < *total_equiv; i++) {
        if (strcmp(equiv[i].equivalente, cifrado) == 0) {
            equiv[i].mensajes_con_palabra++;
            return i;
        }
    }
    /* Nuevo equivalente */
    if (*total_equiv < MAX_EQUIV) {
        int idx = *total_equiv;
        strncpy(equiv[idx].equivalente, cifrado,  254); equiv[idx].equivalente[254] = '\0';
        strncpy(equiv[idx].clave_id,    clave_id,   9); equiv[idx].clave_id[9]     = '\0';
        equiv[idx].mensajes_con_palabra = 1;
        (*total_