// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/GrbEditorUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void GrbEditorUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    auto* lay1 = new QVBoxLayout();
    lay1->setContentsMargins(11, 11, 11, 11);
    lay1->setSpacing(6);
    title_box = new QHBoxLayout();
    title_box->setContentsMargins(0, 0, 0, 0);
    title_box->setSpacing(6);
    icon = new FCLabel(nullptr);
    icon->setPixmap(QPixmap(QString::fromUtf8(":/images/flatcam_icon32.png")));
    title_box->addWidget(icon, 0);
    title_label = new FCLabel(nullptr);
    title_label->setText(QString::fromUtf8("<font size=5><b>Gerber Editor</b></font>"));
    title_box->addWidget(title_label, 1);
    lay1->addLayout(title_box, 0);
    name_box = new QHBoxLayout();
    name_box->setContentsMargins(0, 0, 0, 0);
    name_box->setSpacing(6);
    auto* w2 = new FCLabel(nullptr);
    w2->setText(QString::fromUtf8("Nombre:"));
    name_box->addWidget(w2, 0);
    name_entry = new FCEntry(nullptr);
    name_box->addWidget(name_entry, 0);
    lay1->addLayout(name_box, 0);
    custom_box = new QVBoxLayout();
    custom_box->setContentsMargins(0, 0, 0, 0);
    custom_box->setSpacing(6);
    apertures_table_label = new FCLabel(nullptr);
    apertures_table_label->setText(QString::fromUtf8("<b>Aberturas:</b>"));
    apertures_table_label->setToolTip(QString::fromUtf8("Tabla de Aperturas para el Objeto Gerber."));
    custom_box->addWidget(apertures_table_label, 0);
    apertures_table = new FCTable(nullptr);
    apertures_table->setProperty("columnCount", 5);
    apertures_table->setProperty("selectionBehavior", 1);
    apertures_table->setColumnCount(5);
    apertures_table->setHorizontalHeaderLabels({QString::fromUtf8("#"), QString::fromUtf8("Código"), QString::fromUtf8("Tipo"), QString::fromUtf8("Tamaño"), QString::fromUtf8("Dim")});
    custom_box->addWidget(apertures_table, 0);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    custom_box->addWidget(w3, 0);
    apertures_frame = new QFrame(nullptr);
    apertures_box = new QVBoxLayout();
    apertures_box->setContentsMargins(0, 0, 0, 0);
    apertures_box->setSpacing(6);
    auto* lay4 = new QGridLayout();
    lay4->setContentsMargins(0, 0, 0, 0);
    lay4->setHorizontalSpacing(6);
    lay4->setVerticalSpacing(6);
    auto* w5 = new FCLabel(nullptr);
    w5->setText(QString::fromUtf8("<b>Añadir / Eliminar Apertura:</b>"));
    w5->setToolTip(QString::fromUtf8("Añadir / Eliminar una apertura en la tabla de aperturas"));
    lay4->addWidget(w5, 0, 0, 1, 2);
    auto* w6 = new FCLabel(nullptr);
    w6->setText(QString::fromUtf8("Código de apertura:"));
    w6->setToolTip(QString::fromUtf8("Código para la nueva apertura"));
    lay4->addWidget(w6, 1, 0, 1, 1);
    apcode_entry = new FCSpinner(nullptr);
    apcode_entry->setProperty("maximum", 1000);
    apcode_entry->setWrapping(true);
    lay4->addWidget(apcode_entry, 1, 1, 1, 1);
    auto* w7 = new FCLabel(nullptr);
    w7->setText(QString::fromUtf8("Tamaño de apertura:"));
    w7->setToolTip(QString::fromUtf8("Tamaño para la nueva apertura.\nSi el tipo de apertura es 'R' o 'O' entonces\neste valor es automáticamente\ncalculado como:\nsqrt (ancho ** 2 + altura ** 2)"));
    lay4->addWidget(w7, 2, 0, 1, 1);
    apsize_entry = new FCDoubleSpinner(nullptr);
    apsize_entry->setProperty("decimals", 4);
    apsize_entry->setProperty("maximum", 10000.0);
    apsize_entry->setProperty("value", 0.8);
    lay4->addWidget(apsize_entry, 2, 1, 1, 1);
    auto* w8 = new FCLabel(nullptr);
    w8->setText(QString::fromUtf8("Tipo de apertura:"));
    w8->setToolTip(QString::fromUtf8("Seleccione el tipo de apertura nueva. Puede ser:\nC = circular\nR = rectangular\nO = oblongo"));
    lay4->addWidget(w8, 3, 0, 1, 1);
    aptype_cb = new FCComboBox(nullptr);
    aptype_cb->setProperty("currentText", QString::fromUtf8("C"));
    aptype_cb->addItem(QString::fromUtf8("C"));
    aptype_cb->addItem(QString::fromUtf8("R"));
    aptype_cb->addItem(QString::fromUtf8("O"));
    lay4->addWidget(aptype_cb, 3, 1, 1, 1);
    apdim_lbl = new FCLabel(nullptr);
    apdim_lbl->setText(QString::fromUtf8("Apertura Dim:"));
    apdim_lbl->setToolTip(QString::fromUtf8("Dimensiones para la nueva apertura.\nActivo solo para aberturas rectangulares (tipo R).\nEl formato es (ancho, alto)."));
    apdim_lbl->setHidden(true);
    lay4->addWidget(apdim_lbl, 4, 0, 1, 1);
    apdim_entry = new QLineEdit(nullptr);
    apdim_entry->setText(QString::fromUtf8("0.5, 0.5"));
    apdim_entry->setHidden(true);
    lay4->addWidget(apdim_entry, 4, 1, 1, 1);
    auto* w9 = new QFrame(nullptr);
    w9->setProperty("frameShadow", 48);
    w9->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w9->setSizePolicy(sp); }
    lay4->addWidget(w9, 6, 0, 1, 2);
    auto* lay10 = new QHBoxLayout();
    lay10->setContentsMargins(0, 0, 0, 0);
    lay10->setSpacing(6);
    addaperture_btn = new FCButton(nullptr);
    addaperture_btn->setIcon(QIcon(QString::fromUtf8(":/images/plus16.png")));
    addaperture_btn->setText(QString::fromUtf8("Añadir"));
    addaperture_btn->setToolTip(QString::fromUtf8("Agregar una nueva apertura a la lista de apertura."));
    lay10->addWidget(addaperture_btn, 0);
    delaperture_btn = new FCButton(nullptr);
    delaperture_btn->setIcon(QIcon(QString::fromUtf8(":/images/trash32.png")));
    delaperture_btn->setText(QString::fromUtf8("Borrar"));
    delaperture_btn->setToolTip(QString::fromUtf8("Eliminar una abertura en la lista de aperturas"));
    lay10->addWidget(delaperture_btn, 0);
    lay4->addLayout(lay10, 8, 0, 1, 2);
    lay4->setColumnStretch(1, 1);
    apertures_box->addLayout(lay4, 0);
    apertures_frame->setLayout(apertures_box);
    custom_box->addWidget(apertures_frame, 0);
    buffer_tool_frame = new QFrame(nullptr);
    buffer_tool_frame->setHidden(true);
    buffer_tools_box = new QVBoxLayout();
    buffer_tools_box->setContentsMargins(0, 0, 0, 0);
    buffer_tools_box->setSpacing(6);
    auto* w11 = new FCLabel(nullptr);
    w11->setText(QString::fromUtf8("<b>Apertura del tampón:</b>"));
    w11->setToolTip(QString::fromUtf8("Buffer de apertura en la lista de apertura"));
    buffer_tools_box->addWidget(w11, 0);
    auto* lay12 = new QFormLayout();
    lay12->setContentsMargins(0, 0, 0, 0);
    lay12->setHorizontalSpacing(6);
    lay12->setVerticalSpacing(6);
    auto* w13 = new QLabel(nullptr);
    w13->setText(QString::fromUtf8("Dist. de buffer:"));
    lay12->setWidget(0, QFormLayout::ItemRole(0), w13);
    buffer_distance_entry = new FCDoubleSpinner(nullptr);
    buffer_distance_entry->setProperty("decimals", 4);
    buffer_distance_entry->setProperty("maximum", 10000.0);
    buffer_distance_entry->setProperty("minimum", -10000.0);
    buffer_distance_entry->setProperty("value", 0.1);
    lay12->setWidget(0, QFormLayout::ItemRole(1), buffer_distance_entry);
    buffer_corner_lbl = new FCLabel(nullptr);
    buffer_corner_lbl->setText(QString::fromUtf8("Rincón del búfer:"));
    buffer_corner_lbl->setToolTip(QString::fromUtf8("Hay 3 tipos de esquinas:\n - 'Redondo': la esquina es redondeada.\n - 'Cuadrado:' la esquina se encuentra en un ángulo agudo.\n - 'Biselado:' la esquina es una línea que conecta directamente las funciones que se encuentran en la esquina"));
    lay12->setWidget(1, QFormLayout::ItemRole(0), buffer_corner_lbl);
    buffer_corner_cb = new FCComboBox(nullptr);
    buffer_corner_cb->setProperty("currentText", QString::fromUtf8("Redondo"));
    buffer_corner_cb->addItem(QString::fromUtf8("Redondo"));
    buffer_corner_cb->addItem(QString::fromUtf8("Cuadrado"));
    buffer_corner_cb->addItem(QString::fromUtf8("Biselado"));
    lay12->setWidget(1, QFormLayout::ItemRole(1), buffer_corner_cb);
    auto* w14 = new QFrame(nullptr);
    w14->setProperty("frameShadow", 48);
    w14->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w14->setSizePolicy(sp); }
    lay12->setWidget(2, QFormLayout::ItemRole(2), w14);
    buffer_tools_box->addLayout(lay12, 0);
    auto* lay15 = new QHBoxLayout();
    lay15->setContentsMargins(0, 0, 0, 0);
    lay15->setSpacing(6);
    buffer_button = new FCButton(nullptr);
    buffer_button->setIcon(QIcon(QString::fromUtf8(":/images/buffer16-2.png")));
    buffer_button->setText(QString::fromUtf8("Buffer"));
    lay15->addWidget(buffer_button, 0);
    buffer_tools_box->addLayout(lay15, 0);
    buffer_tool_frame->setLayout(buffer_tools_box);
    custom_box->addWidget(buffer_tool_frame, 0);
    scale_tool_frame = new QFrame(nullptr);
    scale_tool_frame->setHidden(true);
    scale_tools_box = new QVBoxLayout();
    scale_tools_box->setContentsMargins(0, 0, 0, 0);
    scale_tools_box->setSpacing(6);
    auto* w16 = new FCLabel(nullptr);
    w16->setText(QString::fromUtf8("<b>Apertura de la escala:</b>"));
    w16->setToolTip(QString::fromUtf8("Escala una abertura en la lista de aperturas"));
    scale_tools_box->addWidget(w16, 0);
    auto* lay17 = new QFormLayout();
    lay17->setContentsMargins(0, 0, 0, 0);
    lay17->setHorizontalSpacing(6);
    lay17->setVerticalSpacing(6);
    scale_factor_lbl = new FCLabel(nullptr);
    scale_factor_lbl->setText(QString::fromUtf8("Factor de escala:"));
    scale_factor_lbl->setToolTip(QString::fromUtf8("El factor por el cual escalar la apertura seleccionada.\nLos valores pueden estar entre 0.0000 y 999.9999"));
    lay17->setWidget(0, QFormLayout::ItemRole(0), scale_factor_lbl);
    scale_factor_entry = new FCDoubleSpinner(nullptr);
    scale_factor_entry->setProperty("decimals", 4);
    scale_factor_entry->setProperty("maximum", 10000.0);
    scale_factor_entry->setProperty("value", 1.0);
    lay17->setWidget(0, QFormLayout::ItemRole(1), scale_factor_entry);
    auto* w18 = new QFrame(nullptr);
    w18->setProperty("frameShadow", 48);
    w18->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w18->setSizePolicy(sp); }
    lay17->setWidget(1, QFormLayout::ItemRole(2), w18);
    scale_tools_box->addLayout(lay17, 0);
    auto* lay19 = new QHBoxLayout();
    lay19->setContentsMargins(0, 0, 0, 0);
    lay19->setSpacing(6);
    scale_button = new FCButton(nullptr);
    scale_button->setIcon(QIcon(QString::fromUtf8(":/images/clean32.png")));
    scale_button->setText(QString::fromUtf8("Escala"));
    lay19->addWidget(scale_button, 0);
    scale_tools_box->addLayout(lay19, 0);
    scale_tool_frame->setLayout(scale_tools_box);
    custom_box->addWidget(scale_tool_frame, 0);
    ma_tool_frame = new QFrame(nullptr);
    ma_tool_frame->setHidden(true);
    ma_tools_box = new QVBoxLayout();
    ma_tools_box->setContentsMargins(0, 0, 0, 0);
    ma_tools_box->setSpacing(6);
    auto* w20 = new QFrame(nullptr);
    w20->setProperty("frameShadow", 48);
    w20->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w20->setSizePolicy(sp); }
    ma_tools_box->addWidget(w20, 0);
    auto* w21 = new FCLabel(nullptr);
    w21->setText(QString::fromUtf8("<b>Marcar polígonos:</b>"));
    w21->setToolTip(QString::fromUtf8("Marca las áreas del polígono."));
    ma_tools_box->addWidget(w21, 0);
    auto* lay22 = new QFormLayout();
    lay22->setContentsMargins(0, 0, 0, 0);
    lay22->setHorizontalSpacing(6);
    lay22->setVerticalSpacing(6);
    ma_lower_threshold_lbl = new FCLabel(nullptr);
    ma_lower_threshold_lbl->setText(QString::fromUtf8("Umbral inferior de la zona:"));
    ma_lower_threshold_lbl->setToolTip(QString::fromUtf8("El valor de umbral, todas las áreas más que esto están marcadas.\nPuede tener un valor entre 0.0000 y 10000.0000"));
    lay22->setWidget(0, QFormLayout::ItemRole(0), ma_lower_threshold_lbl);
    ma_lower_threshold_entry = new FCDoubleSpinner(nullptr);
    ma_lower_threshold_entry->setProperty("decimals", 4);
    ma_lower_threshold_entry->setProperty("maximum", 10000.0);
    lay22->setWidget(0, QFormLayout::ItemRole(1), ma_lower_threshold_entry);
    ma_upper_threshold_lbl = new FCLabel(nullptr);
    ma_upper_threshold_lbl->setText(QString::fromUtf8("Umbral SUPERIOR área:"));
    ma_upper_threshold_lbl->setToolTip(QString::fromUtf8("El valor de umbral, todas las áreas menos que esto están marcadas.\nPuede tener un valor entre 0.0000 y 10000.0000"));
    lay22->setWidget(1, QFormLayout::ItemRole(0), ma_upper_threshold_lbl);
    ma_upper_threshold_entry = new FCDoubleSpinner(nullptr);
    ma_upper_threshold_entry->setProperty("decimals", 4);
    ma_upper_threshold_entry->setProperty("maximum", 10000.0);
    ma_upper_threshold_entry->setProperty("value", 1.0);
    lay22->setWidget(1, QFormLayout::ItemRole(1), ma_upper_threshold_entry);
    ma_tools_box->addLayout(lay22, 0);
    auto* lay23 = new QHBoxLayout();
    lay23->setContentsMargins(0, 0, 0, 0);
    lay23->setSpacing(6);
    ma_threshold_button = new FCButton(nullptr);
    ma_threshold_button->setIcon(QIcon(QString::fromUtf8(":/images/markarea32.png")));
    ma_threshold_button->setText(QString::fromUtf8("Marque"));
    ma_threshold_button->setToolTip(QString::fromUtf8("Marque los polígonos que se ajustan dentro de los límites."));
    lay23->addWidget(ma_threshold_button, 0);
    ma_delete_button = new FCButton(nullptr);
    ma_delete_button->setIcon(QIcon(QString::fromUtf8(":/images/trash32.png")));
    ma_delete_button->setText(QString::fromUtf8("Borrar"));
    ma_delete_button->setToolTip(QString::fromUtf8("Eliminar todos los polígonos marcados."));
    lay23->addWidget(ma_delete_button, 0);
    ma_clear_button = new FCButton(nullptr);
    ma_clear_button->setIcon(QIcon(QString::fromUtf8(":/images/clean32.png")));
    ma_clear_button->setText(QString::fromUtf8("Limpiar"));
    ma_clear_button->setToolTip(QString::fromUtf8("Borra todas las marcas."));
    lay23->addWidget(ma_clear_button, 0);
    ma_tools_box->addLayout(lay23, 0);
    ma_tool_frame->setLayout(ma_tools_box);
    custom_box->addWidget(ma_tool_frame, 0);
    array_frame = new QFrame(nullptr);
    array_box = new QVBoxLayout();
    array_box->setContentsMargins(0, 0, 0, 0);
    array_box->setSpacing(6);
    auto* w24 = new QFrame(nullptr);
    w24->setProperty("frameShadow", 48);
    w24->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w24->setSizePolicy(sp); }
    array_box->addWidget(w24, 0);
    auto* lay25 = new QGridLayout();
    lay25->setContentsMargins(0, 0, 0, 0);
    lay25->setHorizontalSpacing(6);
    lay25->setVerticalSpacing(6);
    padarray_label = new FCLabel(nullptr);
    padarray_label->setText(QString::fromUtf8("<b>Agregar matriz de pad</b>"));
    padarray_label->setToolTip(QString::fromUtf8("Añadir una matriz de pads (lineal o circular)"));
    lay25->addWidget(padarray_label, 0, 0, 1, 2);
    auto* w26 = new FCLabel(nullptr);
    w26->setText(QString::fromUtf8("Tipo:"));
    w26->setToolTip(QString::fromUtf8("Seleccione el tipo de matriz de pads para crear.\nPuede ser Lineal X (Y) o Circular"));
    lay25->addWidget(w26, 2, 0, 1, 1);
    array_type_radio = new RadioSet({{QString::fromUtf8("Lineal"), QVariant(QString::fromUtf8("linear"))}, {QString::fromUtf8("Circular"), QVariant(QString::fromUtf8("circular"))}}, Qt::Horizontal, true, nullptr);
    array_type_radio->setValue(QVariant(QString::fromUtf8("linear")));
    lay25->addWidget(array_type_radio, 2, 1, 1, 1);
    auto* w27 = new FCLabel(nullptr);
    w27->setText(QString::fromUtf8("Nº de almohadillas:"));
    w27->setToolTip(QString::fromUtf8("Especifique cuántos pads estarán en la matriz."));
    lay25->addWidget(w27, 4, 0, 1, 1);
    pad_array_size_entry = new FCSpinner(nullptr);
    pad_array_size_entry->setProperty("maximum", 10000);
    pad_array_size_entry->setProperty("minimum", 1);
    pad_array_size_entry->setProperty("value", 5);
    lay25->addWidget(pad_array_size_entry, 4, 1, 1, 1);
    lin_separator_line = new QFrame(nullptr);
    lin_separator_line->setProperty("frameShadow", 48);
    lin_separator_line->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); lin_separator_line->setSizePolicy(sp); }
    lay25->addWidget(lin_separator_line, 6, 0, 1, 2);
    pad_axis_label = new FCLabel(nullptr);
    pad_axis_label->setText(QString::fromUtf8("Dirección:"));
    pad_axis_label->setToolTip(QString::fromUtf8("Dirección en la que se orienta la matriz lineal:\n- 'X' - eje horizontal\n- 'Y' - eje vertical o\n- 'Ángulo': un ángulo personalizado para la inclinación de la matriz"));
    lay25->addWidget(pad_axis_label, 8, 0, 1, 1);
    pad_axis_radio = new RadioSet({{QString::fromUtf8("X"), QVariant(QString::fromUtf8("X"))}, {QString::fromUtf8("Y"), QVariant(QString::fromUtf8("Y"))}, {QString::fromUtf8("Ángulo"), QVariant(QString::fromUtf8("A"))}}, Qt::Horizontal, true, nullptr);
    pad_axis_radio->setValue(QVariant(QString::fromUtf8("X")));
    lay25->addWidget(pad_axis_radio, 8, 1, 1, 1);
    pad_pitch_label = new FCLabel(nullptr);
    pad_pitch_label->setText(QString::fromUtf8("Paso:"));
    pad_pitch_label->setToolTip(QString::fromUtf8("Paso = Distancia entre elementos de la matriz."));
    lay25->addWidget(pad_pitch_label, 10, 0, 1, 1);
    pad_pitch_entry = new FCDoubleSpinner(nullptr);
    pad_pitch_entry->setProperty("decimals", 4);
    pad_pitch_entry->setProperty("maximum", 10000.0);
    pad_pitch_entry->setProperty("singleStep", 0.1);
    pad_pitch_entry->setProperty("value", 0.1);
    lay25->addWidget(pad_pitch_entry, 10, 1, 1, 1);
    linear_angle_label = new FCLabel(nullptr);
    linear_angle_label->setText(QString::fromUtf8("Ángulo:"));
    linear_angle_label->setToolTip(QString::fromUtf8("Ángulo en el que se coloca la matriz lineal.\nLa precisión es de max 2 decimales.\nEl valor mínimo es: -360.00 grados.\nEl valor máximo es: 360.00 grados."));
    linear_angle_label->setHidden(true);
    lay25->addWidget(linear_angle_label, 12, 0, 1, 1);
    linear_angle_spinner = new FCDoubleSpinner(nullptr);
    linear_angle_spinner->setProperty("decimals", 4);
    linear_angle_spinner->setProperty("maximum", 360.0);
    linear_angle_spinner->setProperty("minimum", -360.0);
    linear_angle_spinner->setHidden(true);
    lay25->addWidget(linear_angle_spinner, 12, 1, 1, 1);
    circ_separator_line = new QFrame(nullptr);
    circ_separator_line->setProperty("frameShadow", 48);
    circ_separator_line->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); circ_separator_line->setSizePolicy(sp); }
    circ_separator_line->setHidden(true);
    lay25->addWidget(circ_separator_line, 14, 0, 1, 2);
    pad_direction_label = new FCLabel(nullptr);
    pad_direction_label->setText(QString::fromUtf8("Dirección:"));
    pad_direction_label->setToolTip(QString::fromUtf8("Dirección para matriz circular.\nPuede ser CW = en sentido horario o CCW = en sentido antihorario."));
    pad_direction_label->setHidden(true);
    lay25->addWidget(pad_direction_label, 16, 0, 1, 1);
    pad_direction_radio = new RadioSet({{QString::fromUtf8("CW"), QVariant(QString::fromUtf8("CW"))}, {QString::fromUtf8("CCW"), QVariant(QString::fromUtf8("CCW"))}}, Qt::Horizontal, true, nullptr);
    pad_direction_radio->setHidden(true);
    pad_direction_radio->setValue(QVariant(QString::fromUtf8("CW")));
    lay25->addWidget(pad_direction_radio, 16, 1, 1, 1);
    pad_angle_label = new FCLabel(nullptr);
    pad_angle_label->setText(QString::fromUtf8("Ángulo:"));
    pad_angle_label->setToolTip(QString::fromUtf8("Ángulo en el que se coloca cada elemento de la matriz circular."));
    pad_angle_label->setHidden(true);
    lay25->addWidget(pad_angle_label, 18, 0, 1, 1);
    pad_angle_entry = new FCDoubleSpinner(nullptr);
    pad_angle_entry->setProperty("decimals", 4);
    pad_angle_entry->setProperty("maximum", 360.0);
    pad_angle_entry->setProperty("minimum", -360.0);
    pad_angle_entry->setProperty("singleStep", 0.1);
    pad_angle_entry->setHidden(true);
    lay25->addWidget(pad_angle_entry, 18, 1, 1, 1);
    lay25->setColumnStretch(1, 1);
    array_box->addLayout(lay25, 0);
    array_frame->setLayout(array_box);
    custom_box->addWidget(array_frame, 0);
    custom_box->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    lay1->addLayout(custom_box, 0);
    lay1->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    exit_editor_button = new FCButton(nullptr);
    { QFont f = exit_editor_button->font(); f.setBold(true); exit_editor_button->setFont(f); }
    exit_editor_button->setIcon(QIcon(QString::fromUtf8(":/images/power16.png")));
    exit_editor_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t\t\t  QPushButton\n\t\t\t\t\t\t\t\t\t\t\t  {\n\t\t\t\t\t\t\t\t\t\t\t\t  font-weight: bold;\n\t\t\t\t\t\t\t\t\t\t\t  }\n\t\t\t\t\t\t\t\t\t\t\t  "));
    exit_editor_button->setText(QString::fromUtf8("Salir del editor"));
    exit_editor_button->setToolTip(QString::fromUtf8("Salida del editor."));
    lay1->addWidget(exit_editor_button, 0);
    setLayout(lay1);
}

