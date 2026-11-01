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
#include "gui/tools/ToolPanels.h"

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

    /**
     * @brief Muestra en la pestaña Propiedades el panel del objeto elegido, o el panel vacío si no hay ninguno
     * (`on_list_selection_change` + `build_ui` / `setup_default_properties_tab`).
     * @param obj Objeto, o `nullptr`.
     */
    void showProperties(FlatCAMObj* obj);

    /**
     * @brief Abre una herramienta en la pestaña Herramienta (`AppTool.run` y el `run` de cada herramienta).
     *
     * Si el panel izquierdo está cerrado lo abre; si la misma herramienta ya está a la vista en su pestaña, cierra el panel
     * (como el original con `toggle`). Pone el panel, le da el nombre de la herramienta y cambia el título de la pestaña.
     * @param panel Panel de la herramienta.
     * @param info Nombre y título.
     * @param toggle Si es `true` se comporta como el botón (abre o cierra).
     */
    void runTool(QWidget* panel, const ToolInfo& info, bool toggle = true);

    /**
     * @brief Agrega un objeto creado por una herramienta (`new_object` → `on_object_created`): al árbol, lo dibuja y
     * encuadra.
     * @param obj Objeto.
     * @return El objeto agregado.
     */
    FlatCAMObj* addObject(std::unique_ptr<FlatCAMObj> obj);

    /** @brief Objetos elegidos en el árbol, en su orden (`collection.get_selected`). @return Los objetos. */
    std::vector<FlatCAMObj*> selectedObjects() const;

    /**
     * @brief Refleja los objetos elegidos alrededor del centro de la caja que los contiene a todos (`on_flipx` con
     * `Axis::Y`: invierte las X; `on_flipy` con `Axis::X`: invierte las Y).
     * @param axis Eje del reflejo.
     * @return `true` si había objetos y se reflejaron.
     */
    bool flip(Axis axis);

    /** @brief Objeto elegido en el árbol (`collection.get_active`). @return El objeto, o `nullptr`. */
    FlatCAMObj* activeObject() const;

    /**
     * @brief Guarda el G-code de un CNC Job (`on_exportgcode_button_click` → `export_gcode_handler`).
     * @param job Objeto.
     * @param filename Archivo; si está vacío se pregunta con el diálogo de guardar.
     * @return `true` si se guardó.
     */
    bool exportGcode(class CNCJobObject* job, QString filename = QString());

    /**
     * @brief Genera el CNC Job de una Geometría (`on_generatecnc_button_click` → `mtool_gen_cncjob`) y lo agrega.
     * @param geo Geometría.
     * @param toolIds Herramientas elegidas.
     * @return El objeto creado, o `nullptr` si falló (el motivo queda en la barra de estado).
     */
    FlatCAMObj* generateCncJob(class GeometryObject* geo, const std::vector<int>& toolIds);

    NccTool* nccTool;            ///< Panel de NCC.
    PaintTool* paintTool;        ///< Panel de pintura.
    DrillingTool* drillingTool;  ///< Panel de perforación.
    CutOutTool* cutoutTool;      ///< Panel de corte.
    DblSidedTool* dblsidedTool;  ///< Panel de PCB a 2 caras.

    /** @brief Posición del último clic izquierdo en el canvas (`app.pos`). @return La posición en mm. */
    QPointF lastClickPosition() const { return lastClick_; }

    /**
     * @brief Un objeto cambió su geometría (`object_changed` + `plot`): se vuelve a dibujar y, si está a la vista, se
     * recargan sus Propiedades.
     * @param obj Objeto.
     */
    void objectChanged(FlatCAMObj* obj);

protected:
    /** @brief Atajos de teclado de la ventana y arrastrar y soltar archivos. @param obj Objeto. @param ev Evento. @return Si se consumió. */
    bool eventFilter(QObject* obj, QEvent* ev) override;

private:
    void connectSignals();
    bool handleKey(int key, Qt::KeyboardModifiers modifiers);
    QString lastFolder() const;

    std::unique_ptr<MainGUI> ui_;
    QWidget* defaultProperties_{nullptr};  ///< Panel de Propiedades sin objeto (`setup_default_properties_tab`).
    ObjectCollection* collection_;
    QString lastFolder_;
    QPointF lastClick_{0.0, 0.0};
    QString lastGerberFilter_;
    QString lastExcellonFilter_;
};

}  // namespace flatcam::gui
