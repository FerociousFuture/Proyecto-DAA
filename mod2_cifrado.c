#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "modulos.h"

/* ============================================================
 * mod2_cifrado.c  –  Módulo 2: Cifrado y Descifrado
 *
 * Requisito: Complejidad O(L), donde L es la longitud del texto.
 * Estrategia: 
 * - Cifrado: Mapeo directo indexando con el valor ASCII del carácter.
 * - Descifrado: Construcción de una tabla inversa O(k) y mapeo directo O(1).
 * ============================================================ */

void m2_procesar_texto(const char* input, char* output, Clave clave, int descifrar, int *pos) {
    int i;
    
    if (!descifrar) {
        /* --------------------------------------------------------
         * CASO 1: CIFRAR
         * -------------------------------------------------------- */
        char tabla_cifrado[26];
        
        // Inicializar mapeo base de la 'a' a la 'z'
        for (i = 0; i < 26; i++) {
            tabla_cifrado[i] = 'a' + i;
        }
        
        // Cargar alfabeto de sustitución
        for (i = 0; clave.alfabeto_original[i] != '\0' && clave.alfabeto_cifrado[i] != '\0'; i++) {
            char orig = tolower((unsigned char)clave.alfabeto_original[i]);
            if (orig >= 'a' && orig <= 'z') {
                tabla_cifrado[orig - 'a'] = clave.alfabeto_cifrado[i];
            }
        }
        
        // Cifrar el texto original
        for (i = 0; input[i] != '\0'; i++) {
            char c = input[i];
            if (c >= 'a' && c <= 'z') {
                output[i] = tabla_cifrado[c - 'a'];
                pos[i] = 1; /* Marcamos que esta posición SÍ se transformó */
            } else {
                output[i] = c;
                pos[i] = 0; /* Marcamos que se conservó intacto (espacios, ?, :, números) */
            }
        }
        output[i] = '\0';
        
    } else {
        /* --------------------------------------------------------
         * CASO 2: DESCIFRAR
         * -------------------------------------------------------- */
        char tabla_inversa[256];
        
        // Construimos una tabla inversa indexada por el ASCII del símbolo cifrado
        for (i = 0; clave.alfabeto_original[i] != '\0' && clave.alfabeto_cifrado[i] != '\0'; i++) {
            char orig = tolower((unsigned char)clave.alfabeto_original[i]);
            if (orig >= 'a' && orig <= 'z') {
                unsigned char simbolo = (unsigned char)clave.alfabeto_cifrado[i];
                tabla_inversa[simbolo] = orig;
            }
        }
        
        // Descifrar guiándonos ESTRICTAMENTE por lo que registramos en 'pos'
        for (i = 0; input[i] != '\0'; i++) {
            unsigned char c = (unsigned char)input[i];
            
            /* SÓLO revertimos si 'pos' dice que esta posición fue mutada al cifrar */
            if (pos[i] == 1) {
                output[i] = tabla_inversa[c];
            } else {
                output[i] = input[i]; /* Pasa el carácter tal cual (evitando falsos positivos) */
            }
        }
        output[i] = '\0';
    }
}