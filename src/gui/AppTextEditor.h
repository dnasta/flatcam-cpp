/**
 * @file AppTextEditor.h
 * @brief El editor de texto de FlatCAM (`appEditors/AppTextEditor.py`): la pestaña que usan el objeto Script, el
 * Documento, "Ver fuente" y el "Editor de código", con buscar, reemplazar, vista previa, imprimir, abrir, guardar
 * (texto o PDF) y "Ejecutar" (los scripts).
 */
#pragma once

#include <QPlainTextEdit>
#include <QTextEdit>
#include <QWidget>
#include <functional>

#include "gui/widgets/FCButton.h"
#include "gui/widgets/FCEntry.h"
#include "gui/widgets/FCTextEdits.h"

class QCheckBox;
class QFrame;
class QGridLayout;
class QToolButton;

namespace flatcam::gui {

class App;

/** @brief `AppTextEditor`: el editor de texto en una pestaña del área de dibujo. */
class AppTextEditor : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief Arma el editor.
     * @param app La aplicación (carpetas, opciones, mensajes); puede ser nula en los tests.
     * @param plainText Texto plano con números de línea (`FCTextAreaLineNumber`); si no, con formato
     * (`FCTextAreaExtended`, el del Documento).
     * @param parent Padre.
     */
    AppTextEditor(App* app, bool plainText, QWidget* parent = nullptr);

    QFrame* t_frame{nullptr};                ///< `t_frame`: el marco con el editor y los botones.
    QGridLayout* work_editor_layout{nullptr};  ///< `work_editor_layout`.
    FCTextAreaLineNumber* editor_class{nullptr};  ///< `editor_class` (solo en texto plano).
    FCButton* buttonFind{nullptr};           ///< "Buscar".
    FCEntry* entryFind{nullptr};             ///< El texto a buscar.
    FCButton* buttonReplace{nullptr};        ///< "Reemplazar con".
    FCEntry* entryReplace{nullptr};          ///< El texto nuevo.
    QCheckBox* sel_all_cb{nullptr};          ///< "Todos".
    QToolButton* button_update_code{nullptr};  ///< "Guardar los cambios internamente" (oculto).
    QToolButton* buttonPreview{nullptr};     ///< Vista previa de impresión.
    QToolButton* buttonPrint{nullptr};       ///< Imprimir.
    QToolButton* buttonOpen{nullptr};        ///< Abrir un archivo.
    QToolButton* buttonSave{nullptr};        ///< Guardar en un archivo.
    FCButton* buttonRun{nullptr};            ///< "Ejecutar" (oculto salvo en los scripts).

    /** @brief El editor de texto plano (`code_editor`), o nulo si es el de formato. @return El editor. */
    FCTextAreaLineNumber::PlainTextEdit* plainEditor() const { return plain_; }
    /** @brief El editor con formato (`code_editor` del Documento), o nulo. @return El editor. */
    FCTextAreaExtended* richEditor() const { return rich_; }
    /** @brief El editor que haya (`code_editor`). @return El widget. */
    QWidget* codeEditor() const;
    /** @brief El texto (`code_editor.toPlainText()`). @return El texto. */
    QString toPlainText() const;
    /** @brief El texto como HTML (`toHtml`; en el de texto plano, el texto). @return El texto. */
    QString toHtml() const;
    /** @brief El documento del editor. @return El documento. */
    QTextDocument* document() const;
    /** @brief El cursor del editor. @return El cursor. */
    QTextCursor textCursor() const;
    /** @brief Cambia el cursor. @param c Cursor. */
    void setTextCursor(const QTextCursor& c);
    /** @brief Mueve el cursor. @param op Movimiento. @param mode Si extiende la selección. */
    void moveCursor(QTextCursor::MoveOperation op, QTextCursor::MoveMode mode = QTextCursor::MoveAnchor);
    /** @brief Activa el autocompletado (`code_editor.completer_enable`). @param on Activo. */
    void setCompleterEnabled(bool on);
    /** @brief Las palabras del autocompletado (`set_model_data(app.myKeywords)`). @param words Palabras. */
    void setModelData(const QStringList& words);

    /**
     * @brief `load_text`: carga un texto sin marcar el editor como modificado (desconecta `textChanged`).
     * @param text Texto. @param moveToStart Cursor al principio. @param moveToEnd Cursor al final (si no va al principio).
     * @param clearText Borrar lo que había. @param asHtml Como HTML (si no, texto plano).
     */
    void loadText(const QString& text, bool moveToStart = false, bool moveToEnd = false, bool clearText = true,
                  bool asHtml = false);
    /** @brief `handleTextChanged`: el botón Guardar pasa a rojo. */
    void handleTextChanged();
    /** @brief `handleFindGCode`: busca desde el cursor (distingue mayúsculas); si no está, desde el principio. */
    void handleFindGCode();
    /** @brief `handleReplaceGCode`: reemplaza lo elegido o, con "Todos", desde el cursor hasta el final. */
    void handleReplaceGCode();
    /**
     * @brief `handleOpen`: pide un archivo y pone su texto en el editor (como texto plano).
     * @param filter Filtro del diálogo (vacío: los de G-code). @param filename Archivo (vacío: el diálogo).
     */
    void handleOpen(const QString& filter = {}, const QString& filename = {});
    /**
     * @brief `handleSaveGCode`: guarda el texto en un archivo (o en PDF si termina en `.pdf`), lo anota en los recientes
     * como "cncjob" y avisa.
     * @param name Nombre propuesto (vacío: el del objeto activo, o "file"). @param filter Filtro (vacío: los de G-code).
     * @param filename Archivo (vacío: el diálogo). @return Si se guardó.
     */
    bool handleSaveGCode(const QString& name = {}, const QString& filter = {}, const QString& filename = {});
    /** @brief `handlePrint`: el diálogo de imprimir del sistema. */
    void handlePrint();
    /** @brief `handlePreview`: la vista previa de impresión del sistema. */
    void handlePreview();

    /** @brief Lo que hace "Abrir" (el original lo reconecta con otro filtro). */
    std::function<void()> onOpen;
    /** @brief Lo que hace "Guardar" (ídem). */
    std::function<void()> onSave;

Q_SIGNALS:
    /** @brief "Ejecutar" (`buttonRun.clicked`). */
    void runRequested();

private:
    void connectTextChanged();
    App* app_;
    FCTextAreaLineNumber::PlainTextEdit* plain_{nullptr};
    FCTextAreaExtended* rich_{nullptr};
    QMetaObject::Connection textChanged_;
};

/**
 * @brief Escribe un texto en un PDF como el `SimpleDocTemplate` + `Paragraph` de reportlab que usa `handleSaveGCode`:
 * Helvetica de 10 puntos con interlineado de 12, en la hoja y con los márgenes dados.
 * @param text Texto (cada línea, un renglón). @param filename Archivo. @param pageMm Ancho y alto de la hoja en mm.
 * @param marginsMm Márgenes izquierdo, superior, derecho e inferior en mm. @return Si se escribió.
 */
bool writeTextPdf(const QString& text, const QString& filename, QSizeF pageMm, const QMarginsF& marginsMm);

}  // namespace flatcam::gui
