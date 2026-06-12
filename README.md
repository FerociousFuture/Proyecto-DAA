# Sistema Cryptonituv

## Integrantes del Proyecto

Juan Alberto De Los Santos Sanchez - S23002536
Haziel Lopez Castillo - S22002198


---

## Descripción

Cryptonituv es un sistema de consola en C que administra una base de datos de mensajes cifrados mediante sustitución monoalfabética. Permite ordenar mensajes, cifrar y descifrar texto, analizar frecuencias de caracteres, identificar las palabras más frecuentes y buscar equivalencias cifradas.

---

## Estructura de Archivos

```
Proyecto-DAA/
├── main.c
├── mod1_ordenamiento.c
├── mod2_cifrado.c
├── mod3_frecuencias.c
├── mod4_top10.c
├── mod5_busqueda.c
├── modulos.h
├── estructuras.h
├── output/
│   └── main.exe
└── Cryptonituv_DB/
    ├── mensajes_cifrados.txt
    ├── claves.txt
    ├── palabras_frecuentes.txt
    ├── frecuencias.txt
    ├── remitentes.txt
    └── README.txt
```

---

## Compilación

Requiere `gcc` compatible con C99/C11.

**Linux / macOS:**
```bash
gcc -std=c11 -Wall -O2 main.c mod1_ordenamiento.c mod2_cifrado.c mod3_frecuencias.c mod4_top10.c mod5_busqueda.c -o cryptonituv
```

**Windows (MinGW):**
```cmd
gcc -std=c11 -Wall -O2 main.c mod1_ordenamiento.c mod2_cifrado.c mod3_frecuencias.c mod4_top10.c mod5_busqueda.c -o cryptonituv.exe
```

> Si `main.c` incluye los otros `.c` directamente con `#include`, basta compilar solo `main.c`.

---

## Ejecución

El ejecutable debe lanzarse desde la carpeta raíz del proyecto, ya que los archivos de datos se buscan con la ruta relativa `Cryptonituv_DB/`.

```bash
# Linux / macOS
./cryptonituv

# Windows
cryptonituv.exe
output\main.exe   # ejecutable precompilado
```

---

## Archivos de Datos

| Archivo | Descripción | Formato |
|---|---|---|
| `mensajes_cifrados.txt` | Base de datos principal (~1000 mensajes) | `id|remitente|prioridad|fecha|clave_id|texto_original|texto_cifrado` |
| `claves.txt` | 15 tablas de sustitución (K01–K10, S01–S05) | `id|nombre|tipo|alfabeto_original|alfabeto_cifrado` |
| `palabras_frecuentes.txt` | Diccionario para el Módulo 3 | Una palabra por línea |
| `frecuencias.txt` | Frecuencias de letras de referencia | Formato interno |
| `remitentes.txt` | Listado de remitentes | `id|nombre` |

---

## Flujo del Programa

```
========== MENU PRINCIPAL ==========
  1. Modulo 1 - Listar y ordenar mensajes
  2. Modulo 2 - Cifrar / Descifrar texto
  3. Modulo 3 - Analisis de frecuencias y deduccion
  4. Modulo 4 - Top 10 palabras mas presentes
  5. Modulo 5 - Buscar palabra y ver equivalencias
  6. Salir
```

Al iniciar, el programa carga automáticamente los tres archivos de la base de datos. Desde el menú principal se accede a cada módulo de forma independiente. La opción 6 termina la ejecución.

---

## Implementación por Módulo

### estructuras.h
Define los tipos base del sistema: el enum `Prioridad` (BAJA/MEDIA/ALTA), el struct `Mensaje` con todos los campos del registro, el struct `Clave` con la tabla de sustitución, el `WordNode` para la tabla hash del Módulo 4 y `FrecuenciaLetra` para el Módulo 3. También define las constantes `MAX_MENSAJES` (1500), `MAX_CLAVES` (20), `HASH_SIZE` (10007) y `MAX_PALABRAS_DICT` (50000).

