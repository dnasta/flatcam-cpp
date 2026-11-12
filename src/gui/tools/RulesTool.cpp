#include "gui/tools/RulesTool.h"

#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/DocumentObject.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/ObjectCollection.h"
#include "tools/RulesCheck.h"

namespace flatcam::gui {

RulesTool::RulesTool(App* app, QWidget* parent) : RulesToolUIBase(parent), app_(app) {
    setupUi();
    const std::vector<std::pair<FCCheckBox*, QWidget*>> objCbs = {
        {copper_t_cb, copper_t_object}, {copper_b_cb, copper_b_object}, {sm_t_cb, sm_t_object},
        {sm_b_cb, sm_b_object},         {ss_t_cb, ss_t_object},         {ss_b_cb, ss_b_object},
        {out_cb, outline_object},       {e1_cb, e1_object},             {e2_cb, e2_object}};
    for (const auto& [cb, w] : objCbs) {
        QWidget* target = w;
        connect(cb, &QCheckBox::checkStateChanged, this, [target](Qt::CheckState s) { target->setDisabled(s == Qt::Unchecked); });
    }
    connect(all_obj_cb, &QCheckBox::checkStateChanged, this, [objCbs](Qt::CheckState s) {
        for (const auto& [cb, w] : objCbs) cb->setChecked(s != Qt::Unchecked);
    });
    connect(all_cb, &QCheckBox::checkStateChanged, this, [this](Qt::CheckState s) {
        for (FCCheckBox* cb : {trace_size_cb, clearance_copper2copper_cb, clearance_copper2ol_cb, clearance_silk2silk_cb,
                               clearance_silk2sm_cb, clearance_silk2ol_cb, clearance_sm2sm_cb, ring_integrity_cb,
                               clearance_d2d_cb, drill_size_cb})
            cb->setChecked(s != Qt::Unchecked);
    });
    connect(run_button, &QPushButton::clicked, this, [this] { execute(); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
}

ToolInfo RulesTool::info() { return {tr_("Check Rules"), tr_("Check Rules")}; }

void RulesTool::setToolUi() {
    auto fill = [this](FCComboBox* c, ObjectKind k) {
        const QString cur = c->currentText();
        c->clear();
        for (FlatCAMObj* o : app_->collection()->objects())
            if (o->kind() == k) c->addItem(o->name());
        if (!cur.isEmpty() && c->findText(cur) >= 0) c->setValue(cur);
    };
    for (FCComboBox* c : {copper_t_object, copper_b_object, sm_t_object, sm_b_object, ss_t_object, ss_b_object, outline_object})
        fill(c, ObjectKind::Gerber);
    for (FCComboBox* c : {e1_object, e2_object}) fill(c, ObjectKind::Excellon);
    const std::vector<std::tuple<FCCheckBox*, FCDoubleSpinner*, const char*>> rules = {
        {trace_size_cb, trace_size_entry, "tools_cr_trace_size"}, {clearance_copper2copper_cb, clearance_copper2copper_entry, "tools_cr_c2c"},
        {clearance_copper2ol_cb, clearance_copper2ol_entry, "tools_cr_c2o"}, {clearance_silk2silk_cb, clearance_silk2silk_entry, "tools_cr_s2s"},
        {clearance_silk2sm_cb, clearance_silk2sm_entry, "tools_cr_s2sm"}, {clearance_silk2ol_cb, clearance_silk2ol_entry, "tools_cr_s2o"},
        {clearance_sm2sm_cb, clearance_sm2sm_entry, "tools_cr_sm2sm"}, {ring_integrity_cb, ring_integrity_entry, "tools_cr_ri"},
        {clearance_d2d_cb, clearance_d2d_entry, "tools_cr_h2h"}, {drill_size_cb, drill_size_entry, "tools_cr_dh"}};
    for (const auto& [cb, e, key] : rules) {
        cb->setChecked(defBool(key));
        e->setValue(defNum(std::string(key) == "tools_cr_trace_size" ? "tools_cr_trace_size_val" : std::string(key) + "_val"));
    }
}

FlatCAMObj* RulesTool::execute() {
    MainGUI* ui = app_->ui();
    ObjectCollection* col = app_->collection();
    auto gerber = [col](FCComboBox* c) -> std::optional<RulesGerber> {
        auto* o = dynamic_cast<GerberObject*>(col->byName(c->currentText()));
        if (!o) return std::nullopt;
        return RulesGerber{o->name().toStdString(), o->gerber().apertures(), o->gerber().apertureOrder()};
    };
    auto excellon = [col](FCComboBox* c) -> std::optional<RulesExcellon> {
        auto* o = dynamic_cast<ExcellonObject*>(col->byName(c->currentText()));
        if (!o) return std::nullopt;
        RulesExcellon r{o->name().toStdString(), {}};
        for (const auto& [id, t] : o->excellon().drillTools()) r.tools.push_back({t.tooldia, t.solidGeometry});
        return r;
    };
    auto use = [](FCCheckBox* cb, FCComboBox* c) { return cb->isChecked() && !c->currentText().isEmpty(); };
    std::vector<RulesResult> res;
    if (trace_size_cb->isChecked()) {
        std::vector<RulesGerber> l;
        if (use(copper_t_cb, copper_t_object)) if (auto g = gerber(copper_t_object)) l.push_back(*g);
        if (use(copper_b_cb, copper_b_object)) if (auto g = gerber(copper_b_object)) l.push_back(*g);
        auto r = rulesTracesSize(l, trace_size_entry->value());
        // El informe solo usa el primer elemento de cada regla.
        RulesResult first;
        first.title = tr_("Trace Size").toStdString();
        if (!r.empty()) {
            first = r.front();
            first.title = tr_("Trace Size").toStdString();
        }
        res.push_back(first);
    }
    // "Cobre a cobre" y "Serigrafía a serigrafía" leen self.copper_t_cb / self.ss_t_cb, que no existen: el original
    // falla en su hilo y no hay informe (la regla de la máscara depende de la de serigrafía).
    if (clearance_copper2copper_cb->isChecked() || clearance_silk2silk_cb->isChecked()) return nullptr;
    if (clearance_copper2ol_cb->isChecked() && out_cb->isChecked()) {
        std::vector<RulesGerber> objs;
        if (use(copper_t_cb, copper_t_object)) if (auto g = gerber(copper_t_object)) objs.push_back(*g);
        if (use(copper_b_cb, copper_b_object)) if (auto g = gerber(copper_b_object)) objs.push_back(*g);
        auto outl = use(out_cb, outline_object) ? gerber(outline_object) : std::nullopt;
        if (objs.empty() || !outl) {
            ui->inform("[ERROR_NOTCL] " + tr_("Copper to Outline clearance") + ". " +
                       tr_("One of the copper Gerber objects or the Outline Gerber object is not valid."));
            return nullptr;
        }
        objs.push_back(*outl);
        res.push_back(rulesGerberClearance(objs, clearance_copper2ol_entry->value(), tr_("Copper to Outline clearance").toStdString()));
    }
    if (clearance_silk2sm_cb->isChecked()) {
        auto st = use(ss_t_cb, ss_t_object) ? gerber(ss_t_object) : std::nullopt;
        auto sb = use(ss_b_cb, ss_b_object) ? gerber(ss_b_object) : std::nullopt;
        auto mt = use(sm_t_cb, sm_t_object) ? gerber(sm_t_object) : std::nullopt;
        auto mb = use(sm_b_cb, sm_b_object) ? gerber(sm_b_object) : std::nullopt;
        if ((!st && !sb) || (!mt && !mb)) {
            ui->inform("[ERROR_NOTCL] " + tr_("Silk to Solder Mask Clearance") + ". " + tr_("One or more of the Gerber objects is not valid."));
            return nullptr;
        }
        if (st && mt) {
            res.push_back(rulesGerberClearance({*st, *mt}, clearance_silk2sm_entry->value(), tr_("TOP -> Silk to Solder Mask Clearance").toStdString()));
        } else if (sb && mb) {
            res.push_back(rulesGerberClearance({*sb, *mb}, clearance_silk2sm_entry->value(), tr_("BOTTOM -> Silk to Solder Mask Clearance").toStdString()));
        } else {
            ui->inform("[ERROR_NOTCL] " + tr_("Silk to Solder Mask Clearance") + ". " +
                       tr_("Both Silk and Solder Mask Gerber objects has to be either both Top or both Bottom."));
            return nullptr;
        }
    }
    if (clearance_silk2ol_cb->isChecked()) {
        std::vector<RulesGerber> objs;
        if (use(ss_t_cb, ss_t_object)) if (auto g = gerber(ss_t_object)) objs.push_back(*g);
        if (use(ss_b_cb, ss_b_object)) if (auto g = gerber(ss_b_object)) objs.push_back(*g);
        auto outl = use(out_cb, outline_object) ? gerber(outline_object) : std::nullopt;
        if (objs.empty() || !outl) {
            ui->inform("[ERROR_NOTCL] " + tr_("Silk to Outline Clearance") + ". " +
                       tr_("One of the Silk Gerber objects or the Outline Gerber object is not valid."));
            return nullptr;
        }
        objs.push_back(*outl);
        // Como el original, con la separación de cobre a contorno.
        res.push_back(rulesGerberClearance(objs, clearance_copper2ol_entry->value(), tr_("Silk to Outline Clearance").toStdString()));
    }
    if (ring_integrity_cb->isChecked()) {
        auto t = use(copper_t_cb, copper_t_object) ? gerber(copper_t_object) : std::nullopt;
        auto b = use(copper_b_cb, copper_b_object) ? gerber(copper_b_object) : std::nullopt;
        auto e1 = use(e1_cb, e1_object) ? excellon(e1_object) : std::nullopt;
        auto e2 = use(e2_cb, e2_object) ? excellon(e2_object) : std::nullopt;
        if ((!t && !b) || (!e1 && !e2)) {
            ui->inform("[ERROR_NOTCL] " + tr_("Minimum Annular Ring") + ". " +
                       tr_("One of the Copper Gerber objects or the Excellon objects is not valid."));
            return nullptr;
        }
        res.push_back(rulesAnnularRing(t ? *t : *b, e1 ? *e1 : *e2, ring_integrity_entry->value(), tr_("Minimum Annular Ring").toStdString()));
    }
    std::vector<RulesExcellon> excs;
    if (use(e1_cb, e1_object)) if (auto e = excellon(e1_object)) excs.push_back(*e);
    if (use(e2_cb, e2_object)) if (auto e = excellon(e2_object)) excs.push_back(*e);
    if (clearance_d2d_cb->isChecked()) {
        auto r = rulesHolesClearance(excs, clearance_d2d_entry->value());
        r.title = tr_("Hole to Hole Clearance").toStdString();
        res.push_back(r);
    }
    if (drill_size_cb->isChecked()) {
        auto r = rulesHolesSize(excs, drill_size_entry->value());
        RulesResult first;
        first.hasDias = true;
        if (!r.empty()) first = r.front();
        first.title = tr_("Hole Size").toStdString();
        res.push_back(first);
    }
    const auto txt = rulesReport(res, appDecimals(),
                                 {tr_("STATUS").toStdString(), tr_("FAILED").toStdString(), tr_("PASSED").toStdString(),
                                  tr_("Violations: There are no violations for the current rule.").toStdString()});
    if (!txt) return nullptr;
    return app_->addObject(std::make_unique<DocumentObject>("Rules_check_results", QString::fromStdString(*txt)));
}

}  // namespace flatcam::gui
