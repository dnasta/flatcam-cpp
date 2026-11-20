#include <algorithm>
#include <limits>

#include "core/OriginalBugs.h"
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

QColor GeometryObject::plotLineColor() { return colorFromFlatCam(defStr("geometry_plot_line")); }

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
    aliased_ = false;
    for (Shape& s : solid_) s = s.transform(m);
    if (singleGeo_) {
        syncSingleGeo();
        return true;
    }
    for (auto& [uid, t] : tools_)
        for (Shape& s : t.solidGeometry) s = s.transform(m);
    return true;
}

template <typename Fn>
void GeometryObject::applyToAll(Fn fn) {
    aliased_ = false;
    for (Shape& s : solid_) s = fn(s);
    if (singleGeo_) {
        syncSingleGeo();
        return;
    }
    for (auto& [uid, t] : tools_)
        for (Shape& s : t.solidGeometry) s = fn(s);
}

void GeometryObject::setSingleGeo(bool sharesList) {
    singleGeo_ = true;
    aliased_ = sharesList;
    toolAppendError_ = sharesList ? QString() : QString("'NoneType' object has no attribute 'append'");
    solidNotList_ = false;
    syncSingleGeo();
}

void GeometryObject::setSingleGeoShape(const Shape& shape) {
    static const char* const names[] = {"Point", "LineString", "LinearRing", "Polygon", "MultiPoint",
                                         "MultiLineString", "MultiPolygon", "GeometryCollection"};
    singleGeo_ = true;
    aliased_ = false;
    solid_ = {shape};
    solidNotList_ = true;
    toolAppendError_ = QString("'%1' object has no attribute 'append'").arg(names[static_cast<int>(shape.type())]);
    syncSingleGeo();
}

void GeometryObject::syncSingleGeo() {
    if (!singleGeo_) return;
    auto it = tools_.find(1);
    if (it != tools_.end()) it->second.solidGeometry = solid_;
}

void GeometryObject::setSolidGeometry(ShapeList shapes, bool asList) {
    solid_ = std::move(shapes);
    aliased_ = false;
    solidNotList_ = !asList;
    syncSingleGeo();
}

std::optional<QString> GeometryObject::addShapeLikeTheOriginal(const Shape& shape, bool circle) {
    static const char* const names[] = {"Point", "LineString", "LinearRing", "Polygon", "MultiPoint",
                                         "MultiLineString", "MultiPolygon", "GeometryCollection"};
    if (!reproduceOriginalBugs()) {
        // Corregido: la figura se agrega una vez a la geometría (que es siempre una lista) y a la herramienta 1 (el
        // original falla si la geometría es None o una figura sola, y con la lista compartida la agrega dos veces).
        solidNotList_ = false;
        aliased_ = false;
        toolAppendError_.clear();
        solid_.push_back(shape);
        auto t = tools_.find(1);
        if (t == tools_.end()) tools_[1] = defaultGeometryTool(), t = tools_.find(1);
        if (singleGeo_) syncSingleGeo();
        else t->second.solidGeometry.push_back(shape);
        if (singleGeo_) captureOptionBounds();
        return std::nullopt;
    }
    if (solidNotList_) {
        // solid_geometry es una figura: add_circle hace append (AttributeError, solo atrapa TypeError);
        // add_polygon / add_polyline la unen con la nueva.
        const Shape cur = solid_.empty() ? Shape() : solid_.front();
        if (circle)
            return QString("'%1' object has no attribute 'append'").arg(names[static_cast<int>(cur.type())]);
        solid_ = {cur.unite(shape)};
    } else {
        solid_.push_back(shape);
        if (aliased_) solid_.push_back(shape);  // la herramienta 1 es la misma lista
    }
    auto it = tools_.find(1);
    if (it == tools_.end()) return QString("1");  // KeyError: 1
    if (!toolAppendError_.isEmpty()) {
        syncSingleGeo();
        return toolAppendError_;  // tools[1]['solid_geometry'] es None o una figura
    }
    if (singleGeo_) syncSingleGeo();
    else it->second.solidGeometry.push_back(shape);
    if (singleGeo_) captureOptionBounds();  // add_*: "calculate bounds"
    return std::nullopt;
}

