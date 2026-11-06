/**
 * @file ToolsDBTab.h
 * @brief Pestaña "Base de datos de herramientas" (port de `ToolsDB2` de `appDatabase.py`), sobre el panel generado
 * desde el original (`ToolsDB2UIBase`).
 */
#pragma once

#include <functional>
#include <utility>
#include <vector>

#include "gui/generated/ToolsDB2UIBase.h"
#include "io/JsonValue.h"

namespace flatcam::gui {

class App;

/**
 * @brief Editor de la base: lista de herramientas (ID y nombre), su formulario (descripción, fresado, NCC, pintura,
 * aislamiento, perforación y corte, según el objetivo), agregar, copiar, borrar, guardar, importar y exportar, y "Transferir
 * la herramienta" a la herramienta que abrió la pestaña.
 */
class ToolsDBTab : public ToolsDB2UIBase {
    Q_OBJECT
public:
    /**
     * @brief Crea la pestaña y lee la base (`setup_db_ui`).
     * @param app La aplicación.
     * @param onToolRequest Qué hacer con la herramienta elegida ("Transferir la herramienta"; `callback_on_tool_request`).
     * @param parent Padre.
     */
    ToolsDBTab(App* app, std::function<void(const JsonValue&)> onToolRequest, QWidget* parent = nullptr);

    /** @brief La base en memoria (`db_tool_dict`). @return La base. */
    const JsonValue& tools() const { return db_; }
    /** @brief Si hay cambios sin guardar (`tools_db_changed_flag`). @return `true` si los hay. */
    bool changed() const { return changed_; }

    /** @brief Agrega una herramienta (`on_tool_add`). */
    void addTool();
    /** @brief Copia las elegidas (`on_tool_copy`). */
    void copyTools();
    /** @brief Borra las elegidas (`on_tool_delete`). */
    void deleteTools();
    /** @brief Guarda la base (`on_save_tools_db`). @param silent Sin mensaje. */
    void save(bool silent = false);
    /** @brief "Transferir la herramienta" (`on_tool_requested_from_app`). */
    void requestTool();

private:
    void buildDbUi();
    void storageToForm(const JsonValue& tool);
    void onTargetChanged(int target);
    void onWidgetChanged(QWidget* w);
    void markEdited(bool silent = false);
    void calculateToolDia();
    JsonValue* currentTool();

    App* app_;
    std::function<void(const JsonValue&)> onToolRequest_;
    JsonValue db_ = JsonValue::object();
    int currentId_{0};
    bool changed_{false};
    bool muted_{false};
    std::vector<std::pair<const char*, QWidget*>> fields_;  ///< form_fields: clave -> widget.
};

}  // namespace flatcam::gui
