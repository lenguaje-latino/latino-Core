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
#include "latast.h"
#include "latmem.h"
#include "latmv.h"
#include "latrepl.h"

/*
Para depurar en visual studio:
Menu propiedades del proyecto-> Debugging -> Command Arguments. Agregar
$(SolutionDir)..\ejemplos\debug.lat
----------------------------------
Para depurar en Netbeans ir a propiedades del proyecto -> Run command y Agregar
"${OUTPUT_PATH}" ejemplos/debug.lat
 */

#if YYDEBUG
extern int yydebug = 1; /* 1 para debuguear analizador lexico/sintactico */
#endif

int parse_silent = 0;
char *filename = NULL;

void str_ejecutar(lat_mv *mv);

/**
 * Muestra solo la version de latino
 */
static void lat_version_simple() { printf("%s\n", LAT_VERSION); }

/**
 * Muestra la version de latino y los derechos en la consola
 */
static void lat_version() { printf("%s\n", LAT_DERECHOS); }

/**
 * Para crear el logo en ascii
 */
static void lat_logo() { printf("%s\n", LAT_LOGO); }

/**
 * Muestra la ayuda en la consola
 */
static void lat_ayuda() {
    lat_logo();
    lat_version();
    printf("%s\n", "-Uso de latino: latino [opcion] [archivo]");
    printf("\n");
    printf("%s\n", "Opciones:");
    printf("%s\n", "-a | --ayuda     : Muestra la ayuda de Latino");
    printf("%s\n", "-v | --version   : Muestra la version de Latino");
    printf("%s\n", "-e               : Ejecuta una cadena de codigo");
    printf("%s\n", "archivo          : Nombre del archivo con extension .lat");
    printf("%s\n", "Ctrl + C         : Para cerrar");
    printf("\n");
    printf("%s\n", "Variables de entorno:");
    printf("%s\n", "_____________________");
    printf("%s%s\n", "LATINO_PATH      : ", getenv("LATINO_PATH"));
    printf("%s%s\n", "LATINO_LIB       : ", getenv("LATINO_LIB"));
    printf("%s%s\n", "LATINO_LIBC      : ", getenv("LATINO_LIBC"));
    printf("%s%s\n", "HOME             : ", getenv("HOME"));
}

int main(int argc, char *argv[]) {
    #if (defined __WIN32__) || (defined _WIN32)
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    #endif
    setlocale(LC_ALL, "C.UTF-8");
    setlocale(LC_CTYPE, "C.UTF-8");
    /* para numeros decimales */
    // setlocale(LC_NUMERIC, "es_MX");
    // setlocale(LC_MONETARY, "es_MX");
    int i;
    char *infile = NULL;
    int pe = false;
    int pf = false;
    /* procesa params de latino */
    for (i = 1; i < argc; i++) {
        if (strstr(argv[1], ".lat") || argv[1][0] != '-') {
            pf = true;
            break;
        }
        if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
            lat_version_simple();
            return EXIT_SUCCESS;
        } else if (!strcmp(argv[i], "-a") || !strcmp(argv[i], "--ayuda") ||
                   !strcmp(argv[i], "--help")) {
            lat_ayuda();
            return EXIT_SUCCESS;
        } else if (!strcmp(argv[i], "-e")) {
            pe = true;
        }
    }
    lat_mv *mv = latC_crear_mv();
    /* debug eliminado: MV creada */
    if (pe) {
        /* Ejecuta una cadena de codigo */
        if (argc != 3) {
            printf("Error: Se requiere una cadena para ejecución.\n");
            return EXIT_FAILURE;
        }
        char *cmd = malloc(MAX_INPUT_SIZE);
        strcpy(cmd, argv[2]);
        latC_apilar(mv, latC_crear_cadena(mv, cmd));
        /* debug eliminado: Ejecutando cadena -e */
        str_ejecutar(mv);
        /* debug eliminado: Cadena ejecutada */
        free(cmd);
    } else if (argc > 1 && pf) {
        /* Ejecuta un archivo de codigo */
        infile = argv[1];
        /* debug eliminado: Archivo de entrada */
        mv->nombre_archivo = infile;
        mv->global->REPL = false;
        mv->global->argc = argc - 1;
        for (int j = 1; j < argc; j++) {
            latL_agregar(mv, latC_checar_lista(mv, mv->global->argv),
                         latC_crear_cadena(mv, argv[j]));
        }
        int status;
        /* debug eliminado: Iniciando parseo */
        ast *nodo = latA_analizar_arch(infile, &status);
        /* debug eliminado: Parseo terminado */
        if (status == 0 && nodo != NULL) {
            /* debug eliminado: Analizando AST a bytecode */
            lat_objeto *main_func = latC_analizar(mv, nodo);
            /* debug eliminado: Función principal creada */
            if (mv->global->menu) {
                // inicio instrucciones para llamar a menu
                lat_funcion *fval = (lat_funcion *)main_func->val.gc;
                lat_bytecode *codigo = (lat_bytecode *)fval->codigo;
                int ninst = main_func->ninst;
                lat_bytecode *bc = (lat_bytecode *)latM_asignar(
                    NULL, sizeof(lat_bytecode) * (ninst + 2));
                memcpy(bc, codigo, latM_tamanio(bc));
#if DEPURAR_MEM
                printf("main.bc: %p\n", bc);
#endif
                bc[ninst - 1] = latMV_bytecode_crear(
                    LOAD_NAME, 0, 0, latC_crear_cadena(mv, "menu"), 0, 0,
                    mv->nombre_archivo);
                bc[ninst] = latMV_bytecode_crear(CALL_FUNCTION, 2, 0, NULL, 0,
                                                 0, mv->nombre_archivo);
                bc[ninst + 1] = latMV_bytecode_crear(HALT, 0, 0, NULL, 0, 0,
                                                     mv->nombre_archivo);
                lat_objeto *num = latC_crear_numerico(mv, mv->global->argc);
                latC_apilar(mv, num);
                latC_apilar(mv, mv->global->argv);
                // fin instrucciones
                lat_objeto *new_main = latC_crear_funcion(mv, bc, ninst + 2);
                new_main->es_vararg = 0;
                new_main->nparams = 2;
                /* debug eliminado: Ejecutando menú */
                status = latC_llamar_funcion(mv, new_main);
                /* debug eliminado: Menú finalizado */
                latO_destruir(mv, new_main);
            } else {
                /* debug eliminado: Ejecutando función principal */
                status = latC_llamar_funcion(mv, main_func);
                /* debug eliminado: Ejecución finalizada */
            }
            latO_destruir(mv, main_func);
        }
        latA_destruir(nodo);
    } else {
        lat_version();
        /* debug eliminado: Iniciando REPL */
        latR_REPL(mv);
    }
    latC_destruir_mv(mv);
    return EXIT_SUCCESS;
}
