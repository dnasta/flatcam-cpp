/**
 * @file ScriptObject.h
 * @brief El objeto Script (`appObjects/FlatCAMScript.py`): un script Tcl que se edita en una pestaña "Editor de
 * guiones" del área de dibujo (`AppTextEditor` con números de línea y el botón "Ejecutar") y su panel de Propiedades
 * (`ScriptObjectUI`, generado desde el original).
 */
#pragma once

#include <QPointer>

#include "gui/generated/ScriptObjectUIBase.h"
#include "gui/objects/FlatCAMObj.h"

namespace flatcam::gui {

class App;
class AppTextEditor;

/** @brief El panel `ScriptObjectUI` de la pestaña Propiedades. */
class ScriptObjectUI : public ScriptObjectUIBase {
    Q_OBJECT
public:
    /** @brief Crea y arma el panel. @param parent Padre. */
    explicit ScriptObjectUI(QWidget* parent = nullptr) : ScriptObjectUIBase(parent) { setupUi(); }
};

/**
 * @brief Lo común de Script y Documento: un texto (`source_file`) sin geometría que se edita en una pestaña propia.
 *
 * Como en el original: el editor y el panel se arman una sola vez, al agregar el objeto (`set_ui`, @ref setUi) y, cada
 * vez que se lo elige, @ref buildUi vuelve a poner la pestaña si se cerró.
 */
class TextObject : public FlatCAMObj {
    Q_OBJECT
public:
    /** @brief Crea el objeto. @param kind Script o Documento. @param name Nombre. @param source El texto. */
    TextObject(ObjectKind kind, QString name, QString source);
    ~TextObject() override;
    /** @brief El texto guardado (`source_file`). @return El texto. */
    const QString& sourceFile() const { return source_; }
    /** @brief Cambia el texto guardado. @param s Texto. */
    void setSourceFile(const QString& s) { source_ = s; }
    /** @brief El editor de la pestaña (nulo antes de @ref setUi). @return El editor. */
    AppTextEditor* editor() const;
    /** @brief No dibuja nada. @return Vacío. */
    PlotLayerData plotData() const override { return {}; }
    /** @brief `bounds`: sin geometría (`None, None, None, None`). @return Vacío. */
    QRectF bounds() const override { return {}; }
    /** @brief El panel (ya armado por @ref setUi). @return El panel. */
    QWidget* propertiesUi() override { return ui_.get(); }
    /** @brief `set_ui`: arma el panel y el editor y carga el texto. @param app La aplicación. */
    virtual void setUi(App* app) = 0;
    /** @brief `build_ui`: vuelve a poner la pestaña del editor si no está y, según el tipo, la muestra. */
    virtual void buildUi() = 0;

    // mirror, offset, rotate, scale, skew y buffer no hacen nada (`pass`): quedan los de la base.

protected:
    /** @brief Agrega la pestaña si no está. @param title Título. @return Si la agregó. */
    bool addTabIfMissing(const QString& title);
    App* app_{nullptr};              ///< La aplicación.
    QPointer<AppTextEditor> editor_;  ///< El editor (`script_editor_tab` / `document_editor_tab`).

private:
    QString source_;
};

/** @brief El objeto Script (`ScriptObject`). */
class ScriptObject : public TextObject {
    Q_OBJECT
public:
    /** @brief Crea el script. @param name Nombre. @param source Texto (`source_file`). */
    ScriptObject(QString name, QString source);
    /** @brief `set_ui` del Script: editor de texto plano con "Ejecutar" y el autocompletado. @param app Aplicación. */
    void setUi(App* app) override;
    /** @brief `build_ui`: agrega la pestaña "Editor de guiones" (y la muestra) si no estaba. */
    void buildUi() override;
    /** @brief El panel. @return El panel. */
    ScriptObjectUI* ui() const { return static_cast<ScriptObjectUI*>(ui_.get()); }
};

}  // namespace flatcam::gui
