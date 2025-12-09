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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "latcompat.h"
#include "latdo.h"
#include "latgc.h"
#include "latlist.h"
#include "latmem.h"
#include "latmv.h"
#ifndef DEPURAR_MV
#define DEPURAR_MV 1
#endif
#include "latobj.h"
#include "latparse.h"

static const char *const bycode_nombre[] = {"NOP", /* 0 */
                                            "HALT",
                                            "UNARY_MINUS",
                                            "BINARY_ADD",
                                            "BINARY_SUB",
                                            "BINARY_MUL", /* 5 */
                                            "BINARY_DIV",
                                            "BINARY_MOD",
                                            "OP_GT",
                                            "OP_GE",
                                            "OP_LT", /*10*/
                                            "OP_LE",
                                            "OP_EQ",
                                            "OP_NEQ",
                                            "OP_AND",
                                            "OP_OR", /*15*/
                                            "OP_NOT",
                                            "OP_INC",
                                            "OP_DEC",
                                            "CONCAT",
                                            "LOAD_CONST", /*20*/
                                            "LOAD_NAME",
                                            "STORE_NAME",
                                            "JUMP_ABSOLUTE",
                                            "POP_JUMP_IF_FALSE",
                                            "POP_JUMP_IF_TRUE", /*25*/
                                            "PUSH_CTX",
                                            "POP_CTX",
                                            "CALL_FUNCTION",
                                            "RETURN_VALUE",
                                            "MAKE_FUNCTION", /*30*/
                                            "LOAD_ATTR",
                                            "BUILD_LIST",
                                            "STORE_SUBSCR",
                                            "BINARY_SUBSCR",
                                            "BUILD_MAP", /*35*/
                                            "STORE_MAP",
                                            "STORE_ATTR",
                                            "SET_GLOBAL",
                                            "OP_REGEX",
                                            "BINARY_POW", /*40*/
                                            "OP_VAR_ARGS",
                                            "OP_PUSH",
                                            "OP_POP",
                                            "ADJUST_STACK",
                                            "LOAD_VAR_ARGS", /*45*/
                                            "SET_LOCAL",
                                            "POP_JUMP_IF_NEGATIVE",
                                            "JUMP_LABEL",
                                            "STORE_LABEL",
                                            "MAKE_CLASS", /*50*/
                                            "NEW_INSTANCE",
                                            "LOAD_METHOD",
                                            "CALL_METHOD",
                                            "LOAD_SUPER",
                                            "STORE_PROPERTY", /*55*/
                                            "LOAD_PROPERTY"};

void str_concatenar(lat_mv *mv);

void latC_abrir_liblatino_baselib(lat_mv *mv);
void latC_abrir_liblatino_listlib(lat_mv *mv);
void latC_abrir_liblatino_strlib(lat_mv *mv);
void latC_abrir_liblatino_diclib(lat_mv *mv);
void latC_abrir_liblatino_paqlib(lat_mv *mv);
void latC_abrir_liblatino_filelib(lat_mv *mv);
void latC_abrir_liblatino_mathlib(lat_mv *mv);
void latC_abrir_liblatino_syslib(lat_mv *mv);
void latC_abrir_liblatino_devlib(lat_mv *mv);

/*
void latC_abrir_liblatino_uilib     (lat_mv *mv);
void latC_abrir_liblatino_gc(lat_mv *mv);
void latC_abrir_liblatino_gtklib(lat_mv *mv);
 */

const char *latMV_bytecode_nombre(int inst) { return bycode_nombre[inst]; }

static void igualdad(lat_mv *mv)
{
  lat_objeto *b = latC_desapilar(mv);
  lat_objeto *a = latC_desapilar(mv);
  lat_objeto *r = NULL;
  char *buffer = NULL;
  char *buffer2 = NULL;
  switch (a->tipo)
  {
  case T_BOOL:
  {
    r = latC_checar_logico(mv, a) == latC_abool(mv, b) ? latO_verdadero
                                                       : latO_falso;
    latC_apilar(mv, r);
    return;
  }
  break;
  case T_NUMERIC:
  {
    r = (latC_checar_numerico(mv, a) == latC_adouble(mv, b)) ? latO_verdadero
                                                             : latO_falso;
    latC_apilar(mv, r);
    return;
  }
  break;
  case T_STR:
  {
    buffer = latC_astring(mv, b);
    r = strcmp(latC_checar_cadena(mv, a), buffer) == 0 ? latO_verdadero
                                                       : latO_falso;
    latC_apilar(mv, r);
    latM_liberar(mv, buffer);
    return;
  }
  break;
  default:
  {
    buffer = latC_astring(mv, a);
    buffer2 = latC_astring(mv, b);
    r = !strcmp(buffer, buffer2) ? latO_verdadero : latO_falso;
    latC_apilar(mv, r);
    latM_liberar(mv, buffer);
    latM_liberar(mv, buffer2);
    return;
  }
  }
}

void str_regex(lat_mv *mv);

static void diferente(lat_mv *mv)
{
  lat_objeto *b = latC_desapilar(mv);
  lat_objeto *a = latC_desapilar(mv);
  lat_objeto *r = NULL;
  char *buffer = NULL;
  char *buffer2 = NULL;
  switch (a->tipo)
  {
  case T_BOOL:
  {
    r = latC_checar_logico(mv, a) != latC_abool(mv, b) ? latO_verdadero
                                                       : latO_falso;
    latC_apilar(mv, r);
    return;
  }
  break;
  case T_NUMERIC:
  {
    r = (latC_checar_numerico(mv, a) != latC_adouble(mv, b)) ? latO_verdadero
                                                             : latO_falso;
    latC_apilar(mv, r);
    return;
  }
  break;
  case T_STR:
  {
    buffer = latC_astring(mv, b);
    r = strcmp(latC_checar_cadena(mv, a), buffer) ? latO_verdadero : latO_falso;
    latC_apilar(mv, r);
    latM_liberar(mv, buffer);
    return;
  }
  break;
  default:
  {
    buffer = latC_astring(mv, a);
    buffer2 = latC_astring(mv, b);
    r = strcmp(buffer, buffer2) ? latO_verdadero : latO_falso;
    latC_apilar(mv, r);
    latM_liberar(mv, buffer);
    latM_liberar(mv, buffer2);
    return;
  }
  }
}

static void menor_que(lat_mv *mv)
{
  lat_objeto *b = latC_desapilar(mv);
  lat_objeto *a = latC_desapilar(mv);
  lat_objeto *r = NULL;
  if (a->tipo == T_NUMERIC || b->tipo == T_NUMERIC)
  {
    r = (latC_adouble(mv, a) < latC_adouble(mv, b)) ? latO_verdadero
                                                    : latO_falso;
    latC_apilar(mv, r);
    return;
  }
  r = latO_comparar(mv, a, b) < 0 ? latO_verdadero : latO_falso;
  latC_apilar(mv, r);
  return;
}

static void menor_igual(lat_mv *mv)
{
  lat_objeto *b = latC_desapilar(mv);
  lat_objeto *a = latC_desapilar(mv);
  lat_objeto *r = NULL;
  if (a->tipo == T_NUMERIC || b->tipo == T_NUMERIC)
  {
    r = (getNumerico(a) <= getNumerico(b)) ? latO_verdadero : latO_falso;
    latC_apilar(mv, r);
    return;
  }
  r = latO_comparar(mv, a, b) <= 0 ? latO_verdadero : latO_falso;
  latC_apilar(mv, r);
  return;
}

static void mayor_que(lat_mv *mv)
{
  lat_objeto *b = latC_desapilar(mv);
  lat_objeto *a = latC_desapilar(mv);
  lat_objeto *r = NULL;
  if (a->tipo == T_NUMERIC || b->tipo == T_NUMERIC)
  {
    r = (latC_adouble(mv, a) > latC_adouble(mv, b)) ? latO_verdadero
                                                    : latO_falso;
    latC_apilar(mv, r);
    return;
  }
  r = latO_comparar(mv, a, b) > 0 ? latO_verdadero : latO_falso;
  latC_apilar(mv, r);
  return;
}

static void mayor_igual(lat_mv *mv)
{
  lat_objeto *b = latC_desapilar(mv);
  lat_objeto *a = latC_desapilar(mv);
  lat_objeto *r = NULL;
  if (a->tipo == T_NUMERIC || b->tipo == T_NUMERIC)
  {
    r = (latC_adouble(mv, a) >= latC_adouble(mv, b)) ? latO_verdadero
                                                     : latO_falso;
    latC_apilar(mv, r);
    return;
  }
  r = latO_comparar(mv, a, b) >= 0 ? latO_verdadero : latO_falso;
  latC_apilar(mv, r);
  return;
}

static void y_logico(lat_mv *mv)
{
  lat_objeto *b = latC_desapilar(mv);
  lat_objeto *a = latC_desapilar(mv);
  lat_objeto *r = NULL;
  if (latC_abool(mv, a))
  {
    r = b;
  }
  else
  {
    r = a;
  }
  latC_apilar(mv, latO_clonar(mv, r));
}

