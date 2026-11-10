// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/InvertToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void InvertToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Herram. Invertir Gerber"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Herram. Invertir Gerber"));
    layout->addWidget(w1, 0);
    auto* w2 = new FCLabel(nullptr);
    layout->addWidget(w2, 0);
    tools_frame = new QFrame(nullptr);
    tools_box = new QVBoxLayout();
    tools_box->setContentsMargins(0, 0, 0, 0);
    tools_box->setSpacing(6);
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    gerber_label = new FCLabel(nullptr);
    gerber_label->setText(QString::fromUtf8("<b>GERBER:</b>"));
    gerber_label->setToolTip(QString::fromUtf8("Objeto de Gerber que se invertirá."));
    lay3->addWidget(gerber_label, 1, 0, 1, 2);
    gerber_combo = new FCComboBox(nullptr);
    lay3->addWidget(gerber_combo, 2, 0, 1, 2);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay3->addWidget(w4, 3, 0, 1, 2);
    param_label = new FCLabel(nullptr);
    param_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    param_label->setToolTip(QString::fromUtf8("Parám. para esta herramienta."));
    lay3->addWidget(param_label, 4, 0, 1, 2);
    margin_label = new FCLabel(nullptr);
    margin_label->setText(QString::fromUtf8("Margen:"));
    margin_label->setToolTip(QString::fromUtf8("Distancia por la cual evitar\nLos bordes del objeto Gerber."));
    lay3->addWidget(margin_label, 5, 0, 1, 2);
    margin_entry = new FCDoubleSpinner(nullptr);
    margin_entry->setProperty("decimals", 4);
    margin_entry->setProperty("maximum", 10000.0);
    margin_entry->setProperty("objectName", QString::fromUtf8("Margen"));
    margin_entry->setProperty("value", 0.1);
    lay3->addWidget(margin_entry, 6, 0, 1, 2);
    join_label = new FCLabel(nullptr);
    join_label->setText(QString::fromUtf8("Estilo de unión de líneas:"));
    join_label->setToolTip(QString::fromUtf8("La forma en que se unirán las líneas en el contorno del objeto.\nPuede ser:\n- redondeado -> se agrega un arco entre dos líneas de unión\n- cuadrado -> las líneas se encuentran en un ángulo de 90 grados\n- bisel -> las líneas están unidas por una tercera línea"));
    lay3->addWidget(join_label, 7, 0, 1, 2);
    join_radio = new RadioSet({{QString::fromUtf8("Redondeado"), QVariant(QString::fromUtf8("r"))}, {QString::fromUtf8("Cuadrado"), QVariant(QString::fromUtf8("s"))}, {QString::fromUtf8("Bisel"), QVariant(QString::fromUtf8("b"))}}, Qt::Vertical, false, nullptr);
    join_radio->setValue(QVariant(QString::fromUtf8("s")));
    lay3->addWidget(join_radio, 8, 0, 1, 2);
    auto* w5 = new QFrame(nullptr);
    w5->setProperty("frameShadow", 48);
    w5->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w5->setSizePolicy(sp); }
    lay3->addWidget(w5, 9, 0, 1, 2);
    invert_btn = new FCButton(nullptr);
    { QFont f = invert_btn->font(); f.setBold(true); invert_btn->setFont(f); }
    invert_btn->setIcon(QIcon(QString::fromUtf8(":/images/invert32.png")));
    invert_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    invert_btn->setText(QString::fromUtf8("Invertir Gerber"));
    invert_btn->setToolTip(QString::fromUtf8("Invertirá el objeto Gerber: áreas que tienen cobre.\nestará vacío de cobre y el área vacía anterior será\nlleno de cobre."));
    lay3->addWidget(invert_btn, 10, 0, 1, 2);
    lay3->setColumnStretch(1, 1);
    tools_box->addLayout(lay3, 0);
    tools_box->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new FCButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    reset_button->setText(QString::fromUtf8("Restablecer la Herramienta"));
    reset_button->setToolTip(QString::fromUtf8("Restablecerá los parámetros de la herramienta."));
    tools_box->addWidget(reset_button, 0);
    tools_frame->setLayout(tools_box);
    layout->addWidget(tools_frame, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> InvertToolUIBase::membersByName() const {
    return {
        {"gerber_combo", gerber_combo},
        {"gerber_label", gerber_label},
        {"invert_btn", invert_btn},
        {"join_label", join_label},
        {"join_radio", join_radio},
        {"layout", layout},
        {"margin_entry", margin_entry},
        {"margin_label", margin_label},
        {"param_label", param_label},
        {"reset_button", reset_button},
        {"tools_box", tools_box},
        {"tools_frame", tools_frame},
    };
}

}  // namespace flatcam::gui
