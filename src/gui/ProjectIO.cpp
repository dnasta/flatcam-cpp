#include "gui/ProjectIO.h"

#include <QFile>
#include <QRegularExpression>
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

/// Copia del diccionario sin las figuras (quedan nulas): lo que se guarda para volver a escribir el proyecto.
JsonValue stripShapes(const JsonValue& v) {
    if (v.isObject()) {
        if (v["__class__"].text() == "Shply") return {};
        JsonValue out = JsonValue::object();
        for (const auto& [k, m] : v.members()) out.members().emplace_back(k, stripShapes(m));
        return out;
    }
    if (v.isArray()) {
        JsonValue out = JsonValue::array();
        for (const JsonValue& it : v.items()) out.push(stripShapes(it));
        return out;
    }
    return v;
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
    g.projectData = data;
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
            r.data = t["data"];
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
            r.data = t["data"];
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
    obj->projectData = stripShapes(d);
    return obj;
}

// ======================================================================== guardar

namespace {

JsonValue shp(const Shape& s) {
    JsonValue v = JsonValue::object();
    v.set("__class__", JsonValue::string("Shply"));
    v.set("__inst__", JsonValue::string(s.toWkt()));
    return v;
}

JsonValue shpList(const ShapeList& l) {
    JsonValue a = JsonValue::array();
    for (const Shape& s : l) a.push(shp(s));
    return a;
}

JsonValue real(double v) { return JsonValue::real(v); }

/// options de un objeto nuevo: las `<kind>_*` de la aplicación sin el prefijo y las de herramientas (`new_object`).
JsonValue objectOptions(const FlatCAMObj& obj, const char* kind, const JsonValue& app,
                        std::initializer_list<const char*> extraPrefixes) {
    JsonValue o = obj.projectData["options"].isObject() ? obj.projectData["options"] : JsonValue::object();
    if (!obj.projectData["options"].isObject()) {
        const std::string pre = std::string(kind) + "_";
        for (const auto& [k, v] : app.members()) {
            if (k.rfind(pre, 0) == 0) o.set(k.substr(pre.size()), v);
            for (const char* e : extraPrefixes)
                if (k.rfind(e, 0) == 0) o.set(k, v);
        }
    }
    o.set("name", JsonValue::string(obj.name().toStdString()));
    o.set("plot", JsonValue::boolean(obj.plotEnabled()));
    const QRectF b = obj.bounds();
    o.set("xmin", real(b.left()));
    o.set("ymin", real(b.top()));
    o.set("xmax", real(b.right()));
    o.set("ymax", real(b.bottom()));
    return o;
}

/// Partida del diccionario: lo que vino del proyecto, o vacío.
JsonValue base(const FlatCAMObj& obj) { return obj.projectData.isObject() ? obj.projectData : JsonValue::object(); }

void setIfMissing(JsonValue& d, const std::string& key, JsonValue v) {
    if (!d.find(key)) d.set(key, std::move(v));
}

JsonValue gerberDict(const GerberObject& obj, const JsonValue& app) {
    const Gerber& g = const_cast<GerberObject&>(obj).gerber();
    JsonValue d = base(obj);
    setIfMissing(d, "alpha_level", JsonValue::string("bf"));
    setIfMissing(d, "fill_color", app["gerber_plot_fill"]);
    setIfMissing(d, "outline_color", app["gerber_plot_line"]);
    setIfMissing(d, "units", JsonValue::string("MM"));
    setIfMissing(d, "tools", JsonValue());
    if (!d.find("aperture_macros")) {
        JsonValue m = JsonValue::object();
        for (const auto& [name, macro] : g.apertureMacros()) {
            JsonValue inst = JsonValue::object();
            inst.set("name", JsonValue::string(macro->name()));
            inst.set("raw", JsonValue::string(macro->raw()));
            JsonValue am = JsonValue::object();
            am.set("__class__", JsonValue::string("ApertureMacro"));
            am.set("__inst__", inst);
            m.set(name, am);
        }
        d.set("aperture_macros", m);
    }
    JsonValue aps = JsonValue::object();
    const JsonValue& oldAps = d["apertures"];
    for (const auto& [code, a] : g.apertures()) {
        JsonValue ap = oldAps[code].isObject() ? oldAps[code] : JsonValue::object();
        ap.set("type", JsonValue::string(a.type));
        if (a.size) ap.set("size", real(*a.size));
        if (a.type == "R" || a.type == "O") {
            ap.set("width", real(a.width));
            ap.set("height", real(a.height));
        }
        if (a.type == "P") {
            ap.set("diam", real(a.diam));
            ap.set("nVertices", JsonValue::integer(a.nVertices));
            if (a.rotation) ap.set("rotation", real(*a.rotation));
        }
        if (a.type == "AM") {
            if (a.macro) ap.set("macro", JsonValue::string(a.macro->name()));
            JsonValue mods = JsonValue::array();
            for (const std::string& m : a.modifiers) mods.push(JsonValue::string(m));
            ap.set("modifiers", mods);
        }
        if (a.hasGeometry) {
            JsonValue geo = JsonValue::array();
            for (const ApertureGeometry& ag : a.geometry) {
                JsonValue e = JsonValue::object();
                if (ag.solid) e.set("solid", shp(*ag.solid));
                if (ag.follow) e.set("follow", shp(*ag.follow));
                if (ag.clear) e.set("clear", shp(*ag.clear));
                geo.push(e);
            }
            ap.set("geometry", geo);
        }
        aps.set(code, ap);
    }
    d.set("apertures", aps);
    d.set("int_digits", JsonValue::integer(g.intDigits()));
    d.set("frac_digits", JsonValue::integer(g.fracDigits()));
    d.set("source_file", JsonValue::string(g.sourceFile()));
    d.set("solid_geometry", shpList(g.solidGeometry()));
    d.set("follow_geometry", shpList(g.followGeometry()));
    d.set("kind", JsonValue::string("gerber"));
    d.set("options", objectOptions(obj, "gerber", app, {"tools_iso_"}));
    return d;
}

JsonValue excellonDict(const ExcellonObject& obj, const JsonValue& app) {
    const Excellon& e = const_cast<ExcellonObject&>(obj).excellon();
    JsonValue d = base(obj);
    setIfMissing(d, "alpha_level", JsonValue::string("bf"));
    setIfMissing(d, "fill_color", app["excellon_plot_fill"]);
    setIfMissing(d, "outline_color", app["excellon_plot_line"]);
    setIfMissing(d, "units", JsonValue::string("MM"));
    setIfMissing(d, "follow_geometry", JsonValue());
    setIfMissing(d, "zeros", app["excellon_zeros"]);
    setIfMissing(d, "excellon_units", app["excellon_units"]);
    for (const char* k : {"excellon_format_upper_mm", "excellon_format_lower_mm", "excellon_format_upper_in",
                          "excellon_format_lower_in"})
        setIfMissing(d, k, app[k]);
    setIfMissing(d, "source_file", JsonValue::string(e.sourceFile()));
    JsonValue tools = JsonValue::object();
    const JsonValue& oldTools = d["tools"];
    ShapeList all;
    for (const auto& [id, t] : e.drillTools()) {
        const std::string key = std::to_string(id);
        JsonValue tv = oldTools[key].isObject() ? oldTools[key] : JsonValue::object();
        setIfMissing(tv, "data", JsonValue::object());
        tv.set("tooldia", real(t.tooldia));
        JsonValue drills = JsonValue::array();
        for (const Point2D& p : t.drills) drills.push(shp(Shape::point(p)));
        tv.set("drills", drills);
        JsonValue slots = JsonValue::array();
        for (const auto& [a, b] : t.slots) {
            JsonValue s = JsonValue::array();
            s.push(shp(Shape::point(a)));
            s.push(shp(Shape::point(b)));
            slots.push(s);
        }
        tv.set("slots", slots);
        tv.set("solid_geometry", shpList(t.solidGeometry));
        all.insert(all.end(), t.solidGeometry.begin(), t.solidGeometry.end());
        tools.set(key, tv);
    }
    d.set("tools", tools);
    d.set("solid_geometry", shpList(all));
    d.set("kind", JsonValue::string("excellon"));
    d.set("options", objectOptions(obj, "excellon", app, {"tools_drill_", "tools_mill_"}));
    return d;
}

/// `default_data` de una Geometría: geometry_* y tools_mill_* sin el prefijo.
JsonValue geometryDefaultData(const JsonValue& app) {
    JsonValue data = JsonValue::object();
    for (const auto& [k, v] : app.members()) {
        if (k.rfind("geometry_", 0) == 0) data.set(k.substr(9), v);
        else if (k.rfind("tools_mill_", 0) == 0) data.set(k.substr(11), v);
    }
    return data;
}

JsonValue geometryDict(const GeometryObject& obj, const JsonValue& app) {
    JsonValue d = base(obj);
    setIfMissing(d, "alpha_level", JsonValue::string("FF"));
    setIfMissing(d, "fill_color", JsonValue::string("#FFAAAA"));
    setIfMissing(d, "outline_color", JsonValue::string("#FFAAAA"));
    setIfMissing(d, "units", JsonValue::string("MM"));
    setIfMissing(d, "follow_geometry", JsonValue());
    d.set("multigeo", JsonValue::boolean(true));
    JsonValue tools = JsonValue::object();
    ShapeList all;
    for (const auto& [uid, t] : obj.tools()) {
        JsonValue tv = JsonValue::object();
        JsonValue data = geometryToolDataJson(t, t.projectData.isObject() ? t.projectData : geometryDefaultData(app));
        data.set("name", JsonValue::string(obj.name().toStdString()));
        tv.set("data", data);
        tv.set("offset", JsonValue::string(t.offset.toStdString()));
        tv.set("offset_value", real(t.offsetValue));
        tv.set("solid_geometry", shpList(t.solidGeometry));
        tv.set("tool_type", JsonValue::string(t.toolType.toStdString()));
        tv.set("tooldia", real(t.tooldia));
        tv.set("type", JsonValue::string(t.type.toStdString()));
        tools.set(std::to_string(uid), tv);
        all.insert(all.end(), t.solidGeometry.begin(), t.solidGeometry.end());
    }
    d.set("tools", tools);
    d.set("solid_geometry", shpList(all));
    d.set("kind", JsonValue::string("geometry"));
    d.set("options", objectOptions(obj, "geometry", app, {"tools_mill_"}));
    return d;
}

/// Los trazados de corte de un `gcode_parsed` (para la geometría de cada herramienta del CNC Job).
ShapeList cutPaths(const JsonValue& parsed) {
    ShapeList out;
    for (const JsonValue& seg : parsed.items())
        if (seg["kind"].items().size() == 2 && seg["kind"].items()[0].text() == "C") collectShapes(seg["geom"], out);
    return out;
}

JsonValue cncDict(const CNCJobObject& obj, const JsonValue& app) {
    JsonValue d = base(obj);
    const bool drilling = obj.isDrilling();
    const std::vector<std::string>& gcodes = obj.job().toolGcodes();
    std::vector<CncToolRow> rows = obj.tools();
    if (drilling && gcodes.size() == 1 && rows.size() > 1) {
        // Taladrado sin cambio de herramienta: un solo trabajo, con la primera herramienta y todos los agujeros.
        for (std::size_t i = 1; i < rows.size(); ++i) {
            rows.front().drills += rows[i].drills;
            rows.front().slots += rows[i].slots;
        }
        rows.resize(1);
    }
    const JsonValue first = rows.empty() ? JsonValue::object() : rows.front().data;
    auto pick = [&](const char* geoKey, const char* drillKey, JsonValue def) {
        const JsonValue& v = first[drilling ? drillKey : geoKey];
        return v.isNull() ? def : v;
    };
    setIfMissing(d, "append_snippet", JsonValue::string(""));
    setIfMissing(d, "prepend_snippet", JsonValue::string(""));
    setIfMissing(d, "dwell", pick("dwell", "tools_drill_dwell", JsonValue::boolean(false)));
    setIfMissing(d, "dwelltime", pick("dwelltime", "tools_drill_dwelltime", real(1.0)));
    setIfMissing(d, "feedrate", pick("feedrate", "tools_drill_feedrate_z", real(120.0)));
    setIfMissing(d, "feedrate_rapid", pick("feedrate_rapid", "tools_drill_feedrate_rapid", real(1500.0)));
    setIfMissing(d, "spindlespeed", pick("spindlespeed", "tools_drill_spindlespeed", JsonValue()));
    setIfMissing(d, "z_cut", pick("cutz", "tools_drill_cutz", real(0.0)));
    setIfMissing(d, "z_depthpercut", pick("depthperpass", "tools_drill_depthperpass", real(0.0)));
    setIfMissing(d, "z_feedrate", pick("feedrate_z", "tools_drill_feedrate_z", real(0.0)));
    setIfMissing(d, "z_move", pick("travelz", "tools_drill_travelz", real(2.0)));
    setIfMissing(d, "z_toolchange", pick("toolchangez", "tools_drill_toolchangez", real(15.0)));
    setIfMissing(d, "tooldia", rows.empty() ? real(0.0) : real(rows.back().dia));
    setIfMissing(d, "steps_per_circle", JsonValue::integer(64));
    setIfMissing(d, "input_geometry_bounds", JsonValue());
    setIfMissing(d, "follow_geometry", JsonValue());
    setIfMissing(d, "tools", JsonValue());
    setIfMissing(d, "units", JsonValue::string("MM"));
    d.set("gc_header", JsonValue::string(obj.headerText()));
    d.set("multitool", JsonValue::boolean(true));
    d.set("origin_kind", drilling ? JsonValue::string("excellon") : JsonValue());
    JsonValue cnc = JsonValue::object(), exc = JsonValue::object();
    JsonValue lastParsed = JsonValue::array();
    ShapeList drillCircles;
    for (std::size_t i = 0; i < rows.size(); ++i) {
        const CncToolRow& r = rows[i];
        const std::string g = i < gcodes.size() ? gcodes[i] : std::string();
        const bool laser = r.data[drilling ? "tools_drill_ppname_e" : "ppname_g"].text().find("laser") != std::string::npos;
        JsonValue parsed = ProjectIO::gcodeParse(g, laser, drilling ? r.dia : 0.0);
        if (drilling) {
            const std::string key = JsonValue::real(r.dia).text();
            JsonValue tv = d["exc_cnc_tools"][key].isObject() ? d["exc_cnc_tools"][key] : JsonValue::object();
            tv.set("data", r.data.isObject() ? r.data : JsonValue::object());
            tv.set("gcode", JsonValue::string(g));
            tv.set("gcode_parsed", parsed);
            tv.set("nr_drills", JsonValue::integer(r.drills));
            tv.set("nr_slots", JsonValue::integer(r.slots));
            setIfMissing(tv, "offset", JsonValue::integer(0));
            const ShapeList circles = cutPaths(parsed);
            tv.set("solid_geometry", shpList(circles));
            drillCircles.insert(drillCircles.end(), circles.begin(), circles.end());
            tv.set("tool", JsonValue::integer(r.tool));
            exc.set(key, tv);
        } else {
            const std::string key = std::to_string(r.tool);
            JsonValue tv = d["cnc_tools"][key].isObject() ? d["cnc_tools"][key] : JsonValue::object();
            tv.set("data", r.data.isObject() ? r.data : JsonValue::object());
            tv.set("gcode", JsonValue::string(g));
            tv.set("gcode_parsed", parsed);
            tv.set("offset", JsonValue::string(r.offset.toStdString()));
            setIfMissing(tv, "offset_value", real(0.0));
            const ShapeList paths = cutPaths(parsed);
            tv.set("solid_geometry", paths.empty() ? JsonValue() : shp(Shape::collection(paths)));
            tv.set("tool_type", JsonValue::string(r.toolType.toStdString()));
            tv.set("tooldia", real(r.dia));
            tv.set("type", JsonValue::string(r.type.toStdString()));
            cnc.set(key, tv);
        }
        lastParsed = parsed;
    }
    d.set("cnc_tools", cnc);
    d.set("exc_cnc_tools", exc);
    // Atributos del último trabajo generado (`gcode`, `gcode_parsed`), como queda el objeto en el original.
    d.set("gcode", JsonValue::string(gcodes.empty() ? obj.job().gcode() : gcodes.back()));
    d.set("gcode_parsed", lastParsed);
    d.set("solid_geometry", drilling ? shpList(drillCircles) : JsonValue());
    d.set("kind", JsonValue::string("cncjob"));
    d.set("options", objectOptions(obj, "cncjob", app, {}));
    return d;
}

}  // namespace