static void o_logico(lat_mv *mv)
{
  lat_objeto *b = latC_desapilar(mv);
  lat_objeto *a = latC_desapilar(mv);
  lat_objeto *r = NULL;
  if (latC_abool(mv, a))
  {
    r = a;
  }
  else
  {
    r = b;
  }
  latC_apilar(mv, latO_clonar(mv, r));
}

static void no_logico(lat_mv *mv)
{
  lat_objeto *o = latC_desapilar(mv);
  lat_objeto *r = (latC_abool(mv, o) == false) ? latO_verdadero : latO_falso;
  latC_apilar(mv, r);
}

static lat_objeto *obtener_contexto(lat_mv *mv) { return mv->contexto_actual; }

lat_objeto *obtener_contexto_global(lat_mv *mv) { return mv->contexto[0]; }

static void apilar_contexto(lat_mv *mv, lat_objeto *ctx)
{
  if (mv->ptrctx >= MAX_STACK_CONTEXT_SIZE)
  {
    printf(LAT_ERROR_FMT, mv->nombre_archivo, mv->nlin, mv->ncol,
           "Desborde de la pila de contextos");
    exit(EXIT_FAILURE);
  }
  mv->contexto[mv->ptrctx + 1] = latO_clonar(mv, mv->contexto[mv->ptrctx]);
  mv->ptrctx++;
  mv->contexto_actual = mv->contexto[mv->ptrctx];
}

static void desapilar_contexto(lat_mv *mv)
{
  if (mv->ptrctx == 0)
  {
    printf(LAT_ERROR_FMT, mv->nombre_archivo, mv->nlin, mv->ncol,
           "Pila de contextos vacia");
    exit(EXIT_FAILURE);
  }
  latO_destruir(mv, mv->contexto[mv->ptrctx--]);
  mv->contexto_actual = mv->contexto[mv->ptrctx];
}

/* FNV-1a hash for cache keys */
#define FNV_OFFSET_BASIS 2166136261u
#define FNV_PRIME 16777619u

static unsigned int cache_hash(const char *str)
{
  unsigned int h = FNV_OFFSET_BASIS;
  while (*str)
  {
    h ^= (unsigned char)*str++;
    h *= FNV_PRIME;
  }
  return h;
}

/* Lookup variable in cache */
static lat_objeto *latMV_cache_lookup(lat_mv *mv, const char *name,
                                      int ctx_level)
{
  unsigned int h = cache_hash(name);
  int i;
  for (i = 0; i < VAR_CACHE_SIZE; i++)
  {
    var_cache_entry *entry = &mv->var_cache[i];
    if (entry->name != NULL && entry->hash == h &&
        entry->ctx_level == ctx_level && strcmp(entry->name, name) == 0)
    {
      return entry->value;
    }
  }
  return NULL;
}

/* Insert variable into cache (circular buffer) */
static void latMV_cache_insert(lat_mv *mv, const char *name, lat_objeto *value,
                               int ctx_level)
{
  unsigned int h = cache_hash(name);
  var_cache_entry *entry = &mv->var_cache[mv->var_cache_next];

  entry->name = name; /* Points to interned string, no need to copy */
  entry->value = value;
  entry->ctx_level = ctx_level;
  entry->hash = h;

  mv->var_cache_next = (mv->var_cache_next + 1) % VAR_CACHE_SIZE;
}

/* Invalidate cache (called when variables are modified) */
static void latMV_cache_invalidate(lat_mv *mv)
{
  int i;
  for (i = 0; i < VAR_CACHE_SIZE; i++)
  {
    mv->var_cache[i].name = NULL;
    mv->var_cache[i].value = NULL;
  }
}

LATINO_API void latC_abrir_liblatino(lat_mv *mv, const char *nombre_lib,
                                     const lat_CReg *funs)
{
  lat_objeto *ctx = obtener_contexto_global(mv);
  if (!strcmp(nombre_lib, ""))
  {
    // Alcance global o libreria base
    for (; funs->nombre; funs++)
    {
      lat_objeto *cfun = latC_crear_cfuncion(mv, funs->cfun);
      cfun->nombre = funs->nombre;
      cfun->nparams = funs->nparams;
      latO_asignar_ctx(mv, ctx, funs->nombre, cfun);
    }
  }
  else
  {
    lat_objeto *mod = latO_obtener_contexto(mv, ctx, nombre_lib);
    if (mod == NULL)
    {
      mod = latC_crear_dic(mv, latH_crear(mv));
      mod->marca = 0;
    }
    for (; funs->nombre; funs++)
    {
      lat_objeto *cfun = latC_crear_cfuncion(mv, funs->cfun);
      cfun->nombre = funs->nombre;
      cfun->nparams = funs->nparams;
      latH_asignar(mv, latC_checar_dic(mv, mod), funs->nombre, cfun);
    }
    latO_asignar_ctx(mv, ctx, nombre_lib, mod);
  }
}

LATINO_API lat_mv *latC_crear_mv()
{
  lat_mv *mv = (lat_mv *)malloc(sizeof(lat_mv));
  mv->memoria_usada = 0;
#if DEPURAR_MEM
  printf("------------------------------------------------------------\n");
  printf("inicio latC_crear_mv.mv->memoria_usada: %zu\n", mv->memoria_usada);
  printf("------------------------------------------------------------\n");
#endif
  mv->global = (lat_global *)latM_asignar(mv, sizeof(lat_global));
#if DEPURAR_MEM
  printf("latC_crear_mv.mv->global: %p\n", mv->global);
#endif
#if HABILITAR_GC
  mv->global->gc_objetos = latC_crear_lista(mv, latL_crear(mv));
  mv->global->gc_objetos->marca = 0;
#endif
  mv->global->argv = latC_crear_lista(mv, latL_crear(mv));
  mv->global->argv->marca = 0;
  mv->global->strt.size = 0;
  mv->global->strt.nuse = 0;
  mv->global->strt.hash = NULL;
  latS_resize(mv, MIN_STRTABLE_SIZE);
  mv->pila = calloc(MAX_STACK_SIZE, sizeof(lat_objeto));
  mv->ptrpila = 0;
  mv->tope = mv->pila;
  mv->ptrprevio = 1;
  mv->prev_args = 0;
  mv->numejec = 0;
  memset(mv->contexto, 0, 256);
  mv->contexto[0] = latO_contexto_crear(mv);
  mv->label_ctx = latO_contexto_crear(mv);
  mv->contexto[0]->marca = 0;
  mv->ptrctx = 0;
  mv->contexto_actual = mv->contexto[mv->ptrctx];
  mv->error = NULL;
  mv->global->menu = false;
  mv->enBucle = 0;
  mv->enClase = 0;
  /* inicialización de etiquetas break manejada dinámicamente en latdo.c */

  /* Initialize variable lookup cache */
  mv->var_cache_next = 0;
  latMV_cache_invalidate(mv);

  // cargar librerias de latino
  latC_abrir_liblatino_baselib(mv);
  latC_abrir_liblatino_strlib(mv);
  latC_abrir_liblatino_listlib(mv);
  latC_abrir_liblatino_diclib(mv);
  latC_abrir_liblatino_paqlib(mv);
  latC_abrir_liblatino_filelib(mv);
  latC_abrir_liblatino_mathlib(mv);
  latC_abrir_liblatino_syslib(mv);
  latC_abrir_liblatino_devlib(mv);

#if DEPURAR_MEM
  printf("------------------------------------------------------------\n");
  printf("fin latC_crear_mv.mv->memoria_usada: %zu\n", mv->memoria_usada);
  printf("------------------------------------------------------------\n");
#endif
  return mv;
}

LATINO_API void latC_destruir_mv(lat_mv *mv)
{
#if DEPURAR_MEM
  printf("------------------------------------------------------------\n");
  printf("inicio latC_destruir_mv.mv->memoria_usada: %zu\n", mv->memoria_usada);
  printf("------------------------------------------------------------\n");
#endif
  latO_destruir(mv, mv->global->argv);
#if HABILITAR_GC
  latO_destruir(mv, mv->global->gc_objetos);
#endif
  latO_destruir(mv, mv->contexto[0]);
  latM_liberar(mv, mv->global->strt.hash);
  latM_liberar(mv, mv->global);
#if DEPURAR_MEM
  printf("------------------------------------------------------------\n");
  printf("fin latC_destruir_mv.mv->memoria_usada: %zu\n", mv->memoria_usada);
  printf("------------------------------------------------------------\n");
#endif
  free(mv->pila);
  free(mv);
}

void imprimir_pila(lat_mv *mv)
{
  if (mv->ptrpila >= 0)
  {
    int n = (mv->ptrpila);
    printf("\tPILA(%i) = [", n);
    for (int i = 0; i < n; i++)
    {
      lat_objeto o = mv->pila[i];
      latO_imprimir(mv, &o, false);
      printf(", ");
    }
    printf("]");
  }
  printf("\n");
}

void checar_pila(lat_mv *mv)
{
  if (mv->ptrpila >= MAX_STACK_SIZE)
  {
    printf(LAT_ERROR_FMT, mv->nombre_archivo, mv->nlin, mv->ncol,
           "Desborde de la pila");
    exit(EXIT_FAILURE);
  }
  if (mv->ptrpila <= 0)
  {
    printf(LAT_ERROR_FMT, mv->nombre_archivo, mv->nlin, mv->ncol, "Pila vacia");
    exit(EXIT_FAILURE);
  }
}

