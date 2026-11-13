/**
 * @file Film.h
 * @brief Film (port de `appTools/ToolFilm.py`): el SVG de la película positiva o negativa de un objeto dentro de la
 * caja de otro (`export_positive`, `export_negative` y `Geometry.export_svg`), con escala, inclinación y espejo, y el
 * perforado de los pads (`generate_positive_punched_film`).
 */
#pragma once

#include <optional>
#include <string>

#include "core/Shape.h"
#include "parsers/Gerber.h"

namespace flatcam {

/** @brief Los parámetros de la película (los de `generate_*_film` y `export_*`). */
struct FilmOptions {
    bool negative{true};                ///< Negativa (`neg`) o positiva.
    double strokeFactor{0.0};           ///< `scale_stroke_factor` (0 o menos: 0,01).
    double scaleX{1.0};                 ///< Escala en X (1: sin escalar).
    double scaleY{1.0};                 ///< Escala en Y.
    std::optional<double> skewX;        ///< Inclinación en X en grados (vacío: `None`).
    std::optional<double> skewY;        ///< Inclinación en Y.
    std::string skewReference{"center"};  ///< `center`, `topleft`, `bottomleft`, `topright` o `bottomright`.
    std::string mirror;                 ///< `x`, `y`, `both` o vacío.
    std::string fileType{"svg"};        ///< `svg`, `png` o `pdf`.
    double pngDpi{96};                  ///< `png_dpi_spinner`.
    double screenDpi{96};               ///< `logicalDotsPerInch` de la primera pantalla.
    double boundary{1.0};               ///< Borde negro de la negativa.
    std::string color{"#000000"};       ///< Color de la positiva (`tools_film_color`).
    std::string opacity{"1.0"};         ///< `str(opacity_val)`.
};

/** @brief El resultado: el SVG y lo que se usa para el PDF. */
struct FilmResult {
    std::string svg;           ///< El documento (`doc.toprettyxml()`).
    BoundingBox bounds;        ///< Límites de la caja transformada (`my_canvas.translate(bounds[0], bounds[1])`).
    double width{0};           ///< Ancho del SVG con el borde, en las unidades del objeto.
    double height{0};          ///< Alto.
};

/**
 * @brief `make_negative_film` / `make_positive_film` hasta el texto del SVG.
 * @param obj Las figuras del objeto (`flatten()`, o la geometría de las herramientas de una Geometría).
 * @param box Las figuras de la caja.
 * @param boxBounds `box.bounds()`.
 * @param uom `obj.units.lower()`.
 * @param o Los parámetros.
 * @return El SVG.
 */
FilmResult filmSvg(const ShapeList& obj, const ShapeList& box, const BoundingBox& boxBounds, const std::string& uom,
                   const FilmOptions& o);

/** @brief `MultiPolygon(film.solid_geometry).difference(MultiPolygon(exc.solid_geometry))`. @param film El cobre.
 *  @param drills Los agujeros. @return Lo perforado. */
Shape filmPunchExcellon(const ShapeList& film, const ShapeList& drills);

/**
 * @brief El perforado con el centro de los pads: discos de `punchSize` en el punto de cada pad.
 * @param g El Gerber. @param punchSize El diámetro.
 * @param error Si falla: `size` (el agujero no entra en una apertura) o `same` (no cambió nada).
 * @return Lo perforado (vacío si falla).
 */
std::optional<Shape> filmPunchPads(const Gerber& g, double punchSize, std::string& error);

}  // namespace flatcam
