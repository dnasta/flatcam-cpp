cmake_minimum_required(VERSION 3.20)

project(FlatCAMCpp
        VERSION 1.0.0
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
option(FLATCAM_BUILD_GUI   "Compilar la interfaz gráfica (requiere Qt6)" ON)

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

# liblzma (xz): los proyectos de FlatCAM (.FlatPrj) se guardan comprimidos con XZ.
find_package(LibLZMA REQUIRED)

# MSVC: los fuentes están en UTF-8 (textos en español), los archivos generados son grandes, M_PI no está definida
# por defecto y windows.h define min/max como macros.
if(MSVC)
    add_compile_options(/utf-8 /bigobj /Zc:preprocessor /Zc:__cplusplus)
    add_compile_definitions(_USE_MATH_DEFINES NOMINMAX)
endif()

add_subdirectory(src)

# ---------------------------------------------------------------------------
# Interfaz gráfica (Qt6). El núcleo no depende de Qt; si Qt6 no está, solo se compila el núcleo.
# ---------------------------------------------------------------------------
if(FLATCAM_BUILD_GUI)
    find_package(Qt6 6.2 QUIET COMPONENTS Widgets OpenGLWidgets Svg PrintSupport)
    # QtPrintSupport (imprimir desde el editor de texto) arrastra Cups::Cups, que en macOS agrega la carpeta
    # usr/include del SDK como -isystem y rompe los encabezados de libc++: se la quita (el SDK ya está en la ruta).
    if(APPLE AND TARGET Cups::Cups)
        set_property(TARGET Cups::Cups PROPERTY INTERFACE_INCLUDE_DIRECTORIES "")
    endif()
    if(Qt6_FOUND)
        add_subdirectory(src/gui)
    else()
        message(STATUS "Qt6 no encontrado: no se compila la interfaz (FLATCAM_BUILD_GUI)")
    endif()
endif()

# Paquetes de Linux: cpack -G DEB (Debian, Ubuntu) y cpack -G RPM (Fedora, openSUSE...). Las dependencias las calcula
# CPack de las bibliotecas que usa el ejecutable (Qt 6, GEOS, Tcl, FreeType, liblzma de la distribución).
if(UNIX AND NOT APPLE)
    set(CPACK_PACKAGE_NAME "flatcam-cpp")
    set(CPACK_PACKAGE_VENDOR "dnasta")
    set(CPACK_PACKAGE_CONTACT "dnasta <https://github.com/dnasta/flatcam-cpp>")
    set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/dnasta/flatcam-cpp")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "FlatCAM: 2D CAM for making PCBs with CNC routers and lasers (C++/Qt6 port)")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
    set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    set(CPACK_DEBIAN_PACKAGE_SECTION "electronics")
    set(CPACK_RPM_FILE_NAME RPM-DEFAULT)
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    set(CPACK_RPM_PACKAGE_GROUP "Applications/Engineering")
    set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION /usr/share/applications /usr/share/icons /usr/share/icons/hicolor)
    include(CPack)
endif()

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
