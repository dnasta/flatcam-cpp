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
 * Tres métodos, como FlatCAM para NCC y Paint: **estándar** (`method = 0`, se achica el polígono hacia adentro paso a
 * paso y cada contorno es una pasada), **semilla** (`1`, círculos crecientes desde un punto interior) y **líneas** (`2`,
 * líneas paralelas al lado más largo).
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
     * @brief Método de semilla (`Geometry.clear_polygon2`): círculos concéntricos alrededor de un punto interior,
     * recortados al polígono achicado la mitad de la herramienta, y al final (con @p contour) el contorno interior.
     * @param polygon Polígono a cubrir.
     * @param tooldia Diámetro de la herramienta.
     * @param stepsPerCircle Segmentos por cuarto de círculo de los círculos y del achicado.
     * @param overlap Solape, de 0 a 1.
     * @param connect Unir pasadas vecinas (@ref paintConnect).
     * @param contour Recorrer el contorno interior.
     * @param seed Punto de partida; si falta, uno representativo del polígono achicado.
     * @return Las líneas, o nada si el polígono es demasiado chico para la herramienta.
     */
    static std::optional<std::vector<EndpointStorage::Line>> clearPolygonSeed(const Shape& polygon, double tooldia,
                                                                               int stepsPerCircle, double overlap,
                                                                               bool connect, bool contour,
                                                                               std::optional<Point2D> seed = std::nullopt);

    /**
     * @brief Método de líneas (`Geometry.clear_polygon3`): líneas paralelas al lado más largo de la caja del polígono,
     * recortadas al polígono achicado, y con @p contour el contorno interior.
     *
     * Reproduce un detalle del original: en el caso vertical la última línea se toma en `right + tooldia / 2` (fuera del
     * polígono), así que no aporta nada.
     * @param polygon Polígono a cubrir (solo un `Polygon`; otra cosa devuelve nada, como el original).
     * @param tooldia Diámetro de la herramienta.
     * @param stepsPerCircle Segmentos por cuarto de círculo del achicado.
     * @param overlap Solape, de 0 a 1.
     * @param connect Unir pasadas vecinas (@ref paintConnect).
     * @param contour Recorrer el contorno interior.
     * @return Las líneas, o nada si no se pudo.
     */
    static std::optional<std::vector<EndpointStorage::Line>> clearPolygonLines(const Shape& polygon, double tooldia,
                                                                                int stepsPerCircle, double overlap,
                                                                                bool connect, bool contour);

    /** @brief Método de limpieza (`tools_ncc_method` / `tools_paint_method`). */
    enum class Method {
        Standard,  ///< Estándar (`clear_polygon`).
        Seed,      ///< Semilla (`clear_polygon2`).
        Lines,     ///< Líneas (`clear_polygon3`).
        Combo,     ///< Combinado: líneas; si no da nada, semilla; si tampoco, estándar.
        LaserLines ///< "Líneas láser" de Paint: necesita las aperturas del Gerber (Paint::laserLines); aquí no da nada.
    };

    /**
     * @brief Limpia un polígono con el método elegido (`clear_polygon_worker` de NCC y `paint_polygon_worker` de Paint).
     * @param polygon Polígono.
     * @param tooldia Diámetro de la herramienta.
     * @param stepsPerCircle Segmentos por cuarto de círculo.
     * @param overlap Solape, de 0 a 1.
     * @param connect Unir pasadas vecinas.
     * @param contour Recorrer el contorno (semilla y líneas; el estándar no lo usa).
     * @param method Método.
     * @return Las líneas, o nada si el polígono no se pudo limpiar ("Polygon could not be cleared").
     */
    static std::optional<std::vector<EndpointStorage::Line>> clear(const Shape& polygon, double tooldia,
                                                                   int stepsPerCircle, double overlap, bool connect,
                                                                   bool contour, Method method);

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
