// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/CalculatorsToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void CalculatorsToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Calculadoras"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Calculadoras"));
    layout->addWidget(w1, 0);
    unists_spacer_label = new FCLabel(nullptr);
    unists_spacer_label->setText(QString::fromUtf8(" "));
    layout->addWidget(unists_spacer_label, 0);
    auto* w2 = new FCLabel(nullptr);
    w2->setText(QString::fromUtf8("<font size=3><b>Calculadora de unidades</b></font>"));
    layout->addWidget(w2, 0);
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    auto* w4 = new FCLabel(nullptr);
    w4->setText(QString::fromUtf8("MM"));
    lay3->addWidget(w4, 0, 0, 1, 1);
    auto* w5 = new FCLabel(nullptr);
    w5->setText(QString::fromUtf8("PULGADA"));
    lay3->addWidget(w5, 0, 1, 1, 1);
    mm_entry = new NumericalEvalEntry(nullptr);
    mm_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    mm_entry->setText(QString::fromUtf8("0.0000"));
    mm_entry->setToolTip(QString::fromUtf8("Aquí ingresa el valor a convertir de MM a PULGADA"));
    lay3->addWidget(mm_entry, 1, 0, 1, 1);
    inch_entry = new NumericalEvalEntry(nullptr);
    inch_entry->setStyleSheet(QString::fromUtf8("QLineEdit {border: 1px solid #0069A9;}"));
    inch_entry->setText(QString::fromUtf8("0.0000"));
    inch_entry->setToolTip(QString::fromUtf8("Aquí ingresa el valor a convertir de PULGADAS a MM"));
    lay3->addWidget(inch_entry, 1, 1, 1, 1);
    layout->addLayout(lay3, 0);
    v_shape_spacer_label = new FCLabel(nullptr);
    v_shape_spacer_label->setText(QString::fromUtf8(" "));
    layout->addWidget(v_shape_spacer_label, 0);
    auto* w6 = new FCLabel(nullptr);
    w6->setText(QString::fromUtf8("<font size=3><b>Calc. de herra. en forma de V</b></font>"));
    layout->addWidget(w6, 0);
    auto* lay7 = new QFormLayout();
    lay7->setContentsMargins(0, 0, 0, 0);
    lay7->setHorizontalSpacing(6);
    lay7->setVerticalSpacing(6);
    tipDia_label = new FCLabel(nullptr);
    tipDia_label->setText(QString::fromUtf8("Diá. de la punta:"));
    tipDia_label->setToolTip(QString::fromUtf8("Este es el diámetro de la punta de la herramienta.\nEstá especificado por el fabricante."));
    lay7->setWidget(0, QFormLayout::ItemRole(0), tipDia_label);
    tipDia_entry = new FCDoubleSpinner(nullptr);
    tipDia_entry->setProperty("decimals", 4);
    tipDia_entry->setProperty("maximum", 10000.0);
    tipDia_entry->setProperty("singleStep", 0.1);
    tipDia_entry->setProperty("value", 0.2);
    lay7->setWidget(0, QFormLayout::ItemRole(1), tipDia_entry);
    tipAngle_label = new FCLabel(nullptr);
    tipAngle_label->setText(QString::fromUtf8("Ángulo de la punta:"));
    tipAngle_label->setToolTip(QString::fromUtf8("Este es el ángulo de la punta de la herramienta.\nEstá especificado por el fabricante."));
    lay7->setWidget(1, QFormLayout::ItemRole(0), tipAngle_label);
    tipAngle_entry = new FCSpinner(nullptr);
    tipAngle_entry->setProperty("maximum", 180);
    tipAngle_entry->setProperty("singleStep", 5);
    tipAngle_entry->setProperty("value", 30);
    lay7->setWidget(1, QFormLayout::ItemRole(1), tipAngle_entry);
    cutDepth_label = new FCLabel(nullptr);
    cutDepth_label->setText(QString::fromUtf8("Corte Z:"));
    cutDepth_label->setToolTip(QString::fromUtf8("Esta es la profundidad para cortar el material.\nEn el CNCJob se encuentra el parámetro CutZ."));
    lay7->setWidget(2, QFormLayout::ItemRole(0), cutDepth_label);
    cutDepth_entry = new FCDoubleSpinner(nullptr);
    cutDepth_entry->setProperty("decimals", 4);
    cutDepth_entry->setProperty("maximum", 10000.0);
    cutDepth_entry->setProperty("minimum", -10000.0);
    cutDepth_entry->setProperty("value", 0.05);
    lay7->setWidget(2, QFormLayout::ItemRole(1), cutDepth_entry);
    effectiveToolDia_label = new FCLabel(nullptr);
    effectiveToolDia_label->setText(QString::fromUtf8("Diá. de Herram:"));
    effectiveToolDia_label->setToolTip(QString::fromUtf8("Este es el diámetro de la herramienta a ingresar\nSección FlatCAM Gerber.\nEn la sección CNCJob se llama >diá. de herra.<."));
    lay7->setWidget(3, QFormLayout::ItemRole(0), effectiveToolDia_label);
    effectiveToolDia_entry = new FCDoubleSpinner(nullptr);
    effectiveToolDia_entry->setProperty("decimals", 4);
    lay7->setWidget(3, QFormLayout::ItemRole(1), effectiveToolDia_entry);
    layout->addLayout(lay7, 0);
    calculate_vshape_button = new FCButton(nullptr);
    calculate_vshape_button->setIcon(QIcon(QString::fromUtf8(":/images/calculator16.png")));
    calculate_vshape_button->setText(QString::fromUtf8("Calcular"));
    calculate_vshape_button->setToolTip(QString::fromUtf8("Calcule el corte Z o el diámetro efectivo de la herramienta,\ndependiendo de cuál se desee y cuál se conozca. "));
    layout->addWidget(calculate_vshape_button, 0);
    auto* w8 = new FCLabel(nullptr);
    layout->addWidget(w8, 0);
    auto* w9 = new FCLabel(nullptr);
    w9->setText(QString::fromUtf8("<font size=3><b>Calculadora de electrochapado</b></font>"));
    w9->setToolTip(QString::fromUtf8("Esta calculadora es útil para aquellos que platican la vía / la almohadilla / los agujeros de perforación,\nUtilizando un método como tinta de graphite o tinta de hipofosfito de calcio o cloruro de paladio."));
    layout->addWidget(w9, 0);
    auto* lay10 = new QGridLayout();
    lay10->setContentsMargins(0, 0, 0, 0);
    lay10->setHorizontalSpacing(6);
    lay10->setVerticalSpacing(6);
    area_sel_label = new FCLabel(nullptr);
    area_sel_label->setText(QString::fromUtf8("Cálculo de área:"));
    area_sel_label->setToolTip(QString::fromUtf8("Elija cómo calcular el área del PCB."));
    lay10->addWidget(area_sel_label, 0, 0, 1, 1);
    area_sel_radio = new RadioSet({{QString::fromUtf8("Dimensiones"), QVariant(QString::fromUtf8("d"))}, {QString::fromUtf8("Zona"), QVariant(QString::fromUtf8("a"))}}, Qt::Horizontal, false, nullptr);
    area_sel_radio->setValue(QVariant(QString::fromUtf8("d")));
    lay10->addWidget(area_sel_radio, 1, 0, 1, 2);
    pcblengthlabel = new FCLabel(nullptr);
    pcblengthlabel->setText(QString::fromUtf8("Longitud del tablero:"));
    pcblengthlabel->setToolTip(QString::fromUtf8("Esta es la longitud del tablero. En centímetros."));
    lay10->addWidget(pcblengthlabel, 2, 0, 1, 1);
    auto* lay11 = new QHBoxLayout();
    lay11->setContentsMargins(0, 0, 0, 0);
    lay11->setSpacing(6);
    pcblength_entry = new FCDoubleSpinner(nullptr);
    pcblength_entry->setProperty("decimals", 4);
    pcblength_entry->setProperty("maximum", 10000.0);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); pcblength_entry->setSizePolicy(sp); }
    pcblength_entry->setProperty("value", 10.0);
    lay11->addWidget(pcblength_entry, 0);
    length_unit = new FCLabel(nullptr);
    length_unit->setMinimumSize(QSize(25, 0));
    length_unit->setText(QString::fromUtf8("cm"));
    lay11->addWidget(length_unit, 0);
    lay10->addLayout(lay11, 2, 1, 1, 1);
    pcbwidthlabel = new FCLabel(nullptr);
    pcbwidthlabel->setText(QString::fromUtf8("Ancho del tablero:"));
    pcbwidthlabel->setToolTip(QString::fromUtf8("Este es el ancho de la tabla. En centímetros."));
    lay10->addWidget(pcbwidthlabel, 4, 0, 1, 1);
    auto* lay12 = new QHBoxLayout();
    lay12->setContentsMargins(0, 0, 0, 0);
    lay12->setSpacing(6);
    pcbwidth_entry = new FCDoubleSpinner(nullptr);
    pcbwidth_entry->setProperty("decimals", 4);
    pcbwidth_entry->setProperty("maximum", 10000.0);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); pcbwidth_entry->setSizePolicy(sp); }
    pcbwidth_entry->setProperty("value", 10.0);
    lay12->addWidget(pcbwidth_entry, 0);
    width_unit = new FCLabel(nullptr);
    width_unit->setMinimumSize(QSize(25, 0));
    width_unit->setText(QString::fromUtf8("cm"));
    lay12->addWidget(width_unit, 0);
    lay10->addLayout(lay12, 4, 1, 1, 1);
    area_label = new FCLabel(nullptr);
    area_label->setText(QString::fromUtf8("Zona:"));
    area_label->setToolTip(QString::fromUtf8("Esta es el área del PCB."));
    area_label->setHidden(true);
    lay10->addWidget(area_label, 6, 0, 1, 1);
    auto* lay13 = new QHBoxLayout();
    lay13->setContentsMargins(0, 0, 0, 0);
    lay13->setSpacing(6);
    area_entry = new FCDoubleSpinner(nullptr);
    area_entry->setProperty("decimals", 4);
    area_entry->setProperty("maximum", 10000.0);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); area_entry->setSizePolicy(sp); }
    area_entry->setProperty("value", 100.0);
    area_entry->setHidden(true);
    lay13->addWidget(area_entry, 0);
    area_unit = new FCLabel(nullptr);
    area_unit->setMinimumSize(QSize(25, 0));
    area_unit->setText(QString::fromUtf8("cm<sup>2</sup>"));
    area_unit->setHidden(true);
    lay13->addWidget(area_unit, 0);
    lay10->addLayout(lay13, 6, 1, 1, 1);
    auto* w14 = new QFrame(nullptr);
    w14->setProperty("frameShadow", 48);
    w14->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w14->setSizePolicy(sp); }
    lay10->addWidget(w14, 7, 0, 1, 2);
    cdensity_label = new FCLabel(nullptr);
    cdensity_label->setText(QString::fromUtf8("Densidad actual:"));
    cdensity_label->setToolTip(QString::fromUtf8("Densidad de corriente para pasar por el tablero.\nEn amperios por pies cuadrados ASF."));
    lay10->addWidget(cdensity_label, 8, 0, 1, 1);
    auto* lay15 = new QHBoxLayout();
    lay15->setContentsMargins(0, 0, 0, 0);
    lay15->setSpacing(6);
    cdensity_entry = new FCDoubleSpinner(nullptr);
    cdensity_entry->setProperty("decimals", 4);
    cdensity_entry->setProperty("maximum", 10000.0);
    cdensity_entry->setProperty("singleStep", 0.1);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); cdensity_entry->setSizePolicy(sp); }
    cdensity_entry->setProperty("value", 13.0);
    lay15->addWidget(cdensity_entry, 0);
    auto* w16 = new FCLabel(nullptr);
    w16->setMinimumSize(QSize(25, 0));
    w16->setText(QString::fromUtf8("ASF"));
    lay15->addWidget(w16, 0);
    lay10->addLayout(lay15, 8, 1, 1, 1);
    growth_label = new FCLabel(nullptr);
    growth_label->setText(QString::fromUtf8("Crecimiento de cobre:"));
    growth_label->setToolTip(QString::fromUtf8("Qué tan grueso pretende ser el crecimiento del cobre.\nEn micras."));
    lay10->addWidget(growth_label, 10, 0, 1, 1);
    auto* lay17 = new QHBoxLayout();
    lay17->setContentsMargins(0, 0, 0, 0);
    lay17->setSpacing(6);
    growth_entry = new FCDoubleSpinner(nullptr);
    growth_entry->setProperty("decimals", 4);
    growth_entry->setProperty("maximum", 10000.0);
    growth_entry->setProperty("singleStep", 0.01);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); growth_entry->setSizePolicy(sp); }
    growth_entry->setProperty("value", 10.0);
    lay17->addWidget(growth_entry, 0);
    auto* w18 = new FCLabel(nullptr);
    w18->setMinimumSize(QSize(25, 0));
    w18->setText(QString::fromUtf8("um"));
    lay17->addWidget(w18, 0);
    lay10->addLayout(lay17, 10, 1, 1, 1);
    cvaluelabel = new FCLabel(nullptr);
    cvaluelabel->setText(QString::fromUtf8("Valor actual:"));
    cvaluelabel->setToolTip(QString::fromUtf8("Este es el valor de intensidad actual\npara configurar en la fuente de alimentación. En amperios."));
    lay10->addWidget(cvaluelabel, 12, 0, 1, 1);
    auto* lay19 = new QHBoxLayout();
    lay19->setContentsMargins(0, 0, 0, 0);
    lay19->setSpacing(6);
    cvalue_entry = new FCDoubleSpinner(nullptr);
    cvalue_entry->setProperty("decimals", 4);
    cvalue_entry->setProperty("maximum", 10000.0);
    cvalue_entry->setReadOnly(true);
    cvalue_entry->setProperty("singleStep", 0.1);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); cvalue_entry->setSizePolicy(sp); }
    lay19->addWidget(cvalue_entry, 0);
    auto* w20 = new FCLabel(nullptr);
    w20->setMinimumSize(QSize(25, 0));
    w20->setText(QString::fromUtf8("A"));
    lay19->addWidget(w20, 0);
    lay10->addLayout(lay19, 12, 1, 1, 1);
    timelabel = new FCLabel(nullptr);
    timelabel->setText(QString::fromUtf8("Hora:"));
    timelabel->setToolTip(QString::fromUtf8("Este es el tiempo calculado requerido para el procedimiento.\nEn minutos."));
    lay10->addWidget(timelabel, 14, 0, 1, 1);
    auto* lay21 = new QHBoxLayout();
    lay21->setContentsMargins(0, 0, 0, 0);
    lay21->setSpacing(6);
    time_entry = new FCDoubleSpinner(nullptr);
    time_entry->setProperty("decimals", 4);
    time_entry->setProperty("maximum", 10000.0);
    time_entry->setReadOnly(true);
    time_entry->setProperty("singleStep", 0.1);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); time_entry->setSizePolicy(sp); }
    lay21->addWidget(time_entry, 0);
    auto* w22 = new FCLabel(nullptr);
    w22->setMinimumSize(QSize(25, 0));
    w22->setText(QString::fromUtf8("min"));
    lay21->addWidget(w22, 0);
    lay10->addLayout(lay21, 14, 1, 1, 1);
    lay10->setColumnStretch(1, 1);
    layout->addLayout(lay10, 0);
    calculate_plate_button = new FCButton(nullptr);
    calculate_plate_button->setIcon(QIcon(QString::fromUtf8(":/images/calculator16.png")));
    calculate_plate_button->setText(QString::fromUtf8("Calcular"));
    calculate_plate_button->setToolTip(QString::fromUtf8("Calcule el valor de intensidad actual y el tiempo del procedimiento,\ndependiendo de los parámetros anteriores"));
    layout->addWidget(calculate_plate_button, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new FCButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    reset_button->setText(QString::fromUtf8("Restablecer la Herramienta"));
    reset_button->setToolTip(QString::fromUtf8("Restablecerá los parámetros de la herramienta."));
    layout->addWidget(reset_button, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> CalculatorsToolUIBase::membersByName() const {
    return {
        {"area_entry", area_entry},
        {"area_label", area_label},
        {"area_sel_label", area_sel_label},
        {"area_sel_radio", area_sel_radio},
        {"area_unit", area_unit},
        {"calculate_plate_button", calculate_plate_button},
        {"calculate_vshape_button", calculate_vshape_button},
        {"cdensity_entry", cdensity_entry},
        {"cdensity_label", cdensity_label},
        {"cutDepth_entry", cutDepth_entry},
        {"cutDepth_label", cutDepth_label},
        {"cvalue_entry", cvalue_entry},
        {"cvaluelabel", cvaluelabel},
        {"effectiveToolDia_entry", effectiveToolDia_entry},
        {"effectiveToolDia_label", effectiveToolDia_label},
        {"growth_entry", growth_entry},
        {"growth_label", growth_label},
        {"inch_entry", inch_entry},
        {"layout", layout},
        {"length_unit", length_unit},
        {"mm_entry", mm_entry},
        {"pcblength_entry", pcblength_entry},
        {"pcblengthlabel", pcblengthlabel},
        {"pcbwidth_entry", pcbwidth_entry},
        {"pcbwidthlabel", pcbwidthlabel},
        {"reset_button", reset_button},
        {"time_entry", time_entry},
        {"timelabel", timelabel},
        {"tipAngle_entry", tipAngle_entry},
        {"tipAngle_label", tipAngle_label},
        {"tipDia_entry", tipDia_entry},
        {"tipDia_label", tipDia_label},
        {"unists_spacer_label", unists_spacer_label},
        {"v_shape_spacer_label", v_shape_spacer_label},
        {"width_unit", width_unit},
    };
}

}  // namespace flatcam::gui
