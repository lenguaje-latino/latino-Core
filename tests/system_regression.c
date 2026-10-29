#include "latino.h"
#include "latobj.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define EXIGIR(c)                                                              \
  do {                                                                         \
    if (!(c)) {                                                                \
      fprintf(stderr, "FALLO sistema %d: %s\n", __LINE__, #c);                 \
      exit(2);                                                                 \
    }                                                                          \
  } while (0)
static int fallo_cwd;
static char *cwd_probado(char *p, size_t n) {
  if (fallo_cwd) {
    errno = ENOENT;
    return NULL;
  } /* No escribir el buffer: reproduce el caso peligroso. */
  return getcwd(p, n);
}
#define getcwd cwd_probado
#include "../src/latsyslib.c"
#undef getcwd
int main(void) {
  lat_mv *mv = latC_crear_mv();
  EXIGIR(mv);
  fallo_cwd = 1;
  latSO_cwd(mv);
  EXIGIR(latC_desapilar(mv)->tipo == T_NULL && mv->ptrpila == 0);
  fallo_cwd = 0;
  latSO_cwd(mv);
  lat_objeto *o = latC_desapilar(mv);
  EXIGIR(o && o->tipo == T_STR && *latC_checar_cadena(mv, o));
  int invalidos[] = {-1, 0, 4, 2147483647};
  for (size_t i = 0; i < sizeof invalidos / sizeof *invalidos; ++i) {
    latOS_veriones(mv, invalidos[i]);
    EXIGIR(latC_desapilar(mv)->tipo == T_NULL && mv->ptrpila == 0);
  }
  for (int i = 1; i <= 3; ++i) {
    latOS_veriones(mv, i);
    o = latC_desapilar(mv);
    EXIGIR(o && o->tipo == T_NUMERIC);
  }
  size_t saldo = mv->memoria_usada;
  for (int vuelta = 0; vuelta < 200; ++vuelta) {
    latSO_cwd(mv);
    (void)latC_desapilar(mv);
    latOS_veriones(mv, 1);
    (void)latC_desapilar(mv);
  }
  EXIGIR(mv->memoria_usada == saldo);
  latC_destruir_mv(mv);
  puts("OK: getcwd fallido/satisfactorio y selectores de version "
       "validos/invalidos");
  return 0;
}
