/**
 * @file OptimalDistance.h
 * @brief Óptimo: la distancia mínima entre las figuras de cobre de un Gerber (port de `ToolOptimal.find_minimum_distance`,
 * `appTools/ToolOptimal.py`), para elegir la fresa de aislamiento.
 */
#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "core/Types.h"
#include "parsers/Gerber.h"

namespace flatcam {

/** @brief Las distancias entre las figuras. */
struct OptimalResult {
    bool onePolygon{false};  ///< El cobre es un solo polígono: no hay distancias ("The Gerber object has one Polygon").
    /** @brief Cada distancia (redondeada) con los pares de puntos más cercanos (redondeados) donde aparece. */
    std::map<double, std::vector<std::pair<Point2D, Point2D>>> distances;
};

/**
 * @brief Une las figuras sólidas válidas de todas las aperturas (`MultiPolygon(...).buffer(0)`) y mide la distancia
 * entre cada par de polígonos, con @p decimals cifras (las distancias y los puntos).
 * @param g El Gerber. @param decimals "Precisión". @return Las distancias.
 */
OptimalResult findMinimumDistances(const Gerber& g, int decimals);

/**
 * @brief Un par de puntos como lo escribe el original: `str(p1) + ", " + str(p2)`, por ejemplo
 * `(1.2345, -3.0), (1.4, -3.0)`.
 * @param pair Los puntos. @return El texto.
 */
std::string pyPointPair(const std::pair<Point2D, Point2D>& pair);

}  // namespace flatcam
