/**
 * @file Shape.h
 * @brief Figura geométrica 2D inmutable (equivalente a los objetos de Shapely) construida sobre GEOS.
 */
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "AffineTransform.h"
#include "Types.h"

struct GEOSGeom_t;  // Tipo opaco de GEOS (se evita incluir geos_c.h en la API pública).

namespace flatcam {

namespace detail {
struct ShapeAccess;
}

/**
 * @brief Tipo de una figura (mismos valores que `GEOSGeomTypes`).
 */
enum class ShapeType {
    Point = 0,               ///< Punto.
    LineString = 1,          ///< Polilínea abierta.
    LinearRing = 2,          ///< Polilínea cerrada y simple.
    Polygon = 3,             ///< Polígono con agujeros opcionales.
    MultiPoint = 4,          ///< Conjunto de puntos.
    MultiLineString = 5,     ///< Conjunto de polilíneas.
    MultiPolygon = 6,        ///< Conjunto de polígonos.
    GeometryCollection = 7   ///< Colección heterogénea.
};

/**
 * @brief Figura 2D: punto, línea, anillo, polígono o colecciones de ellos.
 *
 * Es el reemplazo de los objetos `shapely.geometry.*` que FlatCAM usa en todo su código. Internamente
 * guarda una geometría GEOS con propiedad compartida, por lo que copiar una @ref Shape es barato y las
 * figuras nunca se modifican: toda operación devuelve una figura nueva.
 *
 * Las operaciones que fallan en GEOS (por ejemplo, geometrías inválidas en una unión) lanzan
 * @ref GeometryError.
 *
 * @code
 * using namespace flatcam;
 * Shape disk = Shape::circle({0, 0}, 1.0, 32);
 * Shape ring = disk.buffer(0.5).difference(disk);
 * double area = ring.area();
 * @endcode
 */
class Shape {
public:
    /** @brief Construye una figura vacía (`GEOMETRYCOLLECTION EMPTY`). */
    Shape();

    // ---------------------------------------------------------------- fábricas

    /**
     * @brief Crea un punto.
     * @param p Coordenadas.
     * @return Figura de tipo Point.
     */
    static Shape point(const Point2D& p);

    /**
     * @brief Crea una polilínea abierta.
     * @param points Vértices (al menos 2).
     * @return Figura de tipo LineString.
     * @throws GeometryError si hay menos de 2 vértices.
     */
    static Shape lineString(const std::vector<Point2D>& points);

    /**
     * @brief Crea un anillo lineal; se cierra automáticamente si el último vértice no repite al primero.
     * @param points Vértices (al menos 3 distintos).
     * @return Figura de tipo LinearRing.
     * @throws GeometryError si el anillo no es válido.
     */
    static Shape linearRing(const std::vector<Point2D>& points);

    /**
     * @brief Crea un polígono; los anillos se cierran automáticamente.
     * @param exterior Vértices del contorno exterior.
     * @param holes Anillos interiores (agujeros), vacío por defecto.
     * @return Figura de tipo Polygon.
     * @throws GeometryError si los anillos no son válidos.
     */
    static Shape polygon(const std::vector<Point2D>& exterior,
                         const std::vector<std::vector<Point2D>>& holes = {});

    /**
     * @brief Crea un disco aproximado por un polígono, como `Point(c).buffer(r, resolution)` en Shapely.
     * @param center Centro.
     * @param radius Radio.
     * @param resolution Segmentos por cuarto de círculo.
     * @return Polígono que aproxima el disco.
     */
    static Shape circle(const Point2D& center, double radius, int resolution);

    /**
     * @brief Crea una figura a partir de texto WKT.
     * @param wkt Texto en formato Well-Known Text.
     * @return Figura correspondiente.
     * @throws GeometryError si el texto no es válido.
     */
    static Shape fromWkt(const std::string& wkt);

    /**
     * @brief Agrupa figuras en una colección (MultiPolygon, MultiLineString, MultiPoint o
     *        GeometryCollection según su contenido), sin fusionarlas.
     * @param shapes Figuras a agrupar.
     * @return Colección.
     */
    static Shape collection(const std::vector<Shape>& shapes);

