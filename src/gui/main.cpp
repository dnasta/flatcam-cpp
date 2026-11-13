// Punto de entrada de FlatCAM-cpp (equivale a FlatCAM.py del original).
//
// Uso: FlatCAM [archivos...]                       abre los Gerber y Excellon indicados (según la extensión)
//      FlatCAM --screenshot <png> [ancho alto] [archivos...]
//                                                  abre la ventana (y los archivos), guarda una captura y sale
//      --select <nombre>   elige ese objeto en el árbol (muestra sus Propiedades)
//      --tool <ncc|paint|drill|cutout>   abre esa herramienta
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
    auto takeOption = [&args](const QString& name) {
        const int i = args.indexOf(name);
        if (i < 0 || i + 1 >= args.size()) return QString();
        const QString v = args[i + 1];
        args.erase(args.begin() + i, args.begin() + i + 2);
        return v;
    };
    const QString select = takeOption("--select"), tool = takeOption("--tool");
    app.ui()->show();
    for (const QString& f : args) {
        // on_startup_args: un script se abre en el editor y se ejecuta (".flatscript" o ".tcl" en el nombre).
        if (f.toLower().contains(".flatscript") || f.toLower().contains(".tcl")) {
            app.onFileOpenScript(f);
            app.ui()->plot_tab_area->setCurrentWidget(app.ui()->plot_tab);
            app.onFileRunScript(f);
        } else {
            app.openByExtension(f);
        }
    }
    if (!select.isEmpty()) {
        if (auto* o = app.collection()->byName(select)) {
            app.ui()->collection_view->setCurrentIndex(app.collection()->indexOf(o));
            app.ui()->notebook->setCurrentWidget(app.ui()->properties_tab);
        }
    }
    if (tool == "ncc") {
        app.runTool(app.nccTool, flatcam::gui::NccTool::info());
        app.nccTool->setToolUi();
    } else if (tool == "paint") {
        app.runTool(app.paintTool, flatcam::gui::PaintTool::info());
        app.paintTool->setToolUi();
    }
    else if (tool == "drill") {
        app.runTool(app.drillingTool, flatcam::gui::DrillingTool::info());
        app.drillingTool->setToolUi();
    }
    else if (tool == "iso") {
        app.runTool(app.isolationTool, flatcam::gui::IsolationTool::info());
        app.isolationTool->setToolUi();
    } else if (tool == "2sided") {
        app.runTool(app.dblsidedTool, flatcam::gui::DblSidedTool::info());
        app.dblsidedTool->setToolUi();
    } else if (tool == "cutout") {
        app.runTool(app.cutoutTool, flatcam::gui::CutOutTool::info());
        app.cutoutTool->setToolUi();
    }
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
