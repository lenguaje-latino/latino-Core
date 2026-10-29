#include "latino.h"
#include "latmem.h"
#include <stdarg.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#define EXIGIR(c) do { if (!(c)) { fprintf(stderr,"FALLO %s:%d: %s\n",__FILE__,__LINE__,#c); exit(2); } } while(0)
static lat_mv *observada;
static void liberar_observando(void *p) {
    if (p==observada) { EXIGIR(observada->memoria_usada==0); observada=NULL; }
    free(p);
}
#define free liberar_observando
#include "../src/latmv.c"
#undef free

enum { BIEN, F_TEMPORAL, F_ESCRITURA, F_CERO, F_SYNC_ARCHIVO,
       F_CIERRE, F_RENAME, F_SYNC_DIR, F_CIERRE_DIR, ESCRITURA_CORTA, EINTR_ESCRITURA, EINTR_SYNC };
static int fallo, llamadas_write, llamadas_sync, ultimo_fd=-1;
static char ultimo_temporal[4096];
static int probar_mkostemp(char *ruta,int flags) {
    if (fallo==F_TEMPORAL) {errno=EMFILE; return -1;}
    int fd=mkostemp(ruta,flags);
    if(fd>=0) {ultimo_fd=fd; snprintf(ultimo_temporal,sizeof ultimo_temporal,"%s",ruta);}
    return fd;
}
static ssize_t probar_write(int fd,const void *p,size_t n) {
    llamadas_write++;
    if (fallo==F_ESCRITURA && llamadas_write>1) {errno=ENOSPC;return -1;}
    if (fallo==F_CERO) return 0;
    if (fallo==EINTR_ESCRITURA && llamadas_write==1) {errno=EINTR;return -1;}
    if ((fallo==F_ESCRITURA || fallo==ESCRITURA_CORTA) && n>1) n=1;
    return write(fd,p,n);
}
static int probar_fsync(int fd) {
    struct stat st; EXIGIR(fstat(fd,&st)==0);
    llamadas_sync++;
    if ((fallo==F_SYNC_ARCHIVO && S_ISREG(st.st_mode)) ||
        (fallo==F_SYNC_DIR && S_ISDIR(st.st_mode))) {errno=EIO;return -1;}
    if (fallo==EINTR_SYNC && llamadas_sync==1) {errno=EINTR;return -1;}
    return fsync(fd);
}
static int probar_close(int fd) {
    struct stat st; EXIGIR(fstat(fd,&st)==0);
    int r=close(fd);
    if ((fallo==F_CIERRE && fd==ultimo_fd) ||
        (fallo==F_CIERRE_DIR && S_ISDIR(st.st_mode))) {errno=EIO;return -1;}
    return r;
}
static int probar_rename(const char *a,const char *b) {
    if (fallo==F_RENAME) {errno=EACCES;return -1;}
    return rename(a,b);
}
#define mkostemp probar_mkostemp
#define write probar_write
#define fsync probar_fsync
#define close probar_close
#define rename probar_rename
#include "../src/latfilelib.c"
#undef mkostemp
#undef write
#undef fsync
#undef close
#undef rename

