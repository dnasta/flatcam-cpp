/**
 * @file SubTool.h
 * @brief Herramienta Restar (port de `appTools/ToolSub.py`).
 */
#pragma once

#include "gui/generated/SubToolUIBase.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/**
 * @brief Panel y lógica de Restar: Gerber menos Gerber crea `<objetivo>_sub` (y, con "Borrar fuentes", borra los dos).
 *
 * Error del original que se reproduce: Geometría menos Geometría nunca crea nada. Si el sustraendo es "multigeo" avisa
 * que no puede serlo; si no, falla al copiar las herramientas del objetivo (`for key, v in tools[tool]` recorre las
 * claves de un diccionario como si fueran pares). Como las Geometrías del port no distinguen "multigeo", se toma como
 * tal a un sustraendo con más de una herramienta. La resta en sí está portada (`subtractGeometry`) para cuando se
 * corrija.
 */
class SubTool : public SubToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit SubTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();
    /** @brief "Restar" de Gerber (`on_subtract_gerber_click`). @return El Gerber nuevo, o `nullptr`. */
    FlatCAMObj* subtractGerber();
    /** @brief "Restar" de Geometría (`on_subtract_geo_click`). @return Siempre `nullptr`, como el original. */
    FlatCAMObj* subtractGeometry();

private:
    App* app_;
};

}  // namespace flatcam::gui
