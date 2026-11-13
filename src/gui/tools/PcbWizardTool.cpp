#include "gui/tools/PcbWizardTool.h"

#include <algorithm>
#include <QDateTime>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QLocale>

#include "cam/PyNumber.h"
#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "parsers/Excellon.h"

namespace flatcam::gui {

PcbWizardTool::PcbWizardTool(App* app, QWidget* parent) : PcbWizardToolUIBase(parent), app_(app) {
    setupUi();
    connect(excellon_brn, &QPushButton::clicked, this, [this] {
        const QString f = QFileDialog::getOpenFileName(this, tr_("Load PcbWizard Excellon file"), app_->lastFolder(),
                                                       "Excellon Files(*.DRL *.DRD *.TXT);;All Files (*.*)");
        if (f.isEmpty()) app_->ui()->inform(tr_("Cancelled."));
        else loadExcellon(f);
    });
    connect(inf_btn, &QPushButton::clicked, this, [this] {
        const QString f = QFileDialog::getOpenFileName(this, tr_("Load PcbWizard INF file"), app_->lastFolder(),
                                                       "INF Files(*.INF);;All Files (*.*)");
        if (f.isEmpty()) app_->ui()->inform(tr_("Cancelled."));
        else loadInf(f);
    });
    connect(import_button, &QPushButton::clicked, this, [this] { importExcellon(); });
    // WizardUI.on_units_change
    connect(units_radio, &RadioSet::activatedCustom, this, [this](const QString& v) {
        int_entry->setValue(v == "INCH" ? 2 : 3);
        frac_entry->setValue(v == "INCH" ? 4 : 3);
    });
}

ToolInfo PcbWizardTool::info() { return {tr_("PcbWizard Import Tool"), tr_("PCBWizard Tool")}; }

void PcbWizardTool::setToolUi() {
    outname_ = "file";
    excLines_.clear();
    inf_ = PcbWizardInf();
    int_entry->setValue(2);
    frac_entry->setValue(4);
    zeros_radio->setValue(QString("LZ"));
    units_radio->setValue(QString("INCH"));
    excellonLoaded_ = infLoaded_ = processFinished_ = false;
    merged_.clear();
    mergedRead_ = false;
    buildUi();
}

void PcbWizardTool::buildUi() {
    if (inf_.tools.empty()) tools_table->setVisible(false);
    std::vector<std::pair<int, double>> sorted = inf_.tools;
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    if (!sorted.empty()) tools_table->setRowCount(static_cast<int>(sorted.size()));
    int row = 0;
    for (const auto& [t, d] : sorted) {
        auto* id = new QTableWidgetItem(QString::number(t));
        id->setFlags(Qt::ItemIsEnabled);
        tools_table->setItem(row, 0, id);
        auto* dia = new QTableWidgetItem(QString::fromStdString(pyFloatRepr(d)));
        dia->setFlags(Qt::ItemIsEnabled);
        tools_table->setItem(row, 1, dia);
        ++row;
    }
    tools_table->resizeColumnsToContents();
    tools_table->resizeRowsToContents();
    tools_table->verticalHeader()->hide();
    tools_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tools_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tools_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tools_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tools_table->setSortingEnabled(false);
    tools_table->setMinimumHeight(tools_table->contentHeight());
    tools_table->setMaximumHeight(tools_table->contentHeight());
}

void PcbWizardTool::loadExcellon(const QString& filename) {
    excLines_ = pcbWizardReadLines(filename.toStdString());
    fileLoaded(false, filename);
}

bool PcbWizardTool::loadInf(const QString& filename) {
    const bool ok = pcbWizardReadInf(pcbWizardReadLines(filename.toStdString()), inf_);
    // El formato encontrado pasa al formulario (aunque después falte la tabla).
    if (inf_.units) units_radio->setValue(QString::fromStdString(*inf_.units));
    if (inf_.integral) int_entry->setValue(*inf_.integral);
    if (inf_.fractional) frac_entry->setValue(*inf_.fractional);
    if (!ok) {
        app_->ui()->inform(QString("[ERROR] %1").arg(
            tr_("The INF file does not contain the tool table.\nTry to open the Excellon file from File -> Open -> "
                "Excellon\nand edit the drill diameters manually.")));
        return false;
    }
    fileLoaded(true, filename);
    return true;
}

void PcbWizardTool::fileLoaded(bool inf, const QString& filename) {
    buildUi();
    // "{:%A, %d %B %Y at %H:%M}" en el idioma C, como Python.
    const QString timeStr = !timeOverride_.isEmpty()
                                ? timeOverride_
                                : QLocale::c().toString(QDateTime::currentDateTime(), "dddd, dd MMMM yyyy 'at' HH:mm");
    if (inf) {
        infLoaded_ = true;
        tools_table->setVisible(true);
        app_->ui()->inform(QString("[success] %1").arg(tr_("PcbWizard .INF file loaded.")));
    } else {
        excellonLoaded_ = true;
        outname_ = QFileInfo(filename).fileName();
        app_->ui()->inform(QString("[success] %1").arg(tr_("Main PcbWizard Excellon file loaded.")));
    }
    if (excellonLoaded_ && infLoaded_) {
        merged_ = pcbWizardMerge(excLines_, inf_, int_entry->value(), frac_entry->value(),
                                 units_radio->value().toString().toStdString(),
                                 zeros_radio->value().toString().toStdString(), Preferences::version(), "2024/04/15",
                                 timeStr.toStdString());
        mergedRead_ = false;
        processFinished_ = true;
    }
    app_->setLastFolder(QFileInfo(filename).absolutePath());
}

FlatCAMObj* PcbWizardTool::importExcellon() {
    if (merged_.empty() && !processFinished_) {
        // modified_excellon_file = '' (nada cargado todavía).
        app_->ui()->inform(QString("[ERROR_NOTCL] %1").arg(tr_("The imported Excellon file is empty.")));
        return nullptr;
    }
    if (!processFinished_) {
        app_->ui()->inform(QString("[WARNING_NOTCL] %1").arg(tr_("Excellon merging is in progress. Please wait...")));
        return nullptr;
    }
    // El original le pasa el mismo StringIO: la segunda vez se lee desde el final y queda vacío.
    std::vector<std::string> lines;
    if (!mergedRead_) {
        std::size_t a = 0;
        while (a < merged_.size()) {
            std::size_t b = merged_.find('\n', a);
            if (b == std::string::npos) b = merged_.size();
            lines.push_back(merged_.substr(a, b - a));
            a = b + 1;
        }
    }
    mergedRead_ = true;
    const QString name = outname_;
    auto exc = std::make_unique<Excellon>();
    bool hasGeometry = false;
    if (exc->parseLines(lines)) {
        exc->createGeometry();
        for (const auto& [id, t] : exc->drillTools()) hasGeometry = hasGeometry || !t.solidGeometry.empty();
    } else {
        app_->ui()->inform(QString("[ERROR_NOTCL] %1").arg(tr_("This is not Excellon file.")));
    }
    if (!hasGeometry) {
        app_->ui()->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("No geometry found in file"), name));
        app_->ui()->inform(QString("[ERROR_NOTCL] %1").arg(tr_("Import Excellon file failed.")));
        return nullptr;
    }
    if (exc->units() != appUnitsKind()) exc->convertUnits(appUnitsKind());
    FlatCAMObj* obj = app_->addObject(std::make_unique<ExcellonObject>(name, std::move(exc)));
    // file_opened.emit("excellon", name): registra el nombre, no la ruta.
    app_->registerRecent("excellon", name);
    app_->ui()->inform(QString("[success] %1: %2").arg(tr_("Imported"), name));
    app_->ui()->notebook->setCurrentWidget(app_->ui()->project_tab);
    return obj;
}

}  // namespace flatcam::gui
