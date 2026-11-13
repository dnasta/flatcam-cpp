/**
 * @file FCTextEdits.h
 * @brief Editores de texto de FlatCAM (port de `MyCompleter`, `FCTextEdit`, `FCTextAreaExtended`,
 * `FCPlainTextAreaExtended` y `FCTextAreaLineNumber` de `appGUI/GUIElements.py`): el menú contextual propio, el
 * autocompletado, los paréntesis automáticos, Ctrl+Shift+V (pega cambiando `\` por `/`) y Ctrl+/ (comentar la línea).
 */
#pragma once

#include <QCompleter>
#include <QFrame>
#include <QPlainTextEdit>
#include <QStringListModel>
#include <QTextEdit>

namespace flatcam::gui {

/** @brief `MyCompleter`: un `QCompleter` que recuerda la última opción resaltada. */
class MyCompleter : public QCompleter {
    Q_OBJECT
public:
    /** @brief Crea el completador. @param parent Padre. */
    explicit MyCompleter(QObject* parent = nullptr);
    /** @brief La última opción resaltada (`getSelected`). @return El texto. */
    QString selected() const { return lastSelected_; }

Q_SIGNALS:
    /** @brief Pide insertar una palabra (`insertText`). @param text La palabra. */
    void insertText(const QString& text);

private:
    QString lastSelected_;
};

/**
 * @brief Lo común de los dos editores con autocompletado (`FCTextAreaExtended` y `FCPlainTextAreaExtended`, que en el
 * original repiten el mismo código): el modelo de palabras, insertar la elegida, comentar y las teclas.
 *
 * @tparam Base `QTextEdit` o `QPlainTextEdit`.
 */
template <typename Base>
class FCTextAreaCommon : public Base {
public:
    /** @brief Crea el editor. @param parent Padre. */
    explicit FCTextAreaCommon(QWidget* parent = nullptr);
    /** @brief Las palabras del autocompletado (`set_model_data`). @param words Palabras. */
    void setModelData(const QStringList& words) { model_.setStringList(words); }
    /** @brief Si el autocompletado está activo (`completer_enable`). */
    bool completerEnable{false};
    /** @brief El completador. @return El completador. */
    MyCompleter* completer() { return &completer_; }
    /** @brief Pone o quita "# " al comienzo de la línea del cursor (`comment`, Ctrl+/). */
    void comment();

protected:
    /** @brief Ctrl+Shift+V, Ctrl+/, la palabra elegida y los paréntesis (`keyPressEvent`). @param e Evento. */
    void keyPressEvent(QKeyEvent* e) override;
    /** @brief Al recibir el foco, el completador pasa a este editor. @param e Evento. */
    void focusInEvent(QFocusEvent* e) override;
    /** @brief Pega solo texto; con Shift, cambiando `\` por `/` (`insertFromMimeData`). @param data Datos. */
    void insertFromMimeData(const QMimeData* data) override;
    /** @brief El menú propio: deshacer, rehacer, cortar, copiar, pegar, borrar, elegir todo. @param e Evento. */
    void contextMenuEvent(QContextMenuEvent* e) override;

private:
    void insertCompletion(const QString& completion);
    MyCompleter completer_;
    QStringListModel model_;
    bool undoFlag_{false};
    bool redoFlag_{false};
};

/** @brief `FCTextAreaExtended`: `QTextEdit` (texto con formato) con autocompletado. */
class FCTextAreaExtended : public FCTextAreaCommon<QTextEdit> {
    Q_OBJECT
public:
    /** @brief Crea el editor. @param parent Padre. */
    explicit FCTextAreaExtended(QWidget* parent = nullptr) : FCTextAreaCommon<QTextEdit>(parent) {}
};

/** @brief `FCPlainTextAreaExtended`: `QPlainTextEdit` con autocompletado. */
class FCPlainTextAreaExtended : public FCTextAreaCommon<QPlainTextEdit> {
    Q_OBJECT
public:
    /** @brief Crea el editor. @param parent Padre. */
    explicit FCPlainTextAreaExtended(QWidget* parent = nullptr) : FCTextAreaCommon<QPlainTextEdit>(parent) {}
};

/**
 * @brief `FCTextAreaLineNumber`: el editor de texto plano con la columna de números de línea (en negrita y azul la del
 * cursor) y la línea actual resaltada.
 */
class FCTextAreaLineNumber : public QFrame {
    Q_OBJECT
public:
    /** @brief El editor de adentro (`FCTextAreaLineNumber.PlainTextEdit`). */
    class PlainTextEdit : public FCPlainTextAreaExtended {
    public:
        /** @brief Crea el editor. @param parent Padre. */
        explicit PlainTextEdit(QWidget* parent = nullptr);
        /** @brief Resalta la línea del cursor (`highlight`). */
        void highlight();
        /** @brief Dibuja los números (`numberbarPaint`). @param bar La columna. @param e Evento. */
        void numberbarPaint(QWidget* bar, QPaintEvent* e);
    };

    /** @brief Crea el marco con la columna y el editor. @param parent Padre. */
    explicit FCTextAreaLineNumber(QWidget* parent = nullptr);
    /** @brief El editor (`edit`). @return El editor. */
    PlainTextEdit* edit() { return edit_; }

private:
    PlainTextEdit* edit_;
};

}  // namespace flatcam::gui
