/**
 * @file App.h
 * @brief La aplicación: objetos del proyecto, apertura de archivos y dibujo (port de partes de `app_Main.App`,
 * `appHandlers/AppIO` y `appObjects/AppObject`).
 */
#pragma once

#include "gui/PlotCanvas.h"

#include <vector>

#include <set>

#include <optional>

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
     * @brief Abre un G-code como CNC Job (`open_gcode`): el texto tal cual (al exportarlo se le agrega la cabecera).
     * @param filename Archivo. @return El objeto, o nulo si no es G-code (una línea con '%', MOIN o MOMM).
     */
    FlatCAMObj* openGcode(const QString& filename);
    /** @brief Archivo -> Abrir G-Code (`on_fileopengcode`). */
    void onFileOpenGcode();

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
    /** @brief `collection.delete_by_name`: borra un objeto sin preguntar. @param name Nombre. @return Si estaba. */
    bool deleteByName(const QString& name);

    /** @name Editores (AppEditors.cpp) */
    ///@{
    /**
     * @brief `object2editor`: abre el objeto elegido en su editor (Geometría, Excellon o Gerber): la pestaña
     * Propiedades pasa a llamarse "Editor" (en rojo), la de Proyecto se deshabilita y se esconde la barra de herramientas.
     */
    void objectToEditor();
    /**
     * @brief `editor2object`: sale del editor; pregunta si se guarda lo editado.
     * @param save La respuesta (vacía: la pregunta con un diálogo). @param cleanup Salir sin preguntar ni guardar.
     * @return `false` si no había un editor abierto o se canceló.
     */
    bool editorToObject(std::optional<bool> save = std::nullopt, bool cleanup = false);
    /** @brief Si hay un editor abierto (`call_source != 'app'`). @return `true` si lo hay. */
    bool editorActive() const;
    /** @brief `call_source`: `app`, `geo_editor`, `exc_editor` o `grb_editor`. @return El valor. */
    const QString& callSource() const { return callSource_; }
    /** @brief Pide borrar el objeto editado al salir del editor (un Gerber o Excellon que quedó vacío). */
    void requestDeleteEditedObject() { pendingDelete_ = true; }
    class GeoEditor* geoEditor{nullptr};  ///< `geo_editor`.
    class ExcEditor* excEditor{nullptr};  ///< `exc_editor`.
    class GrbEditor* grbEditor{nullptr};  ///< `grb_editor`.
    ///@}

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

    /** @brief Opciones de la aplicación: las del proyecto abierto o, si no hay, las preferencias. @return Las opciones. */
    const JsonValue& appOptions() const;
    /**
     * @brief `on_toggle_units`: pasa la aplicación a las unidades elegidas en la pestaña (pregunta antes), convierte
     * todos los objetos y, salvo con @p noPref, escala las preferencias con dimensiones.
     * @param noPref Sin escalar las preferencias (la tecla Q).
     */
    void onToggleUnits(bool noPref = false);
    /** @brief `on_toggle_units_click`: tecla Q y Editar -> Cambiar unidades. */
    void onToggleUnitsClick();
    /**
     * @brief Archivo -> Copia de seguridad -> Importar preferencias (`on_import_preferences`): carga un `.FlatConfig`
     * sobre las opciones (como al arrancar, una de otra versión se descarta) y marca las preferencias como editadas.
     * @param filename Archivo; vacío abre el diálogo.
     */
    void onImportPreferences(QString filename = {});
    /**
     * @brief Archivo -> Copia de seguridad -> Exportar preferencias (`on_export_preferences`): lee la pestaña y guarda las
     * opciones en un `.FlatConfig`.
     * @param filename Archivo; vacío abre el diálogo (en la carpeta de datos, `preferences_<fecha>`).
     * @return Si se guardó.
     */
    bool onExportPreferences(QString filename = {});
    /**
     * @brief "Fresar agujeros" / "Fresar ranuras" del panel de un Excellon (`generate_milling_drills` / `_slots`): una
     * Geometría `<nombre>_mill` o `<nombre>_mill_slot` con el recorrido de la fresa.
     * @param exc El Excellon. @param tools Herramientas elegidas. @param tooldia Diámetro de la fresa.
     * @param slots Ranuras en vez de agujeros. @return El objeto nuevo, o `nullptr` si falla.
     */
    FlatCAMObj* millExcellon(ExcellonObject* exc, const std::vector<int>& tools, double tooldia, bool slots);
    /** @brief `set_screen_units`: el rótulo de unidades de la barra de estado. @param units "MM" o "IN". */
    void setScreenUnits(const QString& units);
    /** @brief En los tests: respuesta al aviso de cambiar las unidades. @param a `true` = Aceptar. */
    void setUnitsAutoAnswer(std::optional<bool> a) { unitsAutoAnswer_ = a; }
    /** @brief Las preferencias (`app.defaults` y la pestaña Preferencias). @return El gestor. */
    class Preferences* preferences() const { return prefs_; }
    /** @brief Las opciones de la aplicación (`app.defaults`). @return Las opciones. */
    const JsonValue& defaults() const;

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
    PanelizeTool* panelizeTool;  ///< Panel de Panelizar.
    InvertTool* invertTool;      ///< Panel de Invertir Gerber.
    CalculatorsTool* calculatorsTool;  ///< Panel de Calculadoras.
    CornersTool* cornersTool;          ///< Panel de Marcas de esquina.
    ExtractDrillsTool* extractDrillsTool;  ///< Panel de Extraer taladros.
    PunchTool* punchTool;                  ///< Panel de Perforar Gerber.
    SubTool* subTool;                      ///< Panel de Restar.
    OptimalTool* optimalTool;              ///< Panel de Óptimo.
    EtchTool* etchTool;                    ///< Panel de Compensación de grabado.
    AlignTool* alignTool;                  ///< Panel de Alinear objetos.
    FiducialsTool* fiducialsTool;          ///< Panel de Fiduciales.
    ThievingTool* thievingTool;            ///< Panel de Robo de cobre.
    RulesTool* rulesTool;                  ///< Panel de Verificación de reglas.
    SolderPasteTool* solderPasteTool;      ///< Panel de Pasta de soldar.
    QRCodeTool* qrcodeTool;                ///< Panel del código QR.
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

    /** @name Selección con el mouse en el canvas (AppCanvasSelection.cpp; públicos para los tests) */
    ///@{
    /**
     * @brief `on_mouse_click_over_plot`: guarda la posición del clic (`app.pos`). @param world Posición (ajustada a la
     * grilla). @param button Botón.
     */
    void onCanvasPress(const QPointF& world, Qt::MouseButton button);
    /**
     * @brief `on_mouse_move_over_plot`: con el botón izquierdo apretado dibuja el rectángulo de selección (azul de
     * izquierda a derecha, que elige lo que queda adentro; verde de derecha a izquierda, que elige lo que toca).
     * @param world Posición. @param leftDragging Si se mueve con el botón izquierdo apretado (`event.is_dragging`).
     */
    void onCanvasMove(const QPointF& world, bool leftDragging);
    /**
     * @brief `on_mouse_click_release_over_plot`: Shift copia las coordenadas del clic; un doble clic abre Propiedades;
     * un rectángulo elige por área; un clic elige el objeto que está debajo (Ctrl: suma a la selección).
     * @param world Posición. @param button Botón. @param modifiers Teclas apretadas.
     * @param dragged Si el botón derecho arrastró (desplazar la vista): entonces no se abre el menú del canvas.
     */
    void onCanvasRelease(const QPointF& world, Qt::MouseButton button, Qt::KeyboardModifiers modifiers,
                         bool dragged = false);
    /** @brief `on_mouse_double_click_over_plot`. @param button Botón. */
    void onCanvasDoubleClick(Qt::MouseButton button);
    /** @brief `delete_selection_shape`: borra los recuadros de selección. */
    void deleteSelectionShape();
    /** @brief `draw_selection_shape`: el recuadro alrededor de un objeto. @param obj Objeto. */
    void drawSelectionShape(FlatCAMObj* obj);
    /** @brief Capa auxiliar de los recuadros de selección (la `sel_shapes` de `ToolMove`). */
    static constexpr int kSelectionLayerId = -4;
    ///@}

    /** @name Menús Archivo, Editar y Ver y el menú del canvas (AppEditMenu.cpp) */
    ///@{
    /** @brief Archivo -> Nuevo proyecto (`on_file_new_click`): si hay objetos pregunta si guardar. */
    void onFileNewClick();
    /** @brief Vuelve al estado inicial (`on_file_new`): sin objetos, sin proyecto, solo el área de dibujo. */
    void onFileNew();
    /** @brief Muestra los objetos (`enable_plots`). @param objs Objetos. */
    void enablePlots(const std::vector<FlatCAMObj*>& objs);
    /** @brief Oculta los objetos (`disable_plots`). @param objs Objetos. */
    void disablePlots(const std::vector<FlatCAMObj*>& objs);
    /** @brief Invierte la visibilidad de los elegidos (`toggle_plots`). */
    void togglePlots();
    /** @brief Objetos que no están elegidos (`get_non_selected`). @return Los objetos. */
    std::vector<FlatCAMObj*> nonSelectedObjects() const;
    /** @brief Vuelve a dibujar todo y encuadra (`plot_all`). */
    void plotAll();
    /** @brief Borra los dibujos de todos los objetos (`clear_plots`); siguen en el Proyecto. */
    void clearPlots();
    /** @brief Muestra u oculta el contorno de la hoja (`on_workspace_toggle`, A4 vertical de fábrica). */
    void onWorkspaceToggle();
    /** @brief El cursor según las preferencias (`on_cursor_type`, `cursor_color_3D`). */
    void applyCursorPreferences();
    /** @brief `on_workspace`: dibuja o borra la hoja según la casilla de Preferencias y lee la pestaña. */
    void onWorkspace();
    /** @brief `on_workspace_modified`: cambió el tamaño o la orientación de la hoja. */
    void onWorkspaceModified();
    /** @brief `draw_workspace`: dibuja la hoja `global_workspaceT` con su orientación. */
    void drawWorkspace();
    /** @brief `delete_workspace`. */
    void deleteWorkspace();
    /**
     * @brief Tamaño de una hoja (`pagesize_dict`).
     * @param name "A4", "LETTER"... @return Ancho y alto en mm, o vacío si no está.
     */
    static std::optional<std::pair<double, double>> pageSize(const QString& name);
    /** @brief Elige todos los objetos con su recuadro (`on_selectall`). */
    void onSelectAll();
    /** @brief Lleva los elegidos al origen (`on_move2origin`): la esquina inferior izquierda de su caja va a (0, 0). */
    void onMove2Origin();
    /** @brief Espera un clic y lo convierte en el origen (`on_set_origin`): mueve todos los objetos. */
    void onSetOrigin();
    /** @brief Mueve todos los objetos para que el punto pase a ser el origen (`on_set_zero_click`). @param p Punto. */
    void setOriginAt(const QPointF& p);
    /** @brief Editar -> Saltar a (`on_jump_to`): pide la posición con `DialogBoxRadio`. */
    void onJumpTo();
    /** @brief Editar -> Ubicar (`on_locate`): una esquina o el centro del objeto activo. */
    void onLocate();
    /** @brief Editar -> Unir Geo/Gerber/Exc -> Geo (`on_edit_join`). @return La Geometría nueva, o nulo si falló. */
    FlatCAMObj* joinGeometry();
    /** @brief Editar -> Unir Exc -> Exc (`on_edit_join_exc`). @return El Excellon nuevo, o nulo si falló. */
    FlatCAMObj* joinExcellon();
    /** @brief Editar -> Unir Gerber -> Gerber (`on_edit_join_grb`). @return El Gerber nuevo, o nulo si falló. */
    FlatCAMObj* joinGerber();
    /** @brief Editar -> Convertir -> a Geometría (`convert_any2geo`), un `<nombre>_conv` por objeto. @return Los nuevos. */
    std::vector<FlatCAMObj*> convertToGeometry();
    /** @brief Editar -> Convertir -> a Gerber (`convert_any2gerber`): Geometría o Excellon. @return Los nuevos. */
    std::vector<FlatCAMObj*> convertToGerber();
    /** @brief Editar -> Convertir -> a Excellon (`convert_any2excellon`): Gerber o Geometría. @return Los nuevos. */
    std::vector<FlatCAMObj*> convertToExcellon();
    /**
     * @brief Opciones -> Girar la selección (`on_rotate`): alrededor del centro de la caja de los elegidos.
     * @param preset Ángulo (sin él, se pide con `FCInputDoubleSpinner`).
     */
    void onRotateSelection(std::optional<double> preset = std::nullopt);
    /**
     * @brief Opciones -> Inclinar en X / Y (`on_skewx`, `on_skewy`): desde la esquina inferior izquierda de los elegidos.
     * @param axis `'X'` o `'Y'`. @param preset Ángulo (sin él, se pide con `FCInputDoubleSpinner`).
     */
    void onSkewSelection(char axis, std::optional<double> preset = std::nullopt);
    /** @brief El diálogo Acerca de (`on_about`), sin mostrar. @return El diálogo. */
    std::unique_ptr<class QDialog> aboutDialog();
    /** @brief El diálogo "Cómo empezar" (`on_howto`), sin mostrar. @return El diálogo. */
    std::unique_ptr<class QDialog> howtoDialog();
    /** @brief La pestaña con la lista de atajos (`ShortcutsTab`). @return La pestaña. */
    QWidget* shortcutsTab();
    /** @brief Ayuda -> Lista de atajos (`on_shortcut_list`, F3): agrega la pestaña al área de dibujo. */
    void onShortcutList();
    /** @brief Abre en el navegador el manual (0, F1) o los videos (1, F4). @param which Cuál. */
    void openHelpLink(int which);
    /**
     * @brief `final_save`: antes de salir, si hay cambios sin guardar pregunta si se guarda el proyecto.
     * @return `true` si se puede salir (se guardó, se eligió "No" o no había cambios); `false` con "Cancelar".
     */
    bool finalSave();
    /** @brief Si hay cambios sin guardar (`should_we_save`). @return El estado. */
    bool shouldWeSave() const { return shouldWeSave_; }
    /**
     * @brief Texto exportado de un Gerber o Excellon (`export_gerber` / `export_excellon` con los formatos
     * `gerber_exp_*` / `excellon_exp_*`). @param obj Objeto. @return El texto, o vacío.
     */
    QString exportedText(FlatCAMObj* obj) const;
    /** @brief Regenera el texto fuente de un Gerber o Excellon movido, como el original (`local_use`). @param obj Objeto. */
    void refreshSourceFile(FlatCAMObj* obj);
    /** @brief La última carpeta usada para guardar (`get_last_save_folder`). @return La carpeta. */
    QString lastFolder() const;
    /** @brief Exporta un Gerber o Excellon a un archivo. @param obj Objeto. @param filename Archivo. @return Si se escribió. */
    bool exportObject(FlatCAMObj* obj, const QString& filename);
    /** @brief Archivo -> Exportar -> Gerber / Excellon (`on_file_exportgerber`, `on_file_exportexcellon`). @param kind Tipo. */
    void onFileExport(ObjectKind kind);
    /** @brief Exporta un objeto a SVG (`export_svg`). @param obj Objeto. @param filename Archivo. @return Si se escribió. */
    bool exportSvgFile(FlatCAMObj* obj, const QString& filename);
    /** @brief Archivo -> Exportar -> SVG (`on_file_exportsvg`). */
    void onFileExportSvg();
    /** @brief Exporta una Geometría a DXF (`export_dxf`). @param geo Geometría. @param filename Archivo. @return Si se escribió. */
    bool exportDxfFile(class GeometryObject* geo, const QString& filename);
    /** @brief Archivo -> Exportar -> DXF (`on_file_exportdxf`); también "Guardar" de una Geometría. */
    void onFileExportDxf();
    /** @brief Guarda lo que muestra el canvas en un PNG, sin transparencia. @param filename Archivo. @return Si se escribió. */
    bool exportPngFile(const QString& filename);
    /** @brief Archivo -> Exportar -> PNG (`on_file_exportpng`). */
    void onFileExportPng();
    /**
     * @brief Importa un SVG (`import_svg`) como Geometría o como Gerber. @param filename Archivo.
     * @param asGerber Como Gerber. @return El objeto, o nulo si falló.
     */
    FlatCAMObj* importSvg(const QString& filename, bool asGerber);
    /** @brief Archivo -> Importar -> SVG (`on_file_importsvg`). @param asGerber Como Gerber. */
    void onFileImportSvg(bool asGerber);
    /**
     * @brief Importa un DXF (`import_dxf`) como Geometría o como Gerber. @param filename Archivo.
     * @param asGerber Como Gerber. @return El objeto, o nulo si falló.
     */
    FlatCAMObj* importDxf(const QString& filename, bool asGerber);
    /** @brief Archivo -> Importar -> DXF (`on_file_importdxf`). @param asGerber Como Gerber. */
    void onFileImportDxf(bool asGerber);
    /** @brief Abre un HPGL2 como Geometría (`open_hpgl2`). @param filename Archivo. @return El objeto, o nulo. */
    FlatCAMObj* openHpgl2(const QString& filename);
    /** @brief Archivo -> Importar -> HPGL2 (`on_fileopenhpgl2`). */
    void onFileOpenHpgl2();
    /** @brief Arma y abre el menú del canvas (`popMenu`) donde está el cursor. */
    void popupCanvasMenu();
    /** @brief El menú del canvas (`popMenu`). @return El menú. */
    class QMenu* canvasMenu() const { return popMenu_; }
    ///@}

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

    std::unique_ptr<MainGUI> ui_;
    QWidget* defaultProperties_{nullptr};  ///< Panel de Propiedades sin objeto (`setup_default_properties_tab`).
    ObjectCollection* collection_;
    QString lastFolder_;
    QString projectFilename_;
    QString dataPath_;
    class CanvasPicker* picker_{nullptr};
    /** @brief `scale_defaults`: escala las opciones con dimensiones. @param factor 25,4 o 1/25,4. */
    void scaleDefaults(double factor);
    /** @brief `float(v)` de Python, o @p def. @param v Valor. @param def Si no es un número. @return El número. */
    static double toFloatOr(const JsonValue& v, double def = 0.0);
    bool toggleUnitsIgnore_{false};          ///< `toggle_units_ignore`.
    bool unitsRadioMuted_{false};            ///< El radio de unidades desconectado (on_toggle_units_click).
    /** @brief Crea las preferencias y conecta la pestaña, el menú y el rótulo de la barra de estado. */
    void setupPreferences();
    class Preferences* prefs_{nullptr};  ///< Preferencias.
    std::optional<bool> unitsAutoAnswer_;   ///< Respuesta automática al aviso de unidades (tests).
    class ExclusionAreasManager* excAreas_{nullptr};  ///< Áreas de exclusión.  ///< Carpeta de datos elegida (vacía: la de la aplicación).
    class QMenu* menuProject_{nullptr};  ///< `menuproject`: menú contextual de la pestaña Proyecto.
    JsonValue projectOptions_;  ///< Opciones del proyecto abierto (`options`), para volver a guardarlas.
    QPointF lastClick_{0.0, 0.0};
    void selectObjects(bool multisel);
    void setupCanvasMenu();
    void setupHelpMenu();
    bool shouldWeSave_{false};  ///< `should_we_save`: hay cambios sin guardar.
    QString callSource_{"app"};          ///< `call_source`.
    QColor oldTabTextColor_;             ///< `old_tab_text_color`.
    bool oldStateOfToolsToolbar_{false}; ///< `old_state_of_tools_toolbar`.
    bool pendingDelete_{false};
    QWidget* shortcutsTab_{nullptr};
    void populateGridMenu();
    class QMenu* popMenu_{nullptr};      ///< `popMenu`: menú contextual del canvas.
    /// Las partes del menú del canvas que cambian dentro de los editores.
    struct CanvasMenu {
        class QAction* disable{nullptr};
        class QAction* newMenu{nullptr};
        class QAction* edit{nullptr};
        class QAction* save{nullptr};
        class QAction* properties{nullptr};
        class QMenu* geo{nullptr};
        class QMenu* grb{nullptr};
        class QMenu* exc{nullptr};
        std::map<QString, class QAction*> grbActions;
        std::map<QString, class QAction*> excActions;
    };
    CanvasMenu canvasMenu_;
    class QMenu* gridMenu_{nullptr};     ///< `cmenu_gridmenu`.
    QStringList gridContextMenu_{"0.1", "0.2", "0.5", "1", "2.54"};  ///< `global_grid_context_menu['mm']`.
    QPointF mouse_{0, 0};                ///< `self.mouse`: la última posición del mouse sobre el canvas.
    QString jumpRef_{"abs"};             ///< `global_jump_ref`.
    QString locatePt_{"bl"};             ///< `global_locate_pt`.
    bool cursorBig_{false};              ///< El cursor grande (on_cursor_type).
    bool cursorColorEnabled_{true};      ///< Si el cursor usa global_cursor_color (`cursor_color_3D`).
    bool workspaceCbMuted_{false};       ///< La casilla del área de trabajo sin su aviso (on_workspace_toggle).
    QMetaObject::Connection setOriginConnection_;
    void selectionAreaHandler(const QPointF& start, const QPointF& end, bool enclosure);
    void selectedMessage(FlatCAMObj* obj);
    void setActive(FlatCAMObj* obj, bool active);
    void redrawSelectionShapes();
    std::vector<PlotGroup> selShapes_;                ///< Recuadros de selección (y el rectángulo que se arrastra).
    std::optional<PlotGroup> movingSelection_;        ///< Rectángulo que se está arrastrando.
    std::optional<bool> selectionType_;               ///< `selection_type`: `true` encierra, `false` toca.
    bool doubleClick_{false};                         ///< `doubleclick`.
    bool clickNoProject_{false};                      ///< `click_noproject`.
    QStringList objectsUnderClick_;                   ///< `objects_under_the_click_list`.
    std::set<FlatCAMObj*> shapeDrawn_;                ///< Objetos con `selection_shape_drawn`.
    QString lastGerberFilter_;
    QString lastGcodeFilter_;
    QString lastExcellonFilter_;
};

}  // namespace flatcam::gui
