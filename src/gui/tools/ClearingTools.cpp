#include "gui/tools/ClearingTools.h"

#include <QApplication>
#include <QHeaderView>
#include <QTableWidget>
#include <algorithm>

#include "gui/MainGUI.h"
#include "gui/Translation.h"
#include "gui/widgets/FCComboBox.h"
#include "gui/widgets/FCTable.h"
#include "tools/Isolation.h"

namespace flatcam::gui {

BusyScope::BusyScope(MainGUI* ui) : ui_(ui) {
    ui_->activity_view->setBusy(tr_("Busy") + "...");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
}

BusyScope::~BusyScope() {
    QApplication::restoreOverrideCursor();
    ui_->activity_view->setIdle();
}

std::vector<int> orderedClearingTools(const std::map<int, ClearingTool>& tools, std::vector<int> ids,
                                      const QString& order) {
    auto dia = [&tools](int id) { return QString::number(tools.at(id).geo.tooldia, 'f', 4).toDouble(); };
    if (order == "fwd") std::stable_sort(ids.begin(), ids.end(), [&](int a, int b) { return dia(a) < dia(b); });
    else if (order == "rev") std::stable_sort(ids.begin(), ids.end(), [&](int a, int b) { return dia(a) > dia(b); });
    return ids;
}

void buildClearingTable(QTableWidget* table, const std::map<int, ClearingTool>& tools, const QString& order) {
    std::vector<int> ids;
    for (const auto& [id, t] : tools) ids.push_back(id);
    ids = orderedClearingTools(tools, ids, order);
    const QSignalBlocker block(table);
    table->setRowCount(static_cast<int>(ids.size()));
    int row = 0;
    for (int id : ids) {
        const ClearingTool& t = tools.at(id);
        auto* num = new QTableWidgetItem(QString::number(row + 1));
        num->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        table->setItem(row, 0, num);
        // La columna del diámetro se puede editar (build_ui la habilita al final).
        auto* dia = new QTableWidgetItem(QString::number(t.geo.tooldia, 'f', 4));
        dia->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        table->setItem(row, 1, dia);
        auto* tt = new FCComboBox();
        tt->addItems({"C1", "C2", "C3", "C4", "B", "V"});
        tt->setCurrentIndex(tt->findText(t.geo.toolType));
        table->setCellWidget(row, 2, tt);
        table->setItem(row, 3, new QTableWidgetItem(QString::number(id)));
        ++row;
    }
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
    table->verticalHeader()->hide();
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QHeaderView* h = table->horizontalHeader();
    h->setMinimumSectionSize(10);
    h->setSectionResizeMode(0, QHeaderView::Fixed);
    h->resizeSection(0, 20);
    h->setSectionResizeMode(1, QHeaderView::Stretch);
    if (auto* fc = qobject_cast<FCTable*>(table)) {
        table->setMinimumHeight(fc->contentHeight());
        table->setMaximumHeight(fc->contentHeight());
    }
}

std::vector<int> selectedClearingTools(const QTableWidget* table) {
    std::vector<int> rows;
    for (const QTableWidgetItem* it : table->selectedItems())
        if (std::find(rows.begin(), rows.end(), it->row()) == rows.end()) rows.push_back(it->row());
    std::sort(rows.begin(), rows.end());
    std::vector<int> ids;
    for (int r : rows)
        if (const QTableWidgetItem* it = table->item(r, 3)) ids.push_back(it->text().toInt());
    return ids;
}

std::optional<double> findSafeToolDia(MainGUI* ui, FlatCAMObj* obj, const QString& name) {
    ui->inform(tr_("Checking tools for validity."));
    auto* grb = dynamic_cast<GerberObject*>(obj);
    if (!grb) {
        ui->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Object not found"), name));
        return std::nullopt;
    }
    std::string error;
    const auto d = Isolation::safeToolDiameter(grb->gerber(), 4, error);
    if (!d && error.rfind("The Gerber object", 0) == 0) ui->inform("[ERROR_NOTCL] " + tr_(error.c_str()));
    return d;
}

}  // namespace flatcam::gui
