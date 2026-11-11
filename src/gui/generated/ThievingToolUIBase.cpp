// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/ThievingToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void ThievingToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Herramienta Thieving Tool"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Herramienta Thieving Tool"));
    layout->addWidget(w1, 0);
    auto* w2 = new FCLabel(nullptr);
    layout->addWidget(w2, 0);
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    grbobj_label = new FCLabel(nullptr);
    grbobj_label->setText(QString::fromUtf8("<b>GERBER:</b>"));
    grbobj_label->setToolTip(QString::fromUtf8("Gerber Objeto al que se agregará un Copper Thieving."));
    lay3->addWidget(grbobj_label, 0, 0, 1, 1);
    grb_object_combo = new FCComboBox(nullptr);
    lay3->addWidget(grb_object_combo, 1, 0, 1, 2);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay3->addWidget(w4, 2, 0, 1, 2);
    lay3->setColumnStretch(1, 1);
    layout->addLayout(lay3, 0);
    auto* lay5 = new QGridLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setHorizontalSpacing(6);
    lay5->setVerticalSpacing(6);
    copper_fill_label = new FCLabel(nullptr);
    copper_fill_label->setText(QString::fromUtf8("<b>Parámetros</b>"));
    copper_fill_label->setToolTip(QString::fromUtf8("Parámetros utilizados para esta herramienta."));
    lay5->addWidget(copper_fill_label, 0, 0, 1, 2);
    clearance_label = new FCLabel(nullptr);
    clearance_label->setText(QString::fromUtf8("Despeje:"));
    clearance_label->setToolTip(QString::fromUtf8("Esto establece la distancia entre los componentes de Copper Thieving\n(el relleno de polígono puede dividirse en múltiples polígonos)\ny las rastros de cobre en el archivo Gerber."));
    lay5->addWidget(clearance_label, 2, 0, 1, 1);
    clearance_entry = new FCDoubleSpinner(nullptr);
    clearance_entry->setProperty("decimals", 4);
    clearance_entry->setProperty("maximum", 10000.0);
    clearance_entry->setProperty("singleStep", 0.1);
    clearance_entry->setProperty("value", 0.25);
    lay5->addWidget(clearance_entry, 2, 1, 1, 1);
    margin_label = new FCLabel(nullptr);
    margin_label->setText(QString::fromUtf8("Margen:"));
    margin_label->setToolTip(QString::fromUtf8("Margen de cuadro delimitador."));
    lay5->addWidget(margin_label, 4, 0, 1, 1);
    margin_entry = new FCDoubleSpinner(nullptr);
    margin_entry->setProperty("decimals", 4);
    margin_entry->setProperty("maximum", 10000.0);
    margin_entry->setProperty("singleStep", 0.1);
    margin_entry->setProperty("value", 1.0);
    lay5->addWidget(margin_entry, 4, 1, 1, 1);
    area_label = new FCLabel(nullptr);
    area_label->setText(QString::fromUtf8("Zona:"));
    area_label->setToolTip(QString::fromUtf8("Robar áreas con un área menor a este valor no se agregará."));
    lay5->addWidget(area_label, 6, 0, 1, 1);
    auto* lay6 = new QHBoxLayout();
    lay6->setContentsMargins(0, 0, 0, 0);
    lay6->setSpacing(6);
    area_entry = new FCDoubleSpinner(nullptr);
    area_entry->setProperty("decimals", 4);
    area_entry->setProperty("maximum", 10000.0);
    area_entry->setProperty("singleStep", 0.1);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); area_entry->setSizePolicy(sp); }
    area_entry->setProperty("value", 0.1);
    lay6->addWidget(area_entry, 0);
    auto* w7 = new FCLabel(nullptr);
    w7->setText(QString::fromUtf8("mm<sup>2</sup>"));
    lay6->addWidget(w7, 0);
    lay5->addLayout(lay6, 6, 1, 1, 1);
    reference_label = new FCLabel(nullptr);
    reference_label->setText(QString::fromUtf8("Referencia:"));
    reference_label->setToolTip(QString::fromUtf8("- 'Sí mismo': la extensión de Copper Thieving se basa en la extensión del objeto.\n- 'Selección de área': haga clic con el botón izquierdo del mouse para iniciar la selección del área a rellenar.\n- 'Objeto de referencia': 'Copper Thieving' dentro del área especificada por otro objeto."));
    lay5->addWidget(reference_label, 8, 0, 1, 1);
    reference_radio = new RadioSet({{QString::fromUtf8("Sí mismo"), QVariant(QString::fromUtf8("itself"))}, {QString::fromUtf8("Selección de área"), QVariant(QString::fromUtf8("area"))}, {QString::fromUtf8("Objeto de referencia"), QVariant(QString::fromUtf8("box"))}}, Qt::Vertical, false, nullptr);
    reference_radio->setValue(QVariant(QString::fromUtf8("itself")));
    lay5->addWidget(reference_radio, 8, 1, 1, 1);
    ref_combo_type_label = new FCLabel(nullptr);
    ref_combo_type_label->setText(QString::fromUtf8("Tipo de Ref:"));
    ref_combo_type_label->setToolTip(QString::fromUtf8("El tipo de objeto FlatCAM que se utilizará como referencia de 'Copper Thieving'.\nPuede ser Gerber, Excellon o Geometry."));
    ref_combo_type_label->setHidden(true);
    lay5->addWidget(ref_combo_type_label, 10, 0, 1, 1);
    ref_combo_type = new FCComboBox(nullptr);
    ref_combo_type->setProperty("currentText", QString::fromUtf8("Gerber"));
    ref_combo_type->setHidden(true);
    ref_combo_type->addItem(QString::fromUtf8("Gerber"));
    ref_combo_type->addItem(QString::fromUtf8("Excellon"));
    ref_combo_type->addItem(QString::fromUtf8("Geometría"));
    lay5->addWidget(ref_combo_type, 10, 1, 1, 1);
    ref_combo_label = new FCLabel(nullptr);
    ref_combo_label->setText(QString::fromUtf8("Objeto de Ref:"));
    ref_combo_label->setToolTip(QString::fromUtf8("El objeto FlatCAM que se utilizará como referencia de compensación sin cobre."));
    ref_combo_label->setHidden(true);
    lay5->addWidget(ref_combo_label, 12, 0, 1, 1);
    ref_combo = new FCComboBox(nullptr);
    ref_combo->setHidden(true);
    lay5->addWidget(ref_combo, 12, 1, 1, 1);
    bbox_type_label = new FCLabel(nullptr);
    bbox_type_label->setText(QString::fromUtf8("Tipo de caja:"));
    bbox_type_label->setToolTip(QString::fromUtf8("- 'Rectangular': el cuadro delimitador tendrá forma rectangular.\n- 'Mínimo': el cuadro delimitador tendrá forma de casco convexo."));
    lay5->addWidget(bbox_type_label, 14, 0, 1, 1);
    bbox_type_radio = new RadioSet({{QString::fromUtf8("Rectangular"), QVariant(QString::fromUtf8("rect"))}, {QString::fromUtf8("Mínimo"), QVariant(QString::fromUtf8("min"))}}, Qt::Horizontal, false, nullptr);
    bbox_type_radio->setValue(QVariant(QString::fromUtf8("rect")));
    lay5->addWidget(bbox_type_radio, 14, 1, 1, 1);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    lay5->addWidget(w8, 16, 0, 1, 2);
    fill_type_label = new FCLabel(nullptr);
    fill_type_label->setText(QString::fromUtf8("Tipo de relleno:"));
    fill_type_label->setToolTip(QString::fromUtf8("- 'Sólido': el robo de cobre será un polígono sólido.\n- 'Cuadrícula de puntos': el área vacía se rellenará con un patrón de puntos.\n- 'Cuadrícula de cuadrados': el área vacía se rellenará con un patrón de cuadrados.\n- 'Cuadrícula de líneas': el área vacía se rellenará con un patrón de líneas."));
    lay5->addWidget(fill_type_label, 18, 0, 1, 1);
    fill_type_radio = new RadioSet({{QString::fromUtf8("Sólido"), QVariant(QString::fromUtf8("solid"))}, {QString::fromUtf8("Cuadrícula de puntos"), QVariant(QString::fromUtf8("dot"))}, {QString::fromUtf8("Cuadrícula de cuadrados"), QVariant(QString::fromUtf8("square"))}, {QString::fromUtf8("Cuadrícula de líneas"), QVariant(QString::fromUtf8("line"))}}, Qt::Vertical, false, nullptr);
    fill_type_radio->setValue(QVariant(QString::fromUtf8("solid")));
    lay5->addWidget(fill_type_radio, 18, 1, 1, 1);
    lay5->setColumnStretch(1, 1);
    layout->addLayout(lay5, 0);
    dots_frame = new QFrame(nullptr);
    dots_frame->setHidden(true);
    auto* lay9 = new QGridLayout();
    lay9->setContentsMargins(0, 0, 0, 0);
    lay9->setHorizontalSpacing(6);
    lay9->setVerticalSpacing(6);
    dots_label = new FCLabel(nullptr);
    dots_label->setText(QString::fromUtf8("<b>Parámetros de cuadrícula de puntos</b>:"));
    lay9->addWidget(dots_label, 0, 0, 1, 2);
    dotdia_label = new FCLabel(nullptr);
    dotdia_label->setText(QString::fromUtf8("Dia:"));
    dotdia_label->setToolTip(QString::fromUtf8("Diámetro de punto en cuadrícula de puntos."));
    lay9->addWidget(dotdia_label, 1, 0, 1, 1);
    dot_dia_entry = new FCDoubleSpinner(nullptr);
    dot_dia_entry->setProperty("decimals", 4);
    dot_dia_entry->setProperty("maximum", 10000.0);
    dot_dia_entry->setProperty("singleStep", 0.1);
    dot_dia_entry->setProperty("value", 1.0);
    lay9->addWidget(dot_dia_entry, 1, 1, 1, 1);
    dotspacing_label = new FCLabel(nullptr);
    dotspacing_label->setText(QString::fromUtf8("Spacing:"));
    dotspacing_label->setToolTip(QString::fromUtf8("Distancia entre cada dos puntos en la cuadrícula de puntos."));
    lay9->addWidget(dotspacing_label, 2, 0, 1, 1);
    dot_spacing_entry = new FCDoubleSpinner(nullptr);
    dot_spacing_entry->setProperty("decimals", 4);
    dot_spacing_entry->setProperty("maximum", 10000.0);
    dot_spacing_entry->setProperty("singleStep", 0.1);
    dot_spacing_entry->setProperty("value", 2.0);
    lay9->addWidget(dot_spacing_entry, 2, 1, 1, 1);
    lay9->setColumnStretch(1, 1);
    dots_frame->setLayout(lay9);
    layout->addWidget(dots_frame, 0);
    squares_frame = new QFrame(nullptr);
    squares_frame->setHidden(true);
    auto* lay10 = new QGridLayout();
    lay10->setContentsMargins(0, 0, 0, 0);
    lay10->setHorizontalSpacing(6);
    lay10->setVerticalSpacing(6);
    squares_label = new FCLabel(nullptr);
    squares_label->setText(QString::fromUtf8("<b>Parámetros de la cuadrícula de cuadrados</b>:"));
    lay10->addWidget(squares_label, 0, 0, 1, 2);
    square_size_label = new FCLabel(nullptr);
    square_size_label->setText(QString::fromUtf8("Tamaño:"));
    square_size_label->setToolTip(QString::fromUtf8("Tamaño del lado cuadrado en cuadrícula de cuadrados."));
    lay10->addWidget(square_size_label, 1, 0, 1, 1);
    square_size_entry = new FCDoubleSpinner(nullptr);
    square_size_entry->setProperty("decimals", 4);
    square_size_entry->setProperty("maximum", 10000.0);
    square_size_entry->setProperty("singleStep", 0.1);
    square_size_entry->setProperty("value", 1.0);
    lay10->addWidget(square_size_entry, 1, 1, 1, 1);
    squares_spacing_label = new FCLabel(nullptr);
    squares_spacing_label->setText(QString::fromUtf8("Spacing:"));
    squares_spacing_label->setToolTip(QString::fromUtf8("Distancia entre cada dos cuadrados en la cuadrícula de cuadrados."));
    lay10->addWidget(squares_spacing_label, 2, 0, 1, 1);
    squares_spacing_entry = new FCDoubleSpinner(nullptr);
    squares_spacing_entry->setProperty("decimals", 4);
    squares_spacing_entry->setProperty("maximum", 10000.0);
    squares_spacing_entry->setProperty("singleStep", 0.1);
    squares_spacing_entry->setProperty("value", 2.0);
    lay10->addWidget(squares_spacing_entry, 2, 1, 1, 1);
    lay10->setColumnStretch(1, 1);
    squares_frame->setLayout(lay10);
    layout->addWidget(squares_frame, 0);
    lines_frame = new QFrame(nullptr);
    lines_frame->setHidden(true);
    auto* lay11 = new QGridLayout();
    lay11->setContentsMargins(0, 0, 0, 0);
    lay11->setHorizontalSpacing(6);
    lay11->setVerticalSpacing(6);
    lines_label = new FCLabel(nullptr);
    lines_label->setText(QString::fromUtf8("<b>Parámetros de cuadrícula de líneas</b>:"));
    lay11->addWidget(lines_label, 0, 0, 1, 2);
    line_size_label = new FCLabel(nullptr);
    line_size_label->setText(QString::fromUtf8("Tamaño:"));
    line_size_label->setToolTip(QString::fromUtf8("Tamaño del grosor de línea en la cuadrícula de líneas."));
    lay11->addWidget(line_size_label, 1, 0, 1, 1);
    line_size_entry = new FCDoubleSpinner(nullptr);
    line_size_entry->setProperty("decimals", 4);
    line_size_entry->setProperty("maximum", 10000.0);
    line_size_entry->setProperty("singleStep", 0.1);
    line_size_entry->setProperty("value", 0.25);
    lay11->addWidget(line_size_entry, 1, 1, 1, 1);
    lines_spacing_label = new FCLabel(nullptr);
    lines_spacing_label->setText(QString::fromUtf8("Spacing:"));
    lines_spacing_label->setToolTip(QString::fromUtf8("Distancia entre cada dos líneas en la cuadrícula de líneas."));
    lay11->addWidget(lines_spacing_label, 2, 0, 1, 1);
    lines_spacing_entry = new FCDoubleSpinner(nullptr);
    lines_spacing_entry->setProperty("decimals", 4);
    lines_spacing_entry->setProperty("maximum", 10000.0);
    lines_spacing_entry->setProperty("singleStep", 0.1);
    lines_spacing_entry->setProperty("value", 2.0);
    lay11->addWidget(lines_spacing_entry, 2, 1, 1, 1);
    lay11->setColumnStretch(1, 1);
    lines_frame->setLayout(lay11);
    layout->addWidget(lines_frame, 0);
    fill_button = new QPushButton(nullptr);
    { QFont f = fill_button->font(); f.setBold(true); fill_button->setFont(f); }
    fill_button->setIcon(QIcon(QString::fromUtf8(":/images/copperfill32.png")));
    fill_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    fill_button->setText(QString::fromUtf8("Insertar Copper thieving"));
    fill_button->setToolTip(QString::fromUtf8("Agregará un polígono (puede dividirse en varias partes)\neso rodeará las huellas reales de Gerber a cierta distancia."));
    layout->addWidget(fill_button, 0);
    auto* lay12 = new QGridLayout();
    lay12->setContentsMargins(0, 0, 0, 0);
    lay12->setHorizontalSpacing(6);
    lay12->setVerticalSpacing(6);
    auto* w13 = new QFrame(nullptr);
    w13->setProperty("frameShadow", 48);
    w13->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w13->setSizePolicy(sp); }
    lay12->addWidget(w13, 0, 0, 1, 3);
    auto* w14 = new FCLabel(nullptr);
    lay12->addWidget(w14, 1, 0, 1, 1);
    robber_bar_label = new FCLabel(nullptr);
    robber_bar_label->setText(QString::fromUtf8("<b>Parámetros de la Robber Bar</b>"));
    robber_bar_label->setToolTip(QString::fromUtf8("Parámetros utilizados para la Robber Bar.\nRobber Bar = borde de cobre para ayudar en el enchapado de agujeros."));
    lay12->addWidget(robber_bar_label, 2, 0, 1, 3);
    rb_margin_label = new FCLabel(nullptr);
    rb_margin_label->setText(QString::fromUtf8("Margen:"));
    rb_margin_label->setToolTip(QString::fromUtf8("Margen límite del recinto para Robber Bar."));
    lay12->addWidget(rb_margin_label, 4, 0, 1, 1);
    rb_margin_entry = new FCDoubleSpinner(nullptr);
    rb_margin_entry->setProperty("decimals", 4);
    rb_margin_entry->setProperty("maximum", 10000.0);
    rb_margin_entry->setProperty("minimum", -10000.0);
    rb_margin_entry->setProperty("singleStep", 0.1);
    rb_margin_entry->setProperty("value", 1.0);
    lay12->addWidget(rb_margin_entry, 4, 1, 1, 2);
    rb_thickness_label = new FCLabel(nullptr);
    rb_thickness_label->setText(QString::fromUtf8("Espesor:"));
    rb_thickness_label->setToolTip(QString::fromUtf8("El grosor de la Robber Bar."));
    lay12->addWidget(rb_thickness_label, 6, 0, 1, 1);
    rb_thickness_entry = new FCDoubleSpinner(nullptr);
    rb_thickness_entry->setProperty("decimals", 4);
    rb_thickness_entry->setProperty("maximum", 10000.0);
    rb_thickness_entry->setProperty("singleStep", 0.1);
    rb_thickness_entry->setProperty("value", 1.0);
    lay12->addWidget(rb_thickness_entry, 6, 1, 1, 2);
    rb_button = new QPushButton(nullptr);
    { QFont f = rb_button->font(); f.setBold(true); rb_button->setFont(f); }
    rb_button->setIcon(QIcon(QString::fromUtf8(":/images/robber32.png")));
    rb_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    rb_button->setText(QString::fromUtf8("Insertar Robber Bar"));
    rb_button->setToolTip(QString::fromUtf8("Agregará un polígono con un grosor definido\nque rodeará el objeto real de Gerber\na cierta distancia.\nSe requiere cuando se hace un patrón de agujeros."));
    lay12->addWidget(rb_button, 8, 0, 1, 3);
    auto* w15 = new QFrame(nullptr);
    w15->setProperty("frameShadow", 48);
    w15->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w15->setSizePolicy(sp); }
    lay12->addWidget(w15, 10, 0, 1, 3);
    patern_mask_label = new FCLabel(nullptr);
    patern_mask_label->setText(QString::fromUtf8("<b>Máscara de baño de patrones</b>"));
    patern_mask_label->setToolTip(QString::fromUtf8("Genere una máscara para el enchapado de patrones."));
    lay12->addWidget(patern_mask_label, 12, 0, 1, 3);
    sm_obj_label = new FCLabel(nullptr);
    sm_obj_label->setText(QString::fromUtf8("Seleccionar objeto Soldermask:"));
    sm_obj_label->setToolTip(QString::fromUtf8("Objeto Gerber con la máscara de soldadura.\nSe utilizará como base para\nEl patrón de la máscara de recubrimiento."));
    lay12->addWidget(sm_obj_label, 14, 0, 1, 3);
    sm_object_combo = new FCComboBox(nullptr);
    lay12->addWidget(sm_object_combo, 16, 0, 1, 3);
    clearance_ppm_label = new FCLabel(nullptr);
    clearance_ppm_label->setText(QString::fromUtf8("Despeje:"));
    clearance_ppm_label->setToolTip(QString::fromUtf8("La distancia entre los posibles elementos de Copper Thieving.\ny / o Robber Bar y las aberturas reales en la máscara."));
    lay12->addWidget(clearance_ppm_label, 18, 0, 1, 1);
    clearance_ppm_entry = new FCDoubleSpinner(nullptr);
    clearance_ppm_entry->setProperty("decimals", 4);
    clearance_ppm_entry->setProperty("maximum", 10000.0);
    clearance_ppm_entry->setProperty("minimum", -10000.0);
    clearance_ppm_entry->setProperty("singleStep", 0.1);
    lay12->addWidget(clearance_ppm_entry, 18, 1, 1, 2);
    plated_area_label = new FCLabel(nullptr);
    plated_area_label->setText(QString::fromUtf8("Área chapada:"));
    plated_area_label->setToolTip(QString::fromUtf8("El área a ser chapada por patrón de chapado.\nBásicamente está hecho de las aberturas en la máscara de recubrimiento.\n\n<<ADVERTENCIA>> - el área calculada es en realidad un poco más grande\nDebido al hecho de que las aberturas de la máscara de soldadura son por diseño\nun poco más grande que las almohadillas de cobre, y esta área es\ncalculado a partir de las aberturas de la máscara de soldadura."));
    lay12->addWidget(plated_area_label, 20, 0, 1, 1);
    plated_area_entry = new FCEntry(nullptr);
    plated_area_entry->setEnabled(false);
    lay12->addWidget(plated_area_entry, 20, 1, 1, 1);
    units_area_label = new FCLabel(nullptr);
    units_area_label->setText(QString::fromUtf8("mm<sup>2</sup>"));
    lay12->addWidget(units_area_label, 20, 2, 1, 1);
    ppm_choice_label = new FCLabel(nullptr);
    ppm_choice_label->setText(QString::fromUtf8("Añadir:"));
    ppm_choice_label->setToolTip(QString::fromUtf8("Elija qué geometría adicional incluir, si está disponible."));
    lay12->addWidget(ppm_choice_label, 22, 0, 1, 1);
    ppm_choice_radio = new RadioSet({{QString::fromUtf8("Ambas"), QVariant(QString::fromUtf8("b"))}, {QString::fromUtf8("Ladrón"), QVariant(QString::fromUtf8("t"))}, {QString::fromUtf8("Barra de Ladrón"), QVariant(QString::fromUtf8("r"))}, {QString::fromUtf8("Ninguno"), QVariant(QString::fromUtf8("n"))}}, Qt::Vertical, false, nullptr);
    ppm_choice_radio->setValue(QVariant(QString::fromUtf8("b")));
    lay12->addWidget(ppm_choice_radio, 22, 1, 1, 2);
    ppm_button = new QPushButton(nullptr);
    { QFont f = ppm_button->font(); f.setBold(true); ppm_button->setFont(f); }
    ppm_button->setIcon(QIcon(QString::fromUtf8(":/images/pattern32.png")));
    ppm_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    ppm_button->setText(QString::fromUtf8("Generar máscara de recubrimiento de patrón"));
    ppm_button->setToolTip(QString::fromUtf8("Agregará a la máscara de soldadura la geometría gerber\nLas geometrías de Copper Thieving y / o\nla Robber Bar si esos fueron generados."));
    lay12->addWidget(ppm_button, 24, 0, 1, 3);
    lay12->setColumnStretch(1, 1);
    layout->addLayout(lay12, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new QPushButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    reset_button->setText(QString::fromUtf8("Restablecer la Herramienta"));
    reset_button->setToolTip(QString::fromUtf8("Restablecerá los parámetros de la herramienta."));
    layout->addWidget(reset_button, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> ThievingToolUIBase::membersByName() const {
    return {
        {"area_entry", area_entry},
        {"area_label", area_label},
        {"bbox_type_label", bbox_type_label},
        {"bbox_type_radio", bbox_type_radio},
        {"clearance_entry", clearance_entry},
        {"clearance_label", clearance_label},
        {"clearance_ppm_entry", clearance_ppm_entry},
        {"clearance_ppm_label", clearance_ppm_label},
        {"copper_fill_label", copper_fill_label},
        {"dot_dia_entry", dot_dia_entry},
        {"dot_spacing_entry", dot_spacing_entry},
        {"dotdia_label", dotdia_label},
        {"dots_frame", dots_frame},
        {"dots_label", dots_label},
        {"dotspacing_label", dotspacing_label},
        {"fill_button", fill_button},
        {"fill_type_label", fill_type_label},
        {"fill_type_radio", fill_type_radio},
        {"grb_object_combo", grb_object_combo},
        {"grbobj_label", grbobj_label},
        {"layout", layout},
        {"line_size_entry", line_size_entry},
        {"line_size_label", line_size_label},
        {"lines_frame", lines_frame},
        {"lines_label", lines_label},
        {"lines_spacing_entry", lines_spacing_entry},
        {"lines_spacing_label", lines_spacing_label},
        {"margin_entry", margin_entry},
        {"margin_label", margin_label},
        {"patern_mask_label", patern_mask_label},
        {"plated_area_entry", plated_area_entry},
        {"plated_area_label", plated_area_label},
        {"ppm_button", ppm_button},
        {"ppm_choice_label", ppm_choice_label},
        {"ppm_choice_radio", ppm_choice_radio},
        {"rb_button", rb_button},
        {"rb_margin_entry", rb_margin_entry},
        {"rb_margin_label", rb_margin_label},
        {"rb_thickness_entry", rb_thickness_entry},
        {"rb_thickness_label", rb_thickness_label},
        {"ref_combo", ref_combo},
        {"ref_combo_label", ref_combo_label},
        {"ref_combo_type", ref_combo_type},
        {"ref_combo_type_label", ref_combo_type_label},
        {"reference_label", reference_label},
        {"reference_radio", reference_radio},
        {"reset_button", reset_button},
        {"robber_bar_label", robber_bar_label},
        {"sm_obj_label", sm_obj_label},
        {"sm_object_combo", sm_object_combo},
        {"square_size_entry", square_size_entry},
        {"square_size_label", square_size_label},
        {"squares_frame", squares_frame},
        {"squares_label", squares_label},
        {"squares_spacing_entry", squares_spacing_entry},
        {"squares_spacing_label", squares_spacing_label},
        {"units_area_label", units_area_label},
    };
}

}  // namespace flatcam::gui
