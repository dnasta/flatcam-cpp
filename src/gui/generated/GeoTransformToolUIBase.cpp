// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/GeoTransformToolUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

#include "gui/Translation.h"

namespace flatcam::gui {

void GeoTransformToolUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setAutoFillBackground(true);
    this->setProperty("objectName", QString::fromUtf8("Transform Tool"));
    layout = new QVBoxLayout();
    layout->setContentsMargins(11, 11, 11, 11);
    layout->setSpacing(6);
    auto* w1 = new FCLabel(nullptr);
    { QFont f = w1->font(); f.setBold(true); f.setPixelSize(16); w1->setFont(f); }
    w1->setStyleSheet(QString::fromUtf8("\n\t\t\t\t\t\t\t\tQLabel\n\t\t\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\t\tfont-size: 16px;\n\t\t\t\t\t\t\t\t\tfont-weight: bold;\n\t\t\t\t\t\t\t\t}\n\t\t\t\t\t\t\t\t"));
    w1->setText(trUi_(QString::fromUtf8("Transform Tool")));
    layout->addWidget(w1, 0);
    auto* w2 = new FCLabel(nullptr);
    layout->addWidget(w2, 0);
    auto* lay3 = new QGridLayout();
    lay3->setContentsMargins(0, 0, 0, 0);
    lay3->setHorizontalSpacing(6);
    lay3->setVerticalSpacing(6);
    auto* w4 = new FCLabel(nullptr);
    lay3->addWidget(w4, 0, 0, 1, 1);
    auto* w5 = new FCLabel(nullptr);
    w5->setText(trUi_(QString::fromUtf8("Reference:")));
    w5->setToolTip(trUi_(QString::fromUtf8("The reference point for Rotate, Skew, Scale, Mirror.\nCan be:\n- Origin -> it is the 0, 0 point\n- Selection -> the center of the bounding box of the selected objects\n- Point -> a custom point defined by X,Y coordinates\n- Min Selection -> the point (minx, miny) of the bounding box of the selection")));
    lay3->addWidget(w5, 0, 0, 1, 1);
    ref_combo = new FCComboBox(nullptr);
    ref_combo->setProperty("currentText", trUi_(QString::fromUtf8("Selection")));
    ref_combo->addItem(trUi_(QString::fromUtf8("Origin")));
    ref_combo->addItem(trUi_(QString::fromUtf8("Selection")));
    ref_combo->addItem(trUi_(QString::fromUtf8("Point")));
    ref_combo->addItem(trUi_(QString::fromUtf8("Minimum")));
    ref_combo->setCurrentIndex(1);
    lay3->addWidget(ref_combo, 0, 1, 1, 2);
    point_label = new FCLabel(nullptr);
    point_label->setText(trUi_(QString::fromUtf8("Value:")));
    point_label->setToolTip(trUi_(QString::fromUtf8("A point of reference in format X,Y.")));
    point_label->setHidden(true);
    lay3->addWidget(point_label, 1, 0, 1, 1);
    point_entry = new NumericalEvalTupleEntry(nullptr);
    point_entry->setText(trUi_(QString::fromUtf8("0, 0")));
    point_entry->setHidden(true);
    lay3->addWidget(point_entry, 1, 1, 1, 2);
    point_button = new FCButton(nullptr);
    point_button->setText(trUi_(QString::fromUtf8("Add")));
    point_button->setToolTip(trUi_(QString::fromUtf8("Add point coordinates from clipboard.")));
    point_button->setHidden(true);
    lay3->addWidget(point_button, 2, 0, 1, 3);
    auto* w6 = new QFrame(nullptr);
    w6->setProperty("frameShadow", 48);
    w6->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w6->setSizePolicy(sp); }
    lay3->addWidget(w6, 5, 0, 1, 3);
    auto* w7 = new FCLabel(nullptr);
    w7->setText(trUi_(QString::fromUtf8("<font size=3><b>Rotate</b></font>")));
    lay3->addWidget(w7, 6, 0, 1, 3);
    rotate_label = new FCLabel(nullptr);
    rotate_label->setText(trUi_(QString::fromUtf8("Angle:")));
    rotate_label->setToolTip(trUi_(QString::fromUtf8("Angle, in degrees.\nFloat number between -360 and 359.\nPositive numbers for CW motion.\nNegative numbers for CCW motion.")));
    lay3->addWidget(rotate_label, 7, 0, 1, 1);
    rotate_entry = new FCDoubleSpinner(nullptr);
    rotate_entry->setProperty("decimals", 4);
    rotate_entry->setProperty("maximum", 360.0);
    rotate_entry->setProperty("minimum", -360.0);
    rotate_entry->setProperty("singleStep", 45.0);
    rotate_entry->setProperty("value", 90.0);
    rotate_entry->setWrapping(true);
    lay3->addWidget(rotate_entry, 7, 1, 1, 1);
    rotate_button = new FCButton(nullptr);
    rotate_button->setMinimumSize(QSize(90, 0));
    rotate_button->setText(trUi_(QString::fromUtf8("Rotate")));
    rotate_button->setToolTip(trUi_(QString::fromUtf8("Rotate the selected object(s).\nThe point of reference is the middle of\nthe bounding box for all selected objects.")));
    lay3->addWidget(rotate_button, 7, 2, 1, 1);
    auto* w8 = new QFrame(nullptr);
    w8->setProperty("frameShadow", 48);
    w8->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w8->setSizePolicy(sp); }
    lay3->addWidget(w8, 8, 0, 1, 3);
    auto* w9 = new FCLabel(nullptr);
    w9->setText(trUi_(QString::fromUtf8("<font size=3><b>Skew/Shear</b></font>")));
    lay3->addWidget(w9, 9, 0, 1, 2);
    skew_link_cb = new FCCheckBox(nullptr);
    skew_link_cb->setChecked(true);
    skew_link_cb->setText(trUi_(QString::fromUtf8("Link")));
    skew_link_cb->setToolTip(trUi_(QString::fromUtf8("Link the Y entry to X entry and copy its content.")));
    lay3->addWidget(skew_link_cb, 9, 2, 1, 1);
    skewx_label = new FCLabel(nullptr);
    skewx_label->setText(trUi_(QString::fromUtf8("X angle:")));
    skewx_label->setToolTip(trUi_(QString::fromUtf8("Angle for Skew action, in degrees.\nFloat number between -360 and 360.")));
    lay3->addWidget(skewx_label, 10, 0, 1, 1);
    skewx_entry = new FCDoubleSpinner(nullptr);
    skewx_entry->setProperty("decimals", 4);
    skewx_entry->setProperty("maximum", 360.0);
    skewx_entry->setProperty("minimum", -360.0);
    lay3->addWidget(skewx_entry, 10, 1, 1, 1);
    skewx_button = new FCButton(nullptr);
    skewx_button->setMinimumSize(QSize(90, 0));
    skewx_button->setText(trUi_(QString::fromUtf8("Skew X")));
    skewx_button->setToolTip(trUi_(QString::fromUtf8("Skew/shear the selected object(s).\nThe point of reference is the middle of\nthe bounding box for all selected objects.")));
    lay3->addWidget(skewx_button, 10, 2, 1, 1);
    skewy_label = new FCLabel(nullptr);
    skewy_label->setEnabled(false);
    skewy_label->setText(trUi_(QString::fromUtf8("Y angle:")));
    skewy_label->setToolTip(trUi_(QString::fromUtf8("Angle for Skew action, in degrees.\nFloat number between -360 and 360.")));
    lay3->addWidget(skewy_label, 12, 0, 1, 1);
    skewy_entry = new FCDoubleSpinner(nullptr);
    skewy_entry->setProperty("decimals", 4);
    skewy_entry->setEnabled(false);
    skewy_entry->setProperty("maximum", 360.0);
    skewy_entry->setProperty("minimum", -360.0);
    lay3->addWidget(skewy_entry, 12, 1, 1, 1);
    skewy_button = new FCButton(nullptr);
    skewy_button->setEnabled(false);
    skewy_button->setMinimumSize(QSize(90, 0));
    skewy_button->setText(trUi_(QString::fromUtf8("Skew Y")));
    skewy_button->setToolTip(trUi_(QString::fromUtf8("Skew/shear the selected object(s).\nThe point of reference is the middle of\nthe bounding box for all selected objects.")));
    lay3->addWidget(skewy_button, 12, 2, 1, 1);
    auto* w10 = new QFrame(nullptr);
    w10->setProperty("frameShadow", 48);
    w10->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w10->setSizePolicy(sp); }
    lay3->addWidget(w10, 14, 0, 1, 3);
    auto* w11 = new FCLabel(nullptr);
    w11->setText(trUi_(QString::fromUtf8("<font size=3><b>Scale</b></font>")));
    lay3->addWidget(w11, 15, 0, 1, 2);
    scale_link_cb = new FCCheckBox(nullptr);
    scale_link_cb->setChecked(true);
    scale_link_cb->setText(trUi_(QString::fromUtf8("Link")));
    scale_link_cb->setToolTip(trUi_(QString::fromUtf8("Link the Y entry to X entry and copy its content.")));
    lay3->addWidget(scale_link_cb, 15, 2, 1, 1);
    scalex_label = new FCLabel(nullptr);
    scalex_label->setText(trUi_(QString::fromUtf8("X factor:")));
    scalex_label->setToolTip(trUi_(QString::fromUtf8("Factor for scaling on X axis.")));
    lay3->addWidget(scalex_label, 17, 0, 1, 1);
    scalex_entry = new FCDoubleSpinner(nullptr);
    scalex_entry->setProperty("decimals", 4);
    scalex_entry->setProperty("minimum", -1000000.0);
    scalex_entry->setProperty("value", 1.0);
    lay3->addWidget(scalex_entry, 17, 1, 1, 1);
    scalex_button = new FCButton(nullptr);
    scalex_button->setMinimumSize(QSize(90, 0));
    scalex_button->setText(trUi_(QString::fromUtf8("Scale X")));
    scalex_button->setToolTip(trUi_(QString::fromUtf8("Scale the selected object(s).\nThe point of reference depends on \nthe Scale reference checkbox state.")));
    lay3->addWidget(scalex_button, 17, 2, 1, 1);
    scaley_label = new FCLabel(nullptr);
    scaley_label->setEnabled(false);
    scaley_label->setText(trUi_(QString::fromUtf8("Y factor:")));
    scaley_label->setToolTip(trUi_(QString::fromUtf8("Factor for scaling on Y axis.")));
    lay3->addWidget(scaley_label, 19, 0, 1, 1);
    scaley_entry = new FCDoubleSpinner(nullptr);
    scaley_entry->setProperty("decimals", 4);
    scaley_entry->setEnabled(false);
    scaley_entry->setProperty("minimum", -1000000.0);
    scaley_entry->setProperty("value", 1.0);
    lay3->addWidget(scaley_entry, 19, 1, 1, 1);
    scaley_button = new FCButton(nullptr);
    scaley_button->setEnabled(false);
    scaley_button->setMinimumSize(QSize(90, 0));
    scaley_button->setText(trUi_(QString::fromUtf8("Scale Y")));
    scaley_button->setToolTip(trUi_(QString::fromUtf8("Scale the selected object(s).\nThe point of reference depends on \nthe Scale reference checkbox state.")));
    lay3->addWidget(scaley_button, 19, 2, 1, 1);
    auto* w12 = new QFrame(nullptr);
    w12->setProperty("frameShadow", 48);
    w12->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w12->setSizePolicy(sp); }
    lay3->addWidget(w12, 21, 0, 1, 3);
    auto* w13 = new FCLabel(nullptr);
    w13->setText(trUi_(QString::fromUtf8("<font size=3><b>Mirror (Flip)</b></font>")));
    lay3->addWidget(w13, 23, 0, 1, 3);
    auto* lay14 = new QHBoxLayout();
    lay14->setContentsMargins(0, 0, 0, 0);
    lay14->setSpacing(6);
    flipx_button = new FCButton(nullptr);
    flipx_button->setText(trUi_(QString::fromUtf8("Flip on X")));
    flipx_button->setToolTip(trUi_(QString::fromUtf8("Flip the selected object(s) over the X axis.")));
    lay14->addWidget(flipx_button, 0);
    flipy_button = new FCButton(nullptr);
    flipy_button->setText(trUi_(QString::fromUtf8("Flip on Y")));
    flipy_button->setToolTip(trUi_(QString::fromUtf8("Flip the selected object(s) over the X axis.")));
    lay14->addWidget(flipy_button, 0);
    lay3->addLayout(lay14, 25, 0, 1, 3);
    auto* w15 = new QFrame(nullptr);
    w15->setProperty("frameShadow", 48);
    w15->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w15->setSizePolicy(sp); }
    lay3->addWidget(w15, 27, 0, 1, 3);
    auto* w16 = new FCLabel(nullptr);
    w16->setText(trUi_(QString::fromUtf8("<font size=3><b>Offset</b></font>")));
    lay3->addWidget(w16, 29, 0, 1, 3);
    offx_label = new FCLabel(nullptr);
    offx_label->setText(trUi_(QString::fromUtf8("X val:")));
    offx_label->setToolTip(trUi_(QString::fromUtf8("Distance to offset on X axis. In current units.")));
    lay3->addWidget(offx_label, 31, 0, 1, 1);
    offx_entry = new FCDoubleSpinner(nullptr);
    offx_entry->setProperty("decimals", 4);
    offx_entry->setProperty("minimum", -1000000.0);
    lay3->addWidget(offx_entry, 31, 1, 1, 1);
    offx_button = new FCButton(nullptr);
    offx_button->setMinimumSize(QSize(90, 0));
    offx_button->setText(trUi_(QString::fromUtf8("Offset X")));
    offx_button->setToolTip(trUi_(QString::fromUtf8("Offset the selected object(s).\nThe point of reference is the middle of\nthe bounding box for all selected objects.\n")));
    lay3->addWidget(offx_button, 31, 2, 1, 1);
    offy_label = new FCLabel(nullptr);
    offy_label->setText(trUi_(QString::fromUtf8("Y val:")));
    offy_label->setToolTip(trUi_(QString::fromUtf8("Distance to offset on Y axis. In current units.")));
    lay3->addWidget(offy_label, 32, 0, 1, 1);
    offy_entry = new FCDoubleSpinner(nullptr);
    offy_entry->setProperty("decimals", 4);
    offy_entry->setProperty("minimum", -1000000.0);
    lay3->addWidget(offy_entry, 32, 1, 1, 1);
    offy_button = new FCButton(nullptr);
    offy_button->setMinimumSize(QSize(90, 0));
    offy_button->setText(trUi_(QString::fromUtf8("Offset Y")));
    offy_button->setToolTip(trUi_(QString::fromUtf8("Offset the selected object(s).\nThe point of reference is the middle of\nthe bounding box for all selected objects.\n")));
    lay3->addWidget(offy_button, 32, 2, 1, 1);
    auto* w17 = new QFrame(nullptr);
    w17->setProperty("frameShadow", 48);
    w17->setProperty("frameShape", 4);
    { QSizePolicy sp(QSizePolicy::Policy(1), QSizePolicy::Policy(0)); sp.setHorizontalStretch(0); sp.setVerticalStretch(0); w17->setSizePolicy(sp); }
    lay3->addWidget(w17, 34, 0, 1, 3);
    auto* w18 = new FCLabel(nullptr);
    w18->setText(trUi_(QString::fromUtf8("<font size=3><b>Buffer</b></font>")));
    lay3->addWidget(w18, 35, 0, 1, 2);
    buffer_rounded_cb = new FCCheckBox(nullptr);
    buffer_rounded_cb->setChecked(true);
    buffer_rounded_cb->setText(trUi_(QString::fromUtf8("Rounded")));
    buffer_rounded_cb->setToolTip(trUi_(QString::fromUtf8("If checked then the buffer will surround the buffered shape,\nevery corner will be rounded.\nIf not checked then the buffer will follow the exact geometry\nof the buffered shape.")));
    lay3->addWidget(buffer_rounded_cb, 35, 2, 1, 1);
    buffer_label = new FCLabel(nullptr);
    buffer_label->setText(trUi_(QString::fromUtf8("Distance:")));
    buffer_label->setToolTip(trUi_(QString::fromUtf8("A positive value will create the effect of dilation,\nwhile a negative value will create the effect of erosion.\nEach geometry element of the object will be increased\nor decreased with the 'distance'.")));
    lay3->addWidget(buffer_label, 37, 0, 1, 1);
    buffer_entry = new FCDoubleSpinner(nullptr);
    buffer_entry->setProperty("decimals", 4);
    buffer_entry->setProperty("maximum", 10000.0);
    buffer_entry->setProperty("minimum", -10000.0);
    buffer_entry->setProperty("singleStep", 0.1);
    buffer_entry->setWrapping(true);
    lay3->addWidget(buffer_entry, 37, 1, 1, 1);
    buffer_button = new FCButton(nullptr);
    buffer_button->setMinimumSize(QSize(90, 0));
    buffer_button->setText(trUi_(QString::fromUtf8("Buffer D")));
    buffer_button->setToolTip(trUi_(QString::fromUtf8("Create the buffer effect on each geometry,\nelement from the selected object, using the distance.")));
    lay3->addWidget(buffer_button, 37, 2, 1, 1);
    buffer_factor_label = new FCLabel(nullptr);
    buffer_factor_label->setText(trUi_(QString::fromUtf8("Value:")));
    buffer_factor_label->setToolTip(trUi_(QString::fromUtf8("A positive value will create the effect of dilation,\nwhile a negative value will create the effect of erosion.\nEach geometry element of the object will be increased\nor decreased to fit the 'Value'. Value is a percentage\nof the initial dimension.")));
    lay3->addWidget(buffer_factor_label, 38, 0, 1, 1);
    buffer_factor_entry = new FCDoubleSpinner(nullptr);
    buffer_factor_entry->setProperty("decimals", 4);
    buffer_factor_entry->setProperty("maximum", 1000.0);
    buffer_factor_entry->setProperty("minimum", -100.0);
    buffer_factor_entry->setProperty("suffix", trUi_(QString::fromUtf8(" %")));
    buffer_factor_entry->setProperty("value", 100.0);
    buffer_factor_entry->setWrapping(true);
    lay3->addWidget(buffer_factor_entry, 38, 1, 1, 1);
    buffer_factor_button = new FCButton(nullptr);
    buffer_factor_button->setMinimumSize(QSize(90, 0));
    buffer_factor_button->setText(trUi_(QString::fromUtf8("Buffer F")));
    buffer_factor_button->setToolTip(trUi_(QString::fromUtf8("Create the buffer effect on each geometry,\nelement from the selected object, using the factor.")));
    lay3->addWidget(buffer_factor_button, 38, 2, 1, 1);
    auto* w19 = new FCLabel(nullptr);
    lay3->addWidget(w19, 42, 0, 1, 3);
    lay3->setColumnStretch(1, 1);
    layout->addLayout(lay3, 0);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(1), QSizePolicy::Policy(7)));
    setLayout(layout);
}

