#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "modulos.h"

// mod4_top10.c - Modulo 4: Top 10 palabras mas presentes
// Tokeniza textos, cuenta apariciones usando hash table y muestra el top 10.
// Complejidad: O(T + w log w)

// Tabla hash (se limpia entre ejecuciones)
static WordNode* hash_table[HASH_SIZE];
static int tabla_inicializada = 0;

// Funcion hash djb2
static unsigned int hash(const char *str) {
    unsigned int h = 5381;
    int c;
    while ((c = (unsigned char)*str++)) h = ((h << 5) + h) + c;
    return h % HASH_SIZE;
}

// Libera todos los nodos de la tabla hash
static void limpiar_tabla_hash(void) {
    for (int i = 0; i < HASH_SIZE; i++) {
        WordNode *nodo = hash_table[i];
        while (nodo) {
            WordNode *sig = nodo->next;  // Guardar siguiente ANTES de liberar
            free(nodo);
            nodo = sig;
        }
        hash_table[i] = NULL;
    }
}

// Pasa a minusculas
static void normalizar_palabra(char *palabra) {
    for (int i = 0; palabra[i]; i++)
        palabra[i] = (char)tolower((unsigned char)palabra[i]);
}

// Busca e inserta palabra en tabla hash
static void insertar_palabra_hash(const char* palabra, int doc_id) {
    unsigned int idx = hash(palabra);
    WordNode *actual = hash_table[idx];

    while (actual != NULL) {
        if (strcmp(actual->word, palabra) == 0) {
            // Palabra ya vista; solo contar si es un mensaje nuevo
            if (actual->last_doc_id != doc_id) {
                actual->doc_count++;
                actual->last_doc_id = doc_id;
            }
            return;
        }
        actual = actual->next;
    }

    // Palabra nueva: crear nodo al inicio de la lista
    WordNode *nuevo = (WordNode*)malloc(sizeof(WordNode));
    if (!nuevo) return;
    strncpy(nuevo->word, palabra, 99);
    nuevo->word[99]     = '\0';
    nuevo->doc_count    = 1;
    nuevo->last_doc_id  = doc_id;
    nuevo->next         = hash_table[idx];
    hash_table[idx]     = nuevo;
}

// Comparador descendente para qsort
static int cmp_word_nodes(const void* a, const void* b) {
    return (*(WordNode**)b)->doc_count - (*(WordNode**)a)->doc_count;
}

// Genera y muestra el top 10 de palabras
void m4_top_10_palabras(Mensaje* db_mensajes, int total_mensajes) {

    // Limpiar ejecuciones anteriores
    if (tabla_inicializada) limpiar_tabla_hash();
    for (int i = 0; i < HASH_SIZE; i++) hash_table[i] = NULL;
    tabla_inicializada = 1;

    char buffer[1024];
    char palabra_norm[100];

    // 1. Insertar palabras en la tabla hash
    for (int i = 0; i < total_mensajes; i++) {
        strncpy(buffer, db_mensajes[i].texto_original, 1023);
        buffer[1023] = '\0';

        char *token = strtok(buffer, " ,.-!?;:\"'()\n\r\t");
        while (token != NULL) {
            if (strlen(token) > 1) {  // Ignorar palabras de una sola letra
                strncpy(palabra_norm, token, 99);
                palabra_norm[99] = '\0';
                normalizar_palabra(palabra_norm);
                insertar_palabra_hash(palabra_norm, db_mensajes[i].id);
            }
            token = strtok(NULL, " ,.-!?;:\"'()\n\r\t");
        }
    }

    // 2. Recolectar nodos en un array dinamico
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

    // 3. Ordenar array descendente
    qsort(array_palabras, count, sizeof(WordNode*), cmp_word_nodes);

    // 4. Mostrar top 10
    printf("\n--- Modulo 4: TOP 10 PALABRAS MAS PRESENTES ---\n");
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