#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modulos.h"

/* ============================================================
 * main.c  –  Sistema Cryptonituv
 * Punto de entrada: carga datos y presenta el menú principal.
 * ============================================================ */

/* Variables globales para evitar desbordamiento de pila */
Mensaje db_mensajes[MAX_MENSAJES];
Clave   db_claves[MAX_CLAVES];
char    diccionario[MAX_PALABRAS_DICT][50];

/* Consume el buffer de stdin hasta el siguiente salto de línea */
static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* ----------------------------------------------------------
 * mostrar_claves
 * Lista todas las claves disponibles para que el usuario elija.
 * ---------------------------------------------------------- */
static void mostrar_claves(int total_claves) {
    printf("\nClaves disponibles:\n");
    for (int i = 0; i < total_claves; i++)
        printf("  %d. %s\n", i + 1, db_claves[i].id);
}

/* ----------------------------------------------------------
 * seleccionar_clave
 * Retorna el índice de la clave elegida por el usuario,
 * o 0 por defecto si la entrada es inválida.
 * ---------------------------------------------------------- */
static int seleccionar_clave(int total_claves) {
    mostrar_claves(total_claves);
    printf("Selecciona el número de clave: ");
    int sel = 0;
    if (scanf("%d", &sel) != 1 || sel < 1 || sel > total_claves) sel = 1;
    limpiar_buffer();
    return sel - 1;  /* índice base-0 */
}

/* ============================================================
 * main
 * ============================================================ */
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
        printf("palabras_frecuentes.txt estén en el directorio de trabajo.\n");
        return 1;
    }

    printf("Carga exitosa: %d mensajes  |  %d claves  |  %d palabras en diccionario.\n\n",
           total_mensajes, total_claves, total_diccionario);

    while (opcion != 6) {
        printf("\n========== MENÚ PRINCIPAL ==========\n");
        printf("  1. Módulo 1 – Listar y ordenar mensajes\n");
        printf("  2. Módulo 2 – Cifrar / Descifrar texto\n");
        printf("  3. Módulo 3 – Análisis de frecuencias y deducción\n");
        printf("  4. Módulo 4 – Top 10 palabras más presentes\n");
        printf("  5. Módulo 5 – Buscar palabra y ver equivalencias\n");
        printf("  6. Salir\n");
        printf("Opción: ");

        if (scanf("%d", &opcion) != 1) { limpiar_buffer(); continue; }
        limpiar_buffer();

        switch (opcion) {

        /* ---- MÓDULO 1: Ordenamiento ---- */
        case 1: {
            int cantidad = 20;
            printf("\n-- Módulo 1: Ordenamiento --\n");
            printf("  1. Prioridad DESC  (Counting Sort  O(n+m))\n");
            printf("  2. Fecha ASC       (Merge Sort     O(n log n))\n");
            printf("  3. Remitente ASC   (Shell Sort     O(n log² n))\n");
            printf("  4. Longitud ASC    (Counting Sort  O(n+m))\n");
            printf("Opción: ");

            int sub = 0;
            if (scanf("%d", &sub) != 1) { limpiar_buffer(); break; }
            limpiar_buffer();

            switch (sub) {
                case 1: m1_ordenar_prioridad_desc(db_mensajes, total_mensajes); break;
                case 2: m1_merge_sort_fechas(db_mensajes, 0, total_mensajes - 1);
                        printf("Ordenado por Fecha ASC (Merge Sort O(n log n)) completado.\n"); break;
                case 3: m1_shell_sort_remitente(db_mensajes, total_mensajes); break;
                case 4: m1_ordenar_longitud_asc(db_mensajes, total_mensajes); break;
                default: printf("Opción no válida.\n"); break;
            }

            if (sub >= 1 && sub <= 4) {
                printf("¿Cuántos mensajes mostrar? (máx %d): ", total_mensajes);
                if (scanf("%d", &cantidad) != 1 || cantidad < 1) cantidad = 20;
                limpiar_buffer();
                m1_imprimir_mensajes(db_mensajes, total_mensajes, cantidad);
            }
            break;
        }

        /* ---- MÓDULO 2: Cifrado / Descifrado ---- */
        case 2: {
            char input[1024], output[1024], descifrado[1024];
            printf("\n-- Módulo 2: Cifrado y Descifrado --\n");
            printf("Ingresa el texto: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';

            int idx_clave = seleccionar_clave(total_claves);
            Clave clave_sel = db_claves[idx_clave];

            m2_procesar_texto(input, output, clave_sel, 0);
            printf("\nTexto original : %s\n", input);
            printf("Texto cifrado  : %s\n", output);

            m2_procesar_texto(output, descifrado, clave_sel, 1);
            printf("Descifrado     : %s\n", descifrado);

            if (strcmp(input, descifrado) == 0)
                printf("[OK] El descifrado coincide con el texto original.\n");
            else
                printf("[AVISO] El descifrado no coincide (posible carácter fuera de clave).\n");
            break;
        }

        /* ---- MÓDULO 3: Frecuencias y Deducción ---- */
        case 3: {
            char palabra[50];
            printf("\n-- Módulo 3: Análisis de Frecuencias y Deducción --\n");
            printf("Ingresa una palabra cifrada: ");
            fgets(palabra, sizeof(palabra), stdin);
            palabra[strcspn(palabra, "\n")] = '\0';

            m3_analisis_frecuencia(palabra);
            m3_deducir_palabra(palabra, diccionario, total_diccionario);
            break;
        }

        /* ---- MÓDULO 4: Top 10 ---- */
        case 4:
            m4_top_10_palabras(db_mensajes, total_mensajes);
            break;

        /* ---- MÓDULO 5: Búsqueda ---- */
        case 5: {
            char busqueda[50];
            printf("\n-- Módulo 5: Búsqueda de Palabra --\n");
            printf("Ingresa la palabra a buscar: ");
            fgets(busqueda, sizeof(busqueda), stdin);
            busqueda[strcspn(busqueda, "\n")] = '\0';

            m5_buscar_palabra(busqueda, db_mensajes, total_mensajes,
                              db_claves, total_claves);
            break;
        }

        case 6:
            printf("Saliendo del sistema. ¡Hasta luego!\n");
            break;

        default:
            printf("Opción no válida. Intenta de nuevo.\n");
        }
    }

    return 0;
}