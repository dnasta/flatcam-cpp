#include "gui/tools/CalculatorsTool.h"

#include <cmath>

#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"

namespace flatcam::gui {

namespace {

/// `'%.*f' % (dec, v)`.
QString fmt(double v, int dec) { return QString::asprintf("%.*f", dec, v); }

}  // namespace

CalculatorsTool::CalculatorsTool(App* app, QWidget* parent) : CalculatorsToolUIBase(parent), app_(app) {
    setupUi();
    connect(cutDepth_entry, &QDoubleSpinBox::valueChanged, this, [this] { calculateToolDia(); });
    connect(cutDepth_entry, &FCDoubleSpinner::returnPressed, this, [this] { calculateToolDia(); });
    connect(tipDia_entry, &FCDoubleSpinner::returnPressed, this, [this] { calculateToolDia(); });
    connect(tipAngle_entry, &FCSpinner::returnPressed, this, [this] { calculateToolDia(); });
    connect(calculate_vshape_button, &QPushButton::clicked, this, [this] { calculateToolDia(); });
    connect(mm_entry, &QLineEdit::editingFinished, this, [this] { calculateInch(); });
    connect(inch_entry, &QLineEdit::editingFinished, this, [this] { calculateMm(); });
    connect(calculate_plate_button, &QPushButton::clicked, this, [this] { calculateElectroplating(); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
    connect(area_sel_radio, &RadioSet::activatedCustom, this, [this](const QString& v) { onAreaRadio(v); });
}

ToolInfo CalculatorsTool::info() { return {tr_("Calculators"), tr_("Calc. Tool")}; }

void CalculatorsTool::setToolUi() {
    const int dec = appDecimals();
    mm_entry->setText(fmt(0, dec));
    inch_entry->setText(fmt(0, dec));
    pcblength_entry->setValue(defNum("tools_calc_electro_length"));
    pcbwidth_entry->setValue(defNum("tools_calc_electro_width"));
    area_entry->setValue(defNum("tools_calc_electro_area"));
    cdensity_entry->setValue(defNum("tools_calc_electro_cdensity"));
    growth_entry->setValue(defNum("tools_calc_electro_growth"));
    cvalue_entry->setValue(0.0);
    time_entry->setValue(0.0);
    tipDia_entry->setValue(defNum("tools_calc_vshape_tip_dia"));
    tipAngle_entry->setValue(defInt("tools_calc_vshape_tip_angle"));
    {
        // Como el original, cambiar la profundidad dispara el cálculo (valueChanged).
        cutDepth_entry->setValue(defNum("tools_calc_vshape_cut_z"));
    }
    effectiveToolDia_entry->setValue(0.0);
    area_sel_radio->setValue(QString("d"));
    onAreaRadio("d");
}

void CalculatorsTool::onAreaRadio(const QString& v) {
    const bool area = v == "a";
    for (QWidget* w : std::initializer_list<QWidget*>{pcbwidthlabel, pcbwidth_entry, width_unit, pcblengthlabel,
                                                      pcblength_entry, length_unit})
        w->setVisible(!area);
    for (QWidget* w : std::initializer_list<QWidget*>{area_label, area_entry, area_unit}) w->setVisible(area);
}

void CalculatorsTool::calculateToolDia() {
    const double tip = tipDia_entry->value();
    const double half = tipAngle_entry->value() / 2.0;
    double depth = cutDepth_entry->value();
    if (depth < 0) depth = -depth;
    const double dia = tip + 2 * depth * std::tan(half * M_PI / 180.0);
    effectiveToolDia_entry->setValue(QString::number(dia, 'f', appDecimals()).toDouble());  // dec_format
}

void CalculatorsTool::calculateInch() {
    const auto v = mm_entry->evaluated();
    if (!v) return;  // float(None): TypeError en el original, no hace nada
    inch_entry->setText(fmt(*v / 25.4, appDecimals()));
}

void CalculatorsTool::calculateMm() {
    const auto v = inch_entry->evaluated();
    if (!v) return;
    mm_entry->setText(fmt(*v * 25.4, appDecimals()));
}

void CalculatorsTool::calculateElectroplating() {
    const double length = pcblength_entry->value(), width = pcbwidth_entry->value(), area = area_entry->value();
    const double density = cdensity_entry->value(), copper = growth_entry->value();
    const double current = area_sel_radio->value().toString() == "d" ? (length * width * density) * 0.0021527820833419
                                                                    : (area * density) * 0.0021527820833419;
    const double time = copper * 2.142857142857143 * (20 / density);
    cvalue_entry->setValue(fmt(current, 2).toDouble());  // set_value('%.2f' % ...)
    time_entry->setValue(fmt(time, 1).toDouble());
}

}  // namespace flatcam::gui
