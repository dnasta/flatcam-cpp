// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/CutOutToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void CutOutToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("PCB de corte"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new QLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("PCB de corte"));
    layout->addWidget(w1, 0);
    auto* w2 = new QLabel(nullptr);
    layout->addWidget(w2, 0);
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    object_label = new QLabel(nullptr);
    object_label->setText(QString::fromUtf8("<b>Objeto fuente:</b>"));
    object_label->setToolTip(QString::fromUtf8("Objeto a recortar."));
    lay3->addWidget(object_label, 0, 0, 1, 2);
    kindlabel = new QLabel(nullptr);
    kindlabel->setText(QString::fromUtf8("Tipo:"));
    kindlabel->setToolTip(QString::fromUtf8("Elección del tipo de objeto que queremos recortar.\n- Único: contiene un único objeto de contorno de PCB Gerber.\n- Panel: un objeto de panel PCB Gerber, que se hace\nde muchos esquemas de PCB individuales."));
    lay3->addWidget(kindlabel, 2, 0, 1, 1);
    obj_kind_combo = new RadioSet({{QString::fromUtf8("Soltero"), QVariant(QString::fromUtf8("single"))}, {QString::fromUtf8("Panel"), QVariant(QString::fromUtf8("panel"))}}, Qt::Horizontal, true, nullptr);
    obj_kind_combo->setValue(QVariant(QString::fromUtf8("single")));
    lay3->addWidget(obj_kind_combo, 2, 1, 1, 1);
    type_obj_combo_label = new QLabel(nullptr);
    type_obj_combo_label->setText(QString::fromUtf8("Tipo:"));
    type_obj_combo_label->setToolTip(QString::fromUtf8("Especifique el tipo de objeto a recortar.\nPuede ser de tipo: Gerber o Geometría.\nLo que se seleccione aquí dictará el tipo\nde objetos que llenarán el cuadro combinado 'Objeto'."));
    lay3->addWidget(type_obj_combo_label, 4, 0, 1, 1);
    type_obj_radio = new RadioSet({{QString::fromUtf8("Gerber"), QVariant(QString::fromUtf8("grb"))}, {QString::fromUtf8("Geometría"), QVariant(QString::fromUtf8("geo"))}}, Qt::Horizontal, true, nullptr);
    type_obj_radio->setValue(QVariant(QString::fromUtf8("grb")));
    lay3->addWidget(type_obj_radio, 4, 1, 1, 1);
    obj_combo = new FCComboBox(nullptr);
    lay3->addWidget(obj_combo, 6, 0, 1, 2);
    convex_box_label = new QLabel(nullptr);
    convex_box_label->setText(QString::fromUtf8("Forma convexa:"));
    convex_box_label->setToolTip(QString::fromUtf8("Crea una forma convexa que rodea toda la PCB.\nSe usa solo si el tipo de objeto de origen es Gerber."));
    lay3->addWidget(convex_box_label, 8, 0, 1, 1);
    convex_box_cb = new FCCheckBox(nullptr);
    convex_box_cb->setToolTip(QString::fromUtf8("Crea una forma convexa que rodea toda la PCB.\nSe usa solo si el tipo de objeto de origen es Gerber."));
    lay3->addWidget(convex_box_cb, 8, 1, 1, 1);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay3->addWidget(w4, 10, 0, 1, 2);
    tool_sel_label = new FCLabel(nullptr);
    tool_sel_label->setText(QString::fromUtf8("<b>Herramienta de Corte</b>"));
    lay3->addWidget(tool_sel_label, 12, 0, 1, 2);
    dia_label = new QLabel(nullptr);
    dia_label->setText(QString::fromUtf8("Diá. de Herram:"));
    dia_label->setToolTip(QString::fromUtf8("Diámetro de la herramienta utilizada para cortar\nLa forma de PCB fuera del material circundante."));
    lay3->addWidget(dia_label, 14, 0, 1, 1);
    dia = new FCDoubleSpinner(nullptr);
    dia->setProperty("decimals", 4);
    dia->setProperty("maximum", 10000.0);
    dia->setProperty("value", 2.4);
    lay3->addWidget(dia, 14, 1, 1, 1);
    auto* lay5 = new QHBoxLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setSpacing(6);
    add_newtool_button = new FCButton(nullptr);
    add_newtool_button->setIcon(QIcon(QString::fromUtf8(":/images/plus16.png")));
    add_newtool_button->setText(QString::fromUtf8("Buscar y agregar"));
    add_newtool_button->setToolTip(QString::fromUtf8("Agregar una nueva herramienta a la tabla de herramientas\ncon el diámetro especificado arriba.\nEsto se hace mediante una búsqueda en segundo plano.\nen la base de datos de herramientas. Si no se encuentra nada\nen la base de datos de herramientas, se agrega una herramienta predeterminada."));
    lay5->addWidget(add_newtool_button, 0);
    addtool_from_db_btn = new FCButton(nullptr);
    addtool_from_db_btn->setIcon(QIcon(QString::fromUtf8(":/images/search_db32.png")));
    addtool_from_db_btn->setText(QString::fromUtf8("Elija de DB"));
    addtool_from_db_btn->setToolTip(QString::fromUtf8("Agregar una nueva herramienta a la tabla de herramientas\nde la base de datos de herramientas.\nHerramientas de administración de bases de datos en:\nMenú: Opciones -> Base de datos de herramientas"));
    lay5->addWidget(addtool_from_db_btn, 0);
    lay3->addLayout(lay5, 16, 0, 1, 2);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    lay3->addWidget(w6, 18, 0, 1, 2);
    param_label = new QLabel(nullptr);
    param_label->setText(QString::fromUtf8("<b>Parámetros de Herramienta:</b>"));
    lay3->addWidget(param_label, 20, 0, 1, 2);
    auto* w7 = new QLabel(nullptr);
    w7->setText(QString::fromUtf8("Corte Z:"));
    w7->setToolTip(QString::fromUtf8("Profundidad de corte (negativo)\ndebajo de la superficie de cobre."));
    lay3->addWidget(w7, 22, 0, 1, 1);
    cutz_entry = new FCDoubleSpinner(nullptr);
    cutz_entry->setProperty("decimals", 4);
    cutz_entry->setProperty("maximum", -0.0);
    cutz_entry->setProperty("minimum", -10000.0);
    cutz_entry->setProperty("singleStep", 0.1);
    cutz_entry->setProperty("value", -1.8);
    lay3->addWidget(cutz_entry, 22, 1, 1, 1);
    mpass_cb = new FCCheckBox(nullptr);
    mpass_cb->setChecked(true);
    mpass_cb->setText(QString::fromUtf8("Profund. Múlti:"));
    mpass_cb->setToolTip(QString::fromUtf8("Usa múltiples pases para limitar\nLa profundidad de corte en cada pasada. Será\ncortar varias veces hasta que el Corte Z sea\nalcanzado."));
    lay3->addWidget(mpass_cb, 24, 0, 1, 1);
    maxdepth_entry = new FCDoubleSpinner(nullptr);
    maxdepth_entry->setProperty("decimals", 4);
    maxdepth_entry->setProperty("maximum", 10000.0);
    maxdepth_entry->setProperty("singleStep", 0.1);
    maxdepth_entry->setToolTip(QString::fromUtf8("Profundidad de cada pase (positivo)."));
    maxdepth_entry->setProperty("value", 0.6);
    lay3->addWidget(maxdepth_entry, 24, 1, 1, 1);
    margin_label = new QLabel(nullptr);
    margin_label->setText(QString::fromUtf8("Margen:"));
    margin_label->setToolTip(QString::fromUtf8("Margen sobre los límites. Un valor positivo aquí\nhará que el corte de la PCB esté más alejado de\nel borde real de PCB"));
    lay3->addWidget(margin_label, 26, 0, 1, 1);
    margin = new FCDoubleSpinner(nullptr);
    margin->setProperty("decimals", 4);
    margin->setProperty("maximum", 10000.0);
    margin->setProperty("minimum", -10000.0);
    margin->setProperty("singleStep", 0.1);
    margin->setProperty("value", 0.1);
    lay3->addWidget(margin, 26, 1, 1, 1);
    gapsize_label = new QLabel(nullptr);
    gapsize_label->setText(QString::fromUtf8("Tamaño de la brecha:"));
    gapsize_label->setToolTip(QString::fromUtf8("El tamaño de los huecos del puente en el recorte\nsolía mantener la placa conectada a\nel material circundante (el\nde la cual se corta el PCB)."));
    lay3->addWidget(gapsize_label, 28, 0, 1, 1);
    gapsize = new FCDoubleSpinner(nullptr);
    gapsize->setProperty("decimals", 4);
    gapsize->setProperty("value", 4.0);
    lay3->addWidget(gapsize, 28, 1, 1, 1);
    gaptype_label = new FCLabel(nullptr);
    gaptype_label->setText(QString::fromUtf8("Tipo de brecha:"));
    gaptype_label->setToolTip(QString::fromUtf8("El tipo de brecha:\n- Puente -> el corte será interrumpido por puentes\n- Delgado -> igual que 'puente' pero será más delgado al fresar parcialmente el espacio\n- M-Bites -> 'Mouse Bites' - igual que el 'puente' pero cubierto con agujeros"));
    lay3->addWidget(gaptype_label, 30, 0, 1, 1);
    gaptype_radio = new RadioSet({{QString::fromUtf8("Puente"), QVariant(QString::fromUtf8("b"))}, {QString::fromUtf8("Delgado"), QVariant(QString::fromUtf8("bt"))}, {QString::fromUtf8("M-Bites"), QVariant(QString::fromUtf8("mb"))}}, Qt::Horizontal, true, nullptr);
    gaptype_radio->setValue(QVariant(QString::fromUtf8("b")));
    lay3->addWidget(gaptype_radio, 30, 1, 1, 1);
    thin_depth_label = new FCLabel(nullptr);
    thin_depth_label->setText(QString::fromUtf8("Profundidad:"));
    thin_depth_label->setToolTip(QString::fromUtf8("La profundidad hasta que se hace el fresado\npara adelgazar los huecos."));
    thin_depth_label->setHidden(true);
    lay3->addWidget(thin_depth_label, 32, 0, 1, 1);
    thin_depth_entry = new FCDoubleSpinner(nullptr);
    thin_depth_entry->setProperty("decimals", 4);
    thin_depth_entry->setProperty("maximum", -0.0);
    thin_depth_entry->setProperty("minimum", -10000.0);
    thin_depth_entry->setProperty("singleStep", 0.1);
    thin_depth_entry->setProperty("value", -1.0);
    thin_depth_entry->setHidden(true);
    lay3->addWidget(thin_depth_entry, 32, 1, 1, 1);
    mb_dia_label = new FCLabel(nullptr);
    mb_dia_label->setText(QString::fromUtf8("Diá. de Herram:"));
    mb_dia_label->setToolTip(QString::fromUtf8("El diámetro del taladro al hacer 'mouse bytes'."));
    mb_dia_label->setHidden(true);
    lay3->addWidget(mb_dia_label, 34, 0, 1, 1);
    mb_dia_entry = new FCDoubleSpinner(nullptr);
    mb_dia_entry->setProperty("decimals", 4);
    mb_dia_entry->setProperty("maximum", 100.0);
    mb_dia_entry->setProperty("value", 0.6);
    mb_dia_entry->setHidden(true);
    lay3->addWidget(mb_dia_entry, 34, 1, 1, 1);
    mb_spacing_label = new FCLabel(nullptr);
    mb_spacing_label->setText(QString::fromUtf8("Spacing:"));
    mb_spacing_label->setToolTip(QString::fromUtf8("El espacio entre los taladros al hacer 'mouse bites'."));
    mb_spacing_label->setHidden(true);
    lay3->addWidget(mb_spacing_label, 36, 0, 1, 1);
    mb_spacing_entry = new FCDoubleSpinner(nullptr);
    mb_spacing_entry->setProperty("decimals", 4);
    mb_spacing_entry->setProperty("maximum", 100.0);
    mb_spacing_entry->setProperty("value", 0.3);
    mb_spacing_entry->setHidden(true);
    lay3->addWidget(mb_spacing_entry, 36, 1, 1, 1);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    lay3->addWidget(w8, 38, 0, 1, 2);
    auto* w9 = new QLabel(nullptr);
    w9->setText(QString::fromUtf8("<b>Auto Huecos de puentes</b>:"));
    w9->setToolTip(QString::fromUtf8("Esta sección maneja la creación de espacios de puente automáticos."));
    lay3->addWidget(w9, 40, 0, 1, 2);
    auto* w10 = new QLabel(nullptr);
    w10->setText(QString::fromUtf8("Brechas:"));
    w10->setToolTip(QString::fromUtf8("Número de huecos utilizados para el recorte automático.\nPuede haber un máximo de 8 puentes / huecos.\nLas opciones son:\n- Ninguno - sin espacios\n- lr - izquierda + derecha\n- tb - arriba + abajo\n- 4 - izquierda + derecha + arriba + abajo\n- 2lr - 2 * izquierda + 2 * derecha\n- 2tb - 2 * arriba + 2 * abajo\n- 8 - 2 * izquierda + 2 * derecha + 2 * arriba + 2 * abajo"));
    lay3->addWidget(w10, 42, 0, 1, 1);
    gaps = new FCComboBox(nullptr);
    gaps->setProperty("currentText", QString::fromUtf8("4"));
    gaps->addItem(QString::fromUtf8("None"));
    gaps->addItem(QString::fromUtf8("LR"));
    gaps->addItem(QString::fromUtf8("TB"));
    gaps->addItem(QString::fromUtf8("4"));
    gaps->addItem(QString::fromUtf8("2LR"));
    gaps->addItem(QString::fromUtf8("2TB"));
    gaps->addItem(QString::fromUtf8("8"));
    gaps->setCurrentIndex(3);
    lay3->addWidget(gaps, 42, 1, 1, 1);
    ff_cutout_object_btn = new FCButton(nullptr);
    { QFont f = ff_cutout_object_btn->font(); f.setBold(true); ff_cutout_object_btn->setFont(f); }
    ff_cutout_object_btn->setIcon(QIcon(QString::fromUtf8(":/images/irregular32.png")));
    ff_cutout_object_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    ff_cutout_object_btn->setText(QString::fromUtf8("Generar Geometría"));
    ff_cutout_object_btn->setToolTip(QString::fromUtf8("Recorta el objeto seleccionado.\nLa forma recortada puede ser de cualquier forma.\nÚtil cuando la PCB tiene una forma no rectangular."));
    lay3->addWidget(ff_cutout_object_btn, 44, 0, 1, 2);
    rect_cutout_object_btn = new FCButton(nullptr);
    { QFont f = rect_cutout_object_btn->font(); f.setBold(true); rect_cutout_object_btn->setFont(f); }
    rect_cutout_object_btn->setIcon(QIcon(QString::fromUtf8(":/images/rectangle32.png")));
    rect_cutout_object_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    rect_cutout_object_btn->setText(QString::fromUtf8("Generar Geometría"));
    rect_cutout_object_btn->setToolTip(QString::fromUtf8("Recorta el objeto seleccionado.\nLa forma de corte resultante es\nsiempre una forma rectangular y será\nEl cuadro delimitador del objeto."));
    lay3->addWidget(rect_cutout_object_btn, 46, 0, 1, 2);
    auto* w11 = new QFrame(nullptr);
    w11->setProperty("frameShadow", 48);
    w11->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w11->setSizePolicy(sp); }
    lay3->addWidget(w11, 48, 0, 1, 2);
    auto* w12 = new QLabel(nullptr);
    w12->setText(QString::fromUtf8("<b>Manual Huecos de puentes</b>:"));
    w12->setToolTip(QString::fromUtf8("Esta sección maneja la creación de espacios de puente manuales.\nEsto se hace haciendo clic con el mouse en el perímetro del\nObjeto de geometría que se utiliza como objeto recortado. "));
    lay3->addWidget(w12, 50, 0, 1, 2);
    auto* w13 = new QLabel(nullptr);
    w13->setText(QString::fromUtf8("Cursor grande:"));
    w13->setToolTip(QString::fromUtf8("Use un cursor grande cuando agregue espacios manuales."));
    lay3->addWidget(w13, 52, 0, 1, 1);
    big_cursor_cb = new FCCheckBox(nullptr);
    big_cursor_cb->setChecked(true);
    lay3->addWidget(big_cursor_cb, 52, 1, 1, 1);
    man_geo_creation_btn = new FCButton(nullptr);
    man_geo_creation_btn->setIcon(QIcon(QString::fromUtf8(":/images/rectangle32.png")));
    man_geo_creation_btn->setText(QString::fromUtf8("Generar geometría manual"));
    man_geo_creation_btn->setToolTip(QString::fromUtf8("Si el objeto a recortar es un Gerber\nprimero crea una Geometría que lo rodea,\npara ser utilizado como recorte, si aún no existe.\nSeleccione el archivo fuente de Gerber en el cuadro combinado de objeto superior."));
    lay3->addWidget(man_geo_creation_btn, 54, 0, 1, 2);
    man_object_label = new QLabel(nullptr);
    man_object_label->setText(QString::fromUtf8("Geometría de corte manual:"));
    man_object_label->setToolTip(QString::fromUtf8("Objeto de geometría utilizado para crear el recorte manual."));
    lay3->addWidget(man_object_label, 56, 0, 1, 2);
    man_object_combo = new FCComboBox(nullptr);
    lay3->addWidget(man_object_combo, 56, 0, 1, 2);
    man_gaps_creation_btn = new FCButton(nullptr);
    { QFont f = man_gaps_creation_btn->font(); f.setBold(true); man_gaps_creation_btn->setFont(f); }
    man_gaps_creation_btn->setIcon(QIcon(QString::fromUtf8(":/images/gaps32.png")));
    man_gaps_creation_btn->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    man_gaps_creation_btn->setText(QString::fromUtf8("Agregar huecos de puente manuales"));
    man_gaps_creation_btn->setToolTip(QString::fromUtf8("Haga clic con el botón izquierdo del mouse (LMB)\npara crear un espacio de puente para separar la PCB de\nEl material circundante.\nEl clic LMB debe hacerse en el perímetro de\nEl objeto Geometry utilizado como geometría de recorte."));
    lay3->addWidget(man_gaps_creation_btn, 58, 0, 1, 2);
    lay3->setColumnStretch(1, 1);
    layout->addLayout(lay3, 0);
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

