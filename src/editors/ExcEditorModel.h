/**
 * @file ExcEditorModel.h
 * @brief El modelo del editor de Excellon (port de lo que no es interfaz de `appEditors/AppExcEditor.py`): cada
 * agujero es una cruz (`MultiLineString`) y cada ranura un polígono, guardados por diámetro; las figuras de agregar
 * agujeros, ranuras y sus grillas, cambiar el diámetro y volver a armar las herramientas al guardar.
 */
#pragma once

#include <map>
#include <optional>
#include <set>
#include <utility>
#include <vector>

#include "core/Shape.h"
#include "editors/EditorStorage.h"
#include "parsers/Excellon.h"

namespace flatcam {

/** @brief Las figuras de las herramientas de dibujo del editor de Excellon. */
namespace excdraw {

/** @brief `DrillAdd.util_shape`: la cruz de un agujero. @param p Centro. @param dia Diámetro. @return La figura. */
Shape drill(const Point2D& p, double dia);

/**
 * @brief `SlotAdd.util_shape` / `SlotArray.util_shape`: una ranura centrada en @p p.
 * @param p Centro. @param dia Diámetro. @param length Largo. @param axis `X`, `Y` o `A` (con ángulo).
 * @param angle Ángulo (`A`). @param steps `geometry_circle_steps`.
 * @param fromArray La de la grilla gira con `A` siempre; la suelta, solo si es más larga que ancha (como el original).
 * @return El polígono.
 */
Shape slot(const Point2D& p, double dia, double length, char axis, double angle, int steps, bool fromArray);

/**
 * @brief Las posiciones de una grilla lineal (`DrillArray` / `SlotArray` lineal).
 * @param p Inicio. @param n Cantidad. @param axis `X`, `Y` o `A`. @param pitch Paso. @param angle Ángulo (`A`).
 * @return Las posiciones.
 */
std::vector<Point2D> linearArray(const Point2D& p, int n, char axis, double pitch, double angle);

/** @brief Un elemento de una grilla circular: su posición y el giro de la ranura (en radianes). */
struct CircularItem {
    Point2D p;        ///< Posición.
    double rotation;  ///< Giro que lleva la ranura.
};

/**
 * @brief `circular_util_shape`: la grilla circular alrededor de @p origin que empieza en @p destination.
 * @param origin Centro. @param destination Primer elemento. @param n Cantidad. @param step Ángulo entre elementos.
 * @param cw Sentido horario.
 * @return Los elementos, o nada si el radio es 0.
 */
std::optional<std::vector<CircularItem>> circularArray(const Point2D& origin, const Point2D& destination, int n,
                                                       double step, bool cw);

}  // namespace excdraw

/**
 * @brief Las figuras del editor de Excellon por diámetro (`storage_dict`) y lo que se hace con ellas.
 */
class ExcEditorModel {
public:
    /** @brief Un diámetro con su almacén. */
    struct Tool {
        double dia;             ///< Diámetro (la clave del original).
        EditorStorage storage;  ///< Sus figuras.
    };
    std::vector<Tool> storages;            ///< `storage_dict`, en orden de inserción.
    std::vector<EditorShapePtr> selected;  ///< `selected`.
    std::map<double, double> olddiaNewdia; ///< `olddia_newdia`.
    std::map<double, int> pointsEdit;      ///< `points_edit`: cuántos agujeros se cuentan por diámetro.
    std::map<double, int> slotPointsEdit;  ///< `slot_points_edit`: cuántas ranuras.

    /** @brief El almacén de un diámetro. @param dia Diámetro. @return El almacén, o nulo. */
    EditorStorage* storage(double dia);
    /** @brief Crea el almacén de un diámetro (`make_storage`), si no existe. @param dia Diámetro. @return El almacén. */
    EditorStorage& ensure(double dia);
    /** @brief Quita el almacén de un diámetro. @param dia Diámetro. */
    void drop(double dia);
    /** @brief El diámetro del almacén donde está una figura. @param s La figura. @return El diámetro, o nada. */
    std::optional<double> diaOf(const EditorShapePtr& s) const;

    /**
     * @brief `edit_fcexcellon`: los agujeros como cruces y las ranuras engrosadas, por diámetro (con @p decimals).
     * @param e El Excellon. @param decimals `app.decimals`.
     */
    void load(const Excellon& e, int decimals);

    /** @brief Agrega una figura al almacén de un diámetro. @param dia Diámetro. @param g La figura. @return La figura. */
    EditorShapePtr add(double dia, const Shape& g);
    /** @brief `delete_shape`: la quita de su almacén y de la elección. @param s La figura. */
    void remove(const EditorShapePtr& s);

    /**
     * @brief `SelectEditorExc.click_release`: elige la figura más cercana si el clic cae en su caja agrandada.
     * @param pos El clic. @param toggle Elección múltiple.
     */
    void clickSelect(const Point2D& pos, bool toggle);

    /**
     * @brief `draw_selection_area_handler`: elige con un rectángulo.
     * @param a Esquina. @param b Opuesta. @param contains Contenidas (de izquierda a derecha) o que toca.
     * @param toggle Elección múltiple.
     */
    void areaSelect(const Point2D& a, const Point2D& b, bool contains, bool toggle);

    /**
     * @brief `ResizeEditorExc.make`: cambia el diámetro de las elegidas que están en @p dias.
     * @param dias Los diámetros elegidos en la tabla. @param newDia El nuevo.
     * @return Los diámetros que el original borra por quedar sin agujeros (`on_tool_delete`), o nada si se
     * cancela a mitad de camino (entonces lo elegido queda borrado, como en el original).
     */
    std::optional<std::vector<double>> resize(const std::set<double>& dias, double newDia);

    /**
     * @brief `on_tool_edit`: pasa las figuras de un diámetro a otro (las cruces y las ranuras cambian de tamaño) y
     * borra el viejo.
     * @param oldDia Diámetro viejo. @param newDia Nuevo.
     */
    void changeDia(double oldDia, double newDia);

    /** @brief Una herramienta del Excellon editado. */
    struct OutTool {
        double dia;                                       ///< Diámetro.
        std::vector<Point2D> drills;                      ///< Agujeros.
        std::vector<std::pair<Point2D, Point2D>> slots;   ///< Ranuras.
    };

    /** @brief `update_fcexcellon`: las herramientas, numeradas por diámetro creciente. @return La lista. */
    std::vector<OutTool> result() const;
};

}  // namespace flatcam
