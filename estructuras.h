#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

// estructuras.h - Sistema Cryptonituv

#define MAX_MENSAJES      1500
#define MAX_CLAVES          20
#define HASH_SIZE        10007
#define MAX_PALABRAS_DICT 50000

// Prioridades para el modulo 1 (Counting Sort)
typedef enum {
    PRIORIDAD_BAJA  = 0,
    PRIORIDAD_MEDIA = 1,
    PRIORIDAD_ALTA  = 2
} Prioridad;

// Registro principal: mensaje cifrado de la bd
typedef struct {
    int      id;
    char     remitente[50];
    Prioridad prioridad;
    char     fecha[15];
    char     clave_id[10];
    char     texto_original[1024];
    char     texto_cifrado[1024];
    int      longitud;          // longitud del texto cifrado
} Mensaje;

// Tabla de sustitucion letra por letra
typedef struct {
    char id[10];
    char alfabeto_original[256];
    char alfabeto_cifrado[256];
} Clave;

// Nodo de la tabla hash (modulo 4)
typedef struct WordNode {
    char  word[100];
    int   doc_count;       // mensajes distintos donde aparece la palabra
    int   last_doc_id;     // evita contar el mismo msj dos veces
    struct WordNode* next;
} WordNode;

// Estructura de Frecuencias
typedef struct { 
    char letra;
    float clave_id;
} Frecuencias;

// Estructura de Remitentes
typedef struct { 
    int remitente_id;
    char nombre[20];
} Remitentes;

// Auxiliar para ordenar letras por frecuencia
typedef struct {
    char letra;
    int conteo;
} FrecuenciaLetra;

#endif