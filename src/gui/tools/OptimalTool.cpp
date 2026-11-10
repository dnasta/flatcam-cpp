#include "gui/tools/OptimalTool.h"

#include <QRegularExpression>
#include <QTextBlockFormat>
#include <QTextCursor>

#include "cam/PyNumber.h"
#include "gui/App.h"
#include "gui/Preferences.h"
#include "gui/Translation.h"
#include "gui/objects/FlatCAMObj.h"
#include "gui/objects/ObjectCollection.h"
#include "tools/OptimalDistance.h"

namespace flatcam::gui {

OptimalTool::OptimalTool(App* app, QWidget* parent) : OptimalToolUIBase(parent), app_(app) {
    setupUi();
    connect(calculate_button, &QPushButton::clicked, this, [this] { findMinimumDistance(); });
    connect(locate_button, &QPushButton::clicked, this, [this] { locate(selectedText_); });
    connect(locate_sec_button, &QPushButton::clicked, this, [this] { locate(selectedSecText_); });
    connect(locations_textb, &QPlainTextEdit::cursorPositionChanged, this,
            [this] { selectedText_ = selectLine(locations_textb); });
    connect(locations_sec_textb, &QPlainTextEdit::cursorPositionChanged, this,
            [this] { selectedSecText_ = selectLine(locations_sec_textb); });
    connect(distances_textb, &QPlainTextEdit::cursorPositionChanged, this, [this] {
        // on_distances_textb_clicked + on_update_locations_text: los puntos de la distancia elegida.
        bool ok = false;
        const double d = selectLine(distances_textb).toDouble(&ok);
        if (!ok || !others_.count(d)) return;
        QString txt;
        for (const auto& p : others_.at(d)) txt += QString::fromStdString(pyPointPair(p)) + "\n";
        locations_sec_textb->setPlainText(txt);
    });
    connect(reset_button, &QPushButton::clicked, this, [this] { setToolUi(); });
    // OptionalHideInputSection: las casillas muestran u ocultan sus secciones.
    connect(locations_cb, &QCheckBox::toggled, this, [this](bool on) {
        locations_textb->setVisible(on);
        locate_button->setVisible(on);
    });
    connect(sec_locations_cb, &QCheckBox::toggled, sec_locations_frame, &QWidget::setVisible);
}

ToolInfo OptimalTool::info() { return {tr_("Optimal Tool"), tr_("Optimal Tool")}; }

void OptimalTool::setToolUi() {
    result_entry->setValue(0.0);
    freq_entry->setText("0");
    precision_spinner->setValue(defInt("tools_opt_precision", 4));
    locations_textb->clear();
    locations_textb->setVisible(false);
    locate_button->setVisible(false);
    const QString cur = gerber_object_combo->currentText();
    gerber_object_combo->clear();
    for (FlatCAMObj* o : app_->collection()->objects())
        if (o->kind() == ObjectKind::Gerber) gerber_object_combo->addItem(o->name());
    gerber_object_combo->setCurrentIndex(0);
    if (!cur.isEmpty()) gerber_object_combo->setValue(cur);
}

QString OptimalTool::selectLine(QPlainTextEdit* box) {
    // Resalta en amarillo la línea del cursor (y saca el resaltado de las demás) y devuelve su texto.
    QTextCursor all = box->textCursor();
    all.select(QTextCursor::Document);
    QTextBlockFormat clear = all.blockFormat();
    clear.clearBackground();
    all.setBlockFormat(clear);
    QTextCursor line = box->textCursor();
    line.select(QTextCursor::LineUnderCursor);
    QTextBlockFormat yellow = line.blockFormat();
    yellow.setBackground(Qt::yellow);
    line.setBlockFormat(yellow);
    return line.selectedText();
}

bool OptimalTool::findMinimumDistance() {
    decimals_ = precision_spinner->value();
    FlatCAMObj* o = app_->collection()->byName(gerber_object_combo->currentText());
    if (!o) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("There is no Gerber object loaded ..."));
        return false;
    }
    if (o->kind() != ObjectKind::Gerber) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("Only Gerber objects can be evaluated."));
        return false;
    }
    app_->ui()->inform(tr_("Optimal Tool. Started to search for the minimum distance between copper features."));
    OptimalResult r = findMinimumDistances(static_cast<GerberObject*>(o)->gerber(), decimals_);
    if (r.onePolygon) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("The Gerber object has one Polygon as geometry.\n"
                                                  "There are no distances between geometry elements to be found."));
        return false;
    }
    if (r.distances.empty()) return false;  // min() de una lista vacía: el original falla
    const auto first = r.distances.begin();
    result_entry->setText(QString::asprintf("%.*f", decimals_, first->first));
    freq_entry->setText(QString::number(first->second.size()));
    QString txt;
    for (const auto& p : first->second) txt += QString::fromStdString(pyPointPair(p)) + "\n";
    locations_textb->setPlainText(txt);
    locate_button->setDisabled(false);
    r.distances.erase(first);
    others_ = r.distances;
    QString dist;
    for (const auto& [d, l] : others_) dist += QString::fromStdString(pyFloatRepr(d)) + "\n";
    distances_textb->setPlainText(dist);
    locate_sec_button->setDisabled(false);
    app_->ui()->inform("[success] " + tr_("Optimal Tool. Finished successfully."));
    return true;
}

bool OptimalTool::locate(const QString& text) {
    if (text.isEmpty()) return false;
    static const QRegularExpression num(R"([-+]?\d+(?:\.\d*)?(?:[eE][-+]?\d+)?)");
    std::vector<double> v;
    for (auto it = num.globalMatch(text); it.hasNext();) v.push_back(it.next().captured().toDouble());
    if (v.size() != 4) {
        app_->ui()->inform("[ERROR_NOTCL] The selected text is no valid location in the format ((x0, y0), (x1, y1)).");
        return false;
    }
    auto mid = [&](double a, double b) {
        return QString::number(std::min(a, b) + std::abs(a - b) / 2, 'f', decimals_).toDouble();
    };
    app_->jumpTo(QPointF(mid(v[0], v[2]), mid(v[1], v[3])));
    return true;
}

}  // namespace flatcam::gui
