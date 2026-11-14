#include "gui/shell/FCShell.h"

#include <tcl.h>

#include <QAbstractItemView>
#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMenu>
#include <QMimeData>
#include <QScrollBar>
#include <QTextBlock>
#include <QVBoxLayout>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/shell/TclCommand.h"
#include "gui/widgets/FCLabel.h"
#include "gui/widgets/FCTextEdits.h"

namespace flatcam::gui {

namespace {

/** `html.escape(text)`. */
QString htmlEscape(const QString& s) {
    QString out;
    for (const QChar c : s) {
        if (c == '&') out += "&amp;";
        else if (c == '<') out += "&lt;";
        else if (c == '>') out += "&gt;";
        else if (c == '"') out += "&quot;";
        else if (c == '\'') out += "&#x27;";
        else out += c;
    }
    return out;
}

/** El comando de Tcl que llama a un comando de FlatCAM (`execute_wrapper`; tkinter pasa los argumentos como texto). */
int dispatch(ClientData data, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[]) {
    auto* cmd = static_cast<TclCommand*>(data);
    QStringList args;
    for (int i = 1; i < objc; ++i) args << QString::fromUtf8(Tcl_GetString(objv[i]));
    try {
        const std::optional<QString> r = cmd->run(args);
        // Un None de Python llega a Tcl como el texto "None" (y la consola no lo muestra).
        const QByteArray b = r.value_or("None").toUtf8();
        Tcl_SetObjResult(interp, Tcl_NewStringObj(b.constData(), static_cast<int>(b.size())));
        return TCL_OK;
    } catch (const TclError& e) {
        // display_tcl_error + volver a lanzar: tkinter deja como resultado el texto de la excepción.
        const QByteArray b = e.text().toUtf8();
        Tcl_SetObjResult(interp, Tcl_NewStringObj(b.constData(), static_cast<int>(b.size())));
        return TCL_ERROR;
    } catch (const std::exception& e) {
        Tcl_SetObjResult(interp, Tcl_NewStringObj(e.what(), -1));
        return TCL_ERROR;
    }
}

}  // namespace

// ---------------------------------------------------------------------------------------------------------------

ShellBrowser::ShellBrowser(QString version, App* app) : version_(std::move(version)), app_(app) {}

void ShellBrowser::copyText() {
    QApplication::clipboard()->clear();
    QApplication::clipboard()->setText(textCursor().selectedText());
}

void ShellBrowser::clearLog() {
    QTextEdit::clear();
    // "!FlatCAM <versión>? - ..." con el '!' y el '?' cambiados por <b> y </b> después de escapar.
    QString text = htmlEscape(QString("!FlatCAM %1? - %2").arg(version_, tr_("Type >help< to get started")));
    text.replace('!', "<b>").replace('?', "</b>");
    text += "<br><br>";
    moveCursor(QTextCursor::End);
    insertHtml(text);
}

void ShellBrowser::contextMenuEvent(QContextMenuEvent* e) {
    QMenu menu;
    QAction* copy = menu.addAction(QString("%1\t%2").arg(tr_("Copy"), tr_("Ctrl+C")));
    connect(copy, &QAction::triggered, this, [this] { copyText(); });
    if (textCursor().selectedText().isEmpty()) copy->setDisabled(true);
    menu.addSeparator();
    QAction* all = menu.addAction(QString("%1\t%2").arg(tr_("Select All"), tr_("Ctrl+A")));
    connect(all, &QAction::triggered, this, &QTextEdit::selectAll);
    if (app_) {
        QAction* save = menu.addAction(QString("%1\t%2").arg(tr_("Save Log"), tr_("Ctrl+S")));
        connect(save, &QAction::triggered, this, [this] { app_->saveToFile(toHtml(), toPlainText()); });
    }
    QAction* clear = menu.addAction(QString("%1\t%2").arg(tr_("Clear All"), tr_("Del")));
    connect(clear, &QAction::triggered, this, [this] { clearLog(); });
    menu.exec(e->globalPos());
}

void ShellBrowser::keyPressEvent(QKeyEvent* e) {
    const Qt::KeyboardModifiers mods = QApplication::keyboardModifiers();
    const int key = e->key();
    if (mods == Qt::ControlModifier) {
        if (key == Qt::Key_A) selectAll();
        else if (key == Qt::Key_C) copyText();
        else if (key == Qt::Key_S && app_) app_->saveToFile(toHtml(), toPlainText());
    } else if (mods == Qt::NoModifier) {
        if (key == Qt::Key_Delete) clearLog();
        if (key == Qt::Key_S && app_) app_->toggleShell();
    }
}

// ---------------------------------------------------------------------------------------------------------------

ShellEdit::ShellEdit(FCShell* shell) : shell_(shell), completer_(new MyCompleter(this)), model_(new QStringListModel(this)) {
    setStyleSheet("font: 9pt \"Courier\";");
    connect(this, &QTextEdit::textChanged, this, [this] { fitToDocument(); });
    fitToDocument();
    completer_->setModel(model_);
    connect(completer_, &MyCompleter::insertText, this, [this](const QString& t) { insertCompletion(t); });
    connect(completer_->popup(), &QAbstractItemView::clicked, this, [this] {
        Q_EMIT completer_->insertText(completer_->selected());
        completer_->setCompletionMode(QCompleter::PopupCompletion);
    });
}

void ShellEdit::setModelData(const QStringList& words) { model_->setStringList(words); }

void ShellEdit::insertCompletion(const QString& completion) {
    QTextCursor tc = textCursor();
    const int extra = static_cast<int>(completion.size() - completer_->completionPrefix().size());
    // Si la palabra ya está completa, solo un espacio.
    if (extra == 0) {
        tc.insertText(" ");
        completer_->popup()->hide();
        return;
    }
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    tc.insertText(" ");
    setTextCursor(tc);
    completer_->popup()->hide();
}

void ShellEdit::focusInEvent(QFocusEvent* e) {
    completer_->setWidget(this);
    QTextEdit::focusInEvent(e);
}

void ShellEdit::insertFromMimeData(const QMimeData* source) { insertPlainText(source->text()); }

void ShellEdit::keyPressEvent(QKeyEvent* e) {
    const int key = e->key();
    if ((key == Qt::Key_Tab || key == Qt::Key_Return || key == Qt::Key_Enter) && completer_->popup()->isVisible()) {
        Q_EMIT completer_->insertText(completer_->selected());
        completer_->setCompletionMode(QCompleter::PopupCompletion);
        return;
    }
    if (e->matches(QKeySequence::InsertParagraphSeparator)) {
        shell_->execCurrentCommand();  // is_command_complete siempre es True
        return;
    }
    if (e->matches(QKeySequence::MoveToNextLine)) {
        const QString after = toPlainText().mid(textCursor().position());
        if (after.split('\n').size() <= 1) {
            Q_EMIT historyNext();
            return;
        }
    } else if (e->matches(QKeySequence::MoveToPreviousLine)) {
        const QString before = toPlainText().left(textCursor().position());
        int lines = static_cast<int>(before.split('\n').size());
        if (!before.isEmpty() && (before.back() == '\n' || before.back() == '\r')) ++lines;
        if (lines <= 1) {
            Q_EMIT historyPrev();
            return;
        }
    } else if (e->matches(QKeySequence::MoveToNextPage) || e->matches(QKeySequence::MoveToPreviousPage)) {
        QApplication::sendEvent(shell_->browser(), e);
        return;
    }
    QTextCursor tc = textCursor();
    QTextEdit::keyPressEvent(e);
    tc.select(QTextCursor::WordUnderCursor);
    QRect cr = cursorRect();
    if (!tc.selectedText().isEmpty()) {
        completer_->setCompletionPrefix(tc.selectedText());
        completer_->popup()->setCurrentIndex(completer_->completionModel()->index(0, 0));
        cr.setWidth(completer_->popup()->sizeHintForColumn(0) + completer_->popup()->verticalScrollBar()->sizeHint().width());
        completer_->complete(cr);
    } else {
        completer_->popup()->hide();
    }
}

QSize ShellEdit::sizeHint() const {
    QSize h = QTextEdit::sizeHint();
    h.setHeight(fittedHeight_);
    return h;
}

void ShellEdit::fitToDocument() {
    const QSize doc = document()->size().toSize();
    fittedHeight_ = doc.height() + (height() - viewport()->height());
    setMaximumHeight(std::max(0, fittedHeight_));
    updateGeometry();
}

// ---------------------------------------------------------------------------------------------------------------

FCShell::FCShell(App* app, const QString& version) : app_(app) {
    // TermWidget
    browser_ = new ShellBrowser(version, app);
    browser_->setStyleSheet("font: 9pt \"Courier\";");
    browser_->setReadOnly(true);
    browser_->document()->setDefaultStyleSheet(browser_->document()->defaultStyleSheet() + "span {white-space:pre;}");
    edit_ = new ShellEdit(this);
    connect(edit_, &ShellEdit::historyNext, this, [this] { onHistoryNext(); });
    connect(edit_, &ShellEdit::historyPrev, this, [this] { onHistoryPrev(); });
    edit_->setFocus();
    setFocusProxy(edit_);
    deleteLine_ = new FCLabel();
    deleteLine_->setPixmap(QPixmap(":/images/clear_line16.png"));
    deleteLine_->setMargin(3);
    deleteLine_->setToolTip(tr_("Clear the text."));
    auto* layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(browser_);
    auto* hlay = new QHBoxLayout();
    hlay->addWidget(deleteLine_);
    hlay->addWidget(new QLabel(" "));
    hlay->addWidget(edit_);
    layout->addLayout(hlay);
    connect(deleteLine_, &FCLabel::clicked, edit_, [this] { edit_->clear(); });

    // FCShell: init_tcl + setup_shell.
    interp_ = Tcl_CreateInterp();
    Tcl_Init(interp_);  // la biblioteca de scripts de Tcl (init.tcl), como tkinter.Tcl()
    commands_ = createTclCommands(app);
    for (const auto& c : commands_)
        for (const QString& alias : c->meta().aliases) {
            byAlias_[alias] = c.get();
            Tcl_CreateObjCommand(interp_, alias.toUtf8().constData(), dispatch, c.get(), nullptr);
        }
    // puts devuelve el texto en vez de escribirlo (con un solo argumento).
    eval("\n            rename puts original_puts\n            proc puts {args} {\n                if {[llength $args] == 1} {\n"
         "                    return \"[lindex $args 0]\"\n                } else {\n"
         "                    eval original_puts $args\n                }\n            }\n            ");
    edit_->setModelData(app->keywords());
    setWindowIcon(QIcon(":/images/flatcam_icon32.png"));
    setWindowTitle(tr_("FlatCAM Shell"));
    append("in", QString("FlatCAM %1 - ").arg(version));
    appendOutput(QString("%1\n\n").arg(tr_("Type >help< to get started")));
}

FCShell::~FCShell() {
    if (interp_) Tcl_DeleteInterp(interp_);
}

QString FCShell::eval(const QString& text, bool* ok) {
    const QByteArray b = text.toUtf8();
    const int code = Tcl_EvalEx(interp_, b.constData(), static_cast<int>(b.size()), TCL_EVAL_GLOBAL);
    if (ok) *ok = code != TCL_ERROR;
    return QString::fromUtf8(Tcl_GetStringResult(interp_));
}

QString FCShell::execCommand(const QString& text, bool noEcho) {
    // exec_command_test con reraise = False.
    if (!noEcho) openProcessing();
    bool ok = true;
    QString result = eval(text, &ok);
    if (ok) {
        if (result != "None" && !noEcho) appendOutput(result + "\n");
    } else {
        result = eval("set errorInfo");
        if (!noEcho) appendError("ERROR Report: " + result + "\n");
    }
    if (!noEcho) closeProcessing();
    return result;
}

void FCShell::execCurrentCommand() {
    QString text = edit_->toPlainText();
#ifdef Q_OS_WIN
    text.replace("\\", "\\\\");  // las rutas de Windows: la barra invertida es el escape de Tcl
#endif
    append("in", "> " + text + "\n");
    if (history_.size() < 2 || history_[history_.size() - 2] != text) {
        if (text.isEmpty()) return;  // text[-1]: IndexError
        history_.insert(history_.size() - 1, text.endsWith('\n') ? text.chopped(1) : text);
    }
    historyIndex_ = static_cast<int>(history_.size()) - 1;
    history_.last() = QString();
    edit_->clear();
    if (text.isEmpty()) return;  // `text[-1]` vuelve a fallar
    if (!text.endsWith('\n')) text += '\n';
    execCommand(text);
}

void FCShell::openProcessing(const QString& detail) {
    edit_->setTextColor(Qt::white);
    edit_->setTextBackgroundColor(Qt::darkGreen);
    edit_->setPlainText(detail.isEmpty() ? tr_("...processing...")
                                         : QString("%1 [%2]").arg(tr_("...processing..."), detail));
    edit_->setDisabled(true);
    edit_->setFocus();
}

void FCShell::closeProcessing() {
    edit_->setTextColor(Qt::black);
    edit_->setTextBackgroundColor(Qt::white);
    edit_->setPlainText("");
    edit_->setDisabled(false);
    edit_->setFocus();
}

void FCShell::append(const QString& style, QString text) {
    // _append_to_browser
    if (style != "raw") {
        text = htmlEscape(text);
        text.replace("\n", "<br/>");
    } else {
        text.replace("\n", "<br>");
        text.replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
    }
    const int idx = static_cast<int>(text.indexOf(']'));
    QString mtype = text.left(idx + 1).toUpper();
    mtype.replace("_NOTCL", "");
    const QString body = text.mid(idx + 1);
    if (style == "in") text = QString("<span style=\"font-weight: bold;\">%1</span>").arg(text);
    else if (style == "err")
        text = QString("<span style=\"font-weight: bold; color: red;\">%1</span><span style=\"font-weight: bold;\">%2</span>")
                   .arg(mtype, body);
    else if (style == "warning")
        text = QString("<span style=\"font-weight: bold; color: #f4b642;\">%1</span><span style=\"font-weight: bold;\">%2"
                       "</span>").arg(mtype, body);
    else if (style == "success")
        text = QString("<span style=\"font-weight: bold; color: #15b300;\">%1</span><span style=\"font-weight: bold;\">%2"
                       "</span>").arg(mtype, body);
    else if (style == "selected") text = QString();
    else if (style != "raw") text = QString("<span>%1</span>").arg(text);
    browser_->moveCursor(QTextCursor::End);
    browser_->insertHtml(text);
    browser_->verticalScrollBar()->setValue(browser_->verticalScrollBar()->maximum());
}

void FCShell::onHistoryNext() {
    if (historyIndex_ + 1 < history_.size()) {
        ++historyIndex_;
        edit_->setPlainText(history_[historyIndex_]);
        edit_->moveCursor(QTextCursor::End);
    }
}

void FCShell::onHistoryPrev() {
    if (historyIndex_ > 0) {
        if (historyIndex_ == history_.size() - 1) history_.last() = edit_->toPlainText();
        --historyIndex_;
        edit_->setPlainText(history_[historyIndex_]);
        edit_->moveCursor(QTextCursor::End);
    }
}

}  // namespace flatcam::gui
