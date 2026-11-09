// ARCHIVO GENERADO por tools/gui_dump/gen_cpp.py a partir de la interfaz original - no editar a mano.
#include "gui/generated/HowtoDialogUIBase.h"

#include <QKeySequence>
#include <QSpacerItem>

namespace flatcam::gui {

void HowtoDialogUIBase::setupUi() {
    // ---- acciones (compartidas entre menús y barras)
    this->setWindowTitle(QString::fromUtf8("Cómo ..."));
    auto* lay1 = new QVBoxLayout();
    lay1->setContentsMargins(11, 11, 11, 11);
    lay1->setSpacing(6);
    auto* lay2 = new QHBoxLayout();
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setSpacing(6);
    auto* w3 = new QTabWidget(nullptr);
    w3->tabBar()->setDrawBase(false);
    w3->tabBar()->setProperty("objectName", QString::fromUtf8("qt_tabwidget_tabbar"));
    intro_tab = new QWidget(nullptr);
    intro_tab_layout = new QHBoxLayout();
    intro_tab_layout->setContentsMargins(2, 2, 2, 2);
    intro_tab_layout->setSpacing(6);
    auto* w4 = new QScrollArea(nullptr);
    w4->setProperty("frameShape", 0);
    w4->setWidgetResizable(true);
    auto* w5 = new QWidget(nullptr);
    w5->setAutoFillBackground(true);
    grid_lay = new QGridLayout();
    grid_lay->setContentsMargins(9, 9, 9, 9);
    grid_lay->setHorizontalSpacing(20);
    grid_lay->setVerticalSpacing(6);
    auto* w6 = new QLabel(nullptr);
    w6->setPixmap(QPixmap(QString::fromUtf8(":/images/contribute256.png")));
    grid_lay->addWidget(w6, 0, 0, 1, 1);
    auto* w7 = new QLabel(nullptr);
    w7->setOpenExternalLinks(true);
    w7->setText(QString::fromUtf8("Este programa es <a href = 'https://opensource.org/'<b>Open Source</b></a> y gratuito en un sentido muy amplio de la palabra.<br>Sin embargo, no puede evolucionar sin <b> contribuciones </b>.<br><br>Si quieres ver esta aplicación crecer y ser mejor y mejor,<br>usted puede <b> contribuir </b> al desarrollo usted mismo:<br><ul><li> &nbsp;Solicitudes de extracción en el repositorio de Bitbucket, si es desarrollador <a href = 'https://bitbucket.org/jpcgt/flatcam/pull-requests/'<b>click</b></a></li><li> &nbsp;Informes de errores al proporcionar los pasos necesarios para reproducir el error <a href = 'https://bitbucket.org/jpcgt/flatcam/issues/new'<b>click</b></a></li></ul><br><br>Si te gusta lo que has visto hasta ahora ... <br><span style='color: blue;'>NO se requieren donaciones.</span> Pero ellos son bienvenidos <a href = 'https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=WLTJJ3Q77D98L&currency_code=USD&source=url'<b>click</b></a><br>"));
    grid_lay->addWidget(w7, 0, 1, 1, 1);
    grid_lay->setColumnStretch(1, 1);
    w5->setLayout(grid_lay);
    w4->setWidget(w5);
    intro_tab_layout->addWidget(w4, 0);
    intro_tab->setLayout(intro_tab_layout);
    w3->addTab(intro_tab, QString::fromUtf8("Contribuir"));
    links_tab = new QWidget(nullptr);
    links_tab->setHidden(true);
    links_tab_layout = new QVBoxLayout();
    links_tab_layout->setContentsMargins(2, 2, 2, 2);
    links_tab_layout->setSpacing(6);
    auto* w8 = new QScrollArea(nullptr);
    w8->setProperty("frameShape", 0);
    w8->setWidgetResizable(true);
    auto* w9 = new QWidget(nullptr);
    w9->setAutoFillBackground(true);
    links_lay = new QHBoxLayout();
    links_lay->setContentsMargins(9, 9, 9, 9);
    links_lay->setSpacing(6);
    auto* w10 = new QLabel(nullptr);
    w10->setText(QString::fromUtf8("Pronto ..."));
    links_lay->addWidget(w10, 0, Qt::Alignment(132));
    w9->setLayout(links_lay);
    w8->setWidget(w9);
    links_tab_layout->addWidget(w8, 0);
    links_tab->setLayout(links_tab_layout);
    w3->addTab(links_tab, QString::fromUtf8("Intercambio de enlaces"));
    howto_tab = new QWidget(nullptr);
    howto_tab->setHidden(true);
    howto_tab_layout = new QVBoxLayout();
    howto_tab_layout->setContentsMargins(2, 2, 2, 2);
    howto_tab_layout->setSpacing(6);
    auto* w11 = new QScrollArea(nullptr);
    w11->setProperty("frameShape", 0);
    w11->setWidgetResizable(true);
    auto* w12 = new QWidget(nullptr);
    w12->setAutoFillBackground(true);
    howto_lay = new QHBoxLayout();
    howto_lay->setContentsMargins(9, 9, 9, 9);
    howto_lay->setSpacing(6);
    auto* w13 = new QLabel(nullptr);
    w13->setText(QString::fromUtf8("Pronto ..."));
    howto_lay->addWidget(w13, 0, Qt::Alignment(132));
    w12->setLayout(howto_lay);
    w11->setWidget(w12);
    howto_tab_layout->addWidget(w11, 0);
    howto_tab->setLayout(howto_tab_layout);
    w3->addTab(howto_tab, QString::fromUtf8("How To's"));
    lay2->addWidget(w3, 0);
    lay1->addLayout(lay2, 0);
    auto* lay14 = new QHBoxLayout();
    lay14->setContentsMargins(0, 0, 0, 0);
    lay14->setSpacing(6);
    lay14->addItem(new QSpacerItem(0, 0, QSizePolicy::Policy(7), QSizePolicy::Policy(1)));
    auto* w15 = new QPushButton(nullptr);
    w15->setAutoDefault(true);
    w15->setText(QString::fromUtf8("Cerca"));
    lay14->addWidget(w15, 0);
    lay1->addLayout(lay14, 0);
    setLayout(lay1);
}

std::map<std::string, QObject*> HowtoDialogUIBase::membersByName() const {
    return {
        {"grid_lay", grid_lay},
        {"howto_lay", howto_lay},
        {"howto_tab", howto_tab},
        {"howto_tab_layout", howto_tab_layout},
        {"intro_tab", intro_tab},
        {"intro_tab_layout", intro_tab_layout},
        {"links_lay", links_lay},
        {"links_tab", links_tab},
        {"links_tab_layout", links_tab_layout},
    };
}

}  // namespace flatcam::gui
