/**
 * @file Panelize.h
 * @brief Panelizar: copias de un objeto en filas y columnas (port de `ToolPanelize.on_panelize`, `appTools/ToolPanelize.py`).
 */
#pragma once

#include <map>
#include <memory>

#include "core/Shape.h"
#include "core/Types.h"
#include "parsers/Excellon.h"
#include "parsers/Gerber.h"

namespace flatcam {

/** @brief Los parámetros del panel (`tools_panelize_*`). */
struct PanelizeSettings {
    double spacingColumns{0.0};  ///< Espacio entre columnas.
    double spacingRows{0.0};     ///< Espacio entre filas.
    int columns{1};              ///< Columnas.
    int rows{1};                 ///< Filas.
    bool constrain{false};       ///< Limitar el panel a un área.
    double constrainX{200.0};    ///< Ancho del área.
    double constrainY{290.0};    ///< Alto del área.
    bool optimize{true};         ///< Unir los tramos que se superponen (Geometrías).
};

/** @brief La grilla del panel: cuántas copias y a qué distancia. */
struct PanelLayout {
    int columns{1};          ///< Columnas (menos si no entran en el área).
    int rows{1};             ///< Filas.
    double stepX{0.0};       ///< Distancia entre columnas: ancho de la caja más el espacio.
    double stepY{0.0};       ///< Distancia entre filas.
    bool constrained{false}; ///< Si se redujeron las filas o columnas para entrar en el área.
};

/**
 * @brief Las filas, columnas y pasos del panel a partir de la caja de referencia (`box.bounds()`), con el ajuste al área.
 * @param box Caja de referencia. @param s Parámetros. @return La grilla.
 */
PanelLayout panelLayout(const BoundingBox& box, const PanelizeSettings& s);

/**
 * @brief `job_init_excellon`: las mismas herramientas con los agujeros y las ranuras de cada copia (fila por fila, de
 * abajo hacia arriba; en cada fila, de izquierda a derecha), con la geometría creada y los ceros y las unidades del original.
 * @param src El Excellon. @param l La grilla. @return El Excellon nuevo.
 */
std::unique_ptr<Excellon> panelizeExcellon(const Excellon& src, const PanelLayout& l);

/**
 * @brief `job_init_geometry` con un Gerber: la geometría sólida y la de cada apertura (sólida, de seguimiento y de borrado)
 * de cada copia. El resultado no tiene geometría de seguimiento general, como el original.
 * @param src El Gerber. @param l La grilla.
 * @param solid Sale la geometría sólida. @param apertures Salen las aperturas con su geometría.
 */
void panelizeGerber(const Gerber& src, const PanelLayout& l, ShapeList& solid, std::map<std::string, Aperture>& apertures);

/**
 * @brief `job_init_geometry` con una Geometría multiherramienta: la geometría de cada herramienta en cada copia; después
 * las líneas de cada herramienta se unen (`linemerge` y `unary_union`), antes acercadas entre sí con `snap(1e-7)` si
 * @p optimize, y van primero, seguidas de las demás figuras.
 * @param tools La geometría de cada herramienta. @param l La grilla. @param optimize "Optimizar".
 * @return La geometría de cada herramienta del panel.
 */
std::map<int, ShapeList> panelizeGeometryTools(const std::map<int, ShapeList>& tools, const PanelLayout& l, bool optimize);

/**
 * @brief Las copias de una lista de figuras (una Geometría de una sola herramienta o un Gerber como Geometría).
 * @param shapes Las figuras. @param l La grilla. @return Las figuras de todas las copias.
 */
ShapeList panelizeShapes(const ShapeList& shapes, const PanelLayout& l);

}  // namespace flatcam
