/*
The MIT License (MIT)

Copyright (c) Latino - Lenguaje de Programacion

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */

#include "latino.h"
#include "latgc.h"
#include "latmem.h"
#include <stdint.h>
#ifdef __linux__
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

// Define en donde el comando Escribir escribira
#define _inicio 0
#define _final -1

#define LIB_ARCHIVO_NAME "archivo"

/* Operaciones de texto: maximo 8 MiB por lectura, sin NUL embebido.
   Todos los errores de E/S cierran el descriptor antes de volver a Latino. */
#define LAT_FILE_MAX_TEXTO (8u * 1024u * 1024u)
/* estado: -1 error, 0 no existe, 1 lectura completa. */
static char *file_leer_texto_estado(const char *path, int *estado) {
    if (estado) *estado = -1;
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        if (estado && errno == ENOENT) *estado = 0;
        return NULL;
    }
    size_t usados = 0, capacidad = 4096;
    char *buf = malloc(capacidad + 1);
    if (!buf) { fclose(fp); return NULL; }
    for (;;) {
        size_t n = fread(buf + usados, 1, capacidad - usados, fp);
        if (memchr(buf + usados, '\0', n)) goto error;
        usados += n;
        if (ferror(fp)) goto error;
        if (feof(fp)) break;
        if (usados == capacidad) {
            if (capacidad == LAT_FILE_MAX_TEXTO) {
                int c = fgetc(fp);
                if (c != EOF || ferror(fp)) goto error;
                break;
            }
            size_t nueva = capacidad * 2;
            char *tmp = realloc(buf, nueva + 1);
            if (!tmp) goto error;
            buf = tmp;
            capacidad = nueva;
        }
    }
    if (fclose(fp) != 0) { free(buf); return NULL; }
    buf[usados] = '\0';
    if (estado) *estado = 1;
    return buf;
error:
    fclose(fp);
    free(buf);
    return NULL;
}

static char *file_leer_texto(const char *path) {
    return file_leer_texto_estado(path, NULL);
}

#ifdef __linux__
static int file_sincronizar(int fd) {
    int r;
    do { r = fsync(fd); } while (r < 0 && errno == EINTR);
    return r;
}
#endif

/* Linux, directorio privado/estable y sistema de archivos local.
   0: fallo antes del reemplazo; 1: reemplazo y fsync confirmados;
   2: reemplazado, pero no se confirmo el cierre/sync del directorio;
  -1: plataforma no implementada. No hay fallback con truncado. */
