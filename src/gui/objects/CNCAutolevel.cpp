#include "gui/objects/CNCAutolevel.h"

#include <QApplication>
#include <QDateTime>
#include <QElapsedTimer>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QHeaderView>
#include <QLocale>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTabBar>
#include <QThread>
#include <QToolButton>
#include <QVBoxLayout>
#ifdef FLATCAM_HAVE_SERIALPORT
#include <QSerialPort>
#include <QSerialPortInfo>
#endif

#include "cam/PyNumber.h"
#include "gui/App.h"
#include "gui/MainGUI.h"
#include "gui/PlotCanvas.h"
#include "gui/Preferences.h"
#include "gui/ProjectIO.h"
#include "gui/Translation.h"
#include "gui/objects/CNCJobObject.h"
#include "gui/objects/ObjectUIs.h"

#ifndef FLATCAM_HAVE_SERIALPORT
class QSerialPort {};
#endif

namespace flatcam::gui {

namespace {

constexpr int kProbingLayer = -16;  ///< `probing_shapes`

QString timeStr() { return QLocale::c().toString(QDateTime::currentDateTime(), "dddd, dd MMMM yyyy 'at' HH:mm"); }

FCDoubleSpinner* sliderSpin(QWidget* w) { return w ? w->findChild<FCDoubleSpinner*>() : nullptr; }

QToolButton* at(QWidget* frame, int r, int c) {
    auto* g = frame ? qobject_cast<QGridLayout*>(frame->layout()) : nullptr;
    QLayoutItem* it = g ? g->itemAtPosition(r, c) : nullptr;
    return it ? qobject_cast<QToolButton*>(it->widget()) : nullptr;
}

}  // namespace

CNCAutolevel::CNCAutolevel(App* app, CNCJobObject* job) : QObject(job), app_(app), job_(job) {}

CNCAutolevel::~CNCAutolevel() {
    if (picking_) finishPicking(false);
}

bool CNCAutolevel::mm() const { return job_->header().units != "IN"; }

std::optional<BoundingBox> CNCAutolevel::cutBounds() const {
    // solid_geo = unary_union de lo que corta en gcode_parsed (kind[0] == 'C').
    ShapeList cuts;
    const auto& tools = job_->tools();
    const auto& gcodes = job_->job().toolGcodes();
    const bool drilling = job_->isDrilling();
    for (std::size_t i = 0; i < std::max<std::size_t>(gcodes.size(), 1); ++i) {
        const std::string g = gcodes.empty() ? job_->job().gcode() : gcodes[i];
        const CncToolRow* r = i < tools.size() ? &tools[i] : (tools.empty() ? nullptr : &tools.back());
        const bool laser =
            r && r->data[drilling ? "tools_drill_ppname_e" : "ppname_g"].text().find("laser") != std::string::npos;
        const JsonValue parsed = ProjectIO::gcodeParse(g, laser, drilling && r ? r->dia : 0.0);
        for (const JsonValue& seg : parsed.items()) {
            const JsonValue& kind = seg["kind"];
            if (!kind.isArray() || kind.items().empty() || kind.items()[0].text() != "C") continue;
            cuts.push_back(Shape::fromWkt(seg["geom"]["__inst__"].text()));
        }
    }
    if (cuts.empty()) return std::nullopt;
    return Shape::unaryUnion(cuts).bounds();
}

void CNCAutolevel::attach(CNCObjectUI* ui) {
    if (ui_ == ui) return;
    ui_ = ui;
    // set_ui
    const bool basic = defStr("global_app_level") != "a";
    ui->level->setText(basic ? QString("<span style=\"color:green;\"><b>%1</b></span>").arg(tr_("Basic"))
                             : QString("<span style=\"color:red;\"><b>%1</b></span>").arg(tr_("Advanced")));
    bool roland = false;
    for (const CncToolRow& r : job_->tools()) {
        const std::string pp = r.data[job_->isDrilling() ? "tools_drill_ppname_e" : "ppname_g"].text();
        roland = roland || pp.find("Roland") != std::string::npos || pp.find("hpgl") != std::string::npos;
    }
    if (basic || roland) {
        ui->sal_btn->hide();
        ui->sal_btn->setChecked(false);
    } else {
        ui->sal_btn->show();
        ui->sal_btn->setChecked(defBool("cncjob_al_status"));
    }
    ui->al_frame->setVisible(ui->sal_btn->isChecked());
    ui->ptravelz_entry->setValue(defNum("cncjob_al_travelz"));
    ui->pdepth_entry->setValue(defNum("cncjob_al_probe_depth"));
    ui->feedrate_probe_entry->setValue(defNum("cncjob_al_probe_fr"));
    ui->al_controller_combo->setValue(defStr("cncjob_al_controller"));
    ui->al_method_radio->setValue(defVariant("cncjob_al_method"));
    ui->al_rows_entry->setValue(defInt("cncjob_al_rows"));
    ui->al_columns_entry->setValue(defInt("cncjob_al_columns"));
    if (auto* s = sliderSpin(ui->jog_step_entry)) s->setValue(defNum("cncjob_al_grbl_jog_step"));
    if (auto* s = sliderSpin(ui->jog_fr_entry)) s->setValue(defNum("cncjob_al_grbl_jog_fr"));
    // Las pestañas Controlar y Remitente se habilitan al conectar.
    for (int i = 0; i < ui->al_toolbar->count(); ++i)
        if (ui->al_toolbar->tabText(i) == tr_("Control") || ui->al_toolbar->tabText(i) == tr_("Sender"))
            ui->al_toolbar->setTabEnabled(i, false);

    connect(ui->sal_btn, &QAbstractButton::toggled, this, [this](bool on) {
        ui_->al_frame->setVisible(on);
        if (Preferences* p = app_->preferences()) p->set("cncjob_al_status", JsonValue::boolean(on));
    });
    connect(ui->show_al_table, &QCheckBox::checkStateChanged, this,
            [this] { ui_->al_probe_points_table->setVisible(ui_->show_al_table->isChecked()); });
    connect(ui->al_mode_radio, &RadioSet::activatedCustom, this, [this](const QString& v) { onModeRadio(v); });
    connect(ui->al_method_radio, &RadioSet::activatedCustom, this, [this](const QString& v) { onMethodRadio(v); });
    connect(ui->al_controller_combo, &QComboBox::currentIndexChanged, this, [this] { onControllerChange(); });
    connect(ui->plot_probing_pts_cb, &QCheckBox::checkStateChanged, this,
            [this] { showProbingGeo(ui_->plot_probing_pts_cb->isChecked()); });
    connect(ui->al_add_button, &QPushButton::clicked, this, [this] { addProbePoints(); });
    connect(ui->h_gcode_button, &QPushButton::clicked, this, [this] {
        const QString f = QFileDialog::getSaveFileName(ui_, tr_("Export Code ..."), app_->lastFolder() + "/probing_gcode",
                                                       defStr("cncjob_save_filters"));
        if (f.isEmpty()) app_->ui()->inform("[WARNING_NOTCL] " + tr_("Export cancelled ..."));
        else saveProbingGcode(f);
    });
    connect(ui->view_h_gcode_button, &QPushButton::clicked, this, [this] {
        // on_edit_probing_gcode: el texto en una pestaña; "Actualizar" lo vuelve a tomar (on_update_probing_gcode).
        if (probingText_.isEmpty()) {
            app_->ui()->inform(QString("[WARNING_NOTCL] %1...").arg(tr_("There is nothing to view")));
            return;
        }
        auto* tab = new QWidget();
        auto* lay = new QVBoxLayout(tab);
        auto* editor = new QPlainTextEdit(probingText_);
        editor->setLineWrapMode(QPlainTextEdit::NoWrap);
        lay->addWidget(editor);
        auto* update = new QPushButton(tr_("Update"));
        lay->addWidget(update);
        connect(update, &QPushButton::clicked, this, [this, editor] { probingText_ = editor->toPlainText(); });
        const int idx = app_->ui()->plot_tab_area->addTab(tab, tr_("Code Viewer"));
        app_->ui()->plot_tab_area->setCurrentIndex(idx);
        app_->ui()->inform(QString("[success] %1...").arg(tr_("Loaded Machine Code into Code Editor")));
    });
    connect(ui->import_heights_button, &QPushButton::clicked, this, [this] {
        const QString f = QFileDialog::getOpenFileName(ui_, tr_("Import Height Map"), app_->lastFolder(),
                                                       "Text File .txt (*.txt);;All Files (*.*)");
        if (f.isEmpty()) app_->ui()->inform("[WARNING_NOTCL] " + tr_("Cancelled."));
        else importHeightMap(f);
    });
    // GRBL
    connect(ui->com_search_button, &QPushButton::clicked, this, [this] { searchPorts(false); });
    connect(ui->add_bd_button, &QPushButton::clicked, this, [this] {
        const QString bd = QString::number(static_cast<int>(ui_->new_baudrate_entry->value()));
        if (bd.toInt() >= 40 && ui_->baudrates_list_combo->findText(bd) < 0) {
            ui_->baudrates_list_combo->addItem(bd);
            ui_->baudrates_list_combo->setCurrentText(bd);
        }
    });
    connect(ui->del_bd_button, &QPushButton::clicked, this,
            [this] { ui_->baudrates_list_combo->removeItem(ui_->baudrates_list_combo->currentIndex()); });
    connect(ui->com_connect_button, &QPushButton::clicked, this, [this] { connectPort(); });
    connect(ui->controller_reset_button, &QPushButton::clicked, this, [this] {
        app_->ui()->inform(tr_("GRBL software reset was sent."));
        wake();
        sendCommand("\x18");
    });
    auto sendEntry = [this] {
        app_->ui()->shell_dock->show();
        sendCommand(ui_->grbl_command_entry->text());
    };
    connect(ui->grbl_send_button, &QPushButton::clicked, this, sendEntry);
    connect(ui->grbl_command_entry, &QLineEdit::returnPressed, this, sendEntry);
    const std::pair<std::pair<int, int>, const char*> jogs[] = {{{2, 1}, "yplus"}, {{4, 1}, "yminus"},
                                                                {{3, 2}, "xplus"}, {{3, 0}, "xminus"},
                                                                {{2, 3}, "zplus"}, {{4, 3}, "zminus"},
                                                                {{3, 1}, "origin"}};
    for (const auto& [pos, dir] : jogs)
        if (QToolButton* b = at(ui->jog_wdg, pos.first, pos.second))
            connect(b, &QToolButton::clicked, this, [this, d = QString(dir)] {
                for (const QString& c : jogCommands(d))
                    if (!sendCommand(c, false)) return;  // con respuesta, el original falla ('ok' in bytes)
            });
    const std::pair<std::pair<int, int>, const char*> zeros[] = {{{1, 0}, "x"}, {{2, 0}, "y"}, {{3, 0}, "z"}, {{1, 1}, "all"}};
    for (const auto& [pos, axis] : zeros)
        if (QToolButton* b = at(ui->zero_axs_wdg, pos.first, pos.second))
            connect(b, &QToolButton::clicked, this, [this, a = QString(axis)] {
                // on_grbl_zero: guarda $10, lo pone en 0, pone el cero y lo restaura.
                const auto mode = getParameter("10");
                if (!mode) return;
                if (!sendCommand("$10=0", false)) return;
                const QString cmd = a == "x"   ? "G10 L2 P1 X0"
                                    : a == "y" ? "G10 L2 P1 Y0"
                                    : a == "z" ? "G10 L2 P1 Z0"
                                               : "G10 L2 P1 X0 Y0 Z0";
                if (!sendCommand(cmd, false)) return;
                sendCommand(QString("$10=%1").arg(static_cast<int>(*mode)), false);
            });
    if (QToolButton* b = at(ui->zero_axs_wdg, 4, 0))
        connect(b, &QToolButton::clicked, this, [this] {
            app_->ui()->inform(tr_("GRBL is doing a home cycle."));
            wake();
            sendCommand("$H");
        });
    connect(ui->pause_resume_button, &QToolButton::toggled, this, [this](bool checked) {
        sendCommand(checked ? "!" : "~");
        app_->ui()->inform(checked ? tr_("GRBL paused.") : tr_("GRBL resumed."));
    });
    connect(ui->grbl_report_button, &QPushButton::clicked, this, [this] { sendCommand("?"); });
    connect(ui->grbl_get_param_button, &QPushButton::clicked, this,
            [this] { getParameter(ui_->grbl_parameter_entry->text()); });
    connect(ui->grbl_get_heightmap_button, &QPushButton::clicked, this, [this] { grblAutolevel(); });
    connect(ui->grbl_save_height_map_button, &QPushButton::clicked, this, [this] { saveHeightMap(); });

    ui->al_mode_radio->setValue(defVariant("cncjob_al_mode"));
    onControllerChange();
    onModeRadio(ui->al_mode_radio->value().toString());
    onMethodRadio(ui->al_method_radio->value().toString());
}

void CNCAutolevel::buildTable() {
    FCTable* t = ui_->al_probe_points_table;
    t->setRowCount(static_cast<int>(points_.size()));
    const int dec = appDecimals();
    for (std::size_t i = 0; i < points_.size(); ++i) {
        const int row = static_cast<int>(i);
        auto* id = new QTableWidgetItem(QString::number(row + 1));
        id->setFlags(Qt::ItemIsEnabled);
        // str((x, y)) de una tupla de floats
        auto* xy = new QTableWidgetItem(QString::fromStdString("(" + pyFloatRepr(decFormat(points_[i].point.x, dec)) +
                                                               ", " + pyFloatRepr(decFormat(points_[i].point.y, dec)) + ")"));
        xy->setFlags(Qt::ItemIsEnabled);
        auto* h = new QTableWidgetItem(QString::fromStdString(pyFloatRepr(decFormat(points_[i].height, dec))));
        h->setFlags(Qt::ItemIsEnabled);
        t->setItem(row, 0, id);
        t->setItem(row, 1, xy);
        t->setItem(row, 2, h);
    }
    t->resizeColumnsToContents();
    t->resizeRowsToContents();
    QHeaderView* hh = t->horizontalHeader();
    hh->setMinimumSectionSize(10);
    hh->setDefaultSectionSize(70);
    hh->setSectionResizeMode(0, QHeaderView::Fixed);
    hh->resizeSection(0, 20);
    hh->setSectionResizeMode(1, QHeaderView::Stretch);
    hh->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    t->setMinimumHeight(t->contentHeight());
    t->setMaximumHeight(t->contentHeight());
    const bool any = t->rowCount() > 0;
    for (QWidget* w : std::initializer_list<QWidget*>{ui_->grbl_get_heightmap_button, ui_->grbl_save_height_map_button,
                                                      ui_->h_gcode_button, ui_->view_h_gcode_button})
        w->setDisabled(!any);
}

void CNCAutolevel::onModeRadio(const QString& v) {
    ui_->al_probe_points_table->setRowCount(0);
    points_.clear();
    app_->ui()->plotCanvas()->removeLayer(kProbingLayer);
    buildTable();
    const bool manual = v == "manual";
    for (QWidget* w : std::initializer_list<QWidget*>{ui_->al_rows_entry, ui_->al_rows_label, ui_->al_columns_entry,
                                                      ui_->al_columns_label, ui_->al_method_lbl, ui_->al_method_radio})
        w->setDisabled(manual);
    ui_->al_method_radio->setValue(manual ? QVariant(QString("v")) : defVariant("cncjob_al_method"));
}

void CNCAutolevel::onMethodRadio(const QString& v) {
    ui_->al_columns_entry->setMinimum(v == "b" ? 2 : 1);
    ui_->al_rows_entry->setMinimum(v == "b" ? 2 : 1);
}

void CNCAutolevel::onControllerChange() {
    const bool grbl = ui_->al_controller_combo->currentText() == "GRBL";
    ui_->h_gcode_button->setVisible(!grbl);
    ui_->view_h_gcode_button->setVisible(!grbl);
    ui_->import_heights_button->setVisible(!grbl);
    ui_->grbl_frame->setVisible(grbl);
    if (grbl) searchPorts(true);
    if (probingText_.isEmpty()) regenerateGcode();
}

void CNCAutolevel::regenerateGcode() {
    ProbingGcodeParams p;
    p.controller = ui_->al_controller_combo->currentText().toStdString();
    p.units = job_->header().units;
    p.name = job_->name().toStdString();
    p.version = Preferences::version();
    p.versionDate = "2024/04/15";
    p.timeStr = timeStr().toStdString();
    p.travelZ = ui_->ptravelz_entry->value();
    p.probeDepth = ui_->pdepth_entry->value();
    p.probeFeedrate = ui_->feedrate_probe_entry->value();
    p.decimals = appDecimals();
    p.coordsDecimals = defInt("cncjob_coords_decimals", 4);
    p.frDecimals = defInt("cncjob_fr_decimals", 2);
    std::vector<Point2D> pts;
    if (ui_->al_method_radio->value().toString() == "v")
        for (const ProbePoint& pp : points_) pts.push_back(pp.point);
    else
        pts = bilinear_;
    probingText_ = QString::fromStdString(autolevelProbingGcode(pts, p));
}

void CNCAutolevel::addProbePoints() {
    MainGUI* ui = app_->ui();
    ui_->al_probe_points_table->setRowCount(0);
    points_.clear();
    const auto b = cutBounds();
    if (!b) return;  // sin nada que corte, el original falla (bounds de una colección vacía)
    if (ui_->al_mode_radio->value().toString() == "grid") {
        const std::vector<Point2D> grid =
            autolevelGrid(*b, ui_->al_rows_entry->value(), ui_->al_columns_entry->value(), appDecimals());
        for (const Point2D& p : grid) points_.push_back({p, std::nullopt, 0.0});
        if (ui_->al_method_radio->value().toString() == "v") {
            autolevelVoronoi(points_, *b, mm());
        } else {
            bilinear_ = grid;
        }
        regenerateGcode();
        buildTable();
        if (ui_->plot_probing_pts_cb->isChecked()) showProbingGeo(true);
        else app_->ui()->plotCanvas()->removeLayer(kProbingLayer);
        return;
    }
    // Manual: el primer punto en (xmin, xmin), como el original, y después los clics.
    points_.push_back({{b->xmin, b->xmin}, std::nullopt, 0.0});
    ui->inform(tr_("Click on canvas to add a Probe Point..."));
    ui->setCanvasHandlersEnabled(false);
    disconnect(conn_);
    conn_ = connect(ui->plotCanvas(), &PlotCanvas::mouseReleased, this,
                    [this](QPointF w, Qt::MouseButton btn, bool dragged) { onRelease(w, btn, dragged); });
    picking_ = true;
    buildTable();
    showProbingGeo(ui_->plot_probing_pts_cb->isChecked());
}

void CNCAutolevel::onRelease(const QPointF& world, Qt::MouseButton button, bool dragged) {
    if (!picking_) return;
    MainGUI* ui = app_->ui();
    if (button == Qt::LeftButton) {
        const auto b = cutBounds();
        const Point2D p{world.x(), world.y()};
        // within(box): estrictamente adentro
        if (!b || !(p.x > b->xmin && p.x < b->xmax && p.y > b->ymin && p.y < b->ymax)) {
            ui->inform(tr_("Point is not within the object area. Choose another point."));
            return;
        }
        points_.push_back({p, std::nullopt, 0.0});
        buildTable();
        showProbingGeo(true);
        ui->inform(tr_("Added a Probe Point... Click again to add another or right click to finish ..."));
    } else if (button == Qt::RightButton && !dragged) {
        finishPicking(true);
        ui->inform(tr_("Finished adding Probe Points..."));
        if (ui_->al_method_radio->value().toString() == "v") {
            if (const auto b = cutBounds()) autolevelVoronoi(points_, *b, mm());
            regenerateGcode();
        }
        buildTable();
        showProbingGeo(ui_->plot_probing_pts_cb->isChecked());
    }
}

bool CNCAutolevel::handleKey(int key) {
    if (!picking_) return false;
    if (key == Qt::Key_Escape) {
        finishPicking(true);
        return true;
    }
    if (key == Qt::Key_G) {
        app_->ui()->grid_snap_btn->trigger();
        return true;
    }
    if (key == Qt::Key_J) {
        app_->onJumpTo();
        return true;
    }
    return false;
}

void CNCAutolevel::finishPicking(bool) {
    disconnect(conn_);
    picking_ = false;
    if (app_ && app_->ui()) app_->ui()->setCanvasHandlersEnabled(true);
}

void CNCAutolevel::showProbingGeo(bool state) {
    PlotCanvas* canvas = app_->ui()->plotCanvas();
    if (!state) {
        canvas->removeLayer(kProbingLayer);
        return;
    }
    // Voronoi: un punto negro por punto y su celda; bilineal: solo los puntos.
    PlotLayerData d;
    const double r = mm() ? 0.1 : 0.004;
    PlotGroup dots, cells;
    dots.fillColor = dots.lineColor = colorFromFlatCam("#000000FF");
    cells.fillColor = colorFromFlatCam("#BBF26888");
    cells.lineColor = colorFromFlatCam("#000000FF");
    QRectF bounds;
    if (ui_->al_method_radio->value().toString() == "v") {
        for (const ProbePoint& p : points_) {
            if (p.cell) {
                FlatCAMObj::addShapeTo(p.cell->buffer(0.0000001), cells, bounds);
                FlatCAMObj::addShapeTo(Shape::circle(p.point, r, 16), dots, bounds);
            } else if (picking_) {  // los puntos agregados con clics (aún sin celda)
                FlatCAMObj::addShapeTo(Shape::circle(p.point, mm() ? 0.3 : 0.012, 16), dots, bounds);
            }
        }
    } else {
        for (const Point2D& p : bilinear_) FlatCAMObj::addShapeTo(Shape::circle(p, r, 16), dots, bounds);
    }
    if (!cells.lines.empty()) d.groups.push_back(std::move(cells));
    if (!dots.lines.empty()) d.groups.push_back(std::move(dots));
    d.bounds = bounds;
    canvas->setLayer(kProbingLayer, std::move(d));
}

bool CNCAutolevel::importHeightMap(const QString& filename) {
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        app_->ui()->inform(QString("[ERROR_NOTCL] %1: %2").arg(tr_("Failed to open height map file"), filename));
        return false;
    }
    std::vector<std::string> lines;
    while (!f.atEnd()) lines.push_back(f.readLine().toStdString());
    const bool ok = autolevelImportHeightMap(lines, points_);
    if (ok) buildTable();  // si una línea falla, el original se corta antes de armar la tabla
    return ok;
}

