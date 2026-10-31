/**
 * @file Ncc.h
 * @brief Limpieza de cobre (Non-Copper Clearing): quita el cobre sobrante de toda la placa (port de `ToolNCC.py`).
 */
#pragma once

#include <string>

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
};

/**
 * @brief Limpieza de cobre de todo un Gerber, con el método estándar y una herramienta.
 *
 * Reproduce `clear_copper` de FlatCAM para la selección "Él mismo" (toda la placa) sobre un Gerber. La región a
 * limpiar es la envolvente convexa del cobre, agrandada el margen, menos el cobre; cada polígono de esa región se
 * cubre con @ref PolygonClearing::clearPolygon. No están portados todavía la selección por área ni por objeto de
 * referencia, el mecanizado de resto, el aislamiento previo (`isotooldia`) ni los métodos de semilla, líneas y combinado.
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
     * @brief Limpia todo el cobre sobrante.
     * @param solidGeometry Figuras del Gerber.
     * @param options Opciones.
     * @param[out] paths Trayectorias resultantes, en el orden del original.
     * @param[out] error Mensaje si falla.
     * @return `true` si se generó al menos una trayectoria.
     */
    static bool clearCopper(const ShapeList& solidGeometry, const NccOptions& options, ShapeList& paths,
                            std::string& error);
};

}  // namespace flatcam
