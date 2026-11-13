#include "gui/tools/SolderPasteTool.h"

#include <QTableWidgetItem>

#include "cam/PyNumber.h"
#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/GeometryObject.h"
#include "gui/objects/ObjectCollection.h"
#include "tools/SolderPaste.h"

namespace flatcam::gui {

namespace {

PyNumber pyNum(double v, bool isInt) { return isInt ? PyNumber(static_cast<int>(v)) : PyNumber(v); }

std::optional<std::array<PyNumber, 2>> pair(const QString& s) {
    const QStringList p = s.split(',');
    if (p.size() != 2) return std::nullopt;
    return std::array<PyNumber, 2>{PyNumber(p[0].trimmed().toDouble()), PyNumber(p[1].trimmed().toDouble())};
}

}  // namespace

SolderPasteTool::SolderPasteTool(App* app, QWidget* parent) : SolderPasteToolUIBase(parent), app_(app) {
    setupUi();
    connect(addtool_btn, &QPushButton::clicked, this, [this] { addTool(); });
    connect(deltool_btn, &QPushButton::clicked, this, [this] {
        for (QTableWidgetItem* it : tools_table->selectedItems())
            if (it->column() == 2) tools_.erase(it->text().toInt());
        buildUi();
    });
    connect(soldergeo_btn, &QPushButton::clicked, this, [this] { createGeometry(); });
    connect(solder_gcode_btn, &QPushButton::clicked, this, [this] { createGcode(); });
    connect(solder_gcode_view_btn, &QPushButton::clicked, this, [this] {
        FlatCAMObj* o = app_->collection()->byName(cnc_obj_combo->currentText());
        if (!o) {
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("There is no CNCJob object."));
            return;
        }
        app_->viewSource(o);
    });
    connect(solder_gcode_save_btn, &QPushButton::clicked, this, [this] {
        if (auto* o = dynamic_cast<CNCJobObject*>(app_->collection()->byName(cnc_obj_combo->currentText())))
            app_->exportGcode(o);
        else app_->ui()->inform("[WARNING_NOTCL] " + tr_("There is no CNCJob object."));
    });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
}

ToolInfo SolderPasteTool::info() { return {tr_("Solder Paste Tool"), tr_("Solder Paste Tool")}; }

void SolderPasteTool::fillCombos() {
    auto fill = [this](FCComboBox* c, ObjectKind k) {
        const QString cur = c->currentText();
        c->clear();
        for (FlatCAMObj* o : app_->collection()->objects())
            if (o->kind() == k) c->addItem(o->name());
        if (!cur.isEmpty() && c->findText(cur) >= 0) c->setValue(cur);
    };
    fill(obj_combo, ObjectKind::Gerber);
    fill(geo_obj_combo, ObjectKind::Geometry);
    fill(cnc_obj_combo, ObjectKind::CNCJob);
}

void SolderPasteTool::setToolUi() {
    tools_.clear();
    int uid = 0;
    for (const QString& d : defStr("tools_solderpaste_tools").split(',', Qt::SkipEmptyParts))
        tools_[++uid] = QString::number(d.trimmed().toDouble(), 'f', appDecimals()).toDouble();
    addtool_entry->setValue(defNum("tools_solderpaste_new"));
    z_start_entry->setValue(defNum("tools_solderpaste_z_start"));
    z_dispense_entry->setValue(defNum("tools_solderpaste_z_dispense"));
    z_stop_entry->setValue(defNum("tools_solderpaste_z_stop"));
    z_travel_entry->setValue(defNum("tools_solderpaste_z_travel"));
    z_toolchange_entry->setValue(defNum("tools_solderpaste_z_toolchange"));
    xy_toolchange_entry->setValue(defStr("tools_solderpaste_xy_toolchange"));
    frxy_entry->setValue(defNum("tools_solderpaste_frxy"));
    frz_entry->setValue(defNum("tools_solderpaste_frz"));
    frz_dispense_entry->setValue(defNum("tools_solderpaste_frz_dispense"));
    speedfwd_entry->setValue(defInt("tools_solderpaste_speedfwd", 300));
    dwellfwd_entry->setValue(defNum("tools_solderpaste_dwellfwd"));
    speedrev_entry->setValue(defInt("tools_solderpaste_speedrev", 200));
    dwellrev_entry->setValue(defNum("tools_solderpaste_dwellrev"));
    pp_combo->clear();
    pp_combo->addItem("Paste_1");
    pp_combo->setValue(defStr("tools_solderpaste_pp"));
    fillCombos();
    buildUi();
}

void SolderPasteTool::buildUi() {
    // build_ui: las boquillas ordenadas por diámetro; la columna 2 (oculta) es el número.
    std::vector<std::pair<double, int>> sorted;
    for (const auto& [uid, d] : tools_) sorted.push_back({d, uid});
    std::sort(sorted.begin(), sorted.end());
    tools_table->setRowCount(static_cast<int>(sorted.size()));
    int row = 0;
    for (const auto& [d, uid] : sorted) {
        tools_table->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
        tools_table->setItem(row, 1, new QTableWidgetItem(QString::number(d, 'f', appDecimals())));
        tools_table->setItem(row, 2, new QTableWidgetItem(QString::number(uid)));
        ++row;
    }
}

