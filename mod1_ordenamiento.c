#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modulos.h"

// mod1_ordenamiento.c - Modulo 1: Carga y ordenamiento
// Estrategias: Counting Sort (prioridad), Merge Sort (fecha), Shell Sort (remitente), Counting Sort (longitud)

// Elimina el salto de linea al final del string
void limpiar_salto_linea(char *str) {
    int len = strlen(str);
    while (len > 0 && (str[len-1] == '\n' || str[len-1] == '\r')) {
        str[--len] = '\0';
    }
}

// Pasa el string de prioridad a enum
static Prioridad parsear_prioridad(const char *str) {
    if (strcmp(str, "baja")  == 0) return PRIORIDAD_BAJA;
    if (strcmp(str, "media") == 0) return PRIORIDAD_MEDIA;
    return PRIORIDAD_ALTA;
}

// Lee los txt y llena las estructuras, retorna el total de mensajes
int cargar_datos(Mensaje* db_mensajes, Clave* db_claves, int* total_claves,
                 char diccionario[][50], int* total_diccionario) {

    int total_mensajes = 0;
    *total_claves      = 0;
    *total_diccionario = 0;

    // 1. Mensajes cifrados
    FILE *file = fopen("Cryptonituv_DB/mensajes_cifrados.txt", "r"); 
    if (!file) file = fopen("Cryptonituv_DB/mensajes.txt", "r");  // nombre alternativo
    
    if (file) {
        char linea[2500];
        // Saltar primera linea si hay encabezado
        // fgets(linea, sizeof(linea), file); 

        while (fgets(linea, sizeof(linea), file) && total_mensajes < MAX_MENSAJES) {
            Mensaje *m = &db_mensajes[total_mensajes];

            // Leer ID
            char *tok = strtok(linea, "|");
            if (tok) m->id = atoi(tok);

            // Remitente
            tok = strtok(NULL, "|");
            if (tok) strncpy(m->remitente, tok, 49);

            // Prioridad
            tok = strtok(NULL, "|");
            if (tok) m->prioridad = parsear_prioridad(tok);

            // Fecha
            tok = strtok(NULL, "|");
            if (tok) strncpy(m->fecha, tok, 14);

            // ID clave
            tok = strtok(NULL, "|");
            if (tok) strncpy(m->clave_id, tok, 9);

            // Texto original
            tok = strtok(NULL, "|");
            if (tok) strncpy(m->texto_original, tok, 1023);

            // Texto cifrado
            tok = strtok(NULL, "|");
            if (tok) {
                strncpy(m->texto_cifrado, tok, 1023);
                limpiar_salto_linea(m->texto_cifrado);
                m->longitud = strlen(m->texto_cifrado);
            }
            total_mensajes++;
        }
        fclose(file);
    }

    // 2. Claves de sustitucion
    FILE *fclaves = fopen("Cryptonituv_DB/claves.txt", "r");
    if (fclaves) {
        char linea[1024];
        while (fgets(linea, sizeof(linea), fclaves) && *total_claves < MAX_CLAVES) {
            Clave *c = &db_claves[*total_claves];
            
            char *t = strtok(linea, "|"); // ID
            if (t){
                strncpy(c->id, t, 9);
                c->id[9] = '\0';
            }
            t = strtok(NULL, "|"); // Nombre
            t = strtok(NULL, "|"); // Tipo
            t = strtok(NULL, "|"); // Alfabeto original
            
            if (t) strncpy(c->alfabeto_original, t, 255);
            
            t = strtok(NULL, "|"); // Tabla sustitucion
            if (t) {
                strncpy(c->alfabeto_cifrado, t, 255);
                limpiar_salto_linea(c->alfabeto_cifrado);
            }
            (*total_claves)++;
        }
        fclose(fclaves);
    }

    // 3. Diccionario de palabras
    FILE *fdict = fopen("Cryptonituv_DB/palabras_frecuentes.txt", "r");
    if (fdict) {
        char linea[100];
        while (fgets(linea, sizeof(linea), fdict) && *total_diccionario < MAX_PALABRAS_DICT) {
            limpiar_salto_linea(linea);
            if (linea[0] != '\0') {
                strncpy(diccionario[*total_diccionario], linea, 49);
                (*total_diccionario)++;
            }
        }
        fclose(fdict);
    }

    return total_mensajes;
}

