Cryptonituv - Base de datos ampliada para proyecto en C

Formato general
---------------
Todos los archivos usan texto plano y campos separados por el caracter |.
La primera linea de cada archivo contiene encabezados.
Los textos originales estan en minusculas, sin acentos y usando el alfabeto:
abcdefghijklmnopqrstuvwxyz

Archivos incluidos
------------------
1. claves.txt
   clave_id|nombre|tipo|alfabeto_original|tabla_sustitucion

2. mensajes_cifrados.txt
   mensaje_id|remitente|prioridad|fecha|clave_id|texto_original|texto_cifrado

3. remitentes.txt
   remitente_id|nombre

4. frecuencias.txt
   letra|frecuencia_esperada

5. palabras_frecuentes.txt
   palabra

Notas para implementacion en C
------------------------------
- Leer cada linea con fgets.
- Separar campos usando el caracter |.
- Para cifrar, buscar la posicion de cada letra original y reemplazarla por
  la letra o simbolo en la misma posicion dentro de tabla_sustitucion.
- Para descifrar claves alfabeticas o simbolicas, construir una tabla inversa.
- Los espacios se conservan sin cifrar.
- Las claves de tipo simbolos pueden generar caracteres como !, @, # o ?.
  El separador | no aparece dentro de ninguna tabla de simbolos para no romper
  el formato del archivo.
- Las fechas estan en formato ISO: AAAA-MM-DD, por lo que se pueden comparar
  como cadenas si todas conservan el mismo formato.
- El archivo mensajes.txt se conserva como copia de mensajes_cifrados.txt por
  compatibilidad con versiones anteriores de la actividad.

Datos cubiertos
---------------
- 10 claves de sustitucion alfabetica.
- 5 claves de sustitucion con simbolos.
- 1000 mensajes cifrados.
- 50 remitentes distintos, incluyendo Ana, Bruno, Carla, Diego y Elena.
- Conjunto amplio de palabras frecuentes para analisis y apoyo al descifrado.
