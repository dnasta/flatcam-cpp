/**
 * @file FlatCAMObj.h
 * @brief Objetos del proyecto: base común y objetos Gerber y Excellon (port de `appObjects/FlatCAMObj.py`,
 * `FlatCAMGerber.py` y `FlatCAMExcellon.py`).
 */
#pragma once

#include <QColor>
#include <QObject>
#include <QPointer>
#include <QWidget>
#include <QRectF>
#include <QString>
#include <memory>

#include "gui/PlotCanvas.h"
#include "io/JsonValue.h"
#include "parsers/Excellon.h"
#include "parsers/Gerber.h"

namespace flatcam::gui {

/** @brief Tipo de objeto (`kind`), en el orden de los grupos del árbol del proyecto. */
enum class ObjectKind { Gerber, Excellon, Geometry, CNCJob, Script, Document };

/**
 * @brief Convierte un color de FlatCAM (`#RRGGBB` o `#RRGGBBAA`, con la transparencia al final) en `QColor`.
 * @param fc Color como lo guarda FlatCAM.
 * @return El color.
 */
QColor colorFromFlatCam(const QString& fc);

/**
 * @brief Base de los objetos del proyecto (`FlatCAMObj`): nombre, tipo, si se dibuja y colores.
 *
 * El núcleo (parsers, herramientas) no depende de Qt; estos objetos lo envuelven para la interfaz, igual que en el
 * original `GerberObject(FlatCAMObj, Gerber)`.
 */
class FlatCAMObj : public QObject {
    Q_OBJECT
public:
    /** @brief Crea el objeto. @param kind Tipo. @param name Nombre. */
    FlatCAMObj(ObjectKind kind, QString name);

    /** @brief Número único del objeto en la sesión (identifica lo que dibuja en el canvas). @return El número. */
    int id() const { return id_; }

    /** @brief Tipo del objeto (`kind`). @return El tipo. */
    ObjectKind kind() const { return kind_; }

    /** @brief Nombre (`options["name"]`). @return El nombre. */
    const QString& name() const { return name_; }

    /** @brief Cambia el nombre. @param name Nombre nuevo. */
    void setName(const QString& name) { name_ = name; }

    /** @brief Si se dibuja (`options["plot"]`). @return El estado. */
    bool plotEnabled() const { return plot_; }

    /** @brief Muestra u oculta el objeto. @param on Dibujar. */
    void setPlotEnabled(bool on) { plot_ = on; }

    /**
     * @brief Lo que trajo el objeto desde un proyecto (`to_dict` del original, sin la geometría ni el G-code), para
     * volver a guardarlo con todo lo que el port todavía no usa. Nulo si el objeto se creó en esta sesión.
     */
    JsonValue projectData;

    QColor outlineColor;  ///< Color del contorno (`outline_color`).
    QColor fillColor;     ///< Color del relleno (`fill_color`).

    /**
     * @brief Lo que hay que dibujar (`plot`: las figuras que el original agrega a su `ShapeCollection`).
     * @return Triángulos, segmentos y colores.
     */
    virtual PlotLayerData plotData() const = 0;

    /** @brief Límites de la geometría (`bounds`). @return El rectángulo en mm (vacío si no hay geometría). */
    virtual QRectF bounds() const = 0;

    /**
     * @brief Panel de la pestaña Propiedades, ya cargado con este objeto (`set_ui` + `build_ui`).
     *
     * El objeto es dueño del panel; quien lo muestre en la pestaña debe sacarlo (`takeWidget`) antes de destruir el objeto.
     * @return El panel.
     */
    virtual QWidget* propertiesUi() = 0;

    /**
     * @brief Refleja el objeto alrededor de un eje (`mirror`).
     * @param axis `Axis::X` invierte las Y; `Axis::Y` invierte las X.
     * @param point Punto por el que pasa el eje.
     * @return `false` si el tipo de objeto no se puede reflejar (en la base, ninguno).
     */
    virtual bool mirror(Axis axis, const Point2D& point);
    /**
     * @brief La geometría sólida del objeto (`solid_geometry`), como lista de figuras.
     * @return Las figuras (en la base, ninguna: el CNC Job no la guarda en el port).
     */
    virtual ShapeList solidShapes() const { return {}; }
    /** @brief Rota (`rotate`). @param angle Grados (antihorario). @param point Centro. @return `false` si el tipo no se transforma. */
    virtual bool rotate(double angle, const Point2D& point);
    /** @brief Inclina (`skew`). @param ax Grados en X. @param ay Grados en Y. @param point Punto fijo. @return Ídem. */
    virtual bool skew(double ax, double ay, const Point2D& point);
    /** @brief Escala (`scale`). @param fx Factor en X. @param fy Factor en Y. @param point Punto fijo. @return Ídem. */
    virtual bool scale(double fx, double fy, const Point2D& point);
    /** @brief Desplaza (`offset`). @param dx En X. @param dy En Y. @return Ídem. */
    virtual bool offset(double dx, double dy);
    /**
     * @brief Engrosa (`buffer`). @param distance Distancia (o factor). @param join Esquinas. @param factor Si es un factor.
     * @return Ídem.
     */
    virtual bool buffer(double distance, JoinStyle join, bool factor);

