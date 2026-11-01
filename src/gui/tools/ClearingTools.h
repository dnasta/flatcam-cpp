/**
 * @file ClearingTools.h
 * @brief Lo común a las tablas de herramientas de Paint y NCC (`build_ui`, `update_ui` y el almacén `paint_tools` /
 * `ncc_tools` de `ToolPaint.py` y `ToolNCC.py`).
 */
#pragma once

#include <map>
#include <vector>

#include "gui/objects/GeometryObject.h"

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
 * de corte) y los parámetros propios de la herramienta (`tools_paint_*` / `tools_ncc_*` de su `data`).
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

/**
 * @brief Diámetros en el orden en que se usan (`sorted_tools` según `order`).
 * @param tools Herramientas.
 * @param ids Identificadores a ordenar.
 * @param order `fwd`, `rev` o `no`.
 * @return Los identificadores ordenados.
 */
std::vector<int> orderedClearingTools(const std::map<int, ClearingTool>& tools, std::vector<int> ids, const QString& order);

}  // namespace flatcam::gui
