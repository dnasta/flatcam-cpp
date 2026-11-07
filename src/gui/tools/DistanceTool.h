/**
 * @file DistanceTool.h
 * @brief Herramientas "Distancia" (port de `appTools/ToolDistance.py`: medir entre dos clics) y "Distancia mínima"
 * (`appTools/ToolDistanceMin.py`: la menor distancia entre dos objetos elegidos).
 */
#pragma once

#include <QMetaObject>
#include <QPointF>
#include <optional>
#include <vector>

#include "gui/generated/DistanceMinToolUIBase.h"
#include "gui/generated/DistanceToolUIBase.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;

/**
 * @brief Mide la distancia entre dos puntos elegidos con el mouse (`Distance`).
 *
 * Al activarse desconecta los manejadores del mouse de la aplicación y toma los suyos: el primer clic izquierdo fija
 * el inicio, el movimiento dibuja la línea provisoria y muestra el ángulo, y el segundo clic calcula Dx, Dy, el ángulo
 * (0 a 360°) y la distancia y termina. El clic derecho (sin arrastre) cancela. Los clics salen ajustados a la grilla
 * si está activa. "Ajustar al centro" solo existe en los editores, que el port no tiene: queda oculto.
 */
class DistanceTool : public DistanceToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit DistanceTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
    /** @brief `run`: activa la medición o, si ya estaba activa, la termina. */
    void run();
    /** @brief Si está midiendo (`active`). @return El estado. */
    bool active() const { return active_; }
    /** @brief Empieza a medir (`activate_measure_tool`; también el botón "Medida"). */
    void activate();
    /** @brief Termina de medir (`deactivate_measure_tool`). */
    void deactivate();
    /**
     * @brief Botón soltado en el canvas (`on_mouse_click_release`; público para los tests).
     * @param world Posición en mm (ya ajustada a la grilla).
     * @param button Botón.
     * @param dragged Si hubo arrastre.
     */
    void onRelease(const QPointF& world, Qt::MouseButton button, bool dragged);
    /** @brief Movimiento sobre el canvas (`on_mouse_move_meas`; público para los tests). @param world Posición en mm. */
    void onMove(const QPointF& world);
    /** @brief Capa auxiliar del canvas donde se dibuja la línea de medición. */
    static constexpr int kLayerId = -2;

private:
    void setToolUi();
    void calculateDistance(const QPointF& pos);
    void drawLine(const QPointF& pos);
    App* app_;
    bool active_{false};
    bool toolDone_{false};
    bool gridMemory_{false};  ///< `grid_status_memory` (el original nunca lo vuelve a `False`).
    std::vector<QPointF> points_;
    std::optional<QPointF> relPoint1_;
    std::vector<QMetaObject::Connection> connections_;
};

/**
 * @brief La menor distancia entre los dos objetos elegidos del Proyecto (`DistanceMin`): sus puntos más cercanos,
 * Dx, Dy, el ángulo (`atan(dy / dx)`, sin cambiar si dx = 0), la distancia y el punto medio, al que se puede saltar.
 */
class DistanceMinTool : public DistanceMinToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit DistanceMinTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
    /** @brief `run`: muestra el panel con sus valores iniciales y pide elegir dos objetos. */
    void run();
    /** @brief Mide (`activate_measure_tool`; el botón "Medida"). */
    void measure();
    /** @brief Salta al punto medio (`on_jump_to_half_point`). */
    void jumpToHalfPoint();
    /** @brief Punto medio de la última medición (`h_point`). @return El punto. */
    QPointF halfPoint() const { return hPoint_; }

private:
    void setToolUi();
    App* app_;
    QPointF hPoint_{0, 0};
};

}  // namespace flatcam::gui
