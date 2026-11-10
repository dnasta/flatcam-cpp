/**
 * @file PunchTool.h
 * @brief Herramienta Perforar Gerber (port de `appTools/ToolPunchGerber.py`).
 */
#pragma once

#include "gui/generated/PunchToolUIBase.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/**
 * @brief Panel y lógica de Perforar Gerber: la tabla con las aperturas de los tipos elegidos (al armarla quedan todas
 * elegidas, como el original) y "Perforar Gerber", que crea `<nombre sin extensión>_punched` con los agujeros
 * (con un Excellon, un diámetro fijo, un anillo o una proporción del pad). Desvío: la columna "M" (marcar las aperturas en
 * el dibujo) no dibuja nada.
 */
class PunchTool : public PunchToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit PunchTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();
    /** @brief Arma la tabla de aperturas (`build_tool_ui`). */
    void buildToolUi();
    /** @brief "Perforar Gerber" (`on_generate_object`). @return El Gerber nuevo, o `nullptr`. */
    FlatCAMObj* punch();

private:
    void onMethod(const QString& v);
    void onSelectAll(bool state);
    FlatCAMObj* gerber() const;
    App* app_;
    bool muted_{false};
};

}  // namespace flatcam::gui
