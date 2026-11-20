// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/PanelizeToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void PanelizeToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Panelize PCB"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(trUi_(QString::fromUtf8("Panelize PCB")));
    layout->addWidget(w1, 0);
    object_label = new FCLabel(nullptr);
    object_label->setText(trUi_(QString::fromUtf8("<b>Source Object:</b>")));
    object_label->setToolTip(trUi_(QString::fromUtf8("Specify the type of object to be panelized\nIt can be of type: Gerber, Excellon or Geometry.\nThe selection here decide the type of objects that will be\nin the Object combobox.")));
    layout->addWidget(object_label, 0);
    auto* lay2 = new QFormLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setHorizontalSpacing(6);
    lay2->setVerticalSpacing(6);
    type_object_label = new FCLabel(nullptr);
    type_object_label->setText(trUi_(QString::fromUtf8("Object Type:")));
    lay2->setWidget(0, QFormLayout::ItemRole(0), type_object_label);
    type_obj_combo = new FCComboBox(nullptr);
    type_obj_combo->setProperty("currentText", trUi_(QString::fromUtf8("Gerber")));
    type_obj_combo->addItem(trUi_(QString::fromUtf8("Gerber")));
    type_obj_combo->addItem(trUi_(QString::fromUtf8("Excellon")));
    type_obj_combo->addItem(trUi_(QString::fromUtf8("Geometry")));
    lay2->setWidget(0, QFormLayout::ItemRole(1), type_obj_combo);
    object_combo = new FCComboBox(nullptr);
    object_combo->setToolTip(trUi_(QString::fromUtf8("Object to be panelized. This means that it will\nbe duplicated in an array of rows and columns.")));
    lay2->setWidget(1, QFormLayout::ItemRole(2), object_combo);
    layout->addLayout(lay2, 0);
    auto* lay3 = new QFormLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    box_label = new FCLabel(nullptr);
    box_label->setText(trUi_(QString::fromUtf8("<b>Penelization Reference:</b>")));
    box_label->setToolTip(trUi_(QString::fromUtf8("Choose the reference for panelization:\n- Object = the bounding box of a different object\n- Bounding Box = the bounding box of the object to be panelized\n\nThe reference is useful when doing panelization for more than one\nobject. The spacings (really offsets) will be applied in reference\nto this reference object therefore maintaining the panelized\nobjects in sync.")));
    lay3->setWidget(0, QFormLayout::ItemRole(2), box_label);
    reference_radio = new RadioSet({{trUi_(QString::fromUtf8("Object")), QVariant(QString::fromUtf8("object"))}, {trUi_(QString::fromUtf8("Bounding Box")), QVariant(QString::fromUtf8("bbox"))}}, Qt::Horizontal, true, nullptr);
    reference_radio->setValue(QVariant(QString::fromUtf8("bbox")));
    lay3->setWidget(1, QFormLayout::ItemRole(2), reference_radio);
    type_box_combo_label = new FCLabel(nullptr);
    type_box_combo_label->setEnabled(false);
    type_box_combo_label->setText(trUi_(QString::fromUtf8("Box Type:")));
    type_box_combo_label->setToolTip(trUi_(QString::fromUtf8("Specify the type of object to be used as an container for\npanelization. It can be: Gerber or Geometry type.\nThe selection here decide the type of objects that will be\nin the Box Object combobox.")));
    lay3->setWidget(2, QFormLayout::ItemRole(0), type_box_combo_label);
    type_box_combo = new FCComboBox(nullptr);
    type_box_combo->setProperty("currentText", trUi_(QString::fromUtf8("Gerber")));
    type_box_combo->setEnabled(false);
    type_box_combo->addItem(trUi_(QString::fromUtf8("Gerber")));
    type_box_combo->addItem(trUi_(QString::fromUtf8("Geometry")));
    lay3->setWidget(2, QFormLayout::ItemRole(1), type_box_combo);
    box_combo = new FCComboBox(nullptr);
    box_combo->setEnabled(false);
    box_combo->setToolTip(trUi_(QString::fromUtf8("The actual object that is used as container for the\n selected object that is to be panelized.")));
    lay3->setWidget(3, QFormLayout::ItemRole(2), box_combo);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay3->setWidget(4, QFormLayout::ItemRole(2), w4);
    auto* w5 = new FCLabel(nullptr);
    w5->setText(trUi_(QString::fromUtf8("<b>Panel Data:</b>")));
    w5->setToolTip(trUi_(QString::fromUtf8("This informations will shape the resulting panel.\nThe number of rows and columns will set how many\nduplicates of the original geometry will be generated.\n\nThe spacings will set the distance between any two\nelements of the panel array.")));
    lay3->setWidget(5, QFormLayout::ItemRole(2), w5);
    spacing_columns_label = new FCLabel(nullptr);
    spacing_columns_label->setText(trUi_(QString::fromUtf8("Spacing cols:")));
    spacing_columns_label->setToolTip(trUi_(QString::fromUtf8("Spacing between columns of the desired panel.\nIn current units.")));
    lay3->setWidget(6, QFormLayout::ItemRole(0), spacing_columns_label);
    spacing_columns = new FCDoubleSpinner(nullptr);
    spacing_columns->setProperty("decimals", 4);
    spacing_columns->setProperty("maximum", 9999.0);
    lay3->setWidget(6, QFormLayout::ItemRole(1), spacing_columns);
    spacing_rows_label = new FCLabel(nullptr);
    spacing_rows_label->setText(trUi_(QString::fromUtf8("Spacing rows:")));
    spacing_rows_label->setToolTip(trUi_(QString::fromUtf8("Spacing between rows of the desired panel.\nIn current units.")));
    lay3->setWidget(7, QFormLayout::ItemRole(0), spacing_rows_label);
    spacing_rows = new FCDoubleSpinner(nullptr);
    spacing_rows->setProperty("decimals", 4);
    spacing_rows->setProperty("maximum", 9999.0);
    lay3->setWidget(7, QFormLayout::ItemRole(1), spacing_rows);
    columns_label = new FCLabel(nullptr);
    columns_label->setText(trUi_(QString::fromUtf8("Columns:")));
    columns_label->setToolTip(trUi_(QString::fromUtf8("Number of columns of the desired panel")));
    lay3->setWidget(8, QFormLayout::ItemRole(0), columns_label);
    columns = new FCSpinner(nullptr);
    columns->setProperty("maximum", 9999);
    columns->setProperty("value", 1);
    lay3->setWidget(8, QFormLayout::ItemRole(1), columns);
    rows_label = new FCLabel(nullptr);
    rows_label->setText(trUi_(QString::fromUtf8("Rows:")));
    rows_label->setToolTip(trUi_(QString::fromUtf8("Number of rows of the desired panel")));
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
    panel_type_label->setText(trUi_(QString::fromUtf8("<b>Panel Type:</b>")));
    panel_type_label->setToolTip(trUi_(QString::fromUtf8("Choose the type of object for the panel object:\n- Gerber\n- Geometry")));
    lay3->setWidget(11, QFormLayout::ItemRole(2), panel_type_label);
    panel_type_radio = new RadioSet({{trUi_(QString::fromUtf8("Gerber")), QVariant(QString::fromUtf8("gerber"))}, {trUi_(QString::fromUtf8("Geo")), QVariant(QString::fromUtf8("geometry"))}}, Qt::Horizontal, true, nullptr);
    panel_type_radio->setValue(QVariant(QString::fromUtf8("gerber")));
    lay3->setWidget(12, QFormLayout::ItemRole(2), panel_type_radio);
    optimization_cb = new FCCheckBox(nullptr);
    optimization_cb->setChecked(true);
    optimization_cb->setEnabled(false);
    optimization_cb->setText(trUi_(QString::fromUtf8("Path Optimization")));
    optimization_cb->setToolTip(trUi_(QString::fromUtf8("Active only for Geometry panel type.\nWhen checked the application will find\nany two overlapping Line elements in the panel\nand will remove the overlapping parts, keeping only one of them.")));
    lay3->setWidget(13, QFormLayout::ItemRole(2), optimization_cb);
    constrain_cb = new FCCheckBox(nullptr);
    constrain_cb->setText(trUi_(QString::fromUtf8("Constrain panel within:")));
    constrain_cb->setToolTip(trUi_(QString::fromUtf8("Area define by DX and DY within to constrain the panel.\nDX and DY values are in current units.\nRegardless of how many columns and rows are desired,\nthe final panel will have as many columns and rows as\nthey fit completely within selected area.")));
    lay3->setWidget(14, QFormLayout::ItemRole(2), constrain_cb);
    x_width_lbl = new FCLabel(nullptr);
    x_width_lbl->setEnabled(false);
    x_width_lbl->setText(trUi_(QString::fromUtf8("Width (DX):")));
    x_width_lbl->setToolTip(trUi_(QString::fromUtf8("The width (DX) within which the panel must fit.\nIn current units.")));
    lay3->setWidget(15, QFormLayout::ItemRole(0), x_width_lbl);
    x_width_entry = new FCDoubleSpinner(nullptr);
    x_width_entry->setProperty("decimals", 4);
    x_width_entry->setEnabled(false);
    x_width_entry->setProperty("maximum", 9999.0);
    x_width_entry->setProperty("value", 200.0);
    lay3->setWidget(15, QFormLayout::ItemRole(1), x_width_entry);
    y_height_lbl = new FCLabel(nullptr);
    y_height_lbl->setEnabled(false);
    y_height_lbl->setText(trUi_(QString::fromUtf8("Height (DY):")));
    y_height_lbl->setToolTip(trUi_(QString::fromUtf8("The height (DY)within which the panel must fit.\nIn current units.")));
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
    panelize_object_button->setText(trUi_(QString::fromUtf8("Panelize Object")));
    panelize_object_button->setToolTip(trUi_(QString::fromUtf8("Panelize the specified object around the specified box.\nIn other words it creates multiple copies of the source object,\narranged in a 2D array of rows and columns.")));
    layout->addWidget(panelize_object_button, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new FCButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    reset_button->setText(trUi_(QString::fromUtf8("Reset Tool")));
    reset_button->setToolTip(trUi_(QString::fromUtf8("Will reset the tool parameters.")));
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
