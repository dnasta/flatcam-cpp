#include "gui/ToolsDatabase.h"

#include <QDir>
#include <QFile>
#include <QStringList>
#include <cmath>

#include "gui/ProjectIO.h"
#include "gui/tools/ClearingTools.h"
#include "io/ProjectFile.h"

namespace flatcam::gui {

namespace {

/// Las claves de `data` de una herramienta nueva y de qué preferencia salen (`ToolsDB2.on_tool_add`, extraídas del
/// original con `ast`); `cast` es la conversión que hace el original (`float`, `int`) o el valor fijo.
struct DefaultKey {
    const char* key;
    const char* option;
    const char* cast;
};

const DefaultKey kDefaults[] = {
    {"plot", nullptr, "True"}, {"tool_target", nullptr, "0"}, {"tol_min", nullptr, "0.0"}, {"tol_max", nullptr, "0.0"},
    {"cutz", "geometry_cutz", "float"}, {"multidepth", "geometry_multidepth", ""},
    {"depthperpass", "geometry_depthperpass", "float"}, {"vtipdia", "geometry_vtipdia", "float"},
    {"vtipangle", "geometry_vtipangle", "float"}, {"travelz", "geometry_travelz", "float"},
    {"feedrate", "geometry_feedrate", "float"}, {"feedrate_z", "geometry_feedrate_z", "float"},
    {"feedrate_rapid", "geometry_feedrate_rapid", "float"}, {"spindlespeed", "geometry_spindlespeed", ""},
    {"dwell", "geometry_dwell", ""}, {"dwelltime", "geometry_dwelltime", "float"}, {"ppname_g", "geometry_ppname_g", ""},
    {"extracut", "geometry_extracut", ""}, {"extracut_length", "geometry_extracut_length", "float"},
    {"toolchange", "geometry_toolchange", ""}, {"toolchangexy", "geometry_toolchangexy", ""},
    {"toolchangez", "geometry_toolchangez", "float"}, {"startz", "geometry_startz", ""},
    {"endz", "geometry_endz", "float"}, {"endxy", "geometry_endxy", ""}, {"search_time", "geometry_search_time", "int"},
    {"z_pdepth", "geometry_z_pdepth", "float"}, {"f_plunge", "geometry_f_plunge", "float"},
    {"spindledir", "geometry_spindledir", ""}, {"optimization_type", "geometry_optimization_type", ""},
    {"feedrate_probe", "geometry_feedrate_probe", ""}, {"segx", "geometry_segx", ""}, {"segy", "geometry_segy", ""},
    {"area_exclusion", "geometry_area_exclusion", ""}, {"area_shape", "geometry_area_shape", ""},
    {"area_strategy", "geometry_area_strategy", ""}, {"area_overz", "geometry_area_overz", ""},
    {"polish", "geometry_polish", ""}, {"polish_dia", "geometry_polish_dia", ""},
    {"polish_pressure", "geometry_polish_pressure", ""}, {"polish_travelz", "geometry_polish_travelz", ""},
    {"polish_margin", "geometry_polish_margin", ""}, {"polish_overlap", "geometry_polish_overlap", ""},
    {"polish_method", "geometry_polish_method", ""}, {"tools_ncc_operation", "tools_ncc_operation", ""},
    {"tools_ncc_milling_type", "tools_ncc_milling_type", ""}, {"tools_ncc_overlap", "tools_ncc_overlap", "float"},
    {"tools_ncc_margin", "tools_ncc_margin", "float"}, {"tools_ncc_method", "tools_ncc_method", ""},
    {"tools_ncc_connect", "tools_ncc_connect", ""}, {"tools_ncc_contour", "tools_ncc_contour", ""},
    {"tools_ncc_offset_choice", "tools_ncc_offset_choice", ""},
    {"tools_ncc_offset_value", "tools_ncc_offset_value", "float"}, {"tools_paint_overlap", "tools_paint_overlap", "float"},
    {"tools_paint_offset", "tools_paint_offset", "float"}, {"tools_paint_method", "tools_paint_method", ""},
    {"tools_paint_connect", "tools_paint_connect", ""}, {"tools_paint_contour", "tools_paint_contour", ""},
    {"tools_iso_passes", "tools_iso_passes", "int"}, {"tools_iso_overlap", "tools_iso_overlap", "float"},
    {"tools_iso_milling_type", "tools_iso_milling_type", ""}, {"tools_iso_follow", "tools_iso_follow", ""},
    {"tools_iso_isotype", "tools_iso_isotype", ""}, {"tools_drill_cutz", "tools_drill_cutz", "float"},
    {"tools_drill_multidepth", "tools_drill_multidepth", ""},
    {"tools_drill_depthperpass", "tools_drill_depthperpass", "float"},
    {"tools_drill_travelz", "tools_drill_travelz", "float"},
    {"tools_drill_feedrate_z", "tools_drill_feedrate_z", "float"},
    {"tools_drill_feedrate_rapid", "tools_drill_feedrate_rapid", "float"},
    {"tools_drill_spindlespeed", "tools_drill_spindlespeed", "float"}, {"tools_drill_dwell", "tools_drill_dwell", ""},
    {"tools_drill_offset", "tools_drill_offset", "float"}, {"tools_drill_drill_slots", "tools_drill_drill_slots", ""},
    {"tools_drill_drill_overlap", "tools_drill_drill_overlap", "float"},
    {"tools_drill_last_drill", "tools_drill_last_drill", ""}, {"tools_cutout_margin", "tools_cutout_margin", "float"},
    {"tools_cutout_gapsize", "tools_cutout_gapsize", "float"}, {"tools_cutout_gaps_ff", "tools_cutout_gaps_ff", ""},
    {"tools_cutout_convexshape", "tools_cutout_convexshape", ""},
    {"tools_cutout_gap_type", "tools_cutout_gap_type", ""},
    {"tools_cutout_gap_depth", "tools_cutout_gap_depth", "float"},
    {"tools_cutout_mb_dia", "tools_cutout_mb_dia", "float"},
    {"tools_cutout_mb_spacing", "tools_cutout_mb_spacing", "float"},
};

double num(const JsonValue& v, double def = 0.0) {
    if (v.isNumber() || v.isBool()) return v.toDouble(def);
    if (v.isString()) {
        bool ok = false;
        const double d = QString::fromStdString(v.text()).toDouble(&ok);
        return ok ? d : def;
    }
    return def;
}

double round4(double v) { return std::round(v * 1e4) / 1e4; }

}  // namespace

QString ToolsDatabase::fileName(const QString& dataPath) { return QDir(dataPath).filePath("tools_db.FlatDB"); }

void ToolsDatabase::ensureFile(const QString& dataPath) {
    const QString path = fileName(dataPath);
    if (QFile::exists(path)) return;
    QDir().mkpath(dataPath);
    QFile f(path);
    if (f.open(QIODevice::WriteOnly)) f.write("{}");
}

bool ToolsDatabase::load(const QString& path, JsonValue& db, QString& error) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        error = "Could not load the file.";
        return false;
    }
    std::string err;
    if (!ProjectFile::parse(f.readAll().toStdString(), db, err) || !db.isObject()) {
        error = "Failed to parse Tools DB file.";
        return false;
    }
    return true;
}

