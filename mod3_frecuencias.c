#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "modulos.h"
#include "estructuras.h"

/* ============================================================
 * mod3_frecuencias.c  –  Módulo 3: Análisis de Frecuencias y Deducción
 *
 * Cumple con las complejidades exigidas por la guía:
 * - m3_analisis_frecuencia: O(L + k log k)
 * - m3_deducir_palabra: O(p) aplicando reducción por patrones estructurales.
 * ============================================================ */



/* Función de comparación para ordenar de mayor a menor frecuencia (qsort) */
static int cmp_frecuencias(const void* a, const void* b) {
    FrecuenciaLetra* fa = (FrecuenciaLetra*)a;
    FrecuenciaLetra* fb = (FrecuenciaLetra*)b;
    if (fb->conteo != fa->conteo) {
        return fb->conteo - fa->conteo; /* Descendente */
    }
    return fa->letra - fb->letra; /* Alfabético en caso de empate */
}

/* ------------------------------------------------------------
 * m3_analisis_frecuencia
 * Complejidad: O(L + k log k) donde k = 26.
 * ------------------------------------------------------------ */
void m3_analisis_frecuencia(const char* texto) {
    FrecuenciaLetra frecuencias[256];
    int tam = strlen(texto);
    int totalAnalizado =0;

    /* Asegurando que todo este en 0 */
    for(int i=0; i<256; i++){
        frecuencias[i].letra = (char)i;
        frecuencias[i].conteo =0;
    }
    /* Contar las frecuencias de las letras en el texto */
    for(int i=0; i<tam; i++){
        unsigned char letra = (unsigned char) texto[i];
        if(letra == ' ' || letra == '\n' || letra == '\r') continue;
        frecuencias[letra].letra = (char) letra;
        frecuencias[letra].conteo++;
        totalAnalizado++;
    }
    
    /* ordenar posiciones de la tabla */
    qsort(frecuencias,256,sizeof(FrecuenciaLetra), cmp_frecuencias);

    /* Impresion de resultados */
    printf("\n--- Módulo 3: ANÁLISIS DE FRECUENCIAS ---\n");
    printf("Total de caracteres analizados (letras/símbolos): %d\n", totalAnalizado);
    printf("Top 5 caracteres más frecuentes encontrados:\n");

    int impresos = 0;
    for (int i = 0; i < 256 && impresos < 5; i++) {
        if (frecuencias[i].conteo > 0) {
            // Mostramos de forma legible si es un espacio o un carácter imprimible
            printf("  [%d] Carácter: '%c' (ASCII %d) -> Conteo: %d\n", 
                impresos + 1, frecuencias[i].letra, (unsigned char)frecuencias[i].letra, frecuencias[i].conteo);
            impresos++;
        }
    }
    if (impresos == 0) {
        printf("  No se encontraron caracteres válidos para el análisis.\n");
    }
}

/* ------------------------------------------------------------
 * generar_patron_estructural
 * Convierte una palabra a un formato numérico basado en su estructura única.
 * Ejemplo: "hola" -> "0.1.2.3." | "ataque" -> "0.1.0.2.3.4." | "igsq" -> "0.1.2.3."
 * Permite verificar si dos palabras son isomórficas en O(longitud).
 * ------------------------------------------------------------ */
static void generar_patron_estructural(const char* palabra, char* patron_out) {
    int len = strlen(palabra);
    int mapaPos[256];

    for (int i = 0; i < 256; i++) {
        mapaPos[i] = -1;
    }

    patron_out[0] = '\0';
    int idSecuencial = 0;
    char bufferNum[15];

    for(int i=0; i<len; i++){
        unsigned char c = (unsigned char)palabra[i];
        /* Si es la primera vez que se ve el simbolo */
        if(mapaPos[c] ==-1){
            mapaPos[c] = idSecuencial;
            idSecuencial++;
        }
        sprintf(bufferNum, "%d.", mapaPos[c]);
        strcat(patron_out, bufferNum);
    }

}

/* ------------------------------------------------------------
 * m3_deducir_palabra
 * Complejidad: O(p) donde p es el número de palabras en el diccionario.
 * Evalúa las candidatas estructurales usando isomorfismo de strings.
 * ------------------------------------------------------------ */
void m3_deducir_palabra(const char* palabra_cifrada, char diccionario[][50], int total_diccionario) {
    int len_cifrada = strlen(palabra_cifrada);
    char patron_cifrado[1024];
    char patron_candidata[1024];
    int encontradas = 0;

    generar_patron_estructural(palabra_cifrada, patron_cifrado);

    printf("\n--- Módulo 3: DEDUCCIÓN DE PALABRAS CANDIDATAS ---\n");
    printf("Palabra cifrada objetivo: \"%s\" (Longitud: %d)\n", palabra_cifrada, len_cifrada);
    printf("Patrón Isomórfico calculado: %s\n", patron_cifrado);
    printf("Buscando coincidencias estructurales exactas en el diccionario...\n\n");

    /* Recorrer las p palabras del diccionario -> O(p) */
    for (int i = 0; i < total_diccionario; i++) {
        /* Filtro rápido 1: Deben tener exactamente la misma longitud */
        if ((int)strlen(diccionario[i]) != len_cifrada) {
            continue;
        }

        /* Filtro 2: Construir el patrón de la palabra del diccionario y comparar */
        generar_patron_estructural(diccionario[i], patron_candidata);

        // Si los patrones estructurales son idénticos, significa que es una traducción isomórfica válida
        if (strcmp(patron_cifrado, patron_candidata) == 0) {
            encontradas++;
            printf("  [Propuesta %d]: \"%s\"\n", encontradas, diccionario[i]);
            
            /* Limitamos las sugerencias impresas para mantener limpia la consola */
            if (encontradas >= 10) {
                printf("  ... (se encontraron más coincidencias, mostrando las primeras 10)\n");
                break;
            }
        }
    }

    if (encontradas == 0) {
        printf("No se encontraron palabras en el diccionario que coincidan con la estructura de \"%s\".\n", palabra_cifrada);
    }
}