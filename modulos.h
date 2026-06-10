#ifndef MODULOS_H
#define MODULOS_H

/* ============================================================
 * modulos.h  –  Declaraciones públicas de todos los módulos
 * Sistema Cryptonituv
 * ============================================================ */

#include "estructuras.h"

/* ---------- Utilidades compartidas ---------- */
void limpiar_salto_linea(char *str);

/* ---------- Módulo 1: Ordenamiento ---------- */

/*
 * Carga mensajes, claves y diccionario desde los archivos de datos.
 * Retorna el número de mensajes leídos.
 */
int cargar_datos(Mensaje* db_mensajes, Clave* db_claves, int* total_claves,
                 char diccionario[][50], int* total_diccionario);

/* Counting Sort por prioridad descendente  –  O(n + m)  */
void m1_ordenar_prioridad_desc(Mensaje* db_mensajes, int total_mensajes);

/* Merge Sort por fecha ascendente           –  O(n log n) */
void m1_merge_sort_fechas(Mensaje* db_mensajes, int l, int r);

/* Shell Sort por remitente ascendente       –  O(n log² n) promedio */
void m1_shell_sort_remitente(Mensaje* db_mensajes, int total_mensajes);

/* Counting Sort por longitud de texto cifrado ascendente  –  O(n + m) */
void m1_ordenar_longitud_asc(Mensaje* db_mensajes, int total_mensajes);

/* Imprime los primeros `cantidad` mensajes de la lista actual */
void m1_imprimir_mensajes(Mensaje* db_mensajes, int total_mensajes, int cantidad);

/* ---------- Módulo 2: Cifrado y Descifrado ---------- */

/*
 * Cifra o descifra `input` usando la tabla de sustitución `clave`.
 * descifrar = 0 → cifrar  |  descifrar = 1 → descifrar
 * Complejidad: O(L)
 */
void m2_procesar_texto(const char* input, char* output, Clave clave, int descifrar);

/* ---------- Módulo 3: Análisis de Frecuencias ---------- */

/* Muestra el top-5 de letras más frecuentes del texto  –  O(L + k log k) */
void m3_analisis_frecuencia(const char* texto);

/*
 * Propone candidatas del diccionario cuyo patrón estructural coincide
 * con la palabra cifrada  –  O(p) donde p = tamaño del diccionario
 */
void m3_deducir_palabra(const char* palabra_cifrada,
                        char diccionario[][50], int total_diccionario);

/* ---------- Módulo 4: Top 10 Palabras ---------- */

/*
 * Identifica las 10 palabras presentes en más mensajes distintos.
 * Complejidad: O(T + w log w)
 */
void m4_top_10_palabras(Mensaje* db_mensajes, int total_mensajes);

/* ---------- Módulo 5: Búsqueda ---------- */

/*
 * Busca una palabra en todos los mensajes y muestra cuántos la contienen,
 * junto con sus equivalentes cifrados por clave.
 * Complejidad: O(T + q)
 */
void m5_buscar_palabra(const char* objetivo, Mensaje* db_mensajes,
                       int total_mensajes, Clave* db_claves, int total_claves);

#endif /* MODULOS_H */