std::map<std::string, QObject*> GrbEditorUIBase::membersByName() const {
    return {
        {"addaperture_btn", addaperture_btn},
        {"apcode_entry", apcode_entry},
        {"apdim_entry", apdim_entry},
        {"apdim_lbl", apdim_lbl},
        {"apertures_box", apertures_box},
        {"apertures_frame", apertures_frame},
        {"apertures_table", apertures_table},
        {"apertures_table_label", apertures_table_label},
        {"apsize_entry", apsize_entry},
        {"aptype_cb", aptype_cb},
        {"array_box", array_box},
        {"array_frame", array_frame},
        {"array_type_radio", array_type_radio},
        {"buffer_button", buffer_button},
        {"buffer_corner_cb", buffer_corner_cb},
        {"buffer_corner_lbl", buffer_corner_lbl},
        {"buffer_distance_entry", buffer_distance_entry},
        {"buffer_tool_frame", buffer_tool_frame},
        {"buffer_tools_box", buffer_tools_box},
        {"circ_separator_line", circ_separator_line},
        {"custom_box", custom_box},
        {"delaperture_btn", delaperture_btn},
        {"exit_editor_button", exit_editor_button},
        {"icon", icon},
        {"lin_separator_line", lin_separator_line},
        {"linear_angle_label", linear_angle_label},
        {"linear_angle_spinner", linear_angle_spinner},
        {"ma_clear_button", ma_clear_button},
        {"ma_delete_button", ma_delete_button},
        {"ma_lower_threshold_entry", ma_lower_threshold_entry},
        {"ma_lower_threshold_lbl", ma_lower_threshold_lbl},
        {"ma_threshold_button", ma_threshold_button},
        {"ma_tool_frame", ma_tool_frame},
        {"ma_tools_box", ma_tools_box},
        {"ma_upper_threshold_entry", ma_upper_threshold_entry},
        {"ma_upper_threshold_lbl", ma_upper_threshold_lbl},
        {"name_box", name_box},
        {"name_entry", name_entry},
        {"pad_angle_entry", pad_angle_entry},
        {"pad_angle_label", pad_angle_label},
        {"pad_array_size_entry", pad_array_size_entry},
        {"pad_axis_label", pad_axis_label},
        {"pad_axis_radio", pad_axis_radio},
        {"pad_direction_label", pad_direction_label},
        {"pad_direction_radio", pad_direction_radio},
        {"pad_pitch_entry", pad_pitch_entry},
        {"pad_pitch_label", pad_pitch_label},
        {"padarray_label", padarray_label},
        {"scale_button", scale_button},
        {"scale_factor_entry", scale_factor_entry},
        {"scale_factor_lbl", scale_factor_lbl},
        {"scale_tool_frame", scale_tool_frame},
        {"scale_tools_box", scale_tools_box},
        {"title_box", title_box},
        {"title_label", title_label},
    };
}

}  // namespace flatcam::gui
