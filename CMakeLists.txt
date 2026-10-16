cmake_minimum_required(VERSION 3.20)

project(FlatCAMCpp
        VERSION 0.1.0
        DESCRIPTION "Port a C++17 / Qt6 de FlatCAM 2024.4 (CAM 2D para PCB)"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Tipo de compilación" FORCE)
endif()

option(FLATCAM_BUILD_TESTS "Compilar los tests unitarios (GoogleTest)" ON)
option(FLATCAM_BUILD_DOCS  "Generar la documentación con Doxygen"      ON)

# ---------------------------------------------------------------------------
# Dependencias
# ---------------------------------------------------------------------------
# GEOS >= 3.11 (se necesita GEOSGeom_transformXY_r). Se busca primero el paquete
# CMake que instala GEOS y, si no está, se usa pkg-config.
find_package(GEOS 3.11 CONFIG QUIET)
if(NOT TARGET GEOS::geos_c)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GEOS REQUIRED IMPORTED_TARGET GLOBAL geos>=3.11)
    add_library(GEOS::geos_c ALIAS PkgConfig::GEOS)
endif()

add_subdirectory(src)

if(FLATCAM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(FLATCAM_BUILD_DOCS)
    find_package(Doxygen QUIET)
    if(DOXYGEN_FOUND)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                       ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generando documentación con Doxygen"
            VERBATIM)
    else()
        message(STATUS "Doxygen no encontrado: el target 'docs' no estará disponible")
    endif()
endif()
