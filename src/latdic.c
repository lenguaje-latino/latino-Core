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

#define LATINO_CORE

#include "latdic.h"
#include "latgc.h"
#include "latino.h"
#include "latmem.h"
#include <string.h>

/* FNV-1a hash algorithm constants */
#define FNV_OFFSET_BASIS 2166136261u
#define FNV_PRIME 16777619u

/* FNV-1a hash function for improved distribution */
static unsigned int latH_hash_fnv1a(const char *key) {
  unsigned int h = FNV_OFFSET_BASIS;
  const unsigned char *p = (const unsigned char *)key;

  while (*p) {
    h ^= *p++;
    h *= FNV_PRIME;
  }

  return h;
}

/* Resize hash table when load factor is exceeded */
static void latH_resize(lat_mv *mv, hash_map *m, size_t new_capacity) {
  lista **new_buckets =
      (lista **)latM_asignar(mv, new_capacity * sizeof(lista *));
  size_t i;

#if DEPURAR_MEM
  printf("latH_resize: %zu -> %zu\n", m->capacity, new_capacity);
#endif

  /* Initialize new buckets */
  for (i = 0; i < new_capacity; i++) {
    new_buckets[i] = NULL;
  }

  /* Rehash all existing entries */
  for (i = 0; i < m->capacity; i++) {
    lista *list = m->buckets[i];
    if (list != NULL && list->longitud > 0) {
      LIST_FOREACH(list, primero, siguiente, cur) {
        if (cur->valor) {
          hash_val *hv = (hash_val *)cur->valor;
          unsigned int h = latH_hash_fnv1a(hv->llave);
          size_t new_idx = h % new_capacity;

          if (new_buckets[new_idx] == NULL) {
            new_buckets[new_idx] = latL_crear(mv);
          }
          latL_agregar(mv, new_buckets[new_idx], hv);
        }
      }
      /* Clean up old bucket list structure (but not the hash_val data) */
      latL_limpiar(mv, list);
    }
  }

  /* Free old buckets array and update to new one */
  latM_liberar(mv, m->buckets);
  m->buckets = new_buckets;
  m->capacity = new_capacity;
}

LATINO_API hash_map *latH_crear(lat_mv *mv) {
  hash_map *ret = (hash_map *)latM_asignar(mv, sizeof(hash_map));
#if DEPURAR_MEM
  printf("latH_crear.ret: %p\n", ret);
#endif

  ret->capacity = HASH_INITIAL_CAPACITY;
  ret->longitud = 0;
  ret->buckets = (lista **)latM_asignar(mv, ret->capacity * sizeof(lista *));

  size_t i;
  for (i = 0; i < ret->capacity; i++) {
    ret->buckets[i] = NULL;
  }

  return ret;
}

LATINO_API void latH_asignar(lat_mv *mv, hash_map *m, const char *key,
                             void *val) {
  unsigned int h = latH_hash_fnv1a(key);
  size_t idx = h % m->capacity;

  /* Check if key already exists */
  if (m->buckets[idx] != NULL) {
    lista *list = m->buckets[idx];
    LIST_FOREACH(list, primero, siguiente, cur) {
      if (cur->valor) {
        if (!strcmp(((hash_val *)cur->valor)->llave, key)) {
          /* Key exists, update value */
          lat_objeto *obj = (lat_objeto *)((hash_val *)cur->valor)->valor;
          setobj2obj(obj, val);
          return;
        }
      }
    }
  } else {
    /* Create new bucket */
    m->buckets[idx] = latL_crear(mv);
  }

  /* Create new key-value pair */
  hash_val *hv = (hash_val *)latM_asignar(mv, sizeof(hash_val));
#if DEPURAR_MEM
  printf("latH_asignar.hv: %p\n", hv);
#endif
  strncpy(hv->llave, key, (strlen(key) + 1));
  lat_objeto *obj = latO_crear(mv);
  setobj2obj(obj, val);
  hv->valor = obj;
  latL_agregar(mv, m->buckets[idx], (void *)hv);
  m->longitud++;

  /* Check load factor and resize if necessary */
  double load_factor = (double)m->longitud / (double)m->capacity;
  if (load_factor > HASH_LOAD_FACTOR) {
    latH_resize(mv, m, m->capacity * 2);
  }
}

LATINO_API void latH_limpiar(lat_mv *mv, hash_map *dic) {
  size_t i;
  for (i = 0; i < dic->capacity; i++) {
    dic->buckets[i] = NULL;
  }
  latM_liberar(mv, dic->buckets);
  latM_liberar(mv, dic);
}

LATINO_API void latH_destruir(lat_mv *mv, hash_map *dic) {
  size_t i;
  for (i = 0; i < dic->capacity; i++) {
    lista *list = dic->buckets[i];
    if (list != NULL) {
      LIST_FOREACH(list, primero, siguiente, cur) {
        hash_val *hv = (hash_val *)cur->valor;
        lat_objeto *val = hv->valor;
        latO_destruir(mv, val);
      }
      latL_limpiar_destruir(mv, list);
    }
  }
  latM_liberar(mv, dic->buckets);
  latM_liberar(mv, dic);
}

/* Legacy hash function for backward compatibility - now uses FNV-1a internally
 */
int latH_hash(const char *key) {
  unsigned int h = latH_hash_fnv1a(key);
  return (int)(h % 256);
}

LATINO_API void *latH_obtener(hash_map *m, const char *key) {
  unsigned int h = latH_hash_fnv1a(key);
  size_t idx = h % m->capacity;
  lista *list = m->buckets[idx];

  if (list == NULL)
    return NULL;
  if (list->longitud > 0) {
    LIST_FOREACH(list, primero, siguiente, cur) {
      if (!strcmp(key, ((hash_val *)cur->valor)->llave)) {
        return ((hash_val *)cur->valor)->valor;
      }
    }
  }
  return NULL;
}

LATINO_API hash_map *latH_clonar(lat_mv *mv, hash_map *m) {
  hash_map *ret = latH_crear(mv);
  size_t i;
  for (i = 0; i < m->capacity; i++) {
    lista *list = m->buckets[i];
    if (list != NULL) {
      if (list->longitud > 0) {
        LIST_FOREACH(list, primero, siguiente, cur) {
          if (cur->valor) {
            char *str_key = ((hash_val *)cur->valor)->llave;
            latH_asignar(mv, ret, str_key, ((hash_val *)cur->valor)->valor);
          }
        }
      }
    }
  }
  return ret;
}
