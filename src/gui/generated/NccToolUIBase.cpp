// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/NccToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void NccToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Compensación sin cobre"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    tools_frame = new QFrame(nullptr);
    tools_box = new QVBoxLayout();
    tools_box->setContentsMargins(0, 0, 0, 0);
    tools_box->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Compensación sin cobre"));
    tools_box->addWidget(w1, 0);
    auto* lay2 = new QFormLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    type_obj_combo_label = new FCLabel(nullptr);
    type_obj_combo_label->setMinimumSize(QSize(60, 0));
    type_obj_combo_label->setText(QString::fromUtf8("Tipo de obj:"));
    type_obj_combo_label->setToolTip(QString::fromUtf8("Especifique el tipo de objeto que se eliminará del exceso de cobre.\nPuede ser de tipo: Gerber o Geometría.\nLo que se seleccione aquí dictará el tipo\nde objetos que llenarán el cuadro combinado 'Objeto'."));
    lay2->setWidget(0, QFormLayout::ItemRole(0), type_obj_combo_label);
    type_obj_radio = new RadioSet({{QString::fromUtf8("Geometría"), QVariant(QString::fromUtf8("geometry"))}, {QString::fromUtf8("Gerber"), QVariant(QString::fromUtf8("gerber"))}}, Qt::Horizontal, true, nullptr);
    type_obj_radio->setValue(QVariant(QString::fromUtf8("gerber")));
    lay2->setWidget(0, QFormLayout::ItemRole(1), type_obj_radio);
    object_label = new FCLabel(nullptr);
    object_label->setText(QString::fromUtf8("Objeto:"));
    object_label->setToolTip(QString::fromUtf8("Objeto a eliminar del exceso de cobre."));
    lay2->setWidget(1, QFormLayout::ItemRole(0), object_label);
    object_combo = new FCComboBox(nullptr);
    lay2->setWidget(1, QFormLayout::ItemRole(1), object_combo);
    tools_box->addLayout(lay2, 0);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    tools_box->addWidget(w3, 0);
    tools_table_label = new FCLabel(nullptr);
    tools_table_label->setText(QString::fromUtf8("<b>Tabla de herramientas</b>"));
    tools_table_label->setToolTip(QString::fromUtf8("Conjunto de herramientas desde el cual el algoritmo\nelegirá los utilizados para la limpieza de cobre."));
    tools_box->addWidget(tools_table_label, 0);
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
    tools_box->addWidget(tools_table, 0);
    auto* lay4 = new QGridLayout();
    lay4->setContentsMargins(0, 0, 0, 0);
    lay4->setHorizontalSpacing(6);
    lay4->setVerticalSpacing(6);
    ncc_order_label = new FCLabel(nullptr);
    ncc_order_label->setText(QString::fromUtf8("Orden de la Herram:"));
    ncc_order_label->setToolTip(QString::fromUtf8("Esto establece la forma en que se utilizan las herramientas en la tabla de herramientas.\n'No' -> significa que el orden utilizado es el de la tabla de herramientas\n'Adelante' -> significa que las herramientas se ordenarán de pequeño a grande\n'Atras' -> means que las herramientas ordenarán de grande a pequeño\n\nADVERTENCIA: el uso del mecanizado en reposo establecerá automáticamente el orden\nen reversa y deshabilitar este control."));
    lay4->addWidget(ncc_order_label, 1, 0, 1, 1);
    ncc_order_radio = new RadioSet({{QString::fromUtf8("No"), QVariant(QString::fromUtf8("no"))}, {QString::fromUtf8("Adelante"), QVariant(QString::fromUtf8("fwd"))}, {QString::fromUtf8("Atras"), QVariant(QString::fromUtf8("rev"))}}, Qt::Horizontal, true, nullptr);
    ncc_order_radio->setToolTip(QString::fromUtf8("Esto establece la forma en que se utilizan las herramientas en la tabla de herramientas.\n'No' -> significa que el orden utilizado es el de la tabla de herramientas\n'Adelante' -> significa que las herramientas se ordenarán de pequeño a grande\n'Atras' -> means que las herramientas ordenarán de grande a pequeño\n\nADVERTENCIA: el uso del mecanizado en reposo establecerá automáticamente el orden\nen reversa y deshabilitar este control."));
    ncc_order_radio->setValue(QVariant(QString::fromUtf8("rev")));
    lay4->addWidget(ncc_order_radio, 1, 1, 1, 1);
    auto* w5 = new QFrame(nullptr);
    w5->setProperty("frameShadow", 48);
    w5->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w5->setSizePolicy(sp); }
    lay4->addWidget(w5, 2, 0, 1, 2);
    lay4->setColumnStretch(1, 1);
    tools_box->addLayout(lay4, 0);
    grid3 = new QGridLayout();
    grid3->setContentsMargins(0, 0, 0, 0);
    grid3->setHorizontalSpacing(6);
    grid3->setVerticalSpacing(6);
    tool_sel_label = new FCLabel(nullptr);
    tool_sel_label->setText(QString::fromUtf8("<b>Agregar desde DB</b>"));
    grid3->addWidget(tool_sel_label, 0, 0, 1, 2);
    new_tooldia_lbl = new FCLabel(nullptr);
    new_tooldia_lbl->setText(QString::fromUtf8("Diá. de Herram:"));
    new_tooldia_lbl->setToolTip(QString::fromUtf8("Diámetro para la nueva herramienta"));
    grid3->addWidget(new_tooldia_lbl, 2, 0, 1, 1);
    auto* lay6 = new QHBoxLayout();
    lay6->setContentsMargins(0, 0, 0, 0);
    lay6->setSpacing(6);
    new_tooldia_entry = new FCDoubleSpinner(nullptr);
    new_tooldia_entry->setProperty("decimals", 4);
    new_tooldia_entry->setProperty("maximum", 10000.0);
    new_tooldia_entry->setProperty("objectName", QString::fromUtf8("Diá. de Herram"));
    new_tooldia_entry->setProperty("value", 0.1);
    lay6->addWidget(new_tooldia_entry, 0);
    find_optimal_button = new QToolButton(nullptr);
    find_optimal_button->setIcon(QIcon(QString::fromUtf8(":/images/open_excellon32.png")));
    find_optimal_button->setText(QString::fromUtf8("Óptimo"));
    find_optimal_button->setProperty("toolButtonStyle", 2);
    find_optimal_button->setToolTip(QString::fromUtf8("Encuentre un diámetro de herramienta garantizado\npara hacer un aislamiento completo."));
    lay6->addWidget(find_optimal_button, 0);
    grid3->addLayout(lay6, 2, 1, 1, 1);
    auto* lay7 = new QHBoxLayout();
    lay7->setContentsMargins(0, 0, 0, 0);
    lay7->setSpacing(6);
    add_newtool_button = new FCButton(nullptr);
    add_newtool_button->setIcon(QIcon(QString::fromUtf8(":/images/plus16.png")));
    add_newtool_button->setText(QString::fromUtf8("Buscar y agregar"));
    add_newtool_button->setToolTip(QString::fromUtf8("Agregar una nueva herramienta a la tabla de herramientas\ncon el diámetro especificado arriba.\nEsto se hace mediante una búsqueda en segundo plano.\nen la base de datos de herramientas. Si no se encuentra nada\nen la base de datos de herramientas, se agrega una herramienta predeterminada."));
    lay7->addWidget(add_newtool_button, 0);
    addtool_from_db_btn = new FCButton(nullptr);
    addtool_from_db_btn->setIcon(QIcon(QString::fromUtf8(":/images/search_db32.png")));
    addtool_from_db_btn->setText(QString::fromUtf8("Elija de DB"));
    addtool_from_db_btn->setToolTip(QString::fromUtf8("Agregar una nueva herramienta a la tabla de herramientas\nde la base de datos de herramientas.\nHerramientas de administración de bases de datos en:\nMenú: Opciones -> Base de datos de herramientas"));
    lay7->addWidget(addtool_from_db_btn, 0);
    grid3->addLayout(lay7, 7, 0, 1, 2);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    grid3->addWidget(w8, 8, 0, 1, 2);
    deltool_btn = new FCButton(nullptr);
    deltool_btn->setIcon(QIcon(QString::fromUtf8(":/images/trash16.png")));
    deltool_btn->setText(QString::fromUtf8("Borrar"));
    deltool_btn->setToolTip(QString::fromUtf8("Eliminar una selección de herramientas en la tabla de herramientas\nseleccionando primero una fila en la Tabla de herramientas."));
    grid3->addWidget(deltool_btn, 9, 0, 1, 2);
    auto* w9 = new QFrame(nullptr);
    w9->setProperty("frameShadow", 48);
    w9->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w9->setSizePolicy(sp); }
    grid3->addWidget(w9, 11, 0, 1, 2);
    tool_data_label = new FCLabel(nullptr);
    tool_data_label->setText(QString::fromUtf8("<b>Parámetros para: <font color=\"#7777FF\">Herramienta 1</font></b>"));
    tool_data_label->setToolTip(QString::fromUtf8("Los datos utilizados para crear GCode.\nCada herramienta almacena su propio conjunto de datos."));
    grid3->addWidget(tool_data_label, 12, 0, 1, 2);
    auto* w10 = new FCLabel(nullptr);
    w10->setText(QString::fromUtf8("Operación:"));
    w10->setToolTip(QString::fromUtf8("La 'Operación' puede ser:\n- Aislamiento -> asegurará que la limpieza sin cobre esté siempre completa.\nSi no tiene éxito, la limpieza sin cobre también fallará.\n- Borrar -> la limpieza regular sin cobre."));
    grid3->addWidget(w10, 13, 0, 1, 1);
    op_radio = new RadioSet({{QString::fromUtf8("Limpiar"), QVariant(QString::fromUtf8("clear"))}, {QString::fromUtf8("Aislamiento"), QVariant(QString::fromUtf8("iso"))}}, Qt::Horizontal, false, nullptr);
    op_radio->setProperty("objectName", QString::fromUtf8("n_operation"));
    op_radio->setValue(QVariant(QString::fromUtf8("clear")));
    grid3->addWidget(op_radio, 13, 1, 1, 1);
    milling_type_label = new FCLabel(nullptr);
    milling_type_label->setEnabled(false);
    milling_type_label->setText(QString::fromUtf8("Tipo de fresado:"));
    milling_type_label->setToolTip(QString::fromUtf8("Tipo de fresado:\n- subir / mejor para fresado de precisión y para reducir el uso de la herramienta\n- convencional / útil cuando no hay compensación de contragolpe"));
    grid3->addWidget(milling_type_label, 14, 0, 1, 1);
    milling_type_radio = new RadioSet({{QString::fromUtf8("Subida"), QVariant(QString::fromUtf8("cl"))}, {QString::fromUtf8("Convencional"), QVariant(QString::fromUtf8("cv"))}}, Qt::Horizontal, true, nullptr);
    milling_type_radio->setEnabled(false);
    milling_type_radio->setProperty("objectName", QString::fromUtf8("n_milling_type"));
    milling_type_radio->setToolTip(QString::fromUtf8("Tipo de fresado:\n- subir / mejor para fresado de precisión y para reducir el uso de la herramienta\n- convencional / útil cuando no hay compensación de contragolpe"));
    milling_type_radio->setValue(QVariant(QString::fromUtf8("cl")));
    grid3->addWidget(milling_type_radio, 14, 1, 1, 1);
    nccoverlabel = new FCLabel(nullptr);
    nccoverlabel->setText(QString::fromUtf8("Superposición:"));
    nccoverlabel->setToolTip(QString::fromUtf8("Cuánto (porcentaje) del ancho de la herramienta para superponer cada pasada de herramienta.\nAjustar el valor comenzando con valores más bajos\ny aumentarlo si las áreas que deben procesarse aún están\nno procesado.Valores más bajos = procesamiento más rápido, ejecución más rápida en CNC.\nValores más altos = procesamiento lento y ejecución lenta en CNC\ndebido a demasiados caminos."));
    grid3->addWidget(nccoverlabel, 15, 0, 1, 1);
    ncc_overlap_entry = new FCDoubleSpinner(nullptr);
    ncc_overlap_entry->setProperty("decimals", 4);
    ncc_overlap_entry->setProperty("maximum", 99.9999);
    ncc_overlap_entry->setProperty("objectName", QString::fromUtf8("n_overlap"));
    ncc_overlap_entry->setProperty("singleStep", 0.1);
    ncc_overlap_entry->setProperty("suffix", QString::fromUtf8(" %"));
    ncc_overlap_entry->setProperty("value", 40.0);
    ncc_overlap_entry->setWrapping(true);
    grid3->addWidget(ncc_overlap_entry, 15, 1, 1, 1);
    methodlabel = new FCLabel(nullptr);
    methodlabel->setText(QString::fromUtf8("Método:"));
    methodlabel->setToolTip(QString::fromUtf8("Algoritmo para la limpieza de cobre:\n- Estándar: paso fijo hacia adentro.\n- Basado en semillas: hacia afuera de la semilla.\n- Basado en líneas: líneas paralelas."));
    grid3->addWidget(methodlabel, 16, 0, 1, 1);
    ncc_method_combo = new FCComboBox2(nullptr);
    ncc_method_combo->setProperty("currentText", QString::fromUtf8("Semilla"));
    ncc_method_combo->setProperty("objectName", QString::fromUtf8("n_method"));
    ncc_method_combo->addItem(QString::fromUtf8("Estándar"));
    ncc_method_combo->addItem(QString::fromUtf8("Semilla"));
    ncc_method_combo->addItem(QString::fromUtf8("Líneas"));
    ncc_method_combo->addItem(QString::fromUtf8("Combo"));
    ncc_method_combo->setCurrentIndex(1);
    grid3->addWidget(ncc_method_combo, 16, 1, 1, 1);
    nccmarginlabel = new FCLabel(nullptr);
    nccmarginlabel->setText(QString::fromUtf8("Margen:"));
    nccmarginlabel->setToolTip(QString::fromUtf8("Margen de cuadro delimitador."));
    grid3->addWidget(nccmarginlabel, 17, 0, 1, 1);
    ncc_margin_entry = new FCDoubleSpinner(nullptr);
    ncc_margin_entry->setProperty("decimals", 4);
    ncc_margin_entry->setProperty("maximum", 10000.0);
    ncc_margin_entry->setProperty("minimum", -10000.0);
    ncc_margin_entry->setProperty("objectName", QString::fromUtf8("n_margin"));
    ncc_margin_entry->setProperty("value", 1.0);
    grid3->addWidget(ncc_margin_entry, 17, 1, 1, 1);
    ncc_connect_cb = new FCCheckBox(nullptr);
    ncc_connect_cb->setChecked(true);
    ncc_connect_cb->setProperty("objectName", QString::fromUtf8("n_connect"));
    ncc_connect_cb->setText(QString::fromUtf8("Conectar"));
    ncc_connect_cb->setToolTip(QString::fromUtf8("Dibuja líneas entre el resultado\nSegmentos para minimizar elevaciones de herramientas."));
    grid3->addWidget(ncc_connect_cb, 18, 0, 1, 1);
    ncc_contour_cb = new FCCheckBox(nullptr);
    ncc_contour_cb->setChecked(true);
    ncc_contour_cb->setProperty("objectName", QString::fromUtf8("n_contour"));
    ncc_contour_cb->setText(QString::fromUtf8("Contorno"));
    ncc_contour_cb->setToolTip(QString::fromUtf8("Corta todo el perímetro del polígono.\nPara recortar los bordes ásperos."));
    grid3->addWidget(ncc_contour_cb, 18, 1, 1, 1);
    ncc_choice_offset_cb = new FCCheckBox(nullptr);
    ncc_choice_offset_cb->setProperty("objectName", QString::fromUtf8("n_offset"));
    ncc_choice_offset_cb->setText(QString::fromUtf8("Compensar"));
    ncc_choice_offset_cb->setToolTip(QString::fromUtf8("Si se usa, agregará una compensación a las características de cobre.\nEl claro de cobre terminará a distancia\nde las características de cobre."));
    grid3->addWidget(ncc_choice_offset_cb, 19, 0, 1, 1);
    ncc_offset_spinner = new FCDoubleSpinner(nullptr);
    ncc_offset_spinner->setProperty("decimals", 4);
    ncc_offset_spinner->setProperty("maximum", 10.0);
    ncc_offset_spinner->setProperty("objectName", QString::fromUtf8("n_offset_value"));
    ncc_offset_spinner->setProperty("singleStep", 0.1);
    ncc_offset_spinner->setWrapping(true);
    grid3->addWidget(ncc_offset_spinner, 19, 1, 1, 1);
    auto* w11 = new QFrame(nullptr);
    w11->setProperty("frameShadow", 48);
    w11->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w11->setSizePolicy(sp); }
    grid3->addWidget(w11, 21, 0, 1, 2);
    apply_param_to_all = new FCButton(nullptr);
    apply_param_to_all->setIcon(QIcon(QString::fromUtf8(":/images/param_all32.png")));
    apply_param_to_all->setText(QString::fromUtf8("Aplicar Parám. a todas las herramientas"));
    apply_param_to_all->setToolTip(QString::fromUtf8("Se aplicarán los parámetros en el formulario actual\nen todas las herramientas de la tabla de herramientas."));
    grid3->addWidget(apply_param_to_all, 22, 0, 1, 2);
    auto* w12 = new QFrame(nullptr);
    w12->setProperty("frameShadow", 48);
    w12->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w12->setSizePolicy(sp); }
    grid3->addWidget(w12, 23, 0, 1, 2);
    gen_param_label = new FCLabel(nullptr);
    gen_param_label->setText(QString::fromUtf8("<b>Parámetros comunes</b>"));
    gen_param_label->setToolTip(QString::fromUtf8("Parámetros que son comunes para todas las herramientas."));
    grid3->addWidget(gen_param_label, 24, 0, 1, 2);
    ncc_rest_cb = new FCCheckBox(nullptr);
    ncc_rest_cb->setProperty("objectName", QString::fromUtf8("n_rest_machining"));
    ncc_rest_cb->setText(QString::fromUtf8("Resto"));
    ncc_rest_cb->setToolTip(QString::fromUtf8("Si está marcado, use 'mecanizado en resto'.\nBásicamente procesará cobre fuera de las características de la PCB,\nusando la herramienta más grande y continúe con las siguientes herramientas,\nde mayor a menor, para procesar las características de cobre que\nno pudo ser procesado por la herramienta anterior, hasta que haya\nno queda nada por procesar o no hay más herramientas.\n\nSi no está marcado, use el algoritmo estándar."));
    grid3->addWidget(ncc_rest_cb, 25, 0, 1, 2);
    rest_nccmarginlabel = new FCLabel(nullptr);
    rest_nccmarginlabel->setText(QString::fromUtf8("Margen:"));
    rest_nccmarginlabel->setToolTip(QString::fromUtf8("Margen de cuadro delimitador."));
    rest_nccmarginlabel->setHidden(true);
    grid3->addWidget(rest_nccmarginlabel, 26, 0, 1, 1);
    rest_ncc_margin_entry = new FCDoubleSpinner(nullptr);
    rest_ncc_margin_entry->setProperty("decimals", 4);
    rest_ncc_margin_entry->setProperty("maximum", 10000.0);
    rest_ncc_margin_entry->setProperty("minimum", -10000.0);
    rest_ncc_margin_entry->setProperty("objectName", QString::fromUtf8("n_margin"));
    rest_ncc_margin_entry->setProperty("value", 1.0);
    rest_ncc_margin_entry->setHidden(true);
    grid3->addWidget(rest_ncc_margin_entry, 26, 1, 1, 1);
    rest_ncc_connect_cb = new FCCheckBox(nullptr);
    rest_ncc_connect_cb->setChecked(true);
    rest_ncc_connect_cb->setText(QString::fromUtf8("Conectar"));
    rest_ncc_connect_cb->setToolTip(QString::fromUtf8("Dibuja líneas entre el resultado\nSegmentos para minimizar elevaciones de herramientas."));
    rest_ncc_connect_cb->setHidden(true);
    grid3->addWidget(rest_ncc_connect_cb, 27, 0, 1, 1);
    rest_ncc_contour_cb = new FCCheckBox(nullptr);
    rest_ncc_contour_cb->setChecked(true);
    rest_ncc_contour_cb->setText(QString::fromUtf8("Contorno"));
    rest_ncc_contour_cb->setToolTip(QString::fromUtf8("Corta todo el perímetro del polígono.\nPara recortar los bordes ásperos."));
    rest_ncc_contour_cb->setHidden(true);
    grid3->addWidget(rest_ncc_contour_cb, 27, 1, 1, 1);
    rest_ncc_choice_offset_cb = new FCCheckBox(nullptr);
    rest_ncc_choice_offset_cb->setText(QString::fromUtf8("Compensar"));
    rest_ncc_choice_offset_cb->setToolTip(QString::fromUtf8("Si se usa, agregará una compensación a las características de cobre.\nEl claro de cobre terminará a distancia\nde las características de cobre."));
    rest_ncc_choice_offset_cb->setHidden(true);
    grid3->addWidget(rest_ncc_choice_offset_cb, 28, 0, 1, 1);
    rest_ncc_offset_spinner = new FCDoubleSpinner(nullptr);
    rest_ncc_offset_spinner->setProperty("decimals", 4);
    rest_ncc_offset_spinner->setEnabled(false);
    rest_ncc_offset_spinner->setProperty("maximum", 10.0);
    rest_ncc_offset_spinner->setProperty("singleStep", 0.1);
    rest_ncc_offset_spinner->setWrapping(true);
    rest_ncc_offset_spinner->setHidden(true);
    grid3->addWidget(rest_ncc_offset_spinner, 28, 1, 1, 1);
    select_label = new FCLabel(nullptr);
    select_label->setText(QString::fromUtf8("Selección:"));
    select_label->setToolTip(QString::fromUtf8("Selección del área a procesar.\n- 'Sí mismo': la extensión del procesamiento se basa en el objeto que se procesa.\n- 'Selección de área': haga clic con el botón izquierdo del mouse para iniciar la selección del área a procesar.\n- 'Objeto de referencia': procesará el área especificada por otro objeto."));
    grid3->addWidget(select_label, 29, 0, 1, 1);
    select_combo = new FCComboBox2(nullptr);
    select_combo->setProperty("currentText", QString::fromUtf8("Sí mismo"));
    select_combo->setProperty("objectName", QString::fromUtf8("n_selection"));
    select_combo->addItem(QString::fromUtf8("Sí mismo"));
    select_combo->addItem(QString::fromUtf8("Selección de área"));
    select_combo->addItem(QString::fromUtf8("Objeto de referencia"));
    grid3->addWidget(select_combo, 29, 1, 1, 1);
    reference_combo_type_label = new FCLabel(nullptr);
    reference_combo_type_label->setText(QString::fromUtf8("Tipo:"));
    reference_combo_type_label->setToolTip(QString::fromUtf8("El tipo de objeto FlatCAM que se utilizará como referencia de compensación sin cobre.\nPuede ser Gerber, Excellon o Geometry."));
    reference_combo_type_label->setHidden(true);
    grid3->addWidget(reference_combo_type_label, 31, 0, 1, 1);
    reference_combo_type = new FCComboBox2(nullptr);
    reference_combo_type->setProperty("currentText", QString::fromUtf8("Gerber"));
    reference_combo_type->setHidden(true);
    reference_combo_type->addItem(QString::fromUtf8("Gerber"));
    reference_combo_type->addItem(QString::fromUtf8("Excellon"));
    reference_combo_type->addItem(QString::fromUtf8("Geometría"));
    grid3->addWidget(reference_combo_type, 31, 1, 1, 1);
    reference_combo = new FCComboBox(nullptr);
    reference_combo->setHidden(true);
    grid3->addWidget(reference_combo, 33, 0, 1, 2);
    area_shape_label = new FCLabel(nullptr);
    area_shape_label->setText(QString::fromUtf8("Forma:"));
    area_shape_label->setToolTip(QString::fromUtf8("El tipo de forma de selección utilizada para la selección de área."));
    area_shape_label->setHidden(true);
    grid3->addWidget(area_shape_label, 35, 0, 1, 1);
    area_shape_radio = new RadioSet({{QString::fromUtf8("Cuadrado"), QVariant(QString::fromUtf8("square"))}, {QString::fromUtf8("Polígono"), QVariant(QString::fromUtf8("polygon"))}}, Qt::Horizontal, true, nullptr);
    area_shape_radio->setHidden(true);
    area_shape_radio->setValue(QVariant(QString::fromUtf8("square")));
    grid3->addWidget(area_shape_radio, 35, 1, 1, 1);
    valid_cb = new FCCheckBox(nullptr);
    valid_cb->setChecked(true);
    valid_cb->setProperty("objectName", QString::fromUtf8("n_check"));
    valid_cb->setText(QString::fromUtf8("Verificar validez"));
    valid_cb->setToolTip(QString::fromUtf8("Si se marca, se verifican los diámetros de las herramientas.\nsi proporcionarán un aislamiento completo."));
    grid3->addWidget(valid_cb, 37, 0, 1, 2);
    auto* w13 = new QFrame(nullptr);
    w13->setProperty("frameShadow", 48);
    w13->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w13->setSizePolicy(sp); }
    grid3->addWidget(w13, 39, 0, 1, 2);
    grid3->setColumnStretch(1, 1);
    tools_box->addLayout(grid3, 0);
    generate_ncc_button = new FCButton(nullptr);
    { QFont f = generate_ncc_button->font(); f.setBold(true); generate_ncc_button->setFont(f); }
    generate_ncc_button->setIcon(QIcon(QString::fromUtf8(":/images/geometry32.png")));
    generate_ncc_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    generate_ncc_button->setText(QString::fromUtf8("Generar Geometría"));
    generate_ncc_button->setToolTip(QString::fromUtf8("Crear el objeto de geometría\npara enrutamiento sin cobre."));
    tools_box->addWidget(generate_ncc_button, 0);
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

