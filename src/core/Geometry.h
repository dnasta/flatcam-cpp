/**
 * @file Geometry.h
 * @brief Contenedor de geometría base de FlatCAM (port de la clase `Geometry` de `camlib.py`).
 */
#pragma once

#include <map>
#include <optional>
#include <utility>
#include <vector>

#include "Shape.h"
#include "Types.h"

namespace flatcam {

/**
 * @brief Geometría asociada a una herramienta (equivale a `tools[n]['solid_geometry']` en Python).
 */
struct ToolEntry {
    ShapeList solidGeometry;  ///< Figuras sólidas de esta herramienta.
};

/**
 * @brief Clase base de todos los objetos con geometría (Gerber, Excellon, Geometry, CNCJob).
 *
 * Es el port de `camlib.Geometry`. Guarda la geometría sólida como una lista de @ref Shape y ofrece
 * las operaciones comunes: agregar figuras, calcular límites, aplanar, reflejar, rotar, inclinar,
 * aplicar buffer y convertir unidades.
 *
 * ### Diferencias respecto de Python
 * - Las listas anidadas de objetos Shapely se representan con una @ref ShapeList plana; las figuras
 *   múltiples (por ejemplo un MultiPolygon) se siguen tratando como un solo elemento y se expanden
 *   al aplanar.
 * - `solid_geometry = None` y `solid_geometry = []` son equivalentes (lista vacía).
 * - `bounds()` devuelve `(0, 0, 0, 0)` cuando no hay geometría, en lugar de `(inf, inf, -inf, -inf)`.
 * - `union()` se llama @ref unionAll porque `union` es una palabra reservada de C++.
 * - Los métodos que en Python dependen de la aplicación (barra de progreso, opciones, mensajes) no
 *   forman parte de esta clase; esa responsabilidad pasa a las capas superiores.
 *
 * Las subclases deben reimplementar @ref scale y @ref offset, que en la base no hacen nada (igual que
 * en `camlib.py`).
 */
class Geometry {
public:
    /**
     * @brief Crea un contenedor vacío.
     * @param stepsPerCircle Segmentos por cuarto de círculo usados en los buffers (`geo_steps_per_circle`).
     * @param units Unidades de la geometría.
     */
    explicit Geometry(int stepsPerCircle = 128, Units units = Units::Millimeters);

    /** @brief Destructor virtual. */
    virtual ~Geometry() = default;

    // ---------------------------------------------------------------- atributos

    /** @brief Unidades actuales de la geometría. @return Las unidades. */
    Units units() const { return units_; }
    /** @brief Cambia las unidades sin escalar la geometría. @param u Nuevas unidades. */
    void setUnits(Units u) { units_ = u; }

    /** @brief Segmentos por cuarto de círculo usados en los buffers. @return Cantidad de segmentos. */
    int stepsPerCircle() const { return stepsPerCircle_; }
    /** @brief Cambia los segmentos por cuarto de círculo. @param n Cantidad (>= 1). */
    void setStepsPerCircle(int n) { stepsPerCircle_ = n; }

    /** @brief Tolerancia de dibujo (`drawing_tolerance`). @return La tolerancia. */
    double drawingTolerance() const { return drawingTolerance_; }
    /** @brief Cambia la tolerancia de dibujo. @param t Nueva tolerancia. */
    void setDrawingTolerance(double t) { drawingTolerance_ = t; }

    /** @brief Indica si la geometría se guarda por herramienta (`multigeo`). @return `true` si está activo el modo multigeo. */
    bool multigeo() const { return multigeo_; }
    /** @brief Activa o desactiva el modo multigeo. @param on Nuevo valor. */
    void setMultigeo(bool on) { multigeo_ = on; }

    /** @brief Factor de conversión aplicado al cambiar de unidades (`file_units_factor`). @return El factor. */
    double fileUnitsFactor() const { return fileUnitsFactor_; }

