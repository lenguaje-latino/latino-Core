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

#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "latdic.h"
#include "latgc.h"
#include "latino.h"
#include "latlist.h"
#include "latmem.h"
#include "latobj.h"

/* Los envoltorios de T_FUN se copian sin contar referencias. Por ello el
   payload se conserva hasta el cierre de la VM, no hasta destruir una copia.
   Este registro NO es un GC de objetos ni recorre metadatos del bytecode. */
struct lat_funcion_registro {
  lat_funcion *funcion;
  struct lat_funcion_registro *siguiente;
};

static bool funcion_registrada(lat_mv *mv, lat_funcion *fun) {
  if (!mv) return false;
  for (struct lat_funcion_registro *r = mv->funciones_propias; r; r = r->siguiente)
    if (r->funcion == fun) return true;
  return false;
}

void latO_registrar_funcion(lat_mv *mv, lat_funcion *fun) {
  if (!mv || !fun || funcion_registrada(mv, fun)) return;
  struct lat_funcion_registro *r = latM_asignar(mv, sizeof(*r));
  r->funcion = fun;
  r->siguiente = mv->funciones_propias;
  mv->funciones_propias = r;
}

void latO_liberar_funciones(lat_mv *mv) {
  if (!mv) return;
  struct lat_funcion_registro *r = mv->funciones_propias;
  mv->funciones_propias = NULL;
  while (r) {
    struct lat_funcion_registro *siguiente = r->siguiente;
    /* nombre se reserva con strdup en el compilador (no latM_asignar). */
    free(r->funcion->nombre);
    latM_liberar(mv, r->funcion->codigo);
    latM_liberar(mv, r->funcion);
    latM_liberar(mv, r);
    r = siguiente;
  }
}

lat_objeto latO_nulo_ = {{NULL}, T_NULL};
/* Los metadatos del compilador se comparten con instrucciones y contextos.
   Solo sus envoltorios pertenecen a este registro (GC desactivado). */
struct lat_constante_registro {
  lat_objeto *objeto;
  struct lat_constante_registro *siguiente;
};

static bool constante_registrada(lat_mv *mv, lat_objeto *o) {
  if (!mv) return false;
  for (struct lat_constante_registro *r = mv->constantes_propias; r; r = r->siguiente)
    if (r->objeto == o) return true;
  return false;
}

lat_objeto *latO_retener_constante(lat_mv *mv, lat_objeto *o) {
#if !HABILITAR_GC
  if (mv && o && o != latO_nulo && o != latO_verdadero && o != latO_falso &&
      !constante_registrada(mv, o)) {
    struct lat_constante_registro *r = latM_asignar(mv, sizeof(*r));
    r->objeto = o;
    r->siguiente = mv->constantes_propias;
    mv->constantes_propias = r;
  }
#endif
  return o;
}

void latO_liberar_constantes(lat_mv *mv) {
  if (!mv) return;
  while (mv->constantes_propias) {
    struct lat_constante_registro *r = mv->constantes_propias;
    mv->constantes_propias = r->siguiente;
    latM_liberar(mv, r->objeto);
    latM_liberar(mv, r);
  }
}

void latS_liberar_cadenas(lat_mv *mv) {
#if !HABILITAR_GC
  stringtable *tb = &mv->global->strt;
  for (int i = 0; i < tb->size; ++i) {
    lat_gcobjeto *p = tb->hash[i];
    while (p) {
      lat_gcobjeto *siguiente = p->gch.next;
      latM_liberar(mv, p);
      p = siguiente;
    }
    tb->hash[i] = NULL;
  }
  tb->nuse = 0;
#endif
}
lat_objeto latO_verdadero_ = {.val.logico = 1, .tipo = T_BOOL};
lat_objeto latO_falso_ = {.val.logico = 0, .tipo = T_BOOL};

char *minusculas(const char *str);
char *logico_acadena(int i);
char *decimal_acadena(double d);
char *entero_acadena(int i);
char *reemplazar(char *str, const char *orig, const char *rep);
char *analizar_fmt(const char *s, size_t len);
char *analizar(const char *s, size_t len);

