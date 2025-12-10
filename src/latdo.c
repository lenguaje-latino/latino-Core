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

#include "latdo.h"
#include "latast.h"
#include "latino.h"
#include "latmem.h"
#include "latmv.h"
#include "latobj.h"

#ifndef DEPURAR_AST
#define DEPURAR_AST 1
#endif

#define dbc(I, A, B, M, L, C, F)                                               \
  codigo[i++] = latMV_bytecode_crear(I, A, B, M, L, C, F)
#define pn(vm, N) i = ast_analizar(vm, N, codigo, i)
#define fdbc(I, A, B, M, L, C, F)                                              \
  funcion_codigo[fi++] = latMV_bytecode_crear(I, A, B, M, L, C, F)
#define fpn(vm, N) fi = ast_analizar(vm, N, funcion_codigo, fi)

struct lat_longjmp {
  struct lat_longjmp *previo;
  lat_jmpbuf b;
  volatile int status;
};

void str_concatenar(lat_mv *mv);
static bool encontrar_romper(ast *nodo);
static bool encontrar_continuar(ast *nodo);
static bool jumpLoop_buscar_NODO_SI(ast *nodo, int NODO_JUMP);

static ast *transformar_caso_casos(ast *cond_izq, ast *izq) {
  if (izq->tipo == NODO_VALOR) {
    ast *igualdad_izq =
        latA_nodo(NODO_IGUALDAD, cond_izq, izq, cond_izq->nlin, cond_izq->ncol);
    return igualdad_izq;
  }
  if (izq->der->tipo == NODO_VALOR) {
    // esto detiene la recursividad
    ast *igualdad_izq = latA_nodo(NODO_IGUALDAD, cond_izq, izq->izq,
                                  cond_izq->nlin, cond_izq->ncol);
    ast *igualdad_der = latA_nodo(NODO_IGUALDAD, cond_izq, izq->der,
                                  cond_izq->nlin, cond_izq->ncol);
    return latA_nodo(NODO_O, igualdad_izq, igualdad_der, cond_izq->nlin,
                     cond_izq->ncol);
  }
  if (izq->tipo == NODO_CASOS) {
    // recursion
    // izq->der->tipo == NODO_CASOS
    ast *primera_igualdad = latA_nodo(NODO_IGUALDAD, cond_izq, izq->izq,
                                      cond_izq->nlin, cond_izq->ncol);
    ast *cond = latA_nodo(NODO_O, primera_igualdad,
                          transformar_caso_casos(cond_izq, izq->der),
                          cond_izq->nlin, cond_izq->ncol);
    return cond;
  }
  return NULL;
}

static ast *transformar_casos(ast *casos, ast *cond_izq) {
  if (casos == NULL) {
    return NULL;
  }
  ast *caso_izq = casos->izq; // caso
  ast *cond = NULL;

  if (caso_izq->tipo == NODO_CASO) {
    if (caso_izq->izq->tipo == NODO_CASOS) {
      // evalua multicasos
      ast *cond = NULL;
      ast *primera_igualdad =
          latA_nodo(NODO_IGUALDAD, cond_izq, caso_izq->izq->izq, cond_izq->nlin,
                    cond_izq->ncol);
      cond = latA_nodo(NODO_O, primera_igualdad,
                       transformar_caso_casos(cond_izq, caso_izq->izq->der),
                       cond_izq->nlin, cond_izq->ncol);
      ast *nSi = latA_si(cond, caso_izq->der,
                         ((ast *)transformar_casos(casos->der, cond_izq)));
      return nSi;
    }
    // un solo caso
    cond = latA_nodo(NODO_IGUALDAD, cond_izq, caso_izq->izq, cond_izq->nlin,
                     cond_izq->ncol);
  }
  if (caso_izq->tipo == NODO_DEFECTO) {
    cond = latA_nodo(NODO_IGUALDAD, cond_izq, cond_izq, cond_izq->nlin,
                     cond_izq->ncol);
  }
  ast *nSi = latA_si(cond, caso_izq->der,
                     ((ast *)transformar_casos(casos->der, cond_izq)));
  return nSi;
}

static ast *transformar_elegir(ast *nodo_elegir) {
  ast *cond_izq = nodo_elegir->izq;
  ast *casos = nodo_elegir->der;
  ast *nSi = NULL;
  nSi = transformar_casos(casos, cond_izq);
  return nSi;
}

static void liberar_elegir(ast *a) {
  if (a) {
    switch (a->tipo) {
    case NODO_SI: {
      nodo_si *nsi = (nodo_si *)a;
      free(nsi->cond);
      liberar_elegir(nsi->entonces);
      if (nsi->_sino) {
        liberar_elegir(nsi->_sino);
      }
      free(a);
      break;
    }
    default: {
      ;
    }
    }
  }
}

static int contar_num_parargs(ast *nodo, nodo_tipo nt) {
  ast *tmp;
  int nparams = 0;
  if (nodo) {
    if (nodo->tipo == nt) {
      tmp = nodo;
      while (tmp->der != NULL && tmp->der->tipo == nt) {
        tmp = tmp->der;
        nparams++;
      }
      if (tmp->izq->tipo) {
        nparams++;
      }
    }
  }
  return nparams;
}

static bool encontrar_vararg(ast *nodo) {
  if (nodo && nodo->izq && nodo->izq->tipo == NODO_VAR_ARGS) {
    return true;
  }
  return false;
}

static int encontrar_load_vararg(ast *nodo) {
  int i = 0;
  if (nodo) {
    ast *tmp;
    tmp = nodo;
    while (tmp->der != NULL && tmp->der->tipo != NODO_LOAD_VAR_ARGS) {
      if (tmp->der != NULL && tmp->der->tipo == NODO_LOAD_VAR_ARGS) {
        i++;
      }
      tmp = tmp->der;
    }
    if (tmp->izq != NULL && tmp->izq->tipo == NODO_LOAD_VAR_ARGS) {
      i++;
    }
  }
  return i;
}

