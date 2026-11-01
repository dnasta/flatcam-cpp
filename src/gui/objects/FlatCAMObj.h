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

    ~FlatCAMObj() override;

protected:
    std::unique_ptr<QWidget> ui_;  ///< Panel de propiedades (se crea al pedirlo por primera vez).

protected:
    /**
     * @brief Agrega una figura a lo dibujado: relleno (si tiene superficie) y contornos (`add_shape`).
     * @param s Figura.
     * @param out Datos a completar.
     * @param fill Si es `false` solo se dibuja el contorno.
     */
    static void addShape(const Shape& s, PlotLayerData& out, bool fill = true);

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

private:
    std::unique_ptr<Excellon> excellon_;
};

}  // namespace flatcam::gui
