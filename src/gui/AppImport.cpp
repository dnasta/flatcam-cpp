// Archivo -> Importar: SVG como Geometría o Gerber (import_svg, on_file_importsvg de app_Main.py).
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/GeometryObject.h"
#include "parsers/Gerber.h"
#include "parsers/DxfImport.h"
#include "parsers/Hpgl2.h"
#include "parsers/SvgImport.h"

namespace flatcam::gui {

FlatCAMObj* App::importSvg(const QString& filename, bool asGerber) {
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        ui_->inform("[ERROR_NOTCL]" + tr_("Import failed."));
        return nullptr;
    }
    const std::string text = f.readAll().toStdString();
    const SvgImportResult r = asGerber ? importSvgAsGerber(text) : importSvgAsGeometry(text);
    if (!r.ok) {
        ui_->inform("[ERROR_NOTCL]" + tr_("Import failed."));
        return nullptr;
    }
    const QString name = QFileInfo(filename).fileName();
    FlatCAMObj* obj = nullptr;
    if (asGerber) {
        // Gerber.import_svg: todo en la apertura "0" (REG, tamaño 0), con el contorno como línea de seguimiento.
        Aperture ap;
        ap.type = "REG";
        ap.size = 0.0;
        ap.hasGeometry = true;
        for (const Shape& p : r.geometry) {
            ApertureGeometry el;
            el.solid = p;
            el.follow = p.isEmpty() ? p : p.exterior();
            ap.geometry.push_back(el);
        }
        auto g = std::make_unique<Gerber>();
        g->restore(r.geometry, {}, {{"0", ap}}, 3, 4, text);
        obj = addObject(std::make_unique<GerberObject>(name, std::move(g)));
    } else {
        // Geometry.import_svg: una herramienta con geometry_cnctooldia (2,4) y toda la geometría.
        auto geo = std::make_unique<GeometryObject>(name);
        geo->solidGeometry() = r.geometry;
        GeometryTool t = defaultGeometryTool();
        t.tooldia = 2.4;
        t.solidGeometry = r.geometry;
        geo->tools()[1] = t;
        obj = addObject(std::move(geo));
    }
    lastFolder_ = QFileInfo(filename).absolutePath();
    registerRecent("svg", filename);
    ui_->inform(QString("[success] %1: %2").arg(tr_("Opened"), filename));
    // Desvío: los textos (tspan) se dibujan con fuentes TTF en el original (ParseFont); el port todavía no los importa.
    if (r.hadText) ui_->inform("[WARNING_NOTCL] El SVG tiene textos: el port todavía no los importa.");
    return obj;
}

void App::onFileImportSvg(bool asGerber) {
    const QStringList files = QFileDialog::getOpenFileNames(ui_.get(), tr_("Import SVG"), lastFolder(),
                                                            "SVG File .svg (*.svg);;All Files (*.*)");
    if (files.isEmpty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return;
    }
    for (const QString& f : files) importSvg(f, asGerber);
}

FlatCAMObj* App::importDxf(const QString& filename, bool asGerber) {
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        ui_->inform("[ERROR_NOTCL]" + tr_("Import failed."));
        return nullptr;
    }
    const std::string text = f.readAll().toStdString();
    const DxfImportResult r = asGerber ? importDxfAsGerber(text) : importDxfAsGeometry(text);
    if (!r.ok) {
        ui_->inform("[ERROR_NOTCL]" + tr_("Import failed."));
        return nullptr;
    }
    const QString name = QFileInfo(filename).fileName();
    FlatCAMObj* obj = nullptr;
    if (asGerber) {
        // import_dxf_as_gerber: la geometría sólida es la unión; cada figura va a la apertura "0" (solid = follow).
        Aperture ap;
        ap.type = "REG";
        ap.size = 0.0;
        ap.hasGeometry = true;
        for (const Shape& p : r.geometry) {
            ApertureGeometry el;
            el.solid = p;
            el.follow = p;
            ap.geometry.push_back(el);
        }
        const Shape u = Shape::unaryUnion(r.geometry);
        const ShapeList solid = u.parts().empty() ? ShapeList{u} : u.parts();
        auto g = std::make_unique<Gerber>();
        g->restore(solid, solid, {{"0", ap}}, 3, 4, text);
        obj = addObject(std::make_unique<GerberObject>(name, std::move(g)));
    } else {
        auto geo = std::make_unique<GeometryObject>(name);
        geo->solidGeometry() = r.geometry;
        GeometryTool t = defaultGeometryTool();
        t.tooldia = 2.4;  // geometry_cnctooldia
        t.solidGeometry = r.geometry;
        geo->tools()[1] = t;
        obj = addObject(std::move(geo));
    }
    lastFolder_ = QFileInfo(filename).absolutePath();
    registerRecent("dxf", filename);
    ui_->inform(QString("[success] %1: %2").arg(tr_("Opened"), filename));
    return obj;
}

void App::onFileImportDxf(bool asGerber) {
    const QStringList files = QFileDialog::getOpenFileNames(ui_.get(), tr_("Import DXF"), lastFolder(),
                                                            "DXF File .dxf (*.DXF);;All Files (*.*)");
    if (files.isEmpty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return;
    }
    for (const QString& f : files) importDxf(f, asGerber);
}

FlatCAMObj* App::openHpgl2(const QString& filename) {
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        ui_->inform("[ERROR_NOTCL] " + tr_("Failed to open file") + ": " + filename);
        return nullptr;
    }
    const std::string text = f.readAll().toStdString();
    const Hpgl2Result r = parseHpgl2(text);
    if (!r.ok) {
        if (!r.error.empty()) ui_->inform("[ERROR] " + tr_("HPGL2 Parser ERROR") + "\n" + QString::fromStdString(r.error));
        ui_->inform("[ERROR_NOTCL] " + tr_("Object is not HPGL2 file or empty. Aborting object creation."));
        ui_->inform("[ERROR_NOTCL]" + tr_("Failed. Probable not a HPGL2 file."));
        return nullptr;
    }
    // open_hpgl2: la unión como solid_geometry y una herramienta por pluma (geometry_cnctooldia, tipo "Iso").
    auto geo = std::make_unique<GeometryObject>(QFileInfo(filename).fileName());
    geo->solidGeometry() = r.solidGeometry;
    for (const auto& [pen, shapes] : r.tools) {
        GeometryTool t = defaultGeometryTool();
        t.tooldia = 2.4;
        t.type = "Iso";
        t.solidGeometry = shapes;
        geo->tools()[pen] = t;
    }
    FlatCAMObj* obj = addObject(std::move(geo));
    lastFolder_ = QFileInfo(filename).absolutePath();
    registerRecent("geometry", filename);
    return obj;
}

void App::onFileOpenHpgl2() {
    const QStringList files = QFileDialog::getOpenFileNames(ui_.get(), tr_("Open HPGL2"), lastFolder(),
                                                            "HPGL2 Files (*.plt);;All Files (*.*)");
    if (files.isEmpty()) {
        ui_->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        return;
    }
    for (const QString& f : files) openHpgl2(f);
}

}  // namespace flatcam::gui