    ~FlatCAMObj() override;

protected:
    std::unique_ptr<QWidget> ui_;  ///< Panel de propiedades (se crea al pedirlo por primera vez).

public:
    /** @brief Los polígonos de una figura (ella misma o sus partes). @param s Figura. @return Los polígonos. */
    static ShapeList polygonsOf(const Shape& s);

    /**
     * @brief Agrega una figura a un grupo de dibujo (para quien dibuja fuera de un objeto, como la selección de zonas).
     * @param s Figura.
     * @param group Grupo a completar.
     * @param bounds Límites a agrandar.
     * @param fill Si es `false` solo se dibuja el contorno.
     */
    static void addShapeTo(const Shape& s, PlotGroup& group, QRectF& bounds, bool fill = true) {
        addShape(s, group, bounds, fill);
    }

protected:
    /**
     * @brief Agrega una figura a lo dibujado: relleno (si tiene superficie) y contornos (`add_shape`).
     * @param s Figura.
     * @param out Datos a completar.
     * @param fill Si es `false` solo se dibuja el contorno.
     */
    static void addShape(const Shape& s, PlotLayerData& out, bool fill = true);

    /**
     * @brief Agrega una figura a un grupo con colores propios.
     * @param s Figura.
     * @param group Grupo a completar.
     * @param bounds Límites a agrandar.
     * @param fill Si es `false` solo se dibuja el contorno.
     */
    static void addShape(const Shape& s, PlotGroup& group, QRectF& bounds, bool fill = true);

private:
    int id_;
    ObjectKind kind_;
    QString name_;
    bool plot_{true};
};

/** @brief Objeto Gerber (`GerberObject`): un Gerber leído, dibujado con su relleno y su contorno. */
class GerberObject : public FlatCAMObj {
    Q_OBJECT
public:
    /** @brief Crea el objeto. @param name Nombre. @param gerber Gerber ya leído. */
    GerberObject(QString name, std::unique_ptr<Gerber> gerber);

    /** @brief El Gerber. @return Referencia al Gerber. */
    Gerber& gerber() { return *gerber_; }

    /** @brief Polígonos y líneas de la geometría sólida, rellenos (`options["solid"]` = verdadero). @return Datos. */
    PlotLayerData plotData() const override;

    /** @brief Límites. @return Rectángulo. */
    QRectF bounds() const override;

    /** @brief Panel `GerberObjectUI` cargado. @return El panel. */
    QWidget* propertiesUi() override;
    /** @brief Refleja el Gerber. @param axis Eje. @param point Punto del eje. @return `true`. */
    bool mirror(Axis axis, const Point2D& point) override;
    /** @brief `solid_geometry`. @return Las figuras. */
    ShapeList solidShapes() const override;
    bool rotate(double angle, const Point2D& point) override;
    bool skew(double ax, double ay, const Point2D& point) override;
    bool scale(double fx, double fy, const Point2D& point) override;
    bool offset(double dx, double dy) override;
    bool buffer(double distance, JoinStyle join, bool factor) override;

private:
    std::unique_ptr<Gerber> gerber_;
};

/** @brief Objeto Excellon (`ExcellonObject`): taladros y ranuras de cada herramienta. */
class ExcellonObject : public FlatCAMObj {
    Q_OBJECT
public:
    /** @brief Crea el objeto. @param name Nombre. @param excellon Excellon ya leído con su geometría. */
    ExcellonObject(QString name, std::unique_ptr<Excellon> excellon);

    /** @brief El Excellon. @return Referencia. */
    Excellon& excellon() { return *excellon_; }

    /** @brief La geometría de cada herramienta, rellena. @return Datos. */
    PlotLayerData plotData() const override;

    /** @brief Límites. @return Rectángulo. */
    QRectF bounds() const override;

    /** @brief Panel `ExcellonObjectUI` cargado. @return El panel. */
    QWidget* propertiesUi() override;
    /** @brief Refleja agujeros y ranuras. @param axis Eje. @param point Punto del eje. @return `true`. */
    bool mirror(Axis axis, const Point2D& point) override;
    /** @brief `solid_geometry`. @return Las figuras. */
    ShapeList solidShapes() const override;
    bool rotate(double angle, const Point2D& point) override;
    bool skew(double ax, double ay, const Point2D& point) override;
    bool scale(double fx, double fy, const Point2D& point) override;
    bool offset(double dx, double dy) override;
    bool buffer(double distance, JoinStyle join, bool factor) override;

private:
    std::unique_ptr<Excellon> excellon_;
};

}  // namespace flatcam::gui
