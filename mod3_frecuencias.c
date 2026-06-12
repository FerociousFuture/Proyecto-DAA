#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "modulos.h"
#include "estructuras.h"

// mod3_frecuencias.c - Modulo 3: Analisis de Frecuencias y Deduccion
// m3_analisis_frecuencia: O(L + k log k)
// m3_deducir_palabra: O(p) aplicando reduccion por patrones estructurales

// Funcion para ordenar de mayor a menor frecuencia
static int cmp_frecuencias(const void* a, const void* b) {
    FrecuenciaLetra* fa = (FrecuenciaLetra*)a;
    FrecuenciaLetra* fb = (FrecuenciaLetra*)b;
    if (fb->conteo != fa->conteo) {
        return fb->conteo - fa->conteo; // Descendente
    }
    return fa->letra - fb->letra; // Alfabetico en caso de empate
}

// Analisis de frecuencia 
void m3_analisis_frecuencia(const char* texto) {
    FrecuenciaLetra frecuencias[256];
    int tam = strlen(texto);
    int totalAnalizado = 0;

    // Inicializar todo en 0
    for(int i = 0; i < 256; i++){
        frecuencias[i].letra = (char)i;
        frecuencias[i].conteo = 0;
    }
    
    // Contar las frecuencias de los caracteres
    for(int i = 0; i < tam; i++){
        unsigned char letra = (unsigned char) texto[i];
        if(letra == ' ' || letra == '\n' || letra == '\r') continue;
        frecuencias[letra].letra = (char) letra;
        frecuencias[letra].conteo++;
        totalAnalizado++;
    }
    
    // Ordenar tabla
    qsort(frecuencias, 256, sizeof(FrecuenciaLetra), cmp_frecuencias);

    printf("\n--- Modulo 3: ANALISIS DE FRECUENCIAS ---\n");
    printf("Total de caracteres analizados: %d\n", totalAnalizado);
    printf("Top 5 caracteres mas frecuentes:\n");

    int impresos = 0;
    for (int i = 0; i < 256 && impresos < 5; i++) {
        if (frecuencias[i].conteo > 0) {
            printf("  [%d] Caracter: '%c' (ASCII %d) -> Conteo: %d\n", 
                impresos + 1, frecuencias[i].letra, (unsigned char)frecuencias[i].letra, frecuencias[i].conteo);
            impresos++;
        }
    }
    if (impresos == 0) {
        printf("  No se encontraron caracteres validos para el analisis.\n");
    }
}

// Convierte palabra a formato numerico por estructura (ej: "hola" -> "0.1.2.3.")
static void generar_patron_estructural(const char* palabra, char* patron_out) {
    int len = strlen(palabra);
    int mapaPos[256];

    for (int i = 0; i < 256; i++) {
        mapaPos[i] = -1;
    }

    patron_out[0] = '\0';
    int idSecuencial = 0;
    char bufferNum[15];

    for(int i = 0; i < len; i++){
        unsigned char c = (unsigned char)palabra[i];
        // Si es la primera vez que se ve el caracter
        if(mapaPos[c] == -1){
            mapaPos[c] = idSecuencial;
            idSecuencial++;
        }
        sprintf(bufferNum, "%d.", mapaPos[c]);
        strcat(patron_out, bufferNum);
    }
}

// Evalua candidatas estructurales usando isomorfismo de strings
void m3_deducir_palabra(const char* palabra_cifrada, char diccionario[][50], int total_diccionario) {
    int len_cifrada = strlen(palabra_cifrada);
    char patron_cifrado[1024];
    char patron_candidata[1024];
    int encontradas = 0;

    generar_patron_estructural(palabra_cifrada, patron_cifrado);

    printf("\n--- Modulo 3: DEDUCCION DE PALABRAS CANDIDATAS ---\n");
    printf("Palabra cifrada objetivo: \"%s\" (Longitud: %d)\n", palabra_cifrada, len_cifrada);
    printf("Patron calculado: %s\n", patron_cifrado);
    printf("Buscando coincidencias estructurales en el diccionario...\n\n");

    // Recorrer el diccionario
    for (int i = 0; i < total_diccionario; i++) {
        // Filtro 1: Misma longitud
        if ((int)strlen(diccionario[i]) != len_cifrada) {
            continue;
        }

        // Filtro 2: Mismo patron
        generar_patron_estructural(diccionario[i], patron_candidata);

        // Si son identicos, es una traduccion isomorfica
        if (strcmp(patron_cifrado, patron_candidata) == 0) {
            encontradas++;
            printf("  [Propuesta %d]: \"%s\"\n", encontradas, diccionario[i]);
            
            // Limitar las sugerencias a 10
            if (encontradas >= 10) {
                printf("  ... (se encontraron mas coincidencias, mostrando las primeras 10)\n");
                break;
            }
        }
    }

    if (encontradas == 0) {
        printf("No se encontraron palabras en el diccionario que coincidan con la estructura de \"%s\".\n", palabra_cifrada);
    }
}