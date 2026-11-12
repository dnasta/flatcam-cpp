// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/GeoTextToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void GeoTextToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Herramienta de texto"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(11, 11, 11, 11);
    layout->setSpacing(6);
    text_tool_frame = new QFrame(nullptr);
    text_tools_box = new QVBoxLayout();
    text_tools_box->setContentsMargins(0, 0, 0, 0);
    text_tools_box->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t}\n\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Herramienta de texto"));
    text_tools_box->addWidget(w1, 0);
    form_layout = new QFormLayout();
    form_layout->setContentsMargins(0, 0, 0, 0);
    form_layout->setHorizontalSpacing(6);
    form_layout->setVerticalSpacing(6);
    auto* w2 = new FCLabel(nullptr);
    w2->setText(QString::fromUtf8("Font:"));
    form_layout->setWidget(0, QFormLayout::ItemRole(0), w2);
    font_type_cb = new QFontComboBox(nullptr);
    // propiedad currentFont (QFont) sin traducir
    font_type_cb->setProperty("currentText", QString::fromUtf8("Helvetica Neue"));
    form_layout->setWidget(0, QFormLayout::ItemRole(1), font_type_cb);
    auto* w3 = new FCLabel(nullptr);
    w3->setText(QString::fromUtf8("Tamaño:"));
    form_layout->setWidget(1, QFormLayout::ItemRole(0), w3);
    auto* lay4 = new QHBoxLayout();
    lay4->setContentsMargins(0, 0, 0, 0);
    lay4->setSpacing(6);
    font_size_cb = new FCComboBox(nullptr);
    font_size_cb->setProperty("currentText", QString::fromUtf8("10"));
    font_size_cb->setEditable(true);
    font_size_cb->setProperty("focusPolicy", 15);
    font_size_cb->setMaximumSize(QSize(70, 16777215));
    font_size_cb->setProperty("minimumContentsLength", 3);
    font_size_cb->addItem(QString::fromUtf8("6"));
    font_size_cb->addItem(QString::fromUtf8("7"));
    font_size_cb->addItem(QString::fromUtf8("8"));
    font_size_cb->addItem(QString::fromUtf8("9"));
    font_size_cb->addItem(QString::fromUtf8("10"));
    font_size_cb->addItem(QString::fromUtf8("11"));
    font_size_cb->addItem(QString::fromUtf8("12"));
    font_size_cb->addItem(QString::fromUtf8("13"));
    font_size_cb->addItem(QString::fromUtf8("14"));
    font_size_cb->addItem(QString::fromUtf8("15"));
    font_size_cb->addItem(QString::fromUtf8("16"));
    font_size_cb->addItem(QString::fromUtf8("18"));
    font_size_cb->addItem(QString::fromUtf8("20"));
    font_size_cb->addItem(QString::fromUtf8("22"));
    font_size_cb->addItem(QString::fromUtf8("24"));
    font_size_cb->addItem(QString::fromUtf8("26"));
    font_size_cb->addItem(QString::fromUtf8("28"));
    font_size_cb->addItem(QString::fromUtf8("32"));
    font_size_cb->addItem(QString::fromUtf8("36"));
    font_size_cb->addItem(QString::fromUtf8("40"));
    font_size_cb->addItem(QString::fromUtf8("44"));
    font_size_cb->addItem(QString::fromUtf8("48"));
    font_size_cb->addItem(QString::fromUtf8("54"));
    font_size_cb->addItem(QString::fromUtf8("60"));
    font_size_cb->addItem(QString::fromUtf8("66"));
    font_size_cb->addItem(QString::fromUtf8("72"));
    font_size_cb->addItem(QString::fromUtf8("80"));
    font_size_cb->addItem(QString::fromUtf8("88"));
    font_size_cb->addItem(QString::fromUtf8("96"));
    font_size_cb->setCurrentIndex(4);
    lay4->addWidget(font_size_cb, 0);
    lay4->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    font_bold_tb = new QToolButton(nullptr);
    font_bold_tb->setCheckable(true);
    font_bold_tb->setIcon(QIcon(QString::fromUtf8(":/images/bold32.png")));
    lay4->addWidget(font_bold_tb, 0);
    font_italic_tb = new QToolButton(nullptr);
    font_italic_tb->setCheckable(true);
    font_italic_tb->setIcon(QIcon(QString::fromUtf8(":/images/italic32.png")));
    lay4->addWidget(font_italic_tb, 0);
    form_layout->setLayout(1, QFormLayout::ItemRole(1), lay4);
    auto* w5 = new FCLabel(nullptr);
    w5->setText(QString::fromUtf8("Texto:"));
    form_layout->setWidget(2, QFormLayout::ItemRole(0), w5);
    text_input_entry = new QTextEdit(nullptr);
    text_input_entry->setMinimumSize(QSize(0, 200));
    text_input_entry->setProperty("tabStopDistance", 12.0);
    text_input_entry->setProperty("tabStopWidth", 12);
    form_layout->setWidget(2, QFormLayout::ItemRole(1), text_input_entry);
    auto* lay6 = new QHBoxLayout();
    lay6->setContentsMargins(0, 0, 0, 0);
    lay6->setSpacing(6);
    lay6->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    apply_button = new FCButton(nullptr);
    apply_button->setText(QString::fromUtf8("Aplicar"));
    lay6->addWidget(apply_button, 0);
    form_layout->setLayout(3, QFormLayout::ItemRole(1), lay6);
    text_tools_box->addLayout(form_layout, 0);
    text_tool_frame->setLayout(text_tools_box);
    layout->addWidget(text_tool_frame, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> GeoTextToolUIBase::membersByName() const {
    return {
        {"apply_button", apply_button},
        {"font_bold_tb", font_bold_tb},
        {"font_italic_tb", font_italic_tb},
        {"font_size_cb", font_size_cb},
        {"font_type_cb", font_type_cb},
        {"form_layout", form_layout},
        {"layout", layout},
        {"text_input_entry", text_input_entry},
        {"text_tool_frame", text_tool_frame},
        {"text_tools_box", text_tools_box},
    };
}

}  // namespace flatcam::gui
