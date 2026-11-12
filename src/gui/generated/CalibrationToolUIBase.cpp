// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/CalibrationToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void CalibrationToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Herramienta de Calibración"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new QLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Herramienta de Calibración"));
    layout->addWidget(w1, 0);
    auto* w2 = new QLabel(nullptr);
    layout->addWidget(w2, 0);
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    gcode_title_label = new QLabel(nullptr);
    gcode_title_label->setText(QString::fromUtf8("<b>Parámetros:</b>"));
    gcode_title_label->setToolTip(QString::fromUtf8("Parámetros utilizados al crear el GCode en esta herramienta."));
    lay3->addWidget(gcode_title_label, 0, 0, 1, 3);
    auto* w4 = new QLabel(nullptr);
    w4->setText(QString::fromUtf8("Viaje Z:"));
    w4->setToolTip(QString::fromUtf8("Altura (Z) para viajar entre los puntos."));
    lay3->addWidget(w4, 1, 0, 1, 1);
    travelz_entry = new FCDoubleSpinner(nullptr);
    travelz_entry->setProperty("decimals", 4);
    travelz_entry->setProperty("maximum", 10000.0);
    travelz_entry->setProperty("minimum", -10000.0);
    travelz_entry->setProperty("singleStep", 0.1);
    travelz_entry->setProperty("value", 2.0);
    lay3->addWidget(travelz_entry, 1, 1, 1, 2);
    auto* w5 = new QLabel(nullptr);
    w5->setText(QString::fromUtf8("Verificación Z:"));
    w5->setToolTip(QString::fromUtf8("Altura (Z) para verificar el punto."));
    lay3->addWidget(w5, 2, 0, 1, 1);
    verz_entry = new FCDoubleSpinner(nullptr);
    verz_entry->setProperty("decimals", 4);
    verz_entry->setProperty("maximum", 10000.0);
    verz_entry->setProperty("minimum", -10000.0);
    verz_entry->setProperty("singleStep", 0.1);
    verz_entry->setProperty("value", 0.1);
    lay3->addWidget(verz_entry, 2, 1, 1, 2);
    zeroz_cb = new FCCheckBox(nullptr);
    zeroz_cb->setText(QString::fromUtf8("Cero la Z para Herram."));
    zeroz_cb->setToolTip(QString::fromUtf8("Incluya una secuencia para poner a cero la altura (Z)\nde la herramienta de verificación."));
    lay3->addWidget(zeroz_cb, 3, 0, 1, 3);
    auto* w6 = new QLabel(nullptr);
    w6->setEnabled(false);
    w6->setText(QString::fromUtf8("Cambio de herramienta Z:"));
    w6->setToolTip(QString::fromUtf8("Altura (Z) para montar la sonda de verificación."));
    lay3->addWidget(w6, 4, 0, 1, 1);
    toolchangez_entry = new FCDoubleSpinner(nullptr);
    toolchangez_entry->setProperty("decimals", 4);
    toolchangez_entry->setEnabled(false);
    toolchangez_entry->setProperty("maximum", 10000.0);
    toolchangez_entry->setProperty("singleStep", 0.1);
    toolchangez_entry->setProperty("value", 15.0);
    lay3->addWidget(toolchangez_entry, 4, 1, 1, 2);
    auto* w7 = new QLabel(nullptr);
    w7->setEnabled(false);
    w7->setText(QString::fromUtf8("Cambio de herra X, Y:"));
    w7->setToolTip(QString::fromUtf8("Posición de cambio de herramienta X, Y.\nSi no se ingresa ningún valor, entonces el actual\n(x, y) se utilizará el punto,"));
    lay3->addWidget(w7, 5, 0, 1, 1);
    toolchange_xy_entry = new FCEntry(nullptr);
    toolchange_xy_entry->setEnabled(false);
    lay3->addWidget(toolchange_xy_entry, 5, 1, 1, 2);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    lay3->addWidget(w8, 6, 0, 1, 3);
    auto* w9 = new QLabel(nullptr);
    w9->setText(QString::fromUtf8("Segundo punto:"));
    w9->setToolTip(QString::fromUtf8("El segundo punto en la verificación de Gcode puede ser:\n- arriba a la izquierda -> el usuario alineará la PCB verticalmente\n- abajo a la derecha -> el usuario alineará la PCB horizontalmente"));
    lay3->addWidget(w9, 7, 0, 1, 1);
    second_point_radio = new RadioSet({{QString::fromUtf8("Arriba a la izquierda"), QVariant(QString::fromUtf8("tl"))}, {QString::fromUtf8("Abajo a la derecha"), QVariant(QString::fromUtf8("br"))}}, Qt::Vertical, true, nullptr);
    second_point_radio->setValue(QVariant(QString::fromUtf8("tl")));
    lay3->addWidget(second_point_radio, 7, 1, 1, 2);
    auto* w10 = new QFrame(nullptr);
    w10->setProperty("frameShadow", 48);
    w10->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w10->setSizePolicy(sp); }
    lay3->addWidget(w10, 8, 0, 1, 3);
    auto* w11 = new QLabel(nullptr);
    lay3->addWidget(w11, 9, 0, 1, 3);
    auto* w12 = new QLabel(nullptr);
    w12->setText(QString::fromUtf8("<b>PASO 1: Adquiera puntos de calibración</b>"));
    w12->setToolTip(QString::fromUtf8("Elija cuatro puntos haciendo clic en el lienzo.\nEsos cuatro puntos deberían estar en los cuatro\n(tanto como sea posible) esquinas del objeto."));
    lay3->addWidget(w12, 10, 0, 1, 3);
    cal_source_lbl = new QLabel(nullptr);
    cal_source_lbl->setText(QString::fromUtf8("<b>Tipo de Fuente:</b>"));
    cal_source_lbl->setToolTip(QString::fromUtf8("La fuente de los puntos de calibración.\nPuede ser:\n- Objeto -> haga clic en un agujero geo para Excellon o una almohadilla para Gerber\n- Libre -> haga clic libremente en el lienzo para adquirir los puntos de calibración"));
    lay3->addWidget(cal_source_lbl, 11, 0, 1, 1);
    cal_source_radio = new RadioSet({{QString::fromUtf8("Objeto"), QVariant(QString::fromUtf8("object"))}, {QString::fromUtf8("Libre"), QVariant(QString::fromUtf8("free"))}}, Qt::Horizontal, false, nullptr);
    cal_source_radio->setValue(QVariant(QString::fromUtf8("object")));
    lay3->addWidget(cal_source_radio, 11, 1, 1, 2);
    obj_type_label = new QLabel(nullptr);
    obj_type_label->setText(QString::fromUtf8("Tipo de objeto:"));
    lay3->addWidget(obj_type_label, 12, 0, 1, 1);
    obj_type_combo = new FCComboBox(nullptr);
    obj_type_combo->setProperty("currentText", QString::fromUtf8("Excellon"));
    obj_type_combo->addItem(QString::fromUtf8("Gerber"));
    obj_type_combo->addItem(QString::fromUtf8("Excellon"));
    obj_type_combo->setCurrentIndex(1);
    lay3->addWidget(obj_type_combo, 12, 1, 1, 2);
    object_label = new QLabel(nullptr);
    object_label->setText(QString::fromUtf8("Selección de objeto de origen:"));
    object_label->setToolTip(QString::fromUtf8("Objeto FlatCAM que se utilizará como fuente de puntos de referencia."));
    lay3->addWidget(object_label, 13, 0, 1, 3);
    object_combo = new FCComboBox(nullptr);
    lay3->addWidget(object_combo, 14, 0, 1, 3);
    points_table_label = new QLabel(nullptr);
    points_table_label->setText(QString::fromUtf8("<b>Puntos de calibración</b>"));
    points_table_label->setToolTip(QString::fromUtf8("Contiene los puntos de calibración esperados y el\nlos medidos."));
    lay3->addWidget(points_table_label, 15, 0, 1, 3);
    points_table = new FCTable(nullptr);
    points_table->setProperty("columnCount", 4);
    points_table->setMaximumSize(QSize(16777215, 259));
    points_table->setMinimumSize(QSize(0, 258));
    points_table->setProperty("rowCount", 8);
    points_table->setProperty("selectionBehavior", 1);
    points_table->setProperty("sizeAdjustPolicy", 2);
    points_table->setProperty("verticalScrollBarPolicy", 1);
    points_table->setColumnCount(4);
    points_table->setHorizontalHeaderLabels({QString::fromUtf8("#"), QString::fromUtf8("Nombre"), QString::fromUtf8("Objetivo"), QString::fromUtf8("Delta encontrado")});
    points_table->verticalHeader()->setVisible(false);
    points_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode(2));
    points_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode(2));
    points_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeMode(1));
    points_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeMode(1));
    lay3->addWidget(points_table, 16, 0, 1, 3);
    start_button = new QPushButton(nullptr);
    { QFont f = start_button->font(); f.setBold(true); start_button->setFont(f); }
    start_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    start_button->setText(QString::fromUtf8("Obtener puntos"));
    start_button->setToolTip(QString::fromUtf8("Elija cuatro puntos haciendo clic en el lienzo si la opción de origen\nes 'libre' o está dentro de la geometría del objeto si la fuente es 'objeto'.\nEsos cuatro puntos deben estar en los cuatro cuadrados de\nel objeto."));
    lay3->addWidget(start_button, 17, 0, 1, 3);
    auto* w13 = new QFrame(nullptr);
    w13->setProperty("frameShadow", 48);
    w13->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w13->setSizePolicy(sp); }
    lay3->addWidget(w13, 18, 0, 1, 3);
    auto* w14 = new QLabel(nullptr);
    lay3->addWidget(w14, 19, 0, 1, 1);
    auto* w15 = new QLabel(nullptr);
    w15->setText(QString::fromUtf8("<b>PASO 2: Verificación GCode</b>"));
    w15->setToolTip(QString::fromUtf8("Genere un archivo GCode para localizar y alinear la PCB utilizando\nLos cuatro puntos adquiridos anteriormente.\nLa secuencia de puntos es:\n- primer punto -> establecer el origen\n- segundo punto -> punto de alineación. Puede ser: arriba a la izquierda o abajo a la derecha.\n- tercer punto -> punto de control. Puede ser: arriba a la izquierda o abajo a la derecha.\n- cuarto punto -> punto de verificación final. Solo para evaluación."));
    lay3->addWidget(w15, 20, 0, 1, 3);
    gcode_button = new QPushButton(nullptr);
    { QFont f = gcode_button->font(); f.setBold(true); gcode_button->setFont(f); }
    gcode_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    gcode_button->setText(QString::fromUtf8("Generar GCode"));
    gcode_button->setToolTip(QString::fromUtf8("Genere un archivo GCode para localizar y alinear la PCB utilizando\nLos cuatro puntos adquiridos anteriormente.\nLa secuencia de puntos es:\n- primer punto -> establecer el origen\n- segundo punto -> punto de alineación. Puede ser: arriba a la izquierda o abajo a la derecha.\n- tercer punto -> punto de control. Puede ser: arriba a la izquierda o abajo a la derecha.\n- cuarto punto -> punto de verificación final. Solo para evaluación."));
    lay3->addWidget(gcode_button, 21, 0, 1, 3);
    auto* w16 = new QFrame(nullptr);
    w16->setProperty("frameShadow", 48);
    w16->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w16->setSizePolicy(sp); }
    lay3->addWidget(w16, 22, 0, 1, 3);
    auto* w17 = new QLabel(nullptr);
    lay3->addWidget(w17, 23, 0, 1, 3);
    auto* w18 = new QLabel(nullptr);
    w18->setText(QString::fromUtf8("<b>PASO 3: Ajustes</b>"));
    w18->setToolTip(QString::fromUtf8("Calcular factores de escala y sesgo basados en las diferencias (delta)\nencontrado al verificar el patrón de PCB. Las diferencias deben llenarse\nen los campos encontrados (Delta)."));
    lay3->addWidget(w18, 24, 0, 1, 3);
    generate_factors_button = new QPushButton(nullptr);
    { QFont f = generate_factors_button->font(); f.setBold(true); generate_factors_button->setFont(f); }
    generate_factors_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    generate_factors_button->setText(QString::fromUtf8("Calcular factores"));
    generate_factors_button->setToolTip(QString::fromUtf8("Calcular factores de escala y sesgo basados en las diferencias (delta)\nencontrado al verificar el patrón de PCB. Las diferencias deben llenarse\nen los campos encontrados (Delta)."));
    lay3->addWidget(generate_factors_button, 25, 0, 1, 3);
    auto* w19 = new QFrame(nullptr);
    w19->setProperty("frameShadow", 48);
    w19->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w19->setSizePolicy(sp); }
    lay3->addWidget(w19, 26, 0, 1, 3);
    auto* w20 = new QLabel(nullptr);
    lay3->addWidget(w20, 27, 0, 1, 3);
    auto* w21 = new QLabel(nullptr);
    w21->setText(QString::fromUtf8("<b>PASO 4: Código GC ajustado</b>"));
    w21->setToolTip(QString::fromUtf8("Generar un archivo GCode de verificación ajustado con\nLos factores anteriores."));
    lay3->addWidget(w21, 28, 0, 1, 3);
    scalex_label = new QLabel(nullptr);
    scalex_label->setText(QString::fromUtf8("Factor de escala X:"));
    scalex_label->setToolTip(QString::fromUtf8("Factor para la acción de escala sobre el eje X."));
    lay3->addWidget(scalex_label, 29, 0, 1, 1);
    scalex_entry = new FCDoubleSpinner(nullptr);
    scalex_entry->setProperty("decimals", 4);
    scalex_entry->setProperty("maximum", 10000.0);
    scalex_entry->setProperty("singleStep", 0.1);
    scalex_entry->setProperty("value", 1.0);
    lay3->addWidget(scalex_entry, 29, 1, 1, 2);
    scaley_label = new QLabel(nullptr);
    scaley_label->setText(QString::fromUtf8("Factor de escala Y:"));
    scaley_label->setToolTip(QString::fromUtf8("Factor de acción de escala sobre eje Y."));
    lay3->addWidget(scaley_label, 30, 0, 1, 1);
    scaley_entry = new FCDoubleSpinner(nullptr);
    scaley_entry->setProperty("decimals", 4);
    scaley_entry->setProperty("maximum", 10000.0);
    scaley_entry->setProperty("singleStep", 0.1);
    scaley_entry->setProperty("value", 1.0);
    lay3->addWidget(scaley_entry, 30, 1, 1, 2);
    scale_button = new QPushButton(nullptr);
    { QFont f = scale_button->font(); f.setBold(true); scale_button->setFont(f); }
    scale_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t  QPushButton\n\t\t\t\t\t\t\t\t\t  {\n\t\t\t\t\t\t\t\t\t\t  font-weight: bold;\n\t\t\t\t\t\t\t\t\t  }\n\t\t\t\t\t\t\t\t\t  "));
    scale_button->setText(QString::fromUtf8("Aplicar factores de escala"));
    scale_button->setToolTip(QString::fromUtf8("Aplicar factores de escala en los puntos de calibración."));
    lay3->addWidget(scale_button, 31, 0, 1, 3);
    skewx_label = new QLabel(nullptr);
    skewx_label->setText(QString::fromUtf8("Ángulo de Inclinar X:"));
    skewx_label->setToolTip(QString::fromUtf8("Ángulo, en grados.\nNúmero Real entre -360 y 359."));
    lay3->addWidget(skewx_label, 32, 0, 1, 1);
    skewx_entry = new FCDoubleSpinner(nullptr);
    skewx_entry->setProperty("decimals", 4);
    skewx_entry->setProperty("maximum", 360.0);
    skewx_entry->setProperty("minimum", -360.0);
    skewx_entry->setProperty("singleStep", 0.1);
    lay3->addWidget(skewx_entry, 32, 1, 1, 2);
    skewy_label = new QLabel(nullptr);
    skewy_label->setText(QString::fromUtf8("Ángulo de Inclinar Y:"));
    skewy_label->setToolTip(QString::fromUtf8("Ángulo, en grados.\nNúmero Real entre -360 y 359."));
    lay3->addWidget(skewy_label, 33, 0, 1, 1);
    skewy_entry = new FCDoubleSpinner(nullptr);
    skewy_entry->setProperty("decimals", 4);
    skewy_entry->setProperty("maximum", 360.0);
    skewy_entry->setProperty("minimum", -360.0);
    skewy_entry->setProperty("singleStep", 0.1);
    lay3->addWidget(skewy_entry, 33, 1, 1, 2);
    skew_button = new QPushButton(nullptr);
    { QFont f = skew_button->font(); f.setBold(true); skew_button->setFont(f); }
    skew_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t  QPushButton\n\t\t\t\t\t\t\t\t\t  {\n\t\t\t\t\t\t\t\t\t\t  font-weight: bold;\n\t\t\t\t\t\t\t\t\t  }\n\t\t\t\t\t\t\t\t\t  "));
    skew_button->setText(QString::fromUtf8("Aplicar factores Sesgados"));
    skew_button->setToolTip(QString::fromUtf8("Aplicar factores de inclinación en los puntos de calibración."));
    lay3->addWidget(skew_button, 34, 0, 1, 3);
    adj_gcode_button = new QPushButton(nullptr);
    { QFont f = adj_gcode_button->font(); f.setBold(true); adj_gcode_button->setFont(f); }
    adj_gcode_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    adj_gcode_button->setText(QString::fromUtf8("Generar código GC ajustado"));
    adj_gcode_button->setToolTip(QString::fromUtf8("Generar un archivo GCode de verificación ajustado con\nLos factores establecidos anteriormente.\nLos parámetros GCode se pueden reajustar\nantes de hacer clic en este botón."));
    lay3->addWidget(adj_gcode_button, 42, 0, 1, 3);
    auto* w22 = new QFrame(nullptr);
    w22->setProperty("frameShadow", 48);
    w22->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w22->setSizePolicy(sp); }
    lay3->addWidget(w22, 43, 0, 1, 3);
    auto* w23 = new QLabel(nullptr);
    lay3->addWidget(w23, 44, 0, 1, 3);
    auto* w24 = new QLabel(nullptr);
    w24->setText(QString::fromUtf8("<b>PASO 5: Calibrar objetos FlatCAM</b>"));
    w24->setToolTip(QString::fromUtf8("Ajuste los objetos FlatCAM\ncon los factores determinados y verificados anteriormente."));
    lay3->addWidget(w24, 45, 0, 1, 3);
    adj_object_type_label = new QLabel(nullptr);
    adj_object_type_label->setText(QString::fromUtf8("Tipo de objeto ajustado:"));
    adj_object_type_label->setToolTip(QString::fromUtf8("Tipo del objeto FlatCAM que se ajustará."));
    lay3->addWidget(adj_object_type_label, 46, 0, 1, 3);
    adj_object_type_combo = new FCComboBox(nullptr);
    adj_object_type_combo->setProperty("currentText", QString::fromUtf8("Gerber"));
    adj_object_type_combo->addItem(QString::fromUtf8("Gerber"));
    adj_object_type_combo->addItem(QString::fromUtf8("Excellon"));
    adj_object_type_combo->addItem(QString::fromUtf8("Geometría"));
    lay3->addWidget(adj_object_type_combo, 47, 0, 1, 3);
    adj_object_label = new QLabel(nullptr);
    adj_object_label->setText(QString::fromUtf8("Selección de objeto ajustada:"));
    adj_object_label->setToolTip(QString::fromUtf8("El objeto FlatCAM a ajustar."));
    lay3->addWidget(adj_object_label, 48, 0, 1, 3);
    adj_object_combo = new FCComboBox(nullptr);
    lay3->addWidget(adj_object_combo, 49, 0, 1, 3);
    cal_button = new QPushButton(nullptr);
    { QFont f = cal_button->font(); f.setBold(true); cal_button->setFont(f); }
    cal_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    cal_button->setText(QString::fromUtf8("Calibrar"));
    cal_button->setToolTip(QString::fromUtf8("Ajustar (escalar y / o sesgar) los objetos\ncon los factores determinados anteriormente."));
    lay3->addWidget(cal_button, 50, 0, 1, 3);
    auto* w25 = new QFrame(nullptr);
    w25->setProperty("frameShadow", 48);
    w25->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w25->setSizePolicy(sp); }
    lay3->addWidget(w25, 51, 0, 1, 3);
    auto* w26 = new QLabel(nullptr);
    lay3->addWidget(w26, 52, 0, 1, 3);
    lay3->setColumnStretch(1, 1);
    layout->addLayout(lay3, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new QPushButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    reset_button->setText(QString::fromUtf8("Restablecer la Herramienta"));
    reset_button->setToolTip(QString::fromUtf8("Restablecerá los parámetros de la herramienta."));
    layout->addWidget(reset_button, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> CalibrationToolUIBase::membersByName() const {
    return {
        {"adj_gcode_button", adj_gcode_button},
        {"adj_object_combo", adj_object_combo},
        {"adj_object_label", adj_object_label},
        {"adj_object_type_combo", adj_object_type_combo},
        {"adj_object_type_label", adj_object_type_label},
        {"cal_button", cal_button},
        {"cal_source_lbl", cal_source_lbl},
        {"cal_source_radio", cal_source_radio},
        {"gcode_button", gcode_button},
        {"gcode_title_label", gcode_title_label},
        {"generate_factors_button", generate_factors_button},
        {"layout", layout},
        {"obj_type_combo", obj_type_combo},
        {"obj_type_label", obj_type_label},
        {"object_combo", object_combo},
        {"object_label", object_label},
        {"points_table", points_table},
        {"points_table_label", points_table_label},
        {"reset_button", reset_button},
        {"scale_button", scale_button},
        {"scalex_entry", scalex_entry},
        {"scalex_label", scalex_label},
        {"scaley_entry", scaley_entry},
        {"scaley_label", scaley_label},
        {"second_point_radio", second_point_radio},
        {"skew_button", skew_button},
        {"skewx_entry", skewx_entry},
        {"skewx_label", skewx_label},
        {"skewy_entry", skewy_entry},
        {"skewy_label", skewy_label},
        {"start_button", start_button},
        {"toolchange_xy_entry", toolchange_xy_entry},
        {"toolchangez_entry", toolchangez_entry},
        {"travelz_entry", travelz_entry},
        {"verz_entry", verz_entry},
        {"zeroz_cb", zeroz_cb},
    };
}

}  // namespace flatcam::gui