void inc_pila(lat_mv *mv)
{
  mv->tope++;
  mv->ptrpila++;
}

void dec_pila(lat_mv *mv)
{
  mv->tope--;
  mv->ptrpila--;
}

LATINO_API void latC_apilar(lat_mv *mv, lat_objeto *o)
{
  switch (o->tipo)
  {
  case T_NULL:
    setNulo(mv->tope, o);
    break;
  case T_BOOL:
    setLogico(mv->tope, getLogico(o));
    break;
  case T_NUMERIC:
    setNumerico(mv->tope, getNumerico(o));
    break;
  case T_STR:
    setCadena(mv->tope, getCadena(o));
    break;
  case T_LIST:
    setLista(mv->tope, getLista(o));
    break;
  case T_DIC:
    setDic(mv->tope, getDic(o));
    break;
  case T_FUN:
    setobj(mv->tope, o);
    break;
  case T_CFUN:
    setobj(mv->tope, o);
    break;
  case T_CPTR:
    setobj(mv->tope, o);
    break;
  case T_CLASS:
  case T_INSTANCE:
    setobj(mv->tope, o);
    break;
  default:;
  }
  inc_pila(mv);
  checar_pila(mv);
}

LATINO_API lat_objeto *latC_desapilar(lat_mv *mv)
{
  checar_pila(mv);
  dec_pila(mv);
  return mv->tope;
}

LATINO_API lat_objeto *latC_tope(lat_mv *mv)
{
  lat_objeto *p = (lat_objeto *)&mv->pila[mv->ptrpila - 1];
  return p;
}

LATINO_API void latC_apilar_double(lat_mv *mv, double num)
{
  setNumerico(mv->tope, num);
  inc_pila(mv);
  checar_pila(mv);
}

LATINO_API void latC_apilar_int(lat_mv *mv, int i)
{
  setEntero(mv->tope, i);
  inc_pila(mv);
  checar_pila(mv);
}

LATINO_API void latC_apilar_char(lat_mv *mv, char c)
{
  setCaracter(mv->tope, c);
  inc_pila(mv);
  checar_pila(mv);
}

LATINO_API void latC_apilar_string(lat_mv *mv, const char *str)
{
  setCadena(mv->tope, getCadena(latC_crear_cadena(mv, str)));
  inc_pila(mv);
  checar_pila(mv);
}

LATINO_API lat_objeto *latC_crear_funcion(lat_mv *mv, lat_bytecode *inslist,
                                          int ninst)
{
  lat_objeto *ret = latO_crear_funcion(mv);
  lat_funcion *fval = (lat_funcion *)latM_asignar(mv, sizeof(lat_funcion));
#if DEPURAR_MEM
  printf("latC_crear_funcion.fval: %p\n", fval);
#endif
  fval->codigo = inslist;
  setFun(ret, fval);
  ret->ninst = ninst;
  return ret;
}

LATINO_API lat_objeto *latC_crear_cfuncion(lat_mv *mv, lat_CFuncion func)
{
  lat_objeto *ret = latO_crear_cfuncion(mv);
  setCfun(ret, func);
  return ret;
}

lat_bytecode latMV_bytecode_crear(int i, int a, int b, void *meta,
                                  long int nlin, long int ncol,
                                  char *nombre_archivo)
{
  lat_bytecode ret;
  ret.ins = i;
  ret.a = a;
  ret.b = b;
  ret.meta = meta;
  ret.nlin = nlin;
  ret.ncol = ncol;
  ret.nombre_archivo = nombre_archivo;
  return ret;
}

lat_objeto *latMV_get_symbol(lat_mv *mv, lat_objeto *name)
{
  const char *str_name = latC_checar_cadena(mv, name);
  int ctx_level = mv->ptrctx;
  lat_objeto *val;

  /* Try cache first for current context */
  val = latMV_cache_lookup(mv, str_name, ctx_level);
  if (val != NULL)
  {
    return val;
  }

  /* Cache miss - lookup in current context */
  lat_objeto *ctx = obtener_contexto(mv);
  val = (lat_objeto *)latO_obtener_contexto(mv, ctx, str_name);
  if (val != NULL)
  {
    /* Cache the result */
    latMV_cache_insert(mv, str_name, val, ctx_level);
    return val;
  }

  /* Try global context */
  ctx = obtener_contexto_global(mv);
  val = (lat_objeto *)latO_obtener_contexto(mv, ctx, str_name);
  if (val != NULL)
  {
    /* Cache the result with global context level */
    latMV_cache_insert(mv, str_name, val, 0);
    return val;
  }

  /* Variable not found */
  latC_error(mv, "Variable '%s' indefinida", str_name);
  return NULL; /* Never reached due to error */
}

lat_objeto *latMV_get_label(lat_mv *mv, lat_objeto *name)
{
  lat_objeto *ctx = mv->label_ctx;
  lat_objeto *val = (lat_objeto *)latO_obtener_contexto(
      mv, ctx, latC_checar_cadena(mv, name));
  if (val == NULL)
  {
    latC_error(mv, "Etiqueta '%s' indefinida", latC_checar_cadena(mv, name));
  }
  return val;
}

void latMV_set_symbol(lat_mv *mv, lat_objeto *name, lat_objeto *val)
{
  char *str_name = latC_checar_cadena(mv, name);
  lat_objeto *ctx = obtener_contexto(mv);
  lat_objeto *tmp = latO_obtener_contexto(mv, ctx, str_name);

  if (name->esconst)
  {
    if (tmp != NULL)
    {
      latC_error(mv, "Intento de reasignar valor a constante '%s'", str_name);
    }
  }

  lat_objeto *global_ctx = obtener_contexto_global(mv);
  lat_objeto *oldVal = latO_obtener_contexto(mv, global_ctx, str_name);
  if (oldVal != NULL)
  {
    if (oldVal->tipo == T_CFUN || oldVal->tipo == T_FUN)
    {
      latC_error(mv, "Intento de reasignar valor a la funcion '%s'", str_name);
    }
    ctx = global_ctx;
  }

  latO_asignar_ctx(mv, ctx, str_name, val);

  /* Invalidate cache when variables are modified */
  latMV_cache_invalidate(mv);
}

void latMV_set_label(lat_mv *mv, lat_objeto *name, lat_objeto *val)
{
  char *str_name = latC_checar_cadena(mv, name);
  lat_objeto *ctx = mv->label_ctx;
  latO_asignar_ctx(mv, ctx, str_name, val);
}