    /** @brief Figuras sólidas (acceso de lectura y escritura). @return Referencia a la lista. */
    ShapeList& solidGeometry() { return solid_; }
    /** @brief Figuras sólidas (solo lectura). @return Referencia constante a la lista. */
    const ShapeList& solidGeometry() const { return solid_; }

    /** @brief Figuras de seguimiento (`follow_geometry`). @return Referencia a la lista. */
    ShapeList& followGeometry() { return follow_; }
    /** @brief Figuras de seguimiento (solo lectura). @return Referencia constante a la lista. */
    const ShapeList& followGeometry() const { return follow_; }

    /** @brief Resultado del último aplanado (`flat_geometry`). @return Referencia constante a la lista. */
    const ShapeList& flatGeometry() const { return flat_; }

    /** @brief Geometría por herramienta (acceso de lectura y escritura). @return Referencia al mapa de herramientas. */
    std::map<int, ToolEntry>& tools() { return tools_; }
    /** @brief Geometría por herramienta (solo lectura). @return Referencia constante al mapa de herramientas. */
    const std::map<int, ToolEntry>& tools() const { return tools_; }

    // ------------------------------------------------------------- agregar figuras

    /**
     * @brief Agrega un círculo a la geometría (`add_circle`).
     * @param origin Centro.
     * @param radius Radio.
     * @param tool Número de herramienta; si no existe se usa la 1.
     * @return `true` si se agregó; `false` si el círculo resultó inválido (`"fail"` en Python).
     */
    bool addCircle(const Point2D& origin, double radius, std::optional<int> tool = std::nullopt);

    /**
     * @brief Agrega un polígono (`add_polygon`).
     * @param points Vértices del contorno.
     * @param tool Número de herramienta; si no existe se usa la 1.
     * @return `true` si se agregó; `false` si el polígono es inválido.
     */
    bool addPolygon(const std::vector<Point2D>& points, std::optional<int> tool = std::nullopt);

    /**
     * @brief Agrega una polilínea (`add_polyline`).
     * @param points Vértices de la polilínea.
     * @param tool Número de herramienta; si no existe se usa la 1.
     * @return `true` si se agregó; `false` si la polilínea es inválida.
     */
    bool addPolyline(const std::vector<Point2D>& points, std::optional<int> tool = std::nullopt);

    /**
     * @brief Resta un polígono de los trazos de la geometría (`subtract_polygon`).
     *
     * Solo opera sobre los trazos: los polígonos se convierten en sus contornos y se les resta
     * @p points. El resultado reemplaza a la geometría sólida.
     * @param points Vértices del polígono a restar.
     */
    void subtractPolygon(const std::vector<Point2D>& points);

    // ------------------------------------------------------------------ consultas

    /** @brief Indica si no hay figuras sólidas (`is_empty`). @return `true` si la lista sólida está vacía. */
    bool isEmpty() const { return solid_.empty(); }

    /**
     * @brief Límites rectangulares de la geometría (`bounds`).
     * @param flatten Si es `true`, primero aplana la geometría y reemplaza la lista sólida por el
     *        resultado (igual que en Python).
     * @return Caja delimitadora, o `(0, 0, 0, 0)` si no hay geometría.
     */
    BoundingBox bounds(bool flatten = false);

    /** @brief Ancho y alto de los límites (`size`). @return Par `(ancho, alto)`. */
    std::pair<double, double> size();

    /**
     * @brief Busca la figura que contiene un punto (`find_polygon`).
     * @param point Punto a ubicar.
     * @param geoset Conjunto donde buscar; si es nulo se usa la geometría sólida.
     * @return La primera figura que contiene al punto, o `std::nullopt`.
     */
    std::optional<Shape> findPolygon(const Point2D& point, const ShapeList* geoset = nullptr) const;