void latO_asignar_ctx(lat_mv *mv, lat_objeto *ns, const char *name,
                      lat_objeto *o) {
  if (ns->tipo != T_CONTEXT) {
    latC_error(mv, "Objeto no es un contexto");
  } else {
    hash_map *h = getCtx(ns);
    if (strlen(name) > MAX_ID_LENGTH) {
      latC_error(mv, "Nombre de id mayor a %i caracteres", MAX_ID_LENGTH);
    }
    latH_asignar(mv, h, name, o);
  }
}

lat_objeto *latO_obtener_contexto(lat_mv *mv, lat_objeto *ns,
                                  const char *name) {
  if (ns->tipo != T_CONTEXT) {
    latC_error(mv, "Objeto no es un contexto");
  } else {
    hash_map *h = getCtx(ns);
    lat_objeto *ret = (lat_objeto *)latH_obtener(h, name);
    return ret;
  }
  return NULL;
}

lat_objeto *latO_crear(lat_mv *mv) {
  lat_objeto *ret = (lat_objeto *)latM_asignar(mv, sizeof(lat_objeto));
#if DEPURAR_MEM
  printf("latO_crear.ret: %p\n", ret);
#endif
  ret->tipo = T_NULL;
  ret->tam = sizeof(lat_objeto);
  ret->nref = 0;
  ret->es_vararg = 0;
  ret->marca = 0;
#ifdef HABILITAR_GC
  gc_agregar(mv, ret);
#endif // HABILITAR_GC
  return ret;
}

lat_objeto *latO_contexto_crear(lat_mv *mv) {
  lat_objeto *ret = latO_crear(mv);
  ret->tipo = T_CONTEXT;
  ret->tam += sizeof(hash_map);
  setCtx(ret, latH_crear(mv));
  return ret;
}

static lat_cadena *nuevaCad(lat_mv *mv, const char *str, size_t l,
                            unsigned int h) {
  lat_cadena *ts;
  stringtable *tb;
  if (l + 1 > LAT_SIZE_MAX - sizeof(lat_cadena)) {
    latC_error(mv, "Cadena muy larga");
  }
  ts = (lat_cadena *)latM_asignar(mv, (l + 1) + sizeof(lat_cadena));
#if DEPURAR_MEM
  printf("nuevaCad.ts: %p\n", ts);
#endif
  ts->tsv.len = l;
  ts->tsv.hash = h;
  ts->tsv.marked = 0;
  ts->tsv.tipo = T_STR;
  ts->tsv.reserved = 0;
  memcpy(ts + 1, str, l);
  ((char *)(ts + 1))[l] = '\0';
  tb = &mv->global->strt;
  h = lmod(h, tb->size);
  ts->tsv.next = tb->hash[h];
  tb->hash[h] = (lat_gcobjeto *)ts;
  tb->nuse++;
  if (tb->nuse > tb->size && tb->size <= INT_MAX / 2) {
    latS_resize(mv, tb->size * 2);
  }
  return ts;
}

static lat_cadena *latO_cadenaNueva(lat_mv *mv, const char *str, size_t l) {
  lat_gcobjeto *o;
  /* Use FNV-1a hash for better distribution and consistency */
  unsigned int h = FNV_OFFSET_BASIS;
  const unsigned char *p = (const unsigned char *)str;
  size_t i;
  for (i = 0; i < l; i++) {
    h ^= p[i];
    h *= FNV_PRIME;
  }
  for (o = mv->global->strt.hash[lmod(h, mv->global->strt.size)]; o != NULL;
       o = o->gch.next) {
    lat_cadena *ts = &(o->cadena);
    if (ts->tsv.len == l && (0 == memcmp(str, getstr(ts), l))) {
      return ts;
    }
  }
  return nuevaCad(mv, str, l, h);
}

lat_objeto *latO_crear_funcion(lat_mv *mv) {
  lat_objeto *ret = latO_crear(mv);
#if DEPURAR_MEM
  printf("latO_crear_funcion: %p\n", ret);
#endif
  ret->tipo = T_FUN;
  return ret; // We don't do anything here: all bytecode will be added
              // later
}

lat_objeto *latO_crear_cfuncion(lat_mv *mv) {
  lat_objeto *ret = latO_crear(mv);
  ret->tipo = T_CFUN;
  ret->marca = 0;
  return ret;
}

