#include "gui/tools/SubTool.h"

#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ObjectCollection.h"
#include "tools/Subtract.h"

namespace flatcam::gui {

SubTool::SubTool(App* app, QWidget* parent) : SubToolUIBase(parent), app_(app) {
    setupUi();
    connect(intersect_btn, &QPushButton::clicked, this, [this] { subtractGerber(); });
    connect(intersect_geo_btn, &QPushButton::clicked, this, [this] { subtractGeometry(); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
}

ToolInfo SubTool::info() { return {tr_("Subtract Tool"), tr_("Sub Tool")}; }

void SubTool::setToolUi() {
    auto fill = [this](FCComboBox* combo, ObjectKind k) {
        const QString cur = combo->currentText();
        combo->clear();
        for (FlatCAMObj* o : app_->collection()->objects())
            if (o->kind() == k) combo->addItem(o->name());
        if (!cur.isEmpty()) combo->setValue(cur);
    };
    fill(target_gerber_combo, ObjectKind::Gerber);
    fill(sub_gerber_combo, ObjectKind::Gerber);
    fill(target_geo_combo, ObjectKind::Geometry);
    fill(sub_geo_combo, ObjectKind::Geometry);
    tools_frame->show();
    close_paths_cb->setChecked(defBool("tools_sub_close_paths"));
    delete_sources_cb->setChecked(defBool("tools_sub_delete_sources"));
}

FlatCAMObj* SubTool::subtractGerber() {
    const QString targetName = target_gerber_combo->currentText();
    if (targetName.isEmpty()) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("No Target object loaded."));
        return nullptr;
    }
    app_->ui()->inform(tr_("Loading geometry from Gerber objects."));
    FlatCAMObj* target = app_->collection()->byName(targetName);
    const QString subName = sub_gerber_combo->currentText();
    if (subName.isEmpty()) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("No Subtractor object loaded."));
        return nullptr;
    }
    FlatCAMObj* sub = app_->collection()->byName(subName);
    if (!target || !sub || target->kind() != ObjectKind::Gerber || sub->kind() != ObjectKind::Gerber) return nullptr;
    const Gerber& tg = static_cast<GerberObject*>(target)->gerber();
    SubtractGerberResult r = ::flatcam::subtractGerber(tg, static_cast<GerberObject*>(sub)->gerber());
    app_->ui()->inform(tr_("Subtraction aperture processing finished."));
    if (r.failed) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("Generating new object failed."));
        return nullptr;
    }
    const QString outname = targetName + "_sub";
    auto g = std::make_unique<Gerber>();
    g->restore(std::move(r.solid), {}, std::move(r.apertures), tg.intDigits(), tg.fracDigits(), std::string());
    FlatCAMObj* out = app_->addObject(std::make_unique<GerberObject>(outname, std::move(g)));
    app_->ui()->inform(QString("[success] %1: %2").arg(tr_("Created"), outname));
    if (delete_sources_cb->isChecked()) {
        app_->deleteByName(targetName);
        app_->deleteByName(subName);
    }
    return out;
}

FlatCAMObj* SubTool::subtractGeometry() {
    const QString targetName = target_geo_combo->currentText();
    if (targetName.isEmpty()) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("No Target object loaded."));
        return nullptr;
    }
    const QString subName = sub_geo_combo->currentText();
    if (subName.isEmpty()) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("No Subtractor object loaded."));
        return nullptr;
    }
    auto* sub = dynamic_cast<GeometryObject*>(app_->collection()->byName(subName));
    if (sub && sub->tools().size() > 1) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("Currently, the Subtractor geometry cannot be of type Multigeo."));
        return nullptr;
    }
    // El original falla acá (ValueError al recorrer las herramientas del objetivo) y no crea nada.
    return nullptr;
}

}  // namespace flatcam::gui
