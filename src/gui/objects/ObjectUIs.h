/**
 * @file ObjectUIs.h
 * @brief Pestaña Propiedades de cada tipo de objeto: la interfaz generada más su `build_ui` (port de `ObjectUI.py` y de
 * los `build_ui` de `FlatCAMGerber.py` y `FlatCAMExcellon.py`).
 */
#pragma once

#include <functional>

#include "gui/generated/CNCObjectUIBase.h"
#include "gui/generated/ExcellonObjectUIBase.h"
#include "gui/generated/GeometryObjectUIBase.h"
#include "gui/generated/GerberObjectUIBase.h"

namespace flatcam::gui {

class GerberObject;
class ExcellonObject;
class CNCJobObject;
class GeometryObject;

/** @brief Propiedades de un Gerber (`GerberObjectUI`). */
class GerberObjectUI : public GerberObjectUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param parent Widget padre. */
    explicit GerberObjectUI(QWidget* parent = nullptr);

    /**
     * @brief Carga el objeto en el panel (`build_ui`): nombre, dibujo y, si está visible, la tabla de aperturas.
     * @param obj Objeto.
     */
    void build(GerberObject& obj);

private:
    void buildAperturesTable(GerberObject& obj);
    GerberObject* obj_{nullptr};
};

/** @brief Propiedades de un Excellon (`ExcellonObjectUI`). */
class ExcellonObjectUI : public ExcellonObjectUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param parent Widget padre. */
    explicit ExcellonObjectUI(QWidget* parent = nullptr);

    /**
     * @brief Carga el objeto en el panel (`build_ui`): la tabla de herramientas ordenada por diámetro, con la cantidad de
     * agujeros y ranuras de cada una y los totales.
     * @param obj Objeto.
     */
    void build(ExcellonObject& obj);
};

/**
 * @brief Propiedades de una Geometría (`GeometryObjectUI` + `build_ui`, `update_ui`, `gui_form_to_storage` y
 * `update_common_param_in_storage` de `FlatCAMGeometry.py`).
 *
 * La tabla muestra las herramientas (las columnas Compensar y Tipo quedan ocultas en el nivel Básico); al elegir una, el
 * formulario muestra sus parámetros, y lo que se cambia se guarda en la herramienta de la fila actual. Los parámetros
 * comunes (cambio de herramienta, Z de cambio, Z final, X,Y final y postprocesador) se guardan en todas.
 */
class GeometryObjectUI : public GeometryObjectUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param parent Widget padre. */
    explicit GeometryObjectUI(QWidget* parent = nullptr);
    /** @brief Carga el objeto (`build_ui`): tabla de herramientas y parámetros de la primera. @param obj Objeto. */
    void build(GeometryObject& obj);
    /** @brief Herramientas elegidas en la tabla, o la única si no hay selección (`on_generatecnc_button_click`). @return Los identificadores. */
    std::vector<int> toolsForCnc() const;
    /** @brief Qué hacer al apretar "Generar objeto CNCJob" (lo fija la aplicación). */
    std::function<void()> onGenerateCnc;
    /** @brief Mensajes en la barra de estado (lo fija la aplicación). */
    std::function<void(const QString&)> inform;
    /** @brief "Agregar desde la base" (abre la pestaña de la base; lo fija la aplicación). */
    std::function<void()> onAddFromDb;
    /** @brief Cambiaron las herramientas (para volver a dibujar; lo fija la aplicación). */
    std::function<void()> onToolsChanged;
    /** @brief Carpeta de datos (donde está `tools_db.FlatDB`). */
    QString dataPath;

    /**
     * @brief "Buscar y agregar" (`on_tool_add`): la herramienta de la base de ese diámetro (objetivo Fresado) o, si no
     * está, una copia de la última (`on_tool_default_add`).
     * @param dia Diámetro.
     */
    void addTool(double dia);
    /** @brief Borra las herramientas elegidas (`on_tool_delete`). */
    void deleteTools();

private:
    void updateUi();
    void formToStorage();
    void commonToStorage();
    /// `on_pp_changed`; @p fromBuild al cargar el panel (no restaura las casillas que hpgl fuerza).
    void onPpChanged(bool fromBuild = false);
    std::vector<int> selectedRows() const;
    int toolAt(int row) const;
    GeometryObject* obj_{nullptr};
    bool oldPpState_{false};          ///< `old_pp_state`: profundidad múltiple antes de elegir hpgl.
    bool oldToolchangeState_{false};  ///< `old_toolchangeg_state`: cambio de herramienta antes de elegir hpgl.
    bool muted_{false};
};

/** @brief Propiedades de un CNC Job (`CNCObjectUI`). */
class CNCObjectUI : public CNCObjectUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param parent Widget padre. */
    explicit CNCObjectUI(QWidget* parent = nullptr);

    /**
     * @brief Carga el trabajo en el panel (`build_ui`): nombre y tabla de herramientas del taladrado.
     * @param obj Objeto.
     */
    void build(CNCJobObject& obj);

    /** @brief Qué hacer al apretar "Guardar código CNC" (lo fija la aplicación). */
    std::function<void()> onExport;
};

}  // namespace flatcam::gui
