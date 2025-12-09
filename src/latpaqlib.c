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

#define LATINO_LIB

#include <sys/stat.h>

#include "latino.h"

#define LIB_PAQ_NAME "paquete"

/*static void lat_unloadlib(void *lib) { dlclose(lib); }*/

#if defined(_WIN32)

#include <windows.h>

static void throw_error(lat_mv *mv) {
    int error = GetLastError();
    char buffer[128];
    if (FormatMessageA(FORMAT_MESSAGE_IGNORE_INSERTS |
                           FORMAT_MESSAGE_FROM_SYSTEM,
                       NULL, error, 0, buffer, sizeof(buffer), NULL))
        latC_error(mv, buffer);
    else {
        latC_error(mv, "error del sistema");
    }
}
static void *lat_load(lat_mv *mv, const char *path) {
    // printf("lat_load->LoadLibraryA: %s\n", path);
    HINSTANCE lib = LoadLibraryA(path);
    if (lib == NULL) {
        // printf("lat_load->throw_error\n");
        throw_error(mv);
    }
    return lib;
}

static lat_CFuncion lat_sym(lat_mv *mv, void *lib, const char *sym) {
    // printf("lat_sym\n");
    lat_CFuncion f = (lat_CFuncion)GetProcAddress((HINSTANCE)lib, sym);
    if (f == NULL) {
        // printf("lat_sym->throw_error\n");
        throw_error(mv);
    }
    return f;
}
#else
static void *lat_load(lat_mv *mv, const char *path) {
    // printf("lat_load->dlopen: %s\n", path);
    void *lib = dlopen(path, RTLD_NOW);
    if (lib == NULL) {
        // printf("dlerror(): %s\n", dlerror());
        latC_error(mv, dlerror());
    }
    return lib;
}

static lat_CFuncion lat_sym(lat_mv *mv, void *lib, const char *sym) {
    lat_CFuncion f = (lat_CFuncion)dlsym(lib, sym);
    if (f == NULL) {
        latC_error(mv, dlerror());
    }
    return f;
}
#endif //  WIN32

LATINO_API int latC_cargarlib(lat_mv *mv, const char *path, const char *sym) {
    // printf("latC_cargarlib: %s: %s\n", path, sym);
    void *handle = lat_load(mv, path);
    if (handle == NULL) {
        // printf("handle == NULL\n");
        return ERRLIB;
    } else {
        // dlerror();
        lat_CFuncion f = lat_sym(mv, handle, sym);
        if (f == NULL) {
            // printf("f == NULL\n");
            return ERRFUNC;
        }
        lat_objeto *cf = latC_crear_cfuncion(mv, f);
        return latC_llamar_funcion(mv, cf);
    }
}

static void latP_cargar(lat_mv *mv) {
    const char *lib = latC_checar_cadena(mv, latC_desapilar(mv));
    const char *path = latC_checar_cadena(mv, latC_desapilar(mv));
    int stat = latC_cargarlib(mv, path, lib);
    if (stat == 0)
        return;
    else {
        latC_error(mv, "Error al cargar libreria dinamica '%s'", lib);
        return;
    }
}

static const lat_CReg libpaq[] = {{"cargar", latP_cargar, 2}, {NULL, NULL, 0}};

void latC_abrir_liblatino_paqlib(lat_mv *mv) {
    latC_abrir_liblatino(mv, LIB_PAQ_NAME, libpaq);
}
