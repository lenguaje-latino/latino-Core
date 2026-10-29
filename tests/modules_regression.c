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
static int efectos;
static void accion(lat_mv *mv) { (void)mv; ++efectos; }
static const lat_CReg funciones[]={{"accion",accion,0},{NULL,NULL,0}};
static const lat_CReg vacio[]={{NULL,NULL,0}};
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
        latC_abrir_liblatino(mv,"modulo",funciones);
        latC_abrir_liblatino(mv,"vacio",vacio);
        lat_objeto *mod=latO_obtener_contexto(mv,mv->contexto[0],"modulo");
        hash_map *map=getDic(mod);
        size_t base=mv->memoria_usada;
        latC_abrir_liblatino(mv,"modulo",funciones);
        assert(base==mv->memoria_usada);
        lat_objeto *ctx=latO_clonar(mv,mv->contexto[0]);
        latO_destruir(mv,ctx);
        /* Destruir un envoltorio prestado no debe invalidar el modulo. */
        lat_objeto *alias=latO_crear(mv);
        setobj2obj(alias,mod);
        latO_destruir(mv,alias);
        lat_objeto *cf=latH_obtener(map,"accion");
        assert(cf && cf->tipo==T_CFUN);
        getCfun(cf)(mv);
        /* Perder el nombre original tampoco debe perder su propietario. */
        lat_objeto numero={0}; setNumerico(&numero,7);
        latO_asignar_ctx(mv,mv->contexto[0],"modulo",&numero);
        observada=mv;
        latC_destruir_mv(mv);
        assert(observada==NULL);
        assert(getLista(compartido)->longitud==0);
        latO_destruir(&externa,compartido);
        assert(externa.memoria_usada==0);
    }
    assert(efectos==500);
    puts("OK: 500 cierres, modulos vacios, reemplazos, alias y callbacks");
    return 0;
}
