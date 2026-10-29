# Persistencia de texto en Linux — entrega 11

Base: entrega cadenas-io10 aplicada sobre `auditoria/memoria-hash`. Se conservan
las APIs anteriores; `intentar_escribir` sigue siendo una escritura con truncado.
Esta entrega añade una operación distinta para no truncar el destino antes de
terminar la escritura. No modifica estructuras públicas ni introduce una ABI nueva.

## API Latino

`archivo.leer_texto_seguro(ruta)` devuelve una cadena (incluida la vacía) si la
lectura termina, `nulo` si `fopen` falla con ENOENT, y `falso` ante los demás
fallos, NUL embebido o contenido superior a 8 MiB. ENOENT también puede indicar
un directorio padre ausente. La lectura no valida UTF-8: lo hace el consumidor.

`archivo.escribir_atomico(ruta, texto)` devuelve un número; comparar su valor
explícitamente, sin convertirlo simplemente a booleano:

| Valor | Significado |
|---|---|
| 0 | Fallo anterior al reemplazo. El destino no fue modificado por esta operación, bajo las condiciones indicadas abajo. |
| 1 | Temporal escrito, sincronizado y cerrado; reemplazo y sincronización/cierre del directorio confirmados. |
| 2 | El reemplazo ya ocurrió, pero falló la sincronización o cierre del directorio. El archivo nuevo ya está instalado; no afirmar que el antiguo sigue vigente. El registro recibe una advertencia. |
| -1 | Plataforma distinta de Linux: implementación no disponible. No se recurre a escritura con truncado. |

Los fallos operativos de E/S regresan esos estados, sin lanzar un error de
Latino. Los tipos de argumentos incorrectos siguen siendo errores del lenguaje;
no se promete recuperación de una VM tras falta de memoria o un error fatal.

## Contrato y límites

Implementación Linux para texto sin NUL, hasta 8 MiB, en un sistema de archivos
local y un directorio privado y estable. El destino existente debe ser regular;
se rechazan directorios, dispositivos y enlaces simbólicos finales.

Se crea un temporal exclusivo `.latino-XXXXXX` en el mismo directorio, con 0600 y
close-on-exec. Se completan escrituras cortas, se reintenta EINTR en write/fsync,
se comprueban fsync y close, y solo entonces se usa rename. Después se sincroniza
el directorio. En Linux no se reintenta close tras error. Si se falla antes del
reemplazo, se cierra y se intenta retirar únicamente el temporal creado.

El reemplazo cambia el inode: el nuevo archivo tiene 0600 y no hereda el modo,
propietario, ACL ni atributos adicionales del anterior. Otros enlaces duros al
inode antiguo conservan su contenido antiguo. Esta política es deliberada para
los datos privados de la demostración, no un reemplazo universal de archivos.

No hay bloqueo entre procesos, detección de ediciones externas ni historial de
copias. No se cubren directorios cambiados concurrentemente ni sistemas remotos
como NFS, donde incluso algunos errores de rename tienen otra interpretación.
Un cierre forzado puede dejar temporales; también puede fallar su retirada.
No se borran temporales ajenos automáticamente. fsync exitoso es una confirmación
del sistema, no una prueba de resistencia a cortes de energía de todo hardware.
La escritura es síncrona: el presupuesto del despachador no interrumpe una E/S lenta.

## Comparación de cadenas

La ruta nueva usa `tipo(valor) == "cadena"`. La comprobación complementaria de
memoria detectó que `==` y `!=` convertían otra cadena mediante strdup y liberaban
la copia con latM_liberar, descontando bytes nunca contados. El primer escenario
terminaba con un contador equivalente a -12, no con una fuga de 12 bytes.
Ahora dos cadenas se comparan directamente, sin reservar temporales. Se conserva
la semántica de conversión entre otros tipos; esas rutas no se declaran auditadas.

## Validación

`bash tests/run_atomic_io_regression.sh` ejecuta funciones reales de archivo y VM.
Solo se sustituyen llamadas de sistema para provocar errores deterministas:
creación de temporal, escritura parcial/ENOSPC/cero, fsync de archivo/directorio,
close de archivo/directorio, rename y EINTR. Comprueba contenido, temporales,
descriptor cerrado, permisos, límites, estados de lectura, comparaciones y
20 cierres de VM con saldo interno cero. No simula un corte eléctrico real.

El lanzador usa ASan/UBSan/LSan por defecto. La entrega también conserva las ocho
suites anteriores. En el entorno de preparación LSan no pudo ejecutarse por la
restricción de ptrace; el saldo interno complementa, pero no reemplaza LSan.

Referencias de las operaciones utilizadas:
- https://man7.org/linux/man-pages/man2/rename.2.html
- https://man7.org/linux/man-pages/man2/fsync.2.html
- https://man7.org/linux/man-pages/man3/mkstemp.3.html
- https://man7.org/linux/man-pages/man2/close.2.html
