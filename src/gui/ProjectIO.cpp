#include "gui/ProjectIO.h"

#include <QString>

#include "gui/objects/CNCJobObject.h"
#include "gui/objects/GeometryObject.h"

namespace flatcam::gui {

namespace {

/// Figuras de un valor del proyecto: `{"__class__": "Shply", "__inst__": WKT}`, una lista de ellas o nulo.
void collectShapes(const JsonValue& v, ShapeList& out) {
    if (v.isArray()) {
        for (const JsonValue& it : v.items()) collectShapes(it, out);
    } else if (v.isObject() && v["__class__"].text() == "Shply") {
        Shape s = Shape::fromWkt(v["__inst__"].text());
        if (!s.isEmpty()) out.push_back(std::move(s));
    }
}

ShapeList shapes(const JsonValue& v) {
    ShapeList out;
    collectShapes(v, out);
    return out;
}

std::optional<Shape> oneShape(const JsonValue& v) {
    const ShapeList s = shapes(v);
    if (s.empty()) return std::nullopt;
    return s.front();
}

Point2D pointOf(const JsonValue& v) {
    const ShapeList s = shapes(v);
    if (s.empty() || s.front().coordinates().empty()) return {0, 0};
    return s.front().coordinates().front();
}

/// Un par X,Y guardado como lista `[0, 0]`, texto `"0.0, 0.0"` o nulo.
std::optional<std::array<double, 2>> pairOf(const JsonValue& v) {
    if (v.isArray() && v.items().size() == 2) return std::array<double, 2>{v.items()[0].toDouble(), v.items()[1].toDouble()};
    if (v.isString()) {
        const QStringList parts = QString::fromStdString(v.text()).remove('(').remove(')').remove('[').remove(']').split(',');
        bool ok1 = false, ok2 = false;
        if (parts.size() == 2) {
            const double x = parts[0].trimmed().toDouble(&ok1), y = parts[1].trimmed().toDouble(&ok2);
            if (ok1 && ok2) return std::array<double, 2>{x, y};
        }
    }
    return std::nullopt;
}

QString str(const JsonValue& v, const QString& def = QString()) {
    return v.isString() ? QString::fromStdString(v.text()) : def;
}

/// Los colores de FlatCAM se guardan como "#RRGGBB" más `alpha_level` aparte, o ya con la transparencia.
void applyColors(FlatCAMObj& o, const JsonValue& d) {
    if (d["fill_color"].isString()) o.fillColor = colorFromFlatCam(str(d["fill_color"]));
    if (d["outline_color"].isString()) o.outlineColor = colorFromFlatCam(str(d["outline_color"]));
}

/// Lo que se guarda del diccionario para volver a escribirlo: todo menos la geometría y el G-code.
JsonValue keepForSave(const JsonValue& d) {
    JsonValue out = JsonValue::object();
    for (const auto& [k, v] : d.members()) {
        if (k == "solid_geometry" || k == "follow_geometry" || k == "apertures" || k == "tools" || k == "gcode" ||
            k == "gcode_parsed" || k == "cnc_tools" || k == "exc_cnc_tools")
            continue;
        out.members().emplace_back(k, v);
    }
    return out;
}

std::unique_ptr<FlatCAMObj> gerberFrom(const JsonValue& d, const QString& name) {
    std::map<std::string, Aperture> apertures;
    for (const auto& [code, a] : d["apertures"].members()) {
        Aperture ap;
        ap.type = a["type"].text();
        if (a["size"].isNumber()) ap.size = a["size"].toDouble();
        ap.width = a["width"].toDouble();
        ap.height = a["height"].toDouble();
        ap.diam = a["diam"].toDouble();
        ap.nVertices = static_cast<int>(a["nVertices"].toDouble());
        if (a["rotation"].isNumber()) ap.rotation = a["rotation"].toDouble();
        for (const JsonValue& m : a["modifiers"].items()) ap.modifiers.push_back(m.isString() ? m.text() : m.text());
        for (const JsonValue& g : a["geometry"].items()) {
            ApertureGeometry ag;
            ag.solid = oneShape(g["solid"]);
            ag.follow = oneShape(g["follow"]);
            ag.clear = oneShape(g["clear"]);
            ap.geometry.push_back(std::move(ag));
        }
        ap.hasGeometry = a.find("geometry") != nullptr;
        apertures[code] = std::move(ap);
    }
    auto g = std::make_unique<Gerber>();
    g->restore(shapes(d["solid_geometry"]), shapes(d["follow_geometry"]), std::move(apertures),
               static_cast<int>(d["int_digits"].toDouble(3)), static_cast<int>(d["frac_digits"].toDouble(4)),
               d["source_file"].text());
    return std::make_unique<GerberObject>(name, std::move(g));
}

std::unique_ptr<FlatCAMObj> excellonFrom(const JsonValue& d, const QString& name) {
    auto e = std::make_unique<Excellon>();
    for (const auto& [id, t] : d["tools"].members()) {
        std::vector<Point2D> drills;
        for (const JsonValue& p : t["drills"].items()) drills.push_back(pointOf(p));
        std::vector<std::pair<Point2D, Point2D>> slots;
        for (const JsonValue& s : t["slots"].items())
            if (s.isArray() && s.items().size() == 2) slots.push_back({pointOf(s.items()[0]), pointOf(s.items()[1])});
        e->addDrillTool(std::stoi(id), t["tooldia"].toDouble(), drills, slots);
    }
    e->createGeometry();
    return std::make_unique<ExcellonObject>(name, std::move(e));
}

GeometryTool geometryToolFrom(const JsonValue& t) {
    GeometryTool g = defaultGeometryTool();
    g.tooldia = t["tooldia"].isString() ? std::stod(t["tooldia"].text()) : t["tooldia"].toDouble(g.tooldia);
    g.offset = str(t["offset"], "Path");
    g.offsetValue = t["offset_value"].toDouble();
    g.type = str(t["type"], "Rough");
    g.toolType = str(t["tool_type"], "C1");
    const JsonValue& data = t["data"];
    GeometryToolSettings& s = g.data;
    s.cutZ = data["cutz"].toDouble(s.cutZ);
    s.multidepth = data["multidepth"].toBool(s.multidepth);
    s.depthPerPass = data["depthperpass"].toDouble(s.depthPerPass);
    s.travelZ = data["travelz"].toDouble(s.travelZ);
    s.feedrate = data["feedrate"].toDouble(s.feedrate);
    s.feedrateZ = data["feedrate_z"].toDouble(s.feedrateZ);
    s.feedrateRapid = data["feedrate_rapid"].toDouble(s.feedrateRapid);
    s.spindleSpeed = data["spindlespeed"].toDouble(s.spindleSpeed);
    if (data["spindledir"].isString()) s.spindleDir = data["spindledir"].text();
    s.dwell = data["dwell"].toBool(s.dwell);
    s.dwellTime = data["dwelltime"].toDouble(s.dwellTime);
    if (data["startz"].isNumber()) s.startZ = data["startz"].toDouble();
    s.endZ = data["endz"].toDouble(s.endZ);
    s.endXY = pairOf(data["endxy"]);
    s.toolchangeZ = data["toolchangez"].toDouble(s.toolchangeZ);
    if (data.find("toolchangexy")) s.toolchangeXY = pairOf(data["toolchangexy"]);
    s.extracut = data["extracut"].toBool(s.extracut);
    s.extracutLength = data["extracut_length"].toDouble(s.extracutLength);
    g.vtipDia = data["vtipdia"].toDouble(g.vtipDia);
    g.vtipAngle = data["vtipangle"].toDouble(g.vtipAngle);
    g.ppname = str(data["ppname_g"], "default");
    g.toolchange = data["toolchange"].toBool(false);
    g.optimization = str(data["optimization_type"], "R");
    if (data["override_color"].isString()) g.overrideColor = colorFromFlatCam(str(data["override_color"]));
    g.solidGeometry = shapes(t["solid_geometry"]);
    return g;
}

std::unique_ptr<FlatCAMObj> geometryFrom(const JsonValue& d, const QString& name) {
    auto obj = std::make_unique<GeometryObject>(name);
    for (const auto& [uid, t] : d["tools"].members()) obj->tools()[std::stoi(uid)] = geometryToolFrom(t);
    obj->solidGeometry() = shapes(d["solid_geometry"]);
    if (obj->solidGeometry().empty()) {
        // Multiherramienta: la copia general se descarta al leer; se arma con la de cada herramienta.
        for (const auto& [uid, t] : obj->tools())
            obj->solidGeometry().insert(obj->solidGeometry().end(), t.solidGeometry.begin(), t.solidGeometry.end());
    }
    if (obj->tools().empty() && !obj->solidGeometry().empty()) {
        // Geometría de una sola herramienta ("singlegeo"): se arma la herramienta con su diámetro.
        GeometryTool t = defaultGeometryTool();
        t.tooldia = std::stod(d["options"]["cnctooldia"].isString() ? d["options"]["cnctooldia"].text() : "2.4");
        t.solidGeometry = obj->solidGeometry();
        obj->tools()[1] = t;
    }
    return obj;
}

std::unique_ptr<FlatCAMObj> cncFrom(const JsonValue& d, const QString& name) {
    std::vector<std::string> gcodes;
    std::vector<CncToolRow> rows;
    const bool excellon = d["origin_kind"].text() == "excellon" && !d["exc_cnc_tools"].members().empty();
    if (excellon) {
        for (const auto& [dia, t] : d["exc_cnc_tools"].members()) {
            if (t["gcode"].isString()) gcodes.push_back(t["gcode"].text());
            CncToolRow r;
            r.tool = static_cast<int>(t["tool"].toDouble());
            r.dia = std::stod(dia);
            r.drills = static_cast<int>(t["nr_drills"].toDouble());
            r.slots = static_cast<int>(t["nr_slots"].toDouble());
            r.cutZ = t["data"]["tools_drill_cutz"].toDouble() - t["offset"].toDouble();
            rows.push_back(r);
        }
    } else {
        for (const auto& [uid, t] : d["cnc_tools"].members()) {
            if (t["gcode"].isString()) gcodes.push_back(t["gcode"].text());
            CncToolRow r;
            r.tool = std::stoi(uid);
            r.dia = t["tooldia"].toDouble();
            r.cutZ = t["data"]["cutz"].toDouble();
            r.offset = str(t["offset"], "Path");
            r.type = str(t["type"], "Rough");
            r.toolType = str(t["tool_type"], "C1");
            rows.push_back(r);
        }
    }
    // Sin herramientas (multitool falso): el G-code general.
    if (!d["multitool"].toBool(false) || gcodes.empty()) gcodes = {d["gcode"].text()};
    auto job = std::make_unique<CncJob>();
    job->restore("", gcodes);  // gc_start no se guarda en el proyecto (tampoco lo recupera el original)
    auto obj = std::make_unique<CNCJobObject>(name, std::move(job), GcodeHeaderInfo{}, std::move(rows));
    obj->setStoredHeader(d["gc_header"].text());
    return obj;
}

}  // namespace

JsonSkipFn ProjectIO::skipFn() {
    return [](const std::vector<std::string>& path, const JsonValue& parent) {
        const std::string& key = path.back();
        if (key == "gcode_parsed") return true;
        // Dentro de un objeto de la lista: la geometría de los CNC Job (solo sirve para los límites) y la copia
        // general de una Geometría multiherramienta.
        if (path.size() == 3 && path[0] == "objs" && key == "solid_geometry") {
            const std::string kind = parent["kind"].text();
            if (kind == "cncjob") return true;
            if (kind == "geometry" && parent["multigeo"].toBool(false)) return true;
        }
        // La geometría de cada herramienta de un CNC Job.
        if (path.size() == 5 && path[0] == "objs" && (path[2] == "cnc_tools" || path[2] == "exc_cnc_tools") &&
            key == "solid_geometry")
            return true;
        return false;
    };
}

std::unique_ptr<FlatCAMObj> ProjectIO::objectFrom(const JsonValue& d, QString& warning) {
    const std::string kind = d["kind"].text();
    const QString name = str(d["options"]["name"], "objeto");
    std::unique_ptr<FlatCAMObj> obj;
    try {
        if (kind == "gerber") obj = gerberFrom(d, name);
        else if (kind == "excellon") obj = excellonFrom(d, name);
        else if (kind == "geometry") obj = geometryFrom(d, name);
        else if (kind == "cncjob") obj = cncFrom(d, name);
    } catch (const std::exception& e) {
        warning = QString("%1: %2").arg(name, QString::fromUtf8(e.what()));
        return nullptr;
    }
    if (!obj) {
        warning = QString("%1: el tipo \"%2\" todavía no se abre").arg(name, QString::fromStdString(kind));
        return nullptr;
    }
    applyColors(*obj, d);
    obj->setPlotEnabled(d["options"]["plot"].toBool(true));
    obj->projectData = keepForSave(d);
    return obj;
}

}  // namespace flatcam::gui
