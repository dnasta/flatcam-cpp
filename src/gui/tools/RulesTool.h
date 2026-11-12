/**
 * @file RulesTool.h
 * @brief Verificación de reglas (port de `ToolRulesCheck`, Alt+R) sobre `RulesCheck`: crea el Documento
 * `Rules_check_results` con el informe.
 */
#pragma once

#include "gui/generated/RulesToolUIBase.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/** @brief El panel de Verificación de reglas. */
class RulesTool : public RulesToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app La aplicación. @param parent Padre. */
    explicit RulesTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y pestaña. @return Los textos. */
    static ToolInfo info();
    /** @brief `set_tool_ui`. */
    void setToolUi();
    /** @brief `execute` + `on_tool_finished`. @return El Documento creado, o nulo. */
    FlatCAMObj* execute();

private:
    App* app_;
};

}  // namespace flatcam::gui
