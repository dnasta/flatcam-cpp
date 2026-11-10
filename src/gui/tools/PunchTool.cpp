#include "gui/tools/PunchTool.h"

#include <set>

#include <QHeaderView>

#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/ObjectCollection.h"
#include "tools/PunchGerber.h"

namespace flatcam::gui {

PunchTool::PunchTool(App* app, QWidget* parent) : PunchToolUIBase(parent), app_(app) {
    setupUi();
    connect(method_punch, &RadioSet::activatedCustom, this, [this](const QString& v) { onMethod(v); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
    connect(punch_object_button, &QPushButton::clicked, this, [this] { punch(); });
    connect(gerber_object_combo, &QComboBox::currentIndexChanged, this, [this] { buildToolUi(); });
    const std::pair<FCCheckBox*, QWidget*> links[] = {{circular_cb, circular_ring_entry},
                                                      {oblong_cb, oblong_ring_entry},
                                                      {square_cb, square_ring_entry},
                                                      {rectangular_cb, rectangular_ring_entry},
                                                      {other_cb, other_ring_entry}};
    for (const auto& link : links) {
        QWidget* entry = link.second;
        connect(link.first, &QCheckBox::checkStateChanged, this, [this, entry](Qt::CheckState s) {
            entry->setDisabled(s == Qt::Unchecked);
            if (!muted_) buildToolUi();
        });
    }
    connect(select_all_cb, &QCheckBox::checkStateChanged, this,
            [this](Qt::CheckState s) { onSelectAll(s != Qt::Unchecked); });
}

ToolInfo PunchTool::info() { return {tr_("Punch Gerber"), tr_("Punch Tool")}; }

void PunchTool::setToolUi() {
    auto fill = [this](FCComboBox* combo, ObjectKind k) {
        const QString cur = combo->currentText();
        const QSignalBlocker b(combo);
        combo->clear();
        for (FlatCAMObj* o : app_->collection()->objects())
            if (o->kind() == k) combo->addItem(o->name());
        if (!cur.isEmpty()) combo->setValue(cur);
    };
    fill(gerber_object_combo, ObjectKind::Gerber);
    fill(exc_combo, ObjectKind::Excellon);
    method_punch->setValue(defVariant("tools_punch_hole_type"));
    select_all_cb->setChecked(false);
    dia_entry->setValue(defNum("tools_punch_hole_fixed_dia"));
    circular_ring_entry->setValue(defNum("tools_punch_circular_ring"));
    oblong_ring_entry->setValue(defNum("tools_punch_oblong_ring"));
    square_ring_entry->setValue(defNum("tools_punch_square_ring"));
    rectangular_ring_entry->setValue(defNum("tools_punch_rectangular_ring"));
    other_ring_entry->setValue(defNum("tools_punch_others_ring"));
    circular_cb->setChecked(defBool("tools_punch_circular"));
    oblong_cb->setChecked(defBool("tools_punch_oblong"));
    square_cb->setChecked(defBool("tools_punch_square"));
    rectangular_cb->setChecked(defBool("tools_punch_rectangular"));
    other_cb->setChecked(defBool("tools_punch_others"));
    factor_entry->setValue(defNum("tools_punch_hole_prop_factor"));
    buildToolUi();
}

FlatCAMObj* PunchTool::gerber() const {
    FlatCAMObj* o = app_->collection()->byName(gerber_object_combo->currentText());
    return o && o->kind() == ObjectKind::Gerber ? o : nullptr;
}

void PunchTool::buildToolUi() {
    apertures_table->setRowCount(0);
    FlatCAMObj* o = gerber();
    if (!o) return;
    const Gerber& g = static_cast<GerberObject*>(o)->gerber();
    // Las aperturas por número, solo las de los tipos elegidos.
    std::vector<int> codes;
    for (const auto& [k, ap] : g.apertures()) codes.push_back(std::stoi(k));
    std::sort(codes.begin(), codes.end());
    auto shown = [&](const Aperture& ap) {
        if (ap.type == "C") return circular_cb->isChecked();
        if (ap.type == "R") return square_cb->isChecked() || rectangular_cb->isChecked();
        if (ap.type == "O") return oblong_cb->isChecked();
        return other_cb->isChecked();
    };
    int row = 0;
    for (int code : codes) {
        const std::string id = std::to_string(code);
        const Aperture& ap = g.apertures().at(id);
        if (!shown(ap)) continue;
        apertures_table->setRowCount(row + 1);
        auto* codeItem = new QTableWidgetItem(QString::fromStdString(id));
        codeItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        auto* typeItem = new QTableWidgetItem(QString::fromStdString(ap.type));
        typeItem->setFlags(Qt::ItemIsEnabled);
        auto* sizeItem = new QTableWidgetItem(
            ap.size ? QString::number(QString::number(*ap.size, 'f', appDecimals()).toDouble()) : QString());
        sizeItem->setFlags(Qt::ItemIsEnabled);
        auto* empty = new QTableWidgetItem("");
        empty->setFlags(Qt::ItemIsEnabled);
        apertures_table->setItem(row, 0, codeItem);
        apertures_table->setItem(row, 1, typeItem);
        apertures_table->setItem(row, 2, sizeItem);
        apertures_table->setItem(row, 3, empty);
        auto* mark = new FCCheckBox();
        mark->setLayoutDirection(Qt::RightToLeft);
        apertures_table->setCellWidget(row, 3, mark);
        ++row;
    }
    // selectColumn(0): la tabla elige celdas (FCTable sin arrastrar y soltar), así que quedan elegidas todas las aperturas.
    apertures_table->selectColumn(0);
    apertures_table->resizeColumnsToContents();
    apertures_table->resizeRowsToContents();
    QHeaderView* h = apertures_table->horizontalHeader();
    h->setMinimumSectionSize(10);
    h->setDefaultSectionSize(70);
    h->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    h->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    h->setSectionResizeMode(2, QHeaderView::Stretch);
    h->setSectionResizeMode(3, QHeaderView::Fixed);
    h->resizeSection(3, 17);
    apertures_table->setColumnWidth(3, 17);
}

void PunchTool::onSelectAll(bool state) {
    // on_select_all: marca o desmarca todos los tipos (sin reconstruir la tabla, que el original desconecta).
    muted_ = true;
    for (FCCheckBox* c : {circular_cb, oblong_cb, square_cb, rectangular_cb, other_cb}) c->setChecked(state);
    muted_ = false;
}

void PunchTool::onMethod(const QString& v) {
    for (QWidget* w : std::initializer_list<QWidget*>{exc_label, exc_combo, fixed_label, dia_label, dia_entry, ring_frame,
                                                      prop_label, factor_label, factor_entry})
        w->hide();
    if (v == "exc") {
        exc_label->show();
        exc_combo->show();
    } else if (v == "fixed") {
        for (QWidget* w : std::initializer_list<QWidget*>{fixed_label, dia_label, dia_entry}) w->show();
    } else if (v == "ring") {
        ring_frame->show();
    } else if (v == "prop") {
        for (QWidget* w : std::initializer_list<QWidget*>{prop_label, factor_label, factor_entry}) w->show();
    }
}

FlatCAMObj* PunchTool::punch() {
    FlatCAMObj* src = gerber();
    if (!src) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("There is no Gerber object loaded ..."));
        return nullptr;
    }
    PunchSettings s;
    const QString m = method_punch->value().toString();
    s.method = m == "exc" ? PunchSettings::Method::Excellon : m == "fixed" ? PunchSettings::Method::Fixed
             : m == "ring" ? PunchSettings::Method::Ring : PunchSettings::Method::Proportional;
    const Excellon* exc = nullptr;
    if (s.method == PunchSettings::Method::Excellon) {
        FlatCAMObj* e = app_->collection()->byName(exc_combo->currentText());
        if (!e || e->kind() != ObjectKind::Excellon) {
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("There is no Excellon object loaded ..."));
            return nullptr;
        }
        exc = &static_cast<ExcellonObject*>(e)->excellon();
    }
    s.fixedDia = dia_entry->value();
    s.ringCircular = circular_ring_entry->value();
    s.ringOblong = oblong_ring_entry->value();
    s.ringSquare = square_ring_entry->value();
    s.ringRectangular = rectangular_ring_entry->value();
    s.ringOther = other_ring_entry->value();
    s.factor = factor_entry->value();
    s.circular = circular_cb->isChecked();
    s.oblong = oblong_cb->isChecked();
    s.square = square_cb->isChecked();
    s.rectangular = rectangular_cb->isChecked();
    s.other = other_cb->isChecked();
    s.decimals = appDecimals();
    std::set<std::string> sel;
    for (QTableWidgetItem* it : apertures_table->selectedItems()) sel.insert(it->text().toStdString());
    const Gerber& g = static_cast<GerberObject*>(src)->gerber();
    PunchResult r = punchGerber(g, sel, s, exc);
    switch (r.error) {
        case PunchResult::Error::ZeroDiameter:
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("The value of the fixed diameter is 0.0. Aborting."));
            return nullptr;
        case PunchResult::Error::HoleTooBig:
            app_->ui()->inform("[ERROR_NOTCL] " +
                               tr_("Failed. Punch hole size is bigger than some of the apertures in the Gerber object."));
            return nullptr;
        case PunchResult::Error::SameGeometry:
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("Failed. The new object geometry is the same as the one in the "
                                                        "source object geometry..."));
            return nullptr;
        case PunchResult::Error::OriginalFails:
            return nullptr;  // el original falla con KeyError (solo queda en el registro)
        case PunchResult::Error::None: break;
    }
    const QString name = src->name();
    const int dot = name.lastIndexOf('.');
    const QString outname = (dot < 0 ? QString() : name.left(dot)) + "_punched";
    auto out = std::make_unique<Gerber>();
    out->restore(std::move(r.solid), {}, std::move(r.apertures), g.intDigits(), g.fracDigits(), std::string());
    auto obj = std::make_unique<GerberObject>(outname, std::move(out));
    obj->fillColor = src->fillColor;
    obj->outlineColor = src->outlineColor;
    return app_->addObject(std::move(obj));
}

}  // namespace flatcam::gui
