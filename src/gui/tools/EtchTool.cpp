#include "gui/tools/EtchTool.h"

#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/ObjectCollection.h"
#include "tools/EtchCompensation.h"

namespace flatcam::gui {

EtchTool::EtchTool(App* app, QWidget* parent) : EtchToolUIBase(parent), app_(app) {
    setupUi();
    connect(compensate_btn, &QPushButton::clicked, this, [this] { compensate(); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
    connect(ratio_radio, &RadioSet::activatedCustom, this, &EtchTool::onRatioChange);
    // Onzas a mils por 1,37 y mils a micras por 25,4.
    connect(oz_entry, &QLineEdit::textChanged, this, [this](const QString& t) { convert(t, 34.798, oz_to_um_entry); });
    connect(mils_entry, &QLineEdit::textChanged, this, [this](const QString& t) { convert(t, 25.4, mils_to_um_entry); });
}

ToolInfo EtchTool::info() { return {tr_("Etch Compensation Tool"), tr_("Etch Compensation Tool")}; }

void EtchTool::setToolUi() {
    thick_entry->setValue(18.0);
    ratio_radio->setValue(QString("factor"));
    onRatioChange("factor");
    const QString cur = gerber_combo->currentText();
    gerber_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == ObjectKind::Gerber) gerber_combo->addItem(o->name());
    if (!cur.isEmpty()) gerber_combo->setValue(cur);
}

void EtchTool::onRatioChange(const QString& val) {
    etchants_label->setVisible(val == "etch_list");
    etchants_combo->setVisible(val == "etch_list");
    factor_label->setVisible(val == "factor");
    factor_entry->setVisible(val == "factor");
    offset_label->setVisible(val != "factor" && val != "etch_list");
    offset_entry->setVisible(val != "factor" && val != "etch_list");
}

void EtchTool::convert(const QString& txt, double factor, FCEntry* out) {
    const auto v = NumericExpression::evaluate(txt);
    if (!v) {
        out->setValue(QString());
        return;
    }
    out->setValue(*v * factor, appDecimals());
}

FlatCAMObj* EtchTool::compensate() {
    const QString type = ratio_radio->value().toString();
    const EtchRatio ratio = type == "factor" ? EtchRatio::Factor : type == "etch_list" ? EtchRatio::EtchList
                                                                                      : EtchRatio::Manual;
    QString name = gerber_combo->currentText();
    FlatCAMObj* src = app_->collection()->byName(name);
    if (!src || src->kind() != ObjectKind::Gerber) {
        if (name.isEmpty()) name = "None";
        app_->ui()->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Object not found"), name));
        return nullptr;
    }
    const auto factor = factor_entry->evaluated();
    const auto off = etchOffset(ratio, thick_entry->value(), factor.value_or(0.0),
                                etchants_combo->currentText().toStdString(), offset_entry->value());
    if (!off || (ratio == EtchRatio::Factor && !factor)) {
        // El original avisa y después falla (offset sin definir): no crea nada.
        app_->ui()->inform(QString("[ERROR_NOTCL] %1: \"%2\"").arg(tr_("Error while Etch Factor calculating"),
                                                                   factor_entry->text()));
        return nullptr;
    }
    const Gerber& g = static_cast<GerberObject*>(src)->gerber();
    EtchResult r = etchCompensate(g, *off, defInt("gerber_circle_steps", 64));
    auto out = std::make_unique<Gerber>();
    out->restore(std::move(r.solid), {}, std::move(r.apertures), g.intDigits(), g.fracDigits(), std::string());
    auto obj = std::make_unique<GerberObject>(name + "_comp", std::move(out));
    obj->fillColor = src->fillColor;
    obj->outlineColor = src->outlineColor;
    return app_->addObject(std::move(obj));
}

}  // namespace flatcam::gui
