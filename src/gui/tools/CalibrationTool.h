/**
 * @file CalibrationTool.h
 * @brief Calibración (port de `appTools/ToolCalibration.py`) sobre `Calibration`: cuatro puntos elegidos con el mouse
 * (sobre los agujeros o pads de un objeto, o libres), el G-code para verificarlos, los factores de escala e inclinación
 * que salen de lo medido y un objeto `<nombre>_calibrated` con ellos.
 */
#pragma once

#include <array>
#include <vector>

#include <QMetaObject>
#include <QPointF>

#include "gui/generated/CalibrationToolUIBase.h"
#include "gui/tools/ToolInfo.h"
#include "tools/Calibration.h"

class QLabel;

namespace flatcam::gui {

class App;
class FCEntry;
class FlatCAMObj;

/** @brief El panel de Calibración. */
class CalibrationTool : public CalibrationToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app La aplicación. @param parent Padre. */
    explicit CalibrationTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y pestaña. @return Los textos. */
    static ToolInfo info();
    /** @brief `set_tool_ui`. */
    void setToolUi();
    /** @brief `on_start_collect_points`. @return Si empezó a esperar clics. */
    bool startCollectPoints();
    /** @brief `on_mouse_click_release`. @param world Posición. @param button Botón. @param dragged Arrastre. */
    void onRelease(const QPointF& world, Qt::MouseButton button, bool dragged);
    /** @brief `generate_verification_gcode`. @return El G-code (vacío si faltan puntos). */
    QString generateVerificationGcode();
    /** @brief `calculate_factors`. */
    void calculateFactors();
    /** @brief `on_scale_button`. */
    void scalePoints();
    /** @brief `on_skew_button`. */
    void skewPoints();
    /** @brief `on_cal_button_click` + `new_calibrated_object`. @return El objeto nuevo, o nulo. */
    FlatCAMObj* calibrate();
    /** @brief Si está esperando clics. @return `local_connected`. */
    bool isPicking() const { return picking_; }
    /** @brief Los puntos elegidos (`click_points`). @return Los puntos. */
    const std::vector<Point2D>& clickPoints() const { return points_; }
    /** @brief "Created on" del G-code (para los tests). @param t Texto fijo; vacío: la hora. */
    void setTimeOverride(const QString& t) { timeOverride_ = t; }

    /** @name Las celdas de la tabla de puntos (EvalEntry del original) */
    ///@{
    std::array<FCEntry*, 8> tgt{};    ///< `*_coordx_tgt` / `*_coordy_tgt`: abajo-izq, abajo-der, arriba-izq, arriba-der.
    std::array<FCEntry*, 8> found{};  ///< `*_coordx_found` / `*_coordy_found`, en el mismo orden.
    ///@}

private:
    void buildPointsTable();
    void fillCombo(FCComboBox* combo, int typeIndex);
    void checkPoints();
    void resetPoints();
    void disconnectEvents();
    void setTargets(const CalibrationPoints& pts);
    CalibrationPoints readTargets();
    App* app_;
    std::vector<Point2D> points_;  ///< `click_points`.
    FlatCAMObj* target_{nullptr};
    bool picking_{false};
    bool gridMemory_{false};
    QMetaObject::Connection conn_;
    QString timeOverride_;
};

}  // namespace flatcam::gui
