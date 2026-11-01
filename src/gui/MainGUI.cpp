#include "gui/MainGUI.h"

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
    connect(plotcanvas_, &PlotCanvas::mouseMoved, this, [this](QPointF w) { updateCoordinates(w); });
    connect(plotcanvas_, &PlotCanvas::mouseClicked, this, [this](QPointF w, Qt::MouseButton b) {
        if (b == Qt::LeftButton) relPoint1_ = w;
    });
    // Botones de la barra de estado: HUD y ejes (on_toggle_hud / on_toggle_axis cambian además su estilo).
    connect(hud_label, &FCLabel::clicked, this, [this] {
        const bool on = !plotcanvas_->hudEnabled();
        plotcanvas_->setHudEnabled(on);
        hud_label->setStyleSheet(on ? "QLabel { color: black; background-color: mediumpurple; }" : "");
        inform(on ? tr_("HUD enabled.") : tr_("HUD disabled."));
    });
    connect(axis_status_label, &FCLabel::clicked, this, [this] {
        const bool on = !plotcanvas_->axisEnabled();
        plotcanvas_->setAxisEnabled(on);
        axis_status_label->setStyleSheet(on ? "QLabel { color: black; background-color: orange; }" : "");
        inform(on ? tr_("Axis enabled.") : tr_("Axis disabled."));
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

void MainGUI::inform(const QString& msg) {
    // App.info: el nivel va entre corchetes al principio del mensaje.
    static const QRegularExpression re(R"(^\[([^\]]+)\]\s*(.*)$)", QRegularExpression::DotMatchesEverythingOption);
    const auto m = re.match(msg);
    if (m.hasMatch()) fcinfo->setStatus(m.captured(2), m.captured(1));
    else fcinfo->setStatus(msg, "info");
}

}  // namespace flatcam::gui
