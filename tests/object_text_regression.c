/* Fuentes reales; solo el error de latobj.c se intercepta para observar rechazos.
   El conteo de buffers libc complementa ASan/LSan y el saldo interno de la VM. */
#include "latino.h"
#include "latmem.h"
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <unistd.h>
#define EXIGIR(c) do { if(!(c)) { fprintf(stderr,"FALLO %d: %s\n",__LINE__,#c); exit(2); } } while(0)
static lat_mv *observada;
static void retirar(void *p);
static void liberar_vm(void *p) {
    if(p==observada) { EXIGIR(observada->memoria_usada==0); observada=NULL; }
    retirar(p);
    free(p);
}
#define free liberar_vm
#include "../src/latmv.c"
#undef free

static void *reservas[4096];
static size_t vivas;
static void registrar(void *p) {
    EXIGIR(p);
    for(size_t i=0;i<4096;++i) if(!reservas[i]) {reservas[i]=p;vivas++;return;}
    EXIGIR(0 && "registro lleno");
}
static void retirar(void *p) {
    for(size_t i=0;i<4096;++i) if(p && reservas[i]==p) {reservas[i]=NULL;vivas--;return;}
}
static void *malloc_medido(size_t n) {void *p=malloc(n); if(p)registrar(p);return p;}
static void *calloc_medido(size_t n,size_t s) {void *p=calloc(n,s);if(p)registrar(p);return p;}
static char *strdup_medido(const char *s) {char *p=strdup(s);if(p)registrar(p);return p;}
static void *realloc_medido(void *p,size_t n) {
    size_t indice=4096;
    for(size_t i=0;i<4096;++i) if(p && reservas[i]==p) {indice=i;break;}
    void *q=realloc(p,n);
    if(q) {
        if(indice<4096) reservas[indice]=q;
        else registrar(q);
    }
    return q;
}
static void free_medido(void *p) {retirar(p);free(p);}
static jmp_buf salto;
static bool espera_error;
static char mensaje[256];
static void error_objeto(lat_mv *mv,const char *fmt,...) {
    (void)mv;va_list ap;va_start(ap,fmt);vsnprintf(mensaje,sizeof mensaje,fmt,ap);va_end(ap);
    if(espera_error) longjmp(salto,1);
    fprintf(stderr,"Error no esperado: %s\n",mensaje);exit(3);
}
#define malloc malloc_medido
#define calloc calloc_medido
#define realloc realloc_medido
#define strdup strdup_medido
#define free free_medido
#define latC_error error_objeto
#include "../src/latobj.c"
#undef malloc
#undef calloc
#undef realloc
#undef strdup
#undef free
#undef latC_error

