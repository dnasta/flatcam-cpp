/**
 * @file Paint.h
 * @brief Herramienta de pintado: cubre polígonos con trayectorias (port de `appTools/ToolPaint.py`).
 */
#pragma once

#include <string>

#include "cam/PolygonClearing.h"
#include "parsers/Gerber.h"

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
 * @brief Pintado de polígonos (`paint_poly_all`, `paint_geo` y `paint_polygon_worker` de FlatCAM).
 *
 * Los métodos Estándar, Semilla, Líneas y Combo están en @ref PolygonClearing; "Líneas láser" necesita las aperturas
 * del Gerber y está en @ref laserLines. La selección por área, polígono u objeto de referencia la arma la interfaz
 * (recorta la geometría antes de llamar a @ref paintAll).
 */
class Paint {
public:
    /**
     * @brief Pinta todos los polígonos.
     * @param solidGeometry Figuras del objeto (lista o figuras múltiples; los anillos cerrados pasan a polígonos).
     * @param options Opciones del pintado.
     * @param[out] paths Trayectorias resultantes, en el orden del original.
     * @param[out] error Mensaje si falla.
     * @param gerber Gerber de origen (solo para "Líneas láser", que usa sus aperturas).
     * @return `true` si se generó al menos una trayectoria.
     */
    static bool paintAll(const ShapeList& solidGeometry, const PaintOptions& options, ShapeList& paths,
                         std::string& error, const Gerber* gerber = nullptr);

    /**
     * @brief "Líneas láser" (`paint_method == 3` de `paint_polygon_worker`): pinta un polígono siguiendo las líneas
     * centrales de los trazos del Gerber que caen dentro, para que el láser haga recorridos largos.
     *
     * Los destellos (pads) que caen dentro se pintan con Semilla (circulares y oblongos) o Líneas (rectangulares),
     * siempre con contorno; cada trazo, con @ref fillWithLines; las líneas de los trazos se unen (`linemerge` de la
     * unión). Reproduce los fallos del original, en los que todo el polígono queda sin pintar: una apertura con geometría
     * y sin tamaño (macros, `KeyError`), un destello que no se puede pintar, una línea central que no es una línea, o
     * que la unión de las líneas de los trazos dé una sola línea (`linemerge` no la acepta).
     * @param polygon Polígono a pintar.
     * @param gerber Gerber del que salió (sus aperturas, en el orden del original).
     * @param options Opciones (diámetro, solape, unir, contorno, segmentos).
     * @return Las trayectorias, o nada si no se pudo pintar.
     */
    static std::optional<ShapeList> laserLines(const Shape& polygon, const Gerber& gerber, const PaintOptions& options);

    /**
     * @brief `Geometry.fill_with_lines`: líneas paralelas a la línea central de un trazo, dentro del trazo achicado
     * medio diámetro de herramienta, más su contorno; con @p connect se unen como en `paint_connect`.
     * @param line Línea central (si no es una línea, el original devuelve None: aquí nada).
     * @param apertureSize Ancho del trazo.
     * @param tooldia Diámetro de la herramienta.
     * @param stepsPerCircle Segmentos por cuarto de círculo.
     * @param overlap Solape, de 0 a 1.
     * @param connect Unir líneas.
     * @param contour Agregar el contorno.
     * @return Las líneas, o nada.
     */
    static std::optional<std::vector<EndpointStorage::Line>> fillWithLines(const Shape& line, double apertureSize,
                                                                           double tooldia, int stepsPerCircle,
                                                                           double overlap, bool connect, bool contour);

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
     * @param gerber Gerber de origen (solo para "Líneas láser").
     * @return `true` si alguna herramienta pintó algo.
     */
    static bool paintRest(const ShapeList& geometry, const std::vector<PaintRestTool>& tools, double offset,
                          PolygonClearing::Method method, int stepsPerCircle,
                          std::vector<std::pair<std::size_t, ShapeList>>& perTool, std::string& error,
                          const Gerber* gerber = nullptr);
};

}  // namespace flatcam
