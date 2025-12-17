#[[
/***********************************************************************************
 * Copyright (c) 2015-2021 Lenguaje-Latino, programación con sintaxis al Español.  *
 *                                                                                 *
 *                             CMake_SRC de Latino                                 *
 ***********************************************************************************/
]]


#
# Configuraciones (GLOBALES)
set(CMAKE_EXPORT_COMPILE_COMMANDS           ON  )
set(CMAKE_VERBOSE_MAKEFILE                  TRUE)
set(CMAKE_C_OUTPUT_EXTENSION_REPLACE        ON  )
set(INSTALL_BIN                             bin     )
set(INSTALL_INCLUDE                         include )
set(INSTALL_LIB                             lib     )
set(INSTALL_SHARE                           share   )

message(STATUS "INSTALL_BIN:             ${INSTALL_BIN}")
message(STATUS "INSTALL_INCLUDE:         ${INSTALL_INCLUDE}")
message(STATUS "INSTALL_LIB:             ${INSTALL_LIB}")

# Set the output folder where your program will be created
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#
# Sistemas Operativos
    # ---
    # MS-Windows
if(WIN32)
    #
    # MS-VC configuraciones
    if(MSVC)
        set(CMAKE_C_FLAGS                   "${CMAKE_C_FLAGS} /W4"                          )
        set(CMAKE_C_FLAGS_DEBUG             "${CMAKE_C_FLAGS_DEBUG} /W4"                    )
        set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS  )
    elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_C_FLAGS                   "${CMAKE_C_FLAGS} -Wall -Wextra -std=c11"               )
        set(CMAKE_C_FLAGS_DEBUG             "${CMAKE_C_FLAGS_DEBUG} -g -O0 -Wall -Wextra -std=c11"  )
    endif(MSVC)
else()
    #
    # UNIX
    link_libraries(m)
    link_libraries(readline)
    set(CMAKE_C_FLAGS                       "${CMAKE_C_FLAGS} -D_GNU_SOURCE -Wall -Wextra -std=c11 -rdynamic -ldl -fPIC"        )
    set(CMAKE_C_FLAGS_DEBUG                 "${CMAKE_C_FLAGS} -D_GNU_SOURCE -g -O0 -Wall -Wextra -std=c11 -rdynamic -ldl -fPIC" )
endif()


#
# Arquitectura del build
set(TARGET_ARCH_REGEX                       "^.*-march[= ]([^ ]+).*$")
string(REGEX MATCH                          "${TARGET_ARCH_REGEX}" TARGET_ARCH_MATCH ${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS})

if(TARGET_ARCH_MATCH)
    string(REGEX REPLACE                    "${TARGET_ARCH_REGEX}" "\\1" TARGET_ARCH ${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS})
else()
    set(TARGET_ARCH                         ${CMAKE_HOST_SYSTEM_PROCESSOR})
endif()

message(STATUS "TARGET_ARCH: ........... ${TARGET_ARCH}")


#
# librerias
add_definitions(-D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -DLATINO_CORE)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/linenoise)
add_subdirectory(linenoise)

# Define source files
set(SRC_LIB
    latast.c
    latclass.c
    latdic.c
    latdo.c
    latgc.c
    latlex.c
    latlist.c
    latmem.c
    latmv.c
    latobj.c
    latparse.c
    latlocale.c
    latbaselib.c
    # latcurseslib.c  # Commented out - requires PDCurses library
    latdevlib.c
    latdiclib.c
    latfilelib.c
    latlistlib.c
    latmathlib.c
    latpaqlib.c
    latrepl.c
    latstrlib.c
    latsyslib.c
    latino.c
)

if(WIN32)
    message(STATUS "CURRENT PATH WIN32....... ${CMAKE_CURRENT_SOURCE_DIR}")
    include_directories(latino-regex/src)
    add_subdirectory(latino-regex/src)
    # Incluir recursos de Windows (icono de la aplicación)
    # Esto compila src/vs_resources/latino.rc y embebe el icono en latino.exe
    add_executable(${PROJECT_NAME} ${SRC_LIB} vs_resources/latino.rc)
    # Allow multiple definitions (temporary workaround)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--allow-multiple-definition")
    target_link_libraries(${PROJECT_NAME} LINK_PUBLIC regex2 linenoise)
endif()

if(NOT WIN32)
    target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${CMAKE_DL_LIBS})
endif()

if(MSVC)
    SET ( CMAKE_SHARED_LINKER_FLAGS /MANIFEST:NO )

    message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
    link_directories("${CMAKE_CURRENT_BINARY_DIR}/../latino-regex/src/${CMAKE_BUILD_TYPE}")
    target_link_libraries(${PROJECT_NAME} regex2)
    link_libraries(regex2)
    set(LATINO_DEF latino.def)

    # add_custom_command(TARGET latino POST_BUILD COMMAND
    #     "mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}\\latino_os_version.manifest\"
    #     -outputresource:\"${CMAKE_CURRENT_BINARY_DIR}\\${CMAKE_BUILD_TYPE}\\latino.dll\"\;\#2
    #     COMMENT "Adding custom manifest containing MSVCRT80 dependency...")
