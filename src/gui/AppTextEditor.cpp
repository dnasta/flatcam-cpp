#include "gui/AppTextEditor.h"

#include <QCheckBox>
#include <QFile>
#include <QFileDialog>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPageLayout>
#include <QPageSize>
#include <QPainter>
#include <QPdfWriter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QTextStream>
#include <QToolButton>
#include <QVBoxLayout>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"

namespace flatcam::gui {

namespace {

const char* kGcodeFilter =
    "G-Code Files (*.nc);; G-Code Files (*.txt);; G-Code Files (*.tap);; G-Code Files (*.cnc);; All Files (*.*)";

QToolButton* toolButton(const char* icon, const char* tip) {
    auto* b = new QToolButton();
    b->setIcon(QIcon(icon));
    b->setToolTip(tr_(tip));
    return b;
}

/** `FCFileSaveDialog.get_saved_filename`: agrega la extensión del filtro elegido si falta. */
QString savedFilename(QWidget* parent, const QString& caption, const QString& dir, const QString& filter) {
    QString chosen;
    QString f = QFileDialog::getSaveFileName(parent, caption, dir, filter, &chosen);
    if (f.isEmpty()) return f;
    QString stripped = chosen;
    while (stripped.endsWith(')')) stripped.chop(1);
    const QString ext = "." + stripped.section('.', -1);
    if (f.endsWith(ext) || ext == ".*") return f;
    return f + ext;
}

}  // namespace

AppTextEditor::AppTextEditor(App* app, bool plainText, QWidget* parent) : QWidget(parent), app_(app) {
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    auto* main = new QVBoxLayout(this);
    main->setContentsMargins(0, 0, 0, 0);
    t_frame = new QFrame();
    t_frame->setContentsMargins(0, 0, 0, 0);
    main->addWidget(t_frame);
    work_editor_layout = new QGridLayout(t_frame);
    work_editor_layout->setContentsMargins(2, 2, 2, 2);

    // El editor (con la misma hoja de estilo que el original: lo elegido en amarillo).
    if (plainText) {
        editor_class = new FCTextAreaLineNumber();
        plain_ = editor_class->edit();
        plain_->setStyleSheet("\n\t\t\t\t\t\t\tQPlainTextEdit { selection-background-color:yellow;\n"
                              "\t\t\t\t\t\t\t\t\t\t\t selection-color:black;\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t ");
        work_editor_layout->addWidget(editor_class, 0, 0, 1, 5);
    } else {
        rich_ = new FCTextAreaExtended();
        rich_->setStyleSheet("\n\t\t\t\t\t\t\tQTextEdit { selection-background-color:yellow;\n"
                             "\t\t\t\t\t\t\t\t\t\tselection-color:black;\n\t\t\t\t\t\t\t}\n\t\t\t\t\t\t ");
        work_editor_layout->addWidget(rich_, 0, 0, 1, 5);
    }

    auto* control = new QHBoxLayout();
    work_editor_layout->addLayout(control, 1, 0, 1, 5);
    buttonFind = new FCButton(nullptr, tr_("Find"));
    buttonFind->setIcon(QIcon(":/images/find32.png"));
    buttonFind->setToolTip(tr_("Will search and highlight in yellow the string in the Find box."));
    control->addWidget(buttonFind);
    entryFind = new FCEntry();
    entryFind->setToolTip(tr_("Find box. Enter here the strings to be searched in the text."));
    control->addWidget(entryFind);
    buttonReplace = new FCButton(nullptr, tr_("Replace With"));
    buttonReplace->setIcon(QIcon(":/images/replace32.png"));
    buttonReplace->setToolTip(tr_("Will replace the string from the Find box with the one in the Replace box."));
    control->addWidget(buttonReplace);
    entryReplace = new FCEntry();
    entryReplace->setToolTip(tr_("String to replace the one in the Find box throughout the text."));
    control->addWidget(entryReplace);
    sel_all_cb = new QCheckBox(tr_("All"));
    sel_all_cb->setToolTip(tr_("When checked it will replace all instances in the 'Find' box\n"
                               "with the text in the 'Replace' box.."));
    control->addWidget(sel_all_cb);
    button_update_code = toolButton(":/images/save_as.png", "Save changes internally.");
    button_update_code->hide();
    control->addWidget(button_update_code);
    buttonPreview = toolButton(":/images/preview32.png", "Open a OS standard Preview Print window.");
    control->addWidget(buttonPreview);
    buttonPrint = toolButton(":/images/printer32.png", "Open a OS standard Print window.");
    control->addWidget(buttonPrint);
    buttonOpen = toolButton(":/images/folder32_bis.png", "Will open a text file in the editor.");
    control->addWidget(buttonOpen);
    buttonSave = toolButton(":/images/save_as.png", "Will save the text in the editor into a file.");
    control->addWidget(buttonSave);
    buttonRun = new FCButton(nullptr, tr_("Run"));
    buttonRun->setToolTip(tr_("Will run the TCL commands found in the text file, one by one."));
    buttonRun->hide();
    control->addWidget(buttonRun);

    connectTextChanged();
    onOpen = [this] { handleOpen(); };
    onSave = [this] { handleSaveGCode(); };
    connect(buttonOpen, &QToolButton::clicked, this, [this] { onOpen(); });
    connect(buttonSave, &QToolButton::clicked, this, [this] { onSave(); });
    connect(buttonPrint, &QToolButton::clicked, this, [this] { handlePrint(); });
    connect(buttonPreview, &QToolButton::clicked, this, [this] { handlePreview(); });
    connect(buttonFind, &QPushButton::clicked, this, [this] { handleFindGCode(); });
    connect(buttonReplace, &QPushButton::clicked, this, [this] { handleReplaceGCode(); });
    connect(buttonRun, &QPushButton::clicked, this, [this] { Q_EMIT runRequested(); });
}

void AppTextEditor::connectTextChanged() {
    if (plain_) textChanged_ = connect(plain_, &QPlainTextEdit::textChanged, this, [this] { handleTextChanged(); });
    else textChanged_ = connect(rich_, &QTextEdit::textChanged, this, [this] { handleTextChanged(); });
}

QWidget* AppTextEditor::codeEditor() const {
    if (plain_) return plain_;
    return rich_;
}

QString AppTextEditor::toPlainText() const { return plain_ ? plain_->toPlainText() : rich_->toPlainText(); }

QString AppTextEditor::toHtml() const { return plain_ ? plain_->toPlainText() : rich_->toHtml(); }

QTextDocument* AppTextEditor::document() const { return plain_ ? plain_->document() : rich_->document(); }

QTextCursor AppTextEditor::textCursor() const { return plain_ ? plain_->textCursor() : rich_->textCursor(); }

void AppTextEditor::setTextCursor(const QTextCursor& c) {
    if (plain_) plain_->setTextCursor(c);
    else rich_->setTextCursor(c);
}

void AppTextEditor::moveCursor(QTextCursor::MoveOperation op, QTextCursor::MoveMode mode) {
    if (plain_) plain_->moveCursor(op, mode);
    else rich_->moveCursor(op, mode);
}

void AppTextEditor::setCompleterEnabled(bool on) {
    if (plain_) plain_->completerEnable = on;
    else rich_->completerEnable = on;
}

void AppTextEditor::setModelData(const QStringList& words) {
    if (plain_) plain_->setModelData(words);
    else rich_->setModelData(words);
}

void AppTextEditor::handleTextChanged() {
    buttonSave->setStyleSheet("QPushButton {color: red;}");
    buttonSave->setIcon(QIcon(":/images/save_as_red.png"));
}

void AppTextEditor::loadText(const QString& text, bool moveToStart, bool moveToEnd, bool clearText, bool asHtml) {
    // El original desconecta todo lo conectado a textChanged y vuelve a conectar solo handleTextChanged.
    disconnect(textChanged_);
    if (clearText) {
        if (plain_) plain_->clear();
        else rich_->clear();
    }
    if (plain_) {
        plain_->setReadOnly(false);
        if (asHtml) plain_->document()->setHtml(text);  // QPlainTextEdit no tiene setHtml: no pasa en el original
        else plain_->setPlainText(text);
    } else {
        rich_->setReadOnly(false);
        if (asHtml) rich_->setHtml(text);
        else rich_->setPlainText(text);
    }
    if (moveToStart) moveCursor(QTextCursor::Start);
    else if (moveToEnd) moveCursor(QTextCursor::End);
    connectTextChanged();
}

void AppTextEditor::handleFindGCode() {
    const QString what = entryFind->text();
    auto find = [&] {
        return plain_ ? plain_->find(what, QTextDocument::FindCaseSensitively)
                      : rich_->find(what, QTextDocument::FindCaseSensitively);
    };
    if (!find()) {
        moveCursor(QTextCursor::Start);
        find();
    }
}

void AppTextEditor::handleReplaceGCode() {
    const QString oldText = entryFind->text();
    const QString newText = entryReplace->text();
    if (sel_all_cb->isChecked()) {
        // Desde el cursor hasta el final (no vuelve a empezar); al terminar, el cursor va al principio.
        QTextCursor cursor = textCursor();
        cursor.beginEditBlock();
        while (true) {
            const bool r = plain_ ? plain_->find(oldText, QTextDocument::FindCaseSensitively)
                                  : rich_->find(oldText, QTextDocument::FindCaseSensitively);
            if (!r) {
                moveCursor(QTextCursor::Start);
                break;
            }
            QTextCursor qc = textCursor();
            if (qc.hasSelection()) qc.insertText(newText);
        }
        cursor.endEditBlock();
    } else {
        QTextCursor cursor = textCursor();
        cursor.beginEditBlock();
        QTextCursor qc = textCursor();
        if (qc.hasSelection()) qc.insertText(newText);
        cursor.endEditBlock();
    }
}

void AppTextEditor::handleOpen(const QString& filter, const QString& filename) {
    QString path = filename;
    if (path.isEmpty()) {
        path = QFileDialog::getOpenFileName(this, tr_("Open file"), app_ ? app_->lastFolder() : QString(),
                                            filter.isEmpty() ? QString(kGcodeFilter) : filter);
    }
    if (path.isEmpty()) return;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        const QString text = stream.readAll();
        if (plain_) plain_->setPlainText(text);
        else rich_->setPlainText(text);
    }
}

