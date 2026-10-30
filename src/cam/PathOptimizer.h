/**
 * @file PathOptimizer.h
 * @brief Orden de visita de los agujeros para reducir el recorrido de la máquina.
 */
#pragma once

#include <vector>

#include "core/Types.h"

namespace flatcam {

/**
 * @brief Optimizador de recorrido propio (reemplaza a OR-Tools en el taladrado).
 *
 * FlatCAM ordena los agujeros con el solver de rutas de OR-Tools. Este port usa un algoritmo propio y
 * determinista, sin dependencias: se parte de dos recorridos (vecino más cercano y orden del archivo), se mejora
 * cada uno con **2-opt** hasta que no se pueda acortar más y se elige el más corto. Por construcción nunca es más
 * largo que el orden del archivo. El resultado visita los mismos agujeros que el original, solo en otro orden.
 */
class PathOptimizer {
public:
    /**
     * @brief Calcula un orden de visita corto para los puntos dados.
     * @param points Puntos a visitar.
     * @param start Posición de la que parte la máquina (no forma parte de los puntos a visitar).
     * @return Índices de @p points en el orden en que conviene visitarlos.
     */
    static std::vector<std::size_t> order(const std::vector<Point2D>& points, const Point2D& start);

    /**
     * @brief Longitud del recorrido que parte de @p start y visita los puntos en el orden indicado.
     * @param points Puntos.
     * @param order Orden de visita (índices de @p points).
     * @param start Punto de partida.
     * @return Distancia total recorrida.
     */
    static double pathLength(const std::vector<Point2D>& points, const std::vector<std::size_t>& order,
                             const Point2D& start);
};

}  // namespace flatcam
