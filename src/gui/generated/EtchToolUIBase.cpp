// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/EtchToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void EtchToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Herramienta de Comp de Grabado"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    tools_frame = new QFrame(nullptr);
    tools_box = new QVBoxLayout();
    tools_box->setContentsMargins(0, 0, 0, 0);
    tools_box->setSpacing(6);
    auto* w1 = new QLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Herramienta de Comp de Grabado"));
    tools_box->addWidget(w1, 0);
    auto* lay2 = new QGridLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    auto* w3 = new QLabel(nullptr);
    lay2->addWidget(w3, 0, 0, 1, 2);
    gerber_label = new QLabel(nullptr);
    gerber_label->setText(QString::fromUtf8("<b>GERBER:</b>"));
    gerber_label->setToolTip(QString::fromUtf8("Objeto de Gerber que se invertirá."));
    lay2->addWidget(gerber_label, 1, 0, 1, 2);
    gerber_combo = new FCComboBox(nullptr);
    lay2->addWidget(gerber_combo, 2, 0, 1, 2);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay2->addWidget(w4, 3, 0, 1, 2);
    util_label = new QLabel(nullptr);
    util_label->setText(QString::fromUtf8("<b>Utilidades:</b>"));
    util_label->setToolTip(QString::fromUtf8("Utilidades de conversión."));
    lay2->addWidget(util_label, 4, 0, 1, 2);
    oz_um_label = new QLabel(nullptr);
    oz_um_label->setText(QString::fromUtf8("Oz a Micrones:"));
    oz_um_label->setToolTip(QString::fromUtf8("Se convertirá del grosor de oz a micras [um].\nPuede usar fórmulas con operadores: /, *, +, -,%,.\nLos números reales usan el separador de decimales de punto."));
    lay2->addWidget(oz_um_label, 5, 0, 1, 2);
    auto* lay5 = new QHBoxLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setSpacing(6);
    oz_entry = new NumericalEvalEntry(nullptr);
    oz_entry->setProperty("placeholderText", QString::fromUtf8("Valor de oz"));
    oz_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    lay5->addWidget(oz_entry, 0);
    oz_to_um_entry = new FCEntry(nullptr);
    oz_to_um_entry->setProperty("placeholderText", QString::fromUtf8("Valor de micras"));
    oz_to_um_entry->setReadOnly(true);
    lay5->addWidget(oz_to_um_entry, 0);
    lay2->addLayout(lay5, 6, 0, 1, 2);
    mils_um_label = new QLabel(nullptr);
    mils_um_label->setText(QString::fromUtf8("Mils a Micrones:"));
    mils_um_label->setToolTip(QString::fromUtf8("Se convertirá de milésimas de pulgada a micras [um].\nPuede usar fórmulas con operadores: /, *, +, -,%,.\nLos números reales usan el separador de decimales de punto."));
    lay2->addWidget(mils_um_label, 7, 0, 1, 2);
    auto* lay6 = new QHBoxLayout();
    lay6->setContentsMargins(0, 0, 0, 0);
    lay6->setSpacing(6);
    mils_entry = new NumericalEvalEntry(nullptr);
    mils_entry->setProperty("placeholderText", QString::fromUtf8("Valor de milésimas"));
    mils_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    lay6->addWidget(mils_entry, 0);
    mils_to_um_entry = new FCEntry(nullptr);
    mils_to_um_entry->setProperty("placeholderText", QString::fromUtf8("Valor de micras"));
    mils_to_um_entry->setReadOnly(true);
    lay6->addWidget(mils_to_um_entry, 0);
    lay2->addLayout(lay6, 8, 0, 1, 2);
    auto* w7 = new QFrame(nullptr);
    w7->setProperty("frameShadow", 48);
    w7->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w7->setSizePolicy(sp); }
    lay2->addWidget(w7, 9, 0, 1, 2);
    param_label = new QLabel(nullptr);
    param_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    param_label->setToolTip(QString::fromUtf8("Parám. para esta herramienta."));
    lay2->addWidget(param_label, 10, 0, 1, 2);
    thick_label = new QLabel(nullptr);
    thick_label->setText(QString::fromUtf8("Espesor de cobre:"));
    thick_label->setToolTip(QString::fromUtf8("El grosor de la lámina de cobre.\nEn micras [um]."));
    lay2->addWidget(thick_label, 12, 0, 1, 1);
    thick_entry = new FCDoubleSpinner(nullptr);
    thick_entry->setProperty("decimals", 4);
    thick_entry->setProperty("maximum", 10000.0);
    thick_entry->setProperty("value", 18.0);
    lay2->addWidget(thick_entry, 12, 1, 1, 1);
    ratio_label = new QLabel(nullptr);
    ratio_label->setText(QString::fromUtf8("Proporción:"));
    ratio_label->setToolTip(QString::fromUtf8("La relación de grabado lateral versus grabado profundo.\nPuede ser:\n- personalizado -> el usuario ingresará un valor personalizado\n- preseleccionado -> valor que depende de una selección de grabadores"));
    lay2->addWidget(ratio_label, 14, 0, 1, 2);
    ratio_radio = new RadioSet({{QString::fromUtf8("Factor de grabado"), QVariant(QString::fromUtf8("factor"))}, {QString::fromUtf8("Lista de grabados"), QVariant(QString::fromUtf8("etch_list"))}, {QString::fromUtf8("Desplazamiento manual"), QVariant(QString::fromUtf8("manual"))}}, Qt::Vertical, false, nullptr);
    ratio_radio->setValue(QVariant(QString::fromUtf8("factor")));
    lay2->addWidget(ratio_radio, 16, 0, 1, 2);
    etchants_label = new QLabel(nullptr);
    etchants_label->setText(QString::fromUtf8("Grabadores:"));
    etchants_label->setToolTip(QString::fromUtf8("Una lista de grabadores."));
    etchants_label->setHidden(true);
    lay2->addWidget(etchants_label, 18, 0, 1, 1);
    etchants_combo = new FCComboBox(nullptr);
    etchants_combo->setProperty("currentText", QString::fromUtf8("CuCl2"));
    etchants_combo->setHidden(true);
    etchants_combo->addItem(QString::fromUtf8("CuCl2"));
    etchants_combo->addItem(QString::fromUtf8("Fe3Cl"));
    etchants_combo->addItem(QString::fromUtf8("Baños alcalinos"));
    lay2->addWidget(etchants_combo, 18, 1, 1, 1);
    factor_label = new QLabel(nullptr);
    factor_label->setText(QString::fromUtf8("Factor de grabado:"));
    factor_label->setToolTip(QString::fromUtf8("La relación entre el grabado profundo y el grabado lateral.\nAcepta números reales y fórmulas utilizando los operadores: /, *, +, -,%"));
    lay2->addWidget(factor_label, 19, 0, 1, 1);
    factor_entry = new NumericalEvalEntry(nullptr);
    factor_entry->setProperty("placeholderText", QString::fromUtf8("Número real o fórmula"));
    factor_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    factor_entry->setText(QString::fromUtf8("1"));
    lay2->addWidget(factor_entry, 19, 1, 1, 1);
    offset_label = new QLabel(nullptr);
    offset_label->setText(QString::fromUtf8("Compensar:"));
    offset_label->setToolTip(QString::fromUtf8("Valor con el que aumentar o disminuir (buffer)\nLas características de cobre. En micras [um]."));
    offset_label->setHidden(true);
    lay2->addWidget(offset_label, 20, 0, 1, 1);
    offset_entry = new FCDoubleSpinner(nullptr);
    offset_entry->setProperty("decimals", 4);
    offset_entry->setProperty("maximum", 10000.0);
    offset_entry->setProperty("minimum", -10000.0);
    offset_entry->setHidden(true);
    lay2->addWidget(offset_entry, 20, 1, 1, 1);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    lay2->addWidget(w8, 22, 0, 1, 2);
    compensate_btn = new FCButton(nullptr);
    { QFont f = compensate_btn->font(); f.setBold(true); compensate_btn->setFont(f); }
    compensate_btn->setIcon(QIcon(QString::fromUtf8(":/images/etch_32.png")));
    compensate_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    compensate_btn->setText(QString::fromUtf8("Compensar"));
    compensate_btn->setToolTip(QString::fromUtf8("Aumentará el grosor de las características de cobre para compensar el grabado lateral."));
    lay2->addWidget(compensate_btn, 24, 0, 1, 2);
    lay2->setColumnStretch(1, 1);
    tools_box->addLayout(lay2, 0);
    tools_box->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new QPushButton(nullptr);
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

std::map<std::string, QObject*> EtchToolUIBase::membersByName() const {
    return {
        {"compensate_btn", compensate_btn},
        {"etchants_combo", etchants_combo},
        {"etchants_label", etchants_label},
        {"factor_entry", factor_entry},
        {"factor_label", factor_label},
        {"gerber_combo", gerber_combo},
        {"gerber_label", gerber_label},
        {"layout", layout},
        {"mils_entry", mils_entry},
        {"mils_to_um_entry", mils_to_um_entry},
        {"mils_um_label", mils_um_label},
        {"offset_entry", offset_entry},
        {"offset_label", offset_label},
        {"oz_entry", oz_entry},
        {"oz_to_um_entry", oz_to_um_entry},
        {"oz_um_label", oz_um_label},
        {"param_label", param_label},
        {"ratio_label", ratio_label},
        {"ratio_radio", ratio_radio},
        {"reset_button", reset_button},
        {"thick_entry", thick_entry},
        {"thick_label", thick_label},
        {"tools_box", tools_box},
        {"tools_frame", tools_frame},
        {"util_label", util_label},
    };
}

}  // namespace flatcam::gui
