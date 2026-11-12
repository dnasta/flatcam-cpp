/**
 * @file GrbEditor.h
 * @brief El editor de Gerber (port de `appEditors/AppGerberEditor.py`) sobre `GrbEditorModel`: la tabla de
 * aperturas, pads y grillas de pads, pistas, regiones, discos y semidiscos, poligonizar, engrosar, escalar, marcar
 * áreas, la goma, mover, copiar, borrar y transformar.
 */
#pragma once

#include <map>
#include <memory>
#include <optional>

#include <QObject>
#include <QPointF>

#include "editors/GrbEditorModel.h"
#include "gui/generated/GrbEditorUIBase.h"
#include "gui/generated/GrbTransformToolUIBase.h"

class QAction;

namespace flatcam::gui {

class App;
class GerberObject;

/**
 * @brief `AppGerberEditor`: edita un Gerber; al guardar crea `<nombre>_edit` (o el número siguiente), como el original.
 */
class GrbEditor : public QObject {
    Q_OBJECT
public:
    /** @brief Crea el editor. @param app La aplicación. */
    explicit GrbEditor(App* app);
    ~GrbEditor() override;

    /** @brief Si está activo. @return `true` si lo está. */
    bool active() const { return active_; }
    /** @brief El panel (`grb_edit_widget`). @return El panel. */
    GrbEditorUIBase* panel() const { return ui_; }
    /** @brief El modelo. @return El modelo. */
    GrbEditorModel& model() { return model_; }

    /** @brief `edit_fcgerber`. @param obj El objeto. */
    void edit(GerberObject* obj);
    /** @brief `update_fcgerber` + `new_edited_gerber`. @param obj El objeto. */
    void save(GerberObject* obj);
    /** @brief `deactivate_grb_editor`. */
    void deactivate();
    /** @brief La rama `grb_editor` de `MainGUI.keyPressEvent`. @param key Tecla. @param modifiers Modificadores.
     *  @return Si se usó. */
    bool handleKey(int key, Qt::KeyboardModifiers modifiers);
    /** @brief `select_tool`. @param name `select`, `pad`, `array`, `track`, `region`, `poligonize`, `semidisc`,
     *  `disc`, `buffer`, `scale`, `markarea`, `eraser`, `copy`, `transform` o `move`. */
    void selectTool(const QString& name);
    /** @brief La herramienta activa. @return El nombre. */
    QString activeToolName() const;
    /** @brief `on_aperture_add`. @param code Código (vacío: el del campo). */
    void addAperture(const QString& code = QString());
    /** @brief `on_aperture_delete` de las filas elegidas. */
    void deleteApertures();
    /** @brief `on_row_selected`. @param row Fila. */
    void rowSelected(int row);
    /** @brief `build_ui`. */
    void buildUi();
    /** @brief `plot_all`. */
    void replot();
    /** @brief El último objeto creado al guardar. @return El objeto. */
    class FlatCAMObj* lastCreated() const { return lastCreated_; }

    /** @name Eventos del canvas (públicos para los tests) */
    ///@{
    /** @brief `on_canvas_click`. @param world Posición. @param button Botón. */
    void onPress(const QPointF& world, Qt::MouseButton button);
    /** @brief `on_canvas_move`. @param world Posición. @param leftDragging Arrastrando. */
    void onMove(const QPointF& world, bool leftDragging);
    /** @brief `on_grb_click_release`. @param world Posición. @param button Botón. @param dragged Arrastre. */
    void onRelease(const QPointF& world, Qt::MouseButton button, bool dragged);
    ///@}

private:
    struct Tool;
    void onToolSelect(const QString& name);
    void newTool(const QString& name);
    void finishTool();
    void updateUtility(const Point2D& data);
    void deleteUtility();
    std::vector<std::string> tableSelectedCodes() const;
    void selectRowsOfSelection();
    void transform(const QString& op);

    App* app_;
    GrbEditorUIBase* ui_;
    GrbTransformToolUIBase* transformUi_;
    GrbEditorModel model_;
    std::unique_ptr<Tool> tool_;
    std::map<QString, QAction*> buttons_;
    std::optional<std::string> lastAperture_;  ///< `last_aperture_selected`.
    std::vector<GrbShapePtr> marked_;          ///< `geo_to_delete` de marcar área.
    GerberObject* obj_{nullptr};
    class FlatCAMObj* lastCreated_{nullptr};
    QString editedName_;
    int bendMode_{1};                          ///< `bend_mode`.
    bool active_{false};
    bool inAction_{false};
    bool toolbarOldState_{false};
    std::optional<bool> selectionType_;
    Point2D pos_{0, 0};
    Point2D mouse_{0, 0};
};

}  // namespace flatcam::gui
