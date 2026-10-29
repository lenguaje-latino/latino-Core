# Objetos11B: conversiones y representación de objetos

Requiere Persistencia11. No cambia el tamaño de lat_mv ni añade una ABI binaria.

`latC_astring` entrega una cadena propia, terminada en NUL, que siempre se libera
con `free`, también para T_CHAR. Las comparaciones de la VM usan esa propiedad.
Los buffers de libc no se descuentan del contador latM de la VM. La evaluación
booleana de cadenas reconoce los valores históricos sin copias temporales.

Listas/diccionarios usan un constructor que comprueba capacidad y recorre todos
los buckets. Su salida agregada está limitada a MAX_STR_LENGTH−1 bytes; se admiten
64 contenedores anidados y se rechazan ciclos en la ruta de recursión. Reutilizar
un mismo contenedor en ramas distintas no se considera ciclo. Ante rechazo se
libera el buffer antes de lanzar el error habitual de Latino. Esto no promete
recuperación del host después de un error fatal de VM.

Las cadenas escalares se copian completas. Los enteros se validan antes de la
conversión a int; T_CHAR representa un byte y NUL se convierte en cadena vacía.
La representación entrecomilla cadenas/llaves y escapa comillas, barras, LF/CR/TAB.
No es JSON ni un formato de persistencia y puede diferir de la salida histórica.

`latO_imprimir(..., false)` imprime literalmente: no reinterpreta barras de datos
externos. El modo true conserva el decodificador histórico de formato, que no se
certifica con esta corrección. Tampoco se revisa aquí todo el ciclo de vida de
listas/diccionarios ni todos los consumidores de conversiones de Core.

La nueva suite `bash tests/run_object_text_regression.sh` comprueba booleanos,
caracteres, impresión literal, comparaciones mixtas, hash de 2000 entradas,
compartición, ciclos, profundidad y límite de tamaño. Usa VM real, observa el saldo
al destruirla y rastrea los buffers libc de la unidad latobj.c. Intercepta sus
errores solo para comprobar los rechazos; no prueba recuperación general longjmp.

En la preparación se aprobaron diez suites Core y quince pruebas UI con ASan/UBSan.
LSan quedó bloqueado por ptrace; se debe ejecutar la comprobación en el equipo
Linux del usuario con detect_leaks=1. No se validaron Windows, GC activado ni GUI
real en este entorno. Consultar la propuesta revisada y la evidencia de la entrega.
