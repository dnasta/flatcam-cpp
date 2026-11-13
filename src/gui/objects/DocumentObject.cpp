#include "gui/objects/DocumentObject.h"

#include <QColorDialog>
#include <QFontComboBox>
#include <QTextDocument>

#include "gui/App.h"
#include "gui/AppTextEditor.h"
#include "gui/MainGUI.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"

namespace flatcam::gui {

DocumentObjectUI::DocumentObjectUI(QWidget* parent) : DocumentObjectUIBase(parent) {
    setupUi();
    // La fuente de cada sistema (el volcado trae la del Mac donde se generó).
#if defined(Q_OS_WIN)
    const QFont f("Arial");
#elif defined(Q_OS_LINUX)
    const QFont f("FreeMono");
#else
    const QFont f("Helvetica Neue");
#endif
    font_type_cb->setCurrentFont(f);
}

DocumentObject::DocumentObject(QString name, QString text, bool readOnly)
    : TextObject(ObjectKind::Document, std::move(name), std::move(text)), readOnly_(readOnly) {}

void DocumentObject::setUi(App* app) {
    app_ = app;
    auto* panel = new DocumentObjectUI();
    ui_.reset(panel);
    panel->name_entry->setText(name());
    if (defStr("global_app_level") == "b")
        panel->level->setText(QString("<span style=\"color:green;\"><b>%1</b></span>").arg(tr_("Basic")));
    else
        panel->level->setText(QString("<span style=\"color:red;\"><b>%1</b></span>").arg(tr_("Advanced")));

    auto* ed = new AppTextEditor(app, false);
    editor_ = ed;
    ed->setModelData(app->keywords());
    FCTextAreaExtended* code = ed->richEditor();
    code->setStyleSheet(QString("\n\t\t\t\t\t\tQTextEdit {selection-background-color:%1;\n\t\t\t\t\t\t\t\t   "
                                "selection-color:white;\n\t\t\t\t\t\t}\n\t\t\t\t\t ")
                            .arg(defStr("document_sel_color")));
    ed->buttonRun->hide();
    panel->autocomplete_cb->setValue(defBool("document_autocompleter"));
    ed->setCompleterEnabled(defBool("document_autocompleter"));
    onTabSizeChange(defInt("document_tab_size"));

    const QString flt = "FlatCAM Docs (*.FlatDoc);;All Files (*.*)";
    ed->onOpen = [ed, flt] { ed->handleOpen(flt); };
    ed->onSave = [ed, flt] { ed->handleSaveGCode({}, flt); };
    // on_text_changed (source_file = toHtml()) se conecta acá en el original, pero el load_text del final desconecta
    // todo lo conectado a textChanged: lo que se escribe nunca llega a source_file. Se reproduce no conectándolo.

    // font_family, font_size, negrita, cursiva, subrayado
    connect(panel->font_type_cb, &QFontComboBox::currentFontChanged, this, [this, panel](QFont font) {
        font.setPointSizeF(panel->font_size_cb->currentText().toDouble());
        editor_->richEditor()->setCurrentFont(font);
    });
    connect(panel->font_size_cb, qOverload<int>(&QComboBox::activated), this, [this, panel] {
        editor_->richEditor()->setFontPointSize(panel->font_size_cb->currentText().toDouble());
    });
    connect(panel->font_bold_tb, &QToolButton::clicked, this, [this, panel] {
        editor_->richEditor()->setFontWeight(panel->font_bold_tb->isChecked() ? QFont::Bold : QFont::Normal);
    });
    connect(panel->font_italic_tb, &QToolButton::clicked, this,
            [this, panel] { editor_->richEditor()->setFontItalic(panel->font_italic_tb->isChecked()); });
    connect(panel->font_under_tb, &QToolButton::clicked, this,
            [this, panel] { editor_->richEditor()->setFontUnderline(panel->font_under_tb->isChecked()); });

    // Colores de la letra y de lo elegido (cambian las preferencias, como el original).
    auto setPref = [app](const char* key, const QString& v) {
        if (app->preferences()) app->preferences()->set(key, JsonValue::string(v.toStdString()));
    };
    connect(panel->font_color_entry, &QLineEdit::editingFinished, this, [panel, setPref] {
        setPref("document_font_color", panel->font_color_entry->text());
        panel->font_color_button->setStyleSheet("background-color:" + defStr("document_font_color"));
    });
    connect(panel->font_color_button, &QPushButton::clicked, this, [this, panel, setPref] {
        const QColor c = QColorDialog::getColor(QColor(defStr("document_font_color")));
        if (!c.isValid()) return;
        editor_->richEditor()->setTextColor(c);
        panel->font_color_button->setStyleSheet("background-color:" + c.name());
        panel->font_color_entry->setText(c.name());
        setPref("document_font_color", c.name());
    });
    connect(panel->sel_color_entry, &QLineEdit::editingFinished, this, [panel, setPref] {
        setPref("document_sel_color", panel->sel_color_entry->text());
        panel->sel_color_button->setStyleSheet("background-color:" + defStr("document_sel_color"));
    });
    connect(panel->sel_color_button, &QPushButton::clicked, this, [this, panel, setPref] {
        const QColor c = QColorDialog::getColor(QColor(defStr("document_sel_color")));
        if (!c.isValid()) return;
        QPalette p;
        p.setColor(QPalette::Highlight, c);
        p.setColor(QPalette::HighlightedText, QColor("white"));
        editor_->richEditor()->setPalette(p);
        panel->sel_color_button->setStyleSheet("background-color:" + c.name());
        panel->sel_color_entry->setText(c.name());
        setPref("document_sel_color", c.name());
    });

    // Alineación
    auto align = [this](Qt::Alignment a) { editor_->richEditor()->setAlignment(a); };
    connect(panel->al_left_tb, &QToolButton::clicked, this, [align] { align(Qt::AlignLeft); });
    connect(panel->al_center_tb, &QToolButton::clicked, this, [align] { align(Qt::AlignCenter); });
    connect(panel->al_right_tb, &QToolButton::clicked, this, [align] { align(Qt::AlignRight); });
    connect(panel->al_justify_tb, &QToolButton::clicked, this, [align] { align(Qt::AlignJustify); });

    connect(panel->autocomplete_cb, &QCheckBox::checkStateChanged, this,
            [this](Qt::CheckState s) { if (editor_) editor_->setCompleterEnabled(s != Qt::Unchecked); });
    connect(panel->tab_size_spinner, &FCSpinner::returnPressed, this, [this] { onTabSizeChange(std::nullopt); });

    panel->font_color_entry->setText(defStr("document_font_color"));
    panel->font_color_button->setStyleSheet("background-color:" + defStr("document_font_color"));
    panel->sel_color_entry->setText(defStr("document_sel_color"));
    panel->sel_color_button->setStyleSheet("background-color:" + defStr("document_sel_color"));
    panel->font_size_cb->setCurrentIndex(defInt("document_font_size"));

    // Texto con formato si lo parece (Qt.mightBeRichText), si no, texto plano; el cursor al principio.
    ed->loadText(sourceFile(), true, false, true, Qt::mightBeRichText(sourceFile()));
    buildUi();
}

void DocumentObject::onTabSizeChange(std::optional<int> val) {
    auto* panel = ui();
    if (val) panel->tab_size_spinner->setValue(*val);
    const int v = panel->tab_size_spinner->value();
    editor_->richEditor()->setTabStopDistance(v);
    if (app_->preferences() && defInt("document_tab_size") != v)
        app_->preferences()->set("document_tab_size", JsonValue::integer(v));
}

void DocumentObject::buildUi() {
    addTabIfMissing(tr_("Document Editor"));
    app_->ui()->plot_tab_area->setCurrentWidget(editor_);
}

}  // namespace flatcam::gui
