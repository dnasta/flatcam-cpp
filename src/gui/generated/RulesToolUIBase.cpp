// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/RulesToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void RulesToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Verificar Reglas"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Verificar Reglas"));
    layout->addWidget(w1, 0);
    grid_layout = new QGridLayout();
    grid_layout->setContentsMargins(0, 0, 0, 0);
    grid_layout->setHorizontalSpacing(6);
    grid_layout->setVerticalSpacing(6);
    gerber_title_lbl = new FCLabel(nullptr);
    gerber_title_lbl->setText(QString::fromUtf8("<b>GERBER</b>:"));
    gerber_title_lbl->setToolTip(QString::fromUtf8("Objetos de Gerber para los cuales verificar las reglas."));
    grid_layout->addWidget(gerber_title_lbl, 0, 0, 1, 2);
    all_obj_cb = new FCCheckBox(nullptr);
    grid_layout->addWidget(all_obj_cb, 0, 2, 1, 1);
    copper_t_object_lbl = new FCLabel(nullptr);
    copper_t_object_lbl->setText(QString::fromUtf8("Top:"));
    copper_t_object_lbl->setToolTip(QString::fromUtf8("El objeto de cobre Top Gerber para el que se verifican las reglas."));
    grid_layout->addWidget(copper_t_object_lbl, 1, 0, 1, 1);
    copper_t_object = new FCComboBox(nullptr);
    copper_t_object->setEnabled(false);
    grid_layout->addWidget(copper_t_object, 1, 1, 1, 1);
    copper_t_cb = new FCCheckBox(nullptr);
    grid_layout->addWidget(copper_t_cb, 1, 2, 1, 1);
    copper_b_object_lbl = new FCLabel(nullptr);
    copper_b_object_lbl->setText(QString::fromUtf8("Inferior:"));
    copper_b_object_lbl->setToolTip(QString::fromUtf8("El objeto de cobre de Gerber inferior para el que se verifican las reglas."));
    grid_layout->addWidget(copper_b_object_lbl, 2, 0, 1, 1);
    copper_b_object = new FCComboBox(nullptr);
    copper_b_object->setEnabled(false);
    grid_layout->addWidget(copper_b_object, 2, 1, 1, 1);
    copper_b_cb = new FCCheckBox(nullptr);
    grid_layout->addWidget(copper_b_cb, 2, 2, 1, 1);
    sm_t_object_lbl = new FCLabel(nullptr);
    sm_t_object_lbl->setText(QString::fromUtf8("SM Top:"));
    sm_t_object_lbl->setToolTip(QString::fromUtf8("El objeto Máscara de soldadura de Gerber superior para el que se verifican las reglas."));
    grid_layout->addWidget(sm_t_object_lbl, 3, 0, 1, 1);
    sm_t_object = new FCComboBox(nullptr);
    sm_t_object->setEnabled(false);
    grid_layout->addWidget(sm_t_object, 3, 1, 1, 1);
    sm_t_cb = new FCCheckBox(nullptr);
    grid_layout->addWidget(sm_t_cb, 3, 2, 1, 1);
    sm_b_object_lbl = new FCLabel(nullptr);
    sm_b_object_lbl->setText(QString::fromUtf8("SM Inferior:"));
    sm_b_object_lbl->setToolTip(QString::fromUtf8("El objeto de máscara de soldadura de Gerber inferior para el que se verifican las reglas."));
    grid_layout->addWidget(sm_b_object_lbl, 4, 0, 1, 1);
    sm_b_object = new FCComboBox(nullptr);
    sm_b_object->setEnabled(false);
    grid_layout->addWidget(sm_b_object, 4, 1, 1, 1);
    sm_b_cb = new FCCheckBox(nullptr);
    grid_layout->addWidget(sm_b_cb, 4, 2, 1, 1);
    ss_t_object_lbl = new FCLabel(nullptr);
    ss_t_object_lbl->setText(QString::fromUtf8("Top de serigrafía:"));
    ss_t_object_lbl->setToolTip(QString::fromUtf8("El objeto de serigrafía Top Gerber para el que se verifican las reglas."));
    grid_layout->addWidget(ss_t_object_lbl, 5, 0, 1, 1);
    ss_t_object = new FCComboBox(nullptr);
    ss_t_object->setEnabled(false);
    grid_layout->addWidget(ss_t_object, 5, 1, 1, 1);
    ss_t_cb = new FCCheckBox(nullptr);
    grid_layout->addWidget(ss_t_cb, 5, 2, 1, 1);
    ss_b_object_lbl = new FCLabel(nullptr);
    ss_b_object_lbl->setText(QString::fromUtf8("Serigrafía Inferior:"));
    ss_b_object_lbl->setToolTip(QString::fromUtf8("El objeto Serigrafía inferior de Gerber para el que se verifican las reglas."));
    grid_layout->addWidget(ss_b_object_lbl, 6, 0, 1, 1);
    ss_b_object = new FCComboBox(nullptr);
    ss_b_object->setEnabled(false);
    grid_layout->addWidget(ss_b_object, 6, 1, 1, 1);
    ss_b_cb = new FCCheckBox(nullptr);
    grid_layout->addWidget(ss_b_cb, 6, 2, 1, 1);
    outline_object_lbl = new FCLabel(nullptr);
    outline_object_lbl->setText(QString::fromUtf8("Contorno:"));
    outline_object_lbl->setToolTip(QString::fromUtf8("El objeto Esquema de Gerber (Recorte) para el que se verifican las reglas."));
    grid_layout->addWidget(outline_object_lbl, 7, 0, 1, 1);
    outline_object = new FCComboBox(nullptr);
    outline_object->setEnabled(false);
    grid_layout->addWidget(outline_object, 7, 1, 1, 1);
    out_cb = new FCCheckBox(nullptr);
    grid_layout->addWidget(out_cb, 7, 2, 1, 1);
    auto* w2 = new FCLabel(nullptr);
    grid_layout->addWidget(w2, 8, 0, 1, 3);
    excellon_title_lbl = new FCLabel(nullptr);
    excellon_title_lbl->setText(QString::fromUtf8("<b>EXCELLON</b>:"));
    excellon_title_lbl->setToolTip(QString::fromUtf8("Excellon objetos para los cuales verificar las reglas."));
    grid_layout->addWidget(excellon_title_lbl, 9, 0, 1, 3);
    e1_object_lbl = new FCLabel(nullptr);
    e1_object_lbl->setText(QString::fromUtf8("Excellon 1:"));
    e1_object_lbl->setToolTip(QString::fromUtf8("Objeto Excellon para el cual verificar las reglas.\nContiene los agujeros chapados o un contenido general del archivo Excellon."));
    grid_layout->addWidget(e1_object_lbl, 10, 0, 1, 1);
    e1_object = new FCComboBox(nullptr);
    e1_object->setEnabled(false);
    grid_layout->addWidget(e1_object, 10, 1, 1, 1);
    e1_cb = new FCCheckBox(nullptr);
    grid_layout->addWidget(e1_cb, 10, 2, 1, 1);
    e2_object_lbl = new FCLabel(nullptr);
    e2_object_lbl->setText(QString::fromUtf8("Excellon 2:"));
    e2_object_lbl->setToolTip(QString::fromUtf8("Objeto Excellon para el cual verificar las reglas.\nSostiene los agujeros no chapados."));
    grid_layout->addWidget(e2_object_lbl, 11, 0, 1, 1);
    e2_object = new FCComboBox(nullptr);
    e2_object->setEnabled(false);
    grid_layout->addWidget(e2_object, 11, 1, 1, 1);
    e2_cb = new FCCheckBox(nullptr);
    grid_layout->addWidget(e2_cb, 11, 2, 1, 1);
    auto* w3 = new FCLabel(nullptr);
    grid_layout->addWidget(w3, 12, 0, 1, 3);
    grid_layout->setColumnStretch(1, 3);
    layout->addLayout(grid_layout, 0);
    all_cb = new FCCheckBox(nullptr);
    { QFont f = all_cb->font(); f.setBold(true); all_cb->setFont(f); }
    all_cb->setStyleSheet(QString::fromUtf8("\n\t\t\tQCheckBox {font-weight: bold; color: green}\n\t\t\t"));
    all_cb->setText(QString::fromUtf8("Todas las reglas"));
    all_cb->setToolTip(QString::fromUtf8("Esto marca / desmarca todas las reglas a continuación."));
    layout->addWidget(all_cb, 0);
    form_layout_1 = new QFormLayout();
    form_layout_1->setContentsMargins(0, 0, 0, 0);
    form_layout_1->setHorizontalSpacing(6);
    form_layout_1->setVerticalSpacing(6);
    auto* w4 = new FCLabel(nullptr);
    form_layout_1->setWidget(0, QFormLayout::ItemRole(2), w4);
    trace_size_cb = new FCCheckBox(nullptr);
    trace_size_cb->setChecked(true);
    trace_size_cb->setText(QString::fromUtf8("Tamaño de traza:"));
    trace_size_cb->setToolTip(QString::fromUtf8("Esto comprueba si se cumple el tamaño mínimo para las trazas."));
    form_layout_1->setWidget(1, QFormLayout::ItemRole(2), trace_size_cb);
    trace_size_lbl = new FCLabel(nullptr);
    trace_size_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    trace_size_lbl->setToolTip(QString::fromUtf8("Tamaño de traza mínimo aceptable."));
    form_layout_1->setWidget(2, QFormLayout::ItemRole(0), trace_size_lbl);
    trace_size_entry = new FCDoubleSpinner(nullptr);
    trace_size_entry->setProperty("decimals", 4);
    trace_size_entry->setProperty("maximum", 1000.0);
    trace_size_entry->setProperty("singleStep", 0.1);
    trace_size_entry->setProperty("value", 0.25);
    form_layout_1->setWidget(2, QFormLayout::ItemRole(1), trace_size_entry);
    clearance_copper2copper_cb = new FCCheckBox(nullptr);
    clearance_copper2copper_cb->setChecked(true);
    clearance_copper2copper_cb->setText(QString::fromUtf8("Distancia de Cobre a Cobre:"));
    clearance_copper2copper_cb->setToolTip(QString::fromUtf8("Esto comprueba si la distancia mínima entre cobre\nhuellas se cumplen."));
    form_layout_1->setWidget(3, QFormLayout::ItemRole(2), clearance_copper2copper_cb);
    clearance_copper2copper_lbl = new FCLabel(nullptr);
    clearance_copper2copper_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    clearance_copper2copper_lbl->setToolTip(QString::fromUtf8("Valor mínimo de distancia aceptable."));
    form_layout_1->setWidget(4, QFormLayout::ItemRole(0), clearance_copper2copper_lbl);
    clearance_copper2copper_entry = new FCDoubleSpinner(nullptr);
    clearance_copper2copper_entry->setProperty("decimals", 4);
    clearance_copper2copper_entry->setProperty("maximum", 1000.0);
    clearance_copper2copper_entry->setProperty("singleStep", 0.1);
    clearance_copper2copper_entry->setProperty("value", 0.25);
    form_layout_1->setWidget(4, QFormLayout::ItemRole(1), clearance_copper2copper_entry);
    clearance_copper2ol_cb = new FCCheckBox(nullptr);
    clearance_copper2ol_cb->setChecked(true);
    clearance_copper2ol_cb->setText(QString::fromUtf8("Distancia de Cobre a Contorno:"));
    clearance_copper2ol_cb->setToolTip(QString::fromUtf8("Esto comprueba si la distancia mínima entre cobre\nhuellas y el esquema se cumple."));
    form_layout_1->setWidget(5, QFormLayout::ItemRole(2), clearance_copper2ol_cb);
    clearance_copper2ol_lbl = new FCLabel(nullptr);
    clearance_copper2ol_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    clearance_copper2ol_lbl->setToolTip(QString::fromUtf8("Valor mínimo de distancia aceptable."));
    form_layout_1->setWidget(6, QFormLayout::ItemRole(0), clearance_copper2ol_lbl);
    clearance_copper2ol_entry = new FCDoubleSpinner(nullptr);
    clearance_copper2ol_entry->setProperty("decimals", 4);
    clearance_copper2ol_entry->setProperty("maximum", 1000.0);
    clearance_copper2ol_entry->setProperty("singleStep", 0.1);
    clearance_copper2ol_entry->setProperty("value", 1.0);
    form_layout_1->setWidget(6, QFormLayout::ItemRole(1), clearance_copper2ol_entry);
    clearance_silk2silk_cb = new FCCheckBox(nullptr);
    clearance_silk2silk_cb->setChecked(true);
    clearance_silk2silk_cb->setText(QString::fromUtf8("Distancia de Serigrafía a Serigrafía:"));
    clearance_silk2silk_cb->setToolTip(QString::fromUtf8("Esto comprueba si la distancia mínima entre serigrafía\nhuellas y huellas de serigrafía se cumplen."));
    form_layout_1->setWidget(7, QFormLayout::ItemRole(2), clearance_silk2silk_cb);
    clearance_silk2silk_lbl = new FCLabel(nullptr);
    clearance_silk2silk_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    clearance_silk2silk_lbl->setToolTip(QString::fromUtf8("Valor mínimo de distancia aceptable."));
    form_layout_1->setWidget(8, QFormLayout::ItemRole(0), clearance_silk2silk_lbl);
    clearance_silk2silk_entry = new FCDoubleSpinner(nullptr);
    clearance_silk2silk_entry->setProperty("decimals", 4);
    clearance_silk2silk_entry->setProperty("maximum", 1000.0);
    clearance_silk2silk_entry->setProperty("singleStep", 0.1);
    clearance_silk2silk_entry->setProperty("value", 0.25);
    form_layout_1->setWidget(8, QFormLayout::ItemRole(1), clearance_silk2silk_entry);
    clearance_silk2sm_cb = new FCCheckBox(nullptr);
    clearance_silk2sm_cb->setChecked(true);
    clearance_silk2sm_cb->setText(QString::fromUtf8("Serigrafía para Soldar Máscara Distancia:"));
    clearance_silk2sm_cb->setToolTip(QString::fromUtf8("Esto comprueba si la distancia mínima entre serigrafía\nTraces y soldermask traces se cumplen."));
    form_layout_1->setWidget(9, QFormLayout::ItemRole(2), clearance_silk2sm_cb);
    clearance_silk2sm_lbl = new FCLabel(nullptr);
    clearance_silk2sm_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    clearance_silk2sm_lbl->setToolTip(QString::fromUtf8("Valor mínimo de distancia aceptable."));
    form_layout_1->setWidget(10, QFormLayout::ItemRole(0), clearance_silk2sm_lbl);
    clearance_silk2sm_entry = new FCDoubleSpinner(nullptr);
    clearance_silk2sm_entry->setProperty("decimals", 4);
    clearance_silk2sm_entry->setProperty("maximum", 1000.0);
    clearance_silk2sm_entry->setProperty("singleStep", 0.1);
    clearance_silk2sm_entry->setProperty("value", 0.25);
    form_layout_1->setWidget(10, QFormLayout::ItemRole(1), clearance_silk2sm_entry);
    clearance_silk2ol_cb = new FCCheckBox(nullptr);
    clearance_silk2ol_cb->setChecked(true);
    clearance_silk2ol_cb->setText(QString::fromUtf8("Serigrafía para Contorno Distancia:"));
    clearance_silk2ol_cb->setToolTip(QString::fromUtf8("Esto verifica si el espacio libre mínimo entre la serigrafía\nhuellas y el contorno se cumple."));
    form_layout_1->setWidget(11, QFormLayout::ItemRole(2), clearance_silk2ol_cb);
    clearance_silk2ol_lbl = new FCLabel(nullptr);
    clearance_silk2ol_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    clearance_silk2ol_lbl->setToolTip(QString::fromUtf8("Valor mínimo de distancia aceptable."));
    form_layout_1->setWidget(12, QFormLayout::ItemRole(0), clearance_silk2ol_lbl);
    clearance_silk2ol_entry = new FCDoubleSpinner(nullptr);
    clearance_silk2ol_entry->setProperty("decimals", 4);
    clearance_silk2ol_entry->setProperty("maximum", 1000.0);
    clearance_silk2ol_entry->setProperty("singleStep", 0.1);
    clearance_silk2ol_entry->setProperty("value", 1.0);
    form_layout_1->setWidget(12, QFormLayout::ItemRole(1), clearance_silk2ol_entry);
    clearance_sm2sm_cb = new FCCheckBox(nullptr);
    clearance_sm2sm_cb->setChecked(true);
    clearance_sm2sm_cb->setText(QString::fromUtf8("Astilla de máscara de soldadura mínima:"));
    clearance_sm2sm_cb->setToolTip(QString::fromUtf8("Esto verifica si la distancia mínima entre la máscara de soldadura\nrastros y rastros de máscara de soldadura se cumplen."));
    form_layout_1->setWidget(13, QFormLayout::ItemRole(2), clearance_sm2sm_cb);
    clearance_sm2sm_lbl = new FCLabel(nullptr);
    clearance_sm2sm_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    clearance_sm2sm_lbl->setToolTip(QString::fromUtf8("Valor mínimo de distancia aceptable."));
    form_layout_1->setWidget(14, QFormLayout::ItemRole(0), clearance_sm2sm_lbl);
    clearance_sm2sm_entry = new FCDoubleSpinner(nullptr);
    clearance_sm2sm_entry->setProperty("decimals", 4);
    clearance_sm2sm_entry->setProperty("maximum", 1000.0);
    clearance_sm2sm_entry->setProperty("singleStep", 0.1);
    clearance_sm2sm_entry->setProperty("value", 0.25);
    form_layout_1->setWidget(14, QFormLayout::ItemRole(1), clearance_sm2sm_entry);
    ring_integrity_cb = new FCCheckBox(nullptr);
    ring_integrity_cb->setChecked(true);
    ring_integrity_cb->setText(QString::fromUtf8("Anillo anular mínimo:"));
    ring_integrity_cb->setToolTip(QString::fromUtf8("Esto verifica si queda el anillo de cobre mínimo al perforar\nSe encuentra un agujero en una almohadilla."));
    form_layout_1->setWidget(15, QFormLayout::ItemRole(2), ring_integrity_cb);
    ring_integrity_lbl = new FCLabel(nullptr);
    ring_integrity_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    ring_integrity_lbl->setToolTip(QString::fromUtf8("Valor mínimo aceptable del anillo."));
    form_layout_1->setWidget(16, QFormLayout::ItemRole(0), ring_integrity_lbl);
    ring_integrity_entry = new FCDoubleSpinner(nullptr);
    ring_integrity_entry->setProperty("decimals", 4);
    ring_integrity_entry->setProperty("maximum", 1000.0);
    ring_integrity_entry->setProperty("singleStep", 0.1);
    ring_integrity_entry->setProperty("value", 0.3);
    form_layout_1->setWidget(16, QFormLayout::ItemRole(1), ring_integrity_entry);
    auto* w5 = new FCLabel(nullptr);
    form_layout_1->setWidget(17, QFormLayout::ItemRole(2), w5);
    clearance_d2d_cb = new FCCheckBox(nullptr);
    clearance_d2d_cb->setChecked(true);
    clearance_d2d_cb->setText(QString::fromUtf8("Distancia entre Agujeros:"));
    clearance_d2d_cb->setToolTip(QString::fromUtf8("Esto verifica si la distancia mínima entre un taladro\ny se encuentra otro taladro."));
    form_layout_1->setWidget(18, QFormLayout::ItemRole(2), clearance_d2d_cb);
    clearance_d2d_lbl = new FCLabel(nullptr);
    clearance_d2d_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    clearance_d2d_lbl->setToolTip(QString::fromUtf8("Valor mínimo de distancia aceptable."));
    form_layout_1->setWidget(19, QFormLayout::ItemRole(0), clearance_d2d_lbl);
    clearance_d2d_entry = new FCDoubleSpinner(nullptr);
    clearance_d2d_entry->setProperty("decimals", 4);
    clearance_d2d_entry->setProperty("maximum", 1000.0);
    clearance_d2d_entry->setProperty("singleStep", 0.1);
    clearance_d2d_entry->setProperty("value", 0.3);
    form_layout_1->setWidget(19, QFormLayout::ItemRole(1), clearance_d2d_entry);
    drill_size_cb = new FCCheckBox(nullptr);
    drill_size_cb->setChecked(true);
    drill_size_cb->setText(QString::fromUtf8("Tamaño del Agujero:"));
    drill_size_cb->setToolTip(QString::fromUtf8("Esto comprueba si los agujeros de perforación\nLos tamaños están por encima del umbral."));
    form_layout_1->setWidget(20, QFormLayout::ItemRole(2), drill_size_cb);
    drill_size_lbl = new FCLabel(nullptr);
    drill_size_lbl->setText(QString::fromUtf8("Valor mínimo:"));
    drill_size_lbl->setToolTip(QString::fromUtf8("Tamaño mínimo aceptable de perforación."));
    form_layout_1->setWidget(21, QFormLayout::ItemRole(0), drill_size_lbl);
    drill_size_entry = new FCDoubleSpinner(nullptr);
    drill_size_entry->setProperty("decimals", 4);
    drill_size_entry->setProperty("maximum", 1000.0);
    drill_size_entry->setProperty("singleStep", 0.1);
    drill_size_entry->setProperty("value", 0.3);
    form_layout_1->setWidget(21, QFormLayout::ItemRole(1), drill_size_entry);
    layout->addLayout(form_layout_1, 0);
    auto* lay6 = new QHBoxLayout();
    lay6->setContentsMargins(0, 0, 0, 0);
    lay6->setSpacing(6);
    run_button = new FCButton(nullptr);
    { QFont f = run_button->font(); f.setBold(true); run_button->setFont(f); }
    run_button->setIcon(QIcon(QString::fromUtf8(":/images/rules32.png")));
    run_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    run_button->setText(QString::fromUtf8("Ejecutar Reglas Verificar"));
    run_button->setToolTip(QString::fromUtf8("Panelizar el objeto especificado alrededor del cuadro especificado.\nEn otras palabras, crea múltiples copias del objeto fuente,\ndispuestos en una matriz 2D de filas y columnas."));
    lay6->addWidget(run_button, 0);
    layout->addLayout(lay6, 0);
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

