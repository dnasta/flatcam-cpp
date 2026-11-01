/**
 * @file IsolationTool.h
 * @brief Herramienta Isolation: recorridos alrededor de las pistas de un Gerber (port de `appTools/ToolIsolation.py`).
 */
#pragma once

#include <map>

#include "gui/generated/IsolationToolUIBase.h"
#include "gui/tools/ClearingTools.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/**
 * @brief Panel y lógica de Isolation, sobre el núcleo `flatcam::Isolation`.
 *
 * Como el original: tabla de herramientas con los valores de fábrica (`tools_iso_*`: una de 0,1 mm, una pasada, 10 %),
 * parámetros por herramienta y "Generar geometría": con "Combinar" crea `<objeto>_iso_combined` (o `_iso_rest` con
 * mecanizado de resto), con una herramienta por diámetro; sin combinar, un objeto por pasada
 * (`<objeto>_<diámetro>[_ext|_int]_iso[N]`). En el nivel Básico se ven las mismas opciones que en el original.
 *
 * Desvíos conscientes: la selección por área y por polígono (que se hacen con el mouse en el canvas), la verificación
 * del diámetro seguro (solo informa), la base de datos de herramientas y el botón de buffer todavía no están.
 */
class IsolationTool : public IsolationToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit IsolationTool(App* app, QWidget* parent = nullptr);

    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();

    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();

    /** @brief Aísla el objeto elegido (`on_iso_button_click` → `isolate`). @return Los objetos creados. */
    std::vector<FlatCAMObj*> isolate();

    /** @brief Herramientas de la tabla (`iso_tools`). @return El almacén. */
    const std::map<int, ClearingTool>& tools() const { return tools_; }

    /**
     * @brief Agrega una herramienta con los valores por defecto (`on_tool_default_add`).
     * @param dia Diámetro.
     * @return `false` si ya había una igual o el diámetro es cero.
     */
    bool addTool(double dia);

private:
    void updateUi();
    void formToStorage();
    void rebuild();
    void onRestMachiningCheck(bool state);

    App* app_;
    std::map<int, ClearingTool> tools_;
    bool muted_{false};
    bool oldCombineState_{true};
};

}  // namespace flatcam::gui