    /**
     * @brief Unión de muchas figuras en una sola (equivale a `shapely.ops.unary_union`).
     * @param shapes Figuras a unir.
     * @return Figura resultante (vacía si la lista está vacía).
     * @throws GeometryError si GEOS falla.
     */
    static Shape unaryUnion(const std::vector<Shape>& shapes);

    // ------------------------------------------------------------- propiedades

    /** @brief Tipo de la figura. @return El tipo. */
    ShapeType type() const;
    /** @brief Indica si la figura no contiene puntos. @return `true` si está vacía. */
    bool isEmpty() const;
    /** @brief Indica si la figura es válida según las reglas OGC. @return `true` si es válida. */
    bool isValid() const;
    /** @brief Área de la figura (0 para puntos y líneas). @return El área. */
    double area() const;
    /** @brief Longitud de la figura (o perímetro en polígonos). @return La longitud. */
    double length() const;

    /**
     * @brief Caja delimitadora.
     * @return La caja, o `std::nullopt` si la figura está vacía.
     */
    std::optional<BoundingBox> bounds() const;

    /** @brief Centroide de la figura. @return El centroide. */
    Point2D centroid() const;

    /** @brief Cantidad de componentes de una figura múltiple (1 para figuras simples). @return Número de componentes. */
    std::size_t numParts() const;

    /**
     * @brief Componentes de una figura múltiple o colección.
     *
     * Reproduce la iteración sobre `MultiPolygon`/`MultiLineString`/`GeometryCollection` que hace
     * `camlib.py`. Una figura simple devuelve una lista con ella misma.
     * @return Lista de componentes.
     */
    std::vector<Shape> parts() const;

    /**
     * @brief Contorno exterior de un polígono.
     * @return Figura LinearRing.
     * @throws GeometryError si la figura no es un polígono.
     */
    Shape exterior() const;

    /**
     * @brief Anillos interiores (agujeros) de un polígono.
     * @return Lista de LinearRing; vacía si la figura no es un polígono.
     */
    std::vector<Shape> interiors() const;

    /**
     * @brief Vértices de un punto, polilínea o anillo (del contorno exterior en un polígono).
     * @return Lista de vértices en orden.
     */
    std::vector<Point2D> coordinates() const;

    // ------------------------------------------------------ operaciones booleanas

    /** @brief Unión con otra figura. @param o Otra figura. @return Resultado. */
    Shape unite(const Shape& o) const;
    /** @brief Diferencia (esta menos @p o). @param o Figura a restar. @return Resultado. */
    Shape difference(const Shape& o) const;
    /** @brief Intersección. @param o Otra figura. @return Resultado. */
    Shape intersection(const Shape& o) const;
    /** @brief Diferencia simétrica. @param o Otra figura. @return Resultado. */
    Shape symmetricDifference(const Shape& o) const;

    // --------------------------------------------------------------- predicados

    /** @brief Indica si @p o está completamente dentro de esta figura. @param o Otra figura. @return `true` si la contiene. */
    bool contains(const Shape& o) const;
    /** @brief Indica si esta figura está completamente dentro de @p o. @param o Otra figura. @return `true` si está contenida. */
    bool within(const Shape& o) const;
    /** @brief Indica si comparten al menos un punto. @param o Otra figura. @return `true` si se intersectan. */
    bool intersects(const Shape& o) const;
    /** @brief Indica si no comparten ningún punto. @param o Otra figura. @return `true` si son disjuntas. */
    bool disjoint(const Shape& o) const;
    /** @brief Distancia mínima a otra figura. @param o Otra figura. @return La distancia. */
    double distance(const Shape& o) const;
    /** @brief Igualdad topológica (mismo conjunto de puntos). @param o Otra figura. @return `true` si son equivalentes. */
    bool equals(const Shape& o) const;

    /**
     * @brief Igualdad exacta de coordenadas con tolerancia.
     * @param o Otra figura.
     * @param tolerance Diferencia máxima admitida por coordenada.
     * @return `true` si tienen la misma estructura y coordenadas dentro de la tolerancia.
     */
    bool equalsExact(const Shape& o, double tolerance = 0.0) const;

