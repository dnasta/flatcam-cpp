/**
 * @file MoveTool.h
 * @brief Herramienta "Mover" (port de `appTools/ToolMove.py`): mueve los objetos elegidos con dos clics.
 */
#pragma once

#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <optional>
#include <vector>

#include "core/Types.h"

namespace flatcam::gui {

class App;

/**
 * @brief Mueve los objetos elegidos del Proyecto: el primer clic izquierdo es el punto de partida, el movimiento
 * arrastra la caja de la selección y el segundo clic es el destino. No tiene panel (en el original es un `AppTool`
 * oculto). Se activa y desactiva con la tecla M o Editar → Mover; Escape cancela.
 *
 * Desvío: el original mueve también un CNC Job llamando a su `offset`, que en un trabajo de Geometría solo mueve el
 * dibujo (el G-code exportado no cambia) y en uno de Perforación no mueve nada; el port deja los CNC Job donde están.
 */
class MoveTool : public QObject {
    Q_OBJECT
public:
    /** @brief Crea la herramienta. @param app Aplicación. @param parent Objeto padre. */
    explicit MoveTool(App* app, QObject* parent = nullptr);
    /** @brief `toggle`: activa la herramienta o, si estaba activa, la termina. */
    void toggle();
    /** @brief Si está activa (`isVisible`). @return El estado. */
    bool active() const { return visible_; }
    /** @brief Escape (`on_key_press`): cancela. */
    void cancel();
    /**
     * @brief Botón presionado en el canvas (`on_left_click`; público para los tests).
     * @param world Posición en mm (ya ajustada a la grilla).
     * @param button Botón.
     */
    void onClick(const QPointF& world, Qt::MouseButton button);
    /** @brief Movimiento sobre el canvas (`on_move`; público para los tests). @param world Posición en mm. */
    void onMove(const QPointF& world);
    /** @brief Capa auxiliar del canvas donde se dibuja la caja de la selección. */
    static constexpr int kLayerId = -3;

private:
    void drawSelectionBox();
    void drawBox(double dx, double dy);
    App* app_;
    bool visible_{false};
    int clickedMove_{0};
    std::optional<QPointF> point1_;
    std::vector<Point2D> oldCoords_;
    std::vector<QMetaObject::Connection> connections_;
};

}  // namespace flatcam::gui
