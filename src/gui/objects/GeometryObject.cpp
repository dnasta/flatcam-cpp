#include "gui/objects/GeometryObject.h"

#include "cam/Preprocessor.h"
#include "core/AffineTransform.h"
#include "gui/Translation.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/ObjectUIs.h"

namespace flatcam::gui {

GeometryTool defaultGeometryTool() {
    // geometry_* de defaults.py (los que usa geometry_tool_gcode_gen).
    GeometryTool t;
    t.tooldia = 2.4;  // geometry_cnctooldia
    GeometryToolSettings& d = t.data;
    d.cutZ = -2.4;
    d.multidepth = false;
    d.depthPerPass = 0.8;
    d.travelZ = 2.0;
    d.feedrate = 120.0;
    d.feedrateZ = 60.0;
    d.feedrateRapid = 1500.0;
    d.spindleSpeed = 0.0;
    d.spindleDir = "CW";
    d.dwell = false;
    d.dwellTime = 1.0;
    d.endZ = 15.0;
    d.toolchangeZ = 15.0;
    d.toolchangeXY = std::array<double, 2>{0.0, 0.0};  // "0.0, 0.0"
    d.extracut = false;
    d.extracutLength = 0.1;
    d.tolerance = 0.005 / 20;  // global_tolerance (0,005) / 20 = 0,00025, en mtool_gen_cncjob
    return t;
}

GeometryObject::GeometryObject(QString name) : FlatCAMObj(ObjectKind::Geometry, std::move(name)) {
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

QWidget* GeometryObject::propertiesUi() {
    if (!ui_) ui_ = std::make_unique<GeometryObjectUI>();
    static_cast<GeometryObjectUI*>(ui_.get())->build(*this);
    return ui_.get();
}

std::unique_ptr<CNCJobObject> GeometryObject::makeCncJob(const std::vector<int>& toolIds, QString& error) const {
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
            error = QString("[ERROR_NOTCL] %1: %2").arg(tr_("Preprocessor"), t.ppname) +
                    " (todavía no está portado; están default y GRBL_laser)";
            return nullptr;
        }
        GeometryToolJob j;
        GeometryJobSettings& s = j.settings;
        s.preprocessor = t.ppname.toStdString();
        s.order = t.optimization == "R" ? DrillOrder::RTree : DrillOrder::FileOrder;
        s.toolchange = true;  // mtool_gen_cncjob pasa toolchange=True a geometry_tool_gcode_gen
        s.tool = id;          // self.tool = str(tool): el identificador de la herramienta (la 9999 imprime T9999)
        s.toolSettings = t.data;
        s.toolSettings.toolDia = t.tooldia;
        s.toolSettings.offsetKind = t.offset.toLower() == "path" ? "Path" : t.offset.toLower().toStdString();
        s.toolSettings.offsetValue = t.offsetValue;
        s.headerBounds = BoundingBox{b.left(), b.top(), b.right(), b.bottom()};
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
    auto obj = std::make_unique<CNCJobObject>(name() + "_cnc", std::move(job), h, std::move(rows));
    return obj;
}

}  // namespace flatcam::gui
