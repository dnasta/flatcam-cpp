#include "gui/objects/GeometryObject.h"
#include "gui/Preferences.h"

#include "cam/Preprocessor.h"
#include "core/AffineTransform.h"
#include "gui/Translation.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/ObjectUIs.h"

namespace flatcam::gui {

GeometryTool defaultGeometryTool() {
    // geometry_* de las preferencias (los que usa geometry_tool_gcode_gen); de fábrica: 2.4, -2.4, 0.8, 2, 120, 60...
    GeometryTool t;
    t.tooldia = defNum("geometry_cnctooldia", 2.4);  // un texto ("2.4") en un FCEntry
    GeometryToolSettings& d = t.data;
    d.cutZ = defNum("geometry_cutz");
    d.multidepth = defBool("geometry_multidepth");
    d.depthPerPass = defNum("geometry_depthperpass");
    d.travelZ = defNum("geometry_travelz");
    d.feedrate = defNum("geometry_feedrate");
    d.feedrateZ = defNum("geometry_feedrate_z");
    d.feedrateRapid = defNum("geometry_feedrate_rapid");
    d.spindleSpeed = defNum("geometry_spindlespeed");
    d.spindleDir = defStr("geometry_spindledir").toStdString();
    d.dwell = defBool("geometry_dwell");
    d.dwellTime = defNum("geometry_dwelltime");
    d.endZ = defNum("geometry_endz");
    d.endXY = defPair("geometry_endxy");  // None de fábrica
    if (appDefaults()["geometry_startz"].isNumber()) d.startZ = defNum("geometry_startz");
    d.toolchangeZ = defNum("geometry_toolchangez");
    d.toolchangeXY = defPair("geometry_toolchangexy");  // "0.0, 0.0"
    d.extracut = defBool("geometry_extracut");
    d.extracutLength = defNum("geometry_extracut_length");
    d.tolerance = defNum("global_tolerance", 0.005) / 20;  // 0,00025, en mtool_gen_cncjob
    return t;
}

JsonValue geometryToolDataJson(const GeometryTool& t, JsonValue base) {
    if (!base.isObject()) base = JsonValue::object();
    const GeometryToolSettings& d = t.data;
    auto pair = [](const std::optional<std::array<double, 2>>& p) {
        if (!p) return JsonValue();
        JsonValue a = JsonValue::array();
        a.push(JsonValue::real((*p)[0]));
        a.push(JsonValue::real((*p)[1]));
        return a;
    };
    base.set("cutz", JsonValue::real(d.cutZ));
    base.set("multidepth", JsonValue::boolean(d.multidepth));
    base.set("depthperpass", JsonValue::real(d.depthPerPass));
    base.set("travelz", JsonValue::real(d.travelZ));
    base.set("feedrate", JsonValue::real(d.feedrate));
    base.set("feedrate_z", JsonValue::real(d.feedrateZ));
    base.set("feedrate_rapid", JsonValue::real(d.feedrateRapid));
    base.set("spindlespeed", JsonValue::integer(static_cast<long long>(d.spindleSpeed)));
    base.set("spindledir", JsonValue::string(d.spindleDir));
    base.set("dwell", JsonValue::boolean(d.dwell));
    base.set("dwelltime", JsonValue::real(d.dwellTime));
    base.set("startz", d.startZ ? JsonValue::real(*d.startZ) : JsonValue());
    base.set("endz", JsonValue::real(d.endZ));
    base.set("endxy", pair(d.endXY));
    base.set("toolchange", JsonValue::boolean(t.toolchange));
    base.set("toolchangez", JsonValue::real(d.toolchangeZ));
    base.set("toolchangexy", pair(d.toolchangeXY));
    base.set("extracut", JsonValue::boolean(d.extracut));
    base.set("extracut_length", JsonValue::real(d.extracutLength));
    base.set("ppname_g", JsonValue::string(t.ppname.toStdString()));
    base.set("optimization_type", JsonValue::string(t.optimization.toStdString()));
    base.set("vtipdia", JsonValue::real(t.vtipDia));
    base.set("vtipangle", JsonValue::real(t.vtipAngle));
    if (t.overrideColor.isValid()) {
        const QColor c = t.overrideColor;
        base.set("override_color", JsonValue::string(QString::asprintf("#%02x%02x%02x%02x", c.red(), c.green(), c.blue(),
                                                                        c.alpha()).toStdString()));
    }
    return base;
}

GeometryObject::GeometryObject(QString name)
    : FlatCAMObj(ObjectKind::Geometry, std::move(name)), units_(appUnitsKind()) {
    outlineColor = plotLineColor();
    fillColor = Qt::transparent;
}

QColor GeometryObject::plotLineColor() { return colorFromFlatCam("#FFAAAA"); }

PlotLayerData GeometryObject::plotData() const {
    PlotLayerData d;
    d.lineColor = outlineColor;
    d.fillColor = fillColor;
    // plot() con multigeo: la geometría de cada herramienta, solo contornos; las que tienen color propio
    // (`override_color`, los puentes finos del Corte) van en su propio grupo.
    for (const auto& [uid, t] : tools_) {
        if (t.overrideColor.isValid()) {
            PlotGroup g;
            g.lineColor = t.overrideColor;
            for (const Shape& s : t.solidGeometry) addShape(s, g, d.bounds, false);
            d.groups.push_back(std::move(g));
        } else {
            for (const Shape& s : t.solidGeometry) addShape(s, d, false);
        }
    }
    return d;
}

QRectF GeometryObject::bounds() const { return plotData().bounds; }

bool GeometryObject::mirror(Axis axis, const Point2D& point) {
    const AffineTransform m = AffineTransform::mirror(axis, point);
    for (Shape& s : solid_) s = s.transform(m);
    for (auto& [uid, t] : tools_)
        for (Shape& s : t.solidGeometry) s = s.transform(m);
    return true;
}

template <typename Fn>
void GeometryObject::applyToAll(Fn fn) {
    for (Shape& s : solid_) s = fn(s);
    for (auto& [uid, t] : tools_)
        for (Shape& s : t.solidGeometry) s = fn(s);
}

bool GeometryObject::rotate(double angle, const Point2D& p) {
    applyToAll([&](const Shape& s) { return s.rotate(angle, p); });
    return true;
}

bool GeometryObject::skew(double ax, double ay, const Point2D& p) {
    applyToAll([&](const Shape& s) { return s.skew(ax, ay, p); });
    return true;
}

bool GeometryObject::scale(double fx, double fy, const Point2D& p) {
    applyToAll([&](const Shape& s) { return s.scale(fx, fy, p); });
    return true;
}

bool GeometryObject::offset(double dx, double dy) {
    applyToAll([&](const Shape& s) { return s.translate(dx, dy); });
    return true;
}

bool GeometryObject::buffer(double distance, JoinStyle join, bool factor) {
    if (distance == 0.0) return true;
    applyToAll([&](const Shape& s) { return factor ? s.scale(distance, distance) : s.buffer(distance, 64, join); });
    return true;
}

QWidget* GeometryObject::propertiesUi() {
    if (!ui_) ui_ = std::make_unique<GeometryObjectUI>();
    static_cast<GeometryObjectUI*>(ui_.get())->build(*this);
    return ui_.get();
}

std::unique_ptr<CNCJobObject> GeometryObject::makeCncJob(const std::vector<int>& toolIds, QString& error,
                                                         const std::vector<ExclusionArea>& exclusionAreas) const {
    if (toolIds.empty()) {
        error = "[ERROR_NOTCL] " + tr_("Failed. No tool selected in the tool table ...");
        return nullptr;
    }
    bool empty = true;
    for (int id : toolIds)
        for (const Shape& sh : tools_.at(id).solidGeometry) empty = empty && sh.isEmpty();
    if (empty) {
        error = "[ERROR_NOTCL] " + tr_("Cancelled. Empty file, it has no geometry") + "...";
        return nullptr;
    }
    // options['xmin'...]: los límites de la geometría del objeto.
    const QRectF b = bounds();
    std::vector<GeometryToolJob> jobs;
    std::vector<CncToolRow> rows;
    for (int id : toolIds) {
        const GeometryTool& t = tools_.at(id);
        if (!makePreprocessor(t.ppname.toStdString())) {
            error = QString("[ERROR_NOTCL] %1: %2").arg(tr_("Preprocessor"), t.ppname);
            return nullptr;
        }
        GeometryToolJob j;
        GeometryJobSettings& s = j.settings;
        s.preprocessor = t.ppname.toStdString();
        s.order = t.optimization == "R" ? DrillOrder::RTree : DrillOrder::FileOrder;
        s.toolchange = true;  // mtool_gen_cncjob pasa toolchange=True a geometry_tool_gcode_gen
        s.units = appUnits().toStdString();
        s.coordsDecimals = defInt("cncjob_coords_decimals", 4);
        s.frDecimals = defInt("cncjob_fr_decimals", 2);
        s.decimals = appDecimals();
        s.stepsPerCircle = defInt("geometry_circle_steps", 64);
        s.tool = id;          // self.tool = str(tool): el identificador de la herramienta (la 9999 imprime T9999)
        s.toolSettings = t.data;
        s.toolSettings.toolDia = t.tooldia;
        s.toolSettings.offsetKind = t.offset.toLower() == "path" ? "Path" : t.offset.toLower().toStdString();
        s.toolSettings.offsetValue = t.offsetValue;
        s.headerBounds = BoundingBox{b.left(), b.top(), b.right(), b.bottom()};
        s.exclusionAreas = exclusionAreas;
        for (const Shape& sh : t.solidGeometry) {
            if (sh.isEmpty()) continue;
            const ShapeList parts = sh.parts();
            if (parts.empty()) j.geometry.push_back(sh);
            else j.geometry.insert(j.geometry.end(), parts.begin(), parts.end());
        }
        jobs.push_back(std::move(j));
        CncToolRow row;
        row.tool = id;
        row.dia = t.tooldia;
        row.cutZ = t.data.cutZ;
        row.offset = t.offset;
        row.type = t.type;
        row.toolType = t.toolType;
        row.data = geometryToolDataJson(t, t.projectData);
        rows.push_back(row);
    }
    auto job = std::make_unique<CncJob>();
    if (!job->generateFromGeometryTools(jobs)) {
        error = "[ERROR_NOTCL] " + QString::fromStdString(job->lastError());
        return nullptr;
    }
    GcodeHeaderInfo h;
    h.version = "2024.4";
    h.versionDate = "2024/04/15";
    h.type = "Geometry";
    h.units = appUnits().toStdString();
    auto obj = std::make_unique<CNCJobObject>(name() + "_cnc", std::move(job), h, std::move(rows));
    return obj;
}

}  // namespace flatcam::gui

namespace flatcam::gui {

double GeometryObject::convertUnits(Units units) {
    // camlib.Geometry.convert_units: scale(factor, factor) desde el origen.
    if (units == units_) return 1.0;
    const double factor = units == Units::Millimeters ? 25.4 : 1 / 25.4;
    units_ = units;
    scale(factor, factor, {0, 0});
    const int dec = appDecimals();
    for (auto& [id, t] : tools_) {
        t.tooldia = QString::number(t.tooldia * factor, 'f', dec).toDouble();  // float('%.*f' % (decimals, dia))
        t.offsetValue *= factor;
        GeometryToolSettings& d = t.data;
        for (double* v : {&d.cutZ, &d.depthPerPass, &d.travelZ, &d.feedrate, &d.feedrateZ, &d.feedrateRapid, &d.endZ,
                          &d.toolchangeZ})
            *v *= factor;
        t.solidGeometry.clear();  // error del original: 'solid_geometry' no pasa al diccionario nuevo
    }
    return factor;
}

}  // namespace flatcam::gui