static void latMV_call_function(lat_mv *mv, lat_objeto *func, lat_bytecode cur,
                                lat_bytecode next)
{
  lat_objeto *fun = latM_asignar(mv, sizeof(lat_objeto));
#if DEPURAR_MEM
  printf("latMV_call_function.fun: %p\n", fun);
#endif
  setobj2obj(fun, latC_desapilar(mv));
  int num_args = cur.a;
  // Si es una clase, crear instancia y ejecutar su constructor
  if (fun->es_clase || fun->tipo == T_CLASS)
  {
    // Desapilar argumentos desde la pila (orden correcto)
    lat_objeto **args = NULL;
    if (num_args > 0)
    {
      args = (lat_objeto **)latM_asignar(mv, sizeof(lat_objeto *) * num_args);
      for (int i = num_args - 1; i >= 0; i--)
      {
        args[i] = latC_desapilar(mv);
      }
    }

    // Usar el objeto copiado como clase
    lat_objeto *clase_obj = fun;
    if (!clase_obj || clase_obj->tipo != T_CLASS)
    {
      latC_error(mv, "No es una clase (tipo=%d)", clase_obj ? clase_obj->tipo : -1);
    }

    lat_class *clase = (lat_class *)clase_obj->val.cpointer;

    // Crear instancia
    lat_objeto *inst = latC_crear_instancia(mv, clase);

    if (clase->constructor)
    {
      // Apilar SOLO los argumentos del usuario (sin inyectar la instancia en la pila)
      for (int i = 0; i < num_args; i++)
      {
        latC_apilar(mv, args[i]);
      }
      // Gestionar contexto y prev_args alrededor de la llamada al ctor
      apilar_contexto(mv, NULL);
      // Inyectar 'mi' en el contexto del constructor para soportar super() y mi.prop
      latO_asignar_ctx(mv, obtener_contexto(mv), "mi", inst);
      mv->ptrprevio = (mv->ptrpila);
      int retc = latC_llamar_funcion(mv, clase->constructor);
      mv->prev_args = retc;
    /* ctor ejecutado (sin logs) */
      desapilar_contexto(mv);
    }
    else
    {
      fprintf(stderr, "  [CALL] Clase sin constructor\n");
    }

    // Liberar arreglo de args
    if (args)
    {
      latM_liberar(mv, args);
    }

    // Resultado de la llamada: la instancia creada
    latC_apilar(mv, inst);
    // Indicar que esta llamada deja 1 valor en la pila (la instancia)
    mv->prev_args = 1;

    // Liberar copia local y salir
    latM_liberar(mv, fun);
    return;
  }

  // Validar función normal (no clase)
  if (!(fun->tipo == T_CFUN || fun->tipo == T_FUN))
  {
    latC_error(mv, "El objeto no es una funcion");
  }

  int nparams = fun->nparams;
  while (mv->ptrpila < nparams)
  {
    latC_apilar(mv, latO_nulo);
    num_args++;
  }
  if (nparams == FUNCION_VAR_ARGS)
  {
    // T_CFUN y varargs
    fun->es_vararg = true;
    lat_objeto *ctx = obtener_contexto(mv);
    lat_objeto *varargs =
        (lat_objeto *)latO_obtener_contexto(mv, ctx, "varargs");
    if (varargs)
    {
      lista *lst = latC_checar_lista(mv, varargs);
      if (latL_longitud(lst))
      {
        num_args = (num_args - 1) + latL_longitud(lst);
      }
    }
    latC_apilar_double(mv, (double)num_args);
  }
  else if (num_args != nparams && !fun->es_vararg)
  {
    if (num_args < nparams)
    {
      while (num_args < nparams)
      {
        latC_apilar(mv, latO_nulo);
        num_args++;
      }
    }
    else
    {
      // T_FUN y no varargs
      latC_error(mv,
                 "Numero invalido de argumentos en "
                 "funcion '%s'",
                 fun->nombre);
    }
  }
  else
  {
    if (!mv->enClase)
    {
      if (mv->prev_args > 1)
      {
        while (mv->ptrpila > (mv->ptrprevio + nparams))
        {
          latC_desapilar(mv);
        }
      }
    }
  }
  mv->numejec++;
  if (mv->numejec >= MAX_CALL_FUNCTION)
  {
    latC_error(mv, "Llamadas a funciones recursivas excedido en '%s'",
               fun->nombre);
  }
  bool apilar = next.ins == STORE_NAME || !strcmp(fun->nombre, "incluir") ||
                (fun->nombre != NULL && func->nombre != NULL &&
                 strcmp(func->nombre, fun->nombre));
  if (apilar)
  {
    apilar_contexto(mv, NULL);
    mv->ptrprevio = (mv->ptrpila);
  }
  else
  {
    mv->ptrprevio = 1; // restore stack
  }
  mv->prev_args = latMV_funcion_correr(mv, fun);
  mv->numejec--;
  if (fun->tipo != T_CFUN && mv->prev_args == 0 && next.ins == ADJUST_STACK)
  {
    latC_error(mv,
               "La funcion '%s' no "
               "retorna ningun valor\n",
               fun->nombre);
  }
  if (fun->tipo == T_FUN && fun->es_vararg)
  {
    lat_objeto *ctx = obtener_contexto(mv);
    latO_asignar_ctx(mv, ctx, "varargs", latC_crear_lista(mv, latL_crear(mv)));
  }
  latM_liberar(mv, fun);
  if (apilar)
  {
    desapilar_contexto(mv);
#if HABILITAR_GC
    gc_checar(mv);
#endif
  }
}

static void latMV_build_list(lat_mv *mv, lat_bytecode cur)
{
  int num_elem = 0;
  if (cur.b)
  {
    lat_objeto *ctx = obtener_contexto(mv);
    lat_objeto *varargs =
        (lat_objeto *)latO_obtener_contexto(mv, ctx, "varargs");
    if (!varargs)
    {
      latC_error(mv, "varargs (...) no existe en este contexto");
    }
    lista *lst = latC_checar_lista(mv, varargs);
    num_elem = (cur.a - cur.b) + (cur.b * latL_longitud(lst));
  }
  else
  {
    num_elem = cur.a;
  }
  if (mv->prev_args)
  {
    num_elem += mv->prev_args - 1;
  }
#if DEPURAR_MV
  printf("%i\t%i\t", cur.a, cur.b);
#endif
  int i;
  lat_objeto *obj = latC_crear_lista(mv, latL_crear(mv));
  for (i = 0; i < num_elem; i++)
  {
    lat_objeto *tmp = latO_clonar(mv, latC_desapilar(mv));
    latL_insertar_inicio(mv, latC_checar_lista(mv, obj), tmp);
  }
  latC_apilar(mv, obj);
}

static void latMV_load_attr(lat_mv *mv, lat_bytecode cur, lat_bytecode next)
{
  lat_objeto *obj = latC_desapilar(mv);
  lat_objeto *attr = (lat_objeto *)cur.meta;
  lat_objeto *val = NULL;
#if DEPURAR_MV
  latO_imprimir(mv, attr, false);
  printf("\t");
#endif
  /* Soporte para instancias: leer propiedad desde lat_instancia */
  if (obj && obj->tipo == T_INSTANCE)
  {
    lat_instancia *instancia = (lat_instancia *)obj->val.cpointer;
    const char *pname = latC_checar_cadena(mv, attr);
    lat_objeto *valor = latC_obtener_propiedad(instancia, pname);
    if (!valor)
    {
      /* Si no existe propiedad, intentar como método de la clase */
      lat_objeto *met = latC_obtener_metodo(instancia->clase, pname);
      if (met)
      {
        /* Inyectar 'mi' en el contexto para la llamada posterior */
        latO_asignar_ctx(mv, obtener_contexto(mv), "mi", obj);
        latC_apilar(mv, met);
        return;
      }
      valor = &latO_nulo_;
    }
    latC_apilar(mv, valor);
    return;
  }
  if (obj->tipo == T_DIC)
  {
    val = (lat_objeto *)latH_obtener(latC_checar_dic(mv, obj),
                                     latC_checar_cadena(mv, attr));
    if (val != NULL)
    {
      latC_apilar(mv, val);
      return;
    }
    else
    {
      if (next.ins == CALL_FUNCTION)
      {
        latC_error(mv, "No se encontro la fun '%s'",
                   latC_checar_cadena(mv, attr));
      }
      else
      {
        val = latC_crear_cadena(mv, "");
        latC_apilar(mv, val);
        return;
      }
    }
  }
  lat_objeto *top = latC_tope(mv);
  if (top && (next.ins == BINARY_SUBSCR))
  {
    if (top->tipo == T_STR)
    {
      if (obj->tipo != T_DIC)
      {
        obj = latC_crear_dic(mv, latH_crear(mv));
        latH_asignar(mv, latC_checar_dic(mv, obj), latC_checar_cadena(mv, attr),
                     val);
        latC_apilar(mv, obj);
        return;
      }
    }
    if (top->tipo == T_NUMERIC)
    {
      if (obj->tipo != T_LIST)
      {
        obj = latC_crear_lista(mv, latL_crear(mv));
        latC_apilar(mv, obj);
        return;
      }
    }
  }
  val = latC_crear_cadena(mv, "");
  latC_apilar(mv, val);
}

static void latMV_store_subscr(lat_mv *mv)
{
  lat_objeto *pos = latC_desapilar(mv);
  lat_objeto *obj = latC_desapilar(mv);
  lat_objeto *exp = latC_desapilar(mv);
  if (!latO_comparar(mv, obj, exp))
  {
    latC_error(mv, "Referencia circular detectada");
  }
  if (obj->tipo == T_DIC)
  {
    latH_asignar(mv, latC_checar_dic(mv, obj), latC_astring(mv, pos), exp);
    return;
  }
  int ipos = latC_checar_numerico(mv, pos);
  if (obj->tipo == T_LIST)
  {
    lista *ll = latC_checar_lista(mv, obj);
    int len = latL_longitud(ll);
    if (ipos < 0)
    {
      ipos = ipos + len;
      getNumerico(pos) = ipos;
    }
    if (ipos == len)
    {
      latL_agregar(mv, ll, latO_clonar(mv, exp));
      return;
    }
    latL_modificar_elemento(mv, latC_checar_lista(mv, obj),
                            latO_clonar(mv, exp), ipos);
  }
  if (obj->tipo == T_STR)
  {
    char *sobj = latC_checar_cadena(mv, obj);
    if (ipos < 0 || ipos >= strlen(sobj))
    {
      latC_error(mv, "Indice fuera de rango");
    }
    char *sexp = latC_checar_cadena(mv, exp);
    if (strlen(sexp) == 0)
    {
      sexp = " ";
    }
    sobj[ipos] = sexp[0];
    setCadena(obj, sobj);
  }
}