const JsonValue& ProjectIO::factoryDefaults() {
    static const JsonValue defaults = [] {
        QFile f(":/factory_defaults.json");
        JsonValue v;
        std::string err;
        if (f.open(QIODevice::ReadOnly)) ProjectFile::parse(f.readAll().toStdString(), v, err);
        return v;
    }();
    return defaults;
}

JsonValue ProjectIO::gcodeParse(const std::string& gcode, bool laser, double drillDia) {
    // codes_split: letras con su número desde el comienzo de la línea (en el láser, X/Y en cualquier parte y M3/M5
    // como Z); gcode_parse: cada cambio de Z cierra el trazado actual.
    static const QRegularExpression code(R"(^\s*([A-Z])\s*([\+\-\.\d\s]+))");
    static const QRegularExpression laserXY(R"(X([\+-]?\d+.[\+-]?\d+)\s*Y([\+-]?\d+.[\+-]?\d+))");
    static const QRegularExpression laserM(R"(^(M0?[3-5]))");
    JsonValue geometry = JsonValue::array();
    double cx = 0, cy = 0, cz = 0;
    int cg = 0;
    std::vector<Point2D> path{{0.0, 0.0}};  // geometry_toolchangexy / tools_drill_toolchangexy de fábrica: (0, 0)
    QString k0 = "C", k1 = "F";
    auto emit = [&] {
        if (path.size() < 2) return;
        JsonValue seg = JsonValue::object();
        seg.set("geom", shp(Shape::lineString(path)));
        JsonValue kind = JsonValue::array();
        kind.push(JsonValue::string(k0.toStdString()));
        kind.push(JsonValue::string(k1.toStdString()));
        seg.set("kind", kind);
        geometry.push(seg);
        path = {path.back()};
    };
    for (const QString& line : QString::fromStdString(gcode).split('\n')) {
        std::map<char, double> g;
        if (laser) {
            if (const auto m = laserXY.match(line); m.hasMatch()) {
                g['X'] = m.captured(1).toDouble();
                g['Y'] = m.captured(2).toDouble();
            }
            if (const auto m = laserM.match(line); m.hasMatch()) g['Z'] = m.captured(1).contains('5') ? 1 : 0;
        } else {
            QString rest = line;
            for (auto m = code.match(rest); m.hasMatch(); m = code.match(rest)) {
                g[m.captured(1).at(0).toLatin1()] = m.captured(2).remove(' ').toDouble();
                rest = rest.mid(m.capturedEnd());
            }
        }
        if (g.count('G') && (g['G'] == 20.0 || g['G'] == 21.0)) continue;
        if (g.count('Z')) {
            cz = g['Z'];
            emit();
            if (drillDia > 0 && cz < 0) {
                JsonValue seg = JsonValue::object();
                seg.set("geom", shp(Shape::point({cx, cy}).buffer(drillDia / 2.0, 16).exterior()));
                JsonValue kind = JsonValue::array();
                kind.push(JsonValue::string("C"));
                kind.push(JsonValue::string("F"));
                seg.set("kind", kind);
                geometry.push(seg);
            }
        }
        if (g.count('G')) cg = static_cast<int>(g['G']);
        if (g.count('X') || g.count('Y')) {
            const double x = g.count('X') ? g['X'] : cx, y = g.count('Y') ? g['Y'] : cy;
            k0 = cz > 0 ? "T" : "C";
            k1 = cg > 0 ? "S" : "F";
            if (cg == 0 || cg == 1) path.push_back({x, y});
            cx = x;
            cy = y;
        }
    }
    emit();
    return geometry;
}

JsonValue ProjectIO::toDict(const FlatCAMObj& obj, const JsonValue& appOptions) {
    switch (obj.kind()) {
        case ObjectKind::Gerber: return gerberDict(static_cast<const GerberObject&>(obj), appOptions);
        case ObjectKind::Excellon: return excellonDict(static_cast<const ExcellonObject&>(obj), appOptions);
        case ObjectKind::Geometry: return geometryDict(static_cast<const GeometryObject&>(obj), appOptions);
        case ObjectKind::CNCJob: return cncDict(static_cast<const CNCJobObject&>(obj), appOptions);
        default: return base(obj);
    }
}

JsonValue ProjectIO::projectDict(const std::vector<FlatCAMObj*>& objs, const JsonValue& appOptions) {
    JsonValue root = JsonValue::object();
    JsonValue list = JsonValue::array();
    for (const FlatCAMObj* o : objs) list.push(toDict(*o, appOptions));
    root.set("objs", list);
    root.set("options", appOptions);
    root.set("version", JsonValue::string("2024.4"));
    return root;
}

}  // namespace flatcam::gui
