// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/ExtractDrillsToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void ExtractDrillsToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Extract Drills"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(9, 9, 9, 9);
    layout->setSpacing(6);
    auto* w1 = new QLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(trUi_(QString::fromUtf8("Extract Drills")));
    layout->addWidget(w1, 0);
    auto* w2 = new QLabel(nullptr);
    layout->addWidget(w2, 0);
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    grb_label = new QLabel(nullptr);
    grb_label->setText(trUi_(QString::fromUtf8("<b>GERBER:</b>")));
    grb_label->setToolTip(trUi_(QString::fromUtf8("Gerber from which to extract drill holes.")));
    lay3->addWidget(grb_label, 0, 0, 1, 2);
    gerber_object_combo = new FCComboBox(nullptr);
    lay3->addWidget(gerber_object_combo, 1, 0, 1, 2);
    padt_label = new QLabel(nullptr);
    padt_label->setText(trUi_(QString::fromUtf8("<b>Processed Pads Type</b>")));
    padt_label->setToolTip(trUi_(QString::fromUtf8("The type of pads shape to be processed.\nIf the PCB has many SMD pads with rectangular pads,\ndisable the Rectangular aperture.")));
    lay3->addWidget(padt_label, 2, 0, 1, 2);
    circular_cb = new FCCheckBox(nullptr);
    circular_cb->setChecked(true);
    circular_cb->setText(trUi_(QString::fromUtf8("Circular")));
    circular_cb->setToolTip(trUi_(QString::fromUtf8("Process Circular Pads.")));
    lay3->addWidget(circular_cb, 3, 0, 1, 2);
    oblong_cb = new FCCheckBox(nullptr);
    oblong_cb->setText(trUi_(QString::fromUtf8("Oblong")));
    oblong_cb->setToolTip(trUi_(QString::fromUtf8("Process Oblong Pads.")));
    lay3->addWidget(oblong_cb, 4, 0, 1, 2);
    square_cb = new FCCheckBox(nullptr);
    square_cb->setText(trUi_(QString::fromUtf8("Square")));
    square_cb->setToolTip(trUi_(QString::fromUtf8("Process Square Pads.")));
    lay3->addWidget(square_cb, 5, 0, 1, 2);
    rectangular_cb = new FCCheckBox(nullptr);
    rectangular_cb->setText(trUi_(QString::fromUtf8("Rectangular")));
    rectangular_cb->setToolTip(trUi_(QString::fromUtf8("Process Rectangular Pads.")));
    lay3->addWidget(rectangular_cb, 6, 0, 1, 2);
    other_cb = new FCCheckBox(nullptr);
    other_cb->setText(trUi_(QString::fromUtf8("Others")));
    other_cb->setToolTip(trUi_(QString::fromUtf8("Process pads not in the categories above.")));
    lay3->addWidget(other_cb, 7, 0, 1, 2);
    auto* w4 = new QFrame(nullptr);
    w4->setProperty("frameShadow", 48);
    w4->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w4->setSizePolicy(sp); }
    lay3->addWidget(w4, 8, 0, 1, 2);
    lay3->setColumnStretch(0, 1);
    layout->addLayout(lay3, 0);
    auto* lay5 = new QGridLayout();
    lay5->setContentsMargins(0, 0, 0, 0);
    lay5->setHorizontalSpacing(6);
    lay5->setVerticalSpacing(6);
    method_label = new QLabel(nullptr);
    method_label->setText(trUi_(QString::fromUtf8("<b>Method</b>")));
    method_label->setToolTip(trUi_(QString::fromUtf8("The method for processing pads. Can be:\n- Fixed Diameter -> all holes will have a set size\n- Fixed Annular Ring -> all holes will have a set annular ring\n- Proportional -> each hole size will be a fraction of the pad size")));
    lay5->addWidget(method_label, 2, 0, 1, 2);
    hole_size_radio = new RadioSet({{trUi_(QString::fromUtf8("Fixed Diameter")), QVariant(QString::fromUtf8("fixed"))}, {trUi_(QString::fromUtf8("Proportional")), QVariant(QString::fromUtf8("prop"))}, {trUi_(QString::fromUtf8("Fixed Annular Ring")), QVariant(QString::fromUtf8("ring"))}}, Qt::Vertical, false, nullptr);
    hole_size_radio->setValue(QVariant(QString::fromUtf8("fixed")));
    lay5->addWidget(hole_size_radio, 3, 0, 1, 2);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    lay5->addWidget(w6, 5, 0, 1, 2);
    fixed_label = new QLabel(nullptr);
    fixed_label->setText(trUi_(QString::fromUtf8("<b>Fixed Diameter</b>")));
    lay5->addWidget(fixed_label, 6, 0, 1, 2);
    dia_label = new QLabel(nullptr);
    dia_label->setText(trUi_(QString::fromUtf8("Value:")));
    dia_label->setToolTip(trUi_(QString::fromUtf8("Fixed hole diameter.")));
    lay5->addWidget(dia_label, 8, 0, 1, 1);
    dia_entry = new FCDoubleSpinner(nullptr);
    dia_entry->setProperty("decimals", 4);
    dia_entry->setProperty("maximum", 10000.0);
    dia_entry->setProperty("value", 0.5);
    lay5->addWidget(dia_entry, 8, 1, 1, 1);
    lay5->setColumnStretch(1, 1);
    layout->addLayout(lay5, 0);
    ring_frame = new QFrame(nullptr);
    ring_frame->setHidden(true);
    ring_box = new QVBoxLayout();
    ring_box->setContentsMargins(0, 0, 0, 0);
    ring_box->setSpacing(6);
    auto* lay7 = new QGridLayout();
    lay7->setContentsMargins(0, 0, 0, 0);
    lay7->setHorizontalSpacing(6);
    lay7->setVerticalSpacing(6);
    ring_label = new QLabel(nullptr);
    ring_label->setText(trUi_(QString::fromUtf8("<b>Fixed Annular Ring</b>")));
    ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring.\nThe copper sliver between the hole exterior\nand the margin of the copper pad.")));
    lay7->addWidget(ring_label, 0, 0, 1, 2);
    circular_ring_label = new QLabel(nullptr);
    circular_ring_label->setText(trUi_(QString::fromUtf8("Circular:")));
    circular_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring for circular pads.")));
    lay7->addWidget(circular_ring_label, 1, 0, 1, 1);
    circular_ring_entry = new FCDoubleSpinner(nullptr);
    circular_ring_entry->setProperty("decimals", 4);
    circular_ring_entry->setProperty("maximum", 10000.0);
    circular_ring_entry->setProperty("value", 0.2);
    lay7->addWidget(circular_ring_entry, 1, 1, 1, 1);
    oblong_ring_label = new QLabel(nullptr);
    oblong_ring_label->setText(trUi_(QString::fromUtf8("Oblong:")));
    oblong_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring for oblong pads.")));
    lay7->addWidget(oblong_ring_label, 2, 0, 1, 1);
    oblong_ring_entry = new FCDoubleSpinner(nullptr);
    oblong_ring_entry->setProperty("decimals", 4);
    oblong_ring_entry->setEnabled(false);
    oblong_ring_entry->setProperty("maximum", 10000.0);
    oblong_ring_entry->setProperty("value", 0.2);
    lay7->addWidget(oblong_ring_entry, 2, 1, 1, 1);
    square_ring_label = new QLabel(nullptr);
    square_ring_label->setText(trUi_(QString::fromUtf8("Square:")));
    square_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring for square pads.")));
    lay7->addWidget(square_ring_label, 3, 0, 1, 1);
    square_ring_entry = new FCDoubleSpinner(nullptr);
    square_ring_entry->setProperty("decimals", 4);
    square_ring_entry->setEnabled(false);
    square_ring_entry->setProperty("maximum", 10000.0);
    square_ring_entry->setProperty("value", 0.2);
    lay7->addWidget(square_ring_entry, 3, 1, 1, 1);
    rectangular_ring_label = new QLabel(nullptr);
    rectangular_ring_label->setText(trUi_(QString::fromUtf8("Rectangular:")));
    rectangular_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring for rectangular pads.")));
    lay7->addWidget(rectangular_ring_label, 4, 0, 1, 1);
    rectangular_ring_entry = new FCDoubleSpinner(nullptr);
    rectangular_ring_entry->setProperty("decimals", 4);
    rectangular_ring_entry->setEnabled(false);
    rectangular_ring_entry->setProperty("maximum", 10000.0);
    rectangular_ring_entry->setProperty("value", 0.2);
    lay7->addWidget(rectangular_ring_entry, 4, 1, 1, 1);
    other_ring_label = new QLabel(nullptr);
    other_ring_label->setText(trUi_(QString::fromUtf8("Others:")));
    other_ring_label->setToolTip(trUi_(QString::fromUtf8("The size of annular ring for other pads.")));
    lay7->addWidget(other_ring_label, 5, 0, 1, 1);
    other_ring_entry = new FCDoubleSpinner(nullptr);
    other_ring_entry->setProperty("decimals", 4);
    other_ring_entry->setEnabled(false);
    other_ring_entry->setProperty("maximum", 10000.0);
    other_ring_entry->setProperty("value", 0.2);
    lay7->addWidget(other_ring_entry, 5, 1, 1, 1);
    lay7->setColumnStretch(1, 1);
    ring_box->addLayout(lay7, 0);
    ring_frame->setLayout(ring_box);
    layout->addWidget(ring_frame, 0);
    auto* lay8 = new QGridLayout();
    lay8->setContentsMargins(0, 0, 0, 0);
    lay8->setHorizontalSpacing(6);
    lay8->setVerticalSpacing(6);
    prop_label = new QLabel(nullptr);
    prop_label->setText(trUi_(QString::fromUtf8("<b>Proportional Diameter</b>")));
    prop_label->setHidden(true);
    lay8->addWidget(prop_label, 2, 0, 1, 2);
    factor_label = new QLabel(nullptr);
    factor_label->setText(trUi_(QString::fromUtf8("Value:")));
    factor_label->setToolTip(trUi_(QString::fromUtf8("Proportional Diameter.\nThe hole diameter will be a fraction of the pad size.")));
    factor_label->setHidden(true);
    lay8->addWidget(factor_label, 3, 0, 1, 1);
    factor_entry = new FCDoubleSpinner(nullptr);
    factor_entry->setProperty("decimals", 4);
    factor_entry->setProperty("maximum", 100.0);
    factor_entry->setProperty("singleStep", 0.1);
    factor_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
    factor_entry->setProperty("value", 80.0);
    factor_entry->setHidden(true);
    lay8->addWidget(factor_entry, 3, 1, 1, 1);
    auto* w9 = new QFrame(nullptr);
    w9->setProperty("frameShadow", 48);
    w9->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w9->setSizePolicy(sp); }
    lay8->addWidget(w9, 5, 0, 1, 2);
    lay8->setColumnStretch(1, 1);
    layout->addLayout(lay8, 0);
    e_drills_button = new QPushButton(nullptr);
    { QFont f = e_drills_button->font(); f.setBold(true); e_drills_button->setFont(f); }
    e_drills_button->setIcon(QIcon(QString::fromUtf8(":/images/drill16.png")));
    e_drills_button->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\t\t\tQPushButton\n\t\t\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t\t\t"));
    e_drills_button->setText(trUi_(QString::fromUtf8("Extract Drills")));
    e_drills_button->setToolTip(trUi_(QString::fromUtf8("Extract drills from a given Gerber file.")));
    layout->addWidget(e_drills_button, 0);
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