endif(MSVC)

# Create dynamic library
add_library(liblatino SHARED ${SRC_LIB} ${LATINO_DEF})
set_target_properties(liblatino PROPERTIES OUTPUT_NAME latino CLEAN_DIRECT_OUTPUT 1)

# message(STATUS "LATINO_BUILD_AS_DLL: ${LATINO_BUILD_AS_DLL}")
if(LATINO_BUILD_AS_DLL)
    if(WIN32)
        target_link_libraries (liblatino regex2 linenoise)
    endif()
    set_target_properties (liblatino PROPERTIES COMPILE_DEFINITIONS LATINO_BUILD_AS_DLL)
endif ()

#Create static library
add_library(liblatino_static STATIC ${SRC_LIB})
target_link_libraries(liblatino_static ${LIBS})


#
# Ubicacion de archivos en UNIX
install(TARGETS     latino
        RUNTIME DESTINATION     ${INSTALL_BIN}                      )

install(TARGETS     liblatino
        RUNTIME DESTINATION     ${INSTALL_BIN}
        LIBRARY DESTINATION     ${INSTALL_LIB}
        ARCHIVE DESTINATION     ${INSTALL_LIB}                      )

install(TARGETS     liblatino_static
        ARCHIVE DESTINATION     ${INSTALL_LIB}                      )

# Dependencias en Windows: linenoise y regex2 (DLL + import/lib)
install(TARGETS     linenoise
        RUNTIME DESTINATION     ${INSTALL_BIN}
        LIBRARY DESTINATION     ${INSTALL_LIB}
        ARCHIVE DESTINATION     ${INSTALL_LIB}                      )

install(TARGETS     regex2
        RUNTIME DESTINATION     ${INSTALL_BIN}
        LIBRARY DESTINATION     ${INSTALL_LIB}
        ARCHIVE DESTINATION     ${INSTALL_LIB}                      )

if(UNIX)
    install(FILES       man/latino.1.gz                 DESTINATION     ${INSTALL_SHARE}/man/man1           )
    install(FILES       gnu-utilities/latino.nanorc     DESTINATION     ${INSTALL_SHARE}/nano               )
    install(FILES       gnu-utilities/latino.desktop    DESTINATION     ${INSTALL_SHARE}/applications       )
endif()

install(FILES       ../logo/Latino-logo.png         DESTINATION     ${INSTALL_SHARE}/latino             )
install(FILES       ../logo/latino.ico              DESTINATION     ${INSTALL_BIN}                      )
install(FILES       ../logo/latino_archivo_Win.ico  DESTINATION     ${INSTALL_BIN}                      )
install(FILES       ../logo/latino.ico              DESTINATION     resources/icons                     )
install(FILES       ../logo/latino_archivo_Win.ico  DESTINATION     resources/icons                     )
install(FILES       ../include/latast.h             DESTINATION     ${INSTALL_INCLUDE} COMPONENT Header )
install(FILES       ../include/latcompat.h          DESTINATION     ${INSTALL_INCLUDE} COMPONENT Header )
install(FILES       ../include/latdic.h             DESTINATION     ${INSTALL_INCLUDE} COMPONENT Header )
install(FILES       ../include/latdo.h              DESTINATION     ${INSTALL_INCLUDE} COMPONENT Header )
install(FILES       ../include/latgc.h              DESTINATION     ${INSTALL_INCLUDE} COMPONENT Header )
install(FILES       ../include/latino.h             DESTINATION     ${INSTALL_INCLUDE} COMPONENT Header )
install(FILES       ../include/latlex.h             DESTINATION     ${INSTALL_INCLUDE} COMPONENT Header )
install(FILES       ../include/latlist.h            DESTINATION     ${INSTALL_INCLUDE} COMPONENT Header )
install(FILES       ../include/latmem.h             DESTINATION     ${INSTALL_INCLUDE} COMPONENT Header )
install(FILES       ../include/latmv.h              DESTINATION     ${INSTALL_INCLUDE} COMPONENT Header )
install(FILES       ../include/latobj.h             DESTINATION     ${INSTALL_INCLUDE} COMPONENT Header )
install(FILES       ../include/latparse.h           DESTINATION     ${INSTALL_INCLUDE} COMPONENT Header )
install(FILES       ../include/latrepl.h            DESTINATION     ${INSTALL_INCLUDE} COMPONENT Header )
install(FILES       ../include/latobj.h             DESTINATION     ${INSTALL_INCLUDE} COMPONENT Header )


#
# Paquete .deb