bool ToolsDatabase::save(const QString& path, const JsonValue& db) {
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly)) return false;
    const std::string text = ProjectFile::dump(db, false);
    return f.write(text.data(), static_cast<qint64>(text.size())) == static_cast<qint64>(text.size());
}

JsonValue ToolsDatabase::newTool(const JsonValue& db, const JsonValue& defaults) {
    const JsonValue& defs = defaults;
    JsonValue data = JsonValue::object();
    for (const DefaultKey& k : kDefaults) {
        const std::string cast = k.cast;
        if (!k.option) {
            if (cast == "True") data.set(k.key, JsonValue::boolean(true));
            else if (cast == "0") data.set(k.key, JsonValue::integer(0));
            else data.set(k.key, JsonValue::real(0.0));
            continue;
        }
        const JsonValue& v = defs[k.option];
        if (cast == "float") data.set(k.key, JsonValue::real(num(v)));
        else if (cast == "int") data.set(k.key, JsonValue::integer(static_cast<long long>(num(v))));
        else data.set(k.key, v);
    }
    // El nombre: new_tool_<máximo + 1>.
    double maxN = 0;
    bool any = false;
    for (const auto& [id, t] : db.members()) {
        const QString name = QString::fromStdString(t["name"].text());
        if (!name.contains("new_tool_")) continue;
        any = true;
        maxN = std::max(maxN, name.section('_', -1).toDouble());
    }
    JsonValue tool = JsonValue::object();
    tool.set("name", JsonValue::string(any ? "new_tool_" + std::to_string(static_cast<long long>(maxN + 1)) : "new_tool_1"));
    // geometry_cnctooldia es un texto "2.4" (o una lista "2.4, 1.0"): el primero.
    const JsonValue& cnc = defs["geometry_cnctooldia"];
    double dia = cnc.isNumber() ? num(cnc) : QString::fromStdString(cnc.text()).split(',').value(0).trimmed().toDouble();
    tool.set("tooldia", JsonValue::real(dia));
    tool.set("offset", JsonValue::string("Path"));
    tool.set("offset_value", JsonValue::real(0.0));
    tool.set("type", JsonValue::string("Rough"));
    tool.set("tool_type", JsonValue::string("C1"));
    tool.set("data", data);
    return tool;
}

