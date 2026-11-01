#include "gui/objects/ObjectUIs.h"

#include <QHeaderView>
#include <algorithm>

#include "gui/Translation.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/widgets/FCCheckBox.h"

namespace flatcam::gui {

namespace {

constexpr int kDecimals = 4;  // App.decimals

QTableWidgetItem* item(const QString& text, Qt::ItemFlags flags) {
    auto* it = new QTableWidgetItem(text);
    it->setFlags(flags);
    return it;
}

QString fmt(double v) { return QString::number(v, 'f', kDecimals); }

}  // namespace

GerberObjectUI::GerberObjectUI(QWidget* parent) : GerberObjectUIBase(parent) {
    setupUi();
    connect(aperture_table_visibility_cb, &QCheckBox::toggled, this, [this](bool on) {
        // on_aperture_table_visibility_change: muestra la tabla (y la arma la primera vez).
        apertures_table->setVisible(on);
        if (on && obj_) buildAperturesTable(*obj_);
    });
}

void GerberObjectUI::build(GerberObject& obj) {
    obj_ = &obj;
    name_entry->setText(obj.name());
    plot_cb->setChecked(obj.plotEnabled());
    apertures_table->setVisible(aperture_table_visibility_cb->isChecked());
    if (aperture_table_visibility_cb->isChecked()) buildAperturesTable(obj);
}

void GerberObjectUI::buildAperturesTable(GerberObject& obj) {
    // GerberObject.build_ui: aperturas ordenadas por código numérico.
    std::vector<std::pair<int, const Aperture*>> sorted;
    for (const auto& [code, ap] : obj.gerber().apertures()) sorted.push_back({std::stoi(code), &ap});
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    apertures_table->setRowCount(static_cast<int>(sorted.size()));
    const auto sel = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    int row = 0;
    for (const auto& [code, ap] : sorted) {
        QString dim;
        if (ap->type == "R" || ap->type == "O") dim = fmt(ap->width) + ", " + fmt(ap->height);
        else if (ap->type == "P") dim = fmt(ap->diam) + ", " + fmt(ap->nVertices);
        apertures_table->setItem(row, 0, item(QString::number(row + 1), sel));
        apertures_table->setItem(row, 1, item(QString::number(code), Qt::ItemIsEnabled));
        apertures_table->setItem(row, 2, item(QString::fromStdString(ap->type), Qt::ItemIsEnabled));
        apertures_table->setItem(row, 3, item(ap->size ? fmt(*ap->size) : QString(), Qt::ItemIsEnabled));
        apertures_table->setItem(row, 4, item(dim, Qt::ItemIsEnabled));
        apertures_table->setItem(row, 5, item("", Qt::ItemIsEnabled));
        auto* mark = new FCCheckBox();
        mark->setLayoutDirection(Qt::RightToLeft);
        apertures_table->setCellWidget(row, 5, mark);
        ++row;
    }
    apertures_table->selectColumn(0);
    apertures_table->resizeColumnsToContents();
    apertures_table->resizeRowsToContents();
    apertures_table->verticalHeader()->hide();
    apertures_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QHeaderView* h = apertures_table->horizontalHeader();
    h->setMinimumSectionSize(10);
    h->setDefaultSectionSize(70);
    h->setSectionResizeMode(0, QHeaderView::Fixed);
    h->resizeSection(0, 27);
    for (int c : {1, 2, 3}) h->setSectionResizeMode(c, QHeaderView::ResizeToContents);
    h->setSectionResizeMode(4, QHeaderView::Stretch);
    h->setSectionResizeMode(5, QHeaderView::Fixed);
    h->resizeSection(5, 17);
    apertures_table->setColumnWidth(5, 17);
    apertures_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    apertures_table->setSortingEnabled(false);
    apertures_table->setMinimumHeight(apertures_table->contentHeight());
    apertures_table->setMaximumHeight(apertures_table->contentHeight());
}

ExcellonObjectUI::ExcellonObjectUI(QWidget* parent) : ExcellonObjectUIBase(parent) { setupUi(); }

void ExcellonObjectUI::build(ExcellonObject& obj) {
    name_entry->setText(obj.name());
    plot_cb->setChecked(obj.plotEnabled());
    const auto& tools = obj.excellon().drillTools();
    std::vector<std::pair<int, double>> sorted;
    for (const auto& [id, t] : tools) sorted.push_back({id, t.tooldia});
    std::stable_sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
    tools_table->setRowCount(static_cast<int>(sorted.size()) + 2);
    const auto sel = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    int row = 0, totDrills = 0, totSlots = 0;
    bool hasDrills = false, hasSlots = false;
    const QColor fill = colorFromFlatCam("#C40000BF");  // excellon_plot_fill
    for (const auto& [id, dia] : sorted) {
        const ExcellonTool& t = tools.at(id);
        const int drills = static_cast<int>(t.drills.size()), slots = static_cast<int>(t.slots.size());
        totDrills += drills;
        totSlots += slots;
        hasDrills = hasDrills || drills > 0;
        hasSlots = hasSlots || slots > 0;
        tools_table->setItem(row, 0, item(QString::number(id), sel));
        tools_table->setItem(row, 1, item(fmt(dia), sel));
        tools_table->setItem(row, 2, item(QString::number(drills), Qt::ItemIsEnabled));
        tools_table->setItem(row, 3, item(slots > 0 ? QString::number(slots) : QString(), Qt::ItemIsEnabled));
        auto* color = item("", Qt::NoItemFlags);
        color->setBackground(fill);
        tools_table->setItem(row, 4, color);
        auto* plot = new FCCheckBox();
        plot->setLayoutDirection(Qt::RightToLeft);
        plot->setChecked(plot_cb->isChecked());
        tools_table->setCellWidget(row, 5, plot);
        ++row;
    }
    // Filas de totales, en negrita y de color.
    QFont bold;
    bold.setBold(true);
    auto totals = [&](int r, const QString& label, int col, int count, const QColor& color) {
        for (int c = 0; c < 6; ++c) tools_table->setItem(r, c, item("", Qt::NoItemFlags));
        tools_table->setItem(r, 1, item(label, Qt::ItemIsEnabled));
        tools_table->setItem(r, col, item(QString::number(count), Qt::ItemIsEnabled));
        for (int c : {1, col}) {
            tools_table->item(r, c)->setForeground(color);
            tools_table->item(r, c)->setFont(bold);
        }
    };
    totals(row, tr_("Total Drills"), 2, totDrills, QColor(127, 0, 255));
    totals(row + 1, tr_("Total Slots"), 3, totSlots, QColor(0, 70, 255));
    tools_table->item(row + 1, 2)->setFont(bold);

    tools_table->selectColumn(0);
    tools_table->resizeColumnsToContents();
    tools_table->resizeRowsToContents();
    tools_table->verticalHeader()->hide();
    tools_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QHeaderView* h = tools_table->horizontalHeader();
    h->setMinimumSectionSize(10);
    h->setDefaultSectionSize(70);
    h->setSectionResizeMode(0, QHeaderView::Fixed);
    h->resizeSection(0, 20);
    h->setSectionResizeMode(1, QHeaderView::Stretch);
    h->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    h->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    h->setSectionResizeMode(4, QHeaderView::Fixed);
    h->resizeSection(4, 17);
    h->setSectionResizeMode(5, QHeaderView::Fixed);
    h->resizeSection(5, 17);
    tools_table->setColumnWidth(5, 17);
    tools_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tools_table->setSortingEnabled(false);
    tools_table->setMinimumHeight(tools_table->contentHeight());
    tools_table->setMaximumHeight(tools_table->contentHeight());
    tooldia_entry->setDisabled(!hasDrills);
    generate_milling_button->setDisabled(!hasDrills);
    slot_tooldia_entry->setDisabled(!hasSlots);
    generate_milling_slots_button->setDisabled(!hasSlots);
}

CNCObjectUI::CNCObjectUI(QWidget* parent) : CNCObjectUIBase(parent) {
    setupUi();
    connect(export_gcode_button, &QPushButton::clicked, this, [this] {
        if (onExport) onExport();
    });
}

void CNCObjectUI::build(CNCJobObject& obj) {
    name_entry->setText(obj.name());
    plot_cb->setChecked(obj.plotEnabled());
    // CNCJobObject.build_ui: los trabajos de taladrado muestran la tabla de herramientas de Excellon.
    const auto& tools = obj.tools();
    const bool excellon = !tools.empty() && tools.front().drills > 0;
    cnc_tools_table->setVisible(!excellon);
    exc_cnc_tools_table->setVisible(excellon);
    if (!excellon) return;
    exc_cnc_tools_table->setRowCount(static_cast<int>(tools.size()));
    int row = 0;
    for (const CncToolRow& t : tools) {
        exc_cnc_tools_table->setItem(row, 0, item(QString::number(row + 1), Qt::ItemIsEnabled));
        exc_cnc_tools_table->setItem(row, 1, item(fmt(t.dia), Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        exc_cnc_tools_table->setItem(row, 2, item(QString::number(t.drills), Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        exc_cnc_tools_table->setItem(row, 3, item(QString::number(t.slots), Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        exc_cnc_tools_table->setItem(row, 4, item(QString::number(t.tool), Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        exc_cnc_tools_table->setItem(row, 5, item(QString::asprintf("%f", t.cutZ), Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        auto* plot = new FCCheckBox();
        plot->setLayoutDirection(Qt::RightToLeft);
        plot->setChecked(true);
        exc_cnc_tools_table->setCellWidget(row, 6, plot);
        ++row;
    }
    exc_cnc_tools_table->resizeColumnsToContents();
    exc_cnc_tools_table->resizeRowsToContents();
    exc_cnc_tools_table->verticalHeader()->hide();
    exc_cnc_tools_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    exc_cnc_tools_table->setColumnHidden(4, true);  // identificador interno de la herramienta
    exc_cnc_tools_table->setMinimumHeight(exc_cnc_tools_table->contentHeight());
    exc_cnc_tools_table->setMaximumHeight(exc_cnc_tools_table->contentHeight());
}

}  // namespace flatcam::gui
