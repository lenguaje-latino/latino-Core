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
int main(void) {
    const char *fuente="funcion doble(n)\n retorno n * 2\nfin\nfuncion calcular()\n retorno doble(21)\nfin\nresultado = calcular()\ntexto = \"compartida\"\n";
    for (int ciclo=0; ciclo<20; ++ciclo) {
        lat_mv *mv=latC_crear_mv();
        mv->nombre_archivo="regresion-constantes";
        mv->global->REPL=false;
        int estado=0;
        ast *arbol=latA_analizar_exp((char *)fuente,&estado);
        assert(arbol && estado==0);
        lat_objeto *programa=latC_analizar(mv,arbol);
        assert(latC_llamar_funcion(mv,programa)==0);
        lat_objeto *callback=latO_obtener_contexto(mv,mv->contexto[0],"calcular");
        assert(callback);
        for(int j=0;j<10;++j) {
            assert(latC_llamar_funcion(mv,callback)==0);
            lat_objeto *salida=latC_desapilar(mv);
            assert(latC_checar_numerico(mv,salida)==42);
            lat_objeto *resultado=latO_obtener_contexto(mv,mv->contexto[0],"resultado");
            assert(resultado && latC_checar_numerico(mv,resultado)==42);
        }
        /* Tabla internada: destruir un envoltorio no invalida otro igual. */
        lat_objeto *a=latC_crear_cadena(mv,"compartida");
        lat_objeto *b=latC_crear_cadena(mv,"compartida");
        assert(getCadena(a)==getCadena(b));
        latO_destruir(mv,a);
        assert(strcmp(latC_checar_cadena(mv,b),"compartida")==0);
        for(int k=0;k<600;++k) {
            char texto[40];snprintf(texto,sizeof texto,"cadena_%d",k);
            lat_objeto *t=latC_crear_cadena(mv,texto);
            latO_destruir(mv,t);
        }
        assert(strcmp(latC_checar_cadena(mv,b),"compartida")==0);
        latO_destruir(mv,b);
        latO_destruir(mv,programa);
        latA_destruir(arbol);
        observada=mv;
        latC_destruir_mv(mv);
        assert(observada==NULL);
    }
    puts("OK: compilacion real, 200 ejecuciones, cadenas compartidas y 20 cierres sin saldo");
    return 0;
}
