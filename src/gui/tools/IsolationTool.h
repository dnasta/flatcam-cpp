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
class GerberObject;

/**
 * @brief Panel y lógica de Isolation, sobre el núcleo `flatcam::Isolation`.
 *
 * Como el original: tabla de herramientas con los valores de fábrica (`tools_iso_*`: una de 0,1 mm, una pasada, 10 %),
 * parámetros por herramienta y "Generar geometría": con "Combinar" crea `<objeto>_iso_combined` (o `_iso_rest` con
 * mecanizado de resto), con una herramienta por diámetro; sin combinar, un objeto por pasada
 * (`<objeto>_<diámetro>[_ext|_int]_iso[N]`). En el nivel Básico se ven las mismas opciones que en el original.
 *
 * Selección "Todos", "Selección de área" y "Selección de polígono" (con el mouse en el canvas; con "Interiores", los
 * agujeros) u "Objeto de referencia".
 *
 * Desvíos conscientes: la verificación del diámetro seguro (solo informa), la base de datos de herramientas y el botón de
 * buffer todavía no están.
 */
class IsolationTool : public IsolationToolUIBase {
    Q_OBJECT
public:
    /**
     * @brief Botón "Óptimo" (`on_find_optimal_tooldia`): calcula el diámetro seguro del Gerber elegido y lo pone en el
     * campo de la herramienta nueva.
     */
    void findOptimalToolDia();
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit IsolationTool(App* app, QWidget* parent = nullptr);

    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();

    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();

    /** @brief Aísla el objeto elegido (`on_iso_button_click` → `isolate`). @return Los objetos creados. */
    std::vector<FlatCAMObj*> isolate();

    /**
     * @brief Aísla con la selección ya resuelta (`isolate`).
     * @param grb Gerber.
     * @param ids Herramientas elegidas.
     * @param geometry Figuras a aislar en lugar del cobre (polígonos elegidos u objeto de referencia), o nulo.
     * @param limitedArea Zona a la que se recortan los recorridos (selección de área), o vacía.
     * @param negativeDia Aislar hacia adentro (agujeros elegidos con "Interiores").
     * @return Los objetos creados.
     */
    std::vector<FlatCAMObj*> runIsolation(GerberObject* grb, const std::vector<int>& ids, const ShapeList* geometry,
                                          const ShapeList& limitedArea, bool negativeDia);

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
    /// form_to_storage: el campo @p w que cambió pasa a las herramientas elegidas (solo ese, como el original).
    void formToStorage(QObject* w);
    void rebuild();
    void onRestMachiningCheck(bool state);
    void onToggleReference();

    App* app_;
    std::map<int, ClearingTool> tools_;
    bool muted_{false};
    bool oldCombineState_{true};
};

}  // namespace flatcam::gui
