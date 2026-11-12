/**
 * @file ExcEditor.h
 * @brief El editor de Excellon (port de `appEditors/AppExcEditor.py`) sobre `ExcEditorModel`: la tabla de
 * herramientas, agregar agujeros, ranuras y sus grillas, cambiar el diámetro, mover, copiar y borrar.
 */
#pragma once

#include <map>
#include <memory>
#include <optional>

#include <QObject>
#include <QPointF>

#include "editors/ExcEditorModel.h"
#include "gui/generated/ExcEditorUIBase.h"

class QAction;

namespace flatcam::gui {

class App;
class ExcellonObject;

/**
 * @brief `AppExcEditor`: edita un Excellon; al guardar crea un Excellon nuevo (`<nombre>_edit` si se cambió alguna
 * herramienta), como el original.
 */
class ExcEditor : public QObject {
    Q_OBJECT
public:
    /** @brief Crea el editor. @param app La aplicación. */
    explicit ExcEditor(App* app);
    ~ExcEditor() override;

    /** @brief Si está activo. @return `true` si lo está. */
    bool active() const { return active_; }
    /** @brief El panel (`exc_edit_widget`). @return El panel. */
    ExcEditorUIBase* panel() const { return ui_; }
    /** @brief El modelo. @return El modelo. */
    ExcEditorModel& model() { return model_; }

    /** @brief `edit_fcexcellon`. @param obj El objeto. */
    void edit(ExcellonObject* obj);
    /** @brief `update_fcexcellon` + `new_edited_excellon`: crea el Excellon editado y cierra. @param obj El objeto. */
    void save(ExcellonObject* obj);
    /** @brief `deactivate`. */
    void deactivate();
    /** @brief La rama `exc_editor` de `MainGUI.keyPressEvent`. @param key Tecla. @param modifiers Modificadores.
     *  @return Si se usó. */
    bool handleKey(int key, Qt::KeyboardModifiers modifiers);

    /** @brief `select_tool`. @param name `drill_select`, `drill_add`, `drill_array`, `slot_add`, `slot_array`,
     *  `drill_resize`, `drill_copy` o `drill_move`. */
    void selectTool(const QString& name);
    /** @brief La herramienta activa. @return El nombre. */
    QString activeToolName() const;

    /** @brief `on_tool_add`. @param dia Diámetro (vacío: el del campo). */
    void addTool(std::optional<double> dia = std::nullopt);
    /** @brief `on_tool_delete` de las filas elegidas o de @p dias. @param dias Diámetros. */
    void deleteTools(const std::vector<double>& dias = {});
    /** @brief `on_row_selected` (clic en la columna 0). @param row Fila. */
    void rowSelected(int row);
    /** @brief `on_slots_conversion`: las ranuras de las herramientas elegidas pasan a agujeros. */
    void convertSlots();

    /** @name Eventos del canvas (públicos para los tests) */
    ///@{
    /** @brief `on_canvas_click`. @param world Posición. @param button Botón. */
    void onPress(const QPointF& world, Qt::MouseButton button);
    /** @brief `on_canvas_move`. @param world Posición. @param leftDragging Arrastrando con el izquierdo. */
    void onMove(const QPointF& world, bool leftDragging);
    /** @brief `on_exc_click_release`. @param world Posición. @param button Botón. @param dragged Arrastre. */
    void onRelease(const QPointF& world, Qt::MouseButton button, bool dragged);
    ///@}

    /** @brief `replot`. */
    void replot();
    /** @brief `build_ui`: la tabla de herramientas. */
    void buildUi();
    /** @brief El último objeto creado al guardar. @return El objeto. */
    class FlatCAMObj* lastCreated() const { return lastCreated_; }

private:
    struct Tool;
    void onToolSelect(const QString& name);
    void newTool(const QString& name);
    void complete();
    void updateUtility(const Point2D& data);
    void deleteUtility();
    void deleteSelected();
    void selectRowsOfSelection();
    std::vector<double> tableSelectedDias() const;
    std::optional<double> lastToolDia() const;

    App* app_;
    ExcEditorUIBase* ui_;
    ExcEditorModel model_;
    std::unique_ptr<Tool> tool_;
    std::map<QString, QAction*> buttons_;
    std::map<int, double> tool2tooldia_;  ///< `tool2tooldia`.
    std::optional<int> lastToolSelected_; ///< `last_tool_selected`.
    ExcellonObject* obj_{nullptr};
    class FlatCAMObj* lastCreated_{nullptr};
    QString editedName_;
    bool isModified_{false};
    bool active_{false};
    bool inAction_{false};
    bool toolbarOldState_{false};
    std::optional<bool> selectionType_;
    Point2D pos_{0, 0};
    Point2D mouse_{0, 0};
};

}  // namespace flatcam::gui