static void texto(lat_mv *mv,lat_objeto *o,const char *esperado) {
    size_t saldo=mv->memoria_usada, anteriores=vivas;
    char *s=latC_astring(mv,o);
    EXIGIR(s && !strcmp(s,esperado));
    EXIGIR(mv->memoria_usada==saldo);
    free_medido(s); EXIGIR(vivas==anteriores);
}
static void rechazo(lat_mv *mv,lat_objeto *o,const char *causa) {
    size_t anteriores=vivas,saldo=mv->memoria_usada;
    espera_error=true;
    if(!setjmp(salto)) {free_medido(latC_astring(mv,o));EXIGIR(0);}
    espera_error=false;
    EXIGIR(strstr(mensaje,causa));
    EXIGIR(vivas==anteriores && mv->memoria_usada==saldo);
}
static void booleanos(lat_mv *mv) {
    const char *s[]={"","0","falso","FaLsO","FALSE","false","true","00"," 0","María","ñ"};
    const bool esperado[]={false,false,false,false,false,false,true,true,true,true,true};
    for(size_t i=0;i<sizeof s/sizeof *s;++i) {
        lat_objeto *o=latC_crear_cadena(mv,s[i]);size_t saldo=mv->memoria_usada,antes=vivas;
        for(int n=0;n<1000;++n) EXIGIR(latC_abool(mv,o)==esperado[i]);
        EXIGIR(saldo==mv->memoria_usada && antes==vivas);
        EXIGIR(!strcmp(latC_checar_cadena(mv,o),s[i]));latO_destruir(mv,o);
    }
}
static void caracteres(lat_mv *mv) {
    lat_objeto entero={0};setEntero(&entero,2147483647);
    texto(mv,&entero,"2147483647");setEntero(&entero,-17);texto(mv,&entero,"-17");
    setEntero(&entero,1e30);rechazo(mv,&entero,"Entero fuera de rango");
    for(int i=0;i<=255;++i) {
        lat_objeto o={0};setCaracter(&o,(char)i);
        char s[2]={(char)i,0};texto(mv,&o,s);
    }
}
static void imprimir(lat_mv *mv) {
    lat_objeto *o=latC_crear_cadena(mv,"María \\uX \\");
    FILE *f=tmpfile();EXIGIR(f);int original=dup(STDOUT_FILENO);EXIGIR(original>=0);
    EXIGIR(fflush(stdout)==0 && dup2(fileno(f),STDOUT_FILENO)>=0);
    size_t antes=mv->memoria_usada,buffers=vivas;
    for(int i=0;i<20;++i)latO_imprimir(mv,o,false);
    EXIGIR(fflush(stdout)==0 && dup2(original,STDOUT_FILENO)>=0 && close(original)==0);
    EXIGIR(mv->memoria_usada==antes && vivas==buffers);
    EXIGIR(fseek(f,0,SEEK_SET)==0);
    char obtenido[1024]={0},esperado[1024]={0};
    for(int i=0;i<20;++i)strcat(esperado,"María \\uX \\");
    EXIGIR(fread(obtenido,1,sizeof obtenido-1,f)==strlen(esperado));
    EXIGIR(!strcmp(obtenido,esperado) && fclose(f)==0);latO_destruir(mv,o);
}
static void comparaciones(lat_mv *mv) {
    for(int i=0;i<200;++i) {
        latC_apilar_string(mv,"7");latC_apilar_double(mv,7);size_t antes=mv->memoria_usada;
        igualdad(mv);EXIGIR(latC_checar_logico(mv,latC_desapilar(mv)));
        EXIGIR(mv->memoria_usada==antes);
        latC_apilar(mv,latO_nulo);latC_apilar_string(mv,"nulo");antes=mv->memoria_usada;
        diferente(mv);EXIGIR(!latC_checar_logico(mv,latC_desapilar(mv)));
        EXIGIR(mv->memoria_usada==antes);
    }

}
static void contenedores(lat_mv *mv) {
    hash_map *m=latH_crear(mv);char llave[64];
    lat_objeto v={0};
    for(int i=0;i<2000;++i) {snprintf(llave,sizeof llave,"clave_%d",i);setNumerico(&v,i);latH_asignar(mv,m,llave,&v);}
    EXIGIR(m->capacity>256);
    lat_objeto dic={0};setDic(&dic,m);
    char *s=latC_astring(mv,&dic);
    for(int i=0;i<2000;++i) {char par[96];snprintf(par,sizeof par,"\"clave_%d\": %d",i,i);EXIGIR(strstr(s,par));}
    free_medido(s);latH_destruir(mv,m);
    lista *l=latL_crear(mv);lat_objeto *cadena=latC_crear_cadena(mv,"x, } \\\"y");
    latL_agregar(mv,l,cadena);latL_agregar(mv,l,NULL);
    lat_objeto obj={0};setLista(&obj,l);
    texto(mv,&obj,"[\"x, } \\\\\\\"y\", nulo]");
    latL_destruir(mv,l);latO_destruir(mv,cadena);
    l=latL_crear(mv);setLista(&obj,l);latL_agregar(mv,l,&obj);
    rechazo(mv,&obj,"circular");latL_destruir(mv,l);
    /* Un mismo recurso en ramas distintas no constituye un ciclo. */
    lista *hoja=latL_crear(mv),*raiz=latL_crear(mv);
    lat_objeto h={0},r={0};setLista(&h,hoja);setLista(&r,raiz);
    latL_agregar(mv,raiz,&h);latL_agregar(mv,raiz,&h);texto(mv,&r,"[[], []]");
    latL_destruir(mv,raiz);latL_destruir(mv,hoja);
    lista niveles[65]={0};nodo_lista nodos[64]={0};lat_objeto objetos[65]={0};
    for(int i=0;i<65;++i) {setLista(&objetos[i],&niveles[i]); if(i<64) {niveles[i].primero=&nodos[i];nodos[i].valor=&objetos[i+1];}}
    rechazo(mv,&objetos[0],"Anidamiento");
    char *grande=malloc(MAX_STR_LENGTH+1);EXIGIR(grande);
    memset(grande,'a',MAX_STR_LENGTH);grande[MAX_STR_LENGTH]=0;
    cadena=latC_crear_cadena(mv,grande);free(grande);
    l=latL_crear(mv);setLista(&obj,l);latL_agregar(mv,l,cadena);
    rechazo(mv,&obj,"demasiado larga");latL_destruir(mv,l);latO_destruir(mv,cadena);
}
int main(int argc,char **argv) {
    EXIGIR(argc==2);lat_mv *mv=latC_crear_mv();mv->nombre_archivo="object_text11b";
    if(!strcmp(argv[1],"booleanos"))booleanos(mv);
    else if(!strcmp(argv[1],"caracteres"))caracteres(mv);
    else if(!strcmp(argv[1],"imprimir"))imprimir(mv);
    else if(!strcmp(argv[1],"comparaciones"))comparaciones(mv);
    else if(!strcmp(argv[1],"contenedores"))contenedores(mv);
    else EXIGIR(0);
    EXIGIR(mv->ptrpila==0 && mv->ptrctx==0 && vivas==0);
    observada=mv;latC_destruir_mv(mv);EXIGIR(!observada && vivas==0);
    printf("OK: %s, saldo VM y buffers observados en cero\n",argv[1]);
}