bool AppTextEditor::handleSaveGCode(const QString& name, const QString& filter, const QString& filename) {
    QString flt = filter.isEmpty() ? QString("G-Code Files (*.nc);; G-Code Files (*.txt);; G-Code Files (*.tap);; "
                                             "G-Code Files (*.cnc);; PDF Files (*.pdf);;All Files (*.*)")
                                   : filter;
    QString objName = name;
    if (objName.isEmpty()) {
        FlatCAMObj* active = app_ ? app_->activeObject() : nullptr;
        if (active) {
            objName = active->name();
        } else {
            objName = "file";
            if (filter.isEmpty()) flt = "FlatConfig Files (*.FlatConfig);;PDF Files (*.pdf);;All Files (*.*)";
        }
    }
    QString f = filename;
    if (f.isEmpty()) f = savedFilename(this, tr_("Export Code ..."), (app_ ? app_->lastFolder() : QString()) + "/" + objName, flt);
    auto inform = [this](const QString& m) {
        if (app_) app_->ui()->inform(m);
    };
    if (f.isEmpty()) {
        inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return false;
    }
    const QString text = toPlainText();
    if (f.section('.', -1).toLower() == "pdf") {
        const QString page = app_ ? defStr("global_workspaceT") : "A4";
        const auto size = App::pageSize(page).value_or(std::make_pair(210.0, 297.0));
        // Los márgenes están en las unidades de la aplicación (mm o pulgadas).
        const double k = defStr("units").toLower() == "mm" ? 1.0 : 25.4;
        const QMarginsF m(defNum("global_tpdf_lmargin") * k, defNum("global_tpdf_tmargin") * k,
                          defNum("global_tpdf_rmargin") * k, defNum("global_tpdf_bmargin") * k);
        if (!writeTextPdf(text, f, QSizeF(size.first, size.second), m)) {
            inform("[WARNING] " + tr_("No such file or directory"));
            return false;
        }
    } else {
        QFile file(f);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            inform("[WARNING] " + (file.exists() ? tr_("Permission denied, saving not possible.\n"
                                                       "Most likely another app is holding the file open and not "
                                                       "accessible.")
                                                 : tr_("No such file or directory")));
            return false;
        }
        file.write(text.toUtf8());
    }
    buttonSave->setStyleSheet("");
    buttonSave->setIcon(QIcon(":/images/save_as.png"));
    // Solo para los recientes (file_opened con global_open_style = False).
    if (app_ && !defBool("global_open_style")) app_->registerRecent("cncjob", f);
    inform(QString("%1: %2").arg(tr_("Saved to"), f));
    return true;
}