void SolderPasteTool::addTool(double dia) {
    const double d = QString::number(dia > 0 ? dia : addtool_entry->value(), 'f', appDecimals()).toDouble();
    for (const auto& [uid, v] : tools_)
        if (v == d) {
            app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled. Tool already in Tool Table."));
            return;
        }
    const int uid = tools_.empty() ? 1 : tools_.rbegin()->first + 1;
    tools_[uid] = d;
    buildUi();
    app_->ui()->inform("[success] " + tr_("New Nozzle tool added to Tool Table."));
}

FlatCAMObj* SolderPasteTool::createGeometry() {
    auto* g = dynamic_cast<GerberObject*>(app_->collection()->byName(obj_combo->currentText()));
    if (!g) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("There is no Gerber object loaded ..."));
        return nullptr;
    }
    std::vector<std::pair<int, double>> nz(tools_.begin(), tools_.end());
    const PasteGeometry r = solderPasteGeometry(g->gerber().solidGeometry(), nz, appDecimals(),
                                                defStr("units").toUpper() == "MM");
    if (!r.error.empty()) {
        app_->ui()->inform((r.error.rfind("No Nozzle", 0) == 0 || r.error.rfind("Some", 0) == 0 ? "[WARNING_NOTCL] "
                                                                                                 : "[ERROR_NOTCL] ") +
                           tr_(r.error.c_str()));
        return nullptr;
    }
    auto geo = std::make_unique<GeometryObject>(g->name() + "_solderpaste");
    for (const PasteTool& t : r.tools) {
        GeometryTool gt = defaultGeometryTool();
        gt.tooldia = t.dia;
        gt.offset = "Path";
        gt.offsetValue = 0.0;
        gt.type = "SolderPaste";
        gt.toolType = "DN";
        gt.solidGeometry = t.paths;
        geo->tools()[t.uid] = gt;
        geo->solidGeometry().insert(geo->solidGeometry().end(), t.paths.begin(), t.paths.end());
    }
    FlatCAMObj* o = app_->addObject(std::move(geo));
    pasteGeometries_[o] = true;
    fillCombos();
    geo_obj_combo->setValue(o->name());
    app_->ui()->inform("[success] " + tr_("Solder Paste geometry generated successfully"));
    return o;
}

FlatCAMObj* SolderPasteTool::createGcode() {
    const QString name = geo_obj_combo->currentText();
    auto* geo = dynamic_cast<GeometryObject*>(app_->collection()->byName(name));
    if (name.isEmpty() || !geo) return nullptr;
    if (!pasteGeometries_.count(geo)) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("This Geometry can't be processed. NOT a solder_paste_tool geometry."));
        return nullptr;
    }
    // originar_name = name.partition('_')[0]
    const QString outname = name.section('_', 0, 0) + "_cnc_solderpaste";
    const QRectF b = geo->bounds();
    std::vector<std::string> gcodes;
    std::vector<CncToolRow> rows;
    for (const auto& [uid, t] : geo->tools()) {
        PreprocessorParams p;
        p.units = defStr("units").toUpper().toStdString();
        p.coordsDecimals = defInt("cncjob_coords_decimals", 4);
        p.frDecimals = defInt("cncjob_fr_decimals", 2);
        p.decimals = appDecimals();
        p.tool = uid;
        p.toolC = PyNumber(t.tooldia);
        p.dwellTime = PyNumber(1000);  // CNCjob(dwelltime=1000)
        p.options.xmin = PyNumber(b.left());
        p.options.ymin = PyNumber(b.top());
        p.options.xmax = PyNumber(b.right());
        p.options.ymax = PyNumber(b.bottom());
        p.options.toolDia = PyNumber(t.tooldia);
        p.zStart = PyNumber(z_start_entry->value());
        p.zDispense = PyNumber(z_dispense_entry->value());
        p.zStop = PyNumber(z_stop_entry->value());
        p.zTravel = PyNumber(z_travel_entry->value());
        p.zToolchange = PyNumber(z_toolchange_entry->value());
        p.xyToolchange = pair(xy_toolchange_entry->text());
        p.xyEnd = pair(defStr("cncjob_xy_end").isEmpty() ? QString("0.0, 0.0") : defStr("cncjob_xy_end"));
        auto num = [](double v) { return pyNum(v, v == static_cast<int>(v)); };
        p.frXY = num(frxy_entry->value());
        p.frZ = num(frz_entry->value());
        p.frZDispense = PyNumber(frz_dispense_entry->value());
        p.speedFwd = PyNumber(speedfwd_entry->value());
        p.dwellFwd = num(dwellfwd_entry->value());
        p.speedRev = PyNumber(speedrev_entry->value());
        p.dwellRev = num(dwellrev_entry->value());
        gcodes.push_back(solderPasteGcode(t.solidGeometry, p, defStr("cncjob_coords_type") != "G91"));
        CncToolRow row;
        row.tool = uid;
        row.dia = t.tooldia;
        row.type = "SolderPaste";
        row.toolType = "DN";
        rows.push_back(row);
    }
    auto job = std::make_unique<CncJob>();
    job->restore(std::string(), gcodes);
    GcodeHeaderInfo h;
    h.version = "2024.4";
    h.versionDate = "2024/04/15";
    h.type = "Geometry";
    h.units = defStr("units").toUpper().toStdString();
    h.name = outname.toStdString();
    FlatCAMObj* o = app_->addObject(std::make_unique<CNCJobObject>(outname, std::move(job), h, rows));
    fillCombos();
    cnc_obj_combo->setValue(o->name());
    app_->ui()->inform(QString("[success] %1: %2").arg(tr_("ToolSolderPaste CNCjob created"), outname));
    return o;
}

}  // namespace flatcam::gui