### mod1_ordenamiento.c
Contiene `cargar_datos()`, que lee los tres archivos de la DB, y las cuatro funciones de ordenamiento: `m1_ordenar_prioridad_desc()` (Counting Sort), `m1_merge_sort_fechas()` (Merge Sort), `m1_shell_sort_remitente()` (Shell Sort) y `m1_ordenar_longitud_asc()` (Counting Sort). La función `m1_imprimir_mensajes()` muestra los primeros N registros en formato tabular.

### mod2_cifrado.c
Una sola función pública: `m2_procesar_texto()`. El parámetro `descifrar` (0 = cifrar, 1 = descifrar) controla el modo. El arreglo `pos[]` registra qué posiciones fueron transformadas al cifrar, garantizando que el descifrado sea exacto incluso con caracteres especiales o números.

### mod3_frecuencias.c
`m3_analisis_frecuencia()` cuenta la frecuencia de cada carácter y muestra el top 5 usando `qsort`. `m3_deducir_palabra()` recorre el diccionario buscando palabras con el mismo patrón estructural (isomorfismo de strings) que la palabra cifrada. La función interna `generar_patron_estructural()` convierte una palabra a su representación numérica, por ejemplo `"hola"` → `"0.1.2.3."`.

### mod4_top10.c
`m4_top_10_palabras()` tokeniza todos los mensajes, inserta las palabras en una tabla hash (función `hash()` djb2) y evita contar el mismo mensaje dos veces mediante `last_doc_id`. Luego recolecta los nodos en un arreglo dinámico, ordena con `qsort` y muestra el top 10.

### mod5_busqueda.c
`m5_buscar_palabra()` recorre todos los mensajes con `strstr()`, cifra la palabra con la clave de cada mensaje y agrupa los resultados por equivalente cifrado. El resultado se ordena descendente por número de mensajes antes de mostrarse.

---

## Complejidades

| Módulo | Operación | Algoritmo | Complejidad |
|---|---|---|---|
| Módulo 1 | Ordenar por prioridad DESC | Counting Sort | O(n + m) |
| Módulo 1 | Ordenar por fecha ASC | Merge Sort | O(n log n) |
| Módulo 1 | Ordenar por remitente ASC | Shell Sort | O(n log² n) |
| Módulo 1 | Ordenar por longitud ASC | Counting Sort | O(n + m) |
| Módulo 2 | Cifrado / Descifrado | Sustitución monoalfabética | O(L) |
| Módulo 3 | Análisis de frecuencias | Conteo + qsort | O(L + k log k) |
| Módulo 3 | Deducción de palabras | Isomorfismo estructural | O(p) |
| Módulo 4 | Top 10 palabras | Hash table + qsort | O(T + w log w) |
| Módulo 5 | Búsqueda y equivalencias | strstr + agrupación lineal | O(T + q) |

`n` = mensajes · `m` = rango de valores · `L` = longitud del texto · `k` = alfabeto (26) · `p` = tamaño del diccionario · `T` = total de caracteres · `w` = palabras únicas · `q` = longitud de la palabra buscada

---

## Pruebas Realizadas

| # | Módulo | Entrada | Resultado |
|---|---|---|---|
| 1 | Módulo 1 – Opción 1 | Prioridad DESC, mostrar 3 | Los 3 primeros tienen prioridad `alta`  |
| 2 | Módulo 1 – Opción 2 | Fecha ASC, mostrar 3 | Ana (2026-02-01), Bruno (2026-02-02), Carla (2026-02-03)  |
| 3 | Módulo 1 – Opción 3 | Remitente ASC, mostrar 3 | Los 3 primeros son `Adrian`  |
| 4 | Módulo 1 – Opción 4 | Longitud ASC, mostrar 3 | Longitud mínima 13 primero, luego 58  |
| 5 | Módulo 2 | Texto `servidor`, clave K01 | Cifrado: `ltkcorgk`. Descifrado: `servidor`  |
| 6 | Módulo 3 | Palabra cifrada `ltkcorgk` | Patrón `0.1.2.3.4.5.6.2`. Propuesta: `servidor`  |
| 7 | Módulo 4 | Todos los mensajes | Top 1: `de` (629), Top 2: `para` (383), Top 3: `con` (281)  |
| 8 | Módulo 5 | Buscar `servidor` | 68 mensajes. Equivalencias: `racqsopc` K07 (34), `ltkcorgk` K01 (34)  |
