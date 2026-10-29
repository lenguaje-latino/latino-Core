/* Registro real; sin iniciar la VM completa. Mismo aislamiento que contextos02. */
#include "../src/latmv.c"
#include <stdarg.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"FAIL %d: %s\n",__LINE__,#x); exit(2); } } while(0)
LATINO_API void latC_error(lat_mv *mv,const char *fmt,...) {
    (void)mv;va_list ap;va_start(ap,fmt);vfprintf(stderr,fmt,ap);va_end(ap);exit(3);
}
static int effects;
static void first(lat_mv *mv) {(void)mv;effects+=1;}
static void second(lat_mv *mv) {(void)mv;effects+=2;}
static const lat_CReg lib1[]={{"accion",first,0},{NULL,NULL,0}};
static const lat_CReg lib2[]={{"accion",second,1},{NULL,NULL,0}};
static const lat_CReg empty[]={{NULL,NULL,0}};
static void call_and_check(lat_objeto *o,lat_mv *mv,int increment,int params) {
    CHECK(o && o->tipo==T_CFUN && o->nparams==params);
    CHECK(strcmp(o->nombre,"accion")==0);
    int before=effects;getCfun(o)(mv);CHECK(effects==before+increment);
}
int main(void) {
    CHECK(HABILITAR_GC==0);
    lat_mv mv={0};mv.contexto[0]=latO_contexto_crear(&mv);
    mv.contexto_actual=mv.contexto[0];
    latC_abrir_liblatino(&mv,"",lib1);
    latC_abrir_liblatino(&mv,"modulo",lib1);
    latC_abrir_liblatino(&mv,"vacio",empty);
    hash_map *module=getDic(latO_obtener_contexto(&mv,mv.contexto[0],"modulo"));
    hash_map *empty_module=getDic(latO_obtener_contexto(&mv,mv.contexto[0],"vacio"));
    size_t baseline=mv.memoria_usada;
    for(int i=0;i<500;++i) {
        const lat_CReg *lib=i%2?lib1:lib2;
        int inc=i%2?1:2,params=i%2?0:1;
        latC_abrir_liblatino(&mv,"",lib);
        latC_abrir_liblatino(&mv,"modulo",lib);
        latC_abrir_liblatino(&mv,"vacio",empty);
        CHECK(getDic(latO_obtener_contexto(&mv,mv.contexto[0],"modulo"))==module);
        CHECK(module->longitud==1 && empty_module->longitud==0);
        call_and_check(latO_obtener_contexto(&mv,mv.contexto[0],"accion"),&mv,inc,params);
        call_and_check(latH_obtener(module,"accion"),&mv,inc,params);
        if(mv.memoria_usada!=baseline) {
            fprintf(stderr,"Registro FFI: antes=%zu despues=%zu ciclo=%d\n",baseline,mv.memoria_usada,i);return 2;
        }
    }
    latO_destruir(&mv,mv.contexto[0]);
    /* Los modulos creados por FFI pertenecen ahora al registro de la VM. */
    latH_liberar_modulos(&mv);
    CHECK(mv.memoria_usada==0);
    CHECK(effects==1500);
    puts("OK: FFI global y modular, reemplazos, modulo vacio y 1000 llamadas");
    return 0;
}