std::map<std::string, QObject*> CutOutToolUIBase::membersByName() const {
    return {
        {"add_newtool_button", add_newtool_button},
        {"addtool_from_db_btn", addtool_from_db_btn},
        {"big_cursor_cb", big_cursor_cb},
        {"convex_box_cb", convex_box_cb},
        {"convex_box_label", convex_box_label},
        {"cutz_entry", cutz_entry},
        {"dia", dia},
        {"dia_label", dia_label},
        {"ff_cutout_object_btn", ff_cutout_object_btn},
        {"gaps", gaps},
        {"gapsize", gapsize},
        {"gapsize_label", gapsize_label},
        {"gaptype_label", gaptype_label},
        {"gaptype_radio", gaptype_radio},
        {"kindlabel", kindlabel},
        {"layout", layout},
        {"man_gaps_creation_btn", man_gaps_creation_btn},
        {"man_geo_creation_btn", man_geo_creation_btn},
        {"man_object_combo", man_object_combo},
        {"man_object_label", man_object_label},
        {"margin", margin},
        {"margin_label", margin_label},
        {"maxdepth_entry", maxdepth_entry},
        {"mb_dia_entry", mb_dia_entry},
        {"mb_dia_label", mb_dia_label},
        {"mb_spacing_entry", mb_spacing_entry},
        {"mb_spacing_label", mb_spacing_label},
        {"mpass_cb", mpass_cb},
        {"obj_combo", obj_combo},
        {"obj_kind_combo", obj_kind_combo},
        {"object_label", object_label},
        {"param_label", param_label},
        {"rect_cutout_object_btn", rect_cutout_object_btn},
        {"reset_button", reset_button},
        {"thin_depth_entry", thin_depth_entry},
        {"thin_depth_label", thin_depth_label},
        {"tool_sel_label", tool_sel_label},
        {"type_obj_combo_label", type_obj_combo_label},
        {"type_obj_radio", type_obj_radio},
    };
}

}  // namespace flatcam::gui