static bool jumpLoop_buscar_NODO_SI(ast *nodo, int NODO_JUMP) {
  ast *tmp = nodo;
  if (tmp == NULL) {
    return false;
  }
  while (tmp != NULL && tmp->izq != NULL) {
    if (tmp->izq->tipo == NODO_SI) {
      nodo_si *nIf = ((nodo_si *)tmp->izq->der);
      if (nIf->entonces->tipo == NODO_BLOQUE) {
        if (nIf->entonces->izq->tipo == NODO_JUMP) {
          return true;
        }
        if (nIf->entonces->der != NULL &&
            nIf->entonces->der->tipo == NODO_JUMP) {
          return true;
        }
      }
      if (nIf->_sino && nIf->_sino->tipo == NODO_BLOQUE) {
        if (nIf->_sino->izq->tipo == NODO_JUMP) {
          return true;
        }
        if (nIf->_sino->der->tipo == NODO_JUMP) {
          return true;
        }
      }
    } else {
      if (NODO_JUMP == NODO_ROMPER) {
        bool found = encontrar_romper(tmp->izq);
        if (found)
          return found;
      } else {
        bool found = encontrar_continuar(tmp->izq);
        if (found)
          return found;
      }
    }
    tmp = tmp->der;
  }
  return false;
}

static bool encontrar_romper(ast *nodo) {
  // TODO: hacer esta funcion recursiva para ROMPER y CONTINUAR
  bool rep = false;
  if (nodo) {
    ast *tmp = nodo;
    if (tmp->izq->tipo == NODO_ROMPER) {
      rep = true;
      return rep;
    }
    while (tmp->izq != NULL) {
      if (tmp->izq->tipo == NODO_ROMPER) {
        rep = true;
        return rep;
      }
      if (tmp->izq->tipo == NODO_SI) {
        rep = jumpLoop_buscar_NODO_SI(tmp->izq, NODO_ROMPER);
        if (rep) {
          return rep;
        }
      };
      if (tmp->der == NULL) {
        break;
      }
      tmp = tmp->der;
    }
  }
  return rep;
}

static bool encontrar_continuar(ast *nodo) {
  // TODO: hacer esta funcion recursiva para ROMPER y CONTINUAR
  bool rep = false;
  if (nodo) {
    ast *tmp = nodo;
    if (tmp->izq->tipo == NODO_CONTINUAR) {
      rep = true;
      return rep;
    }
    while (tmp->izq != NULL) {
      if (tmp->izq->tipo == NODO_CONTINUAR) {
        rep = true;
        return rep;
      }
      if (tmp->izq->tipo == NODO_SI) {
        rep = jumpLoop_buscar_NODO_SI(tmp->izq, NODO_CONTINUAR);
        if (rep) {
          return rep;
        }
      };
      if (tmp->der == NULL) {
        break;
      }
      tmp = tmp->der;
    }
  }
  return rep;
}

void latMV_set_symbol(lat_mv *mv, lat_objeto *name, lat_objeto *val);
lat_objeto *obtener_contexto_global(lat_mv *mv);

