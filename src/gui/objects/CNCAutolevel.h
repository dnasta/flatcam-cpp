/**
 * @file CNCAutolevel.h
 * @brief La autonivelación del panel del CNC Job (port de la parte de `appObjects/FlatCAMCNCJob.py` que la maneja) sobre
 * `Autolevel`: los puntos de sondeo en grilla o con clics, sus celdas, la tabla, el G-code de sondeo para guardar o ver,
 * el mapa de alturas y el control de GRBL por el puerto serie (si Qt trae QtSerialPort).
 */
#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QStringList>

#include "cam/Autolevel.h"

class QSerialPort;

namespace flatcam::gui {

class App;
class CNCJobObject;
class CNCObjectUI;

/** @brief El estado de autonivelación de un CNC Job y los manejadores de su panel. */
class CNCAutolevel : public QObject {
    Q_OBJECT
public:
    /** @brief Crea el controlador. @param app Aplicación. @param job El CNC Job. */
    CNCAutolevel(App* app, CNCJobObject* job);
    ~CNCAutolevel() override;

    /** @brief Conecta el panel y lo carga como `set_ui` (una vez por panel). @param ui El panel. */
    void attach(CNCObjectUI* ui);
    /** @brief `on_add_al_probepoints`. */
    void addProbePoints();
    /** @brief El clic de `on_mouse_click_release` en el modo manual. @param world Posición. @param button Botón.
     *  @param dragged Arrastre. */
    void onRelease(const QPointF& world, Qt::MouseButton button, bool dragged);
    /** @brief `on_key_press` mientras se agregan puntos. @param key Tecla. @return Si se usó. */
    bool handleKey(int key);
    /** @brief `import_height_map`. @param filename Archivo. @return Si se leyó. */
    bool importHeightMap(const QString& filename);
    /** @brief `on_save_probing_gcode` sin el diálogo. @param filename Archivo. @return Si se escribió. */
    bool saveProbingGcode(const QString& filename);
    /** @brief El G-code de sondeo (`probing_gcode_text`). @return El texto. */
    const QString& probingGcode() const { return probingText_; }
    /** @brief Los puntos (`al_voronoi_geo_storage`). @return Los puntos. */
    const std::vector<ProbePoint>& points() const { return points_; }
    /** @brief Si se están agregando puntos con el mouse. @return `mouse_events_connected`. */
    bool isPicking() const { return picking_; }
    /** @brief `on_grbl_jog` sin puerto: los comandos que mandaría. @param direction Dirección. @return Comandos. */
    QStringList jogCommands(const QString& direction) const;

private:
    void buildTable();
    void onModeRadio(const QString& v);
    void onMethodRadio(const QString& v);
    void onControllerChange();
    void showProbingGeo(bool state);
    void regenerateGcode();
    void finishPicking(bool rebuild);
    std::optional<BoundingBox> cutBounds() const;
    bool mm() const;
    // GRBL
    void searchPorts(bool muted);
    void connectPort();
    std::optional<QStringList> sendCommand(const QString& cmd, bool echo = true);
    QStringList wake();
    std::optional<double> getParameter(QString param);
    void grblAutolevel();
    void saveHeightMap();
    void shell(const QString& msg);

    App* app_;
    CNCJobObject* job_;
    CNCObjectUI* ui_{nullptr};
    std::vector<ProbePoint> points_;            ///< `al_voronoi_geo_storage`, por número desde 1.
    std::vector<Point2D> bilinear_;             ///< `al_bilinear_geo_storage`.
    QString probingText_;                       ///< `probing_gcode_text`.
    QString grblProbeResult_;                   ///< `grbl_probe_result`.
    bool picking_{false};
    QMetaObject::Connection conn_;
    std::unique_ptr<QSerialPort> port_;
};

}  // namespace flatcam::gui
