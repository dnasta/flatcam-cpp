/**
 * @file AlignObjects.h
 * @brief Alinear objetos: los puntos que se eligen con el mouse y el desplazamiento y el giro que resultan (port de
 * `AlignObjects.on_mouse_click_release`, `align_translate` y `align_rotate`, `appTools/ToolAlignObjects.py`).
 */
#pragma once

#include <optional>
#include <vector>

#include "core/Types.h"
#include "parsers/Excellon.h"
#include "parsers/Gerber.h"

namespace flatcam {

/**
 * @brief Los puntos que agrega un clic sobre un Gerber: el centroide de cada pad (elemento con un punto como
 * geometría de seguimiento) que contiene al clic, redondeado a @p decimals, en el orden de las aperturas.
 * @param g El Gerber. @param click Posición del clic. @param decimals `app.decimals`.
 * @return Los puntos (uno por cada pad que contiene al clic; ninguno fuera de los pads).
 */
std::vector<Point2D> alignPickGerber(const Gerber& g, const Point2D& click, int decimals = 4);

/**
 * @brief Los puntos que agrega un clic sobre un Excellon: el centroide de cada figura de las herramientas que contiene
 * al clic, redondeado a @p decimals.
 * @param e El Excellon. @param click Posición del clic. @param decimals `app.decimals`.
 * @return Los puntos.
 */
std::vector<Point2D> alignPickExcellon(const Excellon& e, const Point2D& click, int decimals = 4);

/** @brief Lo que se le hace al objeto alineado. */
struct AlignMove {
    Point2D offset;               ///< Desplazamiento: del primer punto (inicio) al segundo (destino).
    std::optional<double> angle;  ///< Giro en grados alrededor de @ref origin (con dos pares de puntos).
    Point2D origin;               ///< Centro del giro: el segundo punto.
    bool fails{false};            ///< El original falla al calcular el ángulo (división por cero) después de desplazar.
};

/**
 * @brief El desplazamiento y, con cuatro puntos, el giro, como el original: el giro se omite si el segundo inicio
 * desplazado y su destino tienen la misma X **o** la misma Y (a menos de `10^-decimals`), y el ángulo sale de
 * `atan` (no `atan2`) de las pendientes.
 * @param points Dos o cuatro puntos: inicio, destino, (segundo inicio, segundo destino).
 * @param decimals `app.decimals`.
 * @return Lo que se le hace al objeto.
 */
AlignMove alignMove(const std::vector<Point2D>& points, int decimals = 4);

}  // namespace flatcam
