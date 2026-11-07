// Archivo -> Exportar -> Gerber / Excellon (on_file_exportgerber, on_file_exportexcellon, export_gerber y
// export_excellon de app_Main.py) y el texto fuente regenerado de un objeto movido (local_use).
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QLocale>

#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "io/FileExport.h"
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

}  // namespace flatcam::gui