static void binary_subscr(lat_mv *mv)
{
  lat_objeto *obj = latC_desapilar(mv);
  lat_objeto *pos = latC_desapilar(mv);
  lat_objeto *o = NULL;
  if (obj->tipo == T_DIC)
  {
    o = latH_obtener(latC_checar_dic(mv, obj), latC_checar_cadena(mv, pos));
    if (o == NULL)
    {
      o = latC_crear_cadena(mv, "");
    }
    latC_apilar(mv, o);
    return;
  }
  int ipos = 0;
  if (pos->tipo == T_NUMERIC)
  {
    ipos = latC_checar_numerico(mv, pos);
  }
  else
  {
    o = latC_crear_cadena(mv, "");
    latC_apilar(mv, o);
    return;
  }
  if (obj->tipo == T_LIST)
  {
    lista *ll = latC_checar_lista(mv, obj);
    int len = latL_longitud(ll);
    if (ipos < 0)
    {
      ipos = ipos + len;
      getNumerico(pos) = ipos;
    }
    if (ipos < 0 || ipos >= len)
    {
      o = latC_crear_cadena(mv, "");
      latC_apilar(mv, o);
      return;
    }
    o = latL_obtener_elemento(mv, latC_checar_lista(mv, obj),
                              latC_checar_numerico(mv, pos));
  }
  if (obj->tipo == T_NUMERIC)
  {
    char *sobj = latC_astring(mv, obj);
    if (ipos < 0 || ipos >= strlen(sobj))
    {
      o = latO_nulo;
      latC_apilar(mv, o);
      return;
    }
    char c[2] = {sobj[ipos], '\0'};
    o = latC_crear_cadena(mv, c);
  }
  if (obj->tipo == T_STR)
  {
    char *sobj = latC_checar_cadena(mv, obj);
    if (ipos < 0 || ipos >= strlen(sobj))
    {
      o = latO_nulo;
      latC_apilar(mv, o);
      return;
    }
    char c[2] = {sobj[ipos], '\0'};
    o = latC_crear_cadena(mv, c);
  }
  latC_apilar(mv, o);
}

static void latMV_store_map(lat_mv *mv)
{
  lat_objeto *key = latC_desapilar(mv);
  lat_objeto *val = latC_desapilar(mv);
  lat_objeto *dic = latC_tope(mv);
  while (dic && dic->tipo != T_DIC)
  {
    latC_desapilar(mv);
    dic = latC_tope(mv);
  }
  if (!latO_comparar(mv, dic, val))
  {
    latC_error(mv, "Referencia circular detectada");
  }
  char *_k = NULL;
  if (key->tipo == T_NUMERIC)
  {
    _k = latC_astring(mv, key);
  }
  else
  {
    _k = latC_checar_cadena(mv, key);
  }
  latH_asignar(mv, latC_checar_dic(mv, dic), _k, val);
}

static void latMV_store_attr(lat_mv *mv, lat_bytecode cur)
{
  lat_objeto *attr = (lat_objeto *)cur.meta;
  lat_objeto *obj = latC_desapilar(mv);
  lat_objeto *val = latC_desapilar(mv);
  /* Soporte para instancias: asignar propiedad en lat_instancia */
  if (obj && obj->tipo == T_INSTANCE)
  {
    lat_instancia *instancia = (lat_instancia *)obj->val.cpointer;
    const char *pname = latC_checar_cadena(mv, attr);
    latC_asignar_propiedad(mv, instancia, pname, val);
    return;
  }
  if (obj->tipo == T_DIC)
  {
    if (!latO_comparar(mv, obj, val))
    {
      latC_error(mv, "Referencia circular detectada");
    }
    latH_asignar(mv, latC_checar_dic(mv, obj), latC_checar_cadena(mv, attr),
                 val);
  }
}

static void latMV_op_var_args(lat_mv *mv, lat_bytecode cur)
{
  int num_elem = cur.a;
  lat_objeto *obj = latC_crear_lista(mv, latL_crear(mv));
  while (num_elem < mv->ptrpila)
  {
    lat_objeto *tmp = latO_clonar(mv, latC_desapilar(mv));
    if (tmp != NULL && tmp->tipo != T_NULL)
    {
      latL_insertar_inicio(mv, latC_checar_lista(mv, obj), tmp);
    }
  }
  lat_objeto *ctx = obtener_contexto(mv);
  latO_asignar_ctx(mv, ctx, "varargs", obj);
}

static void latMV_load_var_args(lat_mv *mv)
{
  lat_objeto *ctx = obtener_contexto(mv);
  lat_objeto *val = (lat_objeto *)latO_obtener_contexto(mv, ctx, "varargs");
  if (!val)
  {
    latC_error(mv, "varargs (...) no existe en este contexto");
  }
  lista *list = latC_checar_lista(mv, val);
  LIST_FOREACH(list, primero, siguiente, cur)
  {
    lat_objeto *tmp = (lat_objeto *)cur->valor;
    if (tmp != NULL && tmp->tipo != T_NULL)
    {
      latC_apilar(mv, tmp);
    }
  }
}

/* Ejecuta el constructor del padre (super) inyectando 'mi' y reusando
 * argumentos */
void latMV_ejecutar_constructor_padre(lat_mv *mv, lat_objeto *constructor,
                                      lat_objeto *instancia, int nargs)
{
  if (!constructor || constructor->tipo != T_FUN)
  {
    latC_error(mv, "Constructor inválido");
    return;
  }
  if (!instancia || instancia->tipo != T_INSTANCE)
  {
    latC_error(mv, "Instancia inválida");
    return;
  }

  /* Desapilar argumentos actuales de super() */
  lat_objeto **args = NULL;
  if (nargs > 0)
  {
    args = (lat_objeto **)latM_asignar(mv, sizeof(lat_objeto *) * nargs);
    for (int i = nargs - 1; i >= 0; i--)
    {
      args[i] = latC_desapilar(mv);
    }
  }

  /* Ejecutar el constructor del padre con manejo de contexto coherente */
  apilar_contexto(mv, NULL);
  /* Inyectar 'mi' en el contexto del constructor padre (después de apilar contexto) */
  latO_asignar_ctx(mv, obtener_contexto(mv), "mi", instancia);
  /* Apilar SOLO los argumentos explícitos en orden (sin 'mi' en la pila) */
  for (int i = 0; i < nargs; i++)
  {
    latC_apilar(mv, args[i]);
  }
  mv->ptrprevio = (mv->ptrpila);
  int retc = latC_llamar_funcion(mv, constructor);
  mv->prev_args = retc;
  desapilar_contexto(mv);
  if (args)
  {
    latM_liberar(mv, args);
  }
}
/* Instrumentación: utilidades para depurar super() dentro del constructor padre */
#if DEPURAR_MV
static const char *dbg_tipo_nombre(int t)
{
  switch (t)
  {
  case T_NULL:
    return "NULL";
  case T_BOOL:
    return "BOOL";
  case T_NUMERIC:
    return "NUMERIC";
  case T_STR:
    return "STR";
  case T_CONTEXT:
    return "CONTEXT";
  case T_LIST:
    return "LIST";
  case T_DIC:
    return "DIC";
  case T_FUN:
    return "FUN";
  case T_CFUN:
    return "CFUN";
  case T_CPTR:
    return "CPTR";
  case T_CLASS:
    return "CLASS";
  case T_INSTANCE:
    return "INSTANCE";
  case T_INTEGER:
    return "INTEGER";
  case T_CHAR:
    return "CHAR";
  case T_LABEL:
    return "LABEL";
  default:
    return "?";
  }
}

static void dbg_super_log_stack(lat_mv *mv, lat_objeto *func,
                                lat_bytecode cur)
{
  (void)mv;
  (void)func;
  (void)cur;
  /* logs generales (solo en DEPURAR_MV) */
}
#else
static void dbg_super_log_stack(lat_mv *mv, lat_objeto *func,
                                lat_bytecode cur)
{
  (void)mv;
  (void)func;
  (void)cur;
}
#endif

