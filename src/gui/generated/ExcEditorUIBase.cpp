// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/ExcEditorUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void ExcEditorUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    auto* lay1 = new QVBoxLayout();
    lay1->setContentsMargins(11, 11, 11, 11);
    lay1->setSpacing(6);
    drills_frame = new QFrame(nullptr);
    ui_vertical_lay = new QVBoxLayout();
    ui_vertical_lay->setContentsMargins(0, 0, 0, 0);
    ui_vertical_lay->setSpacing(6);
    title_box = new QHBoxLayout();
    title_box->setContentsMargins(0, 0, 0, 0);
    title_box->setSpacing(6);
    icon = new FCLabel(nullptr);
    icon->setPixmap(QPixmap(QString::fromUtf8(":/images/flatcam_icon32.png")));
    title_box->addWidget(icon, 0);
    title_label = new FCLabel(nullptr);
    title_label->setText(QString::fromUtf8("<font size=5><b>Excellon Editor</b></font>"));
    title_box->addWidget(title_label, 1);
    ui_vertical_lay->addLayout(title_box, 0);
    name_box = new QHBoxLayout();
    name_box->setContentsMargins(0, 0, 0, 0);
    name_box->setSpacing(6);
    auto* w2 = new FCLabel(nullptr);
    w2->setText(QString::fromUtf8("Nombre:"));
    name_box->addWidget(w2, 0);
    name_entry = new FCEntry(nullptr);
    name_box->addWidget(name_entry, 0);
    ui_vertical_lay->addLayout(name_box, 0);
    tools_table_label = new FCLabel(nullptr);
    tools_table_label->setText(QString::fromUtf8("<b>Tabla de herramientas</b>"));
    tools_table_label->setToolTip(QString::fromUtf8("Herramientas en este objeto Excellon.\nCuando se utilizan para la perforación."));
    ui_vertical_lay->addWidget(tools_table_label, 0);
    tools_table_exc = new FCTable(nullptr);
    tools_table_exc->setProperty("columnCount", 4);
    tools_table_exc->setProperty("selectionBehavior", 1);
    tools_table_exc->setColumnCount(4);
    tools_table_exc->setHorizontalHeaderLabels({QString::fromUtf8("#"), QString::fromUtf8("Diámetro"), QString::fromUtf8("D"), QString::fromUtf8("S")});
    ui_vertical_lay->addWidget(tools_table_exc, 0);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    ui_vertical_lay->addWidget(w3, 0);
    convert_slots_btn = new FCButton(nullptr);
    convert_slots_btn->setIcon(QIcon(QString::fromUtf8(":/images/convert32.png")));
    convert_slots_btn->setText(QString::fromUtf8("Convertir ranuras"));
    convert_slots_btn->setToolTip(QString::fromUtf8("Convierta las ranuras de las herramientas seleccionadas en taladros."));
    ui_vertical_lay->addWidget(convert_slots_btn, 0);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    ui_vertical_lay->addWidget(w4, 0);
    addtool_label = new FCLabel(nullptr);
    addtool_label->setText(QString::fromUtf8("<b>Añadir / Eliminar herramienta</b>"));
    addtool_label->setToolTip(QString::fromUtf8("Agregar / Eliminar una herramienta a la lista de herramientas\npara este objeto Excellon."));
    ui_vertical_lay->addWidget(addtool_label, 0);
    auto* lay5 = new QGridLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setHorizontalSpacing(6);
    lay5->setVerticalSpacing(6);
    auto* w6 = new FCLabel(nullptr);
    w6->setText(QString::fromUtf8("Diá. de Herram:"));
    w6->setToolTip(QString::fromUtf8("Diámetro para la nueva herramienta"));
    lay5->addWidget(w6, 0, 0, 1, 1);
    auto* lay7 = new QHBoxLayout();
    lay7->setContentsMargins(0, 0, 0, 0);
    lay7->setSpacing(6);
    addtool_entry = new FCDoubleSpinner(nullptr);
    addtool_entry->setProperty("decimals", 4);
    addtool_entry->setProperty("maximum", 10000.0);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); addtool_entry->setSizePolicy(sp); }
    lay7->addWidget(addtool_entry, 0);
    addtool_btn = new FCButton(nullptr);
    addtool_btn->setIcon(QIcon(QString::fromUtf8(":/images/plus16.png")));
    addtool_btn->setText(QString::fromUtf8("Añadir"));
    addtool_btn->setToolTip(QString::fromUtf8("Agregar una nueva herramienta a la lista de herramientas\ncon el diámetro especificado anteriormente."));
    lay7->addWidget(addtool_btn, 0);
    lay5->addLayout(lay7, 0, 1, 1, 1);
    deltool_btn = new FCButton(nullptr);
    deltool_btn->setIcon(QIcon(QString::fromUtf8(":/images/trash32.png")));
    deltool_btn->setText(QString::fromUtf8("Eliminar herramienta"));
    deltool_btn->setToolTip(QString::fromUtf8("Eliminar una herramienta en la lista de herramientas\nseleccionando una fila en la tabla de herramientas."));
    lay5->addWidget(deltool_btn, 2, 0, 1, 2);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    lay5->addWidget(w8, 4, 0, 1, 2);
    lay5->setColumnStretch(1, 1);
    ui_vertical_lay->addLayout(lay5, 0);
    resize_frame = new QFrame(nullptr);
    resize_frame->setHidden(true);
    resize_grid = new QGridLayout();
    resize_grid->setContentsMargins(0, 0, 0, 0);
    resize_grid->setHorizontalSpacing(6);
    resize_grid->setVerticalSpacing(6);
    drillresize_label = new FCLabel(nullptr);
    drillresize_label->setText(QString::fromUtf8("<b>Herram. de Cambio de Tamaño</b>"));
    drillresize_label->setToolTip(QString::fromUtf8("Cambiar el tamaño de un ejercicio o una selección de ejercicios."));
    resize_grid->addWidget(drillresize_label, 0, 0, 1, 2);
    auto* w9 = new FCLabel(nullptr);
    w9->setText(QString::fromUtf8("Cambiar el diá:"));
    w9->setToolTip(QString::fromUtf8("Diámetro para redimensionar a."));
    resize_grid->addWidget(w9, 2, 0, 1, 1);
    auto* lay10 = new QHBoxLayout();
    lay10->setContentsMargins(0, 0, 0, 0);
    lay10->setSpacing(6);
    resdrill_entry = new FCDoubleSpinner(nullptr);
    resdrill_entry->setProperty("decimals", 4);
    resdrill_entry->setProperty("maximum", 10000.0);
    { QSizePolicy sp(QSizePolicy::Policy(3), QSizePolicy::Policy(5)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); resdrill_entry->setSizePolicy(sp); }
    lay10->addWidget(resdrill_entry, 0);
    resize_btn = new FCButton(nullptr);
    resize_btn->setIcon(QIcon(QString::fromUtf8(":/images/resize16.png")));
    resize_btn->setText(QString::fromUtf8("Redimensionar"));
    resize_btn->setToolTip(QString::fromUtf8("Cambiar el tamaño de taladro"));
    lay10->addWidget(resize_btn, 0);
    resize_grid->addLayout(lay10, 2, 1, 1, 1);
    auto* w11 = new QFrame(nullptr);
    w11->setProperty("frameShadow", 48);
    w11->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w11->setSizePolicy(sp); }
    resize_grid->addWidget(w11, 6, 0, 1, 2);
    resize_grid->setColumnStretch(1, 1);
    resize_frame->setLayout(resize_grid);
    ui_vertical_lay->addWidget(resize_frame, 0);
    array_frame = new QFrame(nullptr);
    array_frame->setHidden(true);
    array_grid = new QGridLayout();
    array_grid->setContentsMargins(0, 0, 0, 0);
    array_grid->setHorizontalSpacing(6);
    array_grid->setVerticalSpacing(6);
    drill_array_label = new FCLabel(nullptr);
    drill_array_label->setText(QString::fromUtf8("<b>Añadir Drill Array</b>"));
    drill_array_label->setToolTip(QString::fromUtf8("Agregar una matriz de taladros (lineal o circular)"));
    array_grid->addWidget(drill_array_label, 0, 0, 1, 2);
    auto* w12 = new FCLabel(nullptr);
    w12->setText(QString::fromUtf8("Tipo:"));
    w12->setToolTip(QString::fromUtf8("Seleccione el tipo de matriz de ejercicios para crear.\nPuede ser lineal X (Y) o circular"));
    array_grid->addWidget(w12, 2, 0, 1, 1);
    array_type_radio = new RadioSet({{QString::fromUtf8("Lineal"), QVariant(QString::fromUtf8("linear"))}, {QString::fromUtf8("Circular"), QVariant(QString::fromUtf8("circular"))}}, Qt::Horizontal, true, nullptr);
    array_grid->addWidget(array_type_radio, 2, 1, 1, 1);
    drill_array_size_label = new FCLabel(nullptr);
    drill_array_size_label->setText(QString::fromUtf8("Numero:"));
    drill_array_size_label->setToolTip(QString::fromUtf8("Especifique cuántos ejercicios debe estar en la matriz."));
    array_grid->addWidget(drill_array_size_label, 4, 0, 1, 1);
    drill_array_size_entry = new FCSpinner(nullptr);
    drill_array_size_entry->setProperty("maximum", 10000);
    drill_array_size_entry->setProperty("minimum", 1);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); drill_array_size_entry->setSizePolicy(sp); }
    drill_array_size_entry->setProperty("value", 1);
    array_grid->addWidget(drill_array_size_entry, 4, 1, 1, 1);
    array_linear_frame = new QFrame(nullptr);
    array_linear_frame->setHidden(true);
    lin_grid = new QGridLayout();
    lin_grid->setContentsMargins(0, 0, 0, 0);
    lin_grid->setHorizontalSpacing(6);
    lin_grid->setVerticalSpacing(6);
    drill_axis_label = new FCLabel(nullptr);
    drill_axis_label->setText(QString::fromUtf8("Dirección:"));
    drill_axis_label->setToolTip(QString::fromUtf8("Dirección en la que se orienta la matriz lineal:\n- 'X' - eje horizontal\n- 'Y' - eje vertical o\n- 'Ángulo': un ángulo personalizado para la inclinación de la matriz"));
    lin_grid->addWidget(drill_axis_label, 0, 0, 1, 1);
    drill_axis_radio = new RadioSet({{QString::fromUtf8("X"), QVariant(QString::fromUtf8("X"))}, {QString::fromUtf8("Y"), QVariant(QString::fromUtf8("Y"))}, {QString::fromUtf8("Ángulo"), QVariant(QString::fromUtf8("A"))}}, Qt::Horizontal, true, nullptr);
    lin_grid->addWidget(drill_axis_radio, 0, 1, 1, 1);
    drill_pitch_label = new FCLabel(nullptr);
    drill_pitch_label->setText(QString::fromUtf8("Paso:"));
    drill_pitch_label->setToolTip(QString::fromUtf8("Paso = Distancia entre elementos de la matriz."));
    lin_grid->addWidget(drill_pitch_label, 2, 0, 1, 1);
    drill_pitch_entry = new FCDoubleSpinner(nullptr);
    drill_pitch_entry->setProperty("decimals", 4);
    drill_pitch_entry->setProperty("maximum", 10000.0);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); drill_pitch_entry->setSizePolicy(sp); }
    lin_grid->addWidget(drill_pitch_entry, 2, 1, 1, 1);
    linear_angle_label = new FCLabel(nullptr);
    linear_angle_label->setText(QString::fromUtf8("Ángulo:"));
    linear_angle_label->setToolTip(QString::fromUtf8("Ángulo en el que se coloca la matriz lineal.\nLa precisión es de max 2 decimales.\nEl valor mínimo es: -360.00 grados.\nEl valor máximo es: 360.00 grados."));
    linear_angle_label->setHidden(true);
    lin_grid->addWidget(linear_angle_label, 4, 0, 1, 1);
    linear_angle_spinner = new FCDoubleSpinner(nullptr);
    linear_angle_spinner->setProperty("decimals", 4);
    linear_angle_spinner->setProperty("maximum", 360.0);
    linear_angle_spinner->setProperty("minimum", -360.0);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); linear_angle_spinner->setSizePolicy(sp); }
    linear_angle_spinner->setHidden(true);
    lin_grid->addWidget(linear_angle_spinner, 4, 1, 1, 1);
    lin_grid->setColumnStretch(1, 1);
    array_linear_frame->setLayout(lin_grid);
    array_grid->addWidget(array_linear_frame, 6, 0, 1, 2);
    array_circular_frame = new QFrame(nullptr);
    array_circular_frame->setHidden(true);
    circ_grid = new QGridLayout();
    circ_grid->setContentsMargins(0, 0, 0, 0);
    circ_grid->setHorizontalSpacing(6);
    circ_grid->setVerticalSpacing(6);
    drill_array_dir_lbl = new FCLabel(nullptr);
    drill_array_dir_lbl->setText(QString::fromUtf8("Dirección:"));
    drill_array_dir_lbl->setToolTip(QString::fromUtf8("Dirección para matriz circular.\nPuede ser CW = en sentido horario o CCW = en sentido antihorario."));
    circ_grid->addWidget(drill_array_dir_lbl, 0, 0, 1, 1);
    drill_array_dir_radio = new RadioSet({{QString::fromUtf8("CW"), QVariant(QString::fromUtf8("CW"))}, {QString::fromUtf8("CCW"), QVariant(QString::fromUtf8("CCW"))}}, Qt::Horizontal, true, nullptr);
    circ_grid->addWidget(drill_array_dir_radio, 0, 1, 1, 1);
    drill_array_angle_lbl = new FCLabel(nullptr);
    drill_array_angle_lbl->setText(QString::fromUtf8("Ángulo:"));
    drill_array_angle_lbl->setToolTip(QString::fromUtf8("Ángulo en el que se coloca cada elemento de la matriz circular."));
    circ_grid->addWidget(drill_array_angle_lbl, 2, 0, 1, 1);
    drill_angle_entry = new FCDoubleSpinner(nullptr);
    drill_angle_entry->setProperty("decimals", 4);
    drill_angle_entry->setProperty("maximum", 360.0);
    drill_angle_entry->setProperty("minimum", -360.0);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); drill_angle_entry->setSizePolicy(sp); }
    circ_grid->addWidget(drill_angle_entry, 2, 1, 1, 1);
    circ_grid->setColumnStretch(1, 1);
    array_circular_frame->setLayout(circ_grid);
    array_grid->addWidget(array_circular_frame, 8, 0, 1, 2);
    auto* w13 = new QFrame(nullptr);
    w13->setProperty("frameShadow", 48);
    w13->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w13->setSizePolicy(sp); }
    array_grid->addWidget(w13, 10, 0, 1, 2);
    array_grid->setColumnStretch(1, 1);
    array_frame->setLayout(array_grid);
    ui_vertical_lay->addWidget(array_frame, 0);
    slot_frame = new QFrame(nullptr);
    slot_frame->setHidden(true);
    slot_grid = new QGridLayout();
    slot_grid->setContentsMargins(0, 0, 0, 0);
    slot_grid->setHorizontalSpacing(6);
    slot_grid->setVerticalSpacing(6);
    slot_label = new FCLabel(nullptr);
    slot_label->setText(QString::fromUtf8("<b>Parámetros de ranura</b>"));
    slot_label->setToolTip(QString::fromUtf8("Parámetros para agregar una ranura (agujero con forma ovalada)\nya sea solo o como parte de una matriz."));
    slot_grid->addWidget(slot_label, 0, 0, 1, 2);
    slot_length_label = new FCLabel(nullptr);
    slot_length_label->setText(QString::fromUtf8("Longitud:"));
    slot_length_label->setToolTip(QString::fromUtf8("Longitud. La longitud de la ranura."));
    slot_grid->addWidget(slot_length_label, 2, 0, 1, 1);
    slot_length_entry = new FCDoubleSpinner(nullptr);
    slot_length_entry->setProperty("decimals", 4);
    slot_length_entry->setProperty("maximum", 10000.0);
    slot_length_entry->setProperty("singleStep", 0.1);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); slot_length_entry->setSizePolicy(sp); }
    slot_grid->addWidget(slot_length_entry, 2, 1, 1, 1);
    slot_axis_label = new FCLabel(nullptr);
    slot_axis_label->setText(QString::fromUtf8("Dirección:"));
    slot_axis_label->setToolTip(QString::fromUtf8("Dirección en la que se orienta la ranura:\n- 'X' - eje horizontal\n- 'Y' - eje vertical o\n- 'Ángulo': un ángulo personalizado para la inclinación de la ranura"));
    slot_grid->addWidget(slot_axis_label, 4, 0, 1, 1);
    slot_axis_radio = new RadioSet({{QString::fromUtf8("X"), QVariant(QString::fromUtf8("X"))}, {QString::fromUtf8("Y"), QVariant(QString::fromUtf8("Y"))}, {QString::fromUtf8("Ángulo"), QVariant(QString::fromUtf8("A"))}}, Qt::Horizontal, true, nullptr);
    slot_grid->addWidget(slot_axis_radio, 4, 1, 1, 1);
    slot_angle_label = new FCLabel(nullptr);
    slot_angle_label->setText(QString::fromUtf8("Ángulo:"));
    slot_angle_label->setToolTip(QString::fromUtf8("Ángulo en el que se coloca la ranura.\nLa precisión es de un máximo de 2 decimales.\nEl valor mínimo es: -360.00 grados.\nEl valor máximo es: 360.00 grados."));
    slot_grid->addWidget(slot_angle_label, 6, 0, 1, 1);
    slot_angle_spinner = new FCDoubleSpinner(nullptr);
    slot_angle_spinner->setProperty("decimals", 4);
    slot_angle_spinner->setProperty("maximum", 360.0);
    slot_angle_spinner->setProperty("minimum", -360.0);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); slot_angle_spinner->setSizePolicy(sp); }
    slot_angle_spinner->setWrapping(true);
    slot_grid->addWidget(slot_angle_spinner, 6, 1, 1, 1);
    auto* w14 = new QFrame(nullptr);
    w14->setProperty("frameShadow", 48);
    w14->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w14->setSizePolicy(sp); }
    slot_grid->addWidget(w14, 8, 0, 1, 2);
    slot_grid->setColumnStretch(1, 1);
    slot_frame->setLayout(slot_grid);
    ui_vertical_lay->addWidget(slot_frame, 0);
    slot_array_frame = new QFrame(nullptr);
    slot_array_frame->setHidden(true);
    slot_array_grid = new QGridLayout();
    slot_array_grid->setContentsMargins(0, 0, 0, 0);
    slot_array_grid->setHorizontalSpacing(6);
    slot_array_grid->setVerticalSpacing(6);
    slot_array_label = new FCLabel(nullptr);
    slot_array_label->setText(QString::fromUtf8("<b>Parámetros de matriz de ranuras</b>"));
    slot_array_label->setToolTip(QString::fromUtf8("Parámetros para la matriz de ranuras (matriz lineal o circular)"));
    slot_array_grid->addWidget(slot_array_label, 0, 0, 1, 2);
    auto* w15 = new FCLabel(nullptr);
    w15->setText(QString::fromUtf8("Tipo:"));
    w15->setToolTip(QString::fromUtf8("Seleccione el tipo de matriz de ranuras para crear.\nPuede ser lineal X (Y) o circular"));
    slot_array_grid->addWidget(w15, 2, 0, 1, 1);
    slot_array_type_radio = new RadioSet({{QString::fromUtf8("Lineal"), QVariant(QString::fromUtf8("linear"))}, {QString::fromUtf8("Circular"), QVariant(QString::fromUtf8("circular"))}}, Qt::Horizontal, true, nullptr);
    slot_array_grid->addWidget(slot_array_type_radio, 2, 1, 1, 1);
    slot_array_size_label = new FCLabel(nullptr);
    slot_array_size_label->setText(QString::fromUtf8("Numero:"));
    slot_array_size_label->setToolTip(QString::fromUtf8("Especifique cuántas ranuras debe haber en la matriz."));
    slot_array_grid->addWidget(slot_array_size_label, 4, 0, 1, 1);
    slot_array_size_entry = new FCSpinner(nullptr);
    slot_array_size_entry->setProperty("maximum", 10000);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); slot_array_size_entry->setSizePolicy(sp); }
    slot_array_grid->addWidget(slot_array_size_entry, 4, 1, 1, 1);
    slot_array_linear_frame = new QFrame(nullptr);
    slot_array_lin_grid = new QGridLayout();
    slot_array_lin_grid->setContentsMargins(0, 0, 0, 0);
    slot_array_lin_grid->setHorizontalSpacing(6);
    slot_array_lin_grid->setVerticalSpacing(6);
    slot_array_axis_label = new FCLabel(nullptr);
    slot_array_axis_label->setText(QString::fromUtf8("Dirección:"));
    slot_array_axis_label->setToolTip(QString::fromUtf8("Dirección en la que se orienta la matriz lineal:\n- 'X' - eje horizontal\n- 'Y' - eje vertical o\n- 'Ángulo': un ángulo personalizado para la inclinación de la matriz"));
    slot_array_lin_grid->addWidget(slot_array_axis_label, 0, 0, 1, 1);
    slot_array_axis_radio = new RadioSet({{QString::fromUtf8("X"), QVariant(QString::fromUtf8("X"))}, {QString::fromUtf8("Y"), QVariant(QString::fromUtf8("Y"))}, {QString::fromUtf8("Ángulo"), QVariant(QString::fromUtf8("A"))}}, Qt::Horizontal, true, nullptr);
    slot_array_lin_grid->addWidget(slot_array_axis_radio, 0, 1, 1, 1);
    slot_array_pitch_label = new FCLabel(nullptr);
    slot_array_pitch_label->setText(QString::fromUtf8("Paso:"));
    slot_array_pitch_label->setToolTip(QString::fromUtf8("Paso = Distancia entre elementos de la matriz."));
    slot_array_lin_grid->addWidget(slot_array_pitch_label, 2, 0, 1, 1);
    slot_array_pitch_entry = new FCDoubleSpinner(nullptr);
    slot_array_pitch_entry->setProperty("decimals", 4);
    slot_array_pitch_entry->setProperty("maximum", 10000.0);
    slot_array_pitch_entry->setProperty("singleStep", 0.1);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); slot_array_pitch_entry->setSizePolicy(sp); }
    slot_array_lin_grid->addWidget(slot_array_pitch_entry, 2, 1, 1, 1);
    slot_array_linear_angle_label = new FCLabel(nullptr);
    slot_array_linear_angle_label->setText(QString::fromUtf8("Ángulo:"));
    slot_array_linear_angle_label->setToolTip(QString::fromUtf8("Ángulo en el que se coloca la matriz lineal.\nLa precisión es de max 2 decimales.\nEl valor mínimo es: -360.00 grados.\nEl valor máximo es: 360.00 grados."));
    slot_array_linear_angle_label->setHidden(true);
    slot_array_lin_grid->addWidget(slot_array_linear_angle_label, 4, 0, 1, 1);
    slot_array_linear_angle_spinner = new FCDoubleSpinner(nullptr);
    slot_array_linear_angle_spinner->setProperty("decimals", 4);
    slot_array_linear_angle_spinner->setProperty("maximum", 360.0);
    slot_array_linear_angle_spinner->setProperty("minimum", -360.0);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); slot_array_linear_angle_spinner->setSizePolicy(sp); }
    slot_array_linear_angle_spinner->setHidden(true);
    slot_array_lin_grid->addWidget(slot_array_linear_angle_spinner, 4, 1, 1, 1);
    slot_array_lin_grid->setColumnStretch(1, 1);
    slot_array_linear_frame->setLayout(slot_array_lin_grid);
    slot_array_grid->addWidget(slot_array_linear_frame, 6, 0, 1, 2);
    slot_array_circular_frame = new QFrame(nullptr);
    slot_array_circ_grid = new QGridLayout();
    slot_array_circ_grid->setContentsMargins(0, 0, 0, 0);
    slot_array_circ_grid->setHorizontalSpacing(6);
    slot_array_circ_grid->setVerticalSpacing(6);
    slot_array_direction_label = new FCLabel(nullptr);
    slot_array_direction_label->setText(QString::fromUtf8("Dirección:"));
    slot_array_direction_label->setToolTip(QString::fromUtf8("Dirección para matriz circular.\nPuede ser CW = en sentido horario o CCW = en sentido antihorario."));
    slot_array_circ_grid->addWidget(slot_array_direction_label, 0, 0, 1, 1);
    slot_array_direction_radio = new RadioSet({{QString::fromUtf8("CW"), QVariant(QString::fromUtf8("CW"))}, {QString::fromUtf8("CCW"), QVariant(QString::fromUtf8("CCW"))}}, Qt::Horizontal, true, nullptr);
    slot_array_circ_grid->addWidget(slot_array_direction_radio, 0, 1, 1, 1);
    slot_array_angle_label = new FCLabel(nullptr);
    slot_array_angle_label->setText(QString::fromUtf8("Ángulo:"));
    slot_array_angle_label->setToolTip(QString::fromUtf8("Ángulo en el que se coloca cada elemento de la matriz circular."));
    slot_array_circ_grid->addWidget(slot_array_angle_label, 2, 0, 1, 1);
    slot_array_angle_entry = new FCDoubleSpinner(nullptr);
    slot_array_angle_entry->setProperty("decimals", 4);
    slot_array_angle_entry->setProperty("maximum", 360.0);
    slot_array_angle_entry->setProperty("minimum", -360.0);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); slot_array_angle_entry->setSizePolicy(sp); }
    slot_array_circ_grid->addWidget(slot_array_angle_entry, 2, 1, 1, 1);
    slot_array_circ_grid->setColumnStretch(1, 1);
    slot_array_circular_frame->setLayout(slot_array_circ_grid);
    slot_array_grid->addWidget(slot_array_circular_frame, 8, 0, 1, 2);
    slot_array_grid->setColumnStretch(1, 1);
    slot_array_frame->setLayout(slot_array_grid);
    ui_vertical_lay->addWidget(slot_array_frame, 0);
    ui_vertical_lay->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    drills_frame->setLayout(ui_vertical_lay);
    lay1->addWidget(drills_frame, 0);
    lay1->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    lay1->setStretch(lay1->count() - 1, 1);
    exit_editor_button = new FCButton(nullptr);
    { QFont f = exit_editor_button->font(); f.setBold(true); exit_editor_button->setFont(f); }
    exit_editor_button->setIcon(QIcon(QString::fromUtf8(":/images/power16.png")));
    exit_editor_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t  QPushButton\n\t\t\t\t\t\t\t\t\t  {\n\t\t\t\t\t\t\t\t\t\t  font-weight: bold;\n\t\t\t\t\t\t\t\t\t  }\n\t\t\t\t\t\t\t\t\t  "));
    exit_editor_button->setText(QString::fromUtf8("Salir del editor"));
    exit_editor_button->setToolTip(QString::fromUtf8("Salida del editor."));
    lay1->addWidget(exit_editor_button, 0);
    setLayout(lay1);
}

