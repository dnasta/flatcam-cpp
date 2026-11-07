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
#include "gui/MainGUI.h"
#include "gui/PlotCanvas.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/GeometryObject.h"
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

bool App::exportSvgFile(FlatCAMObj* obj, const QString& filename) {
    const std::string text = exportSvg(obj->solidShapes(), "mm");
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
    if (obj->kind() == ObjectKind::CNCJob) {
        // Desvío: el SVG de un CNC Job sale de su G-code interpretado (gcode_parsed), que el port no guarda todavía.
        ui_->inform("[WARNING_NOTCL] Exportar un CNC Job a SVG todavía no está en el port.");
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

}  // namespace flatcam::gui
