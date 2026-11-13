/**
 * @file SvgExport.h
 * @brief Exportar a SVG (port de `App.export_svg` y `Geometry.export_svg`): el `svg()` de Shapely 1.8 de la unión de
 * la geometría, dentro de la cabecera de FlatCAM y con el formato de `xml.dom.minidom.toprettyxml()`.
 */
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "core/Shape.h"

namespace flatcam {

/**
 * @brief El elemento SVG de una figura como `geom.svg(scale_factor)` de Shapely 1.8 (polígonos `path`, líneas
 * `polyline`, puntos `circle`, figuras múltiples en `g`; verde `#66cc99` si es válida, rojo `#ff3333` si no).
 * @param geom Figura. @param scaleFactor Factor del ancho de trazo. @param color Color forzado (vacío: según la validez).
 * @return El texto del elemento.
 */
std::string shapelySvg(const Shape& geom, double scaleFactor, const std::string& color = std::string());

/**
 * @brief El documento SVG completo de `export_svg`: la unión de las figuras, con `width`, `height` y `viewBox` en las
 * unidades del objeto y la escala `(1,-1)`, formateado como `toprettyxml()` (tabulaciones y la declaración XML).
 * @param shapes Figuras del objeto (se unen con `unary_union`).
 * @param units Unidades en minúscula (`"mm"`).
 * @param scaleStrokeFactor `scale_stroke_factor` (0: se usa 0,01).
 * @return El texto, o vacío si no hay figuras.
 */
std::string exportSvg(const ShapeList& shapes, const std::string& units, double scaleStrokeFactor = 0.0);

/** @brief Un nodo del XML mínimo de este módulo (elemento, o texto si `tag` está vacío). */
struct XmlNode {
    std::string tag;                                          ///< Nombre del elemento; vacío: es un texto.
    std::vector<std::pair<std::string, std::string>> attrs;   ///< Atributos, en orden.
    std::vector<XmlNode> children;                            ///< Hijos.
    std::string text;                                         ///< El texto (solo en los nodos de texto).
    /** @brief `element.set(name, value)` de lxml: reemplaza en su lugar o agrega al final. @param name Nombre.
     *  @param value Valor. */
    void set(const std::string& name, const std::string& value);
};

/** @brief Lee el XML que arman estas funciones (sin entidades ni comentarios). @param s El texto.
 *  @return El elemento raíz. */
XmlNode parseMiniXml(const std::string& s);

/** @brief `parseString(doc).toprettyxml()` de minidom. @param root La raíz. @return El texto, con su declaración. */
std::string prettyMiniXml(const XmlNode& root);

/** @brief `etree.tostring(element)` de lxml (sin espacio antes de "/>"). @param n El elemento. @return El texto. */
std::string lxmlToString(const XmlNode& n);

}  // namespace flatcam
