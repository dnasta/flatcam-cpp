// Archivo -> Exportar -> Gerber / Excellon (on_file_exportgerber, on_file_exportexcellon, export_gerber y
// export_excellon de app_Main.py) y el texto fuente regenerado de un objeto movido (local_use).
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QLocale>
#include <QImage>
#include <QMessageBox>

#include "gui/App.h"

#include "gui/ProjectIO.h"
#include "gui/MainGUI.h"
#include "gui/PlotCanvas.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/GeometryObject.h"
#include "gui/Preferences.h"
#include "gui/tools/FilmTool.h"
#include "io/DxfExport.h"
#include "io/FileExport.h"
#include "io/SvgExport.h"
#include "parsers/Excellon.h"
#include "parsers/Gerber.h"

namespace flatcam::gui {

namespace {

ExportHeader headerFor(const QString& name) {
    ExportHeader h;
    h.name = name.toStdString();
    // "{:%A, %d %B %Y at %H:%M}" con los nombres en inglés, como Python.
    h.createdOn = QLocale::c().toString(QDateTime::currentDateTime(), "dddd, dd MMMM yyyy 'at' HH:mm").toStdString();
    return h;
}

GerberExportFormat gerberFormat(const JsonValue& o) {
    GerberExportFormat f;
    if (o["gerber_exp_units"].isString()) f.units = o["gerber_exp_units"].text();
    f.whole = static_cast<int>(o["gerber_exp_integer"].toDouble(f.whole));
    f.fract = static_cast<int>(o["gerber_exp_decimals"].toDouble(f.fract));
    if (o["gerber_exp_zeros"].isString()) f.zeros = o["gerber_exp_zeros"].text();
    return f;
}

ExcellonExportFormat excellonFormat(const JsonValue& o) {
    ExcellonExportFormat f;
    if (o["excellon_exp_units"].isString()) f.units = o["excellon_exp_units"].text();
    f.whole = static_cast<int>(o["excellon_exp_integer"].toDouble(f.whole));
    f.fract = static_cast<int>(o["excellon_exp_decimals"].toDouble(f.fract));
    if (o["excellon_exp_zeros"].isString()) f.zeros = o["excellon_exp_zeros"].text();
    if (o["excellon_exp_format"].isString()) f.format = o["excellon_exp_format"].text();
    if (o["excellon_exp_slot_type"].isString()) f.slotType = o["excellon_exp_slot_type"].text();
    return f;
}

}  // namespace

QString App::exportedText(FlatCAMObj* obj) const {
    if (auto* g = dynamic_cast<GerberObject*>(obj))
        return QString::fromStdString(exportGerber(g->gerber(), gerberFormat(appOptions()), headerFor(obj->name())));
    if (auto* e = dynamic_cast<ExcellonObject*>(obj))
        return QString::fromStdString(exportExcellon(e->excellon(), excellonFormat(appOptions()), headerFor(obj->name())));
    return {};
}

void App::refreshSourceFile(FlatCAMObj* obj) {
    // export_gerber / export_excellon con local_use: el texto fuente pasa a ser el exportado.
    const std::string text = exportedText(obj).toStdString();
    if (text.empty()) return;
    if (auto* g = dynamic_cast<GerberObject*>(obj)) g->gerber().setSourceFile(text);
    else if (auto* e = dynamic_cast<ExcellonObject*>(obj)) e->excellon().setSourceFile(text);
}

bool App::exportObject(FlatCAMObj* obj, const QString& filename) {
    const QString text = exportedText(obj);
    if (text.isEmpty()) {
        ui_->inform("[ERROR_NOTCL] " + tr_("Could not export."));
        return false;
    }
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        ui_->inform("[WARNING] " + tr_("Permission denied, saving not possible.\n"
                                       "Most likely another app is holding the file open and not accessible."));
        return false;
    }
    f.write(text.toUtf8());
    const bool gerber = obj->kind() == ObjectKind::Gerber;
    registerRecent(gerber ? "gerber" : "excellon", filename);  // global_open_style = False: file_opened
    ui_->inform(QString("[success] %1: %2").arg(gerber ? tr_("Gerber file exported to") : tr_("Excellon file exported to"),
                                                filename));
    return true;
}

void App::onFileExport(ObjectKind kind) {
    FlatCAMObj* obj = activeObject();
    if (!obj) {
        ui_->inform("[WARNING_NOTCL] " + tr_("No object is selected."));
        return;
    }
    const bool gerber = kind == ObjectKind::Gerber;
    if (obj->kind() != kind) {
        ui_->inform("[ERROR_NOTCL] " + (gerber ? tr_("Failed. Only Gerber objects can be saved as Gerber files...")
                                               : tr_("Failed. Only Excellon objects can be saved as Excellon files...")));
        return;
    }
    const QString filter = QString::fromStdString(
        appOptions()[gerber ? "gerber_save_filters" : "excellon_save_filters"].text());
    const QString f = QFileDialog::getSaveFileName(ui_.get(), gerber ? tr_("Export Gerber") : tr_("Export Excellon"),
                                                   lastFolder() + "/" + obj->name(), filter);
    if (f.isEmpty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return;
    }
    exportObject(obj, f);
}

