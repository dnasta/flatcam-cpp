/**
 * @file GeometryObject.h
 * @brief Objeto Geometría: trayectorias de herramienta con sus parámetros de corte (port de
 * `appObjects/FlatCAMGeometry.py`).
 */
#pragma once

#include <array>
#include <map>
#include <memory>
#include <optional>

#include "cam/CncJob.h"
#include "gui/objects/FlatCAMObj.h"
#include "io/JsonValue.h"

namespace flatcam::gui {

class CNCJobObject;

/**
 * @brief Una herramienta de la Geometría (`tools[uid]`): diámetro, tipo, parámetros (`data`) y su geometría.
 *
 * Los parámetros que son comunes a todas las herramientas en el original (cambio de herramienta, Z final, X,Y final,
 * postprocesador) también se guardan en cada una, como en `update_common_param_in_storage`.
 */
struct GeometryTool {
    double tooldia{0.0};          ///< Diámetro (`tooldia`).
    QString offset{"Path"};       ///< Desplazamiento: `Path`, `In`, `Out` o `Custom` (`offset`).
    double offsetValue{0.0};      ///< Valor del desplazamiento personalizado (`offset_value`).
    QString type{"Rough"};        ///< Tipo de trabajo (`type`).
    QString toolType{"C1"};       ///< Forma de la herramienta (`tool_type`).
    GeometryToolSettings data;    ///< Parámetros de corte (`data`).
    double vtipDia{0.1};          ///< Diámetro de la punta en V (`data['vtipdia']`).
    double vtipAngle{30.0};       ///< Ángulo de la punta en V (`data['vtipangle']`).
    QString ppname{"default"};    ///< Postprocesador (`data['ppname_g']`).
    bool toolchange{false};       ///< Cambio de herramienta (`data['toolchange']`).
    QString optimization{"R"};    ///< Optimización del recorrido (`data['optimization_type']`; `R` = rtree).
    QColor overrideColor;         ///< Color propio (`data['override_color']`, los puentes finos del corte).
    ShapeList solidGeometry;      ///< Geometría de la herramienta (`solid_geometry`).
    JsonValue projectData;        ///< `data` tal como vino de un proyecto (claves que el port no usa), o nulo.
};

/**
 * @brief Los parámetros de una herramienta como el diccionario `data` del original (para guardar el proyecto).
 * @param t Herramienta.
 * @param base Diccionario de partida (el que vino del proyecto, o vacío); se conservan las claves que el port no usa.
 * @return El diccionario.
 */
JsonValue geometryToolDataJson(const GeometryTool& t, JsonValue base = JsonValue::object());

/**
 * @brief Parámetros de fábrica de una herramienta nueva (`geometry_*` de `defaults.py`, como `default_data`).
 * @return Los parámetros, con la tolerancia del CNC Job (`global_tolerance / 20`).
 */
GeometryTool defaultGeometryTool();

/**
 * @brief Objeto Geometría (`GeometryObject`), siempre "multigeo" como las que crean las herramientas: cada herramienta
 * guarda su propia geometría.
 *
 * Se dibuja solo con líneas, del color `geometry_plot_line` (`#FFAAAA`: en el original `is_theme_black` se usa sin
 * llamarla y la condición siempre es verdadera). Las herramientas con color propio (los puentes finos del Corte,
 * `#29a3a3fa`) se dibujan con el suyo. "Generar objeto CNCJob" crea `<nombre>_cnc` con
 * `mtool_gen_cncjob`, con todas las herramientas elegidas en un mismo trabajo.
 */
class GeometryObject : public FlatCAMObj {
    Q_OBJECT
public:
    /** @brief Crea el objeto. @param name Nombre. */
    explicit GeometryObject(QString name);

    /** @brief Herramientas, por identificador (`tools`). @return El mapa. */
    std::map<int, GeometryTool>& tools() { return tools_; }
    /** @brief Herramientas (solo lectura). @return El mapa. */
    const std::map<int, GeometryTool>& tools() const { return tools_; }

