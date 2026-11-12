/**
 * @file DocumentObject.h
 * @brief El objeto Documento (`appObjects/FlatCAMDocument.py`, en su forma más simple): un texto (HTML) que se ve en
 * solo lectura en la pestaña Propiedades. Lo crea, por ejemplo, la Verificación de reglas con su informe.
 */
#pragma once

#include "gui/objects/FlatCAMObj.h"

namespace flatcam::gui {

/** @brief Un Documento: texto sin geometría. */
class DocumentObject : public FlatCAMObj {
    Q_OBJECT
public:
    /** @brief Crea el documento. @param name Nombre. @param text El texto (`source_file`). @param readOnly Solo lectura. */
    DocumentObject(QString name, QString text, bool readOnly = true);
    /** @brief El texto. @return El texto. */
    const QString& text() const { return text_; }
    /** @brief `read_only`. @return Si es de solo lectura. */
    bool readOnly() const { return readOnly_; }
    /** @brief No dibuja nada. @return Vacío. */
    PlotLayerData plotData() const override { return {}; }
    /** @brief Sin geometría. @return Vacío. */
    QRectF bounds() const override { return {}; }
    /** @brief El texto en un visor. @return El panel. */
    QWidget* propertiesUi() override;

private:
    QString text_;
    bool readOnly_;
};

}  // namespace flatcam::gui
