# Interfaz gráfica de FlatCAM-cpp (Qt6). La ventana principal sale de tools/gui_dump/gen_cpp.py (generated/).
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(flatcam_gui STATIC
    Translation.cpp
    PlotCanvas.cpp
    MainGUI.cpp
    App.cpp
    AppProjectMenu.cpp
    AppRecent.cpp
    AppCanvasSelection.cpp
    AppEditMenu.cpp
    AppJoin.cpp
    AppConvert.cpp
    AppHelp.cpp
    AppExport.cpp
    AppImport.cpp
    AppScripts.cpp
    BookmarkManager.cpp
    shell/FCShell.cpp
    shell/TclCommand.cpp
    shell/TclCommands.cpp
    shell/TclCommandsApp.cpp
    AppTextEditor.cpp
    objects/ScriptObject.cpp
    generated/ScriptObjectUIBase.cpp
    generated/DocumentObjectUIBase.cpp
    widgets/FCTextEdits.cpp
    CanvasPicker.cpp
    ExclusionAreasManager.cpp
    ToolsDatabase.cpp
    ToolsDBTab.cpp
    ProjectIO.cpp
    objects/FlatCAMObj.cpp
    objects/ObjectCollection.cpp
    objects/ObjectUIs.cpp
    tools/ToolPanels.cpp
    tools/DrillingTool.cpp
    tools/CutOutTool.cpp
    tools/ClearingTools.cpp
    tools/PaintTool.cpp
    tools/NccTool.cpp
    tools/DblSidedTool.cpp
    tools/PanelizeTool.cpp
    tools/InvertTool.cpp
    tools/CalculatorsTool.cpp
    tools/CornersTool.cpp
    tools/ExtractDrillsTool.cpp
    tools/PunchTool.cpp
    tools/SubTool.cpp
    tools/OptimalTool.cpp
    generated/OptimalToolUIBase.cpp
    tools/EtchTool.cpp
    generated/EtchToolUIBase.cpp
    tools/AlignTool.cpp
    generated/AlignToolUIBase.cpp
    tools/FiducialsTool.cpp
    generated/FiducialsToolUIBase.cpp
    tools/ThievingTool.cpp
    generated/ThievingToolUIBase.cpp
    AppEditors.cpp
    objects/DocumentObject.cpp
    tools/RulesTool.cpp
    tools/SolderPasteTool.cpp
    tools/PcbWizardTool.cpp
    tools/CalibrationTool.cpp
    tools/FilmTool.cpp
    tools/ImageTool.cpp
    generated/ImageToolUIBase.cpp
    objects/CNCAutolevel.cpp
    generated/RulesToolUIBase.cpp
    generated/SolderPasteToolUIBase.cpp
    generated/PcbWizardToolUIBase.cpp
    generated/CalibrationToolUIBase.cpp
    generated/FilmToolUIBase.cpp
    editors/GeoEditor.cpp
    editors/ExcEditor.cpp
    editors/GrbEditor.cpp
    editors/GCodeEditor.cpp
    generated/GeoEditorUIBase.cpp
    generated/GeoPaintToolUIBase.cpp
    generated/GeoTextToolUIBase.cpp
    generated/GeoTransformToolUIBase.cpp
    generated/ExcEditorUIBase.cpp
    generated/GrbEditorUIBase.cpp
    generated/GCodeEditorUIBase.cpp
    generated/GrbTransformToolUIBase.cpp
    tools/QRCodeTool.cpp
    generated/QRCodeToolUIBase.cpp
    generated/SubToolUIBase.cpp
    generated/PunchToolUIBase.cpp
    generated/ExtractDrillsToolUIBase.cpp
    generated/CornersToolUIBase.cpp
    generated/CalculatorsToolUIBase.cpp
    generated/InvertToolUIBase.cpp
    generated/PanelizeToolUIBase.cpp
    tools/TransformTool.cpp
    tools/DistanceTool.cpp
    tools/MoveTool.cpp
    tools/PropertiesTool.cpp
    tools/IsolationTool.cpp
    generated/IsolationToolUIBase.cpp
    generated/DblSidedToolUIBase.cpp
    generated/TransformToolUIBase.cpp
    generated/DistanceToolUIBase.cpp
    generated/DistanceMinToolUIBase.cpp
    generated/PropertiesToolUIBase.cpp
    objects/GeometryObject.cpp
    objects/CNCJobObject.cpp
    generated/MainGUIBase.cpp
    generated/ToolsDB2UIBase.cpp
    generated/GerberObjectUIBase.cpp
    generated/ExcellonObjectUIBase.cpp
    generated/GeometryObjectUIBase.cpp
    generated/CNCObjectUIBase.cpp
    generated/NccToolUIBase.cpp
    generated/PaintToolUIBase.cpp
    generated/DrillingToolUIBase.cpp
    generated/CutOutToolUIBase.cpp
    generated/AboutDialogUIBase.cpp
    generated/HowtoDialogUIBase.cpp
    generated/PreferencesTabUIBase.cpp
    Preferences.cpp
    PreferencesGroups.cpp
    AppUnits.cpp
    widgets/FCButton.h
    widgets/FCSpinners.cpp
    widgets/FCComboBox.cpp
    widgets/RadioSet.cpp
    widgets/FCPrefWidgets.cpp
    widgets/DialogBoxRadio.cpp
    widgets/FCTable.cpp
    widgets/NumericalEval.cpp
    widgets/FCLabel.cpp
    widgets/FCEntry.cpp
    widgets/FCCheckBox.h
    widgets/FCDock.h
    widgets/FCTree.cpp
    widgets/KeySensitiveListView.cpp
    widgets/VerticalScrollArea.cpp
    widgets/FCDetachableTab.cpp
    widgets/FlatCAMInfoBar.cpp
    widgets/FlatCAMActivityView.cpp
    resources/flatcam.qrc
)
target_include_directories(flatcam_gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
# El núcleo usa identificadores como `slots` (ExcellonTool::slots, como en FlatCAM): sin las palabras clave de Qt no se
# convierten en macros y las estructuras del núcleo tienen el mismo tamaño en la GUI. Se usa Q_SIGNALS/Q_SLOTS/Q_EMIT.
target_compile_definitions(flatcam_gui PUBLIC QT_NO_KEYWORDS)
target_link_libraries(flatcam_gui PUBLIC flatcam::core Qt6::Widgets Qt6::OpenGLWidgets Qt6::Svg Qt6::PrintSupport)
# La consola Tcl (ToolShell) usa el intérprete Tcl real (8.6 o 9; el FlatCAM en Python usa el 8.6 de tkinter).
# pkg-config `tcl` (en macOS, el tcl-tk de Homebrew); si no, FindTCL de CMake.
find_package(PkgConfig QUIET)
if(APPLE AND EXISTS "/opt/homebrew/opt/tcl-tk/lib/pkgconfig")
    set(ENV{PKG_CONFIG_PATH} "/opt/homebrew/opt/tcl-tk/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
endif()
if(PkgConfig_FOUND)
    pkg_check_modules(FLATCAM_TCL QUIET IMPORTED_TARGET tcl)
endif()
if(TARGET PkgConfig::FLATCAM_TCL)
    target_link_libraries(flatcam_gui PUBLIC PkgConfig::FLATCAM_TCL)
    message(STATUS "Tcl ${FLATCAM_TCL_VERSION} (pkg-config) para la consola Tcl")
else()
    find_package(TCL)
    if(NOT TCL_FOUND)
        message(FATAL_ERROR "Falta Tcl (8.6 o 9) para la consola Tcl: brew install tcl-tk / apt install tcl-dev / vcpkg tcl")
    endif()
    target_include_directories(flatcam_gui PUBLIC ${TCL_INCLUDE_PATH})
    target_link_libraries(flatcam_gui PUBLIC ${TCL_LIBRARY})
    message(STATUS "Tcl (FindTCL): ${TCL_LIBRARY}")
endif()
# GRBL de la autonivelación: QtSerialPort es opcional (sin él, no se listan ni abren puertos).
find_package(Qt6 QUIET COMPONENTS SerialPort)
if(TARGET Qt6::SerialPort)
    target_link_libraries(flatcam_gui PUBLIC Qt6::SerialPort)
    target_compile_definitions(flatcam_gui PRIVATE FLATCAM_HAVE_SERIALPORT)
endif()
if(MSVC)
    target_compile_options(flatcam_gui PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(flatcam_gui PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(flatcam MACOSX_BUNDLE WIN32 main.cpp)
target_link_libraries(flatcam PRIVATE flatcam_gui)
set_target_properties(flatcam PROPERTIES
    OUTPUT_NAME "FlatCAM"
    MACOSX_BUNDLE_BUNDLE_NAME "FlatCAM"
    MACOSX_BUNDLE_GUI_IDENTIFIER "org.flatcam.cpp")
