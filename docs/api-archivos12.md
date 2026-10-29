# API C de archivos para Biblioteca12

Esta entrega expone el motor de Entrega11 a los consumidores C. No añade otra
implementación de escritura atómica ni modifica la propiedad de objetos de la VM.

```c
char *latF_leer_texto_seguro(const char *path, int *estado);
int latF_escribir_atomico(const char *path, const char *texto);
```

Ambos símbolos se declaran con `LATINO_API` en `latino.h` y se añaden a
`src/latino.def`. Es necesario recompilar Latino-Core y el consumidor. Estas
funciones no cambian el tamaño de `lat_mv`; el consumidor requiere una biblioteca
que exporte los símbolos nuevos. Windows no se ha validado.

## Lectura

`path` es una ruta C terminada en NUL. La función devuelve un buffer terminado en
NUL que el llamador libera con `free()`, o NULL. No usar `latM_liberar()` para este
buffer: la reserva no se contabiliza en la VM. `estado` puede ser NULL.

| Estado | Significado |
| --- | --- |
| -1 | Argumento, lectura, cierre, reserva, tamaño o contenido inválido |
| 0 | La apertura falló con ENOENT |
| 1 | Lectura completa, incluido un archivo vacío |

El máximo es 8 MiB. Se rechaza cualquier NUL dentro del contenido. La API no
valida UTF-8, esquema, tipo de archivo, permisos del directorio ni normaliza
rutas: esas comprobaciones corresponden al consumidor. ENOENT también puede
indicar que falta un directorio padre; no prueba por sí solo que sea posible
crear el archivo.

## Escritura

`texto` es una cadena C sin NUL internos y de hasta 8 MiB. El buffer se consume
durante la llamada y no se retiene. No se crea automáticamente el directorio.

| Estado | Archivo y actuación del consumidor |
| --- | --- |
| -1 | Plataforma no implementada; no activar el candidato |
| 0 | No se reemplazó el destino; conservar el modelo anterior |
| 1 | Reemplazo y sincronización confirmados; activar el candidato |
| 2 | Ya se reemplazó; activar el candidato y advertir que falta confirmar la sincronización o el cierre del directorio |

Estado 2 no es un rollback ni permite afirmar que el disco contiene todavía la
versión anterior. La implementación Linux usa temporal exclusivo en el mismo
directorio, escritura completa, fsync del temporal, cierre, rename y fsync del
directorio. No hay alternativa que trunque directamente el archivo destino.

El alcance es Linux, sistema de archivos local, directorio privado estable y un
solo escritor. No ofrece coordinación entre procesos, control de versiones,
protección frente a cambios concurrentes de la ruta ni garantías para sistemas
de archivos remotos. Las pruebas de fallos de Entrega11 siguen en
`tests/run_atomic_io_regression.sh`.

## Validación de Biblioteca12

Diez suites de Core y veinte casos CTest de LatinoUI pasaron en el entorno de
preparación; CTest se ejecutó en Debug con ASan/UBSan y en Release. LeakSanitizer
no pudo ejecutarse bajo ptrace aquí: su resultado está pendiente en el equipo
destino, donde `comprobar_biblioteca_ui.sh` mantiene `detect_leaks=1`.
