# Tests unitarios del núcleo. Usan GoogleTest del sistema o, si no está instalado, lo descargan.
find_package(GTest QUIET)
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(core_tests
    test_types.cpp
    test_shape.cpp
    test_affine.cpp
    test_arcmath.cpp
    test_geometry.cpp
    test_aperture_macro.cpp
    test_gerber.cpp
    test_gerber_ops.cpp
    test_excellon.cpp
    test_preprocessors.cpp
    test_cnc_job.cpp
    test_cnc_job_geometry.cpp
    test_cutout.cpp
    test_cnc_job_laser.cpp
    test_paint.cpp
    test_ncc.cpp
    test_double_sided.cpp
    test_isolation.cpp
    test_luz_de_noche.cpp
    test_project_file.cpp
    test_clearing_methods.cpp
    test_laser_lines.cpp
    test_ncc_isolation.cpp
    test_export.cpp
    test_svg_export.cpp
    test_svg_import.cpp
    test_dxf_import.cpp
    test_dxf_export.cpp
    test_hpgl2.cpp
    test_rest_machining.cpp
)
target_link_libraries(core_tests PRIVATE flatcam::core GTest::gtest_main)
target_include_directories(core_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(core_tests PRIVATE FLATCAM_TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

include(GoogleTest)
gtest_discover_tests(core_tests)

# Tests de la interfaz (solo si se compila la GUI). Corren sin pantalla: plataforma "offscreen" de Qt.
if(TARGET flatcam_gui)
    add_executable(gui_tests gui/test_gui.cpp)
    target_link_libraries(gui_tests PRIVATE flatcam_gui GTest::gtest)
    target_compile_definitions(gui_tests PRIVATE FLATCAM_TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
    gtest_discover_tests(gui_tests PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endif()
