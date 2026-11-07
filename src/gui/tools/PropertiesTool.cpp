#include "gui/tools/PropertiesTool.h"

#include <QSplitter>
#include <algorithm>
#include <cmath>
#include <limits>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/ProjectIO.h"
#include "gui/Translation.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/GeometryObject.h"
#include "io/JsonValue.h"

namespace flatcam::gui {

namespace {

constexpr int kDecimals = 4;

QString fmt(double v) { return QString::asprintf("%.*f", kDecimals, v); }

/// `repr` de un texto de Python: comillas simples, salvo que tenga simples y no dobles.
QString pyRepr(const std::string& s) {
    const QString t = QString::fromStdString(s);
    if (t.contains('\'') && !t.contains('"')) return "\"" + t + "\"";
    QString e = t;
    e.replace("\\", "\\\\").replace("'", "\\'").replace("\n", "\\n").replace("\t", "\\t");
    return "'" + e + "'";
}

/// Límites de la unión de una lista de figuras; vacío si no hay ninguna.
std::optional<BoundingBox> boundsOf(const ShapeList& l) {
    std::optional<BoundingBox> b;
    for (const Shape& s : l) {
        const auto ob = s.bounds();
        if (!ob) continue;
        const BoundingBox x = *ob;
        if (!b) b = x;
        else b = BoundingBox{std::min(b->xmin, x.xmin), std::min(b->ymin, x.ymin), std::max(b->xmax, x.xmax),
                             std::max(b->ymax, x.ymax)};
    }
    return b;
}

}  // namespace

QString PropertiesTool::pyStr(const JsonValue& v, bool nested) {
    switch (v.type()) {
        case JsonValue::Type::Null: return "None";
        case JsonValue::Type::Bool: return v.toBool() ? "True" : "False";
        case JsonValue::Type::Number: return QString::fromStdString(v.text());
        case JsonValue::Type::String: return nested ? pyRepr(v.text()) : QString::fromStdString(v.text());
        case JsonValue::Type::Array: {
            QStringList parts;
            for (const JsonValue& x : v.items()) parts << pyStr(x, true);
            return "[" + parts.join(", ") + "]";
        }
        case JsonValue::Type::Object: {
            QStringList parts;
            for (const auto& [k, x] : v.members()) parts << pyRepr(k) + ": " + pyStr(x, true);
            return "{" + parts.join(", ") + "}";
        }
    }
    return {};
}

PropertiesTool::PropertiesTool(App* app, QWidget* parent) : PropertiesToolUIBase(parent), app_(app) { setupUi(); }

ToolInfo PropertiesTool::info() { return {tr_("Properties"), tr_("Properties Tool")}; }

void PropertiesTool::run(bool toggle) {
    MainGUI* ui = app_->ui();
    // run: con toggle, un segundo pedido con el panel a la vista lo esconde (y el original igual lo vuelve a llenar).
    if (toggle && ui->splitter->sizes().value(0) != 0 && ui->tool_scroll_area->widget() == this &&
        ui->notebook->currentWidget() == ui->tool_tab) {
        ui->splitter->setSizes({0, 1});
    } else {
        if (ui->splitter->sizes().value(0) == 0) ui->splitter->setSizes({1, 1});
        app_->runTool(this, info(), false);
    }
    // set_tool_ui
    treeWidget->clear();
    properties_frame->show();
    properties();
}

void PropertiesTool::properties() {
    MainGUI* ui = app_->ui();
    const std::vector<FlatCAMObj*> objs = app_->selectedObjects();
    if (objs.empty()) {
        ui->inform("[ERROR_NOTCL] " + tr_("No object is selected."));
        ui->notebook->setTabText(2, tr_("Tools"));
        properties_frame->hide();
        ui->notebook->setCurrentWidget(ui->project_tab);
        return;
    }
    for (FlatCAMObj* o : objs) {
        addItems(o);
        ui->inform("[success] " + tr_("Object Properties are displayed."));
    }
    treeWidget->onResize();
    ui->notebook->setTabText(2, tr_("Properties Tool"));
}

void PropertiesTool::addItems(FlatCAMObj* obj) {
    const JsonValue& appOptions = app_->appOptions();
    const JsonValue d = ProjectIO::toDict(*obj, appOptions);
    QFont font;
    font.setBold(true);
    const QColor pColor("#000000");  // global_gray_icons = False
    static const char* kKinds[] = {"gerber", "excellon", "geometry", "cncjob", "script", "document"};
    const QString kind = kKinds[static_cast<int>(obj->kind())];

    FCTree* t = treeWidget;
    QTreeWidgetItem* objType = t->addParent(nullptr, tr_("TYPE"), true, pColor, font);
    QTreeWidgetItem* objName = t->addParent(nullptr, tr_("NAME"), true, pColor, font);
    QTreeWidgetItem* dims = t->addParent(nullptr, tr_("Dimensions"), true, pColor, font);
    QTreeWidgetItem* units = t->addParent(nullptr, tr_("Units"), true, pColor, font);
    QTreeWidgetItem* options = t->addParent(nullptr, tr_("Options"), false, pColor, font);
    QTreeWidgetItem *apertures = nullptr, *tools = nullptr, *drills = nullptr, *slots = nullptr, *others = nullptr;
    if (kind == "gerber") apertures = t->addParent(nullptr, tr_("Apertures"), true, pColor, font);
    else tools = t->addParent(nullptr, tr_("Tools"), true, pColor, font);
    if (kind == "excellon") {
        drills = t->addParent(nullptr, tr_("Drills"), true, pColor, font);
        slots = t->addParent(nullptr, tr_("Slots"), true, pColor, font);
    }
    if (kind == "cncjob") others = t->addParent(nullptr, tr_("Others"), true, pColor, font);
    QTreeWidgetItem* separator = t->addParent(nullptr, "");

    t->addChild(objType, {tr_("Object Type") + ":", kind.toUpper()}, true, font, {1});
    // Geo Type: los CNC Job no tienen `multigeo` (el original lo registra y sigue).
    if (kind != "cncjob")
        t->addChild(objType, {tr_("Geo Type") + ":",
                              d["multigeo"].toBool() ? tr_("Multi-Geo") : tr_("Single-Geo")},
                    true);
    t->addChild(objName, {obj->name()});

    // ---- job_thread: dimensiones y áreas
    double length = 0, width = 0, area = 0, copper = 0, chull = 0;
    const ShapeList geo = obj->solidShapes();
    auto setBox = [&](double x0, double y0, double x1, double y1) {
        length = std::abs(x1 - x0);
        width = std::abs(y1 - y0);
        area = length * width / 100;  // mm -> cm2
    };
    if (!geo.empty()) {
        const QRectF b = obj->bounds();
        setBox(b.left(), b.top(), b.right(), b.bottom());
        if (kind == "gerber") {
            for (const Shape& s : geo) copper += s.area();
            copper /= 100;
        }
        if (kind != "cncjob") chull = Shape::unaryUnion(geo).convexHull().area();
    } else {
        std::optional<BoundingBox> all;
        std::vector<double> hulls;
        auto grow = [&](const ShapeList& l) {
            const auto b = boundsOf(l);
            if (!b) return;
            if (!all) all = b;
            else all = BoundingBox{std::min(all->xmin, b->xmin), std::min(all->ymin, b->ymin),
                                   std::max(all->xmax, b->xmax), std::max(all->ymax, b->ymax)};
        };
        if (kind == "cncjob") {
            // Un CNC Job del port no guarda solid_geometry: los límites del objeto (los del G-code).
            const QRectF b = obj->bounds();
            if (!b.isNull()) setBox(b.left(), b.top(), b.right(), b.bottom());
        } else if (auto* g = dynamic_cast<GeometryObject*>(obj)) {
            for (const auto& [uid, tool] : g->tools()) {
                grow(tool.solidGeometry);
                hulls.push_back(Shape::unaryUnion(tool.solidGeometry).convexHull().area());
            }
            if (all) setBox(all->xmin, all->ymin, all->xmax, all->ymax);
            if (!hulls.empty()) chull = *std::max_element(hulls.begin(), hulls.end());
        }
    }
    chull /= 100;

    t->addChild(units, {"FlatCAM units:", tr_("Metric")}, true);
    t->addChild(units, {"Object units:", tr_("Metric")}, true);

    for (const auto& [k, v] : d["options"].members()) {
        if (k == "name") continue;
        t->addChild(options, {QString::fromStdString(k), pyStr(v)}, true);
    }

    if (kind == "gerber") {
        for (const auto& [ap, value] : d["apertures"].members()) {
            QStringList keys, vals;
            int solid = 0, follow = 0, clear = 0;
            if (const JsonValue* g = value.find("geometry")) {
                if (!g->items().empty()) {
                    font.setBold(true);
                    for (const JsonValue& el : g->items()) {
                        if (el.find("solid")) ++solid;
                        if (el.find("follow")) ++follow;
                        if (el.find("clear")) ++clear;
                    }
                }
            } else {
                font.setBold(false);
            }
            QTreeWidgetItem* apid = t->addParent(apertures, QString::fromStdString(ap), false, pColor, font);
            for (const auto& [k, v] : value.members()) {
                if (k == "geometry") continue;
                t->addChild(apid, {QString::fromStdString(k), pyStr(v)}, true);
            }
            t->addChild(apid, {"Solid_Geo", QString("%1 Polygons").arg(solid)}, true);
            t->addChild(apid, {"Follow_Geo", QString("%1 LineStrings").arg(follow)}, true);
            t->addChild(apid, {"Clear_Geo", QString("%1 Polygons").arg(clear)}, true);
        }
    } else if (kind == "excellon") {
        int totDrills = 0, totSlots = 0;
        for (const auto& [tool, value] : d["tools"].members()) {
            QTreeWidgetItem* toolid = t->addParent(tools, QString::fromStdString(tool), false, pColor, font);
            const int nd = static_cast<int>(value["drills"].items().size());
            const int ns = static_cast<int>(value["slots"].items().size());
            totDrills += nd;
            totSlots += ns;
            t->addChild(toolid, {tr_("Diameter"), fmt(value["tooldia"].toDouble()) + " mm"}, true);
            t->addChild(toolid, {tr_("Drills number"), QString::number(nd)}, true);
            t->addChild(toolid, {tr_("Slots number"), QString::number(ns)}, true);
        }
        t->addChild(drills, {tr_("Drills total number:"), QString::number(totDrills)}, true);
        t->addChild(slots, {tr_("Slots total number:"), QString::number(totSlots)}, true);
    } else if (kind == "geometry") {
        for (const auto& [tool, value] : d["tools"].members()) {
            QTreeWidgetItem* geoTool = t->addParent(tools, QString::fromStdString(tool), true, pColor, font);
            for (const auto& [k, v] : value.members()) {
                if (k == "solid_geometry") {
                    t->addChild(geoTool, {QString::fromStdString(k),
                                          v.isArray() ? QString::number(v.items().size()) : QString("1")},
                                true);
                } else if (k == "data") {
                    QString cap = QString::fromStdString(k);
                    cap[0] = cap[0].toUpper();
                    QTreeWidgetItem* data = t->addParent(geoTool, cap, false, pColor, font);
                    for (const auto& [dk, dv] : v.members()) t->addChild(data, {QString::fromStdString(dk), pyStr(dv)}, true);
                } else {
                    t->addChild(geoTool, {QString::fromStdString(k), pyStr(v)}, true);
                }
            }
        }
    } else if (kind == "cncjob") {
        auto present = [](const JsonValue& v) {
            const bool on = v.isArray() ? !v.items().empty() : v.isString() ? !v.text().empty() : !v.isNull();
            return on ? tr_("Present") : tr_("None");
        };
        auto capitalize = [](const std::string& s) {
            QString q = QString::fromStdString(s).toLower();
            if (!q.isEmpty()) q[0] = q[0].toUpper();
            return q;
        };
        for (const auto& [tool, value] : d["cnc_tools"].members()) {
            QTreeWidgetItem* geoTool = t->addParent(tools, QString::fromStdString(tool), true, pColor, font);
            for (const auto& [k, v] : value.members()) {
                if (k == "solid_geometry") t->addChild(geoTool, {tr_("Solid Geometry"), present(v)}, true);
                else if (k == "gcode") t->addChild(geoTool, {tr_("GCode Text"), present(v)}, true);
                else if (k == "gcode_parsed") t->addChild(geoTool, {tr_("GCode Geometry"), present(v)}, true);
                else if (k != "data") t->addChild(geoTool, {QString::fromStdString(k), pyStr(v)}, true);
            }
            QTreeWidgetItem* data = t->addParent(geoTool, tr_("Tool Data"), false, pColor, font);
            for (const auto& [dk, dv] : value["data"].members()) t->addChild(data, {capitalize(dk), pyStr(dv)}, true);
        }
        for (const auto& [dia, value] : d["exc_cnc_tools"].members()) {
            QTreeWidgetItem* excTool = t->addParent(tools, pyStr(value["tool"]), false, pColor, font);
            t->addChild(excTool, {tr_("Diameter"), fmt(std::stod(dia)) + " mm"}, true);
            for (const auto& [k, v] : value.members()) {
                if (k == "solid_geometry") t->addChild(excTool, {tr_("Solid Geometry"), present(v)}, true);
                else if (k == "nr_drills") t->addChild(excTool, {tr_("Drills number"), pyStr(v)}, true);
                else if (k == "nr_slots") t->addChild(excTool, {tr_("Slots number"), pyStr(v)}, true);
                else if (k == "gcode") t->addChild(excTool, {tr_("GCode Text"), present(v)}, true);
                else if (k == "gcode_parsed") t->addChild(excTool, {tr_("GCode Geometry"), present(v)}, true);
            }
            const double offset = std::abs(value["data"]["tools_drill_offset"].toDouble(0));
            t->addChild(excTool, {tr_("Depth of Cut"), fmt(d["z_cut"].toDouble() - offset) + " mm"}, true);
            t->addChild(excTool, {tr_("Clearance Height"), fmt(d["z_move"].toDouble()) + " mm"}, true);
            t->addChild(excTool, {tr_("Feedrate"), fmt(d["feedrate"].toDouble()) + " mm/min"}, true);
            QTreeWidgetItem* data = t->addParent(excTool, tr_("Tool Data"), false, pColor, font);
            for (const auto& [dk, dv] : value["data"].members()) t->addChild(data, {capitalize(dk), pyStr(dv)}, true);
        }
        // routing_time y travel_distance (no se guardan en los proyectos: 0 en un CNC Job abierto de un proyecto).
        const CncJob& job = static_cast<CNCJobObject*>(obj)->job();
        double rTime = job.routingTime();
        QString unitsLbl = "min";
        if (rTime <= 1) {
            rTime *= 60;
            unitsLbl = "sec";
        }
        rTime = std::ceil(rTime);
        t->addChild(others, {tr_("Routing time") + ":", fmt(rTime) + " " + unitsLbl}, true);
        t->addChild(others, {tr_("Travelled distance") + ":", fmt(job.travelDistance()) + " mm"}, true);
    }
    t->addChild(separator, {""});

    // show_area_chull (al terminar job_thread): debajo de Dimensiones.
    t->addChild(dims, {tr_("Length") + ":", fmt(length) + " mm"}, true);
    t->addChild(dims, {tr_("Width") + ":", fmt(width) + " mm"}, true);
    t->addChild(dims, {tr_("Box Area") + ":", fmt(area) + " cm2"}, true);
    t->addChild(dims, {tr_("Convex_Hull Area") + ":", fmt(chull) + " cm2"}, true);
    t->addChild(dims, {tr_("Copper Area") + ":", fmt(copper) + " cm2"}, true);
}

}  // namespace flatcam::gui
