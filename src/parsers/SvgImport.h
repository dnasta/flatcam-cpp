/**
 * @file SvgImport.h
 * @brief Importar SVG (port de `appParsers/ParseSVG.py`, de `Geometry.import_svg` y `Gerber.import_svg`, y del
 * intérprete de trayectos de la biblioteca `svg.path` 7.1 que usa el original).
 */
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "core/Shape.h"

namespace flatcam {

/** @brief Un elemento de un documento XML (lo mínimo que hace falta para leer un SVG). */
struct XmlElement {
    std::string tag;                                         ///< Nombre local (sin prefijo ni espacio de nombres).
    std::vector<std::pair<std::string, std::string>> attrs;  ///< Atributos, con su nombre tal como están.
    std::vector<XmlElement> children;                        ///< Elementos hijos (sin textos ni comentarios).
    std::string text;                                        ///< Texto antes del primer hijo.
    /** @brief Valor de un atributo. @param name Nombre. @return El valor, o vacío si no está. */
    std::optional<std::string> get(const std::string& name) const;
};

/**
 * @brief Lee un documento XML: elementos, atributos (con entidades), texto; saltea la declaración, `DOCTYPE`,
 * comentarios e instrucciones.
 * @param text El documento. @param error Mensaje si falla.
 * @return El elemento raíz, o vacío si el XML está mal formado.
 */
std::optional<XmlElement> parseXml(const std::string& text, std::string& error);

/** @brief Un tramo de un trayecto SVG (`svg.path.Move`, `Line`, `Close`, `CubicBezier`, `QuadraticBezier`, `Arc`). */
struct SvgSegment {
    /** @brief Tipo de tramo. */
    enum class Kind { Move, Line, Close, Cubic, Quadratic, Arc };
    Kind kind{Kind::Line};  ///< Tipo.
    Point2D start;          ///< Inicio.
    Point2D end;            ///< Fin.
    Point2D c1;             ///< Primer punto de control (o el único de la cuadrática).
    Point2D c2;             ///< Segundo punto de control.
    Point2D radius;         ///< Radios del arco.
    double rotation{0};     ///< Rotación del arco en grados.
    bool large{false};      ///< Bandera de arco grande.
    bool sweep{false};      ///< Bandera de sentido.
    // Parametrización del arco (`_parameterize`).
    bool parameterized{false};  ///< Si se calculó el centro (no lo hace si empieza y termina igual o un radio es 0).
    Point2D center;             ///< Centro.
    double theta{0};            ///< Ángulo inicial en grados.
    double delta{0};            ///< Barrido en grados.
    double radiusScale{1};      ///< Escala de los radios (si no alcanzaban).
    /** @brief Punto en la posición @p t (0 a 1), como `point()`. @param t Posición. @return El punto. */
    Point2D point(double t) const;
    /** @brief Largo aproximado, como `length(error)`. @param error Error admitido. @return El largo. */
    double length(double error) const;
};

/**
 * @brief Interpreta el atributo `d` de un trayecto, como `svg.path.parse_path`.
 * @param d El texto. @param error Mensaje si no es válido (`InvalidPathError`).
 * @return Los tramos, o vacío si falla.
 */
std::optional<std::vector<SvgSegment>> parseSvgPath(const std::string& d, std::string& error);

/** @brief Resultado de leer un SVG. */
struct SvgImportResult {
    bool ok{false};        ///< Si se pudo leer.
    std::string error;     ///< El error, si no.
    ShapeList geometry;    ///< Las figuras (ya invertidas en Y y corridas el alto, como `flip=True`).
    bool hadText{false};   ///< Si había textos (`tspan`), que el port no importa.
};

/**
 * @brief `Geometry.import_svg`: las figuras de un SVG para un objeto Geometría (los polígonos y después las líneas
 * unidas con `linemerge`).
 * @param svgText El archivo. @param circleSteps `geometry_circle_steps` (64).
 * @return El resultado.
 */
SvgImportResult importSvgAsGeometry(const std::string& svgText, int circleSteps = 64);

/**
 * @brief `Gerber.import_svg`: las figuras de un SVG para un objeto Gerber (todas en la apertura "0"; con una sola
 * figura, sus agujeros pasan a ser polígonos aparte, como el original). Falla si hay figuras que no son polígonos. Al final va
 * un polígono vacío: el `solid_geometry` inicial de un Gerber (`Polygon()`), como en el original.
 * @param svgText El archivo. @param circleSteps `gerber_circle_steps` (64).
 * @return El resultado (los polígonos de la apertura "0").
 */
SvgImportResult importSvgAsGerber(const std::string& svgText, int circleSteps = 64);

}  // namespace flatcam
