#include "gui/ExclusionAreasManager.h"

#include <QItemSelectionModel>
#include <QPushButton>
#include <algorithm>

#include "cam/PyNumber.h"
#include "gui/CanvasPicker.h"
#include "gui/MainGUI.h"
#include "gui/PlotCanvas.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/widgets/FCCheckBox.h"
#include "gui/widgets/FCSpinners.h"
#include "gui/widgets/FCTable.h"
#include "gui/widgets/RadioSet.h"

namespace flatcam::gui {

namespace {
constexpr int kLayerId = -2;  ///< Capa auxiliar de las áreas (encima de los objetos, fuera del encuadre).

const char* kCncDefaultStyle = "QPushButton\n{\n    font-weight: bold;\n}\n";
}  // namespace

ExclusionAreasManager::ExclusionAreasManager(MainGUI* ui, CanvasPicker* picker, QObject* parent)
    : QObject(parent), ui_(ui), picker_(picker) {}

void ExclusionAreasManager::startAdding(RadioSet* shapeRadio, FCDoubleSpinner* overZ, RadioSet* strategy,
                                        QPushButton* cncButton, ShapeList solidGeometry, const QString& objType) {
    ui_->inform("[WARNING_NOTCL] " + tr_("Click the start point of the area."));
    const auto mode = shapeRadio->value().toString() == "square" ? CanvasPicker::Mode::Rectangles
                                                                  : CanvasPicker::Mode::Polygons;
    picker_->start(mode, {}, [this, overZ, strategy, cncButton, solidGeometry, objType](const ShapeList& zones) {
        // Cada zona con la estrategia y la Z que muestra el panel.
        for (const Shape& z : zones)
            areas_.push_back(ExclusionArea{objType.toStdString(), z, strategy->value().toString().toStdString(),
                                           overZ->value()});
        redraw();
        Q_EMIT changed();
        if (areas_.empty()) return;
        // Como las áreas valen para todos los objetos, solo se controla el objeto actual.
        ui_->inform(tr_("Exclusion areas added. Checking overlap with the object geometry ..."));
        const Shape solid = Shape::unaryUnion(solidGeometry);
        for (const ExclusionArea& a : areas_) {
            if (a.shape.intersects(solid)) {
                clearAll();
                cncButton->setStyleSheet(kCncDefaultStyle);
                cncButton->setToolTip(tr_("Generate the CNC Job object."));
                ui_->inform("[ERROR_NOTCL] " + tr_("Failed. Exclusion areas intersects the object geometry ..."));
                return;
            }
        }
        ui_->inform("[success] " + tr_("Exclusion areas added."));
        cncButton->setStyleSheet("QPushButton\n{\n    font-weight: bold;\n    color: orange;\n}\n");
        cncButton->setToolTip(QString("%1 %2").arg(tr_("Generate the CNC Job object."), tr_("With Exclusion areas.")));
    });
}

void ExclusionAreasManager::clearAll() {
    if (!areas_.empty()) ui_->inform(tr_("All exclusion zones deleted."));
    areas_.clear();
    redraw();
    Q_EMIT changed();
}

void ExclusionAreasManager::deleteRows(std::vector<int> rows) {
    std::sort(rows.rbegin(), rows.rend());
    for (int r : rows)
        if (r >= 0 && r < static_cast<int>(areas_.size())) areas_.erase(areas_.begin() + r);
    redraw();
    if (!areas_.empty()) ui_->inform("[success] " + tr_("Selected exclusion zones deleted."));
    else ui_->inform(tr_("All exclusion zones deleted."));
    Q_EMIT changed();
}

void ExclusionAreasManager::fillTable(FCTable* table) const {
    table->setRowCount(static_cast<int>(areas_.size()));
    auto item = [](const QString& t) {
        auto* it = new QTableWidgetItem(t);
        it->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        return it;
    };
    for (std::size_t i = 0; i < areas_.size(); ++i) {
        const ExclusionArea& a = areas_[i];
        const int r = static_cast<int>(i);
        table->setItem(r, 0, item(QString::number(r + 1)));                   // Area id
        table->setItem(r, 1, item(QString::fromStdString(a.objType)));        // Origin Object
        table->setItem(r, 2, item(QString::fromStdString(a.strategy)));       // Strategy
        table->setItem(r, 3, item(QString::fromStdString(pyFloatRepr(a.overZ))));  // Over Z: str(float)
    }
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
    table->setColumnWidth(0, 20);
}

void ExclusionAreasManager::bindPanel(FCCheckBox* cb, QWidget* frame, RadioSet* shapeRadio, RadioSet* strategy,
                                      QWidget* overZLabel, FCDoubleSpinner* overZ, QPushButton* add,
                                      QPushButton* deleteAll, QPushButton* deleteSelected, QPushButton* cncButton,
                                      FCTable* table, std::function<ShapeList()> solidGeometry,
                                      const QString& objType) {
    frame->setVisible(cb->isChecked());
    connect(cb, &QCheckBox::toggled, frame, [frame](bool on) { frame->setVisible(on); });
    connect(strategy, &RadioSet::activatedCustom, frame, [overZLabel, overZ](const QString& val) {
        overZLabel->setDisabled(val == "around");
        overZ->setDisabled(val == "around");
    });
    connect(add, &QPushButton::clicked, frame, [=] {
        startAdding(shapeRadio, overZ, strategy, cncButton, solidGeometry(), objType);
    });
    connect(deleteAll, &QPushButton::clicked, frame, [this, cncButton] {
        if (areas_.empty()) {
            ui_->inform("[WARNING_NOTCL] " + tr_("Delete failed. There are no exclusion areas to delete."));
            return;
        }
        clearAll();
        cncButton->setStyleSheet(kCncDefaultStyle);
        cncButton->setToolTip(tr_("Generate the CNC Job object."));
    });
    connect(deleteSelected, &QPushButton::clicked, frame, [this, table, cncButton] {
        std::vector<int> rows;
        for (const QModelIndex& i : table->selectionModel()->selectedRows()) rows.push_back(i.row());
        if (rows.empty()) {
            ui_->inform("[WARNING_NOTCL] " + tr_("Delete failed. Nothing is selected."));
            return;
        }
        deleteRows(rows);
        if (areas_.empty()) {
            cncButton->setStyleSheet(kCncDefaultStyle);
            cncButton->setToolTip(tr_("Generate the CNC Job object."));
        }
    });
    connect(this, &ExclusionAreasManager::changed, table, [this, table] { fillTable(table); });
    fillTable(table);
}

void ExclusionAreasManager::redraw() {
    if (areas_.empty()) {
        ui_->plotCanvas()->removeLayer(kLayerId);
        return;
    }
    // Excellon: borde #FF7400; Geometría: #098a8f; relleno #FF7400BF.
    PlotLayerData d;
    d.fillColor = colorFromFlatCam("#FF7400BF");
    d.lineColor = colorFromFlatCam("#FF7400");
    for (const ExclusionArea& a : areas_) {
        PlotGroup g;
        g.lineColor = colorFromFlatCam(a.objType == "excellon" ? "#FF7400" : "#098a8f");
        g.fillColor = colorFromFlatCam("#FF7400BF");
        FlatCAMObj::addShapeTo(a.shape, g, d.bounds, true);
        d.groups.push_back(std::move(g));
    }
    ui_->plotCanvas()->setLayer(kLayerId, std::move(d));
}

}  // namespace flatcam::gui
