/**
 * @file ObjectUIs.h
 * @brief Pestaña Propiedades de cada tipo de objeto: la interfaz generada más su `build_ui` (port de `ObjectUI.py` y de
 * los `build_ui` de `FlatCAMGerber.py` y `FlatCAMExcellon.py`).
 */
#pragma once

#include <functional>

#include "gui/generated/CNCObjectUIBase.h"
#include "gui/generated/ExcellonObjectUIBase.h"
#include "gui/generated/GerberObjectUIBase.h"

namespace flatcam::gui {

class GerberObject;
class ExcellonObject;
class CNCJobObject;

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
