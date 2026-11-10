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
target_link_libraries(flatcam_gui PUBLIC flatcam::core Qt6::Widgets Qt6::OpenGLWidgets)
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
