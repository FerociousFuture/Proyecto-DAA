#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modulos.h"

// main.c - Sistema Cryptonituv
// Punto de entrada: carga datos y muestra el menu principal

// Variables globales para no desbordar la pila
Mensaje db_mensajes[MAX_MENSAJES];
Clave   db_claves[MAX_CLAVES];
char    diccionario[MAX_PALABRAS_DICT][50];

// Limpia el buffer de stdin hasta el salto de linea
static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Muestra todas las claves disponibles
static void mostrar_claves(int total_claves) {
    printf("\nClaves disponibles:\n");
    printf("  ID. %s\n" , db_claves[0].id);
    for (int i = 1; i < total_claves; i++)
        printf("  %d. %s\n", i  , db_claves[i].id);
}

// Retorna el indice de la clave elegida (0 por defecto si es invalida)
static int seleccionar_clave(int total_claves) {
    mostrar_claves(total_claves);
    printf("Selecciona el numero de clave: ");
    int sel = 0;
    if (scanf("%d", &sel) != 1 || sel < 2 || sel > total_claves) sel = 1;
    limpiar_buffer();
    return sel ;  
}

int main(void) {
    int total_mensajes    = 0;
    int total_claves      = 0;
    int total_diccionario = 0;
    int opcion            = 0;

    printf("==============================================\n");
    printf("        Sistema Cryptonituv  v1.0             \n");
    printf("==============================================\n");
    printf("Cargando base de datos...\n");

    total_mensajes = cargar_datos(db_mensajes, db_claves, &total_claves,
                                  diccionario, &total_diccionario);

    if (total_mensajes == 0) {
        printf("Error fatal: no se pudo cargar la base de datos.\n");
        printf("Verifica que los archivos mensajes_cifrados.txt, claves.txt y\n");
        printf("palabras_frecuentes.txt esten en el directorio de trabajo.\n");
        return 1;
    }

    printf("Carga exitosa: %d mensajes  |  %d claves  |  %d palabras en diccionario.\n\n",
           total_mensajes, total_claves, total_diccionario);

    while (opcion != 6) {
        printf("\n========== MENU PRINCIPAL ==========\n");
        printf("  1. Modulo 1 - Listar y ordenar mensajes\n");
        printf("  2. Modulo 2 - Cifrar / Descifrar texto\n");
        printf("  3. Modulo 3 - Analisis de frecuencias y deduccion\n");
        printf("  4. Modulo 4 - Top 10 palabras mas presentes\n");
        printf("  5. Modulo 5 - Buscar palabra y ver equivalencias\n");
        printf("  6. Salir\n");
        printf("Opcion: ");

        if (scanf("%d", &opcion) != 1) { limpiar_buffer(); continue; }
        limpiar_buffer();

        switch (opcion) {

        // ---- MODULO 1: Ordenamiento ----
        case 1: {
            int cantidad = 20;
            printf("\n-- Modulo 1: Ordenamiento --\n");
            printf("  1. Prioridad DESC  (Counting Sort  O(n+m))\n");
            printf("  2. Fecha ASC       (Merge Sort     O(n log n))\n");
            printf("  3. Remitente ASC   (Shell Sort     O(n log^2 n))\n");
            printf("  4. Longitud ASC    (Counting Sort  O(n+m))\n");
            printf("Opcion: ");

            int sub = 0;
            if (scanf("%d", &sub) != 1) { limpiar_buffer(); break; }
            limpiar_buffer();

            switch (sub) {
                case 1: m1_ordenar_prioridad_desc(db_mensajes, total_mensajes); break;
                case 2: m1_merge_sort_fechas(db_mensajes, 0, total_mensajes - 1);
                        printf("Ordenado por Fecha ASC completado.\n"); break;
                case 3: m1_shell_sort_remitente(db_mensajes, total_mensajes); break;
                case 4: m1_ordenar_longitud_asc(db_mensajes, total_mensajes); break;
                default: printf("Opcion no valida.\n"); break;
            }

            if (sub >= 1 && sub <= 4) {
                printf("Cuantos mensajes mostrar? (max %d): ", total_mensajes);
                if (scanf("%d", &cantidad) != 1 || cantidad < 1) cantidad = 20;
                limpiar_buffer();
                m1_imprimir_mensajes(db_mensajes, total_mensajes, cantidad);
            }
            break;
        }

        // ---- MODULO 2: Cifrado / Descifrado ----
        case 2: {
            char input[1024], output[1024], descifrado[1024];
            
            printf("\n-- Modulo 2: Cifrado y Descifrado --\n");
            printf("Ingresa el texto: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';
            int *pos = calloc(strlen(input), sizeof(int));

            int idx_clave = seleccionar_clave(total_claves);
            Clave clave_sel = db_claves[idx_clave];

            m2_procesar_texto(input, output, clave_sel, 0, pos);
            printf("\nTexto original : %s\n", input);
            printf("Texto cifrado  : %s\n", output);

            m2_procesar_texto(output, descifrado, clave_sel, 1, pos);
            printf("Descifrado     : %s\n", descifrado);

            if (strcmp(input, descifrado) == 0){
                printf("[OK] El descifrado coincide con el texto original.\n");

            }else{
                printf("[AVISO] El descifrado no coincide (posible caracter fuera de clave).\n");
            }
            free(pos);
            break;
        }

        // ---- MODULO 3: Frecuencias ----
        case 3: {
            char palabra[50];
            printf("\n-- Modulo 3: Analisis de Frecuencias y Deduccion --\n");
            printf("Ingresa una palabra cifrada: ");
            fgets(palabra, sizeof(palabra), stdin);
            palabra[strcspn(palabra, "\n")] = '\0';

            m3_analisis_frecuencia(palabra);
            m3_deducir_palabra(palabra, diccionario, total_diccionario);
            break;
        }

        // ---- MODULO 4: Top 10 ----
        case 4:
            m4_top_10_palabras(db_mensajes, total_mensajes);
            break;

        // ---- MODULO 5: Busqueda ----
        case 5: {
            char busqueda[50];
            printf("\n-- Modulo 5: Busqueda de Palabra --\n");
            printf("Ingresa la palabra a buscar: ");
            fgets(busqueda, sizeof(busqueda), stdin);
            busqueda[strcspn(busqueda, "\n")] = '\0';

            m5_buscar_palabra(busqueda, db_mensajes, total_mensajes,
                              db_claves, total_claves);
            break;
        }

        case 6:
            printf("Saliendo del sistema. Hasta luego!\n");
            break;

        default:
            printf("Opcion no valida. Intenta de nuevo.\n");
        }
    }

    return 0;
}