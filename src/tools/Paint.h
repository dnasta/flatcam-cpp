/**
 * @file Paint.h
 * @brief Herramienta de pintado: cubre polígonos con trayectorias (port de `appTools/ToolPaint.py`).
 */
#pragma once

#include <string>

#include "cam/PolygonClearing.h"

namespace flatcam {

/** @brief Opciones del pintado con una herramienta (los `tools_paint_*` de FlatCAM). */
struct PaintOptions {
    double toolDia{0.1};        ///< Diámetro de la herramienta.
    double overlap{0.15};       ///< Solape entre pasadas, de 0 a 1 (`tools_paint_overlap` / 100).
    bool connect{true};         ///< Unir pasadas vecinas (`tools_paint_connect`).
    double offset{0.0};         ///< Los polígonos se achican este valor antes de pintar (`tools_paint_offset`).
    int stepsPerCircle{64};     ///< Segmentos por circunferencia (`geometry_circle_steps`).
};

/**
 * @brief Pintado de todos los polígonos de un objeto con el método estándar.
 *
 * Reproduce `paint_poly_all` y el pintado normal de `paint_geo` de FlatCAM para una herramienta y el método
 * estándar. No están portados todavía el pintado por áreas ni por objeto de referencia, el método de semilla, el
 * de líneas ni el de mecanizado de resto.
 */
class Paint {
public:
    /**
     * @brief Pinta todos los polígonos.
     * @param solidGeometry Figuras del objeto (lista o figuras múltiples; los anillos cerrados pasan a polígonos).
     * @param options Opciones del pintado.
     * @param[out] paths Trayectorias resultantes, en el orden del original.
     * @param[out] error Mensaje si falla.
     * @return `true` si se generó al menos una trayectoria.
     */
    static bool paintAll(const ShapeList& solidGeometry, const PaintOptions& options, ShapeList& paths,
                         std::string& error);

    /**
     * @brief Aplana una geometría en polígonos (`recurse` de `paint_poly_all`).
     *
     * Las figuras múltiples se recorren por partes, se descartan las vacías o inválidas y los anillos cerrados se
     * convierten en polígonos.
     * @param geometry Figuras.
     * @return Las figuras simples, en orden.
     */
    static ShapeList flattenToPolygons(const ShapeList& geometry);
};

}  // namespace flatcam
