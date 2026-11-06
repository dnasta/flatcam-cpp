/**
 * @file NccTool.h
 * @brief Herramienta NCC (limpieza de cobre): quita el cobre sobrante (port de `appTools/ToolNCC.py`).
 */
#pragma once

#include "io/JsonValue.h"

#include <map>

#include "gui/generated/NccToolUIBase.h"
#include "gui/tools/ClearingTools.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/**
 * @brief Panel y lógica de NCC.
 *
 * Como el original: elige un Gerber o una Geometría (el activo si hay uno), arma la tabla con las herramientas de fábrica
 * (`tools_ncc_tools` = "1.0, 0.5"), guarda los parámetros de cada herramienta al editarlos y "Generar geometría" limpia
 * toda la región sin cobre con cada herramienta elegida, en el orden de la tabla, y crea `<objeto>_ncc`.
 *
 * Métodos Estándar, Semilla, Líneas y Combo; con "Resto" las herramientas limpian, de mayor a menor, lo que dejó la
 * anterior (`gen_clear_area_rest`).
 *
 * Selección "Sí mismo", "Selección de área" (rectángulos o polígonos con el mouse) u "Objeto de referencia".
 *
 * Desvíos conscientes: la operación "Aislamiento", la verificación del diámetro seguro (que solo informa) y la base de
 * datos de herramientas no están.
 */
class NccTool : public NccToolUIBase {
    Q_OBJECT
public:
    /**
     * @brief Botón "Óptimo" (`on_find_optimal_tooldia`): calcula el diámetro seguro del Gerber elegido y lo pone en el
     * campo de la herramienta nueva.
     */
    void findOptimalToolDia();
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit NccTool(App* app, QWidget* parent = nullptr);

    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();

    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();

    /** @brief Limpia el cobre (`on_ncc_click` → `clear_copper`). @return La Geometría creada, o `nullptr` si falló. */
    FlatCAMObj* clearCopper();

    /**
     * @brief Limpia el cobre de una región (`clear_copper` con la selección ya resuelta).
     * @param src Objeto de origen.
     * @param geometry Cobre.
     * @param ids Herramientas elegidas.
     * @param region Región con el margen ("Selección de área" u "Objeto de referencia"), o nada para "Sí mismo".
     * @return La Geometría creada, o `nullptr`.
     */
    FlatCAMObj* clearRegion(FlatCAMObj* src, const ShapeList& geometry, const std::vector<int>& ids,
                            const std::optional<Shape>& region);

    /** @brief Herramientas de la tabla (`ncc_tools`). @return El almacén. */
    const std::map<int, ClearingTool>& tools() const { return tools_; }

    /**
     * @brief Agrega una herramienta con los valores por defecto (`on_tool_default_add`).
     * @param dia Diámetro.
     * @return `false` si ya había una igual o el diámetro es cero.
     */
    bool addTool(double dia);
    /**
     * @brief Agrega una herramienta de la base de datos: la de fábrica y, encima, sus datos.
     * @param tool Herramienta de la base.
     * @param dia Diámetro.
     * @param prefix Prefijo de los datos propios que se copian (vacío: todos).
     * @return `true` si se agregó.
     */
    bool addDbTool(const JsonValue& tool, double dia, const std::string& prefix);

private:
    void onTypeObjChanged();
    void onRestMachiningCheck(bool state);
    void onToggleReference();
    void updateUi();
    /// form_to_storage: el campo @p w que cambió pasa a las herramientas elegidas (solo ese, como el original).
    void formToStorage(QObject* w);
    void rebuild();

    App* app_;
    std::map<int, ClearingTool> tools_;
    bool muted_{false};
};

}  // namespace flatcam::gui
