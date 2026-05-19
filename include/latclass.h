/*
The MIT License (MIT)

Copyright (c) Latino - Lenguaje de Programacion

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef _LATCLASS_H_
#define _LATCLASS_H_

#include <stdbool.h>

// Forward declarations
typedef struct lat_mv lat_mv;
typedef struct _lat_objeto lat_objeto;
typedef struct hash_map hash_map;

/**
 * @brief Estructura que representa una clase en Latino
 */
typedef struct lat_class {
  char *nombre;
  hash_map *metodos;
  hash_map *metodos_estaticos;
  lat_objeto *constructor;
  struct lat_class *padre;
  int num_propiedades;
} lat_class;

/**
 * @brief Estructura que representa una instancia de clase
 */
typedef struct lat_instancia {
  lat_class *clase;
  hash_map *propiedades;
} lat_instancia;

// Funciones para manejo de clases
lat_class *latC_crear_clase(lat_mv *mv, const char *nombre, lat_class *padre);
lat_objeto *latC_crear_instancia(lat_mv *mv, lat_class *clase);
void latC_agregar_metodo(lat_mv *mv, lat_class *clase, const char *nombre,
                         lat_objeto *funcion, bool estatico);
void latC_establecer_constructor(lat_class *clase, lat_objeto *constructor);
lat_objeto *latC_obtener_metodo(lat_class *clase, const char *nombre);
lat_objeto *latC_obtener_propiedad(lat_instancia *inst, const char *nombre);
void latC_asignar_propiedad(lat_mv *mv, lat_instancia *inst, const char *nombre,
                            lat_objeto *valor);
lat_objeto *latC_llamar_metodo(lat_mv *mv, lat_objeto *instancia,
                               const char *metodo, int nargs);
void latC_llamar_super(lat_mv *mv, lat_objeto *instancia, int nargs);
void latC_destruir_clase(lat_mv *mv, lat_class *clase);
void latC_destruir_instancia(lat_mv *mv, lat_instancia *inst);

#endif /* _LATCLASS_H_ */