std::map<std::string, QObject*> RulesToolUIBase::membersByName() const {
    return {
        {"all_cb", all_cb},
        {"all_obj_cb", all_obj_cb},
        {"clearance_copper2copper_cb", clearance_copper2copper_cb},
        {"clearance_copper2copper_entry", clearance_copper2copper_entry},
        {"clearance_copper2copper_lbl", clearance_copper2copper_lbl},
        {"clearance_copper2ol_cb", clearance_copper2ol_cb},
        {"clearance_copper2ol_entry", clearance_copper2ol_entry},
        {"clearance_copper2ol_lbl", clearance_copper2ol_lbl},
        {"clearance_d2d_cb", clearance_d2d_cb},
        {"clearance_d2d_entry", clearance_d2d_entry},
        {"clearance_d2d_lbl", clearance_d2d_lbl},
        {"clearance_silk2ol_cb", clearance_silk2ol_cb},
        {"clearance_silk2ol_entry", clearance_silk2ol_entry},
        {"clearance_silk2ol_lbl", clearance_silk2ol_lbl},
        {"clearance_silk2silk_cb", clearance_silk2silk_cb},
        {"clearance_silk2silk_entry", clearance_silk2silk_entry},
        {"clearance_silk2silk_lbl", clearance_silk2silk_lbl},
        {"clearance_silk2sm_cb", clearance_silk2sm_cb},
        {"clearance_silk2sm_entry", clearance_silk2sm_entry},
        {"clearance_silk2sm_lbl", clearance_silk2sm_lbl},
        {"clearance_sm2sm_cb", clearance_sm2sm_cb},
        {"clearance_sm2sm_entry", clearance_sm2sm_entry},
        {"clearance_sm2sm_lbl", clearance_sm2sm_lbl},
        {"copper_b_cb", copper_b_cb},
        {"copper_b_object", copper_b_object},
        {"copper_b_object_lbl", copper_b_object_lbl},
        {"copper_t_cb", copper_t_cb},
        {"copper_t_object", copper_t_object},
        {"copper_t_object_lbl", copper_t_object_lbl},
        {"drill_size_cb", drill_size_cb},
        {"drill_size_entry", drill_size_entry},
        {"drill_size_lbl", drill_size_lbl},
        {"e1_cb", e1_cb},
        {"e1_object", e1_object},
        {"e1_object_lbl", e1_object_lbl},
        {"e2_cb", e2_cb},
        {"e2_object", e2_object},
        {"e2_object_lbl", e2_object_lbl},
        {"excellon_title_lbl", excellon_title_lbl},
        {"form_layout_1", form_layout_1},
        {"gerber_title_lbl", gerber_title_lbl},
        {"grid_layout", grid_layout},
        {"layout", layout},
        {"out_cb", out_cb},
        {"outline_object", outline_object},
        {"outline_object_lbl", outline_object_lbl},
        {"reset_button", reset_button},
        {"ring_integrity_cb", ring_integrity_cb},
        {"ring_integrity_entry", ring_integrity_entry},
        {"ring_integrity_lbl", ring_integrity_lbl},
        {"run_button", run_button},
        {"sm_b_cb", sm_b_cb},
        {"sm_b_object", sm_b_object},
        {"sm_b_object_lbl", sm_b_object_lbl},
        {"sm_t_cb", sm_t_cb},
        {"sm_t_object", sm_t_object},
        {"sm_t_object_lbl", sm_t_object_lbl},
        {"ss_b_cb", ss_b_cb},
        {"ss_b_object", ss_b_object},
        {"ss_b_object_lbl", ss_b_object_lbl},
        {"ss_t_cb", ss_t_cb},
        {"ss_t_object", ss_t_object},
        {"ss_t_object_lbl", ss_t_object_lbl},
        {"trace_size_cb", trace_size_cb},
        {"trace_size_entry", trace_size_entry},
        {"trace_size_lbl", trace_size_lbl},
    };
}

}  // namespace flatcam::gui
