/**
 * @file DocumentObject.h
 * @brief El objeto Documento (`appObjects/FlatCAMDocument.py`): un texto con formato (HTML) que se edita en una
 * pestaña "Editor de Documentos" del área de dibujo, con el panel `DocumentObjectUI` (fuente, tamaño, negrita,
 * alineación, colores, tabulación). Lo crean Archivo -> Nuevo -> Documento y, con su informe, la Verificación de reglas.
 */
#pragma once

#include "gui/generated/DocumentObjectUIBase.h"
#include "gui/objects/ScriptObject.h"

namespace flatcam::gui {

/** @brief El panel `DocumentObjectUI` de la pestaña Propiedades. */
class DocumentObjectUI : public DocumentObjectUIBase {
    Q_OBJECT
public:
    /** @brief Crea y arma el panel (con la fuente de cada sistema, como el original). @param parent Padre. */
    explicit DocumentObjectUI(QWidget* parent = nullptr);
};

/** @brief Un Documento (`DocumentObject`). */
class DocumentObject : public TextObject {
    Q_OBJECT
public:
    /** @brief Crea el documento. @param name Nombre. @param text El texto (`source_file`). @param readOnly `read_only`. */
    DocumentObject(QString name, QString text, bool readOnly = false);
    /** @brief `read_only` (el original lo guarda pero el editor nunca queda de solo lectura). @return El valor. */
    bool readOnly() const { return readOnly_; }
    /** @brief `set_ui` del Documento: editor con formato, los controles del panel y el texto (HTML o plano). @param app Aplicación. */
    void setUi(App* app) override;
    /** @brief `build_ui`: agrega la pestaña "Editor de Documentos" si no estaba y la muestra. */
    void buildUi() override;
    /** @brief El panel. @return El panel. */
    DocumentObjectUI* ui() const { return static_cast<DocumentObjectUI*>(ui_.get()); }

private:
    void onTabSizeChange(std::optional<int> val);
    bool readOnly_;
};

}  // namespace flatcam::gui
