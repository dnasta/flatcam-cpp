/**
 * @file CornerMarkers.h
 * @brief Marcas de esquina (port de `ToolCorners.add_markers`, `add_corners_geo` y `on_create_drill_object`,
 * `appTools/ToolCorners.py`).
 */
#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "core/Shape.h"
#include "core/Types.h"
#include "parsers/Gerber.h"

namespace flatcam {

/** @brief Qué esquinas de la caja llevan marca. */
struct CornerSelection {
    bool topLeft{false};      ///< Arriba a la izquierda (`tl`).
    bool topRight{false};     ///< Arriba a la derecha (`tr`).
    bool bottomLeft{false};   ///< Abajo a la izquierda (`bl`).
    bool bottomRight{false};  ///< Abajo a la derecha (`br`).
    /** @brief Si hay alguna. @return `true` si sí. */
    bool any() const { return topLeft || topRight || bottomLeft || bottomRight; }
};

/**
 * @brief Las líneas de las marcas (dos por esquina: horizontal y vertical, en el orden tl, tr, bl, br): en "L" hacia
 * adentro (`s`, semicruz) o en cruz (`c`), corridas afuera de la caja el margen más medio grosor.
 * @param b La caja del Gerber. @param sel Esquinas. @param cross Cruz en vez de semicruz.
 * @param thickness Grosor. @param length Largo. @param margin Margen.
 * @return Las líneas (vacía si no hay esquinas).
 */
ShapeList cornerLines(const BoundingBox& b, const CornerSelection& sel, bool cross, double thickness, double length,
                      double margin);

/** @brief El Gerber con las marcas. */
struct CornersGerber {
    ShapeList solid;                             ///< Geometría sólida (`unary_union` del cobre y las marcas).
    ShapeList follow;                            ///< Geometría de seguimiento: la del Gerber más las líneas.
    std::map<std::string, Aperture> apertures;   ///< Aperturas, con las marcas en la de su grosor.
};

/**
 * @brief `add_corners_geo`: las marcas se engrosan medio grosor (`steps` segmentos por cuarto de círculo) y se agregan a
 * la apertura circular de ese tamaño, si hay una (con esquinas en inglete), o a una nueva con el número siguiente al
 * mayor (comparando los números como texto, como el original; con esquinas biseladas).
 * @param g El Gerber. @param lines Las líneas. @param thickness Grosor. @param steps `gerber_circle_steps`.
 * @return El Gerber nuevo.
 */
CornersGerber addCornerMarkers(const Gerber& g, const ShapeList& lines, double thickness, int steps = 64);

/**
 * @brief `on_create_drill_object`: un agujero en cada esquina elegida (tl, tr, bl, br), donde se cruzan las marcas.
 * @param b La caja. @param sel Esquinas. @param thickness Grosor. @param margin Margen. @return Los agujeros.
 */
std::vector<Point2D> cornerDrills(const BoundingBox& b, const CornerSelection& sel, double thickness, double margin);

}  // namespace flatcam
