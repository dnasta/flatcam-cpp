/**
 * @file EditorStorage.h
 * @brief Almacén de las figuras de un editor (port de `FlatCAMRTreeStorage` con `DrawToolShape.get_pts`): guarda las
 * figuras en orden y encuentra la que tiene el vértice más cercano a un punto.
 */
#pragma once

#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "core/Shape.h"

namespace flatcam {

/** @brief Una figura del editor (`DrawToolShape`): se comparte por puntero, porque la elección guarda la identidad. */
struct EditorShape {
    Shape geo;  ///< `geo`: la figura (puede ser múltiple).
};

/// Puntero a una figura del editor.
using EditorShapePtr = std::shared_ptr<EditorShape>;

/** @brief Crea una figura del editor. @param s La geometría. @return El puntero. */
inline EditorShapePtr makeEditorShape(Shape s) { return std::make_shared<EditorShape>(EditorShape{std::move(s)}); }

/**
 * @brief `FlatCAMRTreeStorage` del editor: indexa los vértices de cada figura **al insertarla**.
 *
 * Como el original, si después se cambia la figura (transformarla, borrar con la goma) el índice sigue con los
 * vértices viejos, así que elegirla con un clic o ajustarse a sus esquinas usa los de antes. Ante dos vértices a la
 * misma distancia gana el de la figura insertada primero (el `rtree` real usa un orden interno).
 */
class EditorStorage {
public:
    /**
     * @brief Inserta una figura (`insert`).
     * @param s La figura.
     */
    void insert(const EditorShapePtr& s);

    /** @brief Quita una figura (`remove`). @param s La figura. */
    void remove(const EditorShapePtr& s);

    /** @brief Las figuras que quedan, en el orden en que se insertaron (`get_objects`). @return La lista. */
    std::vector<EditorShapePtr> objects() const;

    /** @brief Cuántas figuras quedan. @return La cantidad. */
    std::size_t size() const;

    /**
     * @brief El vértice más cercano a @p p y su figura (`nearest`).
     * @param p El punto.
     * @return El vértice y la figura, o nada si el almacén está vacío.
     */
    std::optional<std::pair<Point2D, EditorShapePtr>> nearest(const Point2D& p) const;

    /** @brief Vacía el almacén (`make_storage`). */
    void clear() { entries_.clear(); }

    /**
     * @brief `DrawToolShape.get_pts`: los vértices de una figura (exterior y agujeros de los polígonos, cada línea de
     * las múltiples), simplificados con @p tolerance si la hay.
     * @param g La figura. @param tolerance `DrawToolShape.tolerance`. @return Los vértices.
     */
    static std::vector<Point2D> points(const Shape& g, std::optional<double> tolerance);

    /** @brief `DrawToolShape.tolerance` (la de dibujo del objeto por 10), con la que se simplifican los vértices. */
    std::optional<double> tolerance;

private:
    struct Entry {
        EditorShapePtr shape;
        std::vector<Point2D> points;
    };
    std::vector<Entry> entries_;
};

}  // namespace flatcam