bool CNCAutolevel::saveProbingGcode(const QString& filename) {
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly)) {
        app_->ui()->inform("[WARNING] " + tr_("Permission denied, saving not possible.\nMost likely another app is "
                                              "holding the file open and not accessible."));
        return false;
    }
    QString text = probingText_;
    const bool crlf = defBool("cncjob_line_ending");
#ifdef Q_OS_WIN
    text.replace("\n", "\r\n");
#else
    if (crlf) text.replace("\n", "\r\n");
#endif
    f.write(text.toUtf8());
    return true;
}

QStringList CNCAutolevel::jogCommands(const QString& direction) const {
    const FCDoubleSpinner* step = sliderSpin(ui_->jog_step_entry);
    const FCDoubleSpinner* fr = sliderSpin(ui_->jog_fr_entry);
    QStringList out;
    for (const std::string& c : grblJogCommands(direction.toStdString(), step ? step->value() : 0.0,
                                                fr ? fr->value() : 0.0, job_->header().units,
                                                defNum("cncjob_al_grbl_travelz")))
        out << QString::fromStdString(c);
    return out;
}

void CNCAutolevel::shell(const QString& msg) { app_->ui()->inform(msg); }

void CNCAutolevel::searchPorts(bool muted) {
    ui_->com_list_combo->clear();
#ifdef FLATCAM_HAVE_SERIALPORT
    // on_grbl_list_serial_ports: los puertos que se pueden abrir.
    for (const QSerialPortInfo& info : QSerialPortInfo::availablePorts()) {
        QSerialPort probe(info);
        if (probe.open(QIODevice::ReadWrite)) {
            probe.close();
            ui_->com_list_combo->addItem(info.systemLocation());
        }
    }
#endif
    if (!muted) app_->ui()->inform("[WARNING_NOTCL] " + tr_("COM list updated ..."));
}