void latO_destruir(lat_mv *mv, lat_objeto *o) {
  if (o == NULL)
    return;
  if (constante_registrada(mv, o)) return;
  switch (o->tipo) {
  case T_CONTEXT:
    latH_limpiar(mv, getCtx(o));
    break;
  case T_LIST: {
    lista *list = latC_checar_lista(mv, o);
    if (list->longitud > 0) {
      LIST_FOREACH(list, primero, siguiente, cur) {

        lat_objeto *tmp = (lat_objeto *)cur->valor;
        if (tmp != NULL) {
          latO_destruir(mv, tmp);
        }
      }
    }
    latL_destruir(mv, list);
  } break;
  case T_DIC:
    latH_destruir(mv, latC_checar_dic(mv, o));
    break;
#if !HABILITAR_GC
  case T_LABEL:
#endif
  case T_STR: {
#if HABILITAR_GC
    lat_cadena *str = (lat_cadena *)getCadena(o);
    latM_liberar(mv, str);
#endif
    /* Sin GC, las cadenas internadas pertenecen a strt hasta cerrar la VM. */
  } break;
  case T_FUN: {
    lat_funcion *fun = getFun(o);
    if (!funcion_registrada(mv, fun)) {
      /* Compatibilidad con payloads externos no registrados. */
      latM_liberar(mv, fun->codigo);
      latM_liberar(mv, fun);
    }
  } break;
  case T_CFUN: {
    ;
  } break;
  case T_NUMERIC: {
    ;
  } break;
  case T_NULL:
  case T_BOOL:
    // Nunca colectar nulo y booleano.
    return;
  default:
    return;
  }
  latM_liberar(mv, o);
}

static lista *latL_clonar(lat_mv *mv, lista *list) {
  lista *ret = latL_crear(mv);

  LIST_FOREACH(list, primero, siguiente, cur) {
    lat_objeto *tmp = (lat_objeto *)cur->valor;
    latL_agregar(mv, ret, tmp);
  }
  return ret;
}

lat_objeto *latO_clonar(lat_mv *mv, lat_objeto *obj) {
  lat_objeto *ret = NULL;
  switch (obj->tipo) {
  case T_CONTEXT:
    ret = latO_crear(mv);
    ret->tipo = T_CONTEXT;
    ret->tam = sizeof(hash_map *);
    setCtx(ret, latH_clonar(mv, getCtx(obj)));
    break;
  case T_LIST:
    ret = latC_crear_lista(mv, latL_clonar(mv, latC_checar_lista(mv, obj)));
    break;
  case T_DIC:
    ret = latC_crear_dic(mv, latH_clonar(mv, latC_checar_dic(mv, obj)));
    break;
  case T_FUN:
    ret = latO_crear(mv);
    ret->tipo = obj->tipo;
    ret->nombre = obj->nombre;
    setFun(ret, getFun(obj));
    ret->nparams = obj->nparams;
    break;
  case T_CFUN:
    ret = latO_crear(mv);
    ret->tipo = obj->tipo;
    ret->nombre = obj->nombre;
    setCfun(ret, getCfun(obj));
    ret->nparams = obj->nparams;
    break;
  case T_STR: {
    ret = latC_crear_cadena(mv, latC_checar_cadena(mv, obj));
  } break;
  case T_NUMERIC:
    ret = latC_crear_numerico(mv, latC_checar_numerico(mv, obj));
    break;
  case T_LABEL: {
    ret = latC_crear_cadena(mv, latC_checar_cadena(mv, obj));
    ret->nombre = obj->nombre;
    ret->tipo = obj->tipo;
    ret->jump_label = obj->jump_label;
  } break;
  default:
    ret = latO_crear(mv);
    ret->tipo = obj->tipo;
    ret->val = obj->val;
    break;
  }
  ret->nref = obj->nref;
  ret->marca = obj->marca;
  ret->tam = obj->tam;
  ret->es_vararg = obj->es_vararg;
  ret->esconst = obj->esconst;
  return ret;
}