std::string App::cncSvgText(const CNCJobObject* job) const {
    // export_svg usa self.gcode_parsed: el del último G-code generado (en un G-code abierto, el de todo el archivo).
    const auto& tools = job->tools();
    const auto& gcodes = job->job().toolGcodes();
    const bool drilling = job->isDrilling();
    const std::string gcode = gcodes.empty() ? job->job().gcode() : gcodes.back();
    const CncToolRow* last = tools.empty() ? nullptr : &tools.back();
    const bool laser =
        last && last->data[drilling ? "tools_drill_ppname_e" : "ppname_g"].text().find("laser") != std::string::npos;
    const JsonValue parsed = ProjectIO::gcodeParse(gcode, laser, drilling && last ? last->dia : 0.0);
    ShapeList travels, cuts, all;
    for (const JsonValue& seg : parsed.items()) {
        const Shape g = Shape::fromWkt(seg["geom"]["__inst__"].text());
        all.push_back(g);
        const std::string k = seg["kind"].items().empty() ? std::string() : seg["kind"].items()[0].text();
        if (k == "C") cuts.push_back(g);
        if (k == "T") travels.push_back(g);
    }
    // bounds(): un trabajo de una herramienta (G-code abierto) usa solid_geometry = la unión de todo lo interpretado; uno
    // multiherramienta, los cortes de su última herramienta (CNCjob.bounds reinicia los mínimos en cada una).
    const ShapeList& forBounds = tools.empty() ? all : cuts;
    if (forBounds.empty()) return {};
    const BoundingBox b = Shape::unaryUnion(forBounds).bounds().value_or(BoundingBox{});
    const double tooldia = tools.empty() ? defNum("cncjob_tooldia", 1.0) : last->dia;
    return exportCncSvg(travels, cuts, b, job->header().units == "IN" ? "in" : "mm", tooldia / 2);
}

bool App::exportSvgFile(FlatCAMObj* obj, const QString& filename) {
    const auto* job = dynamic_cast<const CNCJobObject*>(obj);
    const std::string text = job ? cncSvgText(job) : exportSvg(obj->solidShapes(), "mm");
    if (text.empty()) {
        ui_->inform("[ERROR_NOTCL] " + tr_("Could not export."));
        return false;
    }
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        ui_->inform("[WARNING] " + tr_("Permission denied, saving not possible.\n"
                                       "Most likely another app is holding the file open and not accessible."));
        return false;
    }
    f.write(QByteArray::fromStdString(text));
    ui_->inform(QString("[success] %1: %2").arg(tr_("SVG file exported to"), filename));
    return true;
}

void App::onFileExportSvg() {
    FlatCAMObj* obj = activeObject();
    if (!obj) {
        ui_->inform("[WARNING_NOTCL] " + tr_("No object is selected."));
        return;
    }
    const QString f = QFileDialog::getSaveFileName(ui_.get(), tr_("Export SVG"), lastFolder() + "/" + obj->name() + "_svg",
                                                   "SVG File (*.svg);;All Files (*.*)");
    if (f.isEmpty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return;
    }
    exportSvgFile(obj, f);
}

bool App::exportDxfFile(GeometryObject* geo, const QString& filename) {
    // export_dxf (multigeo): la geometría de cada herramienta, en orden.
    ShapeList shapes;
    for (const auto& [uid, t] : geo->tools()) shapes.insert(shapes.end(), t.solidGeometry.begin(), t.solidGeometry.end());
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        ui_->inform("[WARNING] " + tr_("Permission denied, saving not possible.\n"
                                       "Most likely another app is holding the file open and not accessible."));
        return false;
    }
    f.write(QByteArray::fromStdString(exportDxf(shapes)));
    registerRecent("dxf", filename);
    ui_->inform(QString("[success] %1: %2").arg(tr_("DXF file exported to"), filename));
    return true;
}

void App::onFileExportDxf() {
    FlatCAMObj* obj = activeObject();
    if (!obj) {
        ui_->inform("[WARNING_NOTCL] " + tr_("No object is selected."));
        return;
    }
    auto* geo = dynamic_cast<GeometryObject*>(obj);
    if (!geo) {
        // Como el original: un cuadro de aviso con el mensaje como texto informativo.
        QMessageBox box(ui_.get());
        box.setIcon(QMessageBox::Warning);
        box.setInformativeText("[ERROR_NOTCL] " + tr_("Only Geometry objects can be used."));
        box.setDefaultButton(box.addButton(tr_("Ok"), QMessageBox::AcceptRole));
        box.exec();
        return;
    }
    const QString f = QFileDialog::getSaveFileName(ui_.get(), tr_("Export DXF"), lastFolder() + "/" + obj->name(),
                                                   "DXF File .dxf (*.DXF);;All Files (*.*)");
    if (f.isEmpty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return;
    }
    exportDxfFile(geo, f);
}

