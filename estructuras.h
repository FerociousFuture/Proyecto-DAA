#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

/* ============================================================
 * estructuras.h  –  Definiciones de tipos y constantes globales
 * Sistema Cryptonituv
 * ============================================================ */

#define MAX_MENSAJES      1500
#define MAX_CLAVES          20
#define HASH_SIZE        10007
#define MAX_PALABRAS_DICT 50000

/* Niveles de prioridad usados por el Módulo 1 (Counting Sort) */
typedef enum {
    PRIORIDAD_BAJA  = 0,
    PRIORIDAD_MEDIA = 1,
    PRIORIDAD_ALTA  = 2
} Prioridad;

/* Registro principal: un mensaje cifrado de la base de datos */
typedef struct {
    int      id;
    char     remitente[50];
    Prioridad prioridad;
    char     fecha[15];
    char     clave_id[10];
    char     texto_original[1024];
    char     texto_cifrado[1024];
    int      longitud;          /* longitud de texto_cifrado */
} Mensaje;

/* Tabla de sustitución letra-por-letra */
typedef struct {
    char id[10];
    char alfabeto_original[256];
    char alfabeto_cifrado[256];
} Clave;

/* Nodo de la tabla hash usada por el Módulo 4 */
typedef struct WordNode {
    char  word[100];
    int   doc_count;       /* mensajes distintos donde aparece */
    int   last_doc_id;     /* para evitar contar el mismo mensaje dos veces */
    struct WordNode* next;
} WordNode;

/* Estructura para Frecuencias */
typedef struct { 
    char letra;
    float clave_id;
}Frecuencias;

/* Estructura para Remitentes */
typedef struct { //Estructura para remitentes
    int remitente_id;
    char nombre[20];
}Remitentes;

/* Estructura auxiliar para ordenar las letras por frecuencia */
typedef struct {
    char letra;
    int conteo;
} FrecuenciaLetra;

#endif /* ESTRUCTURAS_H */