/**
 * @file InvertTool.h
 * @brief Herramienta Invertir Gerber (port de `appTools/ToolInvertGerber.py`).
 */
#pragma once

#include "gui/generated/InvertToolUIBase.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/**
 * @brief Panel y lógica de Invertir Gerber: un Gerber `<nombre>_inverted` con la caja del objeto (agrandada el margen,
 * con las esquinas elegidas) menos su cobre, todo en la apertura "0" (tipo C, tamaño 0), como el original.
 */
class InvertTool : public InvertToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit InvertTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();
    /** @brief "Invertir Gerber" (`on_grb_invert`). @return El objeto nuevo, o `nullptr`. */
    FlatCAMObj* invert();

private:
    App* app_;
};

}  // namespace flatcam::gui