void CNCAutolevel::connectPort() {
    QString name = ui_->com_list_combo->currentText();
    if (name.contains(" (")) name = name.left(name.lastIndexOf(" ("));
#ifdef FLATCAM_HAVE_SERIALPORT
    port_ = std::make_unique<QSerialPort>(name);
    port_->setBaudRate(ui_->baudrates_list_combo->currentText().toInt());
    port_->setDataBits(QSerialPort::Data8);
    port_->setParity(QSerialPort::NoParity);
    port_->setStopBits(QSerialPort::OneStop);
    port_->setFlowControl(QSerialPort::NoFlowControl);
    if (port_->open(QIODevice::ReadWrite)) {
        // Se alterna DTR para reiniciar el controlador; después "despertar". El original ignora la respuesta (un FIXME
        // la reemplaza por ['ok']): con el puerto abierto, queda conectado.
        port_->setDataTerminalReady(false);
        port_->clear(QSerialPort::Input);
        port_->setDataTerminalReady(true);
        wake();
        ui_->com_connect_button->setStyleSheet("QPushButton {background-color: seagreen;}");
        ui_->com_connect_button->setText(tr_("Connected"));
        ui_->controller_reset_button->setDisabled(false);
        for (int i = 0; i < ui_->al_toolbar->count(); ++i) {
            if (ui_->al_toolbar->tabText(i) == tr_("Connect"))
                ui_->al_toolbar->tabBar()->setTabTextColor(i, QColor("seagreen"));
            if (ui_->al_toolbar->tabText(i) == tr_("Control") || ui_->al_toolbar->tabText(i) == tr_("Sender"))
                ui_->al_toolbar->setTabEnabled(i, true);
        }
        app_->ui()->inform(QString("%1: %2").arg(tr_("Port connected"), name));
        return;
    }
    port_.reset();
#endif
    // serial.SerialException: queda desconectado.
    ui_->com_connect_button->setStyleSheet("QPushButton {background-color: red;}");
    ui_->com_connect_button->setText(tr_("Disconnected"));
    ui_->controller_reset_button->setDisabled(true);
    for (int i = 0; i < ui_->al_toolbar->count(); ++i) {
        if (ui_->al_toolbar->tabText(i) == tr_("Connect"))
            ui_->al_toolbar->tabBar()->setTabTextColor(i, QColor("red"));
        if (ui_->al_toolbar->tabText(i) == tr_("Control") || ui_->al_toolbar->tabText(i) == tr_("Sender"))
            ui_->al_toolbar->setTabEnabled(i, false);
    }
    app_->ui()->inform(QString("%1: %2").arg(tr_("Port is connected. Disconnecting"), name));
}

