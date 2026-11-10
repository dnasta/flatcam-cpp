/**
 * @file ExtractDrillsTool.h
 * @brief Herramienta Extraer taladros (port de `appTools/ToolExtractDrills.py`).
 */
#pragma once

#include "gui/generated/ExtractDrillsToolUIBase.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/**
 * @brief Panel y lógica de Extraer taladros: un Excellon con un agujero en cada flash de los pads elegidos de un Gerber,
 * con un diámetro fijo, el pad menos un anillo o una proporción del pad. El objeto se llama como el Gerber sin la
 * extensión (todo lo que sigue al último punto; sin punto, sin nombre, como el original).
 */
class ExtractDrillsTool : public ExtractDrillsToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit ExtractDrillsTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();
    /** @brief "Extraer taladros" (`on_extract_drills_click`). @return El Excellon nuevo, o `nullptr`. */
    FlatCAMObj* extract();

private:
    void onHoleSizeToggle(const QString& v);
    App* app_;
};

}  // namespace flatcam::gui
