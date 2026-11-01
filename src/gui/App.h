/**
 * @file App.h
 * @brief La aplicación: objetos del proyecto, apertura de archivos y dibujo (port de partes de `app_Main.App`,
 * `appHandlers/AppIO` y `appObjects/AppObject`).
 */
#pragma once

#include <QObject>
#include <QString>
#include <memory>

#include "gui/MainGUI.h"
#include "gui/objects/ObjectCollection.h"

namespace flatcam::gui {

/**
 * @brief Une la ventana principal con los objetos del proyecto.
 *
 * Abre Gerber y Excellon desde el menú Archivo, la barra de herramientas o arrastrándolos a la ventana; crea el objeto,
 * lo agrega al árbol del proyecto, lo dibuja con sus colores y encuadra la vista, con los mismos mensajes que el
 * original. Desvío consciente: el original lee los archivos en hilos de trabajo; acá se leen en el hilo de la
 * interfaz con el indicador de actividad encendido (el Gerber más grande del usuario tarda 0,6 s).
 */
class App : public QObject {
    Q_OBJECT
public:
    /** @brief Crea la aplicación con su ventana. @param parent Objeto padre. */
    explicit App(QObject* parent = nullptr);
    ~App() override;

    /** @brief La ventana principal. @return La ventana. */
    MainGUI* ui() const { return ui_.get(); }

    /** @brief Los objetos del proyecto. @return La colección. */
    ObjectCollection* collection() const { return collection_; }

    /**
     * @brief Abre un Gerber (`open_gerber`): lo lee, crea el objeto, lo dibuja y encuadra.
     * @param filename Ruta del archivo.
     * @return El objeto creado, o `nullptr` si falló (el motivo queda en la barra de estado).
     */
    FlatCAMObj* openGerber(const QString& filename);

    /**
     * @brief Abre un Excellon (`open_excellon`).
     * @param filename Ruta del archivo.
     * @return El objeto creado, o `nullptr` si falló.
     */
    FlatCAMObj* openExcellon(const QString& filename);

    /**
     * @brief Abre un archivo según su extensión, como al soltarlo sobre la ventana (`dropEvent` de `MainGUI`).
     * @param filename Ruta del archivo.
     * @return El objeto creado, o `nullptr` si la extensión no corresponde o falló.
     */
    FlatCAMObj* openByExtension(const QString& filename);

    /** @brief Pide los Gerber con el diálogo de archivos (`on_fileopengerber`). */
    void onFileOpenGerber();

    /** @brief Pide los Excellon con el diálogo de archivos (`on_fileopenexcellon`). */
    void onFileOpenExcellon();

    /** @brief Encuadra todos los objetos (`on_zoom_fit`). */
    void onZoomFit();

protected:
    /** @brief Atajos de teclado de la ventana y arrastrar y soltar archivos. @param obj Objeto. @param ev Evento. @return Si se consumió. */
    bool eventFilter(QObject* obj, QEvent* ev) override;

private:
    FlatCAMObj* onObjectCreated(std::unique_ptr<FlatCAMObj> obj);
    void connectSignals();
    QString lastFolder() const;

    std::unique_ptr<MainGUI> ui_;
    ObjectCollection* collection_;
    QString lastFolder_;
    QString lastGerberFilter_;
    QString lastExcellonFilter_;
};

}  // namespace flatcam::gui