std::map<std::string, QObject*> NccToolUIBase::membersByName() const {
    return {
        {"add_newtool_button", add_newtool_button},
        {"addtool_from_db_btn", addtool_from_db_btn},
        {"apply_param_to_all", apply_param_to_all},
        {"area_shape_label", area_shape_label},
        {"area_shape_radio", area_shape_radio},
        {"deltool_btn", deltool_btn},
        {"find_optimal_button", find_optimal_button},
        {"gen_param_label", gen_param_label},
        {"generate_ncc_button", generate_ncc_button},
        {"grid3", grid3},
        {"layout", layout},
        {"methodlabel", methodlabel},
        {"milling_type_label", milling_type_label},
        {"milling_type_radio", milling_type_radio},
        {"ncc_choice_offset_cb", ncc_choice_offset_cb},
        {"ncc_connect_cb", ncc_connect_cb},
        {"ncc_contour_cb", ncc_contour_cb},
        {"ncc_margin_entry", ncc_margin_entry},
        {"ncc_method_combo", ncc_method_combo},
        {"ncc_offset_spinner", ncc_offset_spinner},
        {"ncc_order_label", ncc_order_label},
        {"ncc_order_radio", ncc_order_radio},
        {"ncc_overlap_entry", ncc_overlap_entry},
        {"ncc_rest_cb", ncc_rest_cb},
        {"nccmarginlabel", nccmarginlabel},
        {"nccoverlabel", nccoverlabel},
        {"new_tooldia_entry", new_tooldia_entry},
        {"new_tooldia_lbl", new_tooldia_lbl},
        {"object_combo", object_combo},
        {"object_label", object_label},
        {"op_radio", op_radio},
        {"reference_combo", reference_combo},
        {"reference_combo_type", reference_combo_type},
        {"reference_combo_type_label", reference_combo_type_label},
        {"reset_button", reset_button},
        {"rest_ncc_choice_offset_cb", rest_ncc_choice_offset_cb},
        {"rest_ncc_connect_cb", rest_ncc_connect_cb},
        {"rest_ncc_contour_cb", rest_ncc_contour_cb},
        {"rest_ncc_margin_entry", rest_ncc_margin_entry},
        {"rest_ncc_offset_spinner", rest_ncc_offset_spinner},
        {"rest_nccmarginlabel", rest_nccmarginlabel},
        {"select_combo", select_combo},
        {"select_label", select_label},
        {"tool_data_label", tool_data_label},
        {"tool_sel_label", tool_sel_label},
        {"tools_box", tools_box},
        {"tools_frame", tools_frame},
        {"tools_table", tools_table},
        {"tools_table_label", tools_table_label},
        {"type_obj_combo_label", type_obj_combo_label},
        {"type_obj_radio", type_obj_radio},
        {"valid_cb", valid_cb},
    };
}

}  // namespace flatcam::gui