static int file_guardar_atomico(const char *path, const char *texto) {
#ifdef __linux__
    if (!path || !*path || !texto || strlen(texto) > LAT_FILE_MAX_TEXTO) return 0;
    const char *barra = strrchr(path, '/');
    const char *nombre = barra ? barra + 1 : path;
    if (!*nombre || !strcmp(nombre, ".") || !strcmp(nombre, "..")) return 0;
    struct stat st;
    if (lstat(path, &st) == 0) {
        if (!S_ISREG(st.st_mode)) return 0; /* no seguir enlaces ni dispositivos */
    } else if (errno != ENOENT) return 0;

    size_t ndir = barra ? (size_t)(barra - path) : 1;
    if (barra == path) ndir = 1;
    static const char sufijo[] = "/.latino-XXXXXX";
    if (ndir > SIZE_MAX - sizeof(sufijo)) return 0;
    char *directorio = malloc(ndir + 1);
    char *temporal = malloc(ndir + sizeof(sufijo));
    if (!directorio || !temporal) { free(directorio); free(temporal); return 0; }
    if (barra) memcpy(directorio, path, ndir);
    else directorio[0] = '.';
    directorio[ndir] = '\0';
    memcpy(temporal, directorio, ndir);
    memcpy(temporal + ndir, sufijo, sizeof(sufijo));
    int dirfd = open(directorio, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
    int fd = -1;
    bool creado = false;
    int resultado = 0;
    if (dirfd < 0) goto salir;
    fd = mkostemp(temporal, O_CLOEXEC); /* exclusivo, permisos 0600 */
    if (fd < 0) goto salir;
    creado = true;
    size_t n = strlen(texto), usados = 0;
    while (usados < n) {
        ssize_t escritos = write(fd, texto + usados, n - usados);
        if (escritos < 0 && errno == EINTR) continue;
        if (escritos <= 0) goto salir;
        usados += (size_t)escritos;
    }
    if (file_sincronizar(fd) < 0) goto salir;
    int cierre = close(fd);
    fd = -1; /* en Linux no reintentar close tras un error */
    if (cierre < 0) goto salir;
    if (rename(temporal, path) < 0) goto salir;
    creado = false;
    resultado = file_sincronizar(dirfd) == 0 ? 1 : 2;
salir:
    if (fd >= 0) (void)close(fd);
    if (creado) (void)unlink(temporal);
    if (dirfd >= 0 && close(dirfd) < 0 && resultado == 1) resultado = 2;
    free(temporal);
    free(directorio);
    return resultado;
#else
    (void)path; (void)texto;
    return -1;
#endif
}

/* API C compartida con los consumidores; conserva exactamente el motor de
   Entrega11. El llamador libera con free() la lectura completa. */
LATINO_API char *latF_leer_texto_seguro(const char *path, int *estado) {
    if (estado) *estado = -1;
    if (!path || !*path) return NULL;
    return file_leer_texto_estado(path, estado);
}

LATINO_API int latF_escribir_atomico(const char *path, const char *texto) {
    return file_guardar_atomico(path, texto);
}

static void file_leer_texto_seguro(lat_mv *mv) {
    const char *path = latC_checar_cadena(mv, latC_desapilar(mv));
    int estado;
    char *buf = latF_leer_texto_seguro(path, &estado);
    if (buf) { latC_apilar_string(mv, buf); free(buf); }
    else latC_apilar(mv, estado == 0 ? latO_nulo : latO_falso);
}

static void file_escribir_atomico(lat_mv *mv) {
    lat_objeto *valor = latC_desapilar(mv);
    lat_objeto *ruta = latC_desapilar(mv);
    const char *path = latC_checar_cadena(mv, ruta);
    const char *texto = latC_checar_cadena(mv, valor);
    int resultado = latF_escribir_atomico(path, texto);
    if (resultado == 2)
        fprintf(stderr, "Latino: '%s' reemplazado; no se confirmo la sincronizacion/cierre del directorio.\n", path);
    latC_apilar_double(mv, (double)resultado);
}

static bool file_escribir_texto(const char *path, const char *texto,
                                const char *modo) {
    FILE *fp = fopen(path, modo);
    if (!fp) return false;
    size_t n = strlen(texto);
    bool ok = fwrite(texto, 1, n, fp) == n;
    /* fclose tambien puede informar un error de escritura diferida. */
    if (fclose(fp) != 0) ok = false;
    return ok;
}

static void file_lineas(lat_mv *mv) {
    const char *path = latC_checar_cadena(mv, latC_desapilar(mv));
    char *buf = file_leer_texto(path);
    if (!buf) {
        latC_error(mv, "No se pudo leer el archivo '%s'", path);
        return;
    }
    lista *lst = latL_crear(mv);
    char *p = buf;
    while (*p) {
        char *fin = strchr(p, '\n');
        if (fin) *fin = '\0';
        latL_agregar(mv, lst, latC_crear_cadena(mv, p));
        if (!fin) break;
        p = fin + 1;
    }
    free(buf);
    lat_objeto *lineas = latC_crear_lista(mv, lst);
    latC_apilar(mv, lineas);
#if !HABILITAR_GC
    latM_liberar(mv, lineas);
#endif
}

static void file_leer(lat_mv *mv) {
    const char *path = latC_checar_cadena(mv, latC_desapilar(mv));
    char *buf = file_leer_texto(path);
    if (!buf) { latC_apilar(mv, latO_falso); return; }
    latC_apilar_string(mv, buf);
    free(buf);
}

/* API optativa: los fallos operativos devuelven falso, sin longjmp.
   Los errores de tipos siguen siendo errores de programacion. */
static void file_intentar_escribir(lat_mv *mv) {
    lat_objeto *s = latC_desapilar(mv);
    lat_objeto *o = latC_desapilar(mv);
    const char *path = latC_checar_cadena(mv, o);
    const char *cad = latC_checar_cadena(mv, s);
    latC_apilar(mv, file_escribir_texto(path, cad, "wb") ? latO_verdadero : latO_falso);
}

static void file_escribir(lat_mv *mv) {
    lat_objeto *s = latC_desapilar(mv);
    lat_objeto *o = latC_desapilar(mv);
    const char *path = latC_checar_cadena(mv, o);
    const char *cad = latC_checar_cadena(mv, s);
    if (!file_escribir_texto(path, cad, "w"))
        latC_error(mv, "No se pudo escribir el archivo '%s'", path);
}

static void file_ejecutar(lat_mv *mv) {
    lat_objeto *o = latC_desapilar(mv);
    char *input = latC_checar_cadena(mv, o);
    char *dot = strrchr(input, '.');
    char *extension;
    if (!dot || dot == input) {
        extension = "";
    } else {
        extension = dot + 1;
    }
    if (!strcmp(extension, "lat")) {
        int status;
        ast *nodo = latA_analizar_arch(input, &status);
        if (!nodo) {
            latC_error(mv, "Error al leer el archivo: '%s'", input);
        }
        mv->global->REPL = false;
        mv->nombre_archivo = input;
        lat_objeto *func = latC_analizar(mv, nodo);
        if (status == 0) {
            status = latC_llamar_funcion(mv, func);
            latO_destruir(mv, func);
            latA_destruir(nodo);
        } else {
            latC_error(mv, "Error al ejecutar archivo '%s'", input);
        }
    }
}

static void file_duplicar(lat_mv *mv) {
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    char *aa = latC_checar_cadena(mv, a);
    char *bb = latC_checar_cadena(mv, b);
    if (strcmp(aa, bb) == 0) {
        latC_error(
            mv,
            "Error al duplicar archivo '%s', nombre o ruta deven ser distintos",
            latC_checar_cadena(mv, b));
    } else {
        FILE *archivo, *copiar;
        int txt;
        archivo = fopen(latC_checar_cadena(mv, a), "r");
        copiar = fopen(latC_checar_cadena(mv, b), "wb");
        if (!archivo) {
            latC_error(mv, "Error: No se pudo abrir el archivo '%s'",
                       latC_checar_cadena(mv, a));
        } else if (!copiar) {
            latC_error(mv, "Error: No se pudo abrir el archivo '%s'",
                       latC_checar_cadena(mv, b));
        }
        while ((txt = fgetc(archivo)) != EOF)
            fputc(txt, copiar);
        fclose(archivo);
        fclose(copiar);
    }
}

static void file_eliminar(lat_mv *mv) {
    int status;
    lat_objeto *a = latC_desapilar(mv);
    status = remove(latC_checar_cadena(mv, a));
    if (status == 0) {
        latC_apilar(mv, latO_verdadero);
    } else {
        latC_apilar(mv, latO_falso);
    }
}

static void file_crear(lat_mv *mv) {
    const char *path = latC_checar_cadena(mv, latC_desapilar(mv));
    FILE *fp = fopen(path, "rb");
    if (fp) { fclose(fp); latC_apilar(mv, latO_falso); return; }
    /* C11: "x" evita truncar un archivo creado entre ambas aperturas. */
    fp = fopen(path, "wbx");
    bool ok = fp != NULL;
    if (fp && fclose(fp) != 0) ok = false;
    latC_apilar(mv, ok ? latO_verdadero : latO_falso);
}

static void file_renombrar(lat_mv *mv) {
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    const char *origen = latC_checar_cadena(mv, a);
    const char *nuevo = latC_checar_cadena(mv, b);
    if (rename(origen, nuevo) == 0) latC_apilar_string(mv, nuevo);
    else latC_apilar(mv, latO_falso);
}

static void file_anexar(lat_mv *mv) {
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    const char *path = latC_checar_cadena(mv, a);
    const char *texto = latC_checar_cadena(mv, b);
    if (!file_escribir_texto(path, texto, "a"))
        latC_error(mv, "No se pudo anexar al archivo '%s'", path);
}

static const lat_CReg libfile[] = {{"leer", file_leer, 1},
                                   {"leer_texto_seguro", file_leer_texto_seguro, 1},
                                   {"escribir_atomico", file_escribir_atomico, 2},
                                   {"intentar_leer", file_leer, 1},
                                   {"intentar_escribir", file_intentar_escribir, 2},
                                   {"lineas", file_lineas, 1},
                                   {"ejecutar", file_ejecutar, 1},
                                   {"escribir", file_escribir, 2},
                                   {"duplicar", file_duplicar, 2},
                                   {"anexar", file_anexar, 2},
                                   {"eliminar", file_eliminar, 1},
                                   {"borrar", file_eliminar, 1},
                                   {"crear", file_crear, 1},
                                   {"renombrar", file_renombrar, 2},
    {NULL, NULL, 0}};

void latC_abrir_liblatino_filelib(lat_mv *mv) {
    latC_abrir_liblatino(mv, LIB_ARCHIVO_NAME, libfile);
}
//agregar funcion de elegiar donde escribir
// {"escribir", file_escribir, 3},
// archivo.escribir("archivo1.lat", "a�adir", 10)
//https://medium.com/10-goto-10/manejo-de-archivos-en-c-19f85b31ddd
