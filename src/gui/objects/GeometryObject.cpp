#include "gui/objects/GeometryObject.h"

#include "cam/Preprocessor.h"
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
    d.tolerance = 0.01 / 20;  // global_tolerance / 20, en mtool_gen_cncjob
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
    // plot() con multigeo: la geometría de cada herramienta, solo contornos. Los colores propios (puentes finos) se
    // dibujan con el color general: el canvas tiene un color de línea por objeto.
    for (const auto& [uid, t] : tools_)
        for (const Shape& s : t.solidGeometry) addShape(s, d, false);
    return d;
}

QRectF GeometryObject::bounds() const { return plotData().bounds; }

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
    if (toolIds.size() > 1) {
        // El núcleo genera un trabajo por herramienta; unir varias en un archivo queda pendiente.
        error = "[WARNING_NOTCL] Todavía se genera una herramienta por vez: elegí una sola en la tabla.";
        return nullptr;
    }
    const GeometryTool& t = tools_.at(toolIds.front());
    bool empty = true;
    for (const Shape& s : t.solidGeometry) empty = empty && s.isEmpty();
    if (empty) {
        error = "[ERROR_NOTCL] " + tr_("Cancelled. Empty file, it has no geometry") + "...";
        return nullptr;
    }
    if (!makePreprocessor(t.ppname.toStdString())) {
        error = QString("[ERROR_NOTCL] %1: %2").arg(tr_("Preprocessor"), t.ppname) +
                " (todavía no está portado; están default y GRBL_laser)";
        return nullptr;
    }
    GeometryJobSettings s;
    s.preprocessor = t.ppname.toStdString();
    s.order = t.optimization == "R" ? DrillOrder::RTree : DrillOrder::FileOrder;
    s.toolchange = true;  // mtool_gen_cncjob pasa toolchange=True a geometry_tool_gcode_gen
    s.tool = 1;
    s.toolSettings = t.data;
    s.toolSettings.toolDia = t.tooldia;
    s.toolSettings.offsetKind = t.offset.toLower() == "path" ? "Path" : t.offset.toLower().toStdString();
    s.toolSettings.offsetValue = t.offsetValue;
    // options['xmin'...]: los límites de la geometría del objeto.
    const QRectF b = bounds();
    s.headerBounds = BoundingBox{b.left(), b.top(), b.right(), b.bottom()};
    ShapeList lines;
    for (const Shape& sh : t.solidGeometry) {
        if (sh.isEmpty()) continue;
        const ShapeList parts = sh.parts();
        if (parts.empty()) lines.push_back(sh);
        else lines.insert(lines.end(), parts.begin(), parts.end());
    }
    auto job = std::make_unique<CncJob>();
    if (!job->generateFromGeometry(lines, s)) {
        error = "[ERROR_NOTCL] " + QString::fromStdString(job->lastError());
        return nullptr;
    }
    GcodeHeaderInfo h;
    h.version = "2024.4";
    h.versionDate = "2024/04/15";
    h.type = "Geometry";
    CncToolRow row;
    row.tool = toolIds.front();
    row.dia = t.tooldia;
    row.cutZ = t.data.cutZ;
    row.offset = t.offset;
    row.type = t.type;
    row.toolType = t.toolType;
    auto obj = std::make_unique<CNCJobObject>(name() + "_cnc", std::move(job), h, std::vector<CncToolRow>{row});
    return obj;
}

}  // namespace flatcam::gui
