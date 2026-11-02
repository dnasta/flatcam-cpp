/**
 * @file ExclusionAreas.h
 * @brief Áreas de exclusión: zonas que los viajes del G-code esquivan o pasan por encima (port de `ExclusionAreas` de
 * `appCommon/Common.py`).
 */
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "core/Shape.h"

namespace flatcam {

/** @brief Un área de exclusión (`exclusion_areas_storage[i]`). */
struct ExclusionArea {
    std::string objType{"excellon"};  ///< Objeto desde el que se agregó: `"excellon"` o `"geometry"` (solo el color).
    Shape shape;                      ///< Zona (rectángulo o polígono).
    std::string strategy{"over"};     ///< `"over"` (pasar por encima, a @ref overZ) o `"around"` (rodearla).
    double overZ{1.0};                ///< Z con que se pasa por encima (`overz`).
};

/** @brief Un tramo del viaje: el punto y, si hay que subir, la Z (`[overz o None, (x, y)]`). */
struct ExclusionTravel {
    std::optional<double> z;  ///< Z a la que se sube en el punto (nada: la Z de viaje normal).
    Point2D point;            ///< Punto.
};

/** @brief Cálculo de los viajes que respetan las áreas de exclusión. */
class ExclusionAreas {
public:
    /**
     * @brief Viaje de @p start a @p end que esquiva las áreas (`travel_coordinates`).
     *
     * Cada área se agranda medio diámetro de herramienta más 0,1 mm (o 0,00393701 pulgadas), con esquinas en inglete, y se
     * recorren de la más cercana a la más lejana al punto de partida. Si el tramo cruza el contorno: con "over" se sube a
     * la Z del área al entrar y se baja al salir; con "around" se rodea el contorno por el camino más corto entre los
     * vértices alcanzables. Sin áreas, el viaje es directo (un solo punto).
     * @param areas Áreas.
     * @param start Punto de partida.
     * @param end Destino.
     * @param tooldia Diámetro de la herramienta.
     * @param inches Si las unidades son pulgadas.
     * @return Los puntos del viaje, terminando en @p end.
     */
    static std::vector<ExclusionTravel> travelCoordinates(const std::vector<ExclusionArea>& areas, const Point2D& start,
                                                          const Point2D& end, double tooldia, bool inches = false);
};

}  // namespace flatcam
