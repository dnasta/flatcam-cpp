// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/SolderPasteToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void SolderPasteToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Herra. de Pasta de Soldadura"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new QLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QLabel\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-size: 16px;\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    w1->setText(QString::fromUtf8("Herra. de Pasta de Soldadura"));
    layout->addWidget(w1, 0);
    auto* lay2 = new QFormLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    object_label = new QLabel(nullptr);
    object_label->setText(QString::fromUtf8("<b>GERBER</b>:"));
    object_label->setToolTip(QString::fromUtf8("Objeto de pasta de soldadura Gerber."));
    lay2->setWidget(0, QFormLayout::ItemRole(2), object_label);
    obj_combo = new FCComboBox(nullptr);
    lay2->setWidget(1, QFormLayout::ItemRole(2), obj_combo);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    lay2->setWidget(2, QFormLayout::ItemRole(2), w3);
    layout->addLayout(lay2, 0);
    tools_table_label = new QLabel(nullptr);
    tools_table_label->setText(QString::fromUtf8("<b>Tabla de herramientas</b>"));
    tools_table_label->setToolTip(QString::fromUtf8("Conjunto de herramientas desde el cual el algoritmo\nelegirá los que se usan para dispensar pasta de soldadura."));
    layout->addWidget(tools_table_label, 0);
    tools_table = new FCTable(nullptr);
    tools_table->setProperty("columnCount", 3);
    tools_table->setProperty("contextMenuPolicy", 2);
    tools_table->setMaximumSize(QSize(16777215, 72));
    tools_table->setMinimumSize(QSize(0, 72));
    tools_table->setProperty("rowCount", 2);
    tools_table->setProperty("verticalScrollBarPolicy", 1);
    tools_table->setColumnCount(3);
    tools_table->setHorizontalHeaderLabels({QString::fromUtf8("#"), QString::fromUtf8("Diámetro"), QString::fromUtf8("")});
    tools_table->verticalHeader()->setVisible(false);
    tools_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode(2));
    tools_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode(1));
    tools_table->setColumnHidden(2, true);
    layout->addWidget(tools_table, 0);
    auto* lay4 = new QHBoxLayout();
    lay4->setContentsMargins(0, 0, 0, 0);
    lay4->setSpacing(6);
    addtool_entry_lbl = new QLabel(nullptr);
    addtool_entry_lbl->setText(QString::fromUtf8("<b>Nueva herra. de boquilla:</b>"));
    addtool_entry_lbl->setToolTip(QString::fromUtf8("Diámetro de la nueva herramienta para agregar en la tabla de herramientas"));
    lay4->addWidget(addtool_entry_lbl, 0);
    addtool_entry = new FCDoubleSpinner(nullptr);
    addtool_entry->setProperty("decimals", 4);
    addtool_entry->setProperty("maximum", 10000.0);
    addtool_entry->setProperty("singleStep", 0.1);
    addtool_entry->setProperty("value", 0.3);
    lay4->addWidget(addtool_entry, 0);
    layout->addLayout(lay4, 0);
    auto* lay5 = new QGridLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setHorizontalSpacing(6);
    lay5->setVerticalSpacing(6);
    addtool_btn = new QPushButton(nullptr);
    addtool_btn->setText(QString::fromUtf8("Añadir"));
    addtool_btn->setToolTip(QString::fromUtf8("Agregue una nueva herramienta de boquilla a la tabla de herramientas\ncon el diámetro especificado anteriormente."));
    lay5->addWidget(addtool_btn, 0, 0, 1, 1);
    deltool_btn = new QPushButton(nullptr);
    deltool_btn->setText(QString::fromUtf8("Borrar"));
    deltool_btn->setToolTip(QString::fromUtf8("Eliminar una selección de herramientas en la tabla de herramientas\nseleccionando primero una fila en la Tabla de herramientas."));
    lay5->addWidget(deltool_btn, 0, 2, 1, 1);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    lay5->addWidget(w6, 1, 0, 1, 3);
    layout->addLayout(lay5, 0);
    auto* lay7 = new QGridLayout();
    lay7->setContentsMargins(0, 0, 0, 0);
    lay7->setHorizontalSpacing(6);
    lay7->setVerticalSpacing(6);
    auto* w8 = new QLabel(nullptr);
    w8->setText(QString::fromUtf8("<b>PASO 1:</b>"));
    w8->setToolTip(QString::fromUtf8("El primer paso es seleccionar una serie de herramientas de boquillas para su uso\ny luego opcionalmente modificar los parámetros GCode a continuación."));
    lay7->addWidget(w8, 0, 0, 1, 1, Qt::Alignment(32));
    auto* w9 = new QLabel(nullptr);
    w9->setText(QString::fromUtf8("Seleccionar herramientas.\nModificar parámetros."));
    lay7->addWidget(w9, 0, 2, 1, 1, Qt::Alignment(64));
    layout->addLayout(lay7, 0);
    gcode_frame = new QFrame(nullptr);
    gcode_box = new QVBoxLayout();
    gcode_box->setContentsMargins(0, 0, 0, 0);
    gcode_box->setSpacing(6);
    gcode_form_layout = new QFormLayout();
    gcode_form_layout->setContentsMargins(0, 0, 0, 0);
    gcode_form_layout->setHorizontalSpacing(6);
    gcode_form_layout->setVerticalSpacing(6);
    z_start_label = new QLabel(nullptr);
    z_start_label->setText(QString::fromUtf8("Inicio de dispen. Z:"));
    z_start_label->setToolTip(QString::fromUtf8("La altura (Z) cuando comienza la dispensación de pasta de soldadura."));
    gcode_form_layout->setWidget(0, QFormLayout::ItemRole(0), z_start_label);
    z_start_entry = new FCDoubleSpinner(nullptr);
    z_start_entry->setProperty("decimals", 4);
    z_start_entry->setProperty("maximum", 10000.0);
    z_start_entry->setProperty("singleStep", 0.1);
    z_start_entry->setProperty("value", 0.05);
    gcode_form_layout->setWidget(0, QFormLayout::ItemRole(1), z_start_entry);
    z_dispense_label = new QLabel(nullptr);
    z_dispense_label->setText(QString::fromUtf8("Dispensación Z:"));
    z_dispense_label->setToolTip(QString::fromUtf8("La altura (Z) al dispensar pasta de soldadura."));
    gcode_form_layout->setWidget(1, QFormLayout::ItemRole(0), z_dispense_label);
    z_dispense_entry = new FCDoubleSpinner(nullptr);
    z_dispense_entry->setProperty("decimals", 4);
    z_dispense_entry->setProperty("maximum", 10000.0);
    z_dispense_entry->setProperty("singleStep", 0.1);
    z_dispense_entry->setProperty("value", 0.1);
    gcode_form_layout->setWidget(1, QFormLayout::ItemRole(1), z_dispense_entry);
    z_stop_label = new QLabel(nullptr);
    z_stop_label->setText(QString::fromUtf8("Parada de dispen. Z:"));
    z_stop_label->setToolTip(QString::fromUtf8("La altura (Z) cuando se detiene la dispensación de pasta de soldadura."));
    gcode_form_layout->setWidget(2, QFormLayout::ItemRole(0), z_stop_label);
    z_stop_entry = new FCDoubleSpinner(nullptr);
    z_stop_entry->setProperty("decimals", 4);
    z_stop_entry->setProperty("maximum", 10000.0);
    z_stop_entry->setProperty("singleStep", 0.1);
    z_stop_entry->setProperty("value", 0.05);
    gcode_form_layout->setWidget(2, QFormLayout::ItemRole(1), z_stop_entry);
    z_travel_label = new QLabel(nullptr);
    z_travel_label->setText(QString::fromUtf8("Viajar Z:"));
    z_travel_label->setToolTip(QString::fromUtf8("La altura (Z) para viajar entre almohadillas\n(sin dispensar pasta de soldadura)."));
    gcode_form_layout->setWidget(3, QFormLayout::ItemRole(0), z_travel_label);
    z_travel_entry = new FCDoubleSpinner(nullptr);
    z_travel_entry->setProperty("decimals", 4);
    z_travel_entry->setProperty("maximum", 10000.0);
    z_travel_entry->setProperty("singleStep", 0.1);
    z_travel_entry->setProperty("value", 0.1);
    gcode_form_layout->setWidget(3, QFormLayout::ItemRole(1), z_travel_entry);
    z_toolchange_label = new QLabel(nullptr);
    z_toolchange_label->setText(QString::fromUtf8("Cambio de herra. Z:"));
    z_toolchange_label->setToolTip(QString::fromUtf8("La altura (Z) para el cambio de herramienta (boquilla)."));
    gcode_form_layout->setWidget(4, QFormLayout::ItemRole(0), z_toolchange_label);
    z_toolchange_entry = new FCDoubleSpinner(nullptr);
    z_toolchange_entry->setProperty("decimals", 4);
    z_toolchange_entry->setProperty("maximum", 10000.0);
    z_toolchange_entry->setProperty("singleStep", 0.1);
    z_toolchange_entry->setProperty("value", 1.0);
    gcode_form_layout->setWidget(4, QFormLayout::ItemRole(1), z_toolchange_entry);
    xy_toolchange_label = new QLabel(nullptr);
    xy_toolchange_label->setText(QString::fromUtf8("Cambio de herra X, Y:"));
    xy_toolchange_label->setToolTip(QString::fromUtf8("La ubicación X, Y para el cambio de herramienta (boquilla).\nEl formato es (x, y) donde x e y son números reales."));
    gcode_form_layout->setWidget(5, QFormLayout::ItemRole(0), xy_toolchange_label);
    xy_toolchange_entry = new FCEntry(nullptr);
    xy_toolchange_entry->setText(QString::fromUtf8("0.0, 0.0"));
    gcode_form_layout->setWidget(5, QFormLayout::ItemRole(1), xy_toolchange_entry);
    frxy_label = new QLabel(nullptr);
    frxy_label->setText(QString::fromUtf8("Avance X-Y:"));
    frxy_label->setToolTip(QString::fromUtf8("Avance (velocidad) mientras se mueve en el plano X-Y."));
    gcode_form_layout->setWidget(6, QFormLayout::ItemRole(0), frxy_label);
    frxy_entry = new FCDoubleSpinner(nullptr);
    frxy_entry->setProperty("decimals", 4);
    frxy_entry->setProperty("maximum", 910000.0);
    frxy_entry->setProperty("singleStep", 0.1);
    frxy_entry->setProperty("value", 150.0);
    gcode_form_layout->setWidget(6, QFormLayout::ItemRole(1), frxy_entry);
    frz_label = new QLabel(nullptr);
    frz_label->setText(QString::fromUtf8("Avance Z:"));
    frz_label->setToolTip(QString::fromUtf8("Avance (velocidad) mientras se mueve verticalmente\n(en el plano Z)."));
    gcode_form_layout->setWidget(7, QFormLayout::ItemRole(0), frz_label);
    frz_entry = new FCDoubleSpinner(nullptr);
    frz_entry->setProperty("decimals", 4);
    frz_entry->setProperty("maximum", 910000.0);
    frz_entry->setProperty("singleStep", 0.1);
    frz_entry->setProperty("value", 150.0);
    gcode_form_layout->setWidget(7, QFormLayout::ItemRole(1), frz_entry);
    frz_dispense_label = new QLabel(nullptr);
    frz_dispense_label->setText(QString::fromUtf8("Avance de Dispens. Z:"));
    frz_dispense_label->setToolTip(QString::fromUtf8("Avance (velocidad) mientras se mueve verticalmente\npara dispensar la posición (en el plano Z)."));
    gcode_form_layout->setWidget(8, QFormLayout::ItemRole(0), frz_dispense_label);
    frz_dispense_entry = new FCDoubleSpinner(nullptr);
    frz_dispense_entry->setProperty("decimals", 4);
    frz_dispense_entry->setProperty("maximum", 910000.0);
    frz_dispense_entry->setProperty("singleStep", 0.1);
    frz_dispense_entry->setProperty("value", 1.0);
    gcode_form_layout->setWidget(8, QFormLayout::ItemRole(1), frz_dispense_entry);
    speedfwd_label = new QLabel(nullptr);
    speedfwd_label->setText(QString::fromUtf8("Veloc. del husillo FWD:"));
    speedfwd_label->setToolTip(QString::fromUtf8("La velocidad del dispensador mientras empuja la pasta de soldadura\na través de la boquilla dispensadora."));
    gcode_form_layout->setWidget(9, QFormLayout::ItemRole(0), speedfwd_label);
    speedfwd_entry = new FCSpinner(nullptr);
    speedfwd_entry->setProperty("maximum", 999999);
    speedfwd_entry->setProperty("singleStep", 1000);
    speedfwd_entry->setProperty("value", 300);
    gcode_form_layout->setWidget(9, QFormLayout::ItemRole(1), speedfwd_entry);
    dwellfwd_label = new QLabel(nullptr);
    dwellfwd_label->setText(QString::fromUtf8("Morar FWD:"));
    dwellfwd_label->setToolTip(QString::fromUtf8("Pausa después de la dispensación de soldadura."));
    gcode_form_layout->setWidget(10, QFormLayout::ItemRole(0), dwellfwd_label);
    dwellfwd_entry = new FCDoubleSpinner(nullptr);
    dwellfwd_entry->setProperty("decimals", 4);
    dwellfwd_entry->setProperty("maximum", 10000.0);
    dwellfwd_entry->setProperty("singleStep", 0.1);
    dwellfwd_entry->setProperty("value", 1.0);
    gcode_form_layout->setWidget(10, QFormLayout::ItemRole(1), dwellfwd_entry);
    speedrev_label = new QLabel(nullptr);
    speedrev_label->setText(QString::fromUtf8("Veloc. del husillo REV:"));
    speedrev_label->setToolTip(QString::fromUtf8("La velocidad del dispensador mientras se retrae la pasta de soldadura\na través de la boquilla dispensadora."));
    gcode_form_layout->setWidget(11, QFormLayout::ItemRole(0), speedrev_label);
    speedrev_entry = new FCSpinner(nullptr);
    speedrev_entry->setProperty("maximum", 999999);
    speedrev_entry->setProperty("singleStep", 1000);
    speedrev_entry->setProperty("value", 200);
    gcode_form_layout->setWidget(11, QFormLayout::ItemRole(1), speedrev_entry);
    dwellrev_label = new QLabel(nullptr);
    dwellrev_label->setText(QString::fromUtf8("Morar REV:"));
    dwellrev_label->setToolTip(QString::fromUtf8("Pausa después de que el dispensador de pasta de soldadura se retraiga,\npara permitir el equilibrio de presión."));
    gcode_form_layout->setWidget(12, QFormLayout::ItemRole(0), dwellrev_label);
    dwellrev_entry = new FCDoubleSpinner(nullptr);
    dwellrev_entry->setProperty("decimals", 4);
    dwellrev_entry->setProperty("maximum", 10000.0);
    dwellrev_entry->setProperty("singleStep", 0.1);
    dwellrev_entry->setProperty("value", 1.0);
    gcode_form_layout->setWidget(12, QFormLayout::ItemRole(1), dwellrev_entry);
    auto* w10 = new QLabel(nullptr);
    w10->setText(QString::fromUtf8("Postprocesador:"));
    w10->setToolTip(QString::fromUtf8("Archivos que controlan la generación de GCode."));
    gcode_form_layout->setWidget(13, QFormLayout::ItemRole(0), w10);
    pp_combo = new FCComboBox(nullptr);
    gcode_form_layout->setWidget(13, QFormLayout::ItemRole(1), pp_combo);
    gcode_box->addLayout(gcode_form_layout, 0);
    gcode_frame->setLayout(gcode_box);
    layout->addWidget(gcode_frame, 0);
    generation_frame = new QFrame(nullptr);
    generation_box = new QVBoxLayout();
    generation_box->setContentsMargins(0, 0, 0, 0);
    generation_box->setSpacing(6);
    auto* lay11 = new QGridLayout();
    lay11->setContentsMargins(0, 0, 0, 0);
    lay11->setHorizontalSpacing(6);
    lay11->setVerticalSpacing(6);
    auto* w12 = new QLabel(nullptr);
    w12->setText(QString::fromUtf8("<b>PASO 2:</b>"));
    w12->setToolTip(QString::fromUtf8("El segundo paso es crear una dispensación de pasta de soldadura\ngeometría de un archivo Gerber de máscara de pasta de soldadura."));
    lay11->addWidget(w12, 0, 0, 1, 1);
    soldergeo_btn = new QPushButton(nullptr);
    { QFont f = soldergeo_btn->font(); f.setBold(true); soldergeo_btn->setFont(f); }
    soldergeo_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t\t\t\t\t   "));
    soldergeo_btn->setText(QString::fromUtf8("Generar Geo"));
    soldergeo_btn->setToolTip(QString::fromUtf8("Generar geometría de dispensación de pasta de soldadura."));
    lay11->addWidget(soldergeo_btn, 0, 2, 1, 1);
    generation_box->addLayout(lay11, 0);
    auto* lay13 = new QFormLayout();
    lay13->setContentsMargins(0, 0, 0, 0);
    lay13->setHorizontalSpacing(6);
    lay13->setVerticalSpacing(6);
    geo_object_label = new QLabel(nullptr);
    geo_object_label->setText(QString::fromUtf8("Resultado Geo:"));
    geo_object_label->setToolTip(QString::fromUtf8("Objeto de pasta de soldadura de geometría.\nEl nombre del objeto tiene que terminar en:\n'_solderpaste' como protección."));
    lay13->setWidget(0, QFormLayout::ItemRole(0), geo_object_label);
    geo_obj_combo = new FCComboBox(nullptr);
    lay13->setWidget(0, QFormLayout::ItemRole(1), geo_obj_combo);
    generation_box->addLayout(lay13, 0);
    auto* lay14 = new QGridLayout();
    lay14->setContentsMargins(0, 0, 0, 0);
    lay14->setHorizontalSpacing(6);
    lay14->setVerticalSpacing(6);
    auto* w15 = new QLabel(nullptr);
    w15->setText(QString::fromUtf8("<b>PASO 3:</b>"));
    w15->setToolTip(QString::fromUtf8("El tercer paso es seleccionar una geometría de distribución de pasta de soldadura,\ny luego generar un objeto CNCJob.\n\nRECUERDE: si desea crear un CNCJob con nuevos parámetros,\nprimero necesitas generar una geometría con esos nuevos parámetros,\ny solo después de eso puede generar un CNCJob actualizado."));
    lay14->addWidget(w15, 0, 0, 1, 1);
    solder_gcode_btn = new QPushButton(nullptr);
    { QFont f = solder_gcode_btn->font(); f.setBold(true); solder_gcode_btn->setFont(f); }
    solder_gcode_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    solder_gcode_btn->setText(QString::fromUtf8("Generar GCode"));
    solder_gcode_btn->setToolTip(QString::fromUtf8("Generar GCodelo para dispensar pasta de soldadura\nen almohadillas de PCB."));
    lay14->addWidget(solder_gcode_btn, 0, 2, 1, 1);
    generation_box->addLayout(lay14, 0);
    auto* lay16 = new QFormLayout();
    lay16->setContentsMargins(0, 0, 0, 0);
    lay16->setHorizontalSpacing(6);
    lay16->setVerticalSpacing(6);
    cnc_object_label = new QLabel(nullptr);
    cnc_object_label->setText(QString::fromUtf8("Resultado del CNC:"));
    cnc_object_label->setToolTip(QString::fromUtf8("CNCJob soldar pegar objeto.\nPara habilitar la sección de guardar GCode,\nel nombre del objeto debe terminar en:\n'_solderpaste' como protección."));
    lay16->setWidget(0, QFormLayout::ItemRole(0), cnc_object_label);
    cnc_obj_combo = new FCComboBox(nullptr);
    lay16->setWidget(0, QFormLayout::ItemRole(1), cnc_obj_combo);
    generation_box->addLayout(lay16, 0);
    auto* lay17 = new QGridLayout();
    lay17->setContentsMargins(0, 0, 0, 0);
    lay17->setHorizontalSpacing(6);
    lay17->setVerticalSpacing(6);
    auto* w18 = new QLabel(nullptr);
    w18->setText(QString::fromUtf8("<b>PASO 4:</b>"));
    w18->setToolTip(QString::fromUtf8("Cuarto paso (y último) es seleccionar un CNCJob hecho de\nuna geometría de dispensación de pasta de soldadura, y luego ver / guardar su código GC."));
    lay17->addWidget(w18, 0, 0, 1, 1);
    solder_gcode_view_btn = new QPushButton(nullptr);
    { QFont f = solder_gcode_view_btn->font(); f.setBold(true); solder_gcode_view_btn->setFont(f); }
    solder_gcode_view_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    solder_gcode_view_btn->setText(QString::fromUtf8("Ver GCode"));
    solder_gcode_view_btn->setToolTip(QString::fromUtf8("Ver el GCode generado para la dispensación de pasta de soldadura\nen almohadillas de PCB."));
    lay17->addWidget(solder_gcode_view_btn, 0, 2, 1, 1);
    solder_gcode_save_btn = new QPushButton(nullptr);
    { QFont f = solder_gcode_save_btn->font(); f.setBold(true); solder_gcode_save_btn->setFont(f); }
    solder_gcode_save_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t   QPushButton\n\t\t\t\t\t\t\t   {\n\t\t\t\t\t\t\t\t   font-weight: bold;\n\t\t\t\t\t\t\t   }\n\t\t\t\t\t\t\t   "));
    solder_gcode_save_btn->setText(QString::fromUtf8("Guardar GCode"));
    solder_gcode_save_btn->setToolTip(QString::fromUtf8("Guarde el GCode generado para la dispensación de pasta de soldadura\nen almohadillas de PCB, a un archivo."));
    lay17->addWidget(solder_gcode_save_btn, 1, 0, 1, 3);
    generation_box->addLayout(lay17, 0);
    generation_frame->setLayout(generation_box);
    layout->addWidget(generation_frame, 0);
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

