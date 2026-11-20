// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/CornersToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void CornersToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Corner Markers Tool"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(trUi_(QString::fromUtf8("Corner Markers Tool")));
    layout->addWidget(w1, 0);
    auto* w2 = new FCLabel(nullptr);
    layout->addWidget(w2, 0);
    object_label = new FCLabel(nullptr);
    object_label->setText(trUi_(QString::fromUtf8("<b>GERBER:</b>")));
    object_label->setToolTip(trUi_(QString::fromUtf8("The Gerber object to which will be added corner markers.")));
    layout->addWidget(object_label, 0);
    object_combo = new FCComboBox(nullptr);
    layout->addWidget(object_combo, 0);
    auto* w3 = new QFrame(nullptr);
    w3->setProperty("frameShadow", 48);
    w3->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w3->setSizePolicy(sp); }
    layout->addWidget(w3, 0);
    points_label = new FCLabel(nullptr);
    points_label->setText(trUi_(QString::fromUtf8("<b>Locations:</b>")));
    points_label->setToolTip(trUi_(QString::fromUtf8("Locations where to place corner markers.")));
    layout->addWidget(points_label, 0);
    auto* lay4 = new QGridLayout();
    lay4->setContentsMargins(0, 0, 0, 0);
    lay4->setHorizontalSpacing(6);
    lay4->setVerticalSpacing(6);
    tl_cb = new FCCheckBox(nullptr);
    tl_cb->setText(trUi_(QString::fromUtf8("Top Left")));
    lay4->addWidget(tl_cb, 0, 0, 1, 1);
    tr_cb = new FCCheckBox(nullptr);
    tr_cb->setText(trUi_(QString::fromUtf8("Top Right")));
    lay4->addWidget(tr_cb, 0, 1, 1, 1);
    bl_cb = new FCCheckBox(nullptr);
    bl_cb->setText(trUi_(QString::fromUtf8("Bottom Left")));
    lay4->addWidget(bl_cb, 1, 0, 1, 1);
    br_cb = new FCCheckBox(nullptr);
    br_cb->setText(trUi_(QString::fromUtf8("Bottom Right")));
    lay4->addWidget(br_cb, 1, 1, 1, 1);
    layout->addLayout(lay4, 0);
    auto* w5 = new QFrame(nullptr);
    w5->setProperty("frameShadow", 48);
    w5->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w5->setSizePolicy(sp); }
    layout->addWidget(w5, 0);
    toggle_all_cb = new FCCheckBox(nullptr);
    toggle_all_cb->setText(trUi_(QString::fromUtf8("Toggle ALL")));
    layout->addWidget(toggle_all_cb, 0);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    layout->addWidget(w6, 0);
    auto* lay7 = new QGridLayout();
    lay7->setContentsMargins(0, 0, 0, 0);
    lay7->setHorizontalSpacing(6);
    lay7->setVerticalSpacing(6);
    param_label = new FCLabel(nullptr);
    param_label->setText(trUi_(QString::fromUtf8("<b>Parameters:</b>")));
    param_label->setToolTip(trUi_(QString::fromUtf8("Parameters used for this tool.")));
    lay7->addWidget(param_label, 0, 0, 1, 2);
    type_label = new FCLabel(nullptr);
    type_label->setText(trUi_(QString::fromUtf8("Type:")));
    type_label->setToolTip(trUi_(QString::fromUtf8("Shape of the marker.")));
    lay7->addWidget(type_label, 2, 0, 1, 1);
    type_radio = new RadioSet({{trUi_(QString::fromUtf8("Semi-Cross")), QVariant(QString::fromUtf8("s"))}, {trUi_(QString::fromUtf8("Cross")), QVariant(QString::fromUtf8("c"))}}, Qt::Horizontal, true, nullptr);
    type_radio->setValue(QVariant(QString::fromUtf8("s")));
    lay7->addWidget(type_radio, 2, 1, 1, 1);
    thick_label = new FCLabel(nullptr);
    thick_label->setText(trUi_(QString::fromUtf8("Thickness:")));
    thick_label->setToolTip(trUi_(QString::fromUtf8("The thickness of the line that makes the corner marker.")));
    lay7->addWidget(thick_label, 4, 0, 1, 1);
    thick_entry = new FCDoubleSpinner(nullptr);
    thick_entry->setProperty("decimals", 4);
    thick_entry->setProperty("maximum", 10.0);
    thick_entry->setProperty("singleStep", 0.0001);
    thick_entry->setProperty("value", 0.1);
    thick_entry->setWrapping(true);
    lay7->addWidget(thick_entry, 4, 1, 1, 1);
    l_label = new FCLabel(nullptr);
    l_label->setText(trUi_(QString::fromUtf8("Length:")));
    l_label->setToolTip(trUi_(QString::fromUtf8("The length of the line that makes the corner marker.")));
    lay7->addWidget(l_label, 6, 0, 1, 1);
    l_entry = new FCDoubleSpinner(nullptr);
    l_entry->setProperty("decimals", 4);
    l_entry->setProperty("maximum", 10000.0);
    l_entry->setProperty("minimum", -10000.0);
    l_entry->setProperty("singleStep", 0.0001);
    l_entry->setProperty("value", 3.0);
    lay7->addWidget(l_entry, 6, 1, 1, 1);
    margin_label = new FCLabel(nullptr);
    margin_label->setText(trUi_(QString::fromUtf8("Margin:")));
    margin_label->setToolTip(trUi_(QString::fromUtf8("Bounding box margin.")));
    lay7->addWidget(margin_label, 8, 0, 1, 1);
    margin_entry = new FCDoubleSpinner(nullptr);
    margin_entry->setProperty("decimals", 4);
    margin_entry->setProperty("maximum", 10000.0);
    margin_entry->setProperty("minimum", -10000.0);
    margin_entry->setProperty("singleStep", 0.1);
    lay7->addWidget(margin_entry, 8, 1, 1, 1);
    add_marker_button = new FCButton(nullptr);
    { QFont f = add_marker_button->font(); f.setBold(true); add_marker_button->setFont(f); }
    add_marker_button->setIcon(QIcon(QString::fromUtf8(":/images/corners_32.png")));
    add_marker_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    add_marker_button->setText(trUi_(QString::fromUtf8("Add Marker")));
    add_marker_button->setToolTip(trUi_(QString::fromUtf8("Will add corner markers to the selected Gerber file.")));
    lay7->addWidget(add_marker_button, 12, 0, 1, 2);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    lay7->addWidget(w8, 14, 0, 1, 2);
    drills_label = new FCLabel(nullptr);
    drills_label->setText(trUi_(QString::fromUtf8("<b>Drills in Corners:</b>")));
    lay7->addWidget(drills_label, 16, 0, 1, 2);
    drill_dia_label = new FCLabel(nullptr);
    drill_dia_label->setText(trUi_(QString::fromUtf8("Drill Dia:")));
    drill_dia_label->setToolTip(trUi_(QString::fromUtf8("Drill Diameter.")));
    lay7->addWidget(drill_dia_label, 18, 0, 1, 1);
    drill_dia_entry = new FCDoubleSpinner(nullptr);
    drill_dia_entry->setProperty("decimals", 4);
    drill_dia_entry->setProperty("maximum", 100.0);
    drill_dia_entry->setProperty("value", 0.5);
    drill_dia_entry->setWrapping(true);
    lay7->addWidget(drill_dia_entry, 18, 1, 1, 1);
    drill_button = new FCButton(nullptr);
    { QFont f = drill_button->font(); f.setBold(true); drill_button->setFont(f); }
    drill_button->setIcon(QIcon(QString::fromUtf8(":/images/drill32.png")));
    drill_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t\t\t"));
    drill_button->setText(trUi_(QString::fromUtf8("Create Excellon Object")));
    drill_button->setToolTip(trUi_(QString::fromUtf8("Will add drill holes in the center of the markers.")));
    lay7->addWidget(drill_button, 20, 0, 1, 2);
    lay7->setColumnStretch(1, 1);
    layout->addLayout(lay7, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    reset_button = new QPushButton(nullptr);
    { QFont f = reset_button->font(); f.setBold(true); reset_button->setFont(f); }
    reset_button->setIcon(QIcon(QString::fromUtf8(":/images/reset32.png")));
    reset_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    reset_button->setText(trUi_(QString::fromUtf8("Reset Tool")));
    reset_button->setToolTip(trUi_(QString::fromUtf8("Will reset the tool parameters.")));
    layout->addWidget(reset_button, 0);
    setLayout(layout);
}

std::map<std::string, QObject*> CornersToolUIBase::membersByName() const {
    return {
        {"add_marker_button", add_marker_button},
        {"bl_cb", bl_cb},
        {"br_cb", br_cb},
        {"drill_button", drill_button},
        {"drill_dia_entry", drill_dia_entry},
        {"drill_dia_label", drill_dia_label},
        {"drills_label", drills_label},
        {"l_entry", l_entry},
        {"l_label", l_label},
        {"layout", layout},
        {"margin_entry", margin_entry},
        {"margin_label", margin_label},
        {"object_combo", object_combo},
        {"object_label", object_label},
        {"param_label", param_label},
        {"points_label", points_label},
        {"reset_button", reset_button},
        {"thick_entry", thick_entry},
        {"thick_label", thick_label},
        {"tl_cb", tl_cb},
        {"toggle_all_cb", toggle_all_cb},
        {"tr_cb", tr_cb},
        {"type_label", type_label},
        {"type_radio", type_radio},
    };
}

}  // namespace flatcam::gui
