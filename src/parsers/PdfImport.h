/**
 * @file PdfImport.h
 * @brief Importar PDF (port de `appParsers/ParsePDF.py` y de las capas de `appTools/ToolPDF.py`): interpreta los
 * operadores del contenido de las páginas (colores, `cm`, `q`/`Q`, `m`, `l`, `c`, `v`, `y`, `re`, `h`, `w`, `S`, `f`,
 * `B`, `W n`, `n`) y arma una capa por color de trazo, más la capa 0 con los rellenos blancos (agujeros).
 */
#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "parsers/Excellon.h"
#include "parsers/Gerber.h"

namespace flatcam {

/** @brief Las aperturas de una capa, en el orden de inserción del diccionario del original. */
using PdfApertures = std::vector<std::pair<std::string, Aperture>>;

/**
 * @brief `PdfParser.parse_pdf`: las capas (1, 2... por color de trazo; 0, los rellenos blancos con curvas).
 * @param content El texto de los streams ya descomprimidos (cada uno seguido de `\r\n`).
 * @param units `MM` o `IN` (el factor de los puntos del PDF).
 * @param circleSteps `gerber_circle_steps` (el original lo usa como segmentos por cuarto de círculo del engrosado y
 * como pasos de las curvas de Bézier).
 * @return Las capas no vacías.
 */
std::map<int, PdfApertures> parsePdf(const std::string& content, const std::string& units, int circleSteps = 64);

/** @brief `bezier_to_points`. @param start Inicio. @param c1 Control 1. @param c2 Control 2. @param stop Fin.
 *  @param steps Pasos. @return Los puntos (sin el final). */
std::vector<Point2D> pdfBezierToPoints(const Point2D& start, const Point2D& c1, const Point2D& c2, const Point2D& stop,
                                       int steps);

/** @brief Lo que `layer_rendering_as_gerber` deja en el objeto. */
struct PdfGerberLayer {
    ShapeList solid;   ///< `solid_geometry` (aplanada).
    ShapeList follow;  ///< `follow_geometry` (aplanada).
};

/** @brief `layer_rendering_as_gerber`: la unión de los sólidos (menos los borrados de la apertura 0) engrosada y
 *  achicada 1e-7. @param aps Las aperturas. @return La geometría. */
PdfGerberLayer pdfLayerAsGerber(const PdfApertures& aps);

/** @brief `layer_rendering_as_excellon`: un agujero por cada borrado de la capa 0, con el diámetro del ancho de su caja
 *  por 0,974 (3 decimales). @param aps Las aperturas de la capa 0. @return Herramienta (desde 1), diámetro y centros. */
std::vector<std::pair<double, std::vector<Point2D>>> pdfLayerAsExcellon(const PdfApertures& aps);

}  // namespace flatcam
