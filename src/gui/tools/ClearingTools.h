/**
 * @file ClearingTools.h
 * @brief Lo común a las tablas de herramientas de Paint y NCC (`build_ui`, `update_ui` y el almacén `paint_tools` /
 * `ncc_tools` de `ToolPaint.py` y `ToolNCC.py`).
 */
#pragma once

#include <functional>
#include <map>
#include <optional>
#include <vector>

#include "gui/objects/GeometryObject.h"
#include "io/JsonValue.h"

class QTableWidget;

namespace flatcam::gui {

class MainGUI;

/**
 * @brief Mientras existe, la barra de estado muestra "Ocupado" y el cursor de espera (`proc_container.new`): para los
 * cálculos largos que el original hace en un hilo de trabajo.
 */
class BusyScope {
public:
    /** @brief Enciende el indicador. @param ui Ventana. */
    explicit BusyScope(MainGUI* ui);
    /** @brief Lo apaga. */
    ~BusyScope();
    BusyScope(const BusyScope&) = delete;
    BusyScope& operator=(const BusyScope&) = delete;

private:
    MainGUI* ui_;
};

/**
 * @brief Una herramienta de la tabla de Paint o NCC: la herramienta de la Geometría que se va a crear (con sus parámetros
 * de corte) y los parámetros propios de la herramienta (`tools_paint_*` / `tools_ncc_*` / `tools_iso_*` de su `data`).
 */
struct ClearingTool {
    GeometryTool geo;          ///< Diámetro, tipo y parámetros de corte que pasan a la Geometría.
    double overlap{20.0};      ///< Solape en % (`*_overlap`).
    int method{0};             ///< Método: 0 estándar, 1 semilla, 2 líneas... (`*_method`).
    bool connect{true};        ///< Unir pasadas (`*_connect`).
    bool contour{true};        ///< Contorno (`*_contour`).
    double offset{0.0};        ///< Paint: achique de los polígonos; NCC: agrandado del cobre (`*_offset[_value]`).
    bool offsetChoice{false};  ///< NCC: si se agranda el cobre (`tools_ncc_offset_choice`).
    double margin{1.0};        ///< NCC: margen de la caja (`tools_ncc_margin`).
    QString operation{"clear"};    ///< NCC: `clear` o `iso` (`tools_ncc_operation`).
    QString millingType{"cl"};     ///< NCC: concordancia o convencional (`tools_ncc_milling_type`).
    bool checkValid{true};         ///< NCC: verificar el diámetro seguro (`tools_ncc_check_valid`).
    int passes{1};                 ///< Isolation: pasadas (`tools_iso_passes`).
    QString isoType{"full"};       ///< Isolation: `full`, `ext` o `int` (`tools_iso_isotype`).
    bool follow{false};            ///< Isolation: seguir las líneas centrales (`tools_iso_follow`).
};

/**
 * @brief Arma la tabla de herramientas (`build_ui`): columnas #, Diámetro, TT y el identificador oculto, ordenadas por
 * diámetro según el orden elegido.
 * @param table Tabla.
 * @param tools Herramientas por identificador.
 * @param order `fwd` (ascendente), `rev` (descendente) o `no` (como están).
 */
void buildClearingTable(QTableWidget* table, const std::map<int, ClearingTool>& tools, const QString& order);

/**
 * @brief Identificadores de las filas elegidas, en el orden de la tabla.
 * @param table Tabla.
 * @return Los identificadores.
 */
std::vector<int> selectedClearingTools(const QTableWidget* table);

/** @brief Qué hacer al agregar una herramienta por diámetro, según la base de datos (`on_tool_add`). */
enum class DbAddResult {
    Default,  ///< Agregar la de fábrica (no está en la base, o no se pudo leer).
    Cancel,   ///< No agregar nada (varias en la base, o ya está en la tabla).
    FromDb    ///< Agregar la de la base (en `tool`, con el diámetro `dia`).
};

/**
 * @brief La búsqueda de `on_tool_add` en la base de datos de herramientas, con sus mensajes. Si el diámetro es 0 o ya está
 * en la tabla devuelve `Default` para que el agregado normal dé el aviso de siempre.
 * @param ui Ventana (mensajes).
 * @param dataPath Carpeta de datos (donde está `tools_db.FlatDB`).
 * @param tools Herramientas de la tabla.
 * @param dia Diámetro pedido.
 * @param targetMatches Si una herramienta de la base sirve, según su `tool_target`.
 * @param[out] tool La herramienta de la base.
 * @param[out] dia2 El diámetro a usar (el de la base si se encontró por la tolerancia).
 * @return Qué hacer.
 */
DbAddResult lookupToolForAdd(MainGUI* ui, const QString& dataPath, const std::map<int, ClearingTool>& tools, double dia,
                             const std::function<bool(const JsonValue&)>& targetMatches, JsonValue& tool,
                             double& dia2);

class FlatCAMObj;

/**
 * @brief Diámetro de herramienta seguro de un Gerber (`find_safe_tooldia_worker` de NCC e Isolation): informa "Checking
 * tools for validity." y, si falla, el error del original.
 * @param ui Ventana (para los mensajes).
 * @param obj Objeto elegido (tiene que ser un Gerber).
 * @param name Nombre que se informa si no hay objeto.
 * @return La menor distancia entre figuras del cobre, o nada.
 */
std::optional<double> findSafeToolDia(MainGUI* ui, FlatCAMObj* obj, const QString& name);

/**
 * @brief Diámetros en el orden en que se usan (`sorted_tools` según `order`).
 * @param tools Herramientas.
 * @param ids Identificadores a ordenar.
 * @param order `fwd`, `rev` o `no`.
 * @return Los identificadores ordenados.
 */
std::vector<int> orderedClearingTools(const std::map<int, ClearingTool>& tools, std::vector<int> ids, const QString& order);

}  // namespace flatcam::gui