    /** @brief Geometría de todo el objeto (`solid_geometry`). @return Las figuras. */
    ShapeList& solidGeometry() { return solid_; }

    /** @brief Líneas de todas las herramientas. @return Datos. */
    PlotLayerData plotData() const override;
    /** @brief Límites de la geometría. @return Rectángulo. */
    QRectF bounds() const override;
    /** @brief Panel `GeometryObjectUI` cargado. @return El panel. */
    QWidget* propertiesUi() override;
    /**
     * @brief Refleja la geometría del objeto y la de cada herramienta (`Geometry.mirror`).
     * @param axis Eje. @param point Punto del eje. @return `true`.
     */
    bool mirror(Axis axis, const Point2D& point) override;
    /** @brief `solid_geometry`. @return Las figuras. */
    ShapeList solidShapes() const override { return solid_; }
    /** @brief Rota la geometría y la de cada herramienta. @param angle Grados. @param point Centro. @return `true`. */
    bool rotate(double angle, const Point2D& point) override;
    /** @brief Inclina. @param ax Grados en X. @param ay Grados en Y. @param point Punto fijo. @return `true`. */
    bool skew(double ax, double ay, const Point2D& point) override;
    /** @brief Escala. @param fx Factor en X. @param fy Factor en Y. @param point Punto fijo. @return `true`. */
    bool scale(double fx, double fy, const Point2D& point) override;
    /** @brief Desplaza. @param dx En X. @param dy En Y. @return `true`. */
    bool offset(double dx, double dy) override;
    /**
     * @brief Engrosa cada figura (`Geometry.buffer`: las líneas pasan a polígonos) o la escala alrededor de su centro.
     * @param distance Distancia (o factor). @param join Esquinas. @param factor Si es un factor. @return `true`.
     */
    bool buffer(double distance, JoinStyle join, bool factor) override;
    /**
     * @brief `GeometryObject.convert_units`: escala la geometría y los parámetros de cada herramienta (diámetro con los
     * decimales de la aplicación, desplazamiento, Z, avances). Error del original que se reproduce: las herramientas
     * pierden su geometría (el diccionario nuevo no copia `solid_geometry`).
     * @param units Unidades. @return El factor.
     */
    double convertUnits(Units units) override;
    /** @brief Unidades del objeto (las de la aplicación al crearlo). @return Las unidades. */
    Units units() const { return units_; }

    /**
     * @brief Genera el CNC Job de las herramientas pedidas (`mtool_gen_cncjob`, rama multigeo).
     * @param toolIds Herramientas, en el orden de la tabla.
     * @param[out] error Motivo si falla.
     * @param exclusionAreas Áreas de exclusión de la aplicación (los viajes las esquivan).
     * @param toolsDict Las herramientas que se usan (`tools_dict`, una copia con otros parámetros, como la del comando
     * Tcl `cncjob`), o nada para las del objeto.
     * @param messages Si no es nulo, recibe los avisos de cada herramienta en orden (los del generador, "G-Code
     * parsing in progress...", los de `gcode_parse`, "G-Code parsing finished..." y "Finished G-Code processing...").
     * @return El objeto, o `nullptr` si falló.
     */
    std::unique_ptr<CNCJobObject> makeCncJob(const std::vector<int>& toolIds, QString& error,
                                             const std::vector<ExclusionArea>& exclusionAreas = {},
                                             const std::map<int, GeometryTool>* toolsDict = nullptr,
                                             QStringList* messages = nullptr) const;

    /** @brief Color de las líneas (`geometry_plot_line`). @return El color. */
    static QColor plotLineColor();

