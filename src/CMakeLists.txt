# Biblioteca del núcleo: geometría, matemática de arcos y utilidades.
# No depende de Qt, de modo que se puede probar sin interfaz gráfica.
add_library(flatcam_core STATIC
    core/AffineTransform.cpp
    core/ArcMath.cpp
    core/Geometry.cpp
    core/Shape.cpp
    core/Types.cpp
    core/internal/GeosContext.cpp
    parsers/ApertureMacro.cpp
    parsers/Gerber.cpp
    parsers/SvgImport.cpp
    parsers/DxfImport.cpp
    parsers/Hpgl2.cpp
    parsers/Excellon.cpp
    cam/PyNumber.cpp
    io/FileExport.cpp
    io/SvgExport.cpp
    io/DxfExport.cpp
    cam/Preprocessor.cpp
    cam/MorePreprocessors.cpp
    cam/PathOptimizer.cpp
    cam/CncJob.cpp
    cam/EndpointStorage.cpp
    cam/PolygonClearing.cpp
    cam/ExclusionAreas.cpp
    tools/CutOut.cpp
    tools/Paint.cpp
    tools/Ncc.cpp
    tools/DoubleSided.cpp
    tools/Isolation.cpp
    tools/ExcellonMilling.cpp
    tools/Panelize.cpp
    tools/InvertGerber.cpp
    tools/EtchCompensation.cpp
    tools/AlignObjects.cpp
    tools/Fiducials.cpp
    tools/CopperThieving.cpp
    tools/RulesCheck.cpp
    tools/SolderPaste.cpp
    tools/PcbWizard.cpp
    tools/Calibration.cpp
    editors/EditorStorage.cpp
    editors/GeoEditorModel.cpp
    editors/ExcEditorModel.cpp
    editors/GrbEditorModel.cpp
    tools/QrCode.cpp
    tools/CornerMarkers.cpp
    tools/ExtractDrills.cpp
    tools/PunchGerber.cpp
    tools/Subtract.cpp
    tools/OptimalDistance.cpp
    io/JsonValue.cpp
    io/ProjectFile.cpp
)
add_library(flatcam::core ALIAS flatcam_core)

target_include_directories(flatcam_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(flatcam_core PUBLIC GEOS::geos_c LibLZMA::LibLZMA)
target_compile_features(flatcam_core PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(flatcam_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(flatcam_core PRIVATE -Wall -Wextra -Wpedantic)
endif()
