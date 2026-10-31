/**
 * @file EndpointStorage.h
 * @brief Almacén de líneas indexado por sus dos extremos (port de `FlatCAMRTreeStorage`).
 */
#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include "core/Types.h"

namespace flatcam {

/**
 * @brief Conjunto de líneas con búsqueda del extremo más cercano a un punto y borrado.
 *
 * Reproduce `FlatCAMRTreeStorage` de FlatCAM, que indexa el primer y el último punto de cada línea en un R-tree
 * (`rtree.Index.nearest`). Aquí se usa una grilla uniforme, mucho más simple, con el mismo resultado salvo ante
 * extremos **exactamente equidistantes**: el original los devuelve en un orden interno de libspatialindex y este
 * almacén elige el que se insertó primero (cada línea aporta primero su inicio y luego su final).
 */
class EndpointStorage {
public:
    /** @brief Una línea guardada. */
    struct Line {
        std::vector<Point2D> points;  ///< Vértices de la línea.
        bool linearRing{false};       ///< `true` si es un anillo (`LinearRing`) y no una `LineString`.
    };

    /** @brief Resultado de @ref nearest. */
    struct Nearest {
        Point2D point;        ///< Extremo más cercano.
        std::size_t index;    ///< Posición de la línea a la que pertenece.
    };

    /**
     * @brief Agrega una línea.
     * @param points Vértices (al menos uno).
     * @param linearRing Si la línea es un anillo cerrado.
     * @return La posición de la línea (crece de a uno desde 0).
     */
    std::size_t insert(std::vector<Point2D> points, bool linearRing = false);

    /**
     * @brief Línea más cercana a un punto, medida desde sus extremos.
     * @param p Punto de consulta.
     * @return El extremo y la línea, o nada si no queda ninguna.
     */
    std::optional<Nearest> nearest(const Point2D& p) const;

    /** @brief Quita una línea del almacén (sigue ocupando su posición). @param index Posición de la línea. */
    void remove(std::size_t index);

    /** @brief Línea guardada. @param index Posición. @return La línea. */
    const Line& line(std::size_t index) const { return lines_[index]; }

    /** @brief Cantidad de líneas que siguen en el almacén. @return La cantidad. */
    std::size_t size() const { return alive_; }

    /** @brief Líneas que quedan, en el orden en que se insertaron (`get_objects`). @return Las líneas. */
    std::vector<Line> objects() const;

private:
    void rebuildGrid() const;

    std::vector<Line> lines_;
    std::vector<bool> removed_;
    std::size_t alive_{0};

    // Grilla de extremos: cada entrada es 2 * línea (inicio) o 2 * línea + 1 (final). Se reconstruye cuando se inserta.
    mutable bool gridDirty_{true};
    mutable double originX_{0}, originY_{0}, cell_{1};
    mutable int nx_{1}, ny_{1};
    mutable std::vector<std::vector<std::size_t>> cells_;
};

}  // namespace flatcam
