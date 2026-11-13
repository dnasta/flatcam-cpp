// Archivo -> Importar: SVG como Geometría o Gerber (import_svg, on_file_importsvg de app_Main.py).
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QStringDecoder>
#include <QtEndian>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/GeometryObject.h"
#include "parsers/Gerber.h"
#include "parsers/DxfImport.h"
#include "parsers/Hpgl2.h"
#include "parsers/PdfImport.h"
#include "parsers/Excellon.h"
#include "gui/Preferences.h"
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

std::vector<FlatCAMObj*> App::openPdf(const QString& filename) {
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        ui_->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Failed to open"), filename));
        return {};
    }
    const QByteArray pdf = f.readAll();
    // re.findall(b'.*?FlateDecode.*?stream(.*?)endstream', pdf, re.S): cada stream después de un FlateDecode.
    std::string text;
    qsizetype pos = 0;
    while (true) {
        const qsizetype fd = pdf.indexOf("FlateDecode", pos);
        if (fd < 0) break;
        const qsizetype st = pdf.indexOf("stream", fd + 11);
        if (st < 0) break;
        const qsizetype end = pdf.indexOf("endstream", st + 6);
        if (end < 0) break;
        QByteArray s = pdf.mid(st + 6, end - st - 6);
        pos = end + 9;
        // s.strip(b'\r\n')
        while (!s.isEmpty() && (s.front() == '\r' || s.front() == '\n')) s.remove(0, 1);
        while (!s.isEmpty() && (s.back() == '\r' || s.back() == '\n')) s.chop(1);
        // zlib.decompress: qUncompress pide el tamaño delante (se le da uno grande; si no alcanza, lo agranda).
        QByteArray withSize(4, '\0');
        qToBigEndian<quint32>(static_cast<quint32>(std::max<qsizetype>(s.size() * 8, 1024)), withSize.data());
        withSize += s;
        const QByteArray raw = qUncompress(withSize);
        QStringDecoder utf8(QStringDecoder::Utf8);
        const QString decoded = utf8(raw);
        if (raw.isEmpty() || utf8.hasError()) {
            // un stream que no es zlib o no es UTF-8 (fuentes, imágenes): el original avisa y no abre nada
            ui_->inform(QString("[ERROR_NOTCL] %1: %2\n%3").arg(tr_("Failed to open"), filename,
                                                                   raw.isEmpty() ? "Error -3 while decompressing data"
                                                                                 : "'utf-8' codec can't decode"));
            return {};
        }
        text += decoded.toStdString() + "\r\n";
    }
    const auto layers = parsePdf(text, appUnits().toUpper().toStdString(), defInt("gerber_circle_steps", 64));
    lastFolder_ = QFileInfo(filename).absolutePath();
    ui_->inform(QString("[success] %1: %2").arg(tr_("Opened"), filename));
    // periodic_check_handler: las capas en el orden del diccionario (1, 2... y la 0 al final).
    std::vector<int> order;
    for (const auto& [k, aps] : layers)
        if (k != 0) order.push_back(k);
    if (layers.count(0)) order.push_back(0);
    const QString shortName = QFileInfo(filename).fileName();
    std::vector<FlatCAMObj*> out;
    for (int k : order) {
        const PdfApertures& aps = layers.at(k);
        const QString name = shortName + "_" + QString::number(k);
        if (k == 0) {
            // layer_rendering_as_excellon
            auto exc = std::make_unique<Excellon>();
            int id = 0;
            for (const auto& [dia, pts] : pdfLayerAsExcellon(aps)) exc->addDrillTool(++id, dia, pts);
            exc->createGeometry();
            bool any = false;
            for (const auto& [tid, t] : exc->drillTools()) any = any || !t.solidGeometry.empty();
            if (!any) {
                ui_->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("No geometry found in file"), name));
                ui_->inform("[ERROR_NOTCL] " + tr_("Open PDF file failed."));
                continue;
            }
            out.push_back(addObject(std::make_unique<ExcellonObject>(name, std::move(exc))));
            registerRecent("excellon", filename);
        } else {
            // layer_rendering_as_gerber
            const PdfGerberLayer g = pdfLayerAsGerber(aps);
            std::map<std::string, Aperture> apMap;
            for (const auto& [code, ap] : aps) apMap[code] = ap;
            auto grb = std::make_unique<Gerber>();
            grb->restore(g.solid, g.follow, apMap, 2, 4, std::string());
            out.push_back(addObject(std::make_unique<GerberObject>(name, std::move(grb))));
            registerRecent("gerber", filename);
        }
        ui_->inform(QString("[success] %1: %2").arg(tr_("Rendered"), name));
    }
    return out;
}

void App::onFileOpenPdf() {
    const QStringList files = QFileDialog::getOpenFileNames(ui_.get(), tr_("Open PDF"), lastFolder(),
                                                            "Adobe PDF Files (*.pdf);;All Files (*.*)");
    if (files.isEmpty()) {
        ui_->inform(QString("[WARNING_NOTCL] %1.").arg(tr_("Open PDF cancelled")));
        return;
    }
    for (const QString& f : files) openPdf(f);
}

}  // namespace flatcam::gui