bool latO_es_igual(lat_mv *mv, lat_objeto *lhs, lat_objeto *rhs) {
  if (lhs->tipo != rhs->tipo) {
    return false;
  }
  if (lhs->tipo == T_NUMERIC) {
    return latC_checar_numerico(mv, lhs) == latC_checar_numerico(mv, rhs);
  }
  if (lhs->tipo == T_STR) {
    return !strcmp(latC_checar_cadena(mv, lhs), latC_checar_cadena(mv, rhs))
               ? true
               : false;
  }
  if (lhs->tipo == T_LIST) {
    return latL_comparar(mv, latC_checar_lista(mv, lhs),
                         latC_checar_lista(mv, rhs)) == 0
               ? true
               : false;
  }
  return false;
}

int latO_comparar(lat_mv *mv, lat_objeto *lhs, lat_objeto *rhs) {
  int res = 1;
  if (lhs->tipo != rhs->tipo || lhs->tipo == T_DIC || rhs->tipo == T_DIC) {
    char *buffer = latC_astring(mv, lhs);
    char *buffer2 = latC_astring(mv, rhs);
    res = strcmp(buffer, buffer2);
    free(buffer);
    free(buffer2);
    goto RESPUESTA;
  }
  if (lhs->tipo == T_BOOL) {
    res = latC_checar_logico(mv, lhs) - latC_checar_logico(mv, rhs);
    goto RESPUESTA;
  }
  if (lhs->tipo == T_NUMERIC) {
    res = latC_checar_numerico(mv, lhs) - latC_checar_numerico(mv, rhs);
    goto RESPUESTA;
  }
  if (lhs->tipo == T_STR) {
    res = strcmp(latC_checar_cadena(mv, lhs), latC_checar_cadena(mv, rhs));
    goto RESPUESTA;
  }
  if (lhs->tipo == T_LIST) {
    res = latL_comparar(mv, latC_checar_lista(mv, lhs),
                        latC_checar_lista(mv, rhs));
    goto RESPUESTA;
  }
RESPUESTA:
  if (res < 0) {
    return -1;
  }
  if (res > 0) {
    return 1;
  }
  return res;
}

/* Representacion de objetos: buffer propio de libc, nunca contabilizado por
   latM. No es un formato de persistencia (el orden hash no es estable). */
typedef struct {
  char *datos;
  size_t usados, capacidad;
  const void *ruta[64];
  int tipos[64];
  size_t profundidad;
  const char *error;
} lat_texto_objeto;

static bool texto_agregar(lat_texto_objeto *t, const char *s, size_t n) {
  if (n > (size_t)MAX_STR_LENGTH - 1 - t->usados) {
    t->error = "Representacion de objeto demasiado larga";
    return false;
  }
  size_t necesaria = t->usados + n + 1;
  if (necesaria > t->capacidad) {
    size_t nueva = t->capacidad ? t->capacidad : 128;
    while (nueva < necesaria)
      nueva = nueva > (size_t)MAX_STR_LENGTH / 2 ? MAX_STR_LENGTH : nueva * 2;
    char *p = realloc(t->datos, nueva);
    if (!p) { t->error = "Memoria agotada al representar objeto"; return false; }
    t->datos = p;
    t->capacidad = nueva;
  }
  if (n) memcpy(t->datos + t->usados, s, n);
  t->usados += n;
  t->datos[t->usados] = '\0';
  return true;
}

static bool texto_literal(lat_texto_objeto *t, const char *s) {
  return texto_agregar(t, s, strlen(s));
}

static bool texto_citado(lat_texto_objeto *t, const char *s) {
  if (!texto_literal(t, "\"")) return false;
  for (const unsigned char *p = (const unsigned char *)s; *p; ++p) {
    const char *escape = *p == '"' ? "\\\"" : *p == '\\' ? "\\\\" :
                         *p == '\n' ? "\\n" : *p == '\r' ? "\\r" :
                         *p == '\t' ? "\\t" : NULL;
    if (escape) { if (!texto_literal(t, escape)) return false; }
    else if (!texto_agregar(t, (const char *)p, 1)) return false;
  }
  return texto_literal(t, "\"");
}

