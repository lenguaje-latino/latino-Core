/* Prueba aislada: fuentes y encabezados reales de Latino-Core.
 * Solo latC_error se sustituye para observar rechazos sin iniciar una VM.
 * No valida el manejo de excepciones ni el ciclo de vida de una VM completa.
 */
#include "latino.h"
#include "latmv.h"
#include "latobj.h"
#include "latmem.h"
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static lat_mv vm;
static jmp_buf error_dest;
static int expecting_error;
static char diagnostic[256];
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"FAIL %d: %s\n",__LINE__,#x); exit(2); } } while(0)

LATINO_API void latC_error(lat_mv *mv, const char *fmt, ...) {
    (void)mv;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(diagnostic, sizeof diagnostic, fmt, ap);
    va_end(ap);
    if (expecting_error) longjmp(error_dest, 1);
    fprintf(stderr, "Unexpected error: %s\n", diagnostic);
    exit(3);
}
static lat_objeto number(double n) {
    lat_objeto o = {0};
    setNumerico(&o, n);
    return o;
}
static void verify(hash_map *map, const char *key, double n) {
    lat_objeto *o = latH_obtener(map, key);
    CHECK(o && isNumerico(o) && getNumerico(o) == n);
}
static void growth(void) {
    size_t initial_memory = vm.memoria_usada;
    hash_map *m = latH_crear(&vm);
    /* Un bucket vacio tambien debe liberarse al redimensionar. */
    m->buckets[0] = latL_crear(&vm);
    char key[64];
    for (int i=0; i<2000; ++i) {
        snprintf(key,sizeof key,"clave_%d",i);
        lat_objeto v=number(i);
        latH_asignar(&vm,m,key,&v);
        CHECK(m->longitud == (size_t)i+1);
        if(i==191) CHECK(m->capacity==256);
        if(i==192) CHECK(m->capacity==512);
        /* Verificar despues de CADA insercion, incluidos todos los crecimientos. */
        for(int j=0;j<=i;++j) {
            snprintf(key,sizeof key,"clave_%d",j);
            verify(m,key,j);
        }
    }
    CHECK(m->capacity==4096);
    lat_objeto v=number(-7);
    latH_asignar(&vm,m,"clave_0",&v);
    CHECK(m->longitud==2000);
    verify(m,"clave_0",-7);
    CHECK(latH_obtener(m,"ausente")==NULL);
    hash_map *copy=latH_clonar(&vm,m);
    latH_destruir(&vm,m);
    for(int j=0;j<2000;++j) {
        snprintf(key,sizeof key,"clave_%d",j);
        verify(copy,key,j==0 ? -7 : j);
    }
    latH_destruir(&vm,copy);
    CHECK(vm.memoria_usada==initial_memory);
}
static void rejected(hash_map *m, const char *key) {
    size_t length=m->longitud, capacity=m->capacity, memory=vm.memoria_usada;
    lista **buckets=m->buckets;
    lat_objeto v=number(99);
    expecting_error=1;
    diagnostic[0]=0;
    if(setjmp(error_dest)==0) {
        latH_asignar(&vm,m,key,&v);
        CHECK(0 && "La clave invalida fue aceptada");
    }
    expecting_error=0;
    CHECK(strstr(diagnostic,"Clave de diccionario")!=NULL);
    CHECK(m->longitud==length && m->capacity==capacity && m->buckets==buckets);
    CHECK(vm.memoria_usada==memory);
    verify(m,"centinela",42);
}
static void keys(int only_long) {
    size_t initial_memory=vm.memoria_usada;
    hash_map *m=latH_crear(&vm);
    lat_objeto v=number(42);
    latH_asignar(&vm,m,"centinela",&v);
    char key[81];
    if(only_long) {
        memset(key,'x',80); key[80]=0;
        rejected(m,key);
    } else {
        latH_asignar(&vm,m,"",&v); verify(m,"",42);
        memset(key,'a',63); key[63]=0;
        latH_asignar(&vm,m,key,&v); verify(m,key,42);
        for(int n=64;n<=65;++n) {
            memset(key,'a',n);key[n]=0;rejected(m,key);
        }
        /* 31 enies (62 bytes) + a = 63 bytes. */
        for(int i=0;i<31;++i) {key[2*i]=(char)0xc3;key[2*i+1]=(char)0xb1;}
        key[62]='a';key[63]=0;
        latH_asignar(&vm,m,key,&v);verify(m,key,42);
        key[62]=(char)0xc3;key[63]=(char)0xb1;key[64]=0;
        rejected(m,key);
        rejected(m,NULL);
    }
    latH_destruir(&vm,m);
    CHECK(vm.memoria_usada==initial_memory);
}
int main(int argc,char **argv) {
    CHECK(argc==2);
    if(strcmp(argv[1],"growth")==0) growth();
    else if(strcmp(argv[1],"keys")==0) keys(0);
    else if(strcmp(argv[1],"long80")==0) keys(1);
    else return 2;
    printf("OK: %s\n",argv[1]);
    return 0;
}
