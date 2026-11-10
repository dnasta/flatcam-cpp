/**
 * @file CornersTool.h
 * @brief Herramienta Marcas de esquina (port de `appTools/ToolCorners.py`).
 */
#pragma once

#include "gui/generated/CornersToolUIBase.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/**
 * @brief Panel y lógica de Marcas de esquina: "Agregar marcas" crea `<nombre>_corners` (el Gerber con las marcas en las
 * esquinas elegidas) y "Crear Excellon" crea `<nombre>_corner_drills` (un agujero en cada esquina). Como el original,
 * "Reiniciar herramienta" no está conectado.
 */
class CornersTool : public CornersToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit CornersTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();
    /** @brief "Agregar marcas" (`add_markers`). @return El Gerber nuevo, o `nullptr`. */
    FlatCAMObj* addMarkers();
    /** @brief "Crear Excellon" (`on_create_drill_object`). @return El Excellon nuevo, o `nullptr`. */
    FlatCAMObj* createDrills();

private:
    FlatCAMObj* gerber() const;
    App* app_;
};

}  // namespace flatcam::gui
