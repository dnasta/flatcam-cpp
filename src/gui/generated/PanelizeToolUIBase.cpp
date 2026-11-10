// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/PanelizeToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void PanelizeToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Panelizar PCB"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(QString::fromUtf8("Panelizar PCB"));
    layout->addWidget(w1, 0);
    object_label = new FCLabel(nullptr);
    object_label->setText(QString::fromUtf8("<b>Objeto fuente:</b>"));
    object_label->setToolTip(QString::fromUtf8("Especifique el tipo de objeto a ser panelizado\nPuede ser de tipo: Gerber, Excellon o Geometry.\nLa selección aquí decide el tipo de objetos que serán\nen el cuadro combinado Objeto."));
    layout->addWidget(object_label, 0);
    auto* lay2 = new QFormLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    type_object_label = new FCLabel(nullptr);
    type_object_label->setText(QString::fromUtf8("Tipo de objeto:"));
    lay2->setWidget(0, QFormLayout::ItemRole(0), type_object_label);
    type_obj_combo = new FCComboBox(nullptr);
    type_obj_combo->setProperty("currentText", QString::fromUtf8("Gerber"));
    type_obj_combo->addItem(QString::fromUtf8("Gerber"));
    type_obj_combo->addItem(QString::fromUtf8("Excellon"));
    type_obj_combo->addItem(QString::fromUtf8("Geometry"));
    lay2->setWidget(0, QFormLayout::ItemRole(1), type_obj_combo);
    object_combo = new FCComboBox(nullptr);
    object_combo->setToolTip(QString::fromUtf8("Objeto a ser panelizado. Esto significa que lo hará\nser duplicado en una matriz de filas y columnas."));
    lay2->setWidget(1, QFormLayout::ItemRole(2), object_combo);
    layout->addLayout(lay2, 0);
    auto* lay3 = new QFormLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    box_label = new FCLabel(nullptr);
    box_label->setText(QString::fromUtf8("<b>Ref. de penelización:</b>"));
    box_label->setToolTip(QString::fromUtf8("Elija la referencia para la panelización:\n- Objeto = el cuadro delimitador de un objeto diferente\n- Cuadro delimitador = el cuadro delimitador del objeto a panelizar\n\nLa referencia es útil cuando se hace panelización para más de uno.\nobjeto. Los espacios (realmente desplazados) se aplicarán en referencia\na este objeto de referencia, por lo tanto, manteniendo el panelizado\nobjetos sincronizados."));
    lay3->setWidget(0, QFormLayout::ItemRole(2), box_label);
    reference_radio = new RadioSet({{QString::fromUtf8("Objeto"), QVariant(QString::fromUtf8("object"))}, {QString::fromUtf8("Cuadro delimitador"), QVariant(QString::fromUtf8("bbox"))}}, Qt::Horizontal, true, nullptr);
    reference_radio->setValue(QVariant(QString::fromUtf8("bbox")));
    lay3->setWidget(1, QFormLayout::ItemRole(2), reference_radio);
    type_box_combo_label = new FCLabel(nullptr);
    type_box_combo_label->setEnabled(false);
    type_box_combo_label->setText(QString::fromUtf8("Tipo de caja:"));
    type_box_combo_label->setToolTip(QString::fromUtf8("Especifique el tipo de objeto que se utilizará como contenedor para\npanelización. Puede ser: tipo Gerber o Geometría.\nLa selección aquí decide el tipo de objetos que serán\nen el cuadro combinado Objeto de caja."));
    lay3->setWidget(2, QFormLayout::ItemRole(0), type_box_combo_label);
    type_box_combo = new FCComboBox(nullptr);
    type_box_combo->setProperty("currentText", QString::fromUtf8("Gerber"));
    type_box_combo->setEnabled(false);
    type_box_combo->addItem(QString::fromUtf8("Gerber"));
    type_box_combo->addItem(QString::fromUtf8("Geometría"));
    lay3->setWidget(2, QFormLayout::ItemRole(1), type_box_combo);
    box_combo = new FCComboBox(nullptr);
    box_combo->setEnabled(false);
    box_combo->setToolTip(QString::fromUtf8("El objeto real que se utiliza como contenedor para\n objeto seleccionado que se va a panelizar."));
    lay3->setWidget(3, QFormLayout::ItemRole(2), box_combo);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay3->setWidget(4, QFormLayout::ItemRole(2), w4);
    auto* w5 = new FCLabel(nullptr);
    w5->setText(QString::fromUtf8("<b>Datos del panel:</b>"));
    w5->setToolTip(QString::fromUtf8("Esta información dará forma al panel resultante.\nEl número de filas y columnas establecerá cuántos\nSe generarán duplicados de la geometría original.\n\nLos espacios establecerán la distancia entre dos\nelementos de la matriz de paneles."));
    lay3->setWidget(5, QFormLayout::ItemRole(2), w5);
    spacing_columns_label = new FCLabel(nullptr);
    spacing_columns_label->setText(QString::fromUtf8("Col. de espaciado:"));
    spacing_columns_label->setToolTip(QString::fromUtf8("Espaciado entre columnas del panel deseado.\nEn unidades actuales."));
    lay3->setWidget(6, QFormLayout::ItemRole(0), spacing_columns_label);
    spacing_columns = new FCDoubleSpinner(nullptr);
    spacing_columns->setProperty("decimals", 4);
    spacing_columns->setProperty("maximum", 9999.0);
    lay3->setWidget(6, QFormLayout::ItemRole(1), spacing_columns);
    spacing_rows_label = new FCLabel(nullptr);
    spacing_rows_label->setText(QString::fromUtf8("Separación de filas:"));
    spacing_rows_label->setToolTip(QString::fromUtf8("Espaciado entre filas del panel deseado.\nEn unidades actuales."));
    lay3->setWidget(7, QFormLayout::ItemRole(0), spacing_rows_label);
    spacing_rows = new FCDoubleSpinner(nullptr);
    spacing_rows->setProperty("decimals", 4);
    spacing_rows->setProperty("maximum", 9999.0);
    lay3->setWidget(7, QFormLayout::ItemRole(1), spacing_rows);
    columns_label = new FCLabel(nullptr);
    columns_label->setText(QString::fromUtf8("Columnas:"));
    columns_label->setToolTip(QString::fromUtf8("Número de columnas del panel deseado"));
    lay3->setWidget(8, QFormLayout::ItemRole(0), columns_label);
    columns = new FCSpinner(nullptr);
    columns->setProperty("maximum", 9999);
    columns->setProperty("value", 1);
    lay3->setWidget(8, QFormLayout::ItemRole(1), columns);
    rows_label = new FCLabel(nullptr);
    rows_label->setText(QString::fromUtf8("Filas:"));
    rows_label->setToolTip(QString::fromUtf8("Número de filas del panel deseado"));
    lay3->setWidget(9, QFormLayout::ItemRole(0), rows_label);
    rows = new FCSpinner(nullptr);
    rows->setProperty("maximum", 9999);
    rows->setProperty("value", 1);
    lay3->setWidget(9, QFormLayout::ItemRole(1), rows);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    lay3->setWidget(10, QFormLayout::ItemRole(2), w6);
    panel_type_label = new FCLabel(nullptr);
    panel_type_label->setText(QString::fromUtf8("<b>Tipo de panel:</b>"));
    panel_type_label->setToolTip(QString::fromUtf8("Elija el tipo de objeto para el objeto del panel:\n- Gerber\n- Geometría"));
    lay3->setWidget(11, QFormLayout::ItemRole(2), panel_type_label);
    panel_type_radio = new RadioSet({{QString::fromUtf8("Gerber"), QVariant(QString::fromUtf8("gerber"))}, {QString::fromUtf8("Geo"), QVariant(QString::fromUtf8("geometry"))}}, Qt::Horizontal, true, nullptr);
    panel_type_radio->setValue(QVariant(QString::fromUtf8("gerber")));
    lay3->setWidget(12, QFormLayout::ItemRole(2), panel_type_radio);
    optimization_cb = new FCCheckBox(nullptr);
    optimization_cb->setChecked(true);
    optimization_cb->setEnabled(false);
    optimization_cb->setText(QString::fromUtf8("Optimización de ruta"));
    optimization_cb->setToolTip(QString::fromUtf8("Activo solo para el tipo de panel Geometría.\nCuando se marca, la aplicación encontrará\ncualesquiera dos elementos de línea superpuestos en el panel\ny eliminará las partes superpuestas, conservando solo una de ellas."));
    lay3->setWidget(13, QFormLayout::ItemRole(2), optimization_cb);
    constrain_cb = new FCCheckBox(nullptr);
    constrain_cb->setText(QString::fromUtf8("Restrinja el panel dentro de:"));
    constrain_cb->setToolTip(QString::fromUtf8("Área definida por DX y DY dentro para restringir el panel.\nLos valores DX y DY están en unidades actuales.\nIndependientemente de cuántas columnas y filas se deseen,\nEl panel final tendrá tantas columnas y filas como\nencajan completamente dentro del área seleccionada."));
    lay3->setWidget(14, QFormLayout::ItemRole(2), constrain_cb);
    x_width_lbl = new FCLabel(nullptr);
    x_width_lbl->setEnabled(false);
    x_width_lbl->setText(QString::fromUtf8("Ancho (DX):"));
    x_width_lbl->setToolTip(QString::fromUtf8("El ancho (DX) dentro del cual debe caber el panel.\nEn unidades actuales."));
    lay3->setWidget(15, QFormLayout::ItemRole(0), x_width_lbl);
    x_width_entry = new FCDoubleSpinner(nullptr);
    x_width_entry->setProperty("decimals", 4);
    x_width_entry->setEnabled(false);
    x_width_entry->setProperty("maximum", 9999.0);
    x_width_entry->setProperty("value", 200.0);
    lay3->setWidget(15, QFormLayout::ItemRole(1), x_width_entry);
    y_height_lbl = new FCLabel(nullptr);
    y_height_lbl->setEnabled(false);
    y_height_lbl->setText(QString::fromUtf8("Altura (DY):"));
    y_height_lbl->setToolTip(QString::fromUtf8("La altura (DY) dentro de la cual debe caber el panel.\nEn unidades actuales."));
    lay3->setWidget(16, QFormLayout::ItemRole(0), y_height_lbl);
    y_height_entry = new FCDoubleSpinner(nullptr);
    y_height_entry->setProperty("decimals", 4);
    y_height_entry->setEnabled(false);
    y_height_entry->setProperty("maximum", 9999.0);
    y_height_entry->setProperty("value", 290.0);
    lay3->setWidget(16, QFormLayout::ItemRole(1), y_height_entry);
    auto* w7 = new QFrame(nullptr);
    w7->setProperty("frameShadow", 48);
    w7->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w7->setSizePolicy(sp); }
    lay3->setWidget(17, QFormLayout::ItemRole(2), w7);
    layout->addLayout(lay3, 0);
    panelize_object_button = new FCButton(nullptr);
    { QFont f = panelize_object_button->font(); f.setBold(true); panelize_object_button->setFont(f); }
    panelize_object_button->setIcon(QIcon(QString::fromUtf8(":/images/panelize16.png")));
    panelize_object_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    panelize_object_button->setText(QString::fromUtf8("Panelizar objeto"));
    panelize_object_button->setToolTip(QString::fromUtf8("Panelizar el objeto especificado alrededor del cuadro especificado.\nEn otras palabras, crea múltiples copias del objeto fuente,\ndispuestos en una matriz 2D de filas y columnas."));
    layout->addWidget(panelize_object_button, 0);
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