    /**
     * @brief Todos los anillos interiores (agujeros) de los polígonos (`get_interiors`).
     * @param geometry Conjunto a recorrer; si es nulo se usa la geometría sólida.
     * @return Lista de anillos interiores.
     */
    ShapeList getInteriors(const ShapeList* geometry = nullptr) const;

    /**
     * @brief Todos los contornos exteriores de los polígonos (`get_exteriors`).
     * @param geometry Conjunto a recorrer; si es nulo se usa la geometría sólida.
     * @return Lista de contornos exteriores.
     */
    ShapeList getExteriors(const ShapeList* geometry = nullptr) const;

    /**
     * @brief Expande figuras múltiples en una lista de figuras simples (`flatten`).
     *
     * El resultado queda en @ref flatGeometry.
     * @param geometry Conjunto a aplanar; si es nulo se usa la geometría sólida.
     * @param reset Si es `true` vacía @ref flatGeometry antes de empezar.
     * @param pathOnly Si es `true`, los polígonos se expanden en su contorno exterior y sus anillos
     *        interiores.
     * @return Referencia a @ref flatGeometry.
     */
    const ShapeList& flatten(const ShapeList* geometry = nullptr, bool reset = true, bool pathOnly = false);

    // ------------------------------------------------------------ transformaciones

    /** @brief Reemplaza la geometría sólida por la unión de todas sus figuras (`union`). */
    void unionAll();

    /**
     * @brief Escala la geometría. En la base no hace nada; las subclases lo reimplementan.
     * @param xfactor Factor en X.
     * @param yfactor Factor en Y.
     * @param point Punto de referencia, si corresponde.
     */
    virtual void scale(double xfactor, double yfactor, const std::optional<Point2D>& point = std::nullopt);

    /**
     * @brief Desplaza la geometría. En la base no hace nada; las subclases lo reimplementan.
     * @param vect Vector de desplazamiento.
     */
    virtual void offset(const Point2D& vect);

    /**
     * @brief Convierte la geometría a otras unidades (`convert_units`).
     * @param objUnits Unidades de destino.
     * @return Factor de escala aplicado (1.0 si las unidades ya coinciden).
     */
    double convertUnits(Units objUnits);

    /**
     * @brief Refleja la geometría alrededor de un eje (`mirror`).
     * @param axis Eje de reflexión.
     * @param point Punto por el que pasa el eje.
     */
    void mirror(Axis axis, const Point2D& point);

    /**
     * @brief Rota la geometría (`rotate`).
     * @param angle Ángulo en grados (antihorario positivo).
     * @param point Centro de giro.
     */
    void rotate(double angle, const Point2D& point);

    /**
     * @brief Inclina la geometría (`skew`).
     * @param angleX Ángulo de cizalla en X, en grados.
     * @param angleY Ángulo de cizalla en Y, en grados.
     * @param point Punto fijo.
     */
    void skew(double angleX, double angleY, const Point2D& point);

    /**
     * @brief Aplica un buffer a cada figura (`buffer`).
     * @param distance Distancia de offset; si @p factor es `true` es el factor de escala.
     * @param join Tipo de unión de esquinas.
     * @param factor Si es `true`, en lugar de un buffer escala cada figura respecto de su centro.
     */
    void buffer(double distance, JoinStyle join, bool factor);

private:
    /** @brief Aplica @p fn a cada figura de la geometría sólida (o de las herramientas en multigeo). */
    template <typename Fn>
    void applyToAll(Fn fn);

    /** @brief Agrega una figura a la herramienta indicada (o a la 1 si no existe). */
    void addToTool(const Shape& shape, std::optional<int> tool);

    Units units_;
    int stepsPerCircle_;
    double drawingTolerance_{0.0};
    bool multigeo_{false};
    double fileUnitsFactor_{1.0};
    ShapeList solid_;
    ShapeList follow_;
    ShapeList flat_;
    std::map<int, ToolEntry> tools_;
};

}  // namespace flatcam