std::map<std::string, QObject*> ExtractDrillsToolUIBase::membersByName() const {
    return {
        {"circular_cb", circular_cb},
        {"circular_ring_entry", circular_ring_entry},
        {"circular_ring_label", circular_ring_label},
        {"dia_entry", dia_entry},
        {"dia_label", dia_label},
        {"e_drills_button", e_drills_button},
        {"factor_entry", factor_entry},
        {"factor_label", factor_label},
        {"fixed_label", fixed_label},
        {"gerber_object_combo", gerber_object_combo},
        {"grb_label", grb_label},
        {"hole_size_radio", hole_size_radio},
        {"layout", layout},
        {"method_label", method_label},
        {"oblong_cb", oblong_cb},
        {"oblong_ring_entry", oblong_ring_entry},
        {"oblong_ring_label", oblong_ring_label},
        {"other_cb", other_cb},
        {"other_ring_entry", other_ring_entry},
        {"other_ring_label", other_ring_label},
        {"padt_label", padt_label},
        {"prop_label", prop_label},
        {"rectangular_cb", rectangular_cb},
        {"rectangular_ring_entry", rectangular_ring_entry},
        {"rectangular_ring_label", rectangular_ring_label},
        {"reset_button", reset_button},
        {"ring_box", ring_box},
        {"ring_frame", ring_frame},
        {"ring_label", ring_label},
        {"square_cb", square_cb},
        {"square_ring_entry", square_ring_entry},
        {"square_ring_label", square_ring_label},
    };
}

}  // namespace flatcam::gui