QStringList CNCAutolevel::wake() {
    QStringList out;
#ifdef FLATCAM_HAVE_SERIALPORT
    if (!port_) return out;
    port_->write("\r\n\r\n");
    port_->waitForBytesWritten(100);
    QThread::msleep(1000);
    port_->waitForReadyRead(100);
    for (const QByteArray& l : port_->readAll().split('\n'))
        if (!l.isEmpty()) out << QString::fromUtf8(l);
    port_->clear(QSerialPort::Input);
#endif
    return out;
}

std::optional<QStringList> CNCAutolevel::sendCommand(const QString& command, bool echo) {
    const QString cmd = command.trimmed();
    if (echo) shell(cmd);
    QStringList lines;
#ifdef FLATCAM_HAVE_SERIALPORT
    if (port_) {
        port_->write((cmd + "\n").toUtf8());
        port_->waitForBytesWritten(100);
        // readlines() con timeout de 0,1 s.
        while (port_->waitForReadyRead(100))
            ;
        for (const QByteArray& l : port_->readAll().split('\n'))
            if (!l.isEmpty()) lines << QString::fromUtf8(l);
    }
#endif
    if (lines.isEmpty()) {
        shell("\t\t\t: No answer\n");
        return QStringList();
    }
    if (echo)
        for (const QString& l : lines) shell("\t\t\t: " + l.trimmed().toUpper());
    // `'ok' in line` con una línea en bytes: TypeError. Con cualquier respuesta, lo que sigue no se hace.
    return std::nullopt;
}