static void reiniciar(int caso) {
    fallo=caso; llamadas_write=llamadas_sync=0; ultimo_fd=-1; ultimo_temporal[0]='\0';
}
static void contenido(const char *ruta,const char *esperado) {
    char *s=file_leer_texto(ruta); EXIGIR(s && !strcmp(s,esperado)); free(s);
}
static void sin_temporales(const char *dir) {
    DIR *d=opendir(dir); EXIGIR(d);
    struct dirent *e;
    while((e=readdir(d))) EXIGIR(strncmp(e->d_name,".latino-",8)!=0);
    EXIGIR(closedir(d)==0);
    if(ultimo_fd>=0) EXIGIR(fcntl(ultimo_fd,F_GETFD)==-1 && errno==EBADF);
}
int main(void) {
    char dir[]="/tmp/latino-atomic11-XXXXXX"; EXIGIR(mkdtemp(dir));
    char ruta[256],enlace[256],ausente[256];
    snprintf(ruta,sizeof ruta,"%s/nombre.txt",dir);
    snprintf(enlace,sizeof enlace,"%s/enlace",dir);
    snprintf(ausente,sizeof ausente,"%s/no-existe/nombre.txt",dir);
    const char *nuevo="María Ñáñez";
    for(int ciclo=0;ciclo<10;++ciclo) {
        for(int caso=F_TEMPORAL;caso<=F_RENAME;++caso) {
            reiniciar(BIEN); EXIGIR(file_guardar_atomico(ruta,"ANTERIOR")==1);
            reiniciar(caso); EXIGIR(file_guardar_atomico(ruta,nuevo)==0);
            contenido(ruta,"ANTERIOR"); sin_temporales(dir);
        }
        for(int caso=F_SYNC_DIR;caso<=F_CIERRE_DIR;++caso) {
            reiniciar(caso); EXIGIR(file_guardar_atomico(ruta,nuevo)==2);
            contenido(ruta,nuevo); sin_temporales(dir);
        }
        for(int caso=ESCRITURA_CORTA;caso<=EINTR_SYNC;++caso) {
            reiniciar(caso); EXIGIR(file_guardar_atomico(ruta,"completo")==1);
            contenido(ruta,"completo"); sin_temporales(dir);
        }
    }
    reiniciar(BIEN);
    EXIGIR(file_guardar_atomico(ausente,"nuevo")==0);
    EXIGIR(file_guardar_atomico(dir,"nuevo")==0);
    EXIGIR(symlink(ruta,enlace)==0);
    EXIGIR(file_guardar_atomico(enlace,"NO")==0); contenido(ruta,"completo");
    EXIGIR(unlink(enlace)==0);
    EXIGIR(file_guardar_atomico(ruta,"")==1); contenido(ruta,"");
    struct stat st; EXIGIR(stat(ruta,&st)==0 && (st.st_mode & 0777)==0600);
    int estado=99; char *s=file_leer_texto_estado(ausente,&estado); EXIGIR(!s && estado==0);
    s=file_leer_texto_estado(dir,&estado); EXIGIR(!s && estado==-1);
    s=file_leer_texto_estado(ruta,&estado); EXIGIR(s && !*s && estado==1); free(s);
    FILE *fp=fopen(ruta,"wb"); EXIGIR(fp); EXIGIR(fwrite("a\0b",1,3,fp)==3 && fclose(fp)==0);
    s=file_leer_texto_estado(ruta,&estado); EXIGIR(!s && estado==-1);
    char *grande=malloc(LAT_FILE_MAX_TEXTO+2); EXIGIR(grande);
    memset(grande,'x',LAT_FILE_MAX_TEXTO+1); grande[LAT_FILE_MAX_TEXTO]='\0';
    EXIGIR(file_guardar_atomico(ruta,grande)==1);
    s=file_leer_texto_estado(ruta,&estado);
    EXIGIR(s && estado==1 && !strcmp(s,grande)); free(s);
    grande[LAT_FILE_MAX_TEXTO]='x'; grande[LAT_FILE_MAX_TEXTO+1]='\0';
    EXIGIR(file_guardar_atomico(ruta,grande)==0);
    s=file_leer_texto_estado(ruta,&estado);
    EXIGIR(s && estado==1 && strlen(s)==LAT_FILE_MAX_TEXTO); free(s); free(grande);
    EXIGIR(file_guardar_atomico(ruta,nuevo)==1);
    for(int ciclo=0;ciclo<20;++ciclo) {
        lat_mv *mv=latC_crear_mv(); mv->nombre_archivo="atomic11";
        /* El formulario selecciona la lectura mediante tipo(...) == "cadena".
           Comparar cadenas no debe modificar el contador ni su contenido. */
        for(int igual=0;igual<2;++igual) {
            latC_apilar_string(mv,"María"); latC_apilar_string(mv,igual ? "María" : "Mario");
            size_t antes=mv->memoria_usada;
            igualdad(mv);
            EXIGIR(latC_checar_logico(mv,latC_desapilar(mv))==(bool)igual);
            EXIGIR(mv->memoria_usada==antes);
            latC_apilar_string(mv,"María"); latC_apilar_string(mv,igual ? "María" : "Mario");
            antes=mv->memoria_usada;
            diferente(mv);
            EXIGIR(latC_checar_logico(mv,latC_desapilar(mv))!=(bool)igual);
            EXIGIR(mv->memoria_usada==antes);
        }
        latC_apilar_string(mv,ausente); file_leer_texto_seguro(mv);
        EXIGIR(latC_desapilar(mv)->tipo==T_NULL);
        latC_apilar_string(mv,dir); file_leer_texto_seguro(mv);
        lat_objeto *o=latC_desapilar(mv); EXIGIR(o->tipo==T_BOOL && !latC_checar_logico(mv,o));
        latC_apilar_string(mv,ruta); latC_apilar_string(mv,nuevo); file_escribir_atomico(mv);
        EXIGIR(latC_checar_numerico(mv,latC_desapilar(mv))==1);
        latC_apilar_string(mv,ruta); file_leer_texto_seguro(mv);
        EXIGIR(!strcmp(latC_checar_cadena(mv,latC_desapilar(mv)),nuevo));
        EXIGIR(mv->ptrpila==0 && mv->ptrctx==0);
        observada=mv; latC_destruir_mv(mv); EXIGIR(!observada);
    }
    sin_temporales(dir); EXIGIR(unlink(ruta)==0 && rmdir(dir)==0);
    puts("OK: escritura atomica, fallos antes/despues del reemplazo, EINTR, escrituras cortas, limpieza, comparacion de cadenas y 20 VM sin saldo");
    return 0;
}
