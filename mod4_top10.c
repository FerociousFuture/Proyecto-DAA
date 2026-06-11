#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "modulos.h"

/* ============================================================
 * mod4_top10.c  –  Módulo 4: Top 10 palabras más presentes
 *
 * Algoritmo:
 *   1. Recorrer todos los mensajes y tokenizar sus textos originales.
 *   2. Insertar cada palabra en una tabla hash (encadenamiento externo).
 *      Si la palabra ya existe en el mismo mensaje, NO se cuenta de nuevo
 *      (se guarda el ID del último mensaje que la aportó).
 *   3. Recolectar todos los nodos en un arreglo y ordenar con qsort.
 *   4. Mostrar el top-10.
 *
 * Complejidad: O(T + w log w)
 *   T = total de palabras procesadas en todos los mensajes
 *   w = palabras distintas encontradas
 * ============================================================ */

/* Tabla hash estática: se reinicializa entre llamadas consecutivas */
static WordNode* hash_table[HASH_SIZE];
static int tabla_inicializada = 0;

/* ----------------------------------------------------------
 * hash  –  función djb2
 * Distribuye cadenas uniformemente en [0, HASH_SIZE).
 * ---------------------------------------------------------- */
static unsigned int hash(const char *str) {
    unsigned int h = 5381;
    int c;
    while ((c = (unsigned char)*str++)) h = ((h << 5) + h) + c;
    return h % HASH_SIZE;
}

/* Libera todos los nodos de la tabla hash */
static void limpiar_tabla_hash(void) {
    for (int i = 0; i < HASH_SIZE; i++) {
        WordNode *nodo = hash_table[i];
        while (nodo) {
            WordNode *sig = nodo->next;  /* guardar siguiente ANTES de liberar */
            free(nodo);
            nodo = sig;
        }
        hash_table[i] = NULL;
    }
}

/* ----------------------------------------------------------
 * normalizar_palabra  –  O(L)
 * Convierte a minúsculas para comparación insensible a caso.
 * ---------------------------------------------------------- */
static void normalizar_palabra(char *palabra) {
    for (int i = 0; palabra[i]; i++)
        palabra[i] = (char)tolower((unsigned char)palabra[i]);
}

/* ----------------------------------------------------------
 * insertar_palabra_hash
 * Busca la palabra en la tabla hash.
 *   - Si existe y ya fue contada para este doc_id → no hace nada.
 *   - Si existe con otro doc_id → incrementa conteo y actualiza ID.
 *   - Si no existe → crea un nuevo nodo con doc_count = 1.
 * ---------------------------------------------------------- */
static void insertar_palabra_hash(const char* palabra, int doc_id) {
    unsigned int idx = hash(palabra);
    WordNode *actual = hash_table[idx];

    while (actual != NULL) {
        if (strcmp(actual->word, palabra) == 0) {
            /* Palabra ya vista; solo contar si es un mensaje nuevo */
            if (actual->last_doc_id != doc_id) {
                actual->doc_count++;
                actual->last_doc_id = doc_id;
            }
            return;
        }
        actual = actual->next;
    }

    /* Palabra nueva: crear nodo y encabezar la lista del bucket */
    WordNode *nuevo = (WordNode*)malloc(sizeof(WordNode));
    if (!nuevo) return;
    strncpy(nuevo->word, palabra, 99);
    nuevo->word[99]     = '\0';
    nuevo->doc_count    = 1;
    nuevo->last_doc_id  = doc_id;
    nuevo->next         = hash_table[idx];
    hash_table[idx]     = nuevo;
}

/* Comparador descendente por doc_count para qsort */
static int cmp_word_nodes(const void* a, const void* b) {
    return (*(WordNode**)b)->doc_count - (*(WordNode**)a)->doc_count;
}

/* ----------------------------------------------------------
 * m4_top_10_palabras  –  O(T + w log w)
 * ---------------------------------------------------------- */
void m4_top_10_palabras(Mensaje* db_mensajes, int total_mensajes) {

    /* Limpiar tabla de ejecuciones anteriores */
    if (tabla_inicializada) limpiar_tabla_hash();
    for (int i = 0; i < HASH_SIZE; i++) hash_table[i] = NULL;
    tabla_inicializada = 1;

    char buffer[1024];
    char palabra_norm[100];

    /* Fase 1: insertar todas las palabras en la tabla hash */
    for (int i = 0; i < total_mensajes; i++) {
        strncpy(buffer, db_mensajes[i].texto_original, 1023);
        buffer[1023] = '\0';

        char *token = strtok(buffer, " ,.-!?;:\"'()\n\r\t");
        while (token != NULL) {
            if (strlen(token) > 1) {  /* ignorar palabras de un solo carácter */
                strncpy(palabra_norm, token, 99);
                palabra_norm[99] = '\0';
                normalizar_palabra(palabra_norm);
                insertar_palabra_hash(palabra_norm, db_mensajes[i].id);
            }
            token = strtok(NULL, " ,.-!?;:\"'()\n\r\t");
        }
    }

    /* Fase 2: recolectar todos los nodos en un arreglo dinámico */
    int capacidad = 10000;
    int count     = 0;
    WordNode **array_palabras = malloc(capacidad * sizeof(WordNode*));
    if (!array_palabras) { printf("Error de memoria.\n"); return; }

    for (int i = 0; i < HASH_SIZE; i++) {
        WordNode *actual = hash_table[i];
        while (actual != NULL) {
            if (count >= capacidad) {
                capacidad *= 2;
                WordNode **tmp = realloc(array_palabras, capacidad * sizeof(WordNode*));
                if (!tmp) { free(array_palabras); return; }
                array_palabras = tmp;
            }
            array_palabras[count++] = actual;
            actual = actual->next;
        }
    }

    /* Fase 3: ordenar por doc_count descendente */
    qsort(array_palabras, count, sizeof(WordNode*), cmp_word_nodes);

    /* Fase 4: mostrar top 10 */
    printf("\n--- Módulo 4: TOP 10 PALABRAS MÁS PRESENTES ---\n");
    printf("%-5s %-30s %s\n", "Pos", "Palabra", "Mensajes distintos");
    printf("%-5s %-30s %s\n", "---", "-----------------------------", "------------------");
    for (int i = 0; i < 10 && i < count; i++) {
        printf("%-5d %-30s %d\n",
               i + 1,
               array_palabras[i]->word,
               array_palabras[i]->doc_count);
    }

    free(array_palabras);
}