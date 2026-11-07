/**
 * @file App.h
 * @brief La aplicación: objetos del proyecto, apertura de archivos y dibujo (port de partes de `app_Main.App`,
 * `appHandlers/AppIO` y `appObjects/AppObject`).
 */
#pragma once

#include <functional>

#include "io/JsonValue.h"

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

    /**
     * @brief Abre un proyecto de FlatCAM (`open_project`): borra el proyecto actual y crea sus objetos.
     * @param filename Ruta del `.FlatPrj` (comprimido o no).
     * @return `true` si se pudo leer (los objetos que no se pudieron crear quedan avisados).
     */
    bool openProject(const QString& filename);

    /** @brief Pide el proyecto con el diálogo de archivos (`on_file_openproject`). */
    void onFileOpenProject();

    /**
     * @brief Guarda el proyecto (`save_project`): todos los objetos y las opciones, comprimido con XZ como el original
     * (`global_save_compressed`, nivel `global_compression_level`).
     * @param filename Ruta del `.FlatPrj`.
     * @return `true` si se pudo escribir.
     */
    bool saveProject(const QString& filename);

    /** @brief Guarda en el archivo del proyecto, o pregunta dónde si no tiene (`on_file_saveproject`). */
    void onFileSaveProject();

    /** @brief Pregunta dónde guardar el proyecto (`on_file_saveprojectas`). */
    void onFileSaveProjectAs();

    /**
     * @brief Borra los objetos elegidos (`on_delete`).
     * @param force Si es `false`, pregunta antes (`global_delete_confirmation`).
     * @return `true` si se borró algo.
     */
    bool deleteSelected(bool force = false);

    /** @brief Copia los Gerber, Excellon y Geometrías elegidos como `<nombre>_copy` (`on_copy_command`). @return Las copias. */
    std::vector<FlatCAMObj*> copySelected();

    /** @brief Muestra u oculta los objetos elegidos (`on_enable_sel_plots` / `on_disable_sel_plots`). @param on Mostrar. */
    void setSelectedPlots(bool on);

    /**
     * @brief Cambia el color de los objetos elegidos (`on_set_color_action_triggered`).
     * @param action `Red`, `Blue`, `Yellow`, `Green`, `Purple`, `Brown`, `White`, `Black`, `Custom`, `Opacity` o `Default`.
     */
    void setSelectedColor(const QString& action);

    /** @brief Muestra el texto fuente del objeto en una pestaña del área de dibujo (`on_view_source`). @param obj Objeto. */
    void viewSource(FlatCAMObj* obj);

    /** @brief Guarda el objeto activo según su tipo (`on_project_context_save`). */
    void saveSelected();

    /** @brief Ajusta las acciones del menú contextual a lo elegido (`on_menu_request`). */
    void updateProjectMenu();

    QAction* menuProjectEnable{nullptr};       ///< "Habilitar Parcela".
    QAction* menuProjectDisable{nullptr};      ///< "Desactivar parcela".
    QMenu* menuProjectColor{nullptr};          ///< Submenú "Establecer color".
    QAction* menuProjectGenerateCnc{nullptr};  ///< "Crear CNCJob".
    QAction* menuProjectViewSource{nullptr};   ///< "Ver fuente".
    QAction* menuProjectEdit{nullptr};         ///< "Editar" (los editores no están en el port).
    QAction* menuProjectCopy{nullptr};         ///< "Copiar".
    QAction* menuProjectDelete{nullptr};       ///< "Borrar".
    QAction* menuProjectSave{nullptr};         ///< "Guardar".
    QAction* menuProjectProperties{nullptr};   ///< "Propiedades".

    /**
     * @brief Carpeta de datos de la aplicación (`data_path`): la de la aplicación en el sistema, salvo que se cambie.
     * @return La ruta.
     */
    QString dataPath() const;

    /** @brief Cambia la carpeta de datos (para los tests) y vuelve a armar los recientes. @param path Ruta. */
    void setDataPath(const QString& path);

    /**
     * @brief Anota un archivo abierto en los recientes (`register_recent`): `recent_projects.json` para los proyectos y
     * `recent.json` para el resto, los últimos 10 (`global_recent_limit`), y rearma los menús.
     * @param kind `project`, `gerber`, `excellon`...
     * @param filename Ruta.
     */
    void registerRecent(const QString& kind, const QString& filename);

    /** @brief Arma los menús Archivo → Proyectos recientes y Archivos recientes (`setup_recent_items`). */
    void setupRecentItems();

    /** @brief Quita todos los objetos (`on_file_new` sin preguntar). */
    void clearProject();

    /** @brief Archivo del proyecto abierto o guardado (`project_filename`), vacío si no hay. @return La ruta. */
    const QString& projectFilename() const { return projectFilename_; }

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

    /**
     * @brief Lleva el cursor a un punto (`on_jump_to` con `custom_location`): mueve el puntero, opcionalmente centra la
     * vista y actualiza las etiquetas de posición de la barra de estado.
     * @param location Punto en mm.
     * @param fitCenter Si se centra la vista en un cuadrado de 40 mm alrededor del punto.
     */
    void jumpTo(const QPointF& location, bool fitCenter = true);

    /** @brief Opciones de la aplicación: las del proyecto abierto o, si no hay, las de fábrica. @return Las opciones. */
    const JsonValue& appOptions() const;

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
     * @brief Abre la pestaña "Base de datos de herramientas" (`on_tools_database`); si ya está abierta no hace nada.
     * @param onToolRequest Qué hacer con la herramienta que se transfiere (la herramienta que la abrió); vacío: la
     *        Geometría activa (`on_geometry_tool_add_from_db_executed`).
     * @return La pestaña, o `nullptr` si no se pudo leer la base.
     */
    class ToolsDBTab* openToolsDatabase(std::function<void(const JsonValue&)> onToolRequest = {});
    /** @brief Cierra la pestaña de la base, si está abierta. */
    void closeToolsDatabase();

    /**
     * @brief Genera el CNC Job de una Geometría (`on_generatecnc_button_click` → `mtool_gen_cncjob`) y lo agrega.
     * @param geo Geometría.
     * @param toolIds Herramientas elegidas.
     * @param polish Panel de la Geometría si hay que agregar el pulido ("Agregar acabado"), o nulo.
     * @return El objeto creado, o `nullptr` si falló (el motivo queda en la barra de estado).
     */
    FlatCAMObj* generateCncJob(class GeometryObject* geo, const std::vector<int>& toolIds,
                               class GeometryObjectUI* polish = nullptr);

    NccTool* nccTool;            ///< Panel de NCC.
    PaintTool* paintTool;        ///< Panel de pintura.
    DrillingTool* drillingTool;  ///< Panel de perforación.
    CutOutTool* cutoutTool;      ///< Panel de corte.
    DblSidedTool* dblsidedTool;  ///< Panel de PCB a 2 caras.
    class TransformTool* transformTool;  ///< Panel de Transformaciones.
    class DistanceTool* distanceTool;  ///< Herramienta Distancia.
    class DistanceMinTool* distanceMinTool;  ///< Herramienta Distancia mínima.
    class MoveTool* moveTool;  ///< Herramienta Mover (sin panel).
    class PropertiesTool* propertiesTool;  ///< Herramienta Propiedades.
    IsolationTool* isolationTool;  ///< Panel de aislamiento.

    /** @brief Selección interactiva de zonas y polígonos en el canvas. @return El selector. */
    class CanvasPicker* picker() const { return picker_; }
    /** @brief Áreas de exclusión de la aplicación (`app.exc_areas`). @return El almacén. */
    class ExclusionAreasManager* excAreas() const { return excAreas_; }

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
    void setupProjectMenu();
    bool handleKey(int key, Qt::KeyboardModifiers modifiers);
    /** @brief Conecta la escala, el desplazamiento y "Transformaciones" de un panel de Propiedades a su objeto. */
    template <typename Panel>
    void connectTransformations(Panel* p, FlatCAMObj* obj);
    QString lastFolder() const;

    std::unique_ptr<MainGUI> ui_;
    QWidget* defaultProperties_{nullptr};  ///< Panel de Propiedades sin objeto (`setup_default_properties_tab`).
    ObjectCollection* collection_;
    QString lastFolder_;
    QString projectFilename_;
    QString dataPath_;
    class CanvasPicker* picker_{nullptr};
    class ExclusionAreasManager* excAreas_{nullptr};  ///< Áreas de exclusión.  ///< Carpeta de datos elegida (vacía: la de la aplicación).
    class QMenu* menuProject_{nullptr};  ///< `menuproject`: menú contextual de la pestaña Proyecto.
    JsonValue projectOptions_;  ///< Opciones del proyecto abierto (`options`), para volver a guardarlas.
    QPointF lastClick_{0.0, 0.0};
    QString lastGerberFilter_;
    QString lastExcellonFilter_;
};

}  // namespace flatcam::gui
