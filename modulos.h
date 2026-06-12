#ifndef MODULOS_H
#define MODULOS_H

// modulos.h - Declaraciones publicas de todos los modulos
// Sistema Cryptonituv

#include "estructuras.h"

// Utilidades
void limpiar_salto_linea(char *str);

// ---- Modulo 1: Ordenamiento ----

// Carga datos desde archivos. Retorna total de mensajes.
int cargar_datos(Mensaje* db_mensajes, Clave* db_claves, int* total_claves,
                 char diccionario[][50], int* total_diccionario);

// Counting Sort (prioridad DESC)
void m1_ordenar_prioridad_desc(Mensaje* db_mensajes, int total_mensajes);

// Merge Sort (fecha ASC)
void m1_merge_sort_fechas(Mensaje* db_mensajes, int l, int r);

// Shell Sort (remitente ASC)
void m1_shell_sort_remitente(Mensaje* db_mensajes, int total_mensajes);

// Counting Sort (longitud ASC)
void m1_ordenar_longitud_asc(Mensaje* db_mensajes, int total_mensajes);

// Imprime N mensajes
void m1_imprimir_mensajes(Mensaje* db_mensajes, int total_mensajes, int cantidad);

// ---- Modulo 2: Cifrado ----

// Cifra o descifra (0=cifrar, 1=descifrar)
void m2_procesar_texto(const char* input, char* output, Clave clave, int descifrar, int *pos);

// ---- Modulo 3: Frecuencias ----

// Top 5 de letras
void m3_analisis_frecuencia(const char* texto);

// Propone candidatas por patron estructural
void m3_deducir_palabra(const char* palabra_cifrada, char diccionario[][50], int total_diccionario);

// ---- Modulo 4: Top 10 ----

// Top 10 palabras en mas mensajes distintos
void m4_top_10_palabras(Mensaje* db_mensajes, int total_mensajes);

// ---- Modulo 5: Busqueda ----

// Busca palabra, cuenta e imprime equivalencias cifradas
void m5_buscar_palabra(const char* objetivo, Mensaje* db_mensajes,
                       int total_mensajes, Clave* db_claves, int total_claves);

#endif