ToolsDatabase::LookupResult ToolsDatabase::lookup(const QString& path,
                                                  const std::function<bool(const JsonValue&)>& targetMatches,
                                                  double truncatedDia) {
    LookupResult r;
    JsonValue db;
    QString error;
    if (!load(path, db, error)) {
        r.status = error.startsWith("Could") ? Lookup::NotLoaded : Lookup::ParseError;
        return r;
    }
    int found = 0;
    for (const auto& [id, t] : db.members()) {
        const JsonValue& data = t["data"];
        if (!targetMatches(data["tool_target"])) continue;
        const double dbDia = num(t["tooldia"]);
        const double lo = num(data["tol_min"]), hi = num(data["tol_max"]);
        if (truncatedDia == dbDia) {
            ++found;
            r.tool = t;
        } else if (hi >= truncatedDia && truncatedDia >= lo) {
            ++found;
            r.tool = t;
            r.updatedDia = dbDia;
        }
    }
    r.status = found == 0 ? Lookup::NotFound : found > 1 ? Lookup::Multiple : Lookup::Found;
    return r;
}

void ToolsDatabase::applyTo(const JsonValue& tool, ClearingTool& t, const std::string& prefix) {
    t.geo.tooldia = round4(num(tool["tooldia"], t.geo.tooldia));
    if (tool["offset"].isString()) t.geo.offset = QString::fromStdString(tool["offset"].text());
    t.geo.offsetValue = num(tool["offset_value"], t.geo.offsetValue);
    if (tool["type"].isString()) t.geo.type = QString::fromStdString(tool["type"].text());
    if (tool["tool_type"].isString()) t.geo.toolType = QString::fromStdString(tool["tool_type"].text());
    for (const auto& [key, v] : tool["data"].members()) {
        // on_tool_add: los propios del prefijo y los generales; los de otras herramientas (tools_*), no.
        if (!prefix.empty() && key.rfind("tools_", 0) == 0 && key.rfind(prefix, 0) != 0) continue;
        GeometryToolSettings& d = t.geo.data;
        const JsonValue& val = v;
        auto str = [&val] { return QString::fromStdString(val.text()); };
        if (key == "cutz") d.cutZ = num(v);
        else if (key == "multidepth") d.multidepth = v.toBool();
        else if (key == "depthperpass") d.depthPerPass = num(v);
        else if (key == "travelz") d.travelZ = num(v);
        else if (key == "feedrate") d.feedrate = num(v);
        else if (key == "feedrate_z") d.feedrateZ = num(v);
        else if (key == "feedrate_rapid") d.feedrateRapid = num(v);
        else if (key == "spindlespeed") d.spindleSpeed = num(v);
        else if (key == "dwell") d.dwell = v.toBool();
        else if (key == "dwelltime") d.dwellTime = num(v);
        else if (key == "endz") d.endZ = num(v);
        else if (key == "toolchangez") d.toolchangeZ = num(v);
        else if (key == "extracut") d.extracut = v.toBool();
        else if (key == "extracut_length") d.extracutLength = num(v);
        else if (key == "vtipdia") t.geo.vtipDia = num(v);
        else if (key == "vtipangle") t.geo.vtipAngle = num(v);
        else if (key == "ppname_g" && v.isString()) t.geo.ppname = str();
        else if (key == "toolchange") t.geo.toolchange = v.toBool();
        else if (key == "tools_iso_passes") t.passes = static_cast<int>(num(v, t.passes));
        else if (key == "tools_iso_overlap" || key == "tools_ncc_overlap" || key == "tools_paint_overlap") t.overlap = num(v);
        else if (key == "tools_iso_milling_type" || key == "tools_ncc_milling_type") t.millingType = str();
        else if (key == "tools_iso_follow") t.follow = v.toBool();
        else if (key == "tools_iso_isotype") t.isoType = str();
        else if (key == "tools_ncc_operation") t.operation = str();
        else if (key == "tools_ncc_margin") t.margin = num(v);
        else if (key == "tools_ncc_method" || key == "tools_paint_method") t.method = static_cast<int>(num(v));
        else if (key == "tools_ncc_connect" || key == "tools_paint_connect") t.connect = v.toBool();
        else if (key == "tools_ncc_contour" || key == "tools_paint_contour") t.contour = v.toBool();
        else if (key == "tools_ncc_offset_choice") t.offsetChoice = v.toBool();
        else if (key == "tools_ncc_offset_value" || key == "tools_paint_offset") t.offset = num(v);
    }
}

}  // namespace flatcam::gui
