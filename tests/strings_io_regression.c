#include "latino.h"
#include "latmem.h"
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#define EXIGIR(c) do { if (!(c)) { fprintf(stderr,"FALLO %s:%d: %s\n",__FILE__,__LINE__,#c); exit(2); } } while (0)
static lat_mv *observada;
static void liberar_observando(void *p) {
    if (p == observada) {
        EXIGIR(observada->memoria_usada == 0);
        observada = NULL;
    }
    free(p);
}
#define free liberar_observando
#include "../src/latmv.c"
#undef free
#include "../src/latfilelib.c"
char *quitar_espacios(const char *s);
void str_recortar(lat_mv *mv);
void str_separar(lat_mv *mv);
void str_longitud(lat_mv *mv);

static void texto(lat_mv *mv, const char *s) { latC_apilar_string(mv,s); }
static void exigir_texto(lat_mv *mv, const char *s) {
    lat_objeto *o=latC_desapilar(mv);
    EXIGIR(o->tipo==T_STR && !strcmp(latC_checar_cadena(mv,o),s));
}
static void exigir_logico(lat_mv *mv, bool b) {
    lat_objeto *o=latC_desapilar(mv);
    EXIGIR(o->tipo==T_BOOL && latC_checar_logico(mv,o)==b);
}
/* El llamador C posee la lista devuelta por estas funciones. */
static void exigir_lista(lat_mv *mv, const char **esperadas, int n) {
    lista *l=latC_checar_lista(mv,latC_desapilar(mv));
    EXIGIR(l->longitud==n);
    int i=0;
    LIST_FOREACH(l,primero,siguiente,cur) {
        lat_objeto *o=cur->valor;
        EXIGIR(!strcmp(latC_checar_cadena(mv,o),esperadas[i++]));
        latO_destruir(mv,o);
    }
    latL_destruir(mv,l);
}
int main(void) {
    const char *entradas[]={""," ","\t\r\n","a","  María Ñáñez  ","sin espacios"};
    const char *salidas[]={"","","","a","María Ñáñez","sin espacios"};
    for (size_t i=0;i<sizeof entradas/sizeof *entradas;++i) {
        char *t=quitar_espacios(entradas[i]);
        EXIGIR(t && !strcmp(t,salidas[i])); free(t);
    }
    char directorio[]="/tmp/latino-io10-XXXXXX";
    EXIGIR(mkdtemp(directorio));
    char ruta[256],ausente[256],mala[256];
    snprintf(ruta,sizeof ruta,"%s/datos.txt",directorio);
    snprintf(ausente,sizeof ausente,"%s/ausente",directorio);
    snprintf(mala,sizeof mala,"%s/no-existe/datos.txt",directorio);
    for(int ciclo=0;ciclo<10;++ciclo) {
        lat_mv *mv=latC_crear_mv();
        mv->nombre_archivo="regresion-cadenas-io";
        /* Dos envoltorios comparten la misma cadena internada. */
        lat_objeto *alias=latC_crear_cadena(mv,"  María Ñáñez  ");
        for(int i=0;i<100;++i) {
            texto(mv,"  María Ñáñez  "); str_recortar(mv); exigir_texto(mv,"María Ñáñez");
            EXIGIR(!strcmp(latC_checar_cadena(mv,alias),"  María Ñáñez  "));
            texto(mv,"a,,b;c,"); texto(mv,",;"); str_separar(mv);
            const char *partes[]={"a","b","c"}; exigir_lista(mv,partes,3);
            texto(mv,"a,,b;c,"); exigir_texto(mv,"a,,b;c,");
            texto(mv,""); texto(mv,","); str_separar(mv); exigir_lista(mv,NULL,0);
            texto(mv," a  b "); texto(mv,""); str_separar(mv);
            const char *espacios[]={"a","b"}; exigir_lista(mv,espacios,2);
            latC_apilar(mv,latO_verdadero); latC_apilar(mv,latO_falso);
            y_logico(mv); exigir_logico(mv,false);
            latC_apilar(mv,latO_falso); texto(mv,"elegida");
            o_logico(mv); exigir_texto(mv,"elegida");
            latC_apilar_double(mv,2); latC_apilar_double(mv,7);
            y_logico(mv); EXIGIR(latC_checar_numerico(mv,latC_desapilar(mv))==7);
            texto(mv,"abc"); str_longitud(mv);
            EXIGIR(latC_checar_numerico(mv,latC_desapilar(mv))==3);
        }
        latO_destruir(mv,alias);
        texto(mv,mala); texto(mv,"no escrito"); file_intentar_escribir(mv); exigir_logico(mv,false);
        texto(mv,ausente); file_leer(mv); exigir_logico(mv,false);
        texto(mv,mala); file_crear(mv); exigir_logico(mv,false);
        texto(mv,ruta); texto(mv,""); file_intentar_escribir(mv); exigir_logico(mv,true);
        texto(mv,ruta); file_leer(mv); exigir_texto(mv,"");
        texto(mv,ruta); file_crear(mv); exigir_logico(mv,false);
        char grande[1100]; memset(grande,'a',sizeof grande); grande[1000]='\n';
        strcpy(grande+1001,"última sin salto");
        texto(mv,ruta); texto(mv,grande); file_intentar_escribir(mv); exigir_logico(mv,true);
        texto(mv,ruta); file_leer(mv); exigir_texto(mv,grande);
        char primera[1001]; memset(primera,'a',1000); primera[1000]='\0';
        const char *lineas[]={primera,"última sin salto"};
        texto(mv,ruta); file_lineas(mv); exigir_lista(mv,lineas,2);
        FILE *fp=fopen(ruta,"wb"); EXIGIR(fp);
        EXIGIR(fwrite("a\0b",1,3,fp)==3 && fclose(fp)==0);
        texto(mv,ruta); file_leer(mv); exigir_logico(mv,false);
#ifdef __linux__
        texto(mv,"/dev/full"); texto(mv,"error diferido");
        file_intentar_escribir(mv); exigir_logico(mv,false);
#endif
        EXIGIR(mv->ptrpila==0 && mv->ptrctx==0);
        observada=mv; latC_destruir_mv(mv); EXIGIR(!observada);
    }
    EXIGIR(remove(ruta)==0 && rmdir(directorio)==0);
    puts("OK: recorte, alias internados, separacion, logicos, 1000 ciclos, E/S fallida y 10 cierres sin saldo");
    return 0;
}
