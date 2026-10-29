# Cadenas y E/S para la ruta de formularios

Base revisada: `2722523442d7f15537bbc51ab1af0c5919da2e56`.

- `cadena.recortar` reserva el terminador incluso para vacío, no cambia la
  entrada internada y libera su buffer temporal. Mantiene `isspace` por bytes;
  no implementa todas las categorías de espacio Unicode.
- `cadena.separar` trabaja sobre una copia con `strspn`/`strcspn`, sin estado
  global ni dependencia POSIX `strtok_r`. Conserva los delimitadores por bytes,
  descarta tokens vacíos y usa espacio como separador vacío/nulo predeterminado.
  No es CSV y no interpreta un separador multibyte como una unidad Unicode.
- `latC_apilar_string` y los operadores `&&`/`||` liberan el envoltorio temporal
  después de copiar el valor a la pila, con GC desactivado. Los operadores
  mantienen la selección/clonado del operando; no se ha añadido cortocircuito.
- `cadena.longitud` sigue contando bytes y ya no reserva un envoltorio numérico.
- `archivo.lineas` lee líneas mayores que 255 bytes y la última sin salto;
  conserva `\r` si el archivo tiene CRLF. Archivo vacío produce lista vacía.
- `archivo.leer` / `archivo.intentar_leer` devuelven cadena o falso. Distinguen
  archivo vacío (`""`) de fallo (`falso`). Leen por bloques y no dependen de ftell.
  En esta entrega se limita la lectura a 8 MiB y se rechaza NUL embebido: son
  operaciones de texto, no una API binaria. Esta restricción también afecta
  `archivo.lineas` y debe considerarse al migrar programas que leen más datos.
- `archivo.intentar_escribir(ruta, texto)` devuelve booleano sin lanzar un error
  por fallos de E/S. Comprueba apertura, escritura y cierre. La función histórica
  `archivo.escribir` conserva su contrato de error; igual que `archivo.anexar`,
  ahora comprueba los fallos antes de usar el descriptor.
- `archivo.crear` no cierra NULL y evita truncar un archivo existente usando el
  modo exclusivo de C11. `archivo.renombrar` ya no copia rutas en un buffer fijo.

Los tipos incorrectos siguen siendo errores de programación. Tampoco se
promete recuperación de falta de memoria en cualquier llamada interna del
intérprete. Las funciones nuevas pertenecen a `archivo`, para que la E/S no
quede acoplada a un módulo gráfico `ui`.

`intentar_escribir` abre el destino con truncado: un error posterior puede dejar
contenido parcial. No promete reemplazo atómico, recuperación de cortes de
energía, sincronización a disco ni bloqueo entre escritores. Esas garantías
son requisito del futuro almacenamiento de la aplicación de biblioteca.

Las regresiones de separar/lineas liberan las listas devueltas desde C. No
resuelven la propiedad general de listas o diccionarios temporales creados por
scripts. La edición del formulario usa cadenas y escalares; no debe extrapolarse
su validación a un CRUD que crea listas repetidamente.

Ejemplo de E/S optativa (capturar siempre el retorno):

```latino
exito = archivo.intentar_escribir("nombre.txt", "María Ñáñez")
contenido = archivo.intentar_leer("nombre.txt")
si (contenido == falso)
    escribir("No se pudo leer el archivo")
fin
```

Prueba: `bash tests/run_strings_io_regression.sh`. Requiere Linux, compilador C,
Readline, ASan y UBSan. Windows y GC activado no están validados en esta entrega.
