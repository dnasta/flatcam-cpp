/**
 * @file PaintTool.h
 * @brief Herramienta Paint: cubre los polígonos de un objeto con trayectorias (port de `appTools/ToolPaint.py`).
 */
#pragma once

#include <map>

#include "gui/generated/PaintToolUIBase.h"
#include "gui/tools/ClearingTools.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/**
 * @brief Panel y lógica de Paint.
 *
 * Como el original: elige un Gerber o una Geometría (el activo si hay uno), arma la tabla de herramientas con los
 * valores de fábrica (`tools_paint_*`), guarda los parámetros de cada herramienta al editarlos y "Generar geometría"
 * pinta todos los polígonos con cada herramienta elegida, en el orden de la tabla, y crea `<objeto>_mt_paint`.
 *
 * Desvíos conscientes: solo están el método estándar y la selección "Todos" (los demás informan que faltan); la base
 * de datos de herramientas no está, así que agregar una herramienta usa siempre los valores por defecto.
 */
class PaintTool : public PaintToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit PaintTool(App* app, QWidget* parent = nullptr);

    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();

    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();

    /** @brief Pinta (`on_paint_button_click`). @return La Geometría creada, o `nullptr` si falló. */
    FlatCAMObj* paint();

    /** @brief Herramientas de la tabla (`paint_tools`). @return El almacén. */
    const std::map<int, ClearingTool>& tools() const { return tools_; }

    /**
     * @brief Agrega una herramienta con los valores por defecto (`on_tool_default_add`).
     * @param dia Diámetro.
     * @return `false` si ya había una igual o el diámetro es cero.
     */
    bool addTool(double dia);

private:
    void onTypeObjChanged();
    void updateUi();
    void formToStorage();
    void rebuild();

    App* app_;
    std::map<int, ClearingTool> tools_;
    bool muted_{false};
};

}  // namespace flatcam::gui
