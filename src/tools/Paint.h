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
    PolygonClearing::Method method{PolygonClearing::Method::Standard};  ///< Método (`tools_paint_method`).
    bool contour{true};       ///< Recorrer el contorno en Semilla y Líneas (`tools_paint_contour`).
};

/** @brief Una herramienta del pintado de resto (`tools_paint_*` de su `data`). */
struct PaintRestTool {
    double toolDia{0.1};   ///< Diámetro.
    double overlap{0.2};   ///< Solape, de 0 a 1.
    bool connect{true};    ///< Unir pasadas.
    bool contour{true};    ///< Contorno (semilla y líneas).
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

    /**
     * @brief Pintado con mecanizado de resto (`job_rest_clear` de `paint_geo`): las herramientas, de la más grande a la
     * más chica, pintan lo que dejó la anterior.
     *
     * Reproduce dos detalles del original: si algún polígono no se puede pintar con una herramienta, esa herramienta se
     * saltea entera (el original lo hace por una excepción); y el área que sigue suma otra vez cada pedazo sin pintar
     * (`tmp += rest_list`), así que hay pedazos repetidos.
     * @param geometry Figuras a pintar (se aplanan como en `paint_poly_all`).
     * @param tools Herramientas (se ordenan de mayor a menor diámetro).
     * @param offset Achique de los polígonos (`rest_offset_entry`).
     * @param method Método de pintado (`paintmethod_combo`, común a todas las herramientas).
     * @param stepsPerCircle Segmentos por cuarto de círculo.
     * @param[out] perTool Trayectorias de cada herramienta que pintó algo, con su índice en @p tools.
     * @param[out] error Mensaje si falla.
     * @return `true` si alguna herramienta pintó algo.
     */
    static bool paintRest(const ShapeList& geometry, const std::vector<PaintRestTool>& tools, double offset,
                          PolygonClearing::Method method, int stepsPerCircle,
                          std::vector<std::pair<std::size_t, ShapeList>>& perTool, std::string& error);
};

}  // namespace flatcam
