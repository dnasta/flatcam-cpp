/**
 * @file CanvasPicker.h
 * @brief Elegir zonas o polígonos con el mouse en el canvas (las selecciones "Selección de área" y "Selección de
 * polígono" de NCC, Paint e Isolation).
 */
#pragma once

#include <QObject>
#include <QPointF>
#include <functional>
#include <vector>

#include "core/Shape.h"

namespace flatcam::gui {

class MainGUI;
class PlotCanvas;

/**
 * @brief Selección interactiva en el canvas, como `on_mouse_release` y `on_poly_mouse_click_release` de las herramientas.
 *
 * - **Zonas** (`area_shape` "square"): cada par de clics izquierdos es un rectángulo; el clic derecho termina.
 * - **Zonas** ("polygon"): cada clic izquierdo agrega un vértice; el derecho cierra el polígono (si tiene más de dos
 *   vértices) y otro derecho, sin vértices pendientes, termina.
 * - **Polígonos**: cada clic izquierdo marca o desmarca el polígono que está debajo; el derecho termina.
 *
 * Lo elegido se dibuja en una capa auxiliar con los colores de selección (`global_sel_fill`, `global_sel_line`). Escape
 * cancela. Un clic derecho que fue un arrastre (para desplazar la vista) no cuenta.
 */
class CanvasPicker : public QObject {
    Q_OBJECT
public:
    /** @brief Qué se elige. */
    enum class Mode { Rectangles, Polygons, PickPolygons };

    /** @brief Crea el selector. @param ui Ventana (canvas y barra de estado). @param parent Objeto padre. */
    CanvasPicker(MainGUI* ui, QObject* parent = nullptr);

    /**
     * @brief Empieza a elegir.
     * @param mode Qué se elige.
     * @param candidates Polígonos que se pueden marcar (modo `PickPolygons`).
     * @param done Qué hacer con lo elegido al terminar (no se llama si se cancela o no se eligió nada).
     */
    void start(Mode mode, ShapeList candidates, std::function<void(const ShapeList&)> done);

    /** @brief Cancela la selección en curso (Escape). @return `true` si había una. */
    bool cancel();

    /** @brief Si hay una selección en curso. @return `true` si la hay. */
    bool active() const { return active_; }

    /**
     * @brief Procesa un botón soltado en el canvas (lo llama la señal del canvas; público para los tests).
     * @param world Posición en mm.
     * @param button Botón.
     * @param dragged Si fue un arrastre.
     */
    void onRelease(const QPointF& world, Qt::MouseButton button, bool dragged);

    /** @brief Identificador de la capa auxiliar donde se dibuja la selección. */
    static constexpr int kLayerId = -1;

private:
    void redraw();
    void finish();

    MainGUI* ui_;
    bool active_{false};
    bool gridMemory_{false};  ///< `grid_status_memory`: la grilla estaba activa al empezar a elegir polígonos.
    Mode mode_{Mode::Rectangles};
    ShapeList candidates_;
    std::function<void(const ShapeList&)> done_;
    ShapeList selected_;                ///< Zonas o polígonos elegidos.
    std::vector<Point2D> points_;       ///< Vértices del polígono en curso (o el primer punto del rectángulo).
};

}  // namespace flatcam::gui
