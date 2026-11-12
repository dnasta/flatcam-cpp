/**
 * @file GeoEditorModel.h
 * @brief El modelo del editor de Geometría (port de lo que no es interfaz de `appEditors/AppGeoEditor.py`): las
 * figuras que arma cada herramienta de dibujo, el almacén con la elección y las operaciones (unir, intersecar,
 * restar, cortar, pintar, explotar, borrar con la goma, transformar), sin Qt.
 */
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "core/Shape.h"
#include "core/Types.h"
#include "editors/EditorStorage.h"

namespace flatcam {

/** @brief Las figuras que crean las herramientas de dibujo del editor (`FCCircle.make`, `FCArc.make`, ...). */
namespace geodraw {

/** @brief `FCCircle`: el círculo de centro @p p1 que pasa por @p p2. @param p1 Centro. @param p2 Borde.
 *  @param steps `geometry_circle_steps`. @return El polígono. */
Shape circle(const Point2D& p1, const Point2D& p2, int steps);

/** @brief Modo de `FCArc` (tecla M). */
enum class ArcMode {
    C12,  ///< `c12`: centro, inicio, fin.
    P132, ///< `132`: punto 1, punto 3, punto 2 (por tres puntos).
    P12C  ///< `12c`: inicio, fin, centro.
};

/**
 * @brief `FCArc.make` (y la vista previa con el tercer punto en el mouse).
 * @param mode Modo. @param a Primer clic. @param b Segundo clic. @param c Tercer clic (o el mouse).
 * @param direction Sentido (tecla D; en el modo de tres puntos sale de los puntos). @param steps Segmentos por vuelta.
 * @return La línea del arco, o nada si los tres puntos están alineados en el modo de tres puntos.
 */
std::optional<Shape> arc(ArcMode mode, const Point2D& a, const Point2D& b, const Point2D& c,
                         ArcDirection direction, int steps);

/** @brief El centro del arco (lo muestra la vista previa). @param mode Modo. @param a Primero. @param b Segundo.
 *  @param c Tercero. @return El centro, o nada si no hay. */
std::optional<Point2D> arcCenter(ArcMode mode, const Point2D& a, const Point2D& b, const Point2D& c);

/** @brief `FCRectangle`: el rectángulo de esquinas opuestas @p p1 y @p p2. @param p1 Primera. @param p2 Segunda.
 *  @return El polígono. */
Shape rectangle(const Point2D& p1, const Point2D& p2);

}  // namespace geodraw

/**
 * @brief Las figuras del editor de Geometría y lo que se hace con ellas.
 *
 * Las operaciones siguen al original al pie de la letra, también en sus fallas (ver `AGENTS.md`, sección 14).
 */
class GeoEditorModel {
public:
    EditorStorage storage;                ///< `storage`.
    std::vector<EditorShapePtr> selected; ///< `selected`, en el orden en que se eligió.

    /** @brief `add_shape`: agrega una figura. @param g La figura. @return La figura agregada. */
    EditorShapePtr add(const Shape& g);

    /** @brief `delete_shape`: quita una figura del almacén y de la elección. @param s La figura. */
    void remove(const EditorShapePtr& s);

    /** @brief `delete_selected`: quita las elegidas. */
    void deleteSelected();

    /**
     * @brief `edit_fcgeometry`: carga la geometría (los polígonos orientados con @p orient y partidos en su exterior y
     * sus agujeros, como anillos).
     * @param geometry La geometría del objeto. @param orient 1 (exterior antihorario) o -1.
     */
    void load(const ShapeList& geometry, int orient);

    /** @brief `update_fcgeometry`: la geometría que vuelve al objeto (las multilíneas, unidas con `linemerge`).
     *  @return La lista. */
    ShapeList result() const;

