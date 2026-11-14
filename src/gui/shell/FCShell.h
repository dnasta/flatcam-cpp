/**
 * @file FCShell.h
 * @brief La consola Tcl (port de `appTools/ToolShell.py`: `TermWidget` y `FCShell`, y de `_BrowserTextEdit` /
 * `_ExpandableTextEdit` de `GUIElements`): el visor con los mensajes, la línea de comandos con historial y
 * autocompletado, y el intérprete Tcl real con los comandos de FlatCAM registrados.
 */
#pragma once

#include <QStringListModel>
#include <QTextEdit>
#include <QWidget>
#include <map>
#include <memory>
#include <optional>
#include <vector>

struct Tcl_Interp;

namespace flatcam::gui {

class App;
class FCLabel;
class MyCompleter;
class TclCommand;

/** @brief El visor de la consola (`_BrowserTextEdit`): copiar, elegir todo, guardar el registro y borrar. */
class ShellBrowser : public QTextEdit {
    Q_OBJECT
public:
    /** @brief Crea el visor. @param version Versión de FlatCAM. @param app La aplicación. */
    ShellBrowser(QString version, App* app);
    /** @brief `clear`: borra y vuelve a poner el encabezado. */
    void clearLog();

protected:
    /** @brief El menú: Copiar, Elegir todo, Guardar registro, Borrar todo. @param e Evento. */
    void contextMenuEvent(QContextMenuEvent* e) override;
    /** @brief Ctrl+A, Ctrl+C, Ctrl+S, Supr y S (mostrar u ocultar la consola). @param e Evento. */
    void keyPressEvent(QKeyEvent* e) override;

private:
    void copyText();
    QString version_;
    App* app_;
};

class FCShell;

/** @brief La línea de comandos (`_ExpandableTextEdit`): crece con el texto, historial y autocompletado. */
class ShellEdit : public QTextEdit {
    Q_OBJECT
public:
    /** @brief Crea la línea. @param shell La consola. */
    explicit ShellEdit(FCShell* shell);
    /** @brief `set_model_data`: las palabras del autocompletado. @param words Palabras. */
    void setModelData(const QStringList& words);
    /** @brief `sizeHint`: el alto del texto. @return El tamaño. */
    QSize sizeHint() const override;

Q_SIGNALS:
    /** @brief Flecha abajo en la última línea. */
    void historyNext();
    /** @brief Flecha arriba en la primera línea. */
    void historyPrev();

protected:
    /** @brief Enter ejecuta; arriba/abajo, el historial; el autocompletado. @param e Evento. */
    void keyPressEvent(QKeyEvent* e) override;
    /** @brief El completador sigue a esta línea. @param e Evento. */
    void focusInEvent(QFocusEvent* e) override;
    /** @brief Solo se pega texto plano. @param source Datos. */
    void insertFromMimeData(const QMimeData* source) override;

private:
    void fitToDocument();
    void insertCompletion(const QString& completion);
    FCShell* shell_;
    int fittedHeight_{1};
    MyCompleter* completer_;
    QStringListModel* model_;
};

/**
 * @brief La consola Tcl (`FCShell`).
 *
 * Como el original: un intérprete Tcl (`tkinter.Tcl()`) con los comandos de FlatCAM y `puts` redefinido para que
 * devuelva el texto; lo que se escribe se evalúa y el resultado (salvo "None") se agrega al visor; un error muestra
 * "ERROR Report: " y el `errorInfo` de Tcl.
 */
class FCShell : public QWidget {
    Q_OBJECT
public:
    /** @brief Crea la consola y registra los comandos. @param app La aplicación. @param version Versión. */
    FCShell(App* app, const QString& version);
    ~FCShell() override;
    /** @brief El intérprete. @return El intérprete. */
    Tcl_Interp* interp() const { return interp_; }
    /** @brief El visor (`_browser`). @return El visor. */
    ShellBrowser* browser() const { return browser_; }
    /** @brief La línea de comandos (`_edit`). @return La línea. */
    ShellEdit* commandLine() const { return edit_; }
    /** @brief Los comandos por alias (`tcl_commands_storage`). @return Los comandos. */
    const std::map<QString, TclCommand*>& commands() const { return byAlias_; }

    /** @brief `exec_current_command`: el texto de la línea al historial y al visor, y se ejecuta. */
    void execCurrentCommand();
    /**
     * @brief `exec_command`: evalúa el texto en Tcl y, salvo con @p noEcho, muestra el resultado o el error.
     * @param text Texto. @param noEcho Sin escribir en el visor (los scripts).
     * @return El resultado (o el `errorInfo` si falló).
     */
    QString execCommand(const QString& text, bool noEcho = false);
    /**
     * @brief Evalúa en Tcl (`tcl.eval`). @param text Texto. @param[out] ok Si no hubo error.
     * @return El resultado de Tcl.
     */
    QString eval(const QString& text, bool* ok = nullptr);

    /** @brief `append_output`. @param text Texto. */
    void appendOutput(const QString& text) { append("out", text); }
    /** @brief `append_raw` (HTML tal cual). @param text Texto. */
    void appendRaw(const QString& text) { append("raw", text); }
    /** @brief `append_success`. @param text Texto. */
    void appendSuccess(const QString& text) { append("success", text); }
    /** @brief `append_selected` (no muestra nada). @param text Texto. */
    void appendSelected(const QString& text) { append("selected", text); }
    /** @brief `append_warning`. @param text Texto. */
    void appendWarning(const QString& text) { append("warning", text); }
    /** @brief `append_error`. @param text Texto. */
    void appendError(const QString& text) { append("err", text); }
    /** @brief `open_processing`: la línea se deshabilita y muestra "...procesando...". @param detail Detalle. */
    void openProcessing(const QString& detail = {});
    /** @brief `close_processing`. */
    void closeProcessing();

private:
    void append(const QString& style, QString text);
    void onHistoryNext();
    void onHistoryPrev();
    App* app_;
    ShellBrowser* browser_;
    ShellEdit* edit_;
    FCLabel* deleteLine_;
    QStringList history_{QString()};
    int historyIndex_{0};
    Tcl_Interp* interp_{nullptr};
    std::vector<std::unique_ptr<TclCommand>> commands_;
    std::map<QString, TclCommand*> byAlias_;
};

}  // namespace flatcam::gui