std::map<std::string, QObject*> SolderPasteToolUIBase::membersByName() const {
    return {
        {"addtool_btn", addtool_btn},
        {"addtool_entry", addtool_entry},
        {"addtool_entry_lbl", addtool_entry_lbl},
        {"cnc_obj_combo", cnc_obj_combo},
        {"cnc_object_label", cnc_object_label},
        {"deltool_btn", deltool_btn},
        {"dwellfwd_entry", dwellfwd_entry},
        {"dwellfwd_label", dwellfwd_label},
        {"dwellrev_entry", dwellrev_entry},
        {"dwellrev_label", dwellrev_label},
        {"frxy_entry", frxy_entry},
        {"frxy_label", frxy_label},
        {"frz_dispense_entry", frz_dispense_entry},
        {"frz_dispense_label", frz_dispense_label},
        {"frz_entry", frz_entry},
        {"frz_label", frz_label},
        {"gcode_box", gcode_box},
        {"gcode_form_layout", gcode_form_layout},
        {"gcode_frame", gcode_frame},
        {"generation_box", generation_box},
        {"generation_frame", generation_frame},
        {"geo_obj_combo", geo_obj_combo},
        {"geo_object_label", geo_object_label},
        {"layout", layout},
        {"obj_combo", obj_combo},
        {"object_label", object_label},
        {"pp_combo", pp_combo},
        {"reset_button", reset_button},
        {"solder_gcode_btn", solder_gcode_btn},
        {"solder_gcode_save_btn", solder_gcode_save_btn},
        {"solder_gcode_view_btn", solder_gcode_view_btn},
        {"soldergeo_btn", soldergeo_btn},
        {"speedfwd_entry", speedfwd_entry},
        {"speedfwd_label", speedfwd_label},
        {"speedrev_entry", speedrev_entry},
        {"speedrev_label", speedrev_label},
        {"tools_table", tools_table},
        {"tools_table_label", tools_table_label},
        {"xy_toolchange_entry", xy_toolchange_entry},
        {"xy_toolchange_label", xy_toolchange_label},
        {"z_dispense_entry", z_dispense_entry},
        {"z_dispense_label", z_dispense_label},
        {"z_start_entry", z_start_entry},
        {"z_start_label", z_start_label},
        {"z_stop_entry", z_stop_entry},
        {"z_stop_label", z_stop_label},
        {"z_toolchange_entry", z_toolchange_entry},
        {"z_toolchange_label", z_toolchange_label},
        {"z_travel_entry", z_travel_entry},
        {"z_travel_label", z_travel_label},
    };
}

}  // namespace flatcam::gui
