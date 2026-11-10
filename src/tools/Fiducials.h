/**
 * @file Fiducials.h
 * @brief Fiduciales: marcas de referencia en el cobre y sus aberturas en la máscara (port de
 * `ToolFiducials.add_fiducials` y `add_fiducials_geo`, `appTools/ToolFiducials.py`).
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

/** @brief Forma del fiducial (`fid_type_radio`). */
enum class FiducialType {
    Circular,  ///< `circular`: un círculo del tamaño.
    Cross,     ///< `cross`: una cruz de líneas del grosor dado.
    Chess      ///< `chess`: dos cuadrados en diagonal (tablero de ajedrez).
};

/**
 * @brief Los puntos del modo automático: las esquinas abajo a la izquierda y arriba a la derecha de la caja agrandada
 * el margen y, según @p second, arriba a la izquierda (`up`), abajo a la derecha (`down`) o ninguna (`no`), redondeadas
 * a @p decimals.
 * @param b Límites del Gerber. @param margin Margen. @param second Segundo punto. @param decimals `app.decimals`.
 * @param steps `gerber_circle_steps`. @return Los puntos.
 */
std::vector<Point2D> fiducialAutoPoints(const BoundingBox& b, double margin, const std::string& second,
                                        int decimals = 4, int steps = 64);

/** @brief El Gerber con los fiduciales. */
struct FiducialsGerber {
    ShapeList solid;                            ///< `unary_union` del cobre y los fiduciales.
    ShapeList follow;                           ///< Seguimiento: el del Gerber más las figuras de los fiduciales.
    std::map<std::string, Aperture> apertures;  ///< Aperturas, con los fiduciales en la que corresponde.
};

/**
 * @brief `add_fiducials_geo`: agrega un fiducial en cada punto, en la apertura del mismo tamaño si existe (C de @p size,
 * C de @p lineThickness para la cruz, R con la misma diagonal redondeada para el tablero) o en una nueva con el número
 * siguiente al mayor (comparando los números como texto, como el original).
 * @param g El Gerber. @param points Puntos. @param size Tamaño total. @param type Forma.
 * @param lineThickness Grosor de las líneas de la cruz. @param steps `gerber_circle_steps`. @param decimals `app.decimals`.
 * @return El Gerber nuevo, o vacío si el original falla: con la cruz, si todas se unen en un solo polígono (`TypeError`
 * al recorrerlo).
 */
std::optional<FiducialsGerber> addFiducials(const Gerber& g, const std::vector<Point2D>& points, double size,
                                            FiducialType type, double lineThickness, int steps = 64, int decimals = 4);

}  // namespace flatcam