int latMV_funcion_correr(lat_mv *mv, lat_objeto *func)
{
  if (func->tipo == T_FUN)
  {
#if DEPURAR_MV
    printf("\n.::Ejecutando fun::.\n");
#endif
    if (func->nombre == NULL)
    {
      func->nombre = "dummy";
      latO_asignar_ctx(mv, obtener_contexto(mv), "dummy", func);
    }
    lat_bytecode *inslist = getFun(func)->codigo;
    lat_bytecode cur;
    int pc = 0;

#if USE_COMPUTED_GOTO
    // OPTIMIZACIÓN: Tabla de etiquetas para despacho rápido
    static void *dispatch_table[] = {&&op_NOP,
                                     &&op_HALT,
                                     &&op_UNARY_MINUS,
                                     &&op_BINARY_ADD,
                                     &&op_BINARY_SUB,
                                     &&op_BINARY_MUL,
                                     &&op_BINARY_DIV,
                                     &&op_BINARY_MOD,
                                     &&op_OP_GT,
                                     &&op_OP_GE,
                                     &&op_OP_LT,
                                     &&op_OP_LE,
                                     &&op_OP_EQ,
                                     &&op_OP_NEQ,
                                     &&op_OP_AND,
                                     &&op_OP_OR,
                                     &&op_OP_NOT,
                                     &&op_OP_INC,
                                     &&op_OP_DEC,
                                     &&op_CONCAT,
                                     &&op_LOAD_CONST,
                                     &&op_LOAD_NAME,
                                     &&op_STORE_NAME,
                                     &&op_JUMP_ABSOLUTE,
                                     &&op_POP_JUMP_IF_FALSE,
                                     &&op_POP_JUMP_IF_TRUE,
                                     &&op_PUSH_CTX,
                                     &&op_POP_CTX,
                                     &&op_CALL_FUNCTION,
                                     &&op_RETURN_VALUE,
                                     &&op_MAKE_FUNCTION,
                                     &&op_LOAD_ATTR,
                                     &&op_BUILD_LIST,
                                     &&op_STORE_SUBSCR,
                                     &&op_BINARY_SUBSCR,
                                     &&op_BUILD_MAP,
                                     &&op_STORE_MAP,
                                     &&op_STORE_ATTR,
                                     &&op_SET_GLOBAL,
                                     &&op_OP_REGEX,
                                     &&op_BINARY_POW,
                                     &&op_OP_VAR_ARGS,
                                     &&op_OP_PUSH,
                                     &&op_OP_POP,
                                     &&op_ADJUST_STACK,
                                     &&op_LOAD_VAR_ARGS,
                                     &&op_SET_LOCAL,
                                     &&op_POP_JUMP_IF_NEGATIVE,
                                     &&op_JUMP_LABEL,
                                     &&op_STORE_LABEL};
#define DISPATCH() goto *dispatch_table[cur.ins]
#else
#define DISPATCH() goto dispatch_switch
#endif

    cur = inslist[pc];
    DISPATCH();

#if USE_COMPUTED_GOTO
  // Instrucciones como etiquetas
  op_NOP:
    pc++;
    cur = inslist[pc];
    DISPATCH();
  op_HALT:
    return 0;
  op_UNARY_MINUS:
  {
    lat_objeto *obj = latC_tope(mv);
    setNumerico(obj, -(latC_adouble(mv, obj)));
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_OP_INC:
  {
    lat_objeto *name = (lat_objeto *)cur.meta;
    lat_objeto *ctx = obtener_contexto(mv);
    lat_objeto *val =
        latO_obtener_contexto(mv, ctx, latC_checar_cadena(mv, name));
    if (val == NULL)
    {
      val = latC_crear_numerico(mv, 0);
    }
    getNumerico(val)++;
    latO_asignar_ctx(mv, ctx, latC_checar_cadena(mv, name), val);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_OP_DEC:
  {
    lat_objeto *name = (lat_objeto *)cur.meta;
    lat_objeto *ctx = obtener_contexto(mv);
    lat_objeto *val =
        latO_obtener_contexto(mv, ctx, latC_checar_cadena(mv, name));
    if (val == NULL)
    {
      val = latC_crear_numerico(mv, 0);
    }
    getNumerico(val)--;
    latO_asignar_ctx(mv, ctx, latC_checar_cadena(mv, name), val);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_BINARY_SUB:
  {
    arith_op(lati_numSub);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_BINARY_ADD:
  {
    arith_op(lati_numAdd);
    pc++;
    cur = inslist[pc];
    DISPATCH();

  op_BINARY_POW:
  {
    arith_op(lati_numPow);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_BINARY_MUL:
  {
    arith_op(lati_numMul);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_BINARY_DIV:
  {
    arith_op(lati_numDiv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_BINARY_MOD:
  {
    arith_op(lati_numMod);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_OP_GT:
  {
    mayor_que(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_OP_GE:
  {
    mayor_igual(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_OP_LT:
  {
    menor_que(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_OP_LE:
  {
    menor_igual(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_OP_EQ:
  {
    igualdad(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_OP_REGEX:
  {
    str_regex(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_OP_NEQ:
  {
    diferente(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_OP_AND:
  {
    y_logico(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_OP_OR:
  {
    o_logico(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_OP_NOT:
  {
    no_logico(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_CONCAT:
  {
    str_concatenar(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_LOAD_CONST:
  {
    lat_objeto *o = (lat_objeto *)cur.meta;
    latC_apilar(mv, o);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_STORE_NAME:
  {
    lat_objeto *val = (mv->ptrpila == 0) ? latO_nulo : latC_desapilar(mv);
    lat_objeto *name = (lat_objeto *)cur.meta;
    latMV_set_symbol(mv, name, val);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_STORE_LABEL:
  {
    lat_objeto *name = (lat_objeto *)cur.meta;
    lat_objeto *val = latO_clonar(mv, name);
    latMV_set_label(mv, name, val);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_SET_GLOBAL:
  {
    mv->contexto_actual = obtener_contexto_global(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_SET_LOCAL:
  {
    mv->contexto_actual = mv->contexto[mv->ptrctx];
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_LOAD_NAME:
  {
    lat_objeto *name = (lat_objeto *)cur.meta;
    lat_objeto *val = latMV_get_symbol(mv, name);
    latC_apilar(mv, val);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_POP_JUMP_IF_FALSE:
  {
    lat_objeto *o = latC_desapilar(mv);
    if (latC_abool(mv, o) == false)
    {
      pc = cur.a;
      cur = inslist[pc];
    }
    else
    {
      pc++;
      cur = inslist[pc];
    }
    DISPATCH();
  }
  op_POP_JUMP_IF_TRUE:
  {
    lat_objeto *o = latC_desapilar(mv);
    if (latC_abool(mv, o) == true)
    {
      pc = cur.a;
      cur = inslist[pc];
    }
    else
    {
      pc++;
      cur = inslist[pc];
    }
    DISPATCH();
  }
  op_JUMP_ABSOLUTE:
  {
    pc = cur.a;
    cur = inslist[pc];
    DISPATCH();
  }
  op_JUMP_LABEL:
  {
    lat_objeto *name = (lat_objeto *)cur.meta;
    lat_objeto *val = latMV_get_label(mv, name);
    pc = val->jump_label - 1;
    cur = inslist[pc];
    DISPATCH();
  }
  op_CALL_FUNCTION:
  {
    /* Instrumentación: registrar estado de pila justo antes de la llamada */
    dbg_super_log_stack(mv, func, cur);
    latMV_call_function(mv, func, cur, inslist[pc + 1]);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_RETURN_VALUE:
  {
    return cur.a;
  }
  op_MAKE_FUNCTION:
  {
    lat_objeto *fun = (lat_objeto *)cur.meta;
    latC_apilar(mv, fun);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_LOAD_ATTR:
  {
    latMV_load_attr(mv, cur, inslist[pc + 1]);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_BUILD_LIST:
  {
    latMV_build_list(mv, cur);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_STORE_SUBSCR:
  {
    latMV_store_subscr(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_BINARY_SUBSCR:
  {
    binary_subscr(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_BUILD_MAP:
  {
    lat_objeto *o = latC_crear_dic(mv, latH_crear(mv));
    latC_apilar(mv, o);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_STORE_MAP:
  {
    latMV_store_map(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_STORE_ATTR:
  {
    latMV_store_attr(mv, cur);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_OP_VAR_ARGS:
  {
    latMV_op_var_args(mv, cur);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_LOAD_VAR_ARGS:
  {
    latMV_load_var_args(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_OP_PUSH:
  {
    lat_objeto *obj = (lat_objeto *)cur.meta;
    latC_apilar(mv, obj ? obj : latO_nulo);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_OP_POP:
  {
    latC_desapilar(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_ADJUST_STACK:
  {
    while (cur.a > mv->ptrpila)
    {
      latC_apilar(mv, latO_nulo);
    }
    while (mv->ptrpila >= (mv->ptrprevio + cur.a))
    {
      latC_desapilar(mv);
    }
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_PUSH_CTX:
  {
    apilar_contexto(mv, NULL);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_POP_CTX:
  {
    desapilar_contexto(mv);
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_STORE_LABEL:
  { /* ya implementado arriba */
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  op_POP_JUMP_IF_NEGATIVE:
  { /* No implementado */
    pc++;
    cur = inslist[pc];
    DISPATCH();
  }
  // Si alguna instrucción no está implementada, continuar
  default:
    pc++;
    cur = inslist[pc];
    DISPATCH();
#endif // USE_COMPUTED_GOTO

  dispatch_switch:
    for (; cur.ins != HALT; cur = inslist[++pc])
    {
      lat_bytecode next = inslist[pc + 1];
      mv->nlin = cur.nlin;
      mv->ncol = cur.ncol;
      mv->nombre_archivo = cur.nombre_archivo;
      switch (cur.ins)
      {
      case HALT:
        return 0;
      case NOP:
        continue;
      case UNARY_MINUS:
      {
        lat_objeto *obj = latC_tope(mv);
        setNumerico(obj, -(latC_adouble(mv, obj)));
      }
      break;
      case OP_INC:
      {
        lat_objeto *name = (lat_objeto *)cur.meta;
        lat_objeto *ctx = obtener_contexto(mv);
        lat_objeto *val =
            latO_obtener_contexto(mv, ctx, latC_checar_cadena(mv, name));
        if (val == NULL)
        {
          val = latC_crear_numerico(mv, 0);
        }
        getNumerico(val)++;
        latO_asignar_ctx(mv, ctx, latC_checar_cadena(mv, name), val);
      }
      break;
      case OP_DEC:
      {
        lat_objeto *name = (lat_objeto *)cur.meta;
        lat_objeto *ctx = obtener_contexto(mv);
        lat_objeto *val =
            latO_obtener_contexto(mv, ctx, latC_checar_cadena(mv, name));
        if (val == NULL)
        {
          val = latC_crear_numerico(mv, 0);
        }
        getNumerico(val)--;
        latO_asignar_ctx(mv, ctx, latC_checar_cadena(mv, name), val);
      }
      break;
      case BINARY_ADD:
      {
        arith_op(lati_numAdd);
      }
      break;
      case BINARY_SUB:
      {
        arith_op(lati_numSub);
      }
      break;
      case BINARY_POW:
      {
        arith_op(lati_numPow);
      }
      break;
      case BINARY_MUL:
      {
        arith_op(lati_numMul);
      }
      break;
      case BINARY_DIV:
      {
        arith_op(lati_numDiv);
      }
      break;
      case BINARY_MOD:
      {
        arith_op(lati_numMod);
      }
      break;
      case OP_GT:
      {
        mayor_que(mv);
      }
      break;
      case OP_GE:
      {
        mayor_igual(mv);
      }
      break;
      case OP_LT:
      {
        menor_que(mv);
      }
      break;
      case OP_LE:
      {
        menor_igual(mv);
      }
      break;
      case OP_EQ:
      {
        igualdad(mv);
      }
      break;
      case OP_REGEX:
      {
        str_regex(mv);
      }
      break;
      case OP_NEQ:
      {
        diferente(mv);
      }
      break;
      case OP_AND:
      {
        y_logico(mv);
      }
      break;
      case OP_OR:
      {
        o_logico(mv);
      }
      break;
      case OP_NOT:
      {
        no_logico(mv);
      }
      break;
      case CONCAT:
      {
        str_concatenar(mv);
      }
      break;
      case LOAD_CONST:
      {
        lat_objeto *o = (lat_objeto *)cur.meta;
#if DEPURAR_MV
        latO_imprimir(mv, o, false);
        printf("\t");
#endif
        latC_apilar(mv, o);
      }
      break;
      case STORE_NAME:
      {
        lat_objeto *val = NULL;
        if (mv->ptrpila == 0)
        {
          val = latO_nulo;
        }
        else
        {
          val = latC_desapilar(mv);
        }
        lat_objeto *name = (lat_objeto *)cur.meta;
        if (DEPURAR_MV)
        {
          const char *n = latC_checar_cadena(mv, name);
          int t = val ? val->tipo : -1;
          printf("[VM] STORE_NAME name='%s' tipo=%d ptrpila=%d\n", n, t, mv->ptrpila);
        }
        latMV_set_symbol(mv, name, val);
#if DEPURAR_MV
        latO_imprimir(mv, name, false);
        printf("\t");
#endif
      }
      break;
      case STORE_LABEL:
      {
        lat_objeto *name = (lat_objeto *)cur.meta;
        lat_objeto *val = latO_clonar(mv, name);
        latMV_set_label(mv, name, val);
#if DEPURAR_MV
        latO_imprimir(mv, name, false);
        printf("\t");
#endif
      }
      break;
      case SET_GLOBAL:
      {
        mv->contexto_actual = obtener_contexto_global(mv);
      }
      break;
      case SET_LOCAL:
      {
        mv->contexto_actual = mv->contexto[mv->ptrctx];
      }
      break;
      case LOAD_NAME:
      {
        lat_objeto *name = (lat_objeto *)cur.meta;
#if DEPURAR_MV
        latO_imprimir(mv, name, false);
        printf("\t");
#endif
        lat_objeto *val = latMV_get_symbol(mv, name);
        latC_apilar(mv, val);
      }
      break;
      case POP_JUMP_IF_FALSE:
      {
#if DEPURAR_MV
        printf("%i\t", cur.a + 1);
#endif
        lat_objeto *o = latC_desapilar(mv);
        if (latC_abool(mv, o) == false)
        {
          pc = cur.a;
        }
      }
      break;
      case POP_JUMP_IF_TRUE:
      {
        // TODO: Unused bytecode
        lat_objeto *o = latC_desapilar(mv);
        if (latC_abool(mv, o) == true)
        {
          pc = cur.a;
        }
      }
      break;
      case JUMP_ABSOLUTE:
#if DEPURAR_MV
        printf("%i\t", cur.a + 1);
#endif
        pc = cur.a;
        break;
      case JUMP_LABEL:
      {
        lat_objeto *name = (lat_objeto *)cur.meta;
#if DEPURAR_MV
        latO_imprimir(mv, name, false);
        printf("\t");
#endif
        lat_objeto *val = latMV_get_label(mv, name);
        // if(val->tipo != T_LABEL) {
        //    char *nombre = latC_checar_cadena(mv, name);
        //    latC_error(mv, "El identificador '%s' no es un tipo
        //    etiqueta", nombre);
        //}
        pc =
            val->jump_label - 1; // saltamos hacia la instruccion de la etiqueta
      }
      break;
      case CALL_FUNCTION:
      {
#if DEPURAR_MV
        printf("\n[RESULTADO] >>> ");
#endif
        /* Instrumentación: registrar estado de pila justo antes de la llamada
         */
        dbg_super_log_stack(mv, func, cur);
        latMV_call_function(mv, func, cur, next);
      }
      break;
      case RETURN_VALUE:
      {
#if DEPURAR_MV
        printf("%i\n", cur.a);
#endif
        return cur.a;
      }
      break;
      case MAKE_FUNCTION:
      {
        lat_objeto *fun = (lat_objeto *)cur.meta;
        latC_apilar(mv, fun);
      }
      break;
      case MAKE_CLASS:
      {
        // Mostrar el stack
        fprintf(stderr, "  Stack: %d\n", mv->ptrpila);
        /* MAKE_CLASS inicio (sin logs) */

        lat_class_meta *cm = (lat_class_meta *)cur.meta;
        if (!cm || !cm->func)
        {
          latC_error(mv, "MAKE_CLASS: metadatos de clase inválidos");
        }

        // Mostrar el nombre de la clase y la clase base si existe
        fprintf(stderr, "  class_name=%s, base_name=%s\n",
                cm->class_name ? cm->class_name : "(null)",
                cm->base_name ? cm->base_name : "(null)");

        // 1. Guardar contexto actual
        lat_objeto *saved_ctx = obtener_contexto(mv);

        // 2. Crear contexto temporal para la clase
        apilar_contexto(mv, NULL);

        // 3. Ejecutar la función de inicialización de clase
        // Esto llenará __clase_dict__ con métodos
        mv->enClase = true;
        latC_llamar_funcion(mv, cm->func);
        mv->enClase = false;

        // 4. Obtener el diccionario de métodos
        lat_objeto *dict = latO_obtener_contexto(mv, obtener_contexto(mv), "__clase_dict__");
        if (!dict || dict->tipo != T_DIC)
        {
          desapilar_contexto(mv);
          latC_error(mv, "MAKE_CLASS: diccionario de clase no encontrado");
        }

        // 5. Crear estructura de clase
        lat_class *clase = latC_crear_clase(mv, cm->class_name, NULL);

        // 6. Buscar clase base si existe
        if (cm->base_name)
        {
          lat_objeto *base_class = latO_obtener_contexto(mv, saved_ctx, cm->base_name);
          if (base_class && base_class->tipo == T_CLASS)
          {
            clase->padre = (lat_class *)base_class->val.cpointer;
          }
        }

        // 7. Copiar métodos del diccionario a la clase
        hash_map *m = latC_checar_dic(mv, dict);
        size_t i;
        for (i = 0; i < m->capacity; i++)
        {
          lista *list = m->buckets[i];
          if (list != NULL)
          {
            LIST_FOREACH(list, primero, siguiente, cur)
            {
              if (cur->valor)
              {
                hash_val *hv = (hash_val *)cur->valor;
                lat_objeto *metodo = (lat_objeto *)hv->valor;

                // Buscar si es el constructor
                if (strcmp(hv->llave, "constructor") == 0)
                {
                  clase->constructor = metodo;
                  // Mostrar el constructor
        /* Encontrado constructor (sin logs) */
                }

                // Agregar método a la clase
                latC_agregar_metodo(mv, clase, hv->llave, metodo, false);
              }
            }
          }
        }

        // 8. Crear objeto de clase
        lat_objeto *clase_obj = latO_crear(mv);
        clase_obj->tipo = T_CLASS;
        clase_obj->val.cpointer = clase;
        clase_obj->es_clase = true;
        clase_obj->nombre = strdup(cm->class_name);

        // 9. Restaurar contexto y apilar clase
        desapilar_contexto(mv);
        latC_apilar(mv, clase_obj);

        // 10. Limpiar metadatos
        if (cm->base_name)
          free((void *)cm->base_name);
        if (cm->class_name)
          free((void *)cm->class_name);
        latM_liberar(mv, cm);

        // Mostrar el nombre de la clase y el constructor
        /* Clase creada (sin logs) */
      }
      break;
      case PUSH_CTX:
        apilar_contexto(mv, NULL);
        break;
      case POP_CTX:
        desapilar_contexto(mv);
        break;
      case BUILD_LIST:
      {
        latMV_build_list(mv, cur);
      }
      break;
      case LOAD_ATTR:
      {
        latMV_load_attr(mv, cur, next);
      }
      break;
      case STORE_SUBSCR:
      {
        latMV_store_subscr(mv);
      }
      break;
      case BINARY_SUBSCR:
      {
        binary_subscr(mv);
      }
      break;
      case BUILD_MAP:
      {
        lat_objeto *o = latC_crear_dic(mv, latH_crear(mv));
        latC_apilar(mv, o);
      }
      break;
      case STORE_MAP:
      {
        latMV_store_map(mv);
      }
      break;
      case STORE_ATTR:
      {
        latMV_store_attr(mv, cur);
      }
      break;
      case OP_VAR_ARGS:
      {
        latMV_op_var_args(mv, cur);
      }
      break;
      case LOAD_VAR_ARGS:
      {
        latMV_load_var_args(mv);
      }
      break;
      case OP_PUSH:
      {
        lat_objeto *obj = (lat_objeto *)cur.meta;
        latC_apilar(mv, obj);
      }
      break;
      case OP_POP:
      {
        latC_desapilar(mv);
      }
      break;
      case ADJUST_STACK:
      {
#if DEPURAR_MV
        printf("%i\t", cur.a);
#endif
        while (cur.a > mv->ptrpila)
        {
          latC_apilar(mv, latO_nulo);
        }
        while (mv->ptrpila >= (mv->ptrprevio + cur.a))
        {
          latC_desapilar(mv);
        }
      }
      break;
      case LOAD_PROPERTY:
      {
        // Obtener propiedad de una instancia: valor = mi.propiedad
        // El nombre de la propiedad está en cur.meta
        // La instancia debe estar en el tope de la pila
        lat_objeto *prop_name = (lat_objeto *)cur.meta;
        lat_objeto *inst = latC_desapilar(mv);

        // Verificar que es una instancia
        if (!inst || inst->tipo != T_INSTANCE)
        {
          latC_error(mv, "LOAD_PROPERTY: No es una instancia");
        }

        // Obtener la instancia interna
        lat_instancia *instancia = (lat_instancia *)inst->val.cpointer;

        // Obtener la propiedad
        lat_objeto *valor = latC_obtener_propiedad(
            instancia, latC_checar_cadena(mv, prop_name));

        // Si no existe, retornar nulo
        if (!valor)
        {
          valor = &latO_nulo_;
        }

        // sin logs: solo apilar el valor

        // Apilar el valor
        latC_apilar(mv, valor);
      }
      break;

      case STORE_PROPERTY:
      {
        // Asignar propiedad de una instancia: mi.propiedad = valor
        // El nombre de la propiedad está en cur.meta
        // En la pila: [instancia, valor] (valor en tope)
        lat_objeto *prop_name = (lat_objeto *)cur.meta;
        lat_objeto *valor = latC_desapilar(mv);
        lat_objeto *inst = latC_desapilar(mv);

        // Verificar que es una instancia
        if (!inst || inst->tipo != T_INSTANCE)
        {
          latC_error(mv, "STORE_PROPERTY: No es una instancia");
        }

        // Obtener la instancia interna
        lat_instancia *instancia = (lat_instancia *)inst->val.cpointer;

        // Asignar la propiedad
        latC_asignar_propiedad(mv, instancia, latC_checar_cadena(mv, prop_name),
                               valor);

        // sin logs
      }
      break;

      case LOAD_SUPER:
      {
        /* super(): llamar constructor de clase padre */
        // Llamar constructor de clase padre: super(args)
        // Número de argumentos en cur.a
        // Los argumentos están en la pila
        // Necesitamos obtener 'mi' del contexto actual
        int num_args = cur.a;

        // Obtener 'mi' del contexto actual (debe estar definido en el método)
        lat_objeto *mi = latO_obtener_contexto(mv, obtener_contexto(mv), "mi");
        if (!mi || mi->tipo != T_INSTANCE)
        {
          latC_error(mv, "LOAD_SUPER: super() solo puede usarse dentro de un "
                         "método de clase");
        }

        // Obtener la instancia
        lat_instancia *inst = (lat_instancia *)mi->val.cpointer;

        // Verificar que hay clase padre
        if (!inst->clase->padre)
        {
          latC_error(mv, "LOAD_SUPER: La clase no tiene padre");
        }

        // Llamar constructor del padre
        latC_llamar_super(mv, mi, num_args);
      }
      break;

      case LOAD_METHOD:
      {
        // Cargar método de una instancia: obj.metodo
        // El nombre del método está en cur.meta
        // La instancia debe estar en el tope de la pila
        lat_objeto *method_name = (lat_objeto *)cur.meta;
        lat_objeto *inst = latC_desapilar(mv);

        // Verificar que es una instancia
        if (!inst || inst->tipo != T_INSTANCE)
        {
          latC_error(mv, "LOAD_METHOD: No es una instancia");
        }

        // Obtener la instancia interna
        lat_instancia *instancia = (lat_instancia *)inst->val.cpointer;

        // Buscar el método en la clase
        lat_objeto *metodo = latC_obtener_metodo(
            instancia->clase, latC_checar_cadena(mv, method_name));

        // Verificar que el método existe
        if (!metodo)
        {
          latC_error(mv, "LOAD_METHOD: Método '%s' no encontrado",
                     latC_checar_cadena(mv, method_name));
        }

        // Apilar el método y la instancia (para CALL_METHOD)
        latC_apilar(mv, metodo);
        latC_apilar(mv, inst); // Guardar instancia para contexto 'mi'
      }
      break;

      case CALL_METHOD:
      {
        // Llamar método con contexto de instancia
        // Número de argumentos en cur.a
        // En la pila: [metodo, instancia, arg1, arg2, ...]
        int num_args = cur.a;

        // Desapilar argumentos
        lat_objeto **args = NULL;
        if (num_args > 0)
        {
          args =
              (lat_objeto **)latM_asignar(mv, sizeof(lat_objeto *) * num_args);
          for (int i = num_args - 1; i >= 0; i--)
          {
            args[i] = latC_desapilar(mv);
          }
        }

        // Desapilar instancia y método
        lat_objeto *inst = latC_desapilar(mv);
        lat_objeto *metodo = latC_desapilar(mv);

        // Establecer 'mi' en el contexto actual para la llamada del método
        latO_asignar_ctx(mv, obtener_contexto(mv), "mi", inst);

        // Apilar argumentos
        for (int i = 0; i < num_args; i++)
        {
          latC_apilar(mv, args[i]);
        }

        // Llamar función
        latC_llamar_funcion(mv, metodo);

        // Liberar array de argumentos
        if (args)
        {
          latM_liberar(mv, args);
        }
      }
      break;

      case NEW_INSTANCE:
      {
        // Crear nueva instancia de clase: obj = nueva Clase(args)
        // Número de argumentos en cur.a
        // La clase debe estar en el tope de la pila
        // Los argumentos están en la pila
        int num_args = cur.a;

        // Mostrar el número de argumentos y el stack
        /* NEW_INSTANCE args/stack (sin logs) */

        // Desapilar argumentos
        lat_objeto **args = NULL;
        if (num_args > 0)
        {
          args =
              (lat_objeto **)latM_asignar(mv, sizeof(lat_objeto *) * num_args);
          for (int i = num_args - 1; i >= 0; i--)
          {
            args[i] = latC_desapilar(mv);
            // Mostrar el tipo de cada argumento y el stack
            fprintf(stderr, "  arg[%d]: tipo=%d, stack=%d\n", i, args[i]->tipo, mv->ptrpila);
          }
        }

        // Desapilar la clase
        lat_objeto *clase_obj = latC_desapilar(mv);
        // Mostrar el tipo de la clase
        fprintf(stderr, "  clase_obj: %p, tipo=%d (T_CLASS=%d)\n",
                clase_obj, clase_obj ? clase_obj->tipo : -1, T_CLASS);

        // Verificar que es una función de clase
        if (!clase_obj || clase_obj->tipo != T_CLASS)
        {
          // Mostrar el stack
          fprintf(stderr, "ERROR: No es una clase, stack=%d\n", mv->ptrpila);
          // Mostrar el stack
          latC_error(mv, "NEW_INSTANCE: No es una clase (tipo=%d, esperado T_CLASS=10)",
                     clase_obj ? clase_obj->tipo : -1, mv->ptrpila);
        }

        // Obtener la clase interna
        lat_class *clase = (lat_class *)clase_obj->val.cpointer;

        // Mostrar el nombre de la clase y el constructor
        fprintf(stderr, "  clase: %p, nombre=%s\n",
                clase, clase ? clase->nombre : "(null)");

        // Crear la instancia
        lat_objeto *inst = latC_crear_instancia(mv, clase);

        // Mostrar el tipo de la instancia (sin logs)

        // Si hay constructor, llamarlo
        if (clase->constructor)
        {
          // Apilar instancia como 'mi'
          latC_apilar(mv, inst);

          // Apilar argumentos
          for (int i = 0; i < num_args; i++)
          {
            latC_apilar(mv, args[i]);
          }

          // Llamar constructor
          latC_llamar_funcion(mv, clase->constructor);
        }
        else
        {
          // No hay constructor (sin logs)
        }

        // Liberar array de argumentos
        if (args)
        {
          latM_liberar(mv, args);
          // Argumentos liberados (sin logs)
        }

        // Apilar la instancia creada
        latC_apilar(mv, inst);
      }
      break;

      default:
        continue;
      } // fin de switch

#if DEPURAR_MV
      imprimir_pila(mv);
#endif
    } // fin for
#if HABILITAR_GC
    gc_recolectar(mv);
#endif
  } // fin if (T_FUN)
    else if (func->tipo == T_CFUN)
    {
      lat_CFuncion f = getCfun(func);
      (f)(mv);
      return 0;
    }
    else
    {
      latC_error(mv, "El objeto no es una funcion");
    }
    return 0;
  }
