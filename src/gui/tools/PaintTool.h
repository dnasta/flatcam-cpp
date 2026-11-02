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
 * Selección "Todos", "Selección de polígono" (clics en el canvas), "Selección de área" (rectángulos o polígonos) u
 * "Objeto de referencia". Métodos Estándar, Semilla, Líneas y Combo; con "Resto" las herramientas pintan, de mayor a menor, lo que dejó la
 * anterior (`job_rest_clear`).
 *
 * Desvíos conscientes: el método "Líneas láser" no está; la base de datos de herramientas tampoco, así que agregar una
 * herramienta usa siempre los valores por defecto.
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

    /**
     * @brief Pinta (`on_paint_button_click`). Con "Selección de polígono" o "Selección de área" empieza a elegir en el
     * canvas y pinta al terminar (devuelve `nullptr`).
     * @return La Geometría creada, o `nullptr` si falló o si se está eligiendo en el canvas.
     */
    FlatCAMObj* paint();

    /**
     * @brief Pinta unas figuras con las herramientas elegidas (`paint_geo`).
     * @param src Objeto de origen (para el nombre).
     * @param geometry Figuras a pintar.
     * @param ids Herramientas, en el orden de la tabla.
     * @return La Geometría creada, o `nullptr`.
     */
    FlatCAMObj* paintGeometry(FlatCAMObj* src, const ShapeList& geometry, const std::vector<int>& ids);

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
    void onRestMachiningCheck(bool state);
    void fillReferenceCombo();
    void onSelection();
    void updateUi();
    /// form_to_storage: el campo @p w que cambió pasa a las herramientas elegidas (solo ese, como el original).
    void formToStorage(QObject* w);
    void rebuild();

    App* app_;
    std::map<int, ClearingTool> tools_;
    bool muted_{false};
};

}  // namespace flatcam::gui
