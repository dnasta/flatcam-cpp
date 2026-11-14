/**
 * @file GCodeEditor.h
 * @brief El editor de G-code de los CNC Job (port de `appEditors/appGCodeEditor.py`): el texto del trabajo en una
 * pestaña "Editor de código" (`AppTextEditor`) y su panel (`GCodeEditorUI`, generado desde el original) con la tabla de
 * herramientas, que elige en el texto el G-code de cada una, y los dos fragmentos de código.
 */
#pragma once

#include <QObject>
#include <QPointer>

#include "gui/generated/GCodeEditorUIBase.h"

namespace flatcam::gui {

class App;
class AppTextEditor;
class CNCJobObject;

/** @brief El panel del editor de G-code (`AppGCodeEditorUI`). */
class GCodeEditorUI : public GCodeEditorUIBase {
    Q_OBJECT
public:
    /** @brief Crea y arma el panel. @param parent Padre. */
    explicit GCodeEditorUI(QWidget* parent = nullptr) : GCodeEditorUIBase(parent) { setupUi(); }
};

/**
 * @brief El editor de G-code (`AppGCodeEditor`).
 *
 * Como el original: al entrar carga `source_file` del CNC Job (el G-code armado al crear el objeto) en una pestaña
 * nueva; elegir filas en la tabla selecciona en el texto el G-code completo, la cabecera, el código de inicio o el de
 * cada herramienta (subrayado); al salir guardando, el texto pasa a `source_file`. "Insertar código" no hace nada
 * (`insert_gcode` es `pass`) y el nombre del panel no se usa.
 */
class GCodeEditor : public QObject {
    Q_OBJECT
public:
    /** @brief Crea el editor y su panel. @param app La aplicación. */
    explicit GCodeEditor(App* app);
    ~GCodeEditor() override;
    /** @brief El panel (`edit_widget`). @return El panel. */
    GCodeEditorUI* panel() const { return ui_; }
    /** @brief La pestaña con el texto (`gcode_editor_tab`), si está abierta. @return El editor o nulo. */
    AppTextEditor* tab() const;
    /** @brief El objeto que se edita. @return El objeto o nulo. */
    CNCJobObject* object() const { return obj_; }

    /** @brief `edit_fcgcode`: arma la pestaña y el panel y carga el texto. @param obj El CNC Job. */
    void edit(CNCJobObject* obj);
    /** @brief `update_fcgcode`: el texto de la pestaña pasa a `source_file`. @param obj El CNC Job. */
    void save(CNCJobObject* obj);
    /** @brief `deactivate`. */
    void deactivate();
    /** @brief Cierra la pestaña `gcode_editor_tab` (lo hace `editor2object` al salir). */
    void closeTab();

    /** @brief `on_row_selection_change`: selecciona en el texto lo de las filas elegidas. */
    void onRowSelectionChange();
    /** @brief `on_toggle_all_rows`: un clic en el encabezado elige todas las filas o ninguna. */
    void onToggleAllRows();

private:
    void setUi();
    void buildUi();
    void buildCncToolsTable();
    void buildExcellonCncTools();
    App* app_;
    GCodeEditorUI* ui_;
    QPointer<AppTextEditor> tab_;
    CNCJobObject* obj_{nullptr};
    QString editedObjName_;
};

}  // namespace flatcam::gui