// analiza los nodos para crear el bytecode
static int ast_analizar(lat_mv *mv, ast *nodo, lat_bytecode *codigo, int i) {
    /* debug eliminado: ast_analizar enter */
  int temp[4] = {0};
  lat_bytecode *funcion_codigo = NULL;
  int fi = 0;
  if (nodo->nlin > 0) {
    mv->nlin = nodo->nlin;
    mv->ncol = nodo->ncol;
  }
  switch (nodo->tipo) {
  case NODO_VALOR: {
    lat_objeto *o = NULL;
    if (nodo->valor->tipo == VALOR_LOGICO) {
      o = (nodo->valor->val.logico == true) ? latO_verdadero : latO_falso;
    }
    if (nodo->valor->tipo == VALOR_NULO) {
      o = latO_nulo;
    }
    if (nodo->valor->tipo == VALOR_NUMERICO) {
      o = latC_crear_numerico(mv, nodo->valor->val.numerico);
#if DEPURAR_MEM
      printf("NODO_VALOR.VALOR_NUMERICO:%p, %.16g\n", o,
             nodo->valor->val.numerico);
#endif
      o->marca = 0;
    }
    if (nodo->valor->tipo == VALOR_CADENA) {
      o = latC_crear_cadena(mv, nodo->valor->val.cadena);
#if DEPURAR_MEM
      printf("NODO_VALOR.VALOR_CADENA:%p, %s\n", o, nodo->valor->val.cadena);
#endif
      o->marca = 0;
    }
    dbc(LOAD_CONST, 0, 0, o, nodo->nlin, nodo->ncol, mv->nombre_archivo);
    /* debug eliminado: LOAD_CONST emitido */
  } break;
  case NODO_IDENTIFICADOR: {
    // nombre del identificador
    lat_objeto *o = latC_crear_cadena(mv, nodo->valor->val.cadena);
    o->marca = 0;
    o->esconst = nodo->valor->esconst;
    dbc(LOAD_NAME, 0, 0, o, nodo->nlin, nodo->ncol, mv->nombre_archivo);
    /* debug eliminado: LOAD_NAME emitido */
  } break;
  case NODO_ASIGNACION: {
    /* debug eliminado: asign LHS tipo */
    if (nodo->der && nodo->der->tipo == NODO_IDENTIFICADOR && nodo->der->valor) {
      /* debug eliminado: asign LHS nombre */
    }
    // Para asignación simple: evaluar RHS y almacenar en el nombre LHS
    if (nodo->izq->tipo == NODO_FUNCION_PARAMETROS) {
      /* debug eliminado: RHS FUNCION_PARAMETROS antes de pn */
      pn(mv, nodo->izq);
      /* debug eliminado: despues de pn RHS FUNCION_PARAMETROS */
    } else if (nodo->der->tipo == NODO_ATRIBUTO) {
      /* debug eliminado: LHS ATRIBUTO antes de pn */
      pn(mv, nodo->izq);
      /* debug eliminado: despues de pn RHS */
      pn(mv, nodo->der->izq);
      /* debug eliminado: despues de pn ATRIBUTO objeto */
      lat_objeto *o = latC_crear_cadena(mv, nodo->der->der->valor->val.cadena);
      o->marca = 0;
      o->esconst = nodo->der->der->valor->esconst;
      dbc(STORE_ATTR, 0, 0, o, nodo->der->der->nlin, nodo->der->der->ncol,
          mv->nombre_archivo);
      /* debug eliminado: STORE_ATTR emitido */
    } else if (nodo->der->tipo == NODO_LISTA_OBTENER_ELEMENTO) {
      // Desazucar: X[Y] op= Z ya viene como NODO_ASIGNACION con RHS evaluando (X[Y] op Z).
      // Para almacenar, evaluamos RHS y luego el destino (objeto y subíndice) para emitir STORE_SUBSCR.
      /* debug eliminado: LHS LISTA_OBTENER_ELEMENTO y RHS tipo */
      pn(mv, nodo->izq);
      /* debug eliminado: despues de pn RHS */
      // En NODO_LISTA_OBTENER_ELEMENTO: izq = índice, der = objeto
      ast *lhs = nodo->der;
      if (lhs->der) {
        pn(mv, lhs->der);
        /* debug eliminado: despues de pn LHS objeto */
      } else {
        /* debug eliminado: LHS objeto NULL */
      }
      if (lhs->izq) {
        pn(mv, lhs->izq);
        /* debug eliminado: despues de pn LHS indice */
      } else {
        /* debug eliminado: LHS indice NULL */
      }
      dbc(STORE_SUBSCR, 0, 0, NULL, mv->nlin, mv->ncol, mv->nombre_archivo);
      /* debug eliminado: STORE_SUBSCR emitido */
    } else {
      // Evaluar RHS (valor a asignar) y usar nombre LHS como destino
      /* debug eliminado: RHS tipo */
      pn(mv, nodo->izq);
      /* debug eliminado: despues de pn RHS */
      /* debug eliminado: antes STORE_NAME */
      const char *lhs_name = (nodo->der && nodo->der->valor && nodo->der->valor->val.cadena)
                                 ? nodo->der->valor->val.cadena
                                 : "__invalid__";
      int lhs_nlin = nodo->der ? nodo->der->nlin : 1;
      int lhs_ncol = nodo->der ? nodo->der->ncol : 1;
      int lhs_esconst = (nodo->der && nodo->der->valor) ? nodo->der->valor->esconst : 0;
      /* debug eliminado: LHS nombre NULL */
      lat_objeto *o = latC_crear_cadena(mv, lhs_name);
      /* debug eliminado: objeto STORE_NAME */
      o->marca = 0;
      o->esconst = lhs_esconst;
      /* debug eliminado: objeto STORE_NAME campos */
      /* debug eliminado: llamando dbc STORE_NAME */
      dbc(STORE_NAME, 0, 0, o, lhs_nlin, lhs_ncol, mv->nombre_archivo);
      /* debug eliminado: STORE_NAME emitido */
    }
  } break;
  case NODO_GLOBAL: {
    dbc(SET_GLOBAL, 0, 0, NULL, mv->nlin, mv->ncol, mv->nombre_archivo);
    pn(mv, nodo->izq);
    dbc(SET_LOCAL, 0, 0, NULL, mv->nlin, mv->ncol, mv->nombre_archivo);
  } break;
  case NODO_MAS_UNARIO: {
    pn(mv, nodo->izq);
  } break;
  case NODO_MENOS_UNARIO: {
    pn(mv, nodo->izq);
    dbc(UNARY_MINUS, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_INC: {
    lat_objeto *o = latC_crear_cadena(mv, nodo->izq->valor->val.cadena);
    o->marca = 0;
    o->esconst = nodo->izq->valor->esconst;
    dbc(OP_INC, 0, 0, o, nodo->izq->nlin, nodo->izq->ncol, mv->nombre_archivo);
  } break;
  case NODO_DEC: {
    lat_objeto *o = latC_crear_cadena(mv, nodo->izq->valor->val.cadena);
    o->marca = 0;
    o->esconst = nodo->izq->valor->esconst;
    dbc(OP_DEC, 0, 0, o, nodo->nlin, nodo->ncol, mv->nombre_archivo);
  } break;
  case NODO_SUMA: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(BINARY_ADD, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_RESTA: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(BINARY_SUB, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_POTENCIA: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(BINARY_POW, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_MULTIPLICACION: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(BINARY_MUL, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_DIVISION: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(BINARY_DIV, 0, 0, NULL, nodo->der->nlin, nodo->der->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_MODULO: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(BINARY_MOD, 0, 0, NULL, nodo->der->nlin, nodo->der->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_MAYOR_QUE: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(OP_GT, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_MAYOR_IGUAL: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(OP_GE, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_MENOR_QUE: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(OP_LT, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_MENOR_IGUAL: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(OP_LE, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_IGUALDAD: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(OP_EQ, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_REGEX: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(OP_REGEX, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_DESIGUALDAD: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(OP_NEQ, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_Y: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(OP_AND, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_O: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(OP_OR, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_NO: {
    pn(mv, nodo->izq);
    dbc(OP_NOT, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_CONCATENAR: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(CONCAT, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_SI: {
    nodo_si *nIf = ((nodo_si *)nodo);
    // Validar que la condición no sea una asignación accidental ('=')
    // En caso de que la condición sea un NODO_ASIGNACION sin lado derecho,
    // es probable que se haya usado '=' en lugar de '==' en la condición.
    if (nIf->cond && nIf->cond->tipo == NODO_ASIGNACION &&
        nIf->cond->der == NULL) {
      latC_error(mv, "Se esta haciendo una asignacion '=', en lugar "
                     "de una comparacion '=='");
    }
    pn(mv, nIf->cond);
    temp[0] = i;
    dbc(NOP, 0, 0, NULL, 0, 0, mv->nombre_archivo);
    pn(mv, nIf->entonces);
    temp[1] = i;
    dbc(NOP, 0, 0, NULL, 0, 0, mv->nombre_archivo);
    temp[2] = i;
    if (nIf->_sino) {
      pn(mv, nIf->_sino);
    }
    temp[3] = i;
    codigo[temp[0]] = latMV_bytecode_crear(POP_JUMP_IF_FALSE, (temp[2] - 1), 0,
                                           NULL, nIf->cond->nlin,
                                           nIf->cond->ncol, mv->nombre_archivo);
    codigo[temp[1]] = latMV_bytecode_crear(JUMP_ABSOLUTE, (temp[3] - 1), 0,
                                           NULL, nIf->cond->nlin,
                                           nIf->cond->ncol, mv->nombre_archivo);
  } break;
  case NODO_ELEGIR: {
    ast *nSi = transformar_elegir(nodo);
    pn(mv, nSi);
    liberar_elegir(nSi);
  } break;
  case NODO_MIENTRAS: {
    mv->enBucle++;
    temp[0] = i;
    pn(mv, nodo->izq); // condicion
    temp[1] = i;
    dbc(NOP, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol, mv->nombre_archivo);
    if (encontrar_romper(nodo->der)) {
      lat_bytecode *code_tmp =
          latM_asignar(mv, sizeof(lat_bytecode) * MAX_BYTECODE_FUNCTION);
      int tmp_i = ast_analizar(mv, nodo->der, code_tmp, 0); // stmts
      mv->goto_break[mv->enBucle] = tmp_i + i;              // 16
      latM_liberar(mv, code_tmp);
    }
    // if (encontrar_continuar(nodo->der)) {
    //     lat_bytecode *code_tmp = latM_asignar(mv,
    //     sizeof(lat_bytecode) * MAX_BYTECODE_FUNCTION); int tmp_i =
    //     ast_analizar(mv, nodo->der, code_tmp, 0);  // stmts
    //     mv->goto_continue[mv->enBucle] = tmp_i + i; // 16
    //     latM_liberar(mv, code_tmp);
    // }
    pn(mv, nodo->der); // stmts
    dbc(JUMP_ABSOLUTE, (temp[0] - 1), 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
    codigo[temp[1]] = latMV_bytecode_crear(POP_JUMP_IF_FALSE, (i - 1), 0, NULL,
                                           nodo->izq->nlin, nodo->izq->ncol,
                                           mv->nombre_archivo);
    mv->enBucle--;
  } break;
  case NODO_REPETIR: {
    mv->enBucle++;
    temp[0] = i;
    pn(mv, nodo->izq);
    if (encontrar_romper(nodo->der)) {
      lat_bytecode *code_tmp =
          latM_asignar(mv, sizeof(lat_bytecode) * MAX_BYTECODE_FUNCTION);
      int tmp_i = ast_analizar(mv, nodo->der, code_tmp, 0);
      mv->goto_break[mv->enBucle] = tmp_i + i;
      latM_liberar(mv, code_tmp);
    }
    pn(mv, nodo->der);
    temp[1] = i;
    dbc(NOP, 0, 0, NULL, 0, 0, mv->nombre_archivo);
    codigo[temp[1]] = latMV_bytecode_crear(
        POP_JUMP_IF_FALSE, (temp[0] - 1), 0, NULL, nodo->izq->izq->nlin,
        nodo->izq->izq->ncol, mv->nombre_archivo);
    mv->enBucle--;
  } break;
  case NODO_FUNCION_LLAMADA: {
    // argumentos
    if (nodo->der) {
      pn(mv, nodo->der);
    }
    // nombre fun
    pn(mv, nodo->izq);
    int num_args = contar_num_parargs(nodo->der, NODO_FUNCION_ARGUMENTOS);
    dbc(CALL_FUNCTION, num_args, 0, NULL, mv->nlin, mv->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_RETORNO: {
    pn(mv, nodo->izq);
    int num_args = contar_num_parargs(nodo->izq, NODO_FUNCION_ARGUMENTOS);
    dbc(RETURN_VALUE, num_args == 0 ? 1 : num_args, 0, NULL, nodo->izq->nlin,
        nodo->izq->ncol, mv->nombre_archivo);
  } break;
  case NODO_ROMPER: {
    if (mv->enBucle <= 0) {
      char *info = malloc(MAX_INPUT_SIZE);
      snprintf(info, MAX_INPUT_SIZE, LAT_ERROR_FMT, mv->nombre_archivo,
               mv->nlin, mv->ncol, "Comando \"romper\" esta fuera de un bucle");
      fprintf(stderr, "%s\n", info);
    }
    dbc(JUMP_ABSOLUTE, mv->goto_break[mv->enBucle], 0, NULL, mv->nlin, mv->ncol,
        mv->nombre_archivo);
  } break;
  // case NODO_CONTINUAR: {
  //     if (mv->enBucle <= 0) {
  //         char *info = malloc(MAX_INPUT_SIZE);
  //         snprintf(info, MAX_INPUT_SIZE, LAT_ERROR_FMT,
  //         mv->nombre_archivo, mv->nlin, mv->ncol, "Comando
  //         \"continuar\" esta fuera de un bucle"); fprintf(stderr,
  //         "%s\n", info);
  //     }
  //     dbc(JUMP_ABSOLUTE, mv->goto_continue[mv->enBucle], 0, NULL,
  //     mv->nlin, mv->ncol,
  //         mv->nombre_archivo);
  // } break;
  case NODO_ETIQUETA: {
    lat_objeto *o = latC_crear_cadena(mv, nodo->izq->valor->val.cadena);
    o->nombre = nodo->izq->valor->val.cadena;
    o->tipo = T_LABEL;
    o->jump_label = i; // hacia qué instruccion vamos a saltar
    // dbc(STORE_LABEL, 0, 0, o, nodo->nlin, nodo->ncol,
    // mv->nombre_archivo);
    lat_objeto *val = latO_clonar(mv, o);
    // latMV_set_symbol(mv, o, val);
    lat_objeto *ctx = mv->label_ctx;
    char *str_name = latC_checar_cadena(mv, o);
    lat_objeto *oldVal = (lat_objeto *)latO_obtener_contexto(mv, ctx, str_name);
    if (oldVal != NULL && oldVal->tipo == T_LABEL) {
      latC_error(mv, "La etiqueta '%s' ya existe!", str_name);
    }
    latO_asignar_ctx(mv, ctx, str_name, val);
  } break;
  case NODO_IR: {
    // if (si no encuentra etiqueta) {}
    lat_objeto *o = latC_crear_cadena(mv, nodo->izq->valor->val.cadena);
    o->nombre = nodo->izq->valor->val.cadena;
    dbc(JUMP_LABEL, 0, 0, o, nodo->nlin, nodo->ncol, mv->nombre_archivo);
  } break;
  case NODO_FUNCION_ARGUMENTOS: {
    if (nodo->izq) {
      pn(mv, nodo->izq);
    }
    if (nodo->der) {
      pn(mv, nodo->der);
    }
  } break;
  case NODO_ATRIBUTO: {
    pn(mv, nodo->izq);
    lat_objeto *o = latC_crear_cadena(mv, nodo->der->valor->val.cadena);
    o->marca = 0;
    o->esconst = nodo->der->valor->esconst;
    dbc(LOAD_ATTR, 0, 0, o, nodo->der->nlin, nodo->der->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_FUNCION_PARAMETROS: {
    if (nodo->izq) {
      if (nodo->izq->tipo == NODO_VAR_ARGS) {
        int num_params = contar_num_parargs(nodo, NODO_FUNCION_PARAMETROS);
        dbc(OP_VAR_ARGS, (num_params - 1), 0, NULL, mv->nlin, mv->ncol,
            mv->nombre_archivo);
      } else {
        lat_objeto *o = latC_crear_cadena(mv, nodo->izq->valor->val.cadena);
        o->marca = 0;
        o->esconst = nodo->izq->valor->esconst;
        dbc(STORE_NAME, 0, 0, o, nodo->izq->nlin, nodo->izq->ncol,
            mv->nombre_archivo);
      }
    }
    if (nodo->der) {
      pn(mv, nodo->der);
    }
  } break;
  case NODO_FUNCION_USUARIO: {
    nodo_funcion *fun = (nodo_funcion *)nodo;
    funcion_codigo = (lat_bytecode *)latM_asignar(
        mv, sizeof(lat_bytecode) * MAX_BYTECODE_FUNCTION);
#if DEPURAR_MEM
    printf("NODO_FUNCION_USUARIO.funcion_codigo: %p\n", funcion_codigo);
#endif
    fi = 0;
    // procesar lista de params
    bool es_vararg = false;
    if (fun->params) {
      fpn(mv, fun->params);
      es_vararg = encontrar_vararg(fun->params);
    }
    // procesar instrucciones
    fpn(mv, fun->stmts);
    fdbc(RETURN_VALUE, 0, 0, latO_nulo, fun->nombre->nlin, fun->nombre->ncol,
         mv->nombre_archivo);
    lat_objeto *f = latC_crear_funcion(mv, funcion_codigo, fi + 1);
    f->marca = 0;
    f->es_vararg = es_vararg;
    dbc(MAKE_FUNCTION, fi + 1, 0, f, fun->nombre->nlin, fun->nombre->ncol,
        mv->nombre_archivo);
    funcion_codigo = NULL;
    fi = 0;
    lat_objeto *o = latC_crear_cadena(mv, fun->nombre->valor->val.cadena);
    o->marca = 0;
    f->nparams = contar_num_parargs(fun->params, NODO_FUNCION_PARAMETROS);
    f->nombre = strdup(fun->nombre->valor->val.cadena);
    dbc(STORE_NAME, 0, 0, o, nodo->nlin, nodo->ncol, mv->nombre_archivo);
    if (mv->enClase) {
      // Insertar el método en el diccionario de clase __clase_dict__
      lat_objeto *dict_var = latC_crear_cadena(mv, "__clase_dict__");
      dict_var->marca = 0;
      dict_var->esconst = false;
      // 1) Cargar el diccionario en la pila
      dbc(LOAD_NAME, 0, 0, dict_var, nodo->der->nlin, nodo->der->ncol,
          mv->nombre_archivo);
      // 2) Cargar el valor (función recién creada) en la pila
      dbc(LOAD_NAME, 0, 0, o, nodo->der->nlin, nodo->der->ncol,
          mv->nombre_archivo);
      // 3) Cargar la clave (nombre del método) como constante en la pila
      lat_objeto *clave = latO_clonar(mv, o);
      clave->marca = 0;
      dbc(LOAD_CONST, 0, 0, clave, nodo->der->nlin, nodo->der->ncol,
          mv->nombre_archivo);
      // 4) Guardar en el diccionario: dic (tope previo), val, key
      dbc(STORE_MAP, 0, 0, NULL, nodo->der->nlin, nodo->der->ncol,
          mv->nombre_archivo);
    }
    if (!strcmp(f->nombre, "anonima")) {
      lat_objeto *anon = latO_clonar(mv, o);
      anon->marca = 0;
      dbc(LOAD_NAME, 0, 0, anon, nodo->nlin, nodo->ncol, mv->nombre_archivo);
    }
    if (!strcmp(getstr(getCadena(o)), "menu")) {
      mv->global->menu = true;
    }
  } break;
  case NODO_LISTA: {
    int es_vararg = 0;
    int nparams = 0;
    if (nodo->izq) {
      pn(mv, nodo->izq);
      nparams = contar_num_parargs(nodo->izq, NODO_LISTA_AGREGAR_ELEMENTO);
      es_vararg = encontrar_load_vararg(nodo->izq);
    }
    dbc(BUILD_LIST, nparams, es_vararg, NULL, mv->nlin, mv->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_LISTA_AGREGAR_ELEMENTO: {
    if (nodo->izq) {
      pn(mv, nodo->izq);
    }
    if (nodo->der) {
      pn(mv, nodo->der);
    }
  } break;
  case NODO_LISTA_ASIGNAR_ELEMENTO: {
    nodo_lista_elem *nl = (nodo_lista_elem *)nodo;
    pn(mv, nl->exp);
    pn(mv, nl->id);
    pn(mv, nl->pos);
    dbc(STORE_SUBSCR, 0, 0, NULL, mv->nlin, mv->ncol, mv->nombre_archivo);
  } break;
  case NODO_LISTA_OBTENER_ELEMENTO: {
    pn(mv, nodo->izq);
    pn(mv, nodo->der);
    dbc(BINARY_SUBSCR, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_DICCIONARIO: {
    int nparams = 0;
    if (nodo->izq) {
      nparams = contar_num_parargs(nodo->izq, NODO_DICC_AGREGAR_ELEMENTO);
    }
    dbc(BUILD_MAP, nparams, 0, NULL, mv->nlin, mv->ncol, mv->nombre_archivo);
    if (nodo->izq) {
      pn(mv, nodo->izq);
    }
  } break;
  case NODO_DICC_AGREGAR_ELEMENTO: {
    if (nodo->der) {
      pn(mv, nodo->der);
    }
    if (nodo->izq) {
      pn(mv, nodo->izq);
    }
    dbc(STORE_MAP, 0, 0, NULL, nodo->izq->nlin, nodo->izq->ncol,
        mv->nombre_archivo);
  } break;
  case NODO_DICC_ELEMENTO: {
    if (nodo->der) {
      pn(mv, nodo->der);
    }
    if (nodo->izq) {
      pn(mv, nodo->izq);
    }
  } break;
  case NODO_BLOQUE: {
    if (nodo->izq) {
      pn(mv, nodo->izq);
    }
    if (nodo->der) {
      pn(mv, nodo->der);
    }
  } break;
  case NODO_NULO:
    break;
  case NODO_VAR_ARGS: {
    ;
  } break;
  case NODO_LOAD_VAR_ARGS: {
    dbc(LOAD_VAR_ARGS, 0, 0, NULL, mv->nlin, mv->ncol, mv->nombre_archivo);
  } break;
  case NODO_CLASE: {
    mv->enClase = true;
    nodo_clase *fun = (nodo_clase *)nodo;
    funcion_codigo = (lat_bytecode *)latM_asignar(
        mv, sizeof(lat_bytecode) * MAX_BYTECODE_FUNCTION);
#if DEPURAR_MEM
    printf("NODO_CLASE.funcion_codigo: %p\n", funcion_codigo);
#endif
    fi = 0;
    // procesar lista de params
    bool es_vararg = false;
    // crear diccionario vacio para métodos ANTES de procesar sentencias
    // (para que NODO_FUNCION_USUARIO pueda insertar métodos en __clase_dict__)
    dbc(BUILD_MAP, 0, 0, NULL, mv->nlin, mv->ncol, mv->nombre_archivo);
    lat_objeto *dict_var = latC_crear_cadena(mv, "__clase_dict__");
    dict_var->marca = 0;
    dict_var->esconst = false;
    dbc(STORE_NAME, 0, 0, dict_var, nodo->der->nlin, nodo->der->ncol,
        mv->nombre_archivo);

    // procesar instrucciones de la clase (métodos/constructor)
    fpn(mv, fun->stmts);

    // cargar el diccionario para retornarlo como resultado del bytecode de clase
    dbc(LOAD_NAME, 0, 0, dict_var, nodo->nlin, nodo->ncol, mv->nombre_archivo);
    fdbc(RETURN_VALUE, 1, 0, dict_var, fun->nombre->nlin, fun->nombre->ncol,
         mv->nombre_archivo);
    lat_objeto *f = latC_crear_funcion(mv, funcion_codigo, fi + 1);
    f->marca = 0;
    f->es_vararg = es_vararg;
    /* Attach class name and optional base name via meta */
    lat_class_meta *cm = (lat_class_meta *)latM_asignar(mv, sizeof(lat_class_meta));
    cm->func = f;
    cm->base_name = NULL;
    if (fun->base && fun->base->valor && fun->base->valor->tipo == VALOR_CADENA) {
      cm->base_name = strdup(fun->base->valor->val.cadena);
    } else if (fun->base && fun->base->tipo == NODO_IDENTIFICADOR && fun->base->valor && fun->base->valor->tipo == VALOR_CADENA) {
      cm->base_name = strdup(fun->base->valor->val.cadena);
    }
    dbc(MAKE_CLASS, fi + 1, 0, cm, fun->nombre->nlin, fun->nombre->ncol,
        mv->nombre_archivo);
    funcion_codigo = NULL;
    fi = 0;
    lat_objeto *o = latC_crear_cadena(mv, fun->nombre->valor->val.cadena);
    o->marca = 0;
    f->es_clase = true;
    f->nombre = strdup(fun->nombre->valor->val.cadena);
    dbc(STORE_NAME, 0, 0, o, nodo->nlin, nodo->ncol, mv->nombre_archivo);
    mv->enClase = false;
  } break;

    /* Generación de bytecode para operaciones con clases */

  case NODO_MI: {
    // Cargar referencia a la instancia actual (equivalente a 'this' o 'self')
    // Dentro de un método de clase, 'mi' se refiere a la instancia actual
    // Se carga como una variable del contexto actual
    lat_objeto *mi_str = latC_crear_cadena(mv, "mi");
    mi_str->marca = 0;
    mi_str->esconst = false;
    dbc(LOAD_NAME, 0, 0, mi_str, nodo->nlin, nodo->ncol, mv->nombre_archivo);
  } break;

  case NODO_SUPER: {
    // Llamar al constructor de la clase padre (herencia)
    // super() se usa dentro del constructor de una clase hija para
    // inicializar la parte de la clase padre
    // Ejemplo: super(nombre) llama al constructor del padre con 'nombre'
    int num_args = 0;
    if (nodo->izq) { // Si hay argumentos
      num_args = contar_num_parargs(nodo->izq, NODO_FUNCION_ARGUMENTOS);
      pn(mv, nodo->izq); // Evaluar los argumentos
    }
    dbc(LOAD_SUPER, num_args, 0, NULL, nodo->nlin, nodo->ncol,
        mv->nombre_archivo);
  } break;

  case NODO_NUEVA: {
    // Crear nueva instancia de clase
    // nueva Clase(args) crea un objeto de la clase especificada
    // Ejemplo: persona = nueva Persona("Juan", 25)
    nodo_nueva *nueva = (nodo_nueva *)nodo;

    // 1. Cargar el nombre de la clase
    pn(mv, nueva->nombre);

    // 2. Evaluar argumentos del constructor
    int num_args = 0;
    if (nueva->args) {
      num_args = contar_num_parargs(nueva->args, NODO_FUNCION_ARGUMENTOS);
      pn(mv, nueva->args); // Evaluar los argumentos
    }

    // 3. Generar opcode NEW_INSTANCE
    dbc(NEW_INSTANCE, num_args, 0, NULL, nodo->nlin, nodo->ncol,
        mv->nombre_archivo);
  } break;

  default:
    printf("ERROR nodo->tipo:%i\n", nodo->tipo);
    return 0;
  }
  return i; // instruccion actual
}

void mostrar_bytecode(lat_mv *mv, lat_bytecode *codigo) {
  lat_bytecode *inslist = codigo;
  lat_bytecode cur;
  lat_objeto *o = NULL;
  int pos;
  for (pos = 0, cur = inslist[pos]; cur.ins && cur.ins != HALT;
       cur = inslist[++pos]) {
    char *buffer = NULL;
    printf("%i\t%i\t", cur.nlin, pos);
    switch (cur.ins) {
    case HALT:
      return;
      break;
    case NOP:
    case UNARY_MINUS:
    case BINARY_ADD:
    case BINARY_SUB:
    case BINARY_POW:
    case BINARY_MUL:
    case BINARY_DIV:
    case BINARY_MOD:
    case OP_GT:
    case OP_GE:
    case OP_LT:
    case OP_LE:
    case OP_EQ:
    case OP_REGEX:
    case OP_NEQ:
    case OP_AND:
    case OP_OR:
    case OP_NOT:
    case CONCAT:
    case PUSH_CTX:
    case POP_CTX:
    case RETURN_VALUE:
    case STORE_SUBSCR:
    case BINARY_SUBSCR:
    case STORE_MAP:
    case STORE_ATTR:
    case SET_GLOBAL:
    case SET_LOCAL:
    case OP_POP:
      printf("%s\n", latMV_bytecode_nombre(cur.ins));
      break;
    case CALL_FUNCTION: {
      printf("CALL_FUNCTION\t%i\n", cur.a);
    } break;
    case LOAD_ATTR: {
      o = (lat_objeto *)cur.meta;
      buffer = latC_astring(mv, o);
      printf("LOAD_ATTR\t(%s)\n", buffer);
      free(buffer);
    } break;
    case LOAD_CONST: {
      o = (lat_objeto *)cur.meta;
      buffer = latC_astring(mv, o);
      printf("LOAD_CONST\t(%s)\n", buffer);
      free(buffer);
    } break;
    case LOAD_NAME: {
      o = (lat_objeto *)cur.meta;
      buffer = latC_astring(mv, o);
      printf("LOAD_NAME\t(%s)\n", buffer);
      free(buffer);
    } break;
    case STORE_NAME: {
      o = (lat_objeto *)cur.meta;
      buffer = latC_astring(mv, o);
      printf("STORE_NAME\t(%s)\n", buffer);
      free(buffer);
    } break;
    case STORE_LABEL: {
      o = (lat_objeto *)cur.meta;
      buffer = latC_astring(mv, o);
      printf("STORE_LABEL\t(%s)\t%i\n", buffer, o->jump_label);
      free(buffer);
    } break;
    case JUMP_ABSOLUTE: {
      printf("JUMP_ABSOLUTE\t(%i)\n", (cur.a + 1));
    } break;
    case JUMP_LABEL: {
      o = (lat_objeto *)cur.meta;
      buffer = latC_astring(mv, o);
      printf("JUMP_LABEL\t(%s)\t%i\n", buffer, o->jump_label);
      free(buffer);
    } break;
    case POP_JUMP_IF_FALSE: {
      printf("POP_JUMP_IF_FALSE\t(%i)\n", (cur.a + 1));
    } break;
    case POP_JUMP_IF_TRUE: {
      printf("POP_JUMP_IF_TRUE\t(%i)\n", (cur.a + 1));
    } break;
    case OP_INC: {
      o = (lat_objeto *)cur.meta;
      buffer = latC_astring(mv, o);
      printf("OP_INC\t(%s)\n", buffer);
      free(buffer);
    } break;
    case OP_DEC: {
      o = (lat_objeto *)cur.meta;
      buffer = latC_astring(mv, o);
      printf("OP_DEC\t(%s)\n", buffer);
      free(buffer);
    } break;
    case BUILD_LIST: {
      printf("BUILD_LIST\t%i\t%i\n", cur.a, cur.b);
    } break;
    case BUILD_MAP: {
      printf("BUILD_MAP\t%i\n", cur.a);
    } break;
    case MAKE_FUNCTION: {
      printf("\n-------------------------------------------\n");
      printf("MAKE_FUNCTION\n");
      printf("-------------------------------------------\n");
      o = (lat_objeto *)cur.meta;
      lat_funcion *fun = getFun(o);
      mostrar_bytecode(mv, fun->codigo);
    } break;
    case OP_VAR_ARGS: {
      printf("OP_VAR_ARGS\t%i\n", cur.a);
    } break;
    case LOAD_VAR_ARGS: {
      printf("LOAD_VAR_ARGS\n");
    } break;
    case OP_PUSH: {
      o = (lat_objeto *)cur.meta;
      buffer = latC_astring(mv, o);
      printf("OP_PUSH\t(%s)\n", buffer);
      free(buffer);
    } break;
    case MAKE_CLASS: {
      printf("\n-------------------------------------------\n");
      printf("MAKE_CLASS\n");
      printf("-------------------------------------------\n");
      o = (lat_objeto *)cur.meta;
      lat_class *fun = getClass(o);
      (void)fun; // Will be used when class bytecode is implemented
      // TODO: Implementar bytecode para clases
      // mostrar_bytecode(mv, fun->codigo);
    } break;
    case ADJUST_STACK: {
      printf("ADJUST_STACK\t%i\n", cur.a);
    } break;
    }
  }
  printf("-------------------------------------------\n");
}

void latD_lanzar(lat_mv *mv, int errcode) {
  if (mv->error) {
    mv->error->status = errcode;
    LAT_THROW(mv, mv->error);
  } else {
    mv->status = errcode;
    mv->error = NULL;
    exit(EXIT_FAILURE);
  }
}

LATINO_API void latC_error(lat_mv *mv, const char *fmt, ...) {
  char buffer[MAX_INPUT_SIZE];
  va_list args;
  va_start(args, fmt);
  vsprintf(buffer, fmt, args);
  va_end(args);
  char *info = malloc(MAX_INPUT_SIZE);
  if (strstr(buffer, "%") != NULL) {
    snprintf(info, MAX_INPUT_SIZE, LAT_ERROR_FMT, mv->nombre_archivo, mv->nlin,
             mv->ncol);
    latC_apilar(mv, latC_crear_cadena(mv, info));
    latC_apilar(mv, latC_crear_cadena(mv, buffer));
    str_concatenar(mv);
    lat_objeto *err = latC_desapilar(mv);
    fprintf(stderr, "%s\n", latC_astring(mv, err));
  } else {
    snprintf(info, MAX_INPUT_SIZE, LAT_ERROR_FMT, mv->nombre_archivo, mv->nlin,
             mv->ncol, buffer);
    fprintf(stderr, "%s\n", info);
  }
  // FIXME: Si esta en REPL que no finalice
  latD_lanzar(mv, LAT_ERRRUN);
}

LATINO_API int latC_llamar_funcion(lat_mv *mv, lat_objeto *func) {
  // printf("latC_llamar_funcion\n");
  struct lat_longjmp lj;
  lj.status = 0;
  lj.previo = mv->error;
  mv->error = &lj;
  LAT_TRY(mv, &lj, latMV_funcion_correr(mv, func););
  mv->error = lj.previo;
  return lj.status;
}

LATINO_API lat_objeto *latC_analizar(lat_mv *mv, ast *nodo) {
  lat_bytecode *codigo =
      latM_asignar(mv, sizeof(lat_bytecode) * MAX_BYTECODE_FUNCTION);
#if DEPURAR_MEM
  printf("latC_analizar.codigo: %p\n", codigo);
#endif
  /* debug eliminado: latC_analizar inicio */
  int i = ast_analizar(mv, nodo, codigo, 0);
  /* debug eliminado: latC_analizar ast_analizar completo */
  dbc(HALT, 0, 0, NULL, 0, 0, mv->nombre_archivo);
#if DEPURAR_AST
  /* debug deshabilitado: dump de bytecode */
#endif
  lat_bytecode *nuevo_codigo = latM_asignar(mv, sizeof(lat_bytecode) * (i + 1));
  memcpy(nuevo_codigo, codigo, sizeof(lat_bytecode) * (i + 1));
#if DEPURAR_MEM
  printf("latC_analizar.nuevo_codigo: %p\n", nuevo_codigo);
#endif
  /* debug eliminado: latC_analizar nuevo_codigo */
  latM_liberar(mv, codigo);
  lat_objeto *fun = latC_crear_funcion(mv, nuevo_codigo, i);
  fun->marca = 0;
  fun->nombre = "dummy";
  /* debug eliminado: latC_analizar función creada */
  return fun;
}