    /**
     * @brief `on_shape_complete`: agrega lo que creó una herramienta. Con fresado por ascenso (`cl`, el de fábrica) da
     * vuelta las coordenadas: un polígono o un anillo sale como polígono invertido y una línea, invertida; en una lista
     * (mover, copiar, explotar) todo se junta en un `MultiPolygon`, salvo que haya una línea (entonces queda igual) y
     * las figuras múltiples se pierden, como en el original.
     * @param geometry Lo creado. @param isList Si la herramienta dejó una lista (`self.geometry = [...]`).
     * @param climb `geometry_editor_milling_type` == `cl`.
     * @return Las figuras agregadas.
     */
    std::vector<EditorShapePtr> complete(const ShapeList& geometry, bool isList, bool climb);

    /**
     * @brief `FCSelect.click_release`: elige la figura con el vértice más cercano a @p p.
     * @param p El punto. @param toggle La tecla de elección múltiple está apretada (agrega o quita).
     */
    void clickSelect(const Point2D& p, bool toggle);

    /**
     * @brief `draw_selection_area_handler`: elige con un rectángulo.
     * @param a Una esquina. @param b La opuesta. @param contains De izquierda a derecha: las figuras contenidas; si no,
     * las que toca. @param toggle Elección múltiple (agrega o quita).
     */
    void areaSelect(const Point2D& a, const Point2D& b, bool contains, bool toggle);

    /** @brief `union`: une las elegidas en una figura nueva. */
    void unite();

    /** @brief `intersection`: interseca la primera con cada una de las demás. @return Falso si hay menos de una. */
    bool intersection();

    /** @brief `subtract`: la primera menos las demás (todo engrosado 1e-7). */
    void subtract();

    /** @brief `cutpath`: corta los contornos de la primera con las demás. @return Falso si el tipo no se admite. */
    bool cutpath();

    /**
     * @brief `FCExplode.make`: parte los polígonos elegidos en sus segmentos.
     * @return Los segmentos, o nada si el original falla (cualquier figura que no sea polígono, o un polígono con
     * agujeros: el recorrido de los agujeros se pasa del final).
     */
    std::optional<ShapeList> explode();

    /**
     * @brief `FCEraser.make`: las elegidas, movidas de @p origin a @p destination, borran lo que tocan (también a sí
     * mismas).
     * @param origin Origen. @param destination Destino.
     */
    void erase(const Point2D& origin, const Point2D& destination);

    /** @brief Método de `paint` (el texto del combo). */
    enum class PaintMethod { Standard, Seed, Lines };

    /**
     * @brief `paint`: pinta las elegidas y agrega el resultado.
     * @param tooldia Diámetro. @param overlap Solape (0 a 1). @param margin Margen. @param connect Unir.
     * @param contour Contorno. @param method Método. @param steps `geometry_circle_steps`.
     * @return Vacío si salió bien, o el error del original.
     */
    std::optional<std::string> paint(double tooldia, double overlap, double margin, bool connect, bool contour,
                                     PaintMethod method, int steps);

    /** @brief `alt_bounds`: la caja de las elegidas. @return La caja. */
    BoundingBox selectionBounds() const;

    /** @brief `DrawToolShape.rotate` de las elegidas. @param angle Grados. @param p Centro. */
    void rotate(double angle, const Point2D& p);
    /** @brief `DrawToolShape.mirror` de las elegidas. @param axis `X` o `Y`. @param p Punto del eje. */
    void mirror(char axis, const Point2D& p);
    /** @brief `DrawToolShape.skew` de las elegidas. @param ax Grados en X. @param ay En Y. @param p Punto fijo. */
    void skew(double ax, double ay, const Point2D& p);
    /** @brief `DrawToolShape.scale` de las elegidas. @param fx Factor X. @param fy Factor Y. @param p Origen. */
    void scale(double fx, double fy, const Point2D& p);
    /** @brief `DrawToolShape.offset` de las elegidas. @param dx X. @param dy Y. */
    void offset(double dx, double dy);
    /**
     * @brief `DrawToolShape.buffer` de las elegidas: engrosa (32 segmentos) o escala desde el centro de la caja.
     * @param value Distancia o factor. @param join 1 redondo, 2 inglete. @param factor Escalar en vez de engrosar.
     */
    void buffer(double value, int join, bool factor);
};

}  // namespace flatcam
