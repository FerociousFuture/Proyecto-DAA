#include <stdio.h>
#include "modulos.h"

int main() {
    Mensaje db_mensajes[MAX_MENSAJES];
    Clave db_claves[MAX_CLAVES];
    int total_mensajes = 0;
    int total_claves = 0;

    printf("Iniciando Sistema Algorítmico de Mensajes Cifrados...\n");

    // Cargar datos
    total_mensajes = cargar_datos(db_mensajes, db_claves, &total_claves);
    if (total_mensajes == 0) {
        printf("Error: No se pudo cargar mensajes_cifrados.txt. Verifica los archivos.\n");
        return 1;
    }
    printf("Base de datos cargada: %d mensajes y %d claves.\n\n", total_mensajes, total_claves);

    // Módulo 1: Ordenamiento
    m1_ordenar_prioridad_desc(db_mensajes, total_mensajes);
    m1_merge_sort_fechas(db_mensajes, 0, total_mensajes - 1);
    
    // Módulo 3: Frecuencias (Probado con el primer mensaje cifrado)
    if (total_mensajes > 0) {
        m3_analisis_frecuencia(db_mensajes[0].texto_cifrado);
    }
    
    // Módulo 4: Top 10
    m4_top_10_palabras(db_mensajes, total_mensajes);
    
    // Módulo 5: Búsqueda (Ejemplo "ataque")
    m5_buscar_palabra("ataque", db_mensajes, total_mensajes, db_claves, total_claves);

    return 0;
}