    // ----------------------------------------------------------- construcciones

    /**
     * @brief Buffer (offset) de la figura, equivalente a `shape.buffer(...)` de Shapely.
     * @param distance Distancia; negativa contrae los polígonos.
     * @param resolution Segmentos por cuarto de círculo.
     * @param join Tipo de unión de esquinas.
     * @param cap Tipo de extremos (solo afecta a líneas).
     * @param mitreLimit Límite de ingletes cuando @p join es JoinStyle::Mitre.
     * @return Figura con el buffer aplicado.
     * @throws GeometryError si GEOS falla.
     */
    Shape buffer(double distance, int resolution = 16, JoinStyle join = JoinStyle::Round,
                 CapStyle cap = CapStyle::Round, double mitreLimit = 5.0) const;

    /**
     * @brief Simplifica la figura (algoritmo de Douglas-Peucker).
     * @param tolerance Distancia máxima de desvío.
     * @param preserveTopology Si es `true` evita generar figuras inválidas.
     * @return Figura simplificada.
     */
    Shape simplify(double tolerance, bool preserveTopology = true) const;

    /** @brief Envolvente convexa. @return Figura convexa mínima que contiene a esta. */
    Shape convexHull() const;

    /** @brief Repara una figura inválida. @return Figura válida equivalente. */
    Shape makeValid() const;

    /**
     * @brief Convierte un LinearRing en LineString (otros tipos se devuelven sin cambios).
     * @return Figura resultante.
     */
    Shape asLineString() const;

    // ----------------------------------------------------------- transformaciones

    /**
     * @brief Aplica una transformación afín a todos los vértices.
     * @param t Transformación.
     * @return Figura transformada.
     */
    Shape transform(const AffineTransform& t) const;

    /**
     * @brief Escala la figura (`shapely.affinity.scale`).
     * @param sx Factor en X.
     * @param sy Factor en Y.
     * @param origin Punto fijo; si no se indica se usa el centro de la caja delimitadora.
     * @return Figura escalada.
     */
    Shape scale(double sx, double sy, const std::optional<Point2D>& origin = std::nullopt) const;

    /** @brief Traslada la figura. @param dx Desplazamiento en X. @param dy Desplazamiento en Y. @return Figura trasladada. */
    Shape translate(double dx, double dy) const;

    /**
     * @brief Rota la figura (`shapely.affinity.rotate`).
     * @param angleDegrees Ángulo en grados (antihorario positivo).
     * @param origin Centro de giro; si no se indica se usa el centro de la caja delimitadora.
     * @return Figura rotada.
     */
    Shape rotate(double angleDegrees, const std::optional<Point2D>& origin = std::nullopt) const;

    /**
     * @brief Inclina la figura (`shapely.affinity.skew`).
     * @param xAngleDegrees Ángulo de cizalla en X, en grados.
     * @param yAngleDegrees Ángulo de cizalla en Y, en grados.
     * @param origin Punto fijo; si no se indica se usa el centro de la caja delimitadora.
     * @return Figura inclinada.
     */
    Shape skew(double xAngleDegrees, double yAngleDegrees,
               const std::optional<Point2D>& origin = std::nullopt) const;

    // ------------------------------------------------------------------ salida

    /**
     * @brief Texto WKT de la figura.
     * @param precision Decimales a conservar; un valor negativo usa la precisión completa.
     * @return Texto WKT.
     */
    std::string toWkt(int precision = -1) const;

private:
    friend struct detail::ShapeAccess;

    /** @brief Construye tomando posesión de una geometría GEOS. @param g Geometría GEOS. */
    explicit Shape(std::shared_ptr<GEOSGeom_t> g) : geom_(std::move(g)) {}

    std::shared_ptr<GEOSGeom_t> geom_;  ///< Geometría GEOS con propiedad compartida.
};

/** @brief Lista de figuras: reemplaza a las listas (anidadas) de objetos Shapely de FlatCAM. */
using ShapeList = std::vector<Shape>;

}  // namespace flatcam