std::map<std::string, QObject*> GeoTransformToolUIBase::membersByName() const {
    return {
        {"buffer_button", buffer_button},
        {"buffer_entry", buffer_entry},
        {"buffer_factor_button", buffer_factor_button},
        {"buffer_factor_entry", buffer_factor_entry},
        {"buffer_factor_label", buffer_factor_label},
        {"buffer_label", buffer_label},
        {"buffer_rounded_cb", buffer_rounded_cb},
        {"flipx_button", flipx_button},
        {"flipy_button", flipy_button},
        {"layout", layout},
        {"offx_button", offx_button},
        {"offx_entry", offx_entry},
        {"offx_label", offx_label},
        {"offy_button", offy_button},
        {"offy_entry", offy_entry},
        {"offy_label", offy_label},
        {"point_button", point_button},
        {"point_entry", point_entry},
        {"point_label", point_label},
        {"ref_combo", ref_combo},
        {"rotate_button", rotate_button},
        {"rotate_entry", rotate_entry},
        {"rotate_label", rotate_label},
        {"scale_link_cb", scale_link_cb},
        {"scalex_button", scalex_button},
        {"scalex_entry", scalex_entry},
        {"scalex_label", scalex_label},
        {"scaley_button", scaley_button},
        {"scaley_entry", scaley_entry},
        {"scaley_label", scaley_label},
        {"skew_link_cb", skew_link_cb},
        {"skewx_button", skewx_button},
        {"skewx_entry", skewx_entry},
        {"skewx_label", skewx_label},
        {"skewy_button", skewy_button},
        {"skewy_entry", skewy_entry},
        {"skewy_label", skewy_label},
    };
}

}  // namespace flatcam::gui
