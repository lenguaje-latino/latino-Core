# Latino Core

![Build Installers](https://github.com/lenguaje-latino/latino-Core/actions/workflows/release.yml/badge.svg)

Núcleo del lenguaje de programación Latino.

## 📥 Descargas

Puedes descargar la última versión compilada para tu sistema operativo desde la sección de [Releases](https://github.com/lenguaje-latino/latino-Core/releases/latest).

| Sistema Operativo | Instalador |
|-------------------|------------|
| **Windows**       | `.exe` (Instalador con asistente) |
| **macOS**         | `.dmg` (Imagen de disco) |
| **Linux (Debian/Ubuntu)** | `.deb` |
| **Linux (Fedora/RHEL)**   | `.rpm` |

## ¿Qué contiene?

Este núcleo contiene todos los archivos que definen la sintaxis de Latino, sus comandos y librerías internas. Este núcleo por si solo es todo lo que se necesita si se desea instalar Latino para ser utilizado en una terminal.

## Requisitos del Sistema

### Windows
- **CMake** 3.10 o superior - [Descargar CMake](https://cmake.org/download/)
- **MinGW-w64** o **Visual Studio 2017+** - [Descargar MinGW](https://www.mingw-w64.org/)
- **Git** - [Descargar Git](https://git-scm.com/download/win)

### Linux
```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake git

# Fedora/RHEL
sudo dnf install gcc gcc-c++ cmake git

# Arch Linux
sudo pacman -S base-devel cmake git
```

### macOS
```bash
# Instalar Homebrew si no lo tienes
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Instalar dependencias
brew install cmake git
```

## Compilación

### 1. Clonar el Repositorio

```bash
git clone https://github.com/lenguaje-latino/latino-Core.git
cd latino-Core
```

### 2. Crear Directorio de Compilación

```bash
mkdir build
cd build
```

### 3. Configurar con CMake

**Windows (MinGW):**
```bash
cmake -G "MinGW Makefiles" ..
```

**Windows (Visual Studio):**
```bash
cmake -G "Visual Studio 16 2019" ..
```

**Linux/macOS:**
```bash
cmake ..
```

### 4. Compilar

**Windows (MinGW):**
```bash
mingw32-make
```

**Windows (Visual Studio):**
```bash
cmake --build . --config Release
```

**Linux/macOS:**
```bash
make
```

### 5. Ejecutar Latino

Después de compilar, el ejecutable estará en:
- **Windows**: `build\latino.exe`
- **Linux/macOS**: `build/latino`

```bash
# Windows
.\latino.exe

# Linux/macOS
./latino
```

## Pruebas

### Ejecutar Pruebas Básicas

**Prueba 1: REPL Interactivo**
```bash
# Ejecutar Latino en modo interactivo
./latino

# Dentro del REPL, prueba:
escribir("¡Hola, Latino!")
x = 10 + 5
escribir(x)
salir()
```

**Prueba 2: Ejecutar un Script**

Crea un archivo `prueba.lat`:
```latino
# prueba.lat
escribir("=== Prueba de Latino ===")

# Variables y operaciones
a = 10
b = 20
suma = a + b
escribir("Suma: " .. suma)

# Listas
numeros = [1, 2, 3, 4, 5]
escribir("Lista: " .. numeros)

# Diccionarios
persona = {"nombre": "Juan", "edad": 25}
escribir("Persona: " .. persona)

# Funciones
funcion saludar(nombre)
    retornar "Hola, " .. nombre
fin

escribir(saludar("Latino"))
```

Ejecuta el script:
```bash
./latino prueba.lat
```

**Prueba 3: Verificar Optimizaciones de Rendimiento**

Crea un archivo `benchmark.lat`:
```latino
# benchmark.lat - Prueba de rendimiento
escribir("=== Benchmark de Rendimiento ===")

# Prueba 1: String Interning
inicio = tiempo()
desde(i = 0; i < 10000; i++)
    cadena = "prueba_" .. i
fin
fin_str = tiempo()
escribir("Strings: " .. (fin_str - inicio) .. "ms")

# Prueba 2: Hash Tables
inicio = tiempo()
dic = {}
desde(i = 0; i < 1000; i++)
    dic["clave_" .. i] = i
fin
fin_dic = tiempo()
escribir("Diccionario: " .. (fin_dic - inicio) .. "ms")

# Prueba 3: Cache de Variables
inicio = tiempo()
contador = 0
desde(i = 0; i < 100000; i++)
    contador = contador + 1
fin
fin_cache = tiempo()
escribir("Cache vars: " .. (fin_cache - inicio) .. "ms")

escribir("=== Pruebas completadas ===")
```

Ejecuta el benchmark:
```bash
./latino benchmark.lat
```

### Ejecutar Suite de Pruebas (si existe)

```bash
# Si hay pruebas unitarias
cd build
ctest

# O ejecutar pruebas manualmente
./latino ../tests/test_basico.lat
./latino ../tests/test_funciones.lat
```

## Estructura del Proyecto

```
latino-Core/
├── include/              # Archivos de cabecera (.h)
│   ├── latast.h         # AST (Árbol de Sintaxis Abstracta)
│   ├── latcompat.h      # Compatibilidad entre plataformas
│   ├── latdic.h         # Hash tables/diccionarios
│   ├── latdo.h          # Bytecode y ejecución
│   ├── latgc.h          # Garbage Collector
│   ├── latino.h         # Definiciones principales
│   ├── latlex.h         # Analizador léxico
│   ├── latlist.h        # Listas enlazadas
│   ├── latmem.h         # Gestión de memoria
│   ├── latmv.h          # Máquina virtual
│   ├── latobj.h         # Objetos y tipos de datos
│   ├── latparse.h       # Parser/analizador sintáctico
│   └── latrepl.h        # REPL (Read-Eval-Print Loop)
│
├── src/                 # Código fuente (.c)
│   ├── latast.c         # Implementación del AST
│   ├── latbaselib.c     # Librería base (funciones básicas)
│   ├── latcurseslib.c   # Librería curses (interfaz terminal)
│   ├── latdevlib.c      # Librería de desarrollo/debug
│   ├── latdic.c         # Implementación de hash tables
│   ├── latdiclib.c      # Librería de diccionarios
│   ├── latdo.c          # Ejecución de bytecode
│   ├── latfilelib.c     # Librería de archivos
│   ├── latgc.c          # Implementación del GC
│   ├── latino.c         # Punto de entrada principal
│   ├── latino_globals.c # Variables globales
│   ├── latlex.c         # Implementación del lexer
│   ├── latlist.c        # Implementación de listas
│   ├── latlistlib.c     # Librería de listas
│   ├── latmathlib.c     # Librería matemática
│   ├── latmem.c         # Gestión de memoria
│   ├── latmv.c          # Implementación de la VM
│   ├── latnetlib.c      # Librería de red
│   ├── latobj.c         # Implementación de objetos
│   ├── latpaqlib.c      # Librería de paquetes
│   ├── latparse.c       # Implementación del parser
│   ├── latregexlib.c    # Librería de expresiones regulares
│   ├── latrepl.c        # Implementación del REPL
│   ├── latstrlib.c      # Librería de cadenas
│   ├── latsyslib.c      # Librería del sistema
│   └── latino-regex/    # Implementación de regex
│
├── lib/                 # Librerías externas
│   └── linenoise/       # Librería para línea de comandos
│
├── build/               # Archivos de compilación (generado)
│   ├── latino.exe       # Ejecutable (Windows)
│   └── latino           # Ejecutable (Linux/macOS)
│
├── tests/               # Pruebas (si existen)
├── CMakeLists.txt       # Configuración de CMake
├── LICENSE              # Licencia MIT
└── README.md            # Este archivo
```

### Archivos Principales

| Archivo | Descripción |
|---------|-------------|
| `latino.c` | Punto de entrada, inicialización del intérprete |
| `latmv.c` | Máquina virtual, ejecución de bytecode |
| `latobj.c` | Sistema de objetos y tipos de datos |
| `latdic.c` | Hash tables con optimizaciones FNV-1a |
| `latparse.c` | Analizador sintáctico |
| `latlex.c` | Analizador léxico/tokenizador |
| `latgc.c` | Recolector de basura |
| `lat*lib.c` | Librerías estándar (math, string, file, etc.) |


## Optimizaciones de Rendimiento (Rama desarrollo)

La rama `desarrollo` incluye optimizaciones significativas:

- **FNV-1a Hash**: Mejor distribución para string interning
- **Hash Table Dinámico**: Redimensionamiento automático con load factor 0.75
- **Cache de Variables**: Cache LRU de 16 entradas para búsquedas repetidas

Estas optimizaciones mejoran el rendimiento en:
- Creación y búsqueda de cadenas
- Operaciones con diccionarios grandes
- Acceso repetido a variables en bucles

## Solución de Problemas

### Error: "CMake no encontrado"
Asegúrate de que CMake esté en tu PATH:
```bash
cmake --version
```

### Error: "Compilador no encontrado"
**Windows**: Instala MinGW o Visual Studio  
**Linux**: `sudo apt-get install build-essential`  
**macOS**: `xcode-select --install`

### Error al compilar en Windows
Si usas MinGW, asegúrate de usar:
```bash
cmake -G "MinGW Makefiles" ..
mingw32-make
```

## Para Mayor Documentación y Reportar Errores

Mire la siguiente sección en [lenguaje-latino/latino](https://github.com/lenguaje-latino/latino).

## Contribuir

Las contribuciones son bienvenidas. Por favor:
1. Fork el proyecto
2. Crea una rama para tu feature (`git checkout -b feature/AmazingFeature`)
3. Commit tus cambios (`git commit -m 'Add some AmazingFeature'`)
4. Push a la rama (`git push origin feature/AmazingFeature`)
5. Abre un Pull Request

## Licencia

Licenciado bajo la licencia [MIT](https://github.com/lenguaje-latino/latino-core/blob/main/LICENSE)

Cualquier aportación o sugerencia es bienvenida.

## Comunidad

- **Sitio Web**: [lenguaje-latino.org](https://lenguaje-latino.org)
- **Documentación**: [Manual de Latino](https://manual.lenguaje-latino.org)
- **Discord**: [Únete a nuestra comunidad](https://discord.gg/latino)
- **GitHub**: [lenguaje-latino](https://github.com/lenguaje-latino)
## Política de errores (listas y diccionarios)

Esta sección documenta el comportamiento esperado ante operaciones inválidas y asignaciones compuestas en colecciones:

- Índices fuera de rango en listas:
  - Lectura `lista[i]` con `i` fuera de rango produce error: `Indice fuera de rango`.
  - Asignación `lista[i] = v` con `i` fuera de rango también produce error. Para crecer una lista, use las operaciones de inserción (`agregar`, `push`).

- Claves inexistentes en diccionarios:
  - Lectura `dic["k"]` de una clave inexistente debe considerarse error (semántica estricta). Inicialice explícitamente la clave antes de leerla o use una API con valor por defecto.
  - Asignación compuesta sobre clave inexistente `dic["k"] += v` requiere que la clave exista y tenga un valor numérico compatible. Si la clave no existe, inicialícela primero, por ejemplo: `dic["k"] = 0`.

- Asignaciones compuestas sobre subíndices y atributos:
  - Las expresiones del tipo `X[Y] op= Z` y `obj.attr op= Z` se desazucaran a una lectura del destino seguida de la operación y un almacenamiento en el mismo destino.
  - Para `X[Y] op= Z`, el analizador evalúa `X[Y]` y `Z`, aplica `op` y emite `STORE_SUBSCR`.
  - Para `obj.attr op= Z`, el analizador evalúa `obj` y `Z`, aplica `op` y emite `STORE_ATTR`.

Estas reglas favorecen un comportamiento predecible y evitan silencios que oculten errores lógicos; los programas deben inicializar explícitamente elementos y claves antes de operar con asignaciones compuestas.
