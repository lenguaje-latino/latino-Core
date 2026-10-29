/* Prueba aislada de propiedad: construccion real y copias reales.
 * No ejecuta el compilador ni el interprete completo. */
#include "../src/latmv.c"
#include <stdarg.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"FAIL %d: %s\n",__LINE__,#x); exit(2); } } while(0)
LATINO_API void latC_error(lat_mv *mv,const char *fmt,...) {
    (void)mv;va_list ap;va_start(ap,fmt);vfprintf(stderr,fmt,ap);va_end(ap);exit(3);
}
static lat_objeto *crear(lat_mv *mv) {
    lat_bytecode *code=latM_asignar(mv,sizeof(*code)*MAX_BYTECODE_FUNCTION);
    code[0].ins=HALT;
    lat_objeto *f=latC_crear_funcion(mv,code,1);
#ifdef REGISTRY_04
    getFun(f)->nombre=strdup("compartida");
    CHECK(getFun(f)->nombre);
    f->nombre=getFun(f)->nombre;
#else
    f->nombre="compartida";
#endif
    return f;
}
int main(void) {
    lat_mv mv={0},other={0};
    lat_objeto *original=crear(&mv),*independent=crear(&other);
    lat_objeto *copy=latO_clonar(&mv,original);
    lat_objeto *ctx=latO_contexto_crear(&mv);
    latO_asignar_ctx(&mv,ctx,"f",original);
    lat_funcion *payload=getFun(original);
    latO_destruir(&mv,original);
    /* Antes del parche: acceso a payload ya liberado. */
    CHECK(getFun(copy)->codigo[0].ins==HALT);
    CHECK(getFun(latO_obtener_contexto(&mv,ctx,"f"))==payload);
    CHECK(strcmp(copy->nombre,"compartida")==0);
    size_t baseline=mv.memoria_usada;
    for(int i=0;i<500;++i) {
        lat_objeto *another=latO_clonar(&mv,copy);
        latO_destruir(&mv,another);
        CHECK(mv.memoria_usada==baseline);
        CHECK(getFun(copy)->codigo[0].ins==HALT);
    }
    latO_destruir(&mv,ctx);
    CHECK(payload->codigo[0].ins==HALT);
    latO_destruir(&mv,copy);
#ifdef REGISTRY_04
    /* Prototipo sin destructor explicito: el registro conserva su payload. */
    lat_objeto *unreferenced=crear(&mv);
    latM_liberar(&mv,unreferenced);
    latO_liberar_funciones(&mv);
    CHECK(mv.funciones_propias==NULL && mv.memoria_usada==0);
    latO_liberar_funciones(&mv);
    CHECK(getFun(independent)->codigo[0].ins==HALT);
    latO_destruir(&other,independent);
    latO_liberar_funciones(&other);
    CHECK(other.memoria_usada==0);
#endif
    puts("OK: bytecode compartido, 500 clones, dos VM y liberacion unica");
    return 0;
}
