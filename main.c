#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modulos.h"

// Variables globales para aligerar la pila
Mensaje db_mensajes[MAX_MENSAJES];
Clave db_claves[MAX_CLAVES];
char diccionario[MAX_PALABRAS_DICT][50];

void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int total_mensajes = 0;
    int total_claves = 0;
    int total_diccionario = 0;
    int opcion = 0;

    printf("Iniciando Sistema Cryptonituv...\n");
    total_mensajes = cargar_datos(db_mensajes, db_claves, &total_claves, diccionario, &total_diccionario);
    
    if (total_mensajes == 0) {
        printf("Error fatal: No se pudo cargar la base de datos.\n");
        return 1;
    }
    
    printf("Carga exitosa: %d mensajes, %d claves, %d palabras en diccionario.\n", 
           total_mensajes, total_claves, total_diccionario);

    while (opcion != 6) {
        printf("\n========== MENU PRINCIPAL ==========\n");
        printf("1. Listar mensajes (Módulo 1)\n");
        printf("2. Cifrar/Descifrar manual (Módulo 2)\n");
        printf("3. Deducir palabra cifrada (Módulo 3)\n");
        printf("4. Top 10 palabras (Módulo 4)\n");
        printf("5. Buscar palabra en base de datos (Módulo 5)\n");
        printf("6. Salir\n");
        printf("Selecciona una opción: ");
        
        if (scanf("%d", &opcion) != 1) {
            limpiar_buffer();
            continue;
        }
        limpiar_buffer();

        switch (opcion) {
            case 1: {
                int sub_op;
                printf("1. Ordenar por Prioridad (DESC) O(n+m)\n");
                printf("2. Ordenar por Fecha (ASC) O(n log n)\n");
                printf("Opcion: ");
                scanf("%d", &sub_op);
                limpiar_buffer();
                if (sub_op == 1) {
                    m1_ordenar_prioridad_desc(db_mensajes, total_mensajes);
                    for(int i=0; i<10; i++) printf("ID: %d | Prioridad: %d\n", db_mensajes[i].id, db_mensajes[i].prioridad);
                } else if (sub_op == 2) {
                    m1_merge_sort_fechas(db_mensajes, 0, total_mensajes - 1);
                    for(int i=0; i<10; i++) printf("ID: %d | Fecha: %s\n", db_mensajes[i].id, db_mensajes[i].fecha);
                }
                break;
            }
            case 2: {
                char input[1024], output[1024];
                printf("Ingresa el texto: ");
                fgets(input, sizeof(input), stdin);
                limpiar_salto_linea(input); // Asume que agregaste esta utilidad en algun lado o usa strcspn
                input[strcspn(input, "\n")] = 0; // Limpia el salto
                
                // Forzamos uso de la primera clave por defecto para la prueba (K01)
                printf("Aplicando clave %s...\n", db_claves[0].id);
                m2_procesar_texto(input, output, db_claves[0], 0); // 0 = Cifrar
                printf("Cifrado: %s\n", output);
                
                char descifrado[1024];
                m2_procesar_texto(output, descifrado, db_claves[0], 1); // 1 = Descifrar
                printf("Descifrado inverso: %s\n", descifrado);
                break;
            }
            case 3: {
                char palabra[50];
                printf("Ingresa una palabra cifrada para intentar deducirla: ");
                fgets(palabra, sizeof(palabra), stdin);
                palabra[strcspn(palabra, "\n")] = 0;
                
                m3_analisis_frecuencia(palabra);
                m3_deducir_palabra(palabra, diccionario, total_diccionario);
                break;
            }
            case 4:
                m4_top_10_palabras(db_mensajes, total_mensajes);
                break;
            case 5: {
                char busqueda[50];
                printf("Ingresa la palabra a buscar: ");
                fgets(busqueda, sizeof(busqueda), stdin);
                busqueda[strcspn(busqueda, "\n")] = 0;
                
                m5_buscar_palabra(busqueda, db_mensajes, total_mensajes, db_claves, total_claves);
                break;
            }
            case 6:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    }
    return 0;
}