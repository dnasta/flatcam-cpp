/**
 * @file ExclusionAreasManager.h
 * @brief Áreas de exclusión de la aplicación (`app.exc_areas`, de `appCommon/Common.py`): agregarlas con el mouse,
 * borrarlas, dibujarlas y la tabla de los paneles de Perforación y Geometría.
 */
#pragma once

#include <QObject>
#include <QString>
#include <functional>
#include <vector>

#include "cam/ExclusionAreas.h"

class QPushButton;
class QWidget;

namespace flatcam::gui {

class MainGUI;
class CanvasPicker;
class FCTable;
class FCDoubleSpinner;
class RadioSet;
class FCCheckBox;

/**
 * @brief Almacén único de áreas de exclusión (`exclusion_areas_storage`), compartido por todas las herramientas: los
 * CNC Job de Perforación y de Geometría esquivan todas, sin importar desde dónde se agregaron.
 */
class ExclusionAreasManager : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Crea el almacén.
     * @param ui Ventana (mensajes y canvas).
     * @param picker Selector de zonas con el mouse.
     * @param parent Padre.
     */
    ExclusionAreasManager(MainGUI* ui, CanvasPicker* picker, QObject* parent = nullptr);

    /** @brief Las áreas. @return El almacén. */
    const std::vector<ExclusionArea>& areas() const { return areas_; }

    /**
     * @brief Agrega áreas con el mouse (`on_add_area_click`): rectángulos de dos clics o polígonos, según
     * @p shapeRadio; el clic derecho termina. Al terminar, si alguna corta la geometría del objeto se borran todas.
     * @param shapeRadio Forma (`square` o `polygon`).
     * @param overZ Z por encima.
     * @param strategy Estrategia (`over` o `around`).
     * @param cncButton Botón de generar el CNC Job (queda en naranja si hay áreas).
     * @param solidGeometry Geometría del objeto (para el control de superposición).
     * @param objType `"excellon"` o `"geometry"`.
     */
    void startAdding(RadioSet* shapeRadio, FCDoubleSpinner* overZ, RadioSet* strategy, QPushButton* cncButton,
                     ShapeList solidGeometry, const QString& objType);

    /** @brief Borra todas (`on_clear_area_click`). */
    void clearAll();

    /**
     * @brief Borra las de las filas elegidas (`delete_sel_shapes`).
     * @param rows Filas de la tabla.
     */
    void deleteRows(std::vector<int> rows);

    /**
     * @brief Llena la tabla de un panel (#, Objeto, Estrategia, Por encima de Z), como `build_ui`.
     * @param table Tabla.
     */
    void fillTable(FCTable* table) const;

    /**
     * @brief Conecta la sección "Agregar áreas de exclusión" de un panel (Perforación o Geometría), como sus `ui_connect`:
     * la casilla muestra la sección (`OptionalHideInputSection`), "Alrededor" deshabilita la Z, "Agregar área", "Borrar
     * todas" y "Borrar elegidas", y la tabla se rehace cuando cambia el almacén.
     * @param cb Casilla "Agregar áreas de exclusión".
     * @param frame Sección que muestra la casilla.
     * @param shapeRadio Forma de las zonas.
     * @param strategy Estrategia.
     * @param overZLabel Rótulo de la Z.
     * @param overZ Z por encima.
     * @param add Botón "Agregar área".
     * @param deleteAll Botón "Borrar todas".
     * @param deleteSelected Botón "Borrar elegidas".
     * @param cncButton Botón de generar el CNC Job.
     * @param table Tabla de áreas.
     * @param solidGeometry Geometría del objeto del panel (para el control de superposición).
     * @param objType `"excellon"` o `"geometry"`.
     */
    void bindPanel(FCCheckBox* cb, QWidget* frame, RadioSet* shapeRadio, RadioSet* strategy, QWidget* overZLabel,
                   FCDoubleSpinner* overZ, QPushButton* add, QPushButton* deleteAll, QPushButton* deleteSelected,
                   QPushButton* cncButton, FCTable* table, std::function<ShapeList()> solidGeometry,
                   const QString& objType);

Q_SIGNALS:
    /** @brief Cambió el almacén (`e_shape_modified`). */
    void changed();

private:
    void redraw();

    MainGUI* ui_;
    CanvasPicker* picker_;
    std::vector<ExclusionArea> areas_;
};

}  // namespace flatcam::gui
