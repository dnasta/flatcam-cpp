/**
 * @file NccTool.h
 * @brief Herramienta NCC (limpieza de cobre): quita el cobre sobrante (port de `appTools/ToolNCC.py`).
 */
#pragma once

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
 * Desvíos conscientes: solo están la operación "Limpiar", el método estándar, la selección "Sí mismo" y sin mecanizado
 * de resto (lo demás informa que falta); la verificación del diámetro seguro (que solo informa) y la base de datos de
 * herramientas no están.
 */
class NccTool : public NccToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit NccTool(App* app, QWidget* parent = nullptr);

    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();

    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();

    /** @brief Limpia el cobre (`on_ncc_click` → `clear_copper`). @return La Geometría creada, o `nullptr` si falló. */
    FlatCAMObj* clearCopper();

    /** @brief Herramientas de la tabla (`ncc_tools`). @return El almacén. */
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