std::map<std::string, QObject*> PanelizeToolUIBase::membersByName() const {
    return {
        {"box_combo", box_combo},
        {"box_label", box_label},
        {"columns", columns},
        {"columns_label", columns_label},
        {"constrain_cb", constrain_cb},
        {"layout", layout},
        {"object_combo", object_combo},
        {"object_label", object_label},
        {"optimization_cb", optimization_cb},
        {"panel_type_label", panel_type_label},
        {"panel_type_radio", panel_type_radio},
        {"panelize_object_button", panelize_object_button},
        {"reference_radio", reference_radio},
        {"reset_button", reset_button},
        {"rows", rows},
        {"rows_label", rows_label},
        {"spacing_columns", spacing_columns},
        {"spacing_columns_label", spacing_columns_label},
        {"spacing_rows", spacing_rows},
        {"spacing_rows_label", spacing_rows_label},
        {"type_box_combo", type_box_combo},
        {"type_box_combo_label", type_box_combo_label},
        {"type_obj_combo", type_obj_combo},
        {"type_object_label", type_object_label},
        {"x_width_entry", x_width_entry},
        {"x_width_lbl", x_width_lbl},
        {"y_height_entry", y_height_entry},
        {"y_height_lbl", y_height_lbl},
    };
}

}  // namespace flatcam::gui