    /**
     * @brief Marca el objeto como de una sola geometría (`multigeo = False`), como los que crea `new_object` sin
     * herramientas (los comandos `new_geometry`, `exteriors`, `interiors`): en el original lo que vale es
     * `solid_geometry`; el port lo muestra y lo corta con la herramienta 1, que pasa a ser una copia de esa lista.
     * `set_ui` le pone a la herramienta 1 `'solid_geometry': self.solid_geometry`: con una lista (`exteriors`,
     * `interiors`) es **la misma lista** y lo que se agregue queda dos veces mientras dure el vínculo (transformar o
     * reemplazar la geometría lo corta: el original reasigna); con `None` (`new_geometry`), agregar falla (ver
     * @ref addShapeLikeTheOriginal).
     * @param sharesList Si `solid_geometry` era una lista al crear la herramienta (si no, era `None`).
     */
    void setSingleGeo(bool sharesList);
    /**
     * @brief Como @ref setSingleGeo, pero con `solid_geometry` una figura sola y no una lista (`geo_obj.solid_geometry =
     * figura` de `bounding_box` y `non_copper_regions`): la herramienta 1 recibe la misma figura, así que agregar le
     * falla a las dos (`add_circle` antes de agregar; `add_polygon` / `add_polyline` unen y fallan al agregar a la
     * herramienta).
     * @param shape La figura.
     */
    void setSingleGeoShape(const Shape& shape);
    /** @brief Si es de una sola geometría (@ref setSingleGeo). @return `true` si lo es. */
    bool isSingleGeo() const { return singleGeo_; }
    /** @brief Si `solid_geometry` es una lista (y no una figura sola, @ref setSingleGeoShape). @return `true` si lo es. */
    bool solidIsList() const { return !solidNotList_; }
    /**
     * @brief Reemplaza `solid_geometry` (`union`, `subtract_polygon`).
     * @param shapes Las figuras.
     * @param asList `false` si en el original queda una figura y no una lista (`subtract_polygon` asigna
     * `unary_union(...)`): entonces `add_circle` falla y `add_polygon` / `add_polyline` unen.
     */
    void setSolidGeometry(ShapeList shapes, bool asList = true);
    /**
     * @brief `Geometry.add_circle` / `add_polygon` / `add_polyline`: agrega la figura a `solid_geometry` y a la
     * herramienta 1, como el original (dos veces si las dos listas son la misma, @ref setSingleGeo).
     * @param shape Figura.
     * @param circle Si es `add_circle` (con `solid_geometry` que no es una lista falla en vez de unir).
     * @return El error de Python que da el original (`KeyError: 1` sin herramienta 1, `AttributeError`), o vacío.
     */
    std::optional<QString> addShapeLikeTheOriginal(const Shape& shape, bool circle);

    /**
     * @brief Guarda los límites en las opciones (`options['xmin'...] = self.bounds()`), como `new_object` y los
     * `add_*` que salen bien. En un objeto de una sola geometría el original no los vuelve a calcular al transformarlo
     * desde la consola, así que `options` y la cabecera del G-code de `cncjob` muestran los guardados.
     * Como `Geometry.bounds`: con `solid_geometry = None` da los enteros 0; con una lista sin figuras, infinitos.
     */
    void captureOptionBounds();
    /**
     * @brief Los límites guardados por @ref captureOptionBounds (con el tipo de Python: enteros o reales).
     * @return xmin, ymin, xmax, ymax, o nada si no se guardaron (entonces valen los actuales).
     */
    const std::optional<std::array<JsonValue, 4>>& optionBounds() const { return optionBounds_; }
    /** @brief Los límites de las opciones: los guardados o, si no hay, los actuales. @return xmin, ymin, xmax, ymax. */
    BoundingBox optionBoundsBox() const;

private:
    template <typename Fn> void applyToAll(Fn fn);
    ShapeList solid_;
    std::map<int, GeometryTool> tools_;
    Units units_{Units::Millimeters};
    void syncSingleGeo();
    bool singleGeo_{false};
    bool aliased_{false};
    QString toolAppendError_;  ///< Lo que da `tools[1]['solid_geometry'].append` si no es una lista.
    bool solidNotList_{false};
    std::optional<std::array<JsonValue, 4>> optionBounds_;
    BoundingBox optionBoundsBox_{};
};

}  // namespace flatcam::gui
