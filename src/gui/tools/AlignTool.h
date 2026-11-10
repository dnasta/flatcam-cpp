/**
 * @file AlignTool.h
 * @brief Herramienta Alinear objetos (port de `appTools/ToolAlignObjects.py`).
 */
#pragma once

#include <vector>

#include <QMetaObject>
#include <QPointF>

#include "core/Types.h"
#include "gui/generated/AlignToolUIBase.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/**
 * @brief Panel y lógica de Alinear objetos (Alt+A): con un punto se desplaza el objeto alineado del pad o agujero
 * elegido al del objeto de referencia; con dos, además se lo gira alrededor del primer destino, como el original.
 */
class AlignTool : public AlignToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit AlignTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();
    /** @brief "Alinear objeto" (`on_align`): empieza a esperar los clics. @return `false` si falta algún objeto. */
    bool align();
    /**
     * @brief Un clic en el canvas mientras se eligen los puntos (`on_mouse_click_release`).
     * @param world Posición. @param button Botón. @param dragged Si hubo arrastre (selección con rectángulo).
     */
    void onRelease(const QPointF& world, Qt::MouseButton button, bool dragged);
    /** @brief Si está esperando clics (`local_connected`). @return `true` si los espera. */
    bool isPicking() const { return picking_; }
    /** @brief Los puntos elegidos hasta ahora (`clicked_points`). @return Los puntos. */
    const std::vector<Point2D>& clickedPoints() const { return points_; }

private:
    /** @brief Llena el combo con los objetos del tipo (`on_type_obj_changed`). */
    void fillCombo(FCComboBox* combo, const QString& type);
    /** @brief Avanza según la cantidad de puntos (`check_points`). */
    void checkPoints();
    /** @brief Dibuja el objeto de destino en azul (`set_color`). */
    void setColor();
    /** @brief Dibuja los dos objetos con sus colores (`reset_color`). */
    void resetColor();
    /** @brief Deja de esperar clics y restaura la grilla (`disconnect_cal_events`). */
    void disconnectEvents();
    /** @brief Aplica el desplazamiento y, si corresponde, el giro. @param rotate Girar (dos pares de puntos). */
    void applyMove(bool rotate);

    App* app_;
    FlatCAMObj* aligned_{nullptr};
    FlatCAMObj* aligner_{nullptr};
    FlatCAMObj* target_{nullptr};
    std::vector<Point2D> points_;
    QString alignType_;
    bool picking_{false};
    bool gridMemory_{false};
    QMetaObject::Connection conn_;
};

}  // namespace flatcam::gui