void GeometryObject::captureOptionBounds() {
    if (solid_.empty() && toolAppendError_.startsWith("'NoneType'")) {
        // solid_geometry is None: return 0, 0, 0, 0
        optionBounds_ = {JsonValue::integer(0), JsonValue::integer(0), JsonValue::integer(0), JsonValue::integer(0)};
        optionBoundsBox_ = {0, 0, 0, 0};
        return;
    }
    const double inf = std::numeric_limits<double>::infinity();
    BoundingBox b{inf, inf, -inf, -inf};
    for (const Shape& sh : solid_) {
        if (sh.isEmpty()) continue;
        const auto r = sh.bounds();
        if (!r) continue;
        b.xmin = std::min(b.xmin, r->xmin);
        b.ymin = std::min(b.ymin, r->ymin);
        b.xmax = std::max(b.xmax, r->xmax);
        b.ymax = std::max(b.ymax, r->ymax);
    }
    const auto num = [](double v) {
        // str(np.inf) es "inf"; JSON no tiene infinitos: se guarda el texto.
        return std::isinf(v) ? JsonValue::number(v > 0 ? "inf" : "-inf") : JsonValue::real(v);
    };
    optionBounds_ = {num(b.xmin), num(b.ymin), num(b.xmax), num(b.ymax)};
    optionBoundsBox_ = b;
}

BoundingBox GeometryObject::optionBoundsBox() const {
    if (optionBounds_) return optionBoundsBox_;
    const QRectF r = bounds();
    return BoundingBox{r.left(), r.top(), r.right(), r.bottom()};
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
                                                         const std::vector<ExclusionArea>& exclusionAreas,
                                                         const std::map<int, GeometryTool>* toolsDict,
                                                         QStringList* messages) const {
    const std::map<int, GeometryTool>& tools = toolsDict ? *toolsDict : tools_;
    if (toolIds.empty()) {
        error = "[ERROR_NOTCL] " + tr_("Failed. No tool selected in the tool table ...");
        return nullptr;
    }
    bool empty = true;
    for (int id : toolIds)
        for (const Shape& sh : tools.at(id).solidGeometry) empty = empty && sh.isEmpty();
    if (empty) {
        error = "[ERROR_NOTCL] " + tr_("Cancelled. Empty file, it has no geometry") + "...";
        return nullptr;
    }
    // options['xmin'...]: los límites de la geometría del objeto.
    const QRectF b = bounds();
    std::vector<GeometryToolJob> jobs;
    std::vector<CncToolRow> rows;
    for (int id : toolIds) {
        const GeometryTool& t = tools.at(id);
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
    const bool ok = job->generateFromGeometryTools(jobs);
    if (messages) {
        // mtool_gen_cncjob, por herramienta: lo que avisa geometry_tool_gcode_gen y el análisis del G-code.
        for (std::size_t i = 0; i < job->toolMessages().size(); ++i) {
            *messages << cncMessagesText(job->toolMessages()[i]);
            if (i >= job->toolGcodes().size()) break;
            *messages << "[success] " + tr_("G-Code parsing in progress...");
            *messages << gcodeParseMessages(job->toolGcodes()[i]);
            *messages << "[success] " + tr_("G-Code parsing finished...");
            *messages << "[success] " + tr_("Finished G-Code processing") + "...";
        }
    }
    if (!ok) {
        error = "[ERROR_NOTCL] " + tr_(QString::fromStdString(job->lastError()));
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
        // Error del original: 'solid_geometry' no pasa al diccionario nuevo (corregido: se conserva, ya escalada).
        if (reproduceOriginalBugs()) t.solidGeometry.clear();
    }
    return factor;
}

}  // namespace flatcam::gui