std::map<std::string, QObject*> ExcEditorUIBase::membersByName() const {
    return {
        {"addtool_btn", addtool_btn},
        {"addtool_entry", addtool_entry},
        {"addtool_label", addtool_label},
        {"array_circular_frame", array_circular_frame},
        {"array_frame", array_frame},
        {"array_grid", array_grid},
        {"array_linear_frame", array_linear_frame},
        {"array_type_radio", array_type_radio},
        {"circ_grid", circ_grid},
        {"convert_slots_btn", convert_slots_btn},
        {"deltool_btn", deltool_btn},
        {"drill_angle_entry", drill_angle_entry},
        {"drill_array_angle_lbl", drill_array_angle_lbl},
        {"drill_array_dir_lbl", drill_array_dir_lbl},
        {"drill_array_dir_radio", drill_array_dir_radio},
        {"drill_array_label", drill_array_label},
        {"drill_array_size_entry", drill_array_size_entry},
        {"drill_array_size_label", drill_array_size_label},
        {"drill_axis_label", drill_axis_label},
        {"drill_axis_radio", drill_axis_radio},
        {"drill_pitch_entry", drill_pitch_entry},
        {"drill_pitch_label", drill_pitch_label},
        {"drillresize_label", drillresize_label},
        {"drills_frame", drills_frame},
        {"exit_editor_button", exit_editor_button},
        {"icon", icon},
        {"lin_grid", lin_grid},
        {"linear_angle_label", linear_angle_label},
        {"linear_angle_spinner", linear_angle_spinner},
        {"name_box", name_box},
        {"name_entry", name_entry},
        {"resdrill_entry", resdrill_entry},
        {"resize_btn", resize_btn},
        {"resize_frame", resize_frame},
        {"resize_grid", resize_grid},
        {"slot_angle_label", slot_angle_label},
        {"slot_angle_spinner", slot_angle_spinner},
        {"slot_array_angle_entry", slot_array_angle_entry},
        {"slot_array_angle_label", slot_array_angle_label},
        {"slot_array_axis_label", slot_array_axis_label},
        {"slot_array_axis_radio", slot_array_axis_radio},
        {"slot_array_circ_grid", slot_array_circ_grid},
        {"slot_array_circular_frame", slot_array_circular_frame},
        {"slot_array_direction_label", slot_array_direction_label},
        {"slot_array_direction_radio", slot_array_direction_radio},
        {"slot_array_frame", slot_array_frame},
        {"slot_array_grid", slot_array_grid},
        {"slot_array_label", slot_array_label},
        {"slot_array_lin_grid", slot_array_lin_grid},
        {"slot_array_linear_angle_label", slot_array_linear_angle_label},
        {"slot_array_linear_angle_spinner", slot_array_linear_angle_spinner},
        {"slot_array_linear_frame", slot_array_linear_frame},
        {"slot_array_pitch_entry", slot_array_pitch_entry},
        {"slot_array_pitch_label", slot_array_pitch_label},
        {"slot_array_size_entry", slot_array_size_entry},
        {"slot_array_size_label", slot_array_size_label},
        {"slot_array_type_radio", slot_array_type_radio},
        {"slot_axis_label", slot_axis_label},
        {"slot_axis_radio", slot_axis_radio},
        {"slot_frame", slot_frame},
        {"slot_grid", slot_grid},
        {"slot_label", slot_label},
        {"slot_length_entry", slot_length_entry},
        {"slot_length_label", slot_length_label},
        {"title_box", title_box},
        {"title_label", title_label},
        {"tools_table_exc", tools_table_exc},
        {"tools_table_label", tools_table_label},
        {"ui_vertical_lay", ui_vertical_lay},
    };
}

}  // namespace flatcam::gui
