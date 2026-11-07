#include "gui/MainGUI.h"

#include <QScreen>

#include <cmath>

#include <QApplication>
#include <QRegularExpression>

#include "gui/PlotCanvas.h"
#include "gui/Translation.h"

namespace flatcam::gui {

MainGUI::MainGUI(QWidget* parent) : MainGUIBase(parent) {
    setupUi();

    // MainGUI.__init__: ícono de la ventana en varios tamaños.
    QIcon appIcon;
    for (int s : {16, 24, 32, 48, 128, 256}) {
        appIcon.addFile(QString(":/images/flatcam_icon%1.png").arg(s), QSize(s, s));
    }
    setWindowIcon(appIcon);

    // App.on_plotcanvas_setup: el canvas va en el layout de la pestaña "Área de la parcela".
    plotcanvas_ = new PlotCanvas(plot_tab);
    right_layout->addWidget(plotcanvas_);

    // restore_main_win_geom con las opciones de fábrica (global_def_win_x/y/w/h) y, como global_project_at_startup es
    // falso, el panel izquierdo empieza cerrado.
    setGeometry(100, 100, 1024, 650);
    splitter->setStretchFactor(1, 2);
    splitter->setSizes({0, 1});

    connectSignals();
}

void MainGUI::connectSignals() {
    connect(menu_toggle_nb, &QAction::triggered, this, [this] { onToggleNotebook(); });
    connect(menufile_exit, &QAction::triggered, qApp, &QApplication::quit);
    connect(plotcanvas_, &PlotCanvas::mouseMoved, this, [this](QPointF w) {
        if (canvasHandlers_) updateCoordinates(w);
    });
    connect(plotcanvas_, &PlotCanvas::mouseClicked, this, [this](QPointF w, Qt::MouseButton b) {
        if (canvasHandlers_ && b == Qt::LeftButton) relPoint1_ = w;
    });
    // Botones de la barra de estado: HUD y ejes (on_toggle_hud / on_toggle_axis cambian además su estilo).
    connect(hud_label, &FCLabel::clicked, this, [this] { onToggleHud(); });
    connect(axis_status_label, &FCLabel::clicked, this, [this] { onToggleAxis(); });
    // Menú Ver.
    connect(menuview_toggle_fscreen, &QAction::triggered, this, [this] { onFullscreen(); });
    connect(menuview_toggle_parea, &QAction::triggered, this, [this] { onTogglePlotArea(); });
    connect(menuview_toggle_notebook, &QAction::triggered, this, [this] { onToggleNotebook(); });
    connect(menuview_toggle_grid, &QAction::triggered, this, [this] { onToggleGrid(); });
    connect(menuview_toggle_grid_lines, &QAction::triggered, this, [this] { onToggleGridLines(); });
    connect(menuview_toggle_axis, &QAction::triggered, this, [this] { onToggleAxis(); });
    connect(menuview_toggle_hud, &QAction::triggered, this, [this] { onToggleHud(); });
    // Ajuste a la grilla: las posiciones del mouse salen ajustadas y la cruz del cursor solo se ve con la grilla activa.
    plotcanvas_->setSnapper([this](QPointF p) { return snapToGrid(p); });
    plotcanvas_->setCursorEnabled(grid_snap_btn->isChecked());
    connect(grid_snap_btn, &QAction::toggled, this, [this](bool on) { plotcanvas_->setCursorEnabled(on); });
    // grid_changed: con "enlazar", la grilla Y copia la X (con 4 decimales).
    connect(grid_gap_x_entry, &QLineEdit::textChanged, this, [this](const QString& text) {
        bool ok = false;
        const double v = QString(text).replace(',', '.').toDouble(&ok);
        if (ok && grid_gap_link_cb->isChecked()) grid_gap_y_entry->setValue(v, 4);
    });
    // Estado inicial: HUD y ejes encendidos (global_hud, global_axis).
    hud_label->setStyleSheet("QLabel { color: black; background-color: mediumpurple; }");
    axis_status_label->setStyleSheet("QLabel { color: black; background-color: orange; }");
}

void MainGUI::onToggleNotebook() {
    if (splitter->sizes().value(0) == 0) {
        splitter->setSizes({1, 1});
        menu_toggle_nb->setChecked(true);
    } else {
        splitter->setSizes({0, 1});
        menu_toggle_nb->setChecked(false);
    }
}

void MainGUI::onToggleHud() {
    const bool on = !plotcanvas_->hudEnabled();
    plotcanvas_->setHudEnabled(on);
    hud_label->setStyleSheet(on ? "QLabel { color: black; background-color: mediumpurple; }" : "");
    inform(on ? tr_("HUD enabled.") : tr_("HUD disabled."));
}

void MainGUI::onToggleAxis() {
    const bool on = !plotcanvas_->axisEnabled();
    plotcanvas_->setAxisEnabled(on);
    axis_status_label->setStyleSheet(on ? "QLabel { color: black; background-color: orange; }" : "");
    inform(on ? tr_("Axis enabled.") : tr_("Axis disabled."));
}

void MainGUI::onToggleGridLines() {
    const bool on = !plotcanvas_->gridEnabled();
    plotcanvas_->setGridEnabled(on);
    inform(on ? tr_("Grid enabled.") : tr_("Grid disabled."));
}

void MainGUI::onTogglePlotArea() {
    QWidget* first = plot_tab_area->widget(0);
    if (!first) {
        plot_tab_area->addTab(plot_tab, tr_("Plot Area"));
        plot_tab_area->protectTab(0);
        return;
    }
    if (first->objectName() != "plotarea_tab") {
        plot_tab_area->insertTab(0, plot_tab, tr_("Plot Area"));
        plot_tab_area->protectTab(0);
    } else {
        plot_tab_area->closeTab(0);
    }
}

void MainGUI::onFullscreen(bool disable) {
    if (!fullscreen_ && !disable) {
        // Sin marco, ocupando la pantalla; se esconden las barras salvo las de la barra de estado.
        normalGeometry_ = geometry();
        setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
        if (QScreen* sc = screen()) setGeometry(sc->geometry().adjusted(-1, -1, 1, 1));
        show();
        hiddenToolbars_.clear();
        for (QToolBar* tb : findChildren<QToolBar*>()) {
            if (tb == coords_toolbar || tb == delta_coords_toolbar || tb == grid_toolbar || tb == status_toolbar) continue;
            if (tb->isVisible()) hiddenToolbars_.push_back(tb);
            tb->setVisible(false);
        }
        splitter->setSizes({0, 1});
        fullscreen_ = true;
    } else if (fullscreen_ || disable) {
        setWindowFlags(windowFlags() & ~Qt::FramelessWindowHint);
        setGeometry(normalGeometry_);
        showNormal();
        for (QToolBar* tb : hiddenToolbars_) tb->setVisible(true);  // restore_toolbar_view
        hiddenToolbars_.clear();
        fullscreen_ = false;
    }
}

void MainGUI::setUiTitle(const QString& name) {
    setWindowTitle(QString("FlatCAM 2024.4 BETA - 64bit - [3D]    %1").arg(name));
}

void MainGUI::updateCoordinates(QPointF w) {
    // App.on_mouse_move_over_plot: etiquetas de posición absoluta y relativa.
    const QPointF d = w - relPoint1_;
    position_label->setText(QString("&nbsp;<b>X</b>: %1&nbsp;&nbsp;   <b>Y</b>: %2&nbsp;")
                                .arg(w.x(), 0, 'f', 4)
                                .arg(w.y(), 0, 'f', 4));
    rel_position_label->setText(QString("<b>Dx</b>: %1&nbsp;&nbsp;  <b>Dy</b>: %2&nbsp;&nbsp;&nbsp;&nbsp;")
                                    .arg(d.x(), 0, 'f', 4)
                                    .arg(d.y(), 0, 'f', 4));
}

QPointF MainGUI::snapToGrid(const QPointF& p) const {
    if (!grid_snap_btn->isChecked()) return p;
    auto value = [](const QLineEdit* e) {
        bool ok = false;
        const double v = e->text().replace(',', '.').toDouble(&ok);
        return ok ? v : 0.0;
    };
    const double gx = value(grid_gap_x_entry);
    const double gy = grid_gap_link_cb->isChecked() ? gx : value(grid_gap_y_entry);
    auto snap = [](double v, double g) { return g != 0 ? std::nearbyint(v / g) * g : v; };
    return {snap(p.x(), gx), snap(p.y(), gy)};
}

void MainGUI::inform(const QString& msg) {
    // App.info: el nivel va entre corchetes al principio del mensaje.
    static const QRegularExpression re(R"(^\[([^\]]+)\]\s*(.*)$)", QRegularExpression::DotMatchesEverythingOption);
    const auto m = re.match(msg);
    if (m.hasMatch()) fcinfo->setStatus(m.captured(2), m.captured(1));
    else fcinfo->setStatus(msg, "info");
}

}  // namespace flatcam::gui
