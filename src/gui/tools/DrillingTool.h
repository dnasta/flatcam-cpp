/**
 * @file DrillingTool.h
 * @brief Herramienta de Perforación: genera el CNC Job de taladrado de un Excellon (port de `appTools/ToolDrilling.py`).
 */
#pragma once

#include <map>

#include "cam/CncJob.h"
#include "gui/generated/DrillingToolUIBase.h"
#include "gui/tools/ToolInfo.h"
#include "io/JsonValue.h"

namespace flatcam::gui {

class App;
class ExcellonObject;

/**
 * @brief Panel y lógica de la herramienta de Perforación.
 *
 * Como el original: elige un Excellon, muestra sus herramientas (ordenadas según "Orden de la Herram", con los totales),
 * guarda los parámetros de cada herramienta (los de fábrica, `tools_drill_*`, con su tipo de Python para que el G-code
 * imprima "2" o "2.0" igual que FlatCAM) y los edita en el formulario: lo que se cambia va a las herramientas
 * seleccionadas, y los parámetros comunes (cambio de herramienta, Z final, X,Y final, postprocesador) a todas. "Generar
 * objeto CNCJob" crea `<objeto>_cnc` con el optimizador propio de agujeros (en lugar de OR-Tools).
 *
 * Desvíos conscientes: las áreas de exclusión, la base de datos de herramientas y el taladrado de ranuras como
 * agujeros todavía no están (sus controles se ven pero no actúan); solo se generan los postprocesadores portados
 * (`default` y `GRBL_laser`), los demás informan que faltan.
 */
class DrillingTool : public DrillingToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit DrillingTool(App* app, QWidget* parent = nullptr);

    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();

    /** @brief Prepara la herramienta al abrirla (`run` → `set_tool_ui`): lista de objetos y postprocesadores. */
    void setToolUi();

    /** @brief Genera el CNC Job (`on_cnc_button_click`). @return El objeto creado, o `nullptr` si falló. */
    class FlatCAMObj* generateCnc();

private:
    /// Parámetros de una herramienta (`excellon_tools[tool]['data']`).
    struct ToolData {
        DrillToolSettings s;
        bool drillSlots{false};
        double drillOverlap{0.0};
        bool lastDrill{true};
    };

    static JsonValue drillDataJson(const ToolData& d, const std::string& pp);
    void onObjectChanged();
    void buildToolUi();
    void onRowSelectionChange();
    void storageToForm(const ToolData& d);
    void formToStorage(QObject* sender);
    void onPpChanged();
    std::vector<int> selectedToolIds() const;
    ToolData defaultToolData() const;

    App* app_;
    ExcellonObject* excellon_{nullptr};
    std::map<int, ToolData> storage_;   ///< Por número de herramienta del Excellon.
    std::vector<int> tableOrder_;       ///< Herramienta de cada fila de la tabla.
    /// El orden de `excellon_tools` del original: lo deja cada `build_tool_ui` (y `rebuild_ui` al arrastrar filas).
    std::vector<int> baseOrder_;
    void rebuildUi();
    void loadToolsFromDb();
    double toolDia(int id) const;
    std::map<int, double> diaOverride_;  ///< Diámetros que cambió la base de herramientas (replace_tools).

public:
    /**
     * @brief `process_slot_as_drills`: los agujeros con que se taladra una ranura.
     * @param start Inicio. @param stop Fin. @param overlap Paso entre agujeros (`(1 - solape) * diámetro`).
     * @param addLastPoint "Último taladro": agregar el final.
     * @return Las posiciones.
     */
    static std::vector<Point2D> slotAsDrills(const Point2D& start, const Point2D& stop, double overlap,
                                             bool addLastPoint);

private:
    bool muted_{false};
};

}  // namespace flatcam::gui