void AppTextEditor::handlePrint() {
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) document()->print(&printer);
}

void AppTextEditor::handlePreview() {
    QPrintPreviewDialog dialog(this);
    connect(&dialog, &QPrintPreviewDialog::paintRequested, this, [this](QPrinter* p) { document()->print(p); });
    dialog.exec();
}

bool writeTextPdf(const QString& text, const QString& filename, QSizeF pageMm, const QMarginsF& marginsMm) {
    QPdfWriter pdf(filename);
    pdf.setPageSize(QPageSize(pageMm, QPageSize::Millimeter));
    pdf.setPageMargins(marginsMm, QPageLayout::Millimeter);
    pdf.setResolution(72);  // un punto por unidad, como reportlab
    QPainter p;
    if (!p.begin(&pdf)) return false;
    QFont font("Helvetica");
    font.setPointSizeF(10);  // styleN de reportlab: Helvetica 10, interlineado 12
    p.setFont(font);
    const QRect area = pdf.pageLayout().paintRectPixels(72);
    const double leading = 12.0;
    double y = leading;
    // Paragraph junta los espacios seguidos y parte las líneas largas en palabras.
    const QFontMetricsF fm(font, &pdf);
    for (const QString& raw : text.split('\n')) {
        const QStringList words = raw.simplified().split(' ', Qt::SkipEmptyParts);
        QString line;
        auto flush = [&] {
            if (y > area.height()) {
                pdf.newPage();
                y = leading;
            }
            p.drawText(QPointF(0, y - 2.0), line);
            y += leading;
            line.clear();
        };
        for (const QString& w : words) {
            const QString candidate = line.isEmpty() ? w : line + " " + w;
            if (!line.isEmpty() && fm.horizontalAdvance(candidate) > area.width()) flush();
            line = line.isEmpty() ? w : line + " " + w;
        }
        flush();
    }
    return p.end();
}

}  // namespace flatcam::gui
