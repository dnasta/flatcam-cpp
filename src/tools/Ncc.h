/**
 * @file Ncc.h
 * @brief Limpieza de cobre (Non-Copper Clearing): quita el cobre sobrante de toda la placa (port de `ToolNCC.py`).
 */
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "cam/PolygonClearing.h"

namespace flatcam {

/** @brief Opciones de la limpieza con una herramienta (los `tools_ncc_*` de FlatCAM). */
struct NccOptions {
    double toolDia{0.1};      ///< Diámetro de la herramienta.
    double overlap{0.15};     ///< Solape entre pasadas, de 0 a 1 (`tools_ncc_overlap` / 100).
    bool connect{true};       ///< Unir pasadas vecinas (`tools_ncc_connect`).
    double margin{0.0};       ///< Margen con que se agranda la caja a limpiar (`tools_ncc_margin`).
    bool hasOffset{false};    ///< Agrandar el cobre antes de restarlo (`tools_ncc_offset_choice`).
    double offset{0.0};       ///< Cuánto se agranda el cobre (`tools_ncc_offset_value`).
    int stepsPerCircle{64};   ///< Segmentos por circunferencia (`gerber_circle_steps`).
    PolygonClearing::Method method{PolygonClearing::Method::Standard};  ///< Método (`tools_ncc_method`).
    bool contour{true};       ///< Recorrer el contorno en Semilla y Líneas (`tools_ncc_contour`).
    /// Región a limpiar ya con el margen (selección por área o por objeto de referencia; ver @ref Ncc::areaRegion y
    /// @ref Ncc::referenceRegion). Si falta, se usa la envolvente del cobre agrandada el margen ("Sí mismo").
    std::optional<Shape> region;
    /// Diámetros de las herramientas con la operación "Aislamiento" (`isotooldia`, en el orden de la tabla). Si hay
    /// alguna, el área a limpiar excluye la envolvente de aislamiento de la última (ver @ref Ncc::isolationPaths).
    std::vector<double> isoToolDias;
    bool climb{true};         ///< Fresado en concordancia en el aislamiento (`milling_type_radio` = `cl`).
};

/** @brief Una herramienta de la limpieza de resto (`tools_ncc_*` de su `data`). */
struct NccRestTool {
    double toolDia{0.1};  ///< Diámetro.
    double overlap{0.4};  ///< Solape, de 0 a 1.
    PolygonClearing::Method method{PolygonClearing::Method::Standard};  ///< Método.
};

/**
 * @brief Limpieza de cobre (`clear_copper` y `gen_clear_area_rest` de FlatCAM).
 *
 * La región a limpiar es la caja (la envolvente convexa del cobre en "Sí mismo", o la zona o el objeto de referencia),
 * agrandada el margen, menos el cobre (o, con herramientas de aislamiento, menos su envolvente); cada polígono de esa
 * región se cubre con el método elegido de @ref PolygonClearing.
 */
class Ncc {
public:
    /**
     * @brief Región del cobre sobrante (`calculate_bounding_box`, `apply_margin_to_bounding_box` y `get_ncc_empty_area`).
     * @param solidGeometry Figuras del Gerber.
     * @param options Opciones.
     * @return La región a limpiar, como lista de polígonos (vacía si no hay nada que limpiar).
     * @throws GeometryError si GEOS falla.
     */
    static ShapeList emptyArea(const ShapeList& solidGeometry, const NccOptions& options);

    /**
     * @brief Caja a limpiar con el margen (`calculate_bounding_box` y `apply_margin_to_bounding_box`).
     * @param solidGeometry Figuras del Gerber.
     * @param options Opciones (`region` si la selección es por área o por referencia; si no, "Sí mismo").
     * @return La caja.
     */
    static Shape clearingBox(const ShapeList& solidGeometry, const NccOptions& options);

    /**
     * @brief Recorrido de una herramienta con la operación "Aislamiento" (`get_tool_empty_area` con `isotooldia`): la
     * envolvente del cobre a medio diámetro (`generate_envelope`, invertida si es en concordancia), sus anillos
     * recortados a la caja.
     * @param solidGeometry Figuras del Gerber.
     * @param isoDia Diámetro de la herramienta de aislamiento.
     * @param climb Fresado en concordancia.
     * @param box Caja a limpiar (@ref clearingBox).
     * @param stepsPerCircle Segmentos por cuarto de círculo (`gerber_circle_steps`).
     * @param[out] broken `true` si algún recorrido quedó partido (una `MultiLineString`): el original avisa que el
     *        aislamiento está roto.
     * @return Los recorridos (vacío si no se pudo generar la envolvente).
     */
    static ShapeList isolationPaths(const ShapeList& solidGeometry, double isoDia, bool climb, const Shape& box,
                                    int stepsPerCircle, bool& broken);

    /**
     * @brief Región de la selección por área (`calculate_bounding_box` y `apply_margin_to_bounding_box` con
     * `ncc_select = 1`): cada zona agrandada el margen con esquinas en ingletes, y todo unido.
     * @param areas Zonas elegidas.
     * @param margin Margen.
     * @return La región.
     */
    static Shape areaRegion(const ShapeList& areas, double margin);

    /**
     * @brief Región de la selección por objeto de referencia (`ncc_select = 2`): con un Gerber, la intersección de las
     * envolventes convexas del cobre y de la referencia, agrandada el margen; con una Geometría, cada figura de la
     * referencia agrandada el margen y todo unido.
     * @param copper Cobre del objeto a limpiar.
     * @param reference Figuras del objeto de referencia.
     * @param referenceIsGerber Si la referencia es un Gerber (si no, una Geometría).
     * @param margin Margen.
     * @return La región.
     */
    static Shape referenceRegion(const ShapeList& copper, const ShapeList& reference, bool referenceIsGerber,
                                 double margin);

    /**
     * @brief Limpia todo el cobre sobrante.
     * @param solidGeometry Figuras del Gerber.
     * @param options Opciones.
     * @param[out] paths Trayectorias resultantes, en el orden del original.
     * @param[out] error Mensaje si falla.
     * @return `true` si se generó al menos una trayectoria.
     */
    static bool clearCopper(const ShapeList& solidGeometry, const NccOptions& options, ShapeList& paths,
                            std::string& error);

    /**
     * @brief Limpieza con mecanizado de resto (`gen_clear_area_rest`): las herramientas, de la más grande a la más chica,
     * limpian lo que quedó; a la región se le resta lo limpiado (cada trazado engrosado `tool / 1.9999999`).
     * @param solidGeometry Figuras del Gerber.
     * @param tools Herramientas (se ordenan de mayor a menor).
     * @param common Margen, unir, contorno y agrandado del cobre (`ncc_margin_entry` y los campos `rest_*`).
     * @param[out] perTool Trayectorias de cada herramienta que limpió algo, con su índice en @p tools.
     * @param[out] notCleared Región que quedó sin limpiar.
     * @param[out] error Mensaje si falla.
     * @return `true` si alguna herramienta limpió algo.
     */
    static bool clearCopperRest(const ShapeList& solidGeometry, const std::vector<NccRestTool>& tools,
                                const NccOptions& common, std::vector<std::pair<std::size_t, ShapeList>>& perTool,
                                ShapeList& notCleared, std::string& error);
};

}  // namespace flatcam
