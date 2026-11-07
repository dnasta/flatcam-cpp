/**
 * @file GeometryObject.h
 * @brief Objeto Geometría: trayectorias de herramienta con sus parámetros de corte (port de
 * `appObjects/FlatCAMGeometry.py`).
 */
#pragma once

#include <map>
#include <memory>

#include "cam/CncJob.h"
#include "gui/objects/FlatCAMObj.h"

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
     * @brief Genera el CNC Job de las herramientas pedidas (`mtool_gen_cncjob`, rama multigeo).
     * @param toolIds Herramientas, en el orden de la tabla.
     * @param[out] error Motivo si falla.
     * @param exclusionAreas Áreas de exclusión de la aplicación (los viajes las esquivan).
     * @return El objeto, o `nullptr` si falló.
     */
    std::unique_ptr<CNCJobObject> makeCncJob(const std::vector<int>& toolIds, QString& error,
                                             const std::vector<ExclusionArea>& exclusionAreas = {}) const;

    /** @brief Color de las líneas (`geometry_plot_line`). @return El color. */
    static QColor plotLineColor();

private:
    template <typename Fn> void applyToAll(Fn fn);
    ShapeList solid_;
    std::map<int, GeometryTool> tools_;
};

}  // namespace flatcam::gui
