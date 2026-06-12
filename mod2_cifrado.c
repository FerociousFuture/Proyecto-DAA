#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "modulos.h"

// mod2_cifrado.c - Modulo 2: Cifrado y Descifrado
// Complejidad O(L). Cifrado por mapeo directo ASCII. Descifrado con tabla inversa.

void m2_procesar_texto(const char* input, char* output, Clave clave, int descifrar, int *pos) {
    int i;
    
    if (!descifrar) {
        // ---- CIFRAR ----
        char tabla_cifrado[26];
        
        // Mapeo base de la 'a' a la 'z'
        for (i = 0; i < 26; i++) {
            tabla_cifrado[i] = 'a' + i;
        }
        
        // Cargar alfabeto de sustitucion
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
                pos[i] = 1; // Marca si la posicion se transformo
            } else {
                output[i] = c;
                pos[i] = 0; // Se conserva intacto (espacios, numeros, etc)
            }
        }
        output[i] = '\0';
        
    } else {
        // ---- DESCIFRAR ----
        char tabla_inversa[256];
        
        // Tabla inversa indexada por el ASCII del simbolo cifrado
        for (i = 0; clave.alfabeto_original[i] != '\0' && clave.alfabeto_cifrado[i] != '\0'; i++) {
            char orig = tolower((unsigned char)clave.alfabeto_original[i]);
            if (orig >= 'a' && orig <= 'z') {
                unsigned char simbolo = (unsigned char)clave.alfabeto_cifrado[i];
                tabla_inversa[simbolo] = orig;
            }
        }
        
        // Descifrar guiandonos por el registro en 'pos'
        for (i = 0; input[i] != '\0'; i++) {
            unsigned char c = (unsigned char)input[i];
            
            // Solo revertimos si 'pos' indica que muto al cifrar
            if (pos[i] == 1) {
                output[i] = tabla_inversa[c];
            } else {
                output[i] = input[i]; // Pasa el caracter tal cual
            }
        }
        output[i] = '\0';
    }
}