static bool texto_objeto(lat_texto_objeto *t, const lat_objeto *o, bool citar) {
  if (!o || o->tipo == T_NULL) return texto_literal(t, "nulo");
  if (o->tipo == T_STR || o->tipo == T_LABEL) {
    const char *s = getstr(getCadena(o));
    return citar ? texto_citado(t, s) : texto_literal(t, s);
  }
  char numero[64];
  switch (o->tipo) {
  case T_BOOL: return texto_literal(t, getLogico(o) ? "verdadero" : "falso");
  case T_NUMERIC:
    snprintf(numero, sizeof numero, LAT_NUMERIC_FMT, getNumerico(o));
    return texto_literal(t, numero);
  case T_INTEGER: {
    double n = getEntero(o);
    if (!isfinite(n) || n < INT_MIN || n > INT_MAX) {
      t->error = "Entero fuera de rango al representar objeto"; return false;
    }
    snprintf(numero, sizeof numero, "%i", (int)n);
    return texto_literal(t, numero);
  }
  case T_CHAR:
    numero[0] = getCaracter(o); numero[1] = '\0';
    return texto_literal(t, numero);
  case T_CONTEXT: return texto_literal(t, "contexto");
  case T_FUN: return texto_literal(t, "fun");
  case T_CFUN: return texto_literal(t, "cfun");
  case T_CLASS: return texto_literal(t, "clase");
  case T_LIST:
  case T_DIC: break;
  default: return texto_literal(t, "");
  }
  const void *recurso = o->tipo == T_LIST ? (void *)getLista(o) : (void *)getDic(o);
  if (!recurso) { t->error = "Contenedor nulo al representar objeto"; return false; }
  if (t->profundidad == sizeof t->ruta / sizeof *t->ruta) {
    t->error = "Anidamiento excesivo al representar objeto"; return false;
  }
  for (size_t i = 0; i < t->profundidad; ++i)
    if (t->ruta[i] == recurso && t->tipos[i] == o->tipo) {
      t->error = "Referencia circular al representar objeto"; return false;
    }
  t->ruta[t->profundidad] = recurso;
  t->tipos[t->profundidad++] = o->tipo;
  bool primero = true;
  if (!texto_literal(t, o->tipo == T_LIST ? "[" : "{")) return false;
  if (o->tipo == T_LIST) {
    for (nodo_lista *cur = getLista(o)->primero; cur; cur = cur->siguiente) {
      if (!primero && !texto_literal(t, ", ")) return false;
      primero = false;
      if (!texto_objeto(t, cur->valor, true)) return false;
    }
  } else {
    const hash_map *m = getDic(o);
    for (size_t i = 0; i < m->capacity; ++i) {
      const lista *l = m->buckets[i];
      for (nodo_lista *cur = l ? l->primero : NULL; cur; cur = cur->siguiente) {
        const hash_val *hv = cur->valor;
        if (!hv) continue;
        if (!primero && !texto_literal(t, ", ")) return false;
        primero = false;
        if (!texto_citado(t, hv->llave) || !texto_literal(t, ": ") ||
            !texto_objeto(t, hv->valor, true)) return false;
      }
    }
  }
  t->profundidad--;
  return texto_literal(t, o->tipo == T_LIST ? "]" : "}");
}

static char *objeto_a_texto(lat_mv *mv, const lat_objeto *o) {
  lat_texto_objeto t = {0};
  if (!texto_objeto(&t, o, false)) {
    free(t.datos);
    latC_error(mv, "%s", t.error ? t.error : "Error al representar objeto");
    return NULL;
  }
  return t.datos;
}

char *latL_acadena(lat_mv *mv, lista *list) {
  lat_objeto o = {0};
  setLista(&o, list);
  return objeto_a_texto(mv, &o);
}

char *latH_acadena(lat_mv *mv, hash_map *m) {
  lat_objeto o = {0};
  setDic(&o, m);
  return objeto_a_texto(mv, &o);
}

void latL_modificar_elemento(lat_mv *mv, lista *list, void *data, int pos) {
  int i = 0;
  if (pos < 0 || pos >= latL_longitud(list)) {
    latC_error(mv, "Indice fuera de rango");
  }

  LIST_FOREACH(list, primero, siguiente, cur) {
    if (i == pos) {
      cur->valor = data;
    }
    i++;
  }
}

int latL_comparar(lat_mv *mv, lista *lhs, lista *rhs) {
  int res = 0;
  int len1 = latL_longitud(lhs);
  int len2 = latL_longitud(rhs);
  if (len1 < len2) {
    return -1;
  }
  if (len1 > len2) {
    return 1;
  }
  int i;
  for (i = 0; i < len1; i++) {
    lat_objeto *tmp1 = latL_obtener_elemento(mv, lhs, i);
    lat_objeto *tmp2 = latL_obtener_elemento(mv, rhs, i);
    res = latO_comparar(mv, tmp1, tmp2);
    if (res < 0) {
      return -1;
    }
    if (res > 0) {
      return 1;
    }
  }
  return res;
}

int latL_obtener_indice(lat_mv *mv, lista *list, void *data) {
  int i = 0;
  lat_objeto *find = (lat_objeto *)data;

  LIST_FOREACH(list, primero, siguiente, cur) {
    // if (memcmp(cur->valor, data, sizeof(cur->valor)) == 0)
    lat_objeto *tmp = (lat_objeto *)cur->valor;
    if (latO_es_igual(mv, find, tmp)) {
      return i;
    }
    i++;
  }
  return -1;
}

void latO_imprimir(lat_mv *mv, lat_objeto *o, bool fmt) {
  char *tmp = latC_astring(mv, o);
  /* Sin formato, el texto es literal: no volver a analizar las barras de
     datos externos. El modo formateado conserva su decodificador existente. */
  char *salida = fmt ? analizar_fmt(tmp, strlen(tmp)) : tmp;
  if (!salida) {
    free(tmp);
    latC_error(mv, "Memoria agotada al imprimir objeto");
    return;
  }
  printf("%s", salida);
  if (salida != tmp) free(salida);
  free(tmp);
}

void latS_resize(lat_mv *mv, int newsize) {
  lat_gcobjeto **newhash;
  stringtable *tb;
  int i;
  newhash = latM_asignar(mv, newsize * sizeof(lat_gcobjeto *));
#if DEPURAR_MEM
  printf("latS_resize.newhash: %p\n", newhash);
#endif
  tb = &mv->global->strt;
  for (i = 0; i < newsize; i++) {
    newhash[i] = NULL;
  }
  for (i = 0; i < tb->size; i++) {
    lat_gcobjeto *p = tb->hash[i];
    while (p) {
      lat_gcobjeto *next = p->gch.next;
      unsigned int h = (&p->cadena)->tsv.hash;
      int h1 = lmod(h, newsize);
      p->gch.next = newhash[h1];
      newhash[h1] = p;
      p = next;
    }
  }
  latM_liberar(mv, tb->hash);
  tb->size = newsize;
  tb->hash = newhash;
}

LATINO_API lat_objeto *latC_crear_logico(lat_mv *mv, bool val) {
  lat_objeto *ret = latO_crear(mv);
#if DEPURAR_MEM
  printf("latC_crear_logico: %p\n", ret);
#endif
  ret->tam += sizeof(bool);
  setLogico(ret, val);
  return ret;
}

LATINO_API lat_objeto *latC_crear_numerico(lat_mv *mv, double val) {
  lat_objeto *ret = latO_crear(mv);
#if DEPURAR_MEM
  printf("latC_crear_numerico: %p\n", ret);
#endif
  ret->tam += sizeof(double);
  setNumerico(ret, val);
  return ret;
}

LATINO_API lat_objeto *latC_crear_entero(lat_mv *mv, int val) {
  lat_objeto *ret = latO_crear(mv);
#if DEPURAR_MEM
  printf("latC_crear_entero: %p\n", ret);
#endif
  ret->tam += sizeof(int);
  setEntero(ret, val);
  return ret;
}

LATINO_API lat_objeto *latC_crear_caracter(lat_mv *mv, char val) {
  lat_objeto *ret = latO_crear(mv);
#if DEPURAR_MEM
  printf("latC_crear_caracter: %p\n", ret);
#endif
  ret->tam += sizeof(int);
  setCaracter(ret, val);
  return ret;
}

