// Punto de entrada de FlatCAM-cpp (equivale a FlatCAM.py del original).
//
// Opciones de línea de comandos (propias del port, para pruebas):
//   --screenshot <archivo.png> [ancho alto]   abre la ventana, guarda una captura y sale.
#include <QApplication>
#include <cstring>
#include <QTimer>

#include "gui/MainGUI.h"

int main(int argc, char* argv[]) {
    // Qt saca "-style" de los argumentos al construir QApplication: hay que mirarlo antes.
    bool styleRequested = false;
    for (int i = 1; i < argc; ++i) styleRequested = styleRequested || std::strncmp(argv[i], "-style", 6) == 0;
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication app(argc, argv);
    QApplication::setOrganizationName("Open Source");
    QApplication::setApplicationName("FlatCAM");
    Q_INIT_RESOURCE(flatcam);
    // Desvío consciente: el original usa el estilo nativo (en Windows, "windowsvista"). Para que la interfaz se vea igual
    // que en Windows en las tres plataformas se usa Fusion, salvo que se pida otro con -style.
    if (!styleRequested) QApplication::setStyle("Fusion");

    flatcam::gui::MainGUI window;
    const QStringList args = QApplication::arguments();
    const int shot = args.indexOf("--screenshot");
    if (shot >= 0 && shot + 1 < args.size()) {
        if (shot + 3 < args.size()) window.resize(args[shot + 2].toInt(), args[shot + 3].toInt());
        window.show();
        const QString path = args[shot + 1];
        QTimer::singleShot(1500, &window, [&window, path] {
            window.grab().save(path);
            QApplication::quit();
        });
        return app.exec();
    }
    window.show();
    return app.exec();
}
