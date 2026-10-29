/* Ejercita el destructor real con una VM minima, sin cargar bibliotecas. */
#include "latino.h"
#include "latmem.h"
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
static lat_mv *observada;
static void liberar_observando(void *p) {
    if (p == observada) {
        if (observada->memoria_usada != 0) {
            fprintf(stderr, "FAIL: cierre conserva %zu bytes\n", observada->memoria_usada);
            exit(2);
        }
        observada = NULL;
    }
    free(p);
}
#define free liberar_observando
#include "../src/latmv.c"
#undef free
LATINO_API void latC_error(lat_mv *mv, const char *fmt, ...) {
    (void)mv; va_list ap; va_start(ap,fmt); vfprintf(stderr,fmt,ap);
    va_end(ap); exit(3);
}
int main(void) {
    for (int i=0; i<500; ++i) {
        lat_mv *mv=calloc(1,sizeof(*mv));
        assert(mv);
        mv->global=latM_asignar(mv,sizeof(*mv->global));
        mv->global->argv=latC_crear_lista(mv,latL_crear(mv));
        mv->contexto[0]=latO_contexto_crear(mv);
        mv->label_ctx=latO_contexto_crear(mv);
        mv->pila=calloc(1,sizeof(lat_objeto));
        assert(mv->pila);
        /* Recursos compartidos: limpiar etiquetas no debe destruir la lista
           externa. Un ciclo vacio alterna con uno que contiene una entrada. */
        lat_mv externa={0};
        lat_objeto *compartido=latC_crear_lista(&externa,latL_crear(&externa));
        if (i%2) latO_asignar_ctx(mv,mv->label_ctx,"referencia",compartido);
        observada=mv;
        latC_destruir_mv(mv);
        assert(observada==NULL);
        assert(getLista(compartido)->longitud==0);
        latO_destruir(&externa,compartido);
        assert(externa.memoria_usada==0);
    }
    puts("OK: 500 cierres, contexto vacio y poblado, recurso compartido conservado");
    return 0;
}