std::optional<double> CNCAutolevel::getParameter(QString param) {
    // on_grbl_get_parameter: "$$" y la línea con "$<n>" (decodificada: acá no falla).
    param.remove('$');
    QStringList lines;
#ifdef FLATCAM_HAVE_SERIALPORT
    if (port_) {
        port_->write("$$\n");
        port_->waitForBytesWritten(100);
        while (port_->waitForReadyRead(100))
            ;
        for (const QByteArray& l : port_->readAll().split('\n'))
            if (!l.isEmpty()) lines << QString::fromUtf8(l);
    }
#endif
    for (const QString& l : lines)
        if (l.contains("$" + param)) {
            const double v = l.mid(l.lastIndexOf('=') + 1).trimmed().toDouble();
            shell(QString("GRBL Parameter: %1 = %2").arg(param, QString::fromStdString(pyFloatRepr(v))));
            return v;
        }
    return std::nullopt;
}

void CNCAutolevel::grblAutolevel() {
    app_->ui()->shell_dock->show();
    app_->ui()->inform(tr_("Sending probing GCode to the GRBL controller."));
    grblProbeResult_.clear();
    const QString travel = QString::fromStdString(pyFloatRepr(ui_->ptravelz_entry->value()));
    const QString fr = QString::fromStdString(pyFloatRepr(ui_->feedrate_probe_entry->value()));
    const QString depth = QString::fromStdString(pyFloatRepr(ui_->pdepth_entry->value()));
    if (!sendCommand("G21\n") || !sendCommand("G90\n")) return;
    for (const ProbePoint& p : points_) {
        if (!sendCommand("G0 Z" + travel + "\n")) return;
        if (!sendCommand(QString::fromStdString("G0 X" + pyFloatRepr(p.point.x) + " Y" + pyFloatRepr(p.point.y)) + "\n"))
            return;
        const auto out = sendCommand("G38.2 Z" + depth + " F" + fr);
        if (!out) return;
        grblProbeResult_ += out->join("") + "\n";
    }
    if (!sendCommand("M2\n")) return;
    app_->ui()->inform(tr_("Finished probing. Doing the autolevelling."));
    app_->ui()->inform(tr_("Finished autolevelling."));  // on_grbl_apply_autolevel: solo el mensaje
}

void CNCAutolevel::saveHeightMap() {
    if (grblProbeResult_.isEmpty()) {
        app_->ui()->inform("[ERROR_NOTCL] " + tr_("Empty GRBL heightmap."));
        return;
    }
    const QString f = QFileDialog::getSaveFileName(ui_, tr_("Export Code ..."), app_->lastFolder() + "/probing_gcode",
                                                   "Text File .txt (*.txt);;All Files (*.*)");
    if (f.isEmpty()) {
        app_->ui()->inform("[WARNING_NOTCL] " + tr_("Export cancelled ..."));
        return;
    }
    QFile out(f);
    if (out.open(QIODevice::WriteOnly | QIODevice::Text)) out.write(grblProbeResult_.toUtf8());
}

}  // namespace flatcam::gui
