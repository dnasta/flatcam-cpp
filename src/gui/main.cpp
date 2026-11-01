// Punto de entrada de FlatCAM-cpp (equivale a FlatCAM.py del original).
//
// Uso: FlatCAM [archivos...]                       abre los Gerber y Excellon indicados (según la extensión)
//      FlatCAM --screenshot <png> [ancho alto] [archivos...]
//                                                  abre la ventana (y los archivos), guarda una captura y sale
#include <QApplication>
#include <QTimer>
#include <cstring>

#include "gui/App.h"

int main(int argc, char* argv[]) {
    // Qt saca "-style" de los argumentos al construir QApplication: hay que mirarlo antes.
    bool styleRequested = false;
    for (int i = 1; i < argc; ++i) styleRequested = styleRequested || std::strncmp(argv[i], "-style", 6) == 0;
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication qapp(argc, argv);
    QApplication::setOrganizationName("Open Source");
    QApplication::setApplicationName("FlatCAM");
    Q_INIT_RESOURCE(flatcam);
    // Desvío consciente: el original usa el estilo nativo (en Windows, "windowsvista"). Para que la interfaz se vea igual
    // que en Windows en las tres plataformas se usa Fusion, salvo que se pida otro con -style.
    if (!styleRequested) QApplication::setStyle("Fusion");

    flatcam::gui::App app;
    QStringList args = QApplication::arguments().mid(1);
    QString shotPath;
    const int shot = args.indexOf("--screenshot");
    if (shot >= 0 && shot + 1 < args.size()) {
        shotPath = args[shot + 1];
        int take = 2;
        bool okW = false, okH = false;
        if (shot + 3 < args.size()) {
            const int w = args[shot + 2].toInt(&okW), h = args[shot + 3].toInt(&okH);
            if (okW && okH) {
                app.ui()->resize(w, h);
                take = 4;
            }
        }
        args.erase(args.begin() + shot, args.begin() + shot + take);
    }
    app.ui()->show();
    for (const QString& f : args) app.openByExtension(f);
    if (!shotPath.isEmpty()) {
        QTimer::singleShot(1500, app.ui(), [&app, shotPath] {
            app.onZoomFit();
            QApplication::processEvents();
            app.ui()->grab().save(shotPath);
            QApplication::quit();
        });
    }
    return qapp.exec();
}
