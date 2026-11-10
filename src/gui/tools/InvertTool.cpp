#include "gui/tools/InvertTool.h"

#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/ObjectCollection.h"
#include "tools/InvertGerber.h"

namespace flatcam::gui {

InvertTool::InvertTool(App* app, QWidget* parent) : InvertToolUIBase(parent), app_(app) {
    setupUi();
    connect(invert_btn, &QPushButton::clicked, this, [this] { invert(); });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
}

ToolInfo InvertTool::info() { return {tr_("Invert Gerber Tool"), tr_("Invert Tool")}; }

void InvertTool::setToolUi() {
    margin_entry->setValue(defNum("tools_invert_margin"));
    join_radio->setValue(defVariant("tools_invert_join_style"));
    // El combo muestra los Gerber (setRootModelIndex del grupo Gerber).
    const QString cur = gerber_combo->currentText();
    gerber_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == ObjectKind::Gerber) gerber_combo->addItem(o->name());
    if (!cur.isEmpty()) gerber_combo->setValue(cur);
}

FlatCAMObj* InvertTool::invert() {
    const QString j = join_radio->value().toString();
    const JoinStyle join = j == "r" ? JoinStyle::Round : j == "b" ? JoinStyle::Bevel : JoinStyle::Mitre;
    QString name = gerber_combo->currentText();
    FlatCAMObj* src = app_->collection()->byName(name);
    if (!src || src->kind() != ObjectKind::Gerber) {
        if (name.isEmpty()) name = "None";
        app_->ui()->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Object not found"), name));
        return nullptr;
    }
    const QRectF b = src->bounds();
    const Gerber& g = static_cast<GerberObject*>(src)->gerber();
    const ShapeList polys = invertGerber(g.solidGeometry(), {b.left(), b.top(), b.right(), b.bottom()},
                                         margin_entry->value(), join, defInt("gerber_circle_steps", 64), appDecimals());
    Aperture ap;
    ap.type = "C";
    ap.size = 0.0;
    ap.hasGeometry = true;
    for (const Shape& p : polys) {
        ApertureGeometry el;
        el.solid = p;
        el.follow = p.exterior();
        ap.geometry.push_back(el);
    }
    auto out = std::make_unique<Gerber>();
    out->restore(polys, {}, {{"0", ap}}, g.intDigits(), g.fracDigits(), std::string());
    auto obj = std::make_unique<GerberObject>(name + "_inverted", std::move(out));
    // El mismo color de relleno y de contorno que el Gerber original.
    obj->fillColor = src->fillColor;
    obj->outlineColor = src->outlineColor;
    return app_->addObject(std::move(obj));
}

}  // namespace flatcam::gui
