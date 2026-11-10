/**
 * @file InvertGerber.h
 * @brief Invertir un Gerber: la caja del objeto, agrandada un margen, menos su cobre (port de
 * `ToolInvertGerber.on_grb_invert`, `appTools/ToolInvertGerber.py`).
 */
#pragma once

#include "core/Shape.h"
#include "core/Types.h"

namespace flatcam {

/**
 * @brief Las figuras del Gerber invertido: la caja de @p bounds engrosada @p margin (con las esquinas de @p join y
 * @p steps segmentos por cuarto de círculo) menos cada polígono de @p solid, de a uno; un margen que redondeado a
 * @p decimals es 0 vale 1e-10, como el original.
 * @param solid La geometría sólida del Gerber. @param bounds Sus límites. @param margin Margen.
 * @param join Esquinas. @param steps `gerber_circle_steps`. @param decimals `app.decimals`.
 * @return Los polígonos (la apertura "0" del Gerber nuevo).
 */
ShapeList invertGerber(const ShapeList& solid, const BoundingBox& bounds, double margin, JoinStyle join, int steps = 64,
                       int decimals = 4);

}  // namespace flatcam
