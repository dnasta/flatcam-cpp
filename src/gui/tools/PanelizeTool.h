/**
 * @file PanelizeTool.h
 * @brief Herramienta Panelizar (port de `appTools/ToolPanelize.py`): copias de un objeto en filas y columnas.
 */
#pragma once

#include "gui/generated/PanelizeToolUIBase.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/**
 * @brief Panel y lógica de Panelizar.
 *
 * Como el original: el objeto (Gerber, Excellon o Geometría) se copia en una grilla de columnas y filas separadas por
 * el ancho y el alto de su caja (o de la de otro objeto) más el espacio; con "Restringir" se quitan columnas y filas
 * hasta que el panel entra en el área. Un Excellon da un Excellon; un Gerber o una Geometría dan un Gerber o una
 * Geometría según "Tipo de panel" (en una Geometría con varias herramientas, "Optimizar" une los tramos de corte que se
 * superponen). El objeto nuevo se llama `<nombre>_panelized`.
 */
class PanelizeTool : public PanelizeToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit PanelizeTool(App* app, QWidget* parent = nullptr);

    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();

    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();

    /** @brief "Panelizar objeto" (`on_panelize`). @return El objeto nuevo, o `nullptr`. */
    FlatCAMObj* panelize();

private:
    void onTypeObjChanged();
    void onTypeBoxChanged();
    void onReferenceChanged(const QString& v);
    App* app_;
};

}  // namespace flatcam::gui