LATINO_API lat_objeto *latC_crear_cadena(lat_mv *mv, const char *p) {
  lat_objeto *ret = latO_crear(mv);
#if DEPURAR_MEM
  printf("latC_crear_cadena: %p\n", ret);
#endif
  ret->tam += strlen(p);
  setCadena(ret, latO_cadenaNueva(mv, p, strlen(p)));
  return ret;
}

LATINO_API lat_objeto *latC_crear_lista(lat_mv *mv, lista *l) {
  lat_objeto *ret = latO_crear(mv);
#if DEPURAR_MEM
  printf("latC_crear_lista: %p\n", ret);
#endif
  ret->tam += sizeof(lista);
  setLista(ret, l);
  return ret;
}

LATINO_API lat_objeto *latC_crear_dic(lat_mv *mv, hash_map *dic) {
  lat_objeto *ret = latO_crear(mv);
#if DEPURAR_MEM
  printf("latC_crear_dic: %p\n", ret);
#endif
  ret->tam += sizeof(hash_map);
  setDic(ret, dic);
  return ret;
}

LATINO_API lat_objeto *latC_crear_cdato(lat_mv *mv, void *ptr) {
  lat_objeto *ret = latO_crear(mv);
#if DEPURAR_MEM
  printf("latC_crear_cdato: %p\n", ret);
#endif
  setPtr(ret, ptr);
  return ret;
}

LATINO_API bool latC_checar_logico(lat_mv *mv, lat_objeto *o) {
  if (o->tipo == T_BOOL) {
    return getLogico(o);
  }
  latC_error(mv, "El parametro debe de ser un valor logico");
  return false;
}

LATINO_API double latC_checar_numerico(lat_mv *mv, lat_objeto *o) {
  if (o->tipo == T_NUMERIC || o->tipo == T_INTEGER) {
    return getNumerico(o);
  }
  latC_error(mv, "El parametro debe de ser un decimal");
  return 0;
}

LATINO_API int latC_checar_entero(lat_mv *mv, lat_objeto *o) {
  if (o->tipo == T_INTEGER) {
    return getEntero(o);
  }
  latC_error(mv, "El parametro debe de ser un entero");
  return 0;
}

LATINO_API char latC_checar_caracter(lat_mv *mv, lat_objeto *o) {
  if (o->tipo == T_CHAR) {
    return getCaracter(o);
  }
  latC_error(mv, "El parametro debe de ser un caracter");
  return 0;
}

LATINO_API char *latC_checar_cadena(lat_mv *mv, lat_objeto *o) {
  if (o->tipo == T_STR || o->tipo == T_LABEL) {
    return getstr(getCadena(o));
  }
  latC_error(mv, "El parametro debe de ser una cadena");
  return 0;
}

LATINO_API lista *latC_checar_lista(lat_mv *mv, lat_objeto *o) {
  if (o->tipo == T_LIST) {
    return getLista(o);
  }
  latC_error(mv, "El parametro debe de ser una lista");
  return NULL;
}

LATINO_API hash_map *latC_checar_dic(lat_mv *mv, lat_objeto *o) {
  if (o->tipo == T_DIC) {
    return getDic(o);
  }
  latC_error(mv, "El parametro debe de ser un diccionario");
  return NULL;
}

LATINO_API void *latC_checar_cptr(lat_mv *mv, lat_objeto *o) {
  if (o->tipo == T_CPTR) {
    return getPtr(o);
  }
  latC_error(mv, "El parametro debe de ser un dato de c (void *)");
  return NULL;
}

static bool texto_igual_ascii(const char *s, const char *palabra) {
  while (*s && *palabra) {
    unsigned char c = (unsigned char)*s++;
    if (c >= 'A' && c <= 'Z') c = (unsigned char)(c + ('a' - 'A'));
    if (c != (unsigned char)*palabra++) return false;
  }
  return !*s && !*palabra;
}

LATINO_API bool latC_abool(lat_mv *mv, lat_objeto *o) {
  switch (o->tipo) {
  case T_NULL:
    return false;
    break;
  case T_BOOL:
    return latC_checar_logico(mv, o);
    break;
  case T_NUMERIC:
    return latC_checar_numerico(mv, o) == 0 ? false : true;
    break;
  case T_STR: {
    const char *s = latC_checar_cadena(mv, o);
    return *s && strcmp(s, "0") && !texto_igual_ascii(s, "falso") &&
           !texto_igual_ascii(s, "false");
  }
  case T_LIST:
    return latL_longitud(latC_checar_lista(mv, o)) == 0 ? false : true;
  case T_DIC:
    return latH_longitud(latC_checar_dic(mv, o)) == 0 ? false : true;
  default:
    latC_error(mv, "Conversion de tipo de dato incompatible");
    break;
  }
  return false;
}

