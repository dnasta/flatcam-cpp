/**
 * @file SolderPasteTool.h
 * @brief Pasta de soldar (port de `ToolSolderPaste`, Alt+K) sobre `SolderPaste`: la Geometría de dispensado
 * (`<objeto>_solderpaste`) y su CNC Job (`<nombre>_cnc_solderpaste`) con el postprocesador Paste_1.
 */
#pragma once

#include <map>

#include "gui/generated/SolderPasteToolUIBase.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/** @brief El panel de Pasta de soldar. */
class SolderPasteTool : public SolderPasteToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app La aplicación. @param parent Padre. */
    explicit SolderPasteTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y pestaña. @return Los textos. */
    static ToolInfo info();
    /** @brief `set_tool_ui`: las boquillas de `tools_solderpaste_tools` y los parámetros de fábrica. */
    void setToolUi();
    /** @brief `on_tool_add`. @param dia Diámetro (0: el del campo). */
    void addTool(double dia = 0.0);
    /** @brief `on_create_geo_click`. @return La Geometría creada, o nula. */
    FlatCAMObj* createGeometry();
    /** @brief `on_create_gcode_click`. @return El CNC Job creado, o nulo. */
    FlatCAMObj* createGcode();

private:
    void buildUi();
    void fillCombos();
    App* app_;
    std::map<int, double> tools_;         ///< `tooltable_tools`: número -> diámetro.
    std::map<const FlatCAMObj*, bool> pasteGeometries_;  ///< Las Geometrías con `special_group = 'solder_paste_tool'`.
};

}  // namespace flatcam::gui
