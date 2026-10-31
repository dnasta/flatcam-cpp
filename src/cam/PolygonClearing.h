/**
 * @file PolygonClearing.h
 * @brief Limpieza de polígonos con trayectorias en espiral (port de `Geometry.clear_polygon` y `paint_connect`).
 */
#pragma once

#include <optional>

#include "cam/EndpointStorage.h"
#include "core/Shape.h"

namespace flatcam {

/**
 * @brief Genera las trayectorias con las que una herramienta cubre todo el interior de un polígono.
 *
 * Es el método **estándar** de FlatCAM para NCC y Paint (`method = 0`): se achica el polígono hacia adentro paso
 * a paso y cada contorno resultante es una pasada. Los otros métodos del original (semilla y líneas) no están
 * portados todavía.
 */
class PolygonClearing {
public:
    /**
     * @brief Trayectorias que cubren el interior de un polígono (`Geometry.clear_polygon`).
     *
     * Achica el polígono la mitad de la herramienta y después, repetidamente, `tooldia * (1 - overlap)`, hasta que
     * no queda área. Cada contorno (exterior y agujeros) es una pasada. Con @p connect las pasadas vecinas se unen
     * cuando el tramo de unión cabe dentro del polígono (@ref paintConnect). El parámetro `contour` del original no
     * se usa en ese algoritmo y por eso no existe aquí.
     * @param polygon Polígono o multipolígono a cubrir.
     * @param tooldia Diámetro de la herramienta.
     * @param stepsPerCircle Segmentos por circunferencia al achicar.
     * @param overlap Solape entre pasadas, de 0 a 1 (0,8 = 80 %).
     * @param connect Unir pasadas vecinas para levantar menos la herramienta.
     * @return Las líneas, o nada si el polígono es demasiado chico para la herramienta.
     */
    static std::optional<std::vector<EndpointStorage::Line>> clearPolygon(const Shape& polygon, double tooldia,
                                                                           int stepsPerCircle, double overlap,
                                                                           bool connect);

    /**
     * @brief Une pasadas cuyo tramo de unión queda dentro del polígono (`Geometry.paint_connect`).
     *
     * Parte del punto (0, 0), toma siempre la pasada de extremo más cercano y la une a la anterior si el segmento
     * que las separa, engrosado la mitad de la herramienta, cabe dentro de @p boundary y mide menos de
     * `10 * tooldia`; si no, termina la línea y empieza otra.
     * @param storage Pasadas a unir (se vacía).
     * @param boundary Polígono dentro del que debe quedar la unión.
     * @param tooldia Diámetro de la herramienta.
     * @param stepsPerCircle Segmentos por circunferencia al engrosar el tramo de unión.
     * @return Las líneas resultantes, o nada si no había pasadas.
     */
    static std::optional<std::vector<EndpointStorage::Line>> paintConnect(EndpointStorage& storage,
                                                                          const Shape& boundary, double tooldia,
                                                                          int stepsPerCircle);

    /**
     * @brief Convierte una línea del almacén en una figura (`LineString` o `LinearRing`).
     * @param line Línea.
     * @return La figura.
     */
    static Shape toShape(const EndpointStorage::Line& line);
};

}  // namespace flatcam
