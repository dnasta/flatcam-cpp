/**
 * @file CutOutTool.h
 * @brief Herramienta de Corte: contorno con puentes alrededor de un Gerber o una Geometría (port de
 * `appTools/ToolCutOut.py`).
 */
#pragma once

#include "gui/generated/CutOutToolUIBase.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/**
 * @brief Panel y lógica de la herramienta de Corte.
 *
 * Como el original: elige un Gerber o una Geometría (el activo si hay uno), y "Generar geometría de forma libre" o
 * "Generar geometría rectangular" crean `<objeto>_cutout`, una Geometría con una herramienta del diámetro elegido cuyos
 * parámetros son los `geometry_*` de fábrica con la Z de corte y la profundidad múltiple del panel (`cut_tool_dict`).
 * Con puentes finos agrega la herramienta 9999 con la Z de los puentes.
 *
 * Desvíos conscientes: los "mouse bites" no crean todavía su Excellon de agujeros; los puentes manuales, la base de datos
 * de herramientas y el cursor grande no están (sus controles se ven pero no actúan).
 */
class CutOutTool : public CutOutToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit CutOutTool(App* app, QWidget* parent = nullptr);

    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();

    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`): objeto activo y valores de fábrica. */
    void setToolUi();

    /** @brief Corte libre (`on_freeform_cutout`). @return La Geometría creada, o `nullptr` si falló. */
    FlatCAMObj* freeformCutout();

    /** @brief Corte rectangular (`on_rectangular_cutout`). @return La Geometría creada, o `nullptr` si falló. */
    FlatCAMObj* rectangularCutout();

private:
    void onTypeObjChanged();
    FlatCAMObj* cutout(bool rectangular);

    App* app_;
};

}  // namespace flatcam::gui
