/**
 * @file DblSidedTool.h
 * @brief Herramienta PCB a 2 caras: reflejar objetos y crear agujeros de alineación (port de `appTools/ToolDblSided.py`).
 */
#pragma once

#include <QMetaObject>
#include <QPointF>

#include "gui/generated/DblSidedToolUIBase.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/**
 * @brief Panel y lógica de PCB a 2 caras.
 *
 * Como el original: "Espejo" refleja el objeto elegido respecto de un eje (X o Y) que pasa por un punto escrito, por el
 * centro de la caja de otro objeto o por el centro de un agujero de un Excellon que se elige con un clic en el canvas;
 * "Crear Excellon" arma el objeto "Alignment Drills" con los agujeros pedidos y sus reflejos; los botones de límites
 * calculan la caja de los objetos elegidos y llevan sus valores al punto.
 */
class DblSidedTool : public DblSidedToolUIBase {
    Q_OBJECT
public:
    /** @brief Si está esperando el clic sobre un agujero (desconecta la selección de la aplicación). @return El estado. */
    bool picking() const { return static_cast<bool>(pickConnection_); }
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit DblSidedTool(App* app, QWidget* parent = nullptr);

    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();

    /** @brief Prepara la herramienta al abrirla (`set_tool_ui`). */
    void setToolUi();

    /** @brief Refleja el objeto elegido (`on_mirror`). @return `true` si se reflejó. */
    bool mirrorObject();

    /** @brief Crea el Excellon de alineación (`on_create_alignment_holes`). @return El objeto, o `nullptr`. */
    FlatCAMObj* createAlignmentHoles();

    /**
     * @brief Elige como punto del eje el centro del agujero que contiene a @p world (clic con `on_pick_hole` activo).
     * @param world Posición del clic, en mm.
     * @return `true` si había un agujero ahí.
     */
    bool pickHoleAt(const QPointF& world);

private:
    void fillCombo(class FCComboBox* combo, const QString& type);
    void onTogglePointbox();
    void onBboxCoordinates();
    void setPointCoordinate(int index, double value);
    std::optional<QPointF> point() const;
    FlatCAMObj* comboObject(const class FCComboBox* combo) const;
    void stopPicking();

    App* app_;
    QString drillValues_;                 ///< `drill_values`: los agujeros agregados con el botón.
    FlatCAMObj* pickObject_{nullptr};     ///< Excellon del que se elige un agujero.
    QMetaObject::Connection pickConnection_;
};

}  // namespace flatcam::gui
