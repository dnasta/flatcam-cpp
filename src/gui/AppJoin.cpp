// Editar -> Unir: on_edit_join (GeometryObject.merge), on_edit_join_exc (ExcellonObject.merge) y on_edit_join_grb
// (GerberObject.merge) de app_Main.py.
#include <QString>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/GeometryObject.h"
#include "parsers/Excellon.h"
#include "parsers/Gerber.h"

namespace flatcam::gui {

namespace {

QString tooFew(std::size_t n) {
    return QString("[ERROR_NOTCL] %1: %2").arg(tr_("At least two objects are required for join. Objects currently selected")).arg(n);
}

}  // namespace

FlatCAMObj* App::joinGeometry() {
    const std::vector<FlatCAMObj*> objs = selectedObjects();
    if (objs.size() < 2) {
        ui_->inform(tooFew(objs.size()));
        return nullptr;
    }
    // multigeo: las Geometrías del port son todas multiherramienta; Gerber y Excellon, de una sola.
    std::set<bool> types;
    for (FlatCAMObj* o : objs) types.insert(o->kind() == ObjectKind::Geometry);
    if (types.size() != 1) {
        ui_->inform("[ERROR] " + tr_("Failed join. The Geometry objects are of different types.\n"
                                     "At least one is MultiGeo type and the other is SingleGeo type. A possibility is to "
                                     "convert from one to another and retry joining \n"
                                     "but in the case of converting from MultiGeo to SingleGeo, informations may be lost and "
                                     "the result may not be what was expected. \n"
                                     "Check the generated GCODE."));
        return nullptr;
    }
    const bool multi = *types.begin();
    const QString name = multi ? "Combo_MultiGeo" : "Combo_SingleGeo";
    auto geo = std::make_unique<GeometryObject>(name);
    // GeometryObject.merge: la geometría de todos, y las herramientas renumeradas (Gerber y Excellon no aportan).
    std::map<int, GeometryTool> newTools;
    for (FlatCAMObj* o : objs) {
        const ShapeList s = o->solidShapes();
        geo->solidGeometry().insert(geo->solidGeometry().end(), s.begin(), s.end());
        if (auto* g = dynamic_cast<GeometryObject*>(o)) {
            int maxUid = newTools.empty() ? 0 : newTools.rbegin()->first;
            for (const auto& [uid, t] : g->tools()) newTools[++maxUid] = t;
        }
    }
    // geometry_merge_fuse_tools = True: se juntan las de igual diámetro, tipo y forma, en el orden del original (los
    // diámetros, tipos y formas en el orden en que aparecen; cada grupo ordenado por número) y cada una queda con los
    // datos de la última de su grupo.
    std::map<int, GeometryTool> finalTools;
    if (!newTools.empty()) {
        std::vector<double> dias;
        std::vector<QString> typesL, toolTypes;
        for (const auto& [k, t] : newTools) {
            if (std::find(dias.begin(), dias.end(), t.tooldia) == dias.end()) dias.push_back(t.tooldia);
            if (std::find(typesL.begin(), typesL.end(), t.type) == typesL.end()) typesL.push_back(t.type);
            if (std::find(toolTypes.begin(), toolTypes.end(), t.toolType) == toolTypes.end()) toolTypes.push_back(t.toolType);
        }
        int nr = 1;
        for (double d : dias)
            for (const QString& ty : typesL)
                for (const QString& tt : toolTypes) {
                    std::vector<int> group;
                    for (const auto& [k, t] : newTools)
                        if (t.tooldia == d && t.type == ty && t.toolType == tt) group.push_back(k);
                    if (group.empty()) continue;
                    ShapeList solid;
                    for (int k : group) solid.insert(solid.end(), newTools[k].solidGeometry.begin(), newTools[k].solidGeometry.end());
                    if (solid.empty()) continue;
                    GeometryTool t = newTools[group.back()];
                    t.solidGeometry = solid;
                    finalTools[nr++] = t;
                }
    }
    geo->tools() = finalTools;
    if (geo->tools().empty() && !geo->solidGeometry().empty()) {
        // Sin herramientas (Gerber y Excellon): una con la geometría, como una Geometría nueva.
        GeometryTool t = defaultGeometryTool();
        t.solidGeometry = geo->solidGeometry();
        geo->tools()[1] = t;
    }
    FlatCAMObj* out = addObject(std::move(geo));
    ui_->inform("[success] " + tr_("Geometry merging finished") + ".");
    return out;
}

FlatCAMObj* App::joinExcellon() {
    const std::vector<FlatCAMObj*> objs = selectedObjects();
    for (FlatCAMObj* o : objs)
        if (o->kind() != ObjectKind::Excellon) {
            ui_->inform("[ERROR_NOTCL] " + tr_("Failed. Excellon joining works only on Excellon objects."));
            return nullptr;
        }
    if (objs.size() < 2) {
        ui_->inform(tooFew(objs.size()));
        return nullptr;
    }
    // ExcellonObject.merge: herramientas renumeradas desde 1 en el orden de los objetos y, con
    // excellon_merge_fuse_tools, las de igual diámetro (a 4 decimales) en la primera.
    struct Tool {
        double dia;
        std::vector<Point2D> drills;
        std::vector<std::pair<Point2D, Point2D>> slots;
    };
    std::vector<std::pair<int, Tool>> tools;
    int id = 0;
    for (FlatCAMObj* o : objs)
        for (const auto& [tid, t] : static_cast<ExcellonObject*>(o)->excellon().drillTools())
            tools.push_back({++id, Tool{t.tooldia, t.drills, t.slots}});
    std::vector<std::pair<int, Tool>> fused;
    for (auto& [tid, t] : tools) {
        const double dia = std::stod(QString::asprintf("%.4f", t.dia).toStdString());
        auto it = std::find_if(fused.begin(), fused.end(), [dia](const auto& f) { return f.second.dia == dia; });
        if (it != fused.end()) {
            it->second.drills.insert(it->second.drills.end(), t.drills.begin(), t.drills.end());
            it->second.slots.insert(it->second.slots.end(), t.slots.begin(), t.slots.end());
        } else {
            t.dia = dia;
            fused.push_back({tid, t});
        }
    }
    auto ex = std::make_unique<Excellon>();
    for (const auto& [tid, t] : fused) ex->addDrillTool(tid, t.dia, t.drills, t.slots);
    ex->createGeometry();
    FlatCAMObj* out = addObject(std::make_unique<ExcellonObject>("Combo_Excellon", std::move(ex)));
    ui_->inform("[success] " + tr_("Excellon merging finished") + ".");
    return out;
}

FlatCAMObj* App::joinGerber() {
    const std::vector<FlatCAMObj*> objs = selectedObjects();
    for (FlatCAMObj* o : objs)
        if (o->kind() != ObjectKind::Gerber) {
            ui_->inform("[ERROR_NOTCL] " + tr_("Failed. Gerber joining works only on Gerber objects."));
            return nullptr;
        }
    if (objs.size() < 2) {
        ui_->inform(tooFew(objs.size()));
        return nullptr;
    }
    // GerberObject.merge: la geometría de todos (también como follow_geometry, como el original) y las aperturas; una
    // apertura repetida se agrega con el número siguiente al mayor.
    ShapeList solid, follow;
    std::map<std::string, Aperture> apertures;
    int intDigits = 3, fracDigits = 4;
    for (FlatCAMObj* o : objs) {
        Gerber& g = static_cast<GerberObject*>(o)->gerber();
        intDigits = g.intDigits();
        fracDigits = g.fracDigits();
        solid.insert(solid.end(), g.solidGeometry().begin(), g.solidGeometry().end());
        follow.insert(follow.end(), g.solidGeometry().begin(), g.solidGeometry().end());
        for (const std::string& code : g.apertureOrder()) {
            const Aperture& ap = g.apertures().at(code);
            if (!apertures.count(code)) {
                apertures[code] = ap;
            } else {
                int maxAp = 0;
                for (const auto& [k, v] : apertures) maxAp = std::max(maxAp, std::atoi(k.c_str()));
                apertures[std::to_string(maxAp + 1)] = ap;
            }
        }
    }
    auto g = std::make_unique<Gerber>();
    g->restore(std::move(solid), std::move(follow), std::move(apertures), intDigits, fracDigits, std::string());
    FlatCAMObj* out = addObject(std::make_unique<GerberObject>("Combo_Gerber", std::move(g)));
    ui_->inform("[success] " + tr_("Gerber merging finished") + ".");
    return out;
}

}  // namespace flatcam::gui
