// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/PaintToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void PaintToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Herramienta de pintura"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Herramienta de pintura"));
    layout->addWidget(w1, 0);
    tools_frame = new QFrame(nullptr);
    tools_box = new QVBoxLayout();
    tools_box->setContentsMargins(0, 0, 0, 0);
    tools_box->setSpacing(6);
    auto* lay2 = new QGridLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    type_obj_combo_label = new FCLabel(nullptr);
    type_obj_combo_label->setMinimumSize(QSize(60, 0));
    type_obj_combo_label->setText(QString::fromUtf8("Tipo de obj:"));
    type_obj_combo_label->setToolTip(QString::fromUtf8("Especifique el tipo de objeto a pintar.\nPuede ser de tipo: Gerber o Geometría.\nLo que se seleccione aquí dictará el tipo\nde objetos que llenarán el cuadro combinado 'Objeto'."));
    lay2->addWidget(type_obj_combo_label, 1, 0, 1, 1);
    type_obj_radio = new RadioSet({{QString::fromUtf8("Geometry"), QVariant(QString::fromUtf8("geometry"))}, {QString::fromUtf8("Gerber"), QVariant(QString::fromUtf8("gerber"))}}, Qt::Horizontal, true, nullptr);
    type_obj_radio->setValue(QVariant(QString::fromUtf8("geometry")));
    lay2->addWidget(type_obj_radio, 1, 1, 1, 1);
    object_label = new FCLabel(nullptr);
    object_label->setText(QString::fromUtf8("Objeto:"));
    object_label->setToolTip(QString::fromUtf8("Objeto a pintar."));
    lay2->addWidget(object_label, 2, 0, 1, 1);
    obj_combo = new FCComboBox(nullptr);
    lay2->addWidget(obj_combo, 2, 1, 1, 1);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    lay2->addWidget(w3, 5, 0, 1, 2);
    tools_table_label = new FCLabel(nullptr);
    tools_table_label->setText(QString::fromUtf8("<b>Tabla de herramientas</b>"));
    tools_table_label->setToolTip(QString::fromUtf8("Conjunto de herramientas desde el cual el algoritmo\nelegirá los que se usan para pintar."));
    lay2->addWidget(tools_table_label, 6, 0, 1, 2);
    tools_table = new FCTable(nullptr);
    tools_table->setAcceptDrops(true);
    tools_table->setProperty("columnCount", 4);
    tools_table->setProperty("contextMenuPolicy", 2);
    tools_table->setProperty("dragDropMode", 4);
    tools_table->setDragDropOverwriteMode(false);
    tools_table->setDragEnabled(true);
    tools_table->setMaximumSize(QSize(16777215, 53));
    tools_table->setMinimumSize(QSize(0, 53));
    tools_table->setProperty("rowCount", 1);
    tools_table->setProperty("selectionBehavior", 1);
    tools_table->setProperty("verticalScrollBarPolicy", 1);
    tools_table->setColumnCount(4);
    tools_table->setHorizontalHeaderLabels({QString::fromUtf8("#"), QString::fromUtf8("Diámetro"), QString::fromUtf8("TT"), QString::fromUtf8("")});
    tools_table->verticalHeader()->setVisible(false);
    tools_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode(2));
    tools_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode(1));
    tools_table->setColumnHidden(3, true);
    lay2->addWidget(tools_table, 7, 0, 1, 2);
    order_label = new FCLabel(nullptr);
    order_label->setText(QString::fromUtf8("<b>Orden de la Herram:</b>"));
    order_label->setToolTip(QString::fromUtf8("Esto establece la forma en que se utilizan las herramientas en la tabla de herramientas.\n'No' -> significa que el orden utilizado es el de la tabla de herramientas\n'Adelante' -> significa que las herramientas se ordenarán de pequeño a grande\n'Atras' -> means que las herramientas ordenarán de grande a pequeño\n\nADVERTENCIA: el uso del mecanizado en reposo establecerá automáticamente el orden\nen reversa y deshabilitar este control."));
    lay2->addWidget(order_label, 9, 0, 1, 1);
    order_radio = new RadioSet({{QString::fromUtf8("No"), QVariant(QString::fromUtf8("no"))}, {QString::fromUtf8("Adelante"), QVariant(QString::fromUtf8("fwd"))}, {QString::fromUtf8("Atras"), QVariant(QString::fromUtf8("rev"))}}, Qt::Horizontal, true, nullptr);
    order_radio->setToolTip(QString::fromUtf8("Esto establece la forma en que se utilizan las herramientas en la tabla de herramientas.\n'No' -> significa que el orden utilizado es el de la tabla de herramientas\n'Adelante' -> significa que las herramientas se ordenarán de pequeño a grande\n'Atras' -> means que las herramientas ordenarán de grande a pequeño\n\nADVERTENCIA: el uso del mecanizado en reposo establecerá automáticamente el orden\nen reversa y deshabilitar este control."));
    order_radio->setValue(QVariant(QString::fromUtf8("rev")));
    lay2->addWidget(order_radio, 9, 1, 1, 1);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay2->addWidget(w4, 10, 0, 1, 2);
    lay2->setColumnStretch(1, 1);
    tools_box->addLayout(lay2, 0);
    grid3 = new QGridLayout();
    grid3->setContentsMargins(0, 0, 0, 0);
    grid3->setHorizontalSpacing(6);
    grid3->setVerticalSpacing(6);
    tool_sel_label = new FCLabel(nullptr);
    tool_sel_label->setText(QString::fromUtf8("<b>Agregar desde DB</b>"));
    grid3->addWidget(tool_sel_label, 0, 0, 1, 2);
    new_tooldia_lbl = new FCLabel(nullptr);
    new_tooldia_lbl->setText(QString::fromUtf8("Diá. de Herram:"));
    new_tooldia_lbl->setToolTip(QString::fromUtf8("Diámetro de la nueva herramienta para agregar en la Tabla de herramientas.\nSi la herramienta es de tipo V, este valor es automáticamente\ncalculado a partir de los otros parámetros."));
    grid3->addWidget(new_tooldia_lbl, 2, 0, 1, 1);
    new_tooldia_entry = new FCDoubleSpinner(nullptr);
    new_tooldia_entry->setProperty("decimals", 4);
    new_tooldia_entry->setProperty("maximum", 10000.0);
    new_tooldia_entry->setProperty("objectName", QString::fromUtf8("p_tool_dia"));
    new_tooldia_entry->setProperty("value", 0.1);
    grid3->addWidget(new_tooldia_entry, 2, 1, 1, 1);
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
    grid3->addLayout(lay5, 7, 0, 1, 2);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    grid3->addWidget(w6, 8, 0, 1, 2);
    deltool_btn = new FCButton(nullptr);
    deltool_btn->setIcon(QIcon(QString::fromUtf8(":/images/trash16.png")));
    deltool_btn->setText(QString::fromUtf8("Borrar"));
    deltool_btn->setToolTip(QString::fromUtf8("Eliminar una selección de herramientas en la tabla de herramientas\nseleccionando primero una fila en la Tabla de herramientas."));
    grid3->addWidget(deltool_btn, 9, 0, 1, 2);
    auto* w7 = new QFrame(nullptr);
    w7->setProperty("frameShadow", 48);
    w7->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w7->setSizePolicy(sp); }
    grid3->addWidget(w7, 11, 0, 1, 2);
    tool_data_label = new FCLabel(nullptr);
    tool_data_label->setText(QString::fromUtf8("<b>Parámetros para: <font color='#0000FF'>Herramienta 1</font></b>"));
    tool_data_label->setToolTip(QString::fromUtf8("Los datos utilizados para crear GCode.\nCada herramienta almacena su propio conjunto de datos."));
    grid3->addWidget(tool_data_label, 12, 0, 1, 2);
    grid3->setColumnStretch(1, 1);
    tools_box->addLayout(grid3, 0);
    auto* lay8 = new QGridLayout();
    lay8->setContentsMargins(0, 0, 0, 0);
    lay8->setHorizontalSpacing(6);
    lay8->setVerticalSpacing(6);
    auto* w9 = new FCLabel(nullptr);
    w9->setText(QString::fromUtf8("Superposición:"));
    w9->setToolTip(QString::fromUtf8("Cuánto (porcentaje) del ancho de la herramienta para superponer cada pasada de herramienta.\nAjustar el valor comenzando con valores más bajos\ny aumentarlo si las áreas que deben procesarse aún están\nno procesado.Valores más bajos = procesamiento más rápido, ejecución más rápida en CNC.\nValores más altos = procesamiento lento y ejecución lenta en CNC\ndebido a demasiados caminos."));
    lay8->addWidget(w9, 1, 0, 1, 1);
    paintoverlap_entry = new FCDoubleSpinner(nullptr);
    paintoverlap_entry->setProperty("decimals", 3);
    paintoverlap_entry->setProperty("maximum", 100.0);
    paintoverlap_entry->setProperty("objectName", QString::fromUtf8("p_overlap"));
    paintoverlap_entry->setProperty("singleStep", 0.1);
    paintoverlap_entry->setProperty("suffix", QString::fromUtf8(" %"));
    paintoverlap_entry->setProperty("value", 20.0);
    paintoverlap_entry->setWrapping(true);
    lay8->addWidget(paintoverlap_entry, 1, 1, 1, 1);
    offset_label = new FCLabel(nullptr);
    offset_label->setText(QString::fromUtf8("Compensar:"));
    offset_label->setToolTip(QString::fromUtf8("Distancia por la cual evitar\nlos bordes del polígono a\nser pintado."));
    lay8->addWidget(offset_label, 2, 0, 1, 1);
    offset_entry = new FCDoubleSpinner(nullptr);
    offset_entry->setProperty("decimals", 4);
    offset_entry->setProperty("maximum", 10000.0);
    offset_entry->setProperty("minimum", -10000.0);
    offset_entry->setProperty("objectName", QString::fromUtf8("p_offset"));
    lay8->addWidget(offset_entry, 2, 1, 1, 1);
    auto* w10 = new FCLabel(nullptr);
    w10->setText(QString::fromUtf8("Método:"));
    w10->setToolTip(QString::fromUtf8("Algoritmo para pintar:\n- Estándar: paso fijo hacia adentro.\n- Basado en semillas: hacia afuera de la semilla.\n- Basado en líneas: líneas paralelas.\n- Líneas láser: activas solo para objetos Gerber.\nCreará líneas que siguen los rastros.\n- Combo: en caso de falla, se elegirá un nuevo método de los anteriores\nen el orden especificado."));
    lay8->addWidget(w10, 7, 0, 1, 1);
    paintmethod_combo = new FCComboBox2(nullptr);
    paintmethod_combo->setProperty("currentText", QString::fromUtf8("Estándar"));
    paintmethod_combo->setProperty("objectName", QString::fromUtf8("p_method"));
    paintmethod_combo->addItem(QString::fromUtf8("Estándar"));
    paintmethod_combo->addItem(QString::fromUtf8("Semilla"));
    paintmethod_combo->addItem(QString::fromUtf8("Líneas"));
    paintmethod_combo->addItem(QString::fromUtf8("Lineas laser"));
    paintmethod_combo->addItem(QString::fromUtf8("Combo"));
    lay8->addWidget(paintmethod_combo, 7, 1, 1, 1);
    pathconnect_cb = new FCCheckBox(nullptr);
    pathconnect_cb->setChecked(true);
    pathconnect_cb->setProperty("objectName", QString::fromUtf8("p_connect"));
    pathconnect_cb->setText(QString::fromUtf8("Conectar"));
    pathconnect_cb->setToolTip(QString::fromUtf8("Dibuja líneas entre el resultado\nSegmentos para minimizar elevaciones de herramientas."));
    lay8->addWidget(pathconnect_cb, 10, 0, 1, 1);
    paintcontour_cb = new FCCheckBox(nullptr);
    paintcontour_cb->setChecked(true);
    paintcontour_cb->setProperty("objectName", QString::fromUtf8("p_contour"));
    paintcontour_cb->setText(QString::fromUtf8("Contorno"));
    paintcontour_cb->setToolTip(QString::fromUtf8("Corta todo el perímetro del polígono.\nPara recortar los bordes ásperos."));
    lay8->addWidget(paintcontour_cb, 10, 1, 1, 1);
    auto* w11 = new QFrame(nullptr);
    w11->setProperty("frameShadow", 48);
    w11->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w11->setSizePolicy(sp); }
    lay8->addWidget(w11, 11, 0, 1, 2);
    apply_param_to_all = new FCButton(nullptr);
    apply_param_to_all->setIcon(QIcon(QString::fromUtf8(":/images/param_all32.png")));
    apply_param_to_all->setText(QString::fromUtf8("Aplicar Parám. a todas las herramientas"));
    apply_param_to_all->setToolTip(QString::fromUtf8("Se aplicarán los parámetros en el formulario actual\nen todas las herramientas de la tabla de herramientas."));
    lay8->addWidget(apply_param_to_all, 12, 0, 1, 2);
    auto* w12 = new QFrame(nullptr);
    w12->setProperty("frameShadow", 48);
    w12->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w12->setSizePolicy(sp); }
    lay8->addWidget(w12, 13, 0, 1, 2);
    gen_param_label = new FCLabel(nullptr);
    gen_param_label->setText(QString::fromUtf8("<b>Parámetros comunes</b>"));
    gen_param_label->setToolTip(QString::fromUtf8("Parámetros que son comunes para todas las herramientas."));
    lay8->addWidget(gen_param_label, 15, 0, 1, 2);
    rest_cb = new FCCheckBox(nullptr);
    rest_cb->setProperty("objectName", QString::fromUtf8("p_rest_machining"));
    rest_cb->setText(QString::fromUtf8("Resto"));
    rest_cb->setToolTip(QString::fromUtf8("Si está marcado, use 'mecanizado en resto'.\nBásicamente procesará cobre fuera de las características de la PCB,\nusando la herramienta más grande y continúe con las siguientes herramientas,\nde mayor a menor, para procesar las características de cobre que\nno pudo ser procesado por la herramienta anterior, hasta que haya\nno queda nada por procesar o no hay más herramientas.\n\nSi no está marcado, use el algoritmo estándar."));
    lay8->addWidget(rest_cb, 16, 0, 1, 2);
    rest_offset_label = new FCLabel(nullptr);
    rest_offset_label->setText(QString::fromUtf8("Compensar:"));
    rest_offset_label->setToolTip(QString::fromUtf8("Distancia por la cual evitar\nlos bordes del polígono a\nser pintado."));
    rest_offset_label->setHidden(true);
    lay8->addWidget(rest_offset_label, 17, 0, 1, 1);
    rest_offset_entry = new FCDoubleSpinner(nullptr);
    rest_offset_entry->setProperty("decimals", 4);
    rest_offset_entry->setProperty("maximum", 10000.0);
    rest_offset_entry->setProperty("minimum", -10000.0);
    rest_offset_entry->setHidden(true);
    lay8->addWidget(rest_offset_entry, 17, 1, 1, 1);
    auto* w13 = new FCLabel(nullptr);
    w13->setText(QString::fromUtf8("Selección:"));
    w13->setToolTip(QString::fromUtf8("Selección del área a procesar.\n- 'Selección de polígonos': haga clic con el botón izquierdo del mouse para agregar / eliminar polígonos que se procesarán.\n- 'Selección de área': haga clic con el botón izquierdo del mouse para iniciar la selección del área a procesar.\nMantener presionada una tecla modificadora (CTRL o SHIFT) permitirá agregar múltiples áreas.\n- 'Todos los polígonos': el proceso comenzará después de hacer clic.\n- 'Objeto de referencia': procesará el área especificada por otro objeto."));
    lay8->addWidget(w13, 18, 0, 1, 1);
    selectmethod_combo = new FCComboBox2(nullptr);
    selectmethod_combo->setProperty("currentText", QString::fromUtf8("Todos"));
    selectmethod_combo->setProperty("objectName", QString::fromUtf8("p_selection"));
    selectmethod_combo->addItem(QString::fromUtf8("Todos"));
    selectmethod_combo->addItem(QString::fromUtf8("Selección de polígono"));
    selectmethod_combo->addItem(QString::fromUtf8("Selección de área"));
    selectmethod_combo->addItem(QString::fromUtf8("Objeto de referencia"));
    lay8->addWidget(selectmethod_combo, 18, 1, 1, 1);
    reference_type_label = new FCLabel(nullptr);
    reference_type_label->setText(QString::fromUtf8("Tipo:"));
    reference_type_label->setToolTip(QString::fromUtf8("El tipo de objeto FlatCAM que se utilizará como referencia de pintura.\nPuede ser Gerber, Excellon o Geometry."));
    reference_type_label->setHidden(true);
    lay8->addWidget(reference_type_label, 20, 0, 1, 1);
    reference_type_combo = new FCComboBox2(nullptr);
    reference_type_combo->setProperty("currentText", QString::fromUtf8("Gerber"));
    reference_type_combo->setHidden(true);
    reference_type_combo->addItem(QString::fromUtf8("Gerber"));
    reference_type_combo->addItem(QString::fromUtf8("Excellon"));
    reference_type_combo->addItem(QString::fromUtf8("Geometría"));
    lay8->addWidget(reference_type_combo, 20, 1, 1, 1);
    reference_combo = new FCComboBox(nullptr);
    reference_combo->setHidden(true);
    lay8->addWidget(reference_combo, 22, 0, 1, 2);
    area_shape_label = new FCLabel(nullptr);
    area_shape_label->setText(QString::fromUtf8("Forma:"));
    area_shape_label->setToolTip(QString::fromUtf8("El tipo de forma de selección utilizada para la selección de área."));
    area_shape_label->setHidden(true);
    lay8->addWidget(area_shape_label, 24, 0, 1, 1);
    area_shape_radio = new RadioSet({{QString::fromUtf8("Cuadrado"), QVariant(QString::fromUtf8("square"))}, {QString::fromUtf8("Polígono"), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    area_shape_radio->setHidden(true);
    area_shape_radio->setValue(QVariant(QString::fromUtf8("square")));
    lay8->addWidget(area_shape_radio, 24, 1, 1, 1);
    lay8->setColumnStretch(1, 1);
    tools_box->addLayout(lay8, 0);
    generate_paint_button = new FCButton(nullptr);
    { QFont f = generate_paint_button->font(); f.setBold(true); generate_paint_button->setFont(f); }
    generate_paint_button->setIcon(QIcon(QString::fromUtf8(":/images/geometry32.png")));
    generate_paint_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t\t\t\t QPushButton\n\t\t\t\t\t\t\t\t\t\t\t\t {\n\t\t\t\t\t\t\t\t\t\t\t\t\t font-weight: bold;\n\t\t\t\t\t\t\t\t\t\t\t\t }\n\t\t\t\t\t\t\t\t\t\t\t\t "));
    generate_paint_button->setText(QString::fromUtf8("Generar Geometría"));
    generate_paint_button->setToolTip(QString::fromUtf8("Crea un objeto de geometría que pinte los polígonos."));
    tools_box->addWidget(generate_paint_button, 0);
    tools_box->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new FCButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t\t\t"));
    reset_button->setText(QString::fromUtf8("Restablecer la Herramienta"));
    reset_button->setToolTip(QString::fromUtf8("Restablecerá los parámetros de la herramienta."));
    tools_box->addWidget(reset_button, 0);
    tools_frame->setLayout(tools_box);
    layout->addWidget(tools_frame, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> PaintToolUIBase::membersByName() const {
    return {
        {"add_newtool_button", add_newtool_button},
        {"addtool_from_db_btn", addtool_from_db_btn},
        {"apply_param_to_all", apply_param_to_all},
        {"area_shape_label", area_shape_label},
        {"area_shape_radio", area_shape_radio},
        {"deltool_btn", deltool_btn},
        {"gen_param_label", gen_param_label},
        {"generate_paint_button", generate_paint_button},
        {"grid3", grid3},
        {"layout", layout},
        {"new_tooldia_entry", new_tooldia_entry},
        {"new_tooldia_lbl", new_tooldia_lbl},
        {"obj_combo", obj_combo},
        {"object_label", object_label},
        {"offset_entry", offset_entry},
        {"offset_label", offset_label},
        {"order_label", order_label},
        {"order_radio", order_radio},
        {"paintcontour_cb", paintcontour_cb},
        {"paintmethod_combo", paintmethod_combo},
        {"paintoverlap_entry", paintoverlap_entry},
        {"pathconnect_cb", pathconnect_cb},
        {"reference_combo", reference_combo},
        {"reference_type_combo", reference_type_combo},
        {"reference_type_label", reference_type_label},
        {"reset_button", reset_button},
        {"rest_cb", rest_cb},
        {"rest_offset_entry", rest_offset_entry},
        {"rest_offset_label", rest_offset_label},
        {"selectmethod_combo", selectmethod_combo},
        {"tool_data_label", tool_data_label},
        {"tool_sel_label", tool_sel_label},
        {"tools_box", tools_box},
        {"tools_frame", tools_frame},
        {"tools_table", tools_table},
        {"tools_table_label", tools_table_label},
        {"type_obj_combo_label", type_obj_combo_label},
        {"type_obj_radio", type_obj_radio},
    };
}

}  // namespace flatcam::gui
