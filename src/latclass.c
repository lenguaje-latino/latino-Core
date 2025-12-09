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

#include "latclass.h"
#include "latdic.h"
#include "latino.h"
#include "latmem.h"
#include "latmv.h"
#include <string.h>

lat_class *latC_crear_clase(lat_mv *mv, const char *nombre, lat_class *padre) {
  lat_class *clase = (lat_class *)latM_asignar(mv, sizeof(lat_class));
  if (!clase) {
    return NULL;
  }

  clase->nombre = strdup(nombre);
  clase->metodos = latH_crear(mv);
  clase->metodos_estaticos = latH_crear(mv);
  clase->constructor = NULL;
  clase->padre = padre;
  clase->num_propiedades = 0;

  return clase;
}

lat_objeto *latC_crear_instancia(lat_mv *mv, lat_class *clase) {
  if (!clase) {
    return NULL;
  }

  lat_instancia *inst =
      (lat_instancia *)latM_asignar(mv, sizeof(lat_instancia));
  if (!inst) {
    return NULL;
  }

  inst->clase = clase;
  inst->propiedades = latH_crear(mv);

  lat_objeto *obj = latO_crear(mv);
  if (!obj) {
    return NULL;
  }

  obj->tipo = T_INSTANCE;
  obj->val.cpointer = inst;

  return obj;
}

void latC_agregar_metodo(lat_mv *mv, lat_class *clase, const char *nombre,
                         lat_objeto *funcion, bool estatico) {
  if (!clase || !nombre || !funcion) {
    return;
  }

  hash_map *map = estatico ? clase->metodos_estaticos : clase->metodos;
  latH_asignar(mv, map, nombre, funcion);
}

void latC_establecer_constructor(lat_class *clase, lat_objeto *constructor) {
  if (!clase || !constructor) {
    return;
  }

  clase->constructor = constructor;
}

lat_objeto *latC_obtener_metodo(lat_class *clase, const char *nombre) {
  if (!clase || !nombre) {
    return NULL;
  }

  // Buscar en clase actual
  lat_objeto *metodo = latH_obtener(clase->metodos, nombre);
  if (metodo) {
    return metodo;
  }

  // Buscar en clase padre (herencia)
  if (clase->padre) {
    return latC_obtener_metodo(clase->padre, nombre);
  }

  return NULL;
}

lat_objeto *latC_obtener_propiedad(lat_instancia *inst, const char *nombre) {
  if (!inst || !nombre) {
    return NULL;
  }

  return latH_obtener(inst->propiedades, nombre);
}

void latC_asignar_propiedad(lat_mv *mv, lat_instancia *inst, const char *nombre,
                            lat_objeto *valor) {
  if (!inst || !nombre || !valor) {
    return;
  }

  latH_asignar(mv, inst->propiedades, nombre, valor);
}

lat_objeto *latC_llamar_metodo(lat_mv *mv, lat_objeto *instancia,
                               const char *metodo, int nargs) {
  if (!instancia || instancia->tipo != T_INSTANCE || !metodo) {
    latC_error(mv, "No es una instancia de clase");
    return NULL;
  }

  lat_instancia *inst = (lat_instancia *)instancia->val.cpointer;
  if (!inst || !inst->clase) {
    latC_error(mv, "Instancia inválida");
    return NULL;
  }

  lat_objeto *func = latC_obtener_metodo(inst->clase, metodo);
  if (!func) {
    latC_error(mv, "Método '%s' no encontrado en clase '%s'", metodo,
               inst->clase->nombre);
    return NULL;
  }

  // Guardar instancia actual en contexto (para 'mi')
  // TODO: Implementar contexto de 'mi' cuando se implemente el bytecode

  // Llamar función y obtener resultado de la pila
  latC_llamar_funcion(mv, func);
  lat_objeto *resultado = latC_desapilar(mv);

  // TODO: Restaurar 'mi' anterior

  return resultado;
}

void latC_llamar_super(lat_mv *mv, lat_objeto *instancia, int nargs) {
  if (!instancia || instancia->tipo != T_INSTANCE) {
    latC_error(mv, "No es una instancia de clase");
    return;
  }

  lat_instancia *inst = (lat_instancia *)instancia->val.cpointer;
  if (!inst || !inst->clase) {
    latC_error(mv, "Instancia inválida");
    return;
  }

  if (!inst->clase->padre) {
    const char *clase_nombre = inst->clase && inst->clase->nombre ? inst->clase->nombre : "(sin nombre)";
    latC_error(mv, "La clase '%s' no tiene clase padre", clase_nombre);
    return;
  }

  if (!inst->clase->padre->constructor) {
    const char *padre_nombre = inst->clase->padre && inst->clase->padre->nombre ? inst->clase->padre->nombre : "(sin nombre)";
    latC_error(mv, "Clase padre '%s' no tiene constructor",
               padre_nombre);
    return;
  }

  // Activar instrumentación de super() para el constructor padre
  mv->dbg_super_active = true;
  mv->dbg_target_ctor = inst->clase->padre->constructor;
  mv->dbg_super_depth = 0;

  // Usar la función especializada que ejecuta constructor padre con contexto
  // correcto e inyección de 'mi'
  latMV_ejecutar_constructor_padre(mv, inst->clase->padre->constructor,
                                   instancia, nargs);

  // Desactivar instrumentación tras ejecutar el constructor padre
  mv->dbg_super_active = false;
  mv->dbg_target_ctor = NULL;
  mv->dbg_super_depth = 0;

}

void latC_destruir_clase(lat_mv *mv, lat_class *clase) {
  if (!clase) {
    return;
  }

  // Liberar hash maps de métodos
  if (clase->metodos) {
    latH_destruir(mv, clase->metodos);
  }

  if (clase->metodos_estaticos) {
    latH_destruir(mv, clase->metodos_estaticos);
  }

  // Liberar nombre
  if (clase->nombre) {
    latM_liberar(mv, (void *)clase->nombre);
  }

  // Liberar estructura
  latM_liberar(mv, clase);
}

void latC_destruir_instancia(lat_mv *mv, lat_instancia *inst) {
  if (!inst) {
    return;
  }

  // Liberar hash map de propiedades
  if (inst->propiedades) {
    latH_destruir(mv, inst->propiedades);
  }

  // Liberar estructura
  latM_liberar(mv, inst);
}
