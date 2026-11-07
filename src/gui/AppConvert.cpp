// Editar -> Convertir: convert_any2geo, convert_any2gerber y convert_any2excellon de app_Main.py.
#include <QString>
#include <algorithm>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/GeometryObject.h"
#include "parsers/Excellon.h"
#include "parsers/Gerber.h"

namespace flatcam::gui {

namespace {

/// `app.dec_format(v, 4)`.
double dec4(double v) { return std::stod(QString::asprintf("%.4f", v).toStdString()); }

struct ConvTool {
    double dia;
    std::vector<Point2D> drills;
    std::vector<std::pair<Point2D, Point2D>> slots;
};

std::unique_ptr<Excellon> excellonOf(const std::vector<ConvTool>& tools) {
    auto ex = std::make_unique<Excellon>();
    int id = 0;
    for (const ConvTool& t : tools) ex->addDrillTool(++id, t.dia, t.drills, t.slots);
    ex->createGeometry();
    return ex;
}

}  // namespace

std::vector<FlatCAMObj*> App::convertToGeometry() {
    std::vector<FlatCAMObj*> out;
    const std::vector<FlatCAMObj*> sel = selectedObjects();
    if (sel.empty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("No object is selected."));
        return out;
    }
    for (FlatCAMObj* o : sel) {
        auto geo = std::make_unique<GeometryObject>(o->name() + "_conv");
        if (auto* g = dynamic_cast<GeometryObject*>(o)) {
            // initialize: la misma geometría y las mismas herramientas.
            geo->solidGeometry() = g->solidGeometry();
            geo->tools() = g->tools();
        } else if (o->kind() == ObjectKind::Gerber || o->kind() == ObjectKind::Excellon) {
            // Gerber: su geometría sólida; Excellon (initialize_excellon): la de sus herramientas.
            geo->solidGeometry() = o->solidShapes();
            if (geo->solidGeometry().empty()) continue;  // "convert_any2geo() failed"
            GeometryTool t = defaultGeometryTool();  // una Geometría de una sola herramienta
            t.solidGeometry = geo->solidGeometry();
            geo->tools()[1] = t;
        } else {
            continue;
        }
        out.push_back(addObject(std::move(geo)));
    }
    return out;
}

std::vector<FlatCAMObj*> App::convertToGerber() {
    std::vector<FlatCAMObj*> out;
    const std::vector<FlatCAMObj*> sel = selectedObjects();
    if (sel.empty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("No object is selected."));
        return out;
    }
    for (FlatCAMObj* o : sel) {
        std::map<std::string, Aperture> apertures;
        ShapeList solid;
        if (auto* g = dynamic_cast<GeometryObject*>(o)) {
            // initialize_geometry: todo en la apertura "0" (C de tamaño 0), con el contorno como línea de seguimiento.
            Aperture ap;
            ap.type = "C";
            ap.size = 0.0;
            ap.hasGeometry = true;
            for (const Shape& s : g->solidGeometry()) {
                ApertureGeometry el;
                el.solid = s;
                if (s.type() == ShapeType::Polygon) el.follow = s.exterior();
                ap.geometry.push_back(el);
            }
            apertures["0"] = ap;
            solid = g->solidGeometry();
        } else if (auto* e = dynamic_cast<ExcellonObject*>(o)) {
            // initialize_excellon: una apertura C por herramienta desde la 10, con su diámetro; la geometría unida.
            int apid = 10;
            ShapeList all;
            for (const auto& [tid, t] : e->excellon().drillTools()) {
                Aperture ap;
                ap.type = "C";
                ap.size = t.tooldia;
                ap.hasGeometry = true;
                for (const Shape& s : t.solidGeometry) {
                    ApertureGeometry el;
                    el.solid = s;
                    el.follow = s.exterior();
                    ap.geometry.push_back(el);
                    all.push_back(s);
                }
                apertures[std::to_string(apid++)] = ap;
            }
            const Shape u = Shape::collection(all).buffer(0.0000001);  // MultiPolygon(...).buffer(1e-7)
            solid = u.parts().empty() ? ShapeList{u} : u.parts();
        } else {
            continue;  // "This is no valid object for conversion."
        }
        auto gb = std::make_unique<Gerber>();
        gb->restore(solid, {}, std::move(apertures), 3, 4, std::string());
        out.push_back(addObject(std::make_unique<GerberObject>(o->name() + "_conv", std::move(gb))));
    }
    return out;
}

std::vector<FlatCAMObj*> App::convertToExcellon() {
    std::vector<FlatCAMObj*> out;
    const std::vector<FlatCAMObj*> sel = selectedObjects();
    if (sel.empty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("No object is selected."));
        return out;
    }
    for (FlatCAMObj* o : sel) {
        std::vector<ConvTool> tools;
        auto addDrill = [&tools](const Shape& geo, bool exactMatch) {
            const auto b = geo.bounds();
            if (!b) return;
            const double dia = std::min(b->xmax - b->xmin, b->ymax - b->ymin);
            const Point2D c = geo.centroid();
            bool found = false;
            for (const ConvTool& t : tools) found = found || (exactMatch ? t.dia == dia : dec4(t.dia) == dec4(dia));
            if (found) {
                for (ConvTool& t : tools)
                    if (dec4(t.dia) == dec4(dia) &&
                        (exactMatch || std::find(t.drills.begin(), t.drills.end(), c) == t.drills.end()))
                        t.drills.push_back(c);
            } else {
                tools.push_back({dia, {c}, {}});
            }
        };
        if (auto* g = dynamic_cast<GeometryObject*>(o)) {
            // initialize_geometry: un agujero por polígono, en su centroide, del menor lado de su caja.
            for (const Shape& s : g->solidGeometry())
                if (s.type() == ShapeType::Polygon || s.type() == ShapeType::MultiPolygon ||
                    s.type() == ShapeType::LinearRing)
                    addDrill(s, true);
        } else if (auto* gr = dynamic_cast<GerberObject*>(o)) {
            // initialize_gerber: los destellos (follow = punto) son agujeros y los trazos de dos puntos, ranuras.
            const Gerber& gb = gr->gerber();
            for (const std::string& code : gb.apertureOrder()) {
                const Aperture& ap = gb.apertures().at(code);
                for (const ApertureGeometry& el : ap.geometry) {
                    if (!el.follow || !el.solid) continue;
                    if (el.follow->type() == ShapeType::Point) {
                        addDrill(*el.solid, false);
                    } else if (el.follow->type() == ShapeType::LineString) {
                        const auto c = el.follow->coordinates();
                        if (c.size() != 2 || !ap.size) continue;
                        const double dia = *ap.size;
                        const std::pair<Point2D, Point2D> slot{c[0], c[1]};
                        bool found = false;
                        for (const ConvTool& t : tools) found = found || dec4(t.dia) == dec4(dia);
                        if (found) {
                            for (ConvTool& t : tools)
                                if (dec4(t.dia) == dec4(dia) &&
                                    std::find(t.slots.begin(), t.slots.end(), slot) == t.slots.end())
                                    t.slots.push_back(slot);
                        } else {
                            tools.push_back({dia, {}, {slot}});
                        }
                    }
                }
            }
        } else {
            continue;
        }
        if (tools.empty()) continue;  // sin geometría: 'fail'
        out.push_back(addObject(std::make_unique<ExcellonObject>(o->name() + "_conv", excellonOf(tools))));
    }
    return out;
}

}  // namespace flatcam::gui