// Muestra N mensajes con sus campos principales
void m1_imprimir_mensajes(Mensaje* db_mensajes, int total_mensajes, int cantidad) {
    if (cantidad > total_mensajes) cantidad = total_mensajes;
    const char *prio_str[] = {"baja", "media", "alta"};
    
    printf("\n%-5s %-20s %-8s %-12s %-8s %s\n",
           "ID", "Remitente", "Prioridad", "Fecha", "Clave", "Longitud");
    printf("%-5s %-20s %-8s %-12s %-8s %s\n",
           "---", "-------------------", "--------", "-----------", "-------", "--------");
           
    for (int i = 0; i < cantidad; i++) {
        printf("%-5d %-20s %-8s %-12s %-8s %d\n",
               db_mensajes[i].id,
               db_mensajes[i].remitente,
               prio_str[db_mensajes[i].prioridad],
               db_mensajes[i].fecha,
               db_mensajes[i].clave_id,
               db_mensajes[i].longitud);
    }
}

// Counting Sort por prioridad DESC: alta -> media -> baja
void m1_ordenar_prioridad_desc(Mensaje* db_mensajes, int total_mensajes) {
    Mensaje *salida = (Mensaje *)malloc(total_mensajes * sizeof(Mensaje));
    if (!salida) { printf("Error: memoria insuficiente.\n"); return; }

    // 1: contar por prioridad
    int conteo[3] = {0, 0, 0};
    for (int i = 0; i < total_mensajes; i++)
        conteo[db_mensajes[i].prioridad]++;

    // 2: posiciones de inicio (alta primero)
    int pos[3];
    pos[2] = 0;
    pos[1] = conteo[2];
    pos[0] = conteo[2] + conteo[1];

    // 3: distribuir manteniendo estabilidad
    for (int i = 0; i < total_mensajes; i++) {
        int p = db_mensajes[i].prioridad;
        salida[pos[p]++] = db_mensajes[i];
    }

    for (int i = 0; i < total_mensajes; i++) db_mensajes[i] = salida[i];
    free(salida);
    printf("Ordenado por Prioridad DESC completado.\n");
}

// Auxiliar de Merge Sort
static void merge_fechas(Mensaje* db, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    Mensaje *L = malloc(n1 * sizeof(Mensaje));
    Mensaje *R = malloc(n2 * sizeof(Mensaje));
    if (!L || !R) { free(L); free(R); return; }

    for (int i = 0; i < n1; i++) L[i] = db[l + i];
    for (int j = 0; j < n2; j++) R[j] = db[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (strcmp(L[i].fecha, R[j].fecha) <= 0) db[k++] = L[i++];
        else                                      db[k++] = R[j++];
    }
    while (i < n1) db[k++] = L[i++];
    while (j < n2) db[k++] = R[j++];
    free(L); free(R);
}

// Merge Sort recursivo para fechas (YYYY-MM-DD)
void m1_merge_sort_fechas(Mensaje* db_mensajes, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        m1_merge_sort_fechas(db_mensajes, l, m);
        m1_merge_sort_fechas(db_mensajes, m + 1, r);
        merge_fechas(db_mensajes, l, m, r);
    }
}

// Shell Sort por remitente ASC (Knuth)
void m1_shell_sort_remitente(Mensaje* db_mensajes, int total_mensajes) {
    // Gap de Knuth
    int gap = 1;
    while (gap < total_mensajes / 3) gap = gap * 3 + 1;

    while (gap >= 1) {
        for (int i = gap; i < total_mensajes; i++) {
            Mensaje temp = db_mensajes[i];
            int j = i;
            // Desplazar a la derecha
            while (j >= gap && strcmp(db_mensajes[j - gap].remitente, temp.remitente) > 0) {
                db_mensajes[j] = db_mensajes[j - gap];
                j -= gap;
            }
            db_mensajes[j] = temp;
        }
        gap /= 3;
    }
    printf("Ordenado por Remitente ASC completado.\n");
}

// Counting Sort por longitud ASC
void m1_ordenar_longitud_asc(Mensaje* db_mensajes, int total_mensajes) {
    // Buscar longitud maxima
    int max_long = 0;
    for (int i = 0; i < total_mensajes; i++)
        if (db_mensajes[i].longitud > max_long) max_long = db_mensajes[i].longitud;

    int *conteo = calloc(max_long + 1, sizeof(int));
    Mensaje *salida = malloc(total_mensajes * sizeof(Mensaje));
    if (!conteo || !salida) { free(conteo); free(salida); return; }

    // Contar frecuencias
    for (int i = 0; i < total_mensajes; i++) conteo[db_mensajes[i].longitud]++;

    // Acumular posiciones
    for (int i = 1; i <= max_long; i++) conteo[i] += conteo[i - 1];

    // Recorrido inverso para guardar orden (estabilidad)
    for (int i = total_mensajes - 1; i >= 0; i--) {
        int lon = db_mensajes[i].longitud;
        salida[conteo[lon] - 1] = db_mensajes[i];
        conteo[lon]--;
    }

    for (int i = 0; i < total_mensajes; i++) db_mensajes[i] = salida[i];
    free(conteo); free(salida);
    printf("Ordenado por Longitud ASC completado.\n");
}