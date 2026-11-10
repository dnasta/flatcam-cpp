/**
 * @file Subtract.h
 * @brief Restar un objeto de otro (port de `ToolSub.aperture_intersection`, `new_gerber_object` y
 * `toolgeo_intersection`, `appTools/ToolSub.py`).
 */
#pragma once

#include <map>
#include <string>

#include "core/Shape.h"
#include "parsers/Gerber.h"

namespace flatcam {

/** @brief El Gerber restado. */
struct SubtractGerberResult {
    std::map<std::string, Aperture> apertures;  ///< Las aperturas que quedaron con geometría (y la "0" con lo modificado).
    ShapeList solid;                            ///< Geometría sólida (las figuras engrosadas y achicadas 1e-7).
    bool failed{false};                         ///< El original falla al armar la geometría (no crea el objeto).
};

/**
 * @brief Gerber menos Gerber: cada elemento de cada apertura del objetivo pierde lo que tapan las figuras sólidas
 * (y de borrado) del sustraendo. Los elementos sin cambios quedan en su apertura; los modificados pasan a la apertura "0"
 * (tipo `REG`). Los elementos nuevos solo llevan su figura sólida y de borrado (no la de seguimiento), como el original.
 *
 * Errores del original que se reproducen: cada figura del sustraendo se resta por separado y el resultado es la unión de
 * esas restas (con dos figuras superpuestas sobre un elemento, no se resta lo que tapa cada una); un elemento que queda
 * vacío no cuenta como modificado y queda como estaba; al armar la geometría sólida, de un elemento que quedó partido en
 * varios polígonos solo entra el primero (el `MultiPolygon` de Shapely 1.8), y si es el primer elemento, falla.
 * @param target El objetivo. @param sub El sustraendo. @return El resultado.
 */
SubtractGerberResult subtractGerber(const Gerber& target, const Gerber& sub);

/**
 * @brief Geometría menos Geometría (`toolgeo_intersection`): con @p closePaths, la unión de todo menos el sustraendo (una
 * figura); si no, los anillos de cada polígono y cada línea, de a uno, menos el sustraendo (quedan líneas).
 * @param geo La geometría del objetivo. @param subUnion La unión del sustraendo. @param closePaths "Cerrar trayectos".
 * @return Las figuras resultantes (las vacías no se agregan).
 */
ShapeList subtractGeometry(const ShapeList& geo, const Shape& subUnion, bool closePaths);

}  // namespace flatcam
