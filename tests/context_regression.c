/* Prueba interna: incluye latmv.c para ejercitar push/pop y cache reales.
 * El linker descarta funciones no usadas. No inicia una VM completa.
 * latC_error se sustituye por un fallo inmediato; GC=0 es el alcance probado.
 */
#include "../src/latmv.c"
#include <stdarg.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr,"FAIL %d: %s\n",__LINE__,#x); exit(2); } } while (0)
LATINO_API void latC_error(lat_mv *mv, const char *fmt, ...) {
    (void)mv;
    va_list ap; va_start(ap,fmt); vfprintf(stderr,fmt,ap); va_end(ap);
    exit(3);
}
static int cache_vacio(lat_mv *mv) {
    for (int i=0;i<VAR_CACHE_SIZE;++i)
        if(mv->var_cache[i].name || mv->var_cache[i].value) return 0;
    return 1;
}
static void probar_limpieza(void) {
    lat_mv mv={0};
    lat_objeto *ctx=latO_contexto_crear(&mv);
    lat_objeto value={0}; setNumerico(&value,7);
    latO_asignar_ctx(&mv,ctx,"numero",&value);
    size_t baseline=mv.memoria_usada;
    for(int i=0;i<500;++i) {
        lat_objeto *copy=latO_clonar(&mv,ctx);
        latO_destruir(&mv,copy);
        if(mv.memoria_usada!=baseline) {
            fprintf(stderr,"Fuga de contexto: antes=%zu despues=%zu ciclo=%d\n",
                    baseline,mv.memoria_usada,i);
            exit(2);
        }
        CHECK(getNumerico(latO_obtener_contexto(&mv,ctx,"numero"))==7);
    }
    latO_destruir(&mv,ctx);
    CHECK(mv.memoria_usada==0);
}
int main(void) {
    probar_limpieza();
    CHECK(HABILITAR_GC == 0);
    lat_mv mv = {0};
    mv.contexto[0]=latO_contexto_crear(&mv);
    mv.contexto_actual=mv.contexto[0];
    lat_objeto numeric={0}; setNumerico(&numeric,42);
    latO_asignar_ctx(&mv,mv.contexto_actual,"numero",&numeric);
    lat_objeto null_value={0}; setNulo(&null_value,NULL);
    latO_asignar_ctx(&mv,mv.contexto_actual,"nulo",&null_value);
    lat_objeto boolean={0}; setLogico(&boolean,1);
    latO_asignar_ctx(&mv,mv.contexto_actual,"booleano",&boolean);

    /* Recursos compartidos propiedad de la prueba, no del envoltorio hash. */
    lista *shared_list=latL_crear(&mv);
    latL_agregar(&mv,shared_list,&numeric);
    lat_objeto list_value={0}; setLista(&list_value,shared_list);
    latO_asignar_ctx(&mv,mv.contexto_actual,"lista",&list_value);
    lat_funcion *shared_fun=latM_asignar(&mv,sizeof(*shared_fun));
    shared_fun->codigo=latM_asignar(&mv,sizeof(lat_bytecode));
    lat_objeto fun_value={0}; setFun(&fun_value,shared_fun);
    latO_asignar_ctx(&mv,mv.contexto_actual,"funcion",&fun_value);

    lat_objeto *parent_num=latO_obtener_contexto(&mv,mv.contexto[0],"numero");
    size_t baseline=mv.memoria_usada;
    for(int iteration=0;iteration<500;++iteration) {
        latMV_cache_insert(&mv,"numero",parent_num,0);
        apilar_contexto(&mv,NULL);
        CHECK(cache_vacio(&mv));
        CHECK(mv.ptrctx==1);
        lat_objeto *child_num=latO_obtener_contexto(&mv,mv.contexto_actual,"numero");
        CHECK(child_num!=parent_num && getNumerico(child_num)==42);
        latMV_cache_insert(&mv,"numero",child_num,1);
        CHECK(latMV_cache_lookup(&mv,"numero",1)==child_num);
        setNumerico(child_num,iteration);
        apilar_contexto(&mv,NULL);
        CHECK(cache_vacio(&mv) && mv.ptrctx==2);
        lat_objeto *nested=latO_obtener_contexto(&mv,mv.contexto_actual,"numero");
        CHECK(nested!=child_num && getNumerico(nested)==iteration);
        latMV_cache_insert(&mv,"numero",nested,2);
        desapilar_contexto(&mv);
        CHECK(cache_vacio(&mv) && mv.contexto[2]==NULL);
        CHECK(getNumerico(child_num)==iteration);
        CHECK(getLista(latO_obtener_contexto(&mv,mv.contexto_actual,"lista"))==shared_list);
        CHECK(getFun(latO_obtener_contexto(&mv,mv.contexto_actual,"funcion"))==shared_fun);
        latMV_cache_insert(&mv,"numero",child_num,1);
        desapilar_contexto(&mv);
        CHECK(cache_vacio(&mv) && mv.contexto[1]==NULL && mv.ptrctx==0);
        CHECK(getNumerico(parent_num)==42);
        CHECK(shared_list->longitud==1 && shared_list->primero->valor==&numeric);
        CHECK(shared_fun->codigo!=NULL);
        CHECK(mv.memoria_usada==baseline);
    }
    latO_destruir(&mv,mv.contexto[0]);
    CHECK(shared_list->longitud==1);
    shared_fun->codigo[0].ins=HALT; /* El contexto no debe liberar este recurso. */
    latL_destruir(&mv,shared_list);
    latM_liberar(&mv,shared_fun->codigo);
    latM_liberar(&mv,shared_fun);
    latH_limpiar(&mv,NULL);
    hash_map *empty=latH_crear(&mv);
    empty->buckets[0]=latL_crear(&mv);
    latH_limpiar(&mv,empty);
    CHECK(mv.memoria_usada==0);
    puts("OK: 500 ciclos anidados, cache invalidado, recursos compartidos conservados");
    return 0;
}
