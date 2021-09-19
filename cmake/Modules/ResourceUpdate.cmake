if(NOT latCore)
    set(DIR_LATINO_RC   ${CMAKE_SOURCE_DIR}/latino-core/src/vs_resources/latino.rc)
    set(DIR_LATINO_ICO  ${CMAKE_SOURCE_DIR}\\latino-core\\logo\\icon_build.ico)
else()
    set(DIR_LATINO_RC   ${CMAKE_SOURCE_DIR}/src/vs_resources/latino.rc)
    set(DIR_LATINO_ICO  ${CMAKE_SOURCE_DIR}\\logo\\icon_build.ico)
endif()

set(LATINO_VERSION_COMAS    ${LAT_VER_MAYOR},${LAT_VER_MENOR},${LAT_VER_PARCHE},0)

file(READ ${DIR_LATINO_RC} _LATINO_RC)

string(REGEX REPLACE    "FILEVERSION ([0-9]*),([0-9]*),([0-9]*),([0-9]*)"                   "FILEVERSION ${LATINO_VERSION_COMAS}"           _LATINO_RC ${_LATINO_RC})
string(REGEX REPLACE    "PRODUCTVERSION ([0-9]*),([0-9]*),([0-9]*),([0-9]*)"                "PRODUCTVERSION ${LATINO_VERSION_COMAS}"        _LATINO_RC ${_LATINO_RC})

string(REGEX REPLACE    "\"FileVersion\", \"([0-9]*).([0-9]*).([0-9]*).([0-9]*)?"           "\"FileVersion\", \"${LATINO_VERSION}"          _LATINO_RC ${_LATINO_RC})
string(REGEX REPLACE    "2015\-([0-9]*)"                                                    "2015\-${CURRENT_YEAR}"                          _LATINO_RC ${_LATINO_RC})
string(REGEX REPLACE    "\"ProductVersion\", \"([0-9]*).([0-9]*).([0-9]*).([0-9]*)"         "\"ProductVersion\", \"${LATINO_VERSION}"       _LATINO_RC ${_LATINO_RC})

file(WRITE ${DIR_LATINO_RC} "${_LATINO_RC}")