bool App::exportPngFile(const QString& filename) {
    // _screenshot(alpha=False) + write_png: lo que muestra el canvas, sin transparencia.
    const QImage img = ui_->plotCanvas()->grabFramebuffer().convertToFormat(QImage::Format_RGB888);
    if (img.isNull() || !img.save(filename, "PNG")) return false;
    registerRecent("png", filename);  // global_open_style = False: file_opened
    return true;
}

void App::onFileExportPng() {
    // Nombre propuesto: png_AAAAMMDD_HHMMSS.
    const QString date = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    const QString f = QFileDialog::getSaveFileName(ui_.get(), tr_("Export PNG Image"), lastFolder() + "/png_" + date,
                                                   "PNG File (*.png);;All Files (*.*)");
    if (f.isEmpty()) {
        ui_->inform(tr_("Cancelled."));
        return;
    }
    exportPngFile(f);
}

bool App::printPdf(const QString& filename, const std::vector<FlatCAMObj*>& objs, std::string* svgOut) {
    if (objs.empty()) return false;
    std::vector<PrintSvgItem> items;
    std::string color = "black", opacity = "1.0";
    BoundingBox b{1e300, 1e300, -1e300, -1e300};
    auto alphaHex = [](const QColor& c) { return QString("%1").arg(c.alpha(), 2, 16, QChar('0')).toUpper().toStdString(); };
    for (FlatCAMObj* o : objs) {
        ShapeList shapes;
        if (auto* g = dynamic_cast<GerberObject*>(o)) {
            shapes = g->gerber().solidGeometry();
        } else if (auto* e = dynamic_cast<ExcellonObject*>(o)) {
            shapes = e->excellon().solidGeometry();
        } else if (auto* geo = dynamic_cast<GeometryObject*>(o)) {
            for (const auto& [id, t] : geo->tools()) shapes.insert(shapes.end(), t.solidGeometry.begin(), t.solidGeometry.end());
        } else {
            // CNCJob.export_svg no acepta los factores de escala: TypeError, no se escribe nada.
            ui_->inform("[ERROR_NOTCL] " + tr_("Failed."));
            return false;
        }
        if (o->kind() == ObjectKind::Geometry) {
            color = defStr("global_draw_color").toStdString();
        } else {
            color = o->fillColor.name(QColor::HexRgb).toUpper().toStdString();  // fill_color[:-2]
            opacity = alphaHex(o->fillColor);                                    // fill_color[-2:]
        }
        items.push_back({shapes, color, opacity});
        const QRectF r = o->bounds();
        b = {std::min(b.xmin, r.left()), std::min(b.ymin, r.top()), std::max(b.xmax, r.right()), std::max(b.ymax, r.bottom())};
    }
    const std::string uom = appUnits().toLower() == "in" ? "in" : "mm";
    const std::string svg = exportPrintSvg(items, b, uom);
    if (svgOut) *svgOut = svg;
    const double boundary = uom == "mm" ? 1.0 : 0.0393701;
    const double toPt = uom == "in" ? 72.0 : 72.0 / 25.4;
    const QSizeF drawing((b.xmax - b.xmin + 2 * boundary) * toPt, (b.ymax - b.ymin + 2 * boundary) * toPt);
    const double unit = appUnits().toUpper() == "IN" ? 72.0 : 72.0 / 25.4;
    if (!renderSvgToPdf(QByteArray::fromStdString(svg), filename, drawing, defStr("global_workspaceT"),
                        defStr("global_workspace_orientation") == "p", {b.xmin * unit, b.ymin * unit}))
        return false;
    ui_->inform(QString("[success] %1: %2").arg(tr_("PDF file saved to"), filename));
    return true;
}

void App::onFilePrintPdf() {
    const std::vector<FlatCAMObj*> sel = selectedObjects();
    if (sel.empty()) {
        ui_->inform("[ERROR_NOTCL] " + tr_("No object is selected."));
        return;
    }
    // date = str(datetime.today()) sin los microsegundos, sin ':' ni '-' y con '_' en el espacio
    const QString date = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    const QString objName = sel.size() == 1 ? sel.front()->name() : tr_("FlatCAM objects print");
    const QString f = QFileDialog::getSaveFileName(ui_.get(), tr_("Save Object as PDF ..."),
                                                   lastFolder() + "/" + objName + "_" + date,
                                                   "PDF File .pdf (*.PDF);; All Files (*.*)");
    if (f.isEmpty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return;
    }
    printPdf(f, sel);
    lastFolder_ = QFileInfo(f).absolutePath();
}

}  // namespace flatcam::gui