LATINO_API double latC_adouble(lat_mv *mv, lat_objeto *o) {
  switch (o->tipo) {
  case T_NULL:
    return 0;
    break;
  case T_BOOL:
    return latC_checar_logico(mv, o) == false ? 0 : 1;
    break;
  case T_NUMERIC:
    return latC_checar_numerico(mv, o);
    break;
  case T_STR: {
    char *ptr;
    double ret;
    ret = latC_strtod_c(latC_checar_cadena(mv, o), &ptr);
    if (!strcmp(ptr, "")) {
      return ret;
    } else {
      ret = (int)(latC_checar_cadena(mv, o)[0]);
      return ret;
    }
  } break;
  case T_LIST:
    return latL_longitud(latC_checar_lista(mv, o));
    break;
  case T_DIC:
    return latH_longitud(latC_checar_dic(mv, o));
    break;
  default:
    latC_error(mv, "Conversion de tipo de dato incompatible");
    break;
  }
  return 0;
}

// FIXME:
LATINO_API int latC_aint(lat_mv *mv, lat_objeto *o) {
  switch (o->tipo) {
  case T_NULL:
    return 0;
    break;
  case T_BOOL:
    return latC_checar_logico(mv, o) == false ? 0 : 1;
    break;
  case T_NUMERIC:
    return latC_checar_entero(mv, o);
    break;
  case T_CHAR:
    return latC_checar_caracter(mv, o);
    break;
  case T_STR: {
    char *ptr;
    double ret;
    ret = latC_strtod_c(latC_checar_cadena(mv, o), &ptr);
    if (!strcmp(ptr, "")) {
      return ret;
    } else {
      ret = (int)(latC_checar_cadena(mv, o)[0]);
      return ret;
    }
  } break;
  case T_LIST:
    return latL_longitud(latC_checar_lista(mv, o));
    break;
  case T_DIC:
    return latH_longitud(latC_checar_dic(mv, o));
    break;
  default:
    latC_error(mv, "Conversion de tipo de dato incompatible");
    break;
  }
  return 0;
}

// FIXME:
LATINO_API char latC_achar(lat_mv *mv, lat_objeto *o) {
  switch (o->tipo) {
  case T_NULL:
    return 0;
    break;
  case T_BOOL:
    return latC_checar_logico(mv, o) == false ? 0 : 1;
    break;
  case T_NUMERIC:
    return latC_checar_entero(mv, o);
    break;
  case T_INTEGER:
    return latC_checar_entero(mv, o);
    break;
  case T_CHAR:
    return latC_checar_caracter(mv, o);
    break;
  case T_STR: {
    char *ptr;
    double ret;
    ret = latC_strtod_c(latC_checar_cadena(mv, o), &ptr);
    if (!strcmp(ptr, "")) {
      return ret;
    } else {
      ret = (int)(latC_checar_cadena(mv, o)[0]);
      return ret;
    }
  } break;
  case T_LIST:
    return latL_longitud(latC_checar_lista(mv, o));
    break;
  case T_DIC:
    return latH_longitud(latC_checar_dic(mv, o));
    break;
  default:
    latC_error(mv, "Conversion de tipo de dato incompatible");
    break;
  }
  return 0;
}

/* El llamador libera siempre con free(), tambien para T_CHAR. */
LATINO_API char *latC_astring(lat_mv *mv, lat_objeto *o) {
  /* Conservar el contrato de cadenas escalares: copiar su longitud completa.
     El limite agregado corresponde a la representacion de contenedores. */
  if (o && (o->tipo == T_STR || o->tipo == T_LABEL)) {
    char *s = strdup(latC_checar_cadena(mv, o));
    if (!s) latC_error(mv, "Memoria agotada al representar objeto");
    return s;
  }
  return objeto_a_texto(mv, o);
}