# ---
# crear paquete .deb https://cmake.org/Wiki/CMake/CPack/Examples/Linux/DEB
# ejecutar en terminal
# make package
if (WIN32)
    # Generador de instalador para Windows (NSIS)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_PACKAGE_NAME "Latino")
    set(CPACK_PACKAGE_VENDOR "Lenguaje-Latino")
    set(CPACK_PACKAGE_VERSION "${LATINO_VERSION}")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Lenguaje de programación en español")
    # Nombre visible del directorio de instalación (más corto)
    # Ajuste del nombre (quitando último .0 si existe)
    string(REGEX REPLACE "\\.0$" "" LATINO_VERSION_SHORT "${LATINO_VERSION}")
    # Directorio de instalación corto para evitar problemas al agregar al PATH
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "Latino")
 set(CPACK_RESOURCE_FILE_LICENSE      "${CMAKE_SOURCE_DIR}/LICENSE")
 # Mostrar página de Información como en 1.4.4
 set(CPACK_RESOURCE_FILE_README       "${CMAKE_SOURCE_DIR}/README.md")
 # Idioma del instalador (MUI) en Español
 set(CPACK_NSIS_INSTALL_LANGUAGES     "Spanish")
    # Empaquetar en directorio de staging (evita escribir en Program Files)
    set(CPACK_SET_DESTDIR ON)
    # Evita rutas absolutas (con unidad) dentro del staging de CPack en Windows
    set(CPACK_PACKAGING_INSTALL_PREFIX "/")
    # Opciones NSIS
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
 # Mostrar la casilla de PATH (UI como 1.4.4); la lógica real se
 # implementa en CPackOptions.cmake.in para evitar el aviso de PATH largo
 set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_PACKAGE_EXECUTABLES       "latino" "Latino")

    # Asegurar nombre del desinstalador para accesos directos
    set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")

    # Iconos del instalador y del acceso directos (usa archivos en logo/)
    # Icono del instalador (cuadro de diálogo MUI)
 # Icono del instalador (preferido respecto al de 1.4.4)
 set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/logo/icon_build.ico")
 set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/logo/icon_build.ico")
    # Asegurar icono del ejecutable del instalador (.exe)
    # Usamos MUI_ICON/MUI_UNICON para el icono del instalador; sin 'Icon' directo
    # Imagen de marca dentro del instalador (bienvenida/finalización)
 set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/logo/Latino-logo.png")
    # Icono del acceso directo instalado (menú inicio)
 set(CPACK_NSIS_INSTALLED_ICON_NAME "bin/latino.exe")
 # Página final con opción "Ejecutar Latino"
 set(CPACK_NSIS_MUI_FINISHPAGE_RUN   "$INSTDIR/bin/latino.exe")

    # Asociar archivos .lat con Latino y establecer icono
    # (se ejecuta al instalar y se elimina al desinstalar)
    # Registrar asociación de archivos .lat (por usuario)
    # Usamos argumentos con corchetes para evitar escapes de comillas.
    # Generar archivo de opciones de CPack con comandos NSIS usando literales [[...]]
    set(CPACK_PROJECT_CONFIG_FILE "${CMAKE_BINARY_DIR}/CPackOptions.cmake")
    configure_file("${CMAKE_SOURCE_DIR}/cmake/CPackOptions.cmake.in" "${CPACK_PROJECT_CONFIG_FILE}" @ONLY)

    # Nombre del archivo del instalador (quitar último .0)
    set(CPACK_PACKAGE_FILE_NAME "Latino-${LATINO_VERSION_SHORT}-win32")

elseif(APPLE)
    # Configuración para macOS
    set(CPACK_GENERATOR "DragNDrop;TGZ")
    set(CPACK_PACKAGE_FILE_NAME "Latino-${LATINO_VERSION}-Darwin")
    set(CPACK_DMG_VOLUME_NAME "Latino ${LATINO_VERSION}")
    set(CPACK_DMG_FORMAT "UDBZ") # Comprimido
    # set(CPACK_DMG_DS_STORE "${CMAKE_SOURCE_DIR}/cmake/MacOSX/DS_Store") # Opcional si se tiene

else()
    # Configuración para Linux (Debian/Ubuntu, RHEL/Fedora)
    set(CPACK_GENERATOR "DEB;RPM;TGZ")
    set(CPACK_PACKAGE_FILE_NAME "latino-${LATINO_VERSION}-Linux")
    
    # Configuración común Linux
    set(CPACK_SET_DESTDIR ON)
    set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")

    # Configuración DEB
    set(CPACK_DEBIAN_PACKAGE_NAME "latino")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "primitivo.roman.montero@gmail.com")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Lenguaje de programación de código abierto para latinos y de habla hispana.")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://lenguajelatino.org/")
    set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON) # Auto-detectar dependencias
    set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION ON)

    # Configuración RPM
    set(CPACK_RPM_PACKAGE_NAME "latino")
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    set(CPACK_RPM_PACKAGE_GROUP "Development/Languages")
    set(CPACK_RPM_PACKAGE_URL "https://lenguajelatino.org/")
    set(CPACK_RPM_PACKAGE_AUTOREQPROV ON) # Auto-detectar dependencias
endif()

include(CPack)

# Forzar prefijo de instalación relativo en el staging de CPack
set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR};${PROJECT_NAME};ALL;/")
