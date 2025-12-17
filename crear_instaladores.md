# Crear instaladores de Latino (Windows, Linux y macOS)

Esta guía explica los requisitos y los pasos para generar:

- Instalador `.exe` para Windows (NSIS)
- Paquetes `.deb` (Debian/Ubuntu) y `.rpm` (Fedora/RHEL)
- Imágenes `.dmg` (DragNDrop) o instalador `.pkg` en macOS
- Paquete portable `.zip` para Windows

La configuración de CMake/CPack ya está integrada en el proyecto.

---

## Requisitos (comunes)

- `CMake` ≥ 3.13 (incluye `CPack`)
- Compilador C:
  - Windows: MinGW-w64 (GCC) o MSVC (Visual Studio)
  - Linux: GCC o Clang
  - macOS: Xcode Command Line Tools
- `git` (opcional, para obtener el código)

## Requisitos adicionales por plataforma

**Windows (NSIS)**
- `NSIS` (Nullsoft Scriptable Install System)
  - Descarga: https://nsis.sourceforge.io/Download
  - Durante la instalación, habilita “Add to PATH” o agrega manualmente la ruta de `makensis.exe` al `PATH`.

**Linux (DEB/RPM)**
- Para `.deb`: `dpkg`, `fakeroot` (recomendado) y opcionalmente `lintian` para validación.
- Para `.rpm`: `rpmbuild` y herramientas de RPM (por ejemplo `rpm-build`).

**macOS (DMG / PKG)**
- Para `.dmg` (DragNDrop): solo CPack.
- Para `.pkg` (`productbuild`): Xcode Command Line Tools y utilidades de empaquetado de Apple. Firma opcional.

---

## Construcción (común)

1) Configura el proyecto en modo Release con prefijo de instalación relativo (esto evita escribir en `Program Files` durante el empaquetado):

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/
```

2) Compila:

```powershell
cmake --build build --config Release -j 8
```

Al compilar en Windows con MinGW, se generan `latino.exe`, `liblatino.dll` y las dependencias `liblinenoise.dll` y `libregex2.dll`. CPack ya empaqueta estas DLLs dentro de `bin`.

---

## Windows

### Construcción con Visual Studio (MSVC)

Si prefieres MSVC en lugar de MinGW, usa el generador de Visual Studio. Ejemplo para VS 2022 y arquitectura x64:

```powershell
# Configurar
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_INSTALL_PREFIX=/

# Compilar en Release
cmake --build build --config Release -j 8

# Empaquetar (desde el directorio build)
cd build
cpack -G ZIP -C Release
cpack -G NSIS -C Release
```

Notas:
- Con MSVC no necesitarás las DLLs del runtime de MinGW (como `libwinpthread-1.dll`).
- El empaquetado con CPack es idéntico: usa `-C Release` para tomar los artefactos de esa configuración.

### Portable `.zip`

Genera un paquete portable con todo lo necesario:

```powershell
cd build
cpack -G ZIP -C Release
```

Salida esperada: `Latino-<version>-win32.zip`, con:
- `bin/latino.exe`, `bin/liblatino.dll`, `bin/liblinenoise.dll`, `bin/libregex2.dll`
- `lib/*.a` (import libraries) y `include/*.h`

Si usas MinGW y Windows solicita DLLs del runtime (p. ej., `libwinpthread-1.dll`, `libgcc_s_seh-1.dll`, `libstdc++-6.dll`), añádelas a `bin/` o compila con MSVC para evitarlas.

### Instalador `.exe` (NSIS)

1) Instala NSIS y asegúrate de tener `makensis` en el `PATH`.
2) Genera el instalador:

```powershell
cd build
cpack -G NSIS -C Release
```

Salida esperada: `Latino-<version>-win32.exe`.

Características configuradas:
- Crea atajo de Inicio para `latino` (`CPACK_PACKAGE_EXECUTABLES`).
- Opción para agregar `bin` al `PATH` (`CPACK_NSIS_MODIFY_PATH ON`).
- Incluye licencia `LICENSE`.

Personalización opcional (añadir en `src/CMakeLists.txt` dentro de `if(WIN32)`):

```cmake
set(CPACK_PACKAGE_ICON           "${CMAKE_SOURCE_DIR}/logo/latino.ico")
set(CPACK_NSIS_MUI_ICON          "${CMAKE_SOURCE_DIR}/logo/latino.ico")
set(CPACK_NSIS_MUI_UNIICON       "${CMAKE_SOURCE_DIR}/logo/latino.ico")
```

---

## Linux

Generadores soportados en este proyecto: `DEB` y `RPM`.

```bash
cd build
# Debian/Ubuntu
cpack -G DEB -C Release

# Fedora/RHEL
cpack -G RPM -C Release
```

Requisitos:
- `.deb`: tener `dpkg` y preferiblemente `fakeroot` instalado para empaquetar sin privilegios.
- `.rpm`: tener `rpmbuild` y utilidades RPM.

Notas:
- Los ficheros se instalan bajo prefijos relativos: `bin`, `lib`, `include`, `share`.
- Puedes inspeccionar el contenido con `dpkg -c <paquete.deb>` o `rpm -qlp <paquete.rpm>`.

---

## macOS

### `.dmg` (DragNDrop)

```bash
cd build
cpack -G DragNDrop -C Release
```

### `.pkg` (productbuild)

```bash
cd build
cpack -G productbuild -C Release
```

Requiere herramientas de Apple (Xcode CLT). Firma y `identifier` son opcionales y se pueden configurar vía variables `CPACK_*`.

---

## Verificación

- Windows (ZIP): ejecuta `bin\latino.exe --version` y prueba un `.lat`.
- Linux/macOS: tras instalar el paquete, ejecuta `latino --version`.
- Revisa que `latino` se instale en `bin` y bibliotecas en `lib`.

---

## Solución de problemas

- "Faltan DLLs" (Windows): coloca las DLLs del runtime de MinGW en `bin` o compila con MSVC.
- "Permisos" (Linux): usa `fakeroot` con CPack si fuera necesario; evita `sudo` al empaquetar.
- "NSIS no encontrado": instala NSIS y asegúrate de que `makensis` esté en el `PATH`.

---

## Versionado

La versión del paquete proviene de `include/latino.h`:

```c
#define LAT_VERSION_MAYOR   1
#define LAT_VERSION_MENOR   5
#define LAT_VERSION_PARCHE  0
```

Tras cambiar, reconfigura el proyecto:

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/
```

---

## Comandos resumidos

```powershell
# Configurar y compilar (común)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/
cmake --build build --config Release -j 8

# Windows: ZIP y NSIS
cpack -G ZIP -C Release
cpack -G NSIS -C Release

# Linux: DEB y RPM
cpack -G DEB -C Release
cpack -G RPM -C Release

# macOS: DMG y PKG
cpack -G DragNDrop -C Release
cpack -G productbuild -C Release
```

