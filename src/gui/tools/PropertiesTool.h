/**
 * @file PropertiesTool.h
 * @brief Herramienta "Propiedades" (port de `appTools/ToolProperties.py`): un árbol con los datos de los objetos
 * elegidos (tipo, nombre, dimensiones, unidades, opciones, aperturas o herramientas).
 */
#pragma once

#include "gui/generated/PropertiesToolUIBase.h"
#include "gui/tools/ToolInfo.h"

class QTreeWidgetItem;

namespace flatcam {
class JsonValue;
}

namespace flatcam::gui {

class App;
class FlatCAMObj;

/**
 * @brief Muestra en la pestaña Herramienta las propiedades de cada objeto elegido (`Properties`).
 *
 * Los valores salen del mismo diccionario con que el port guarda el objeto en un proyecto (`to_dict`), escritos como
 * los escribe Python (`str()`), así que las opciones aparecen en el orden de ese diccionario. Las dimensiones
 * (largo, ancho, área de la caja, de la envolvente convexa y del cobre) se calculan como `job_thread` del original,
 * pero en el mismo hilo.
 */
class PropertiesTool : public PropertiesToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app Aplicación. @param parent Widget padre. */
    explicit PropertiesTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
    /**
     * @brief `run`: abre el panel y lo llena con los objetos elegidos.
     * @param toggle Como el original: con `true` (tecla P) un segundo pedido esconde el panel; el menú usa `false`.
     */
    void run(bool toggle);
    /**
     * @brief Escribe un valor como `str()` de Python (`True`, `None`, `[1, 'a']`, `{'k': 0.1}`).
     * @param v Valor.
     * @param nested Si está dentro de una lista o un diccionario (los textos van entre comillas, como `repr`).
     * @return El texto.
     */
    static QString pyStr(const JsonValue& v, bool nested = false);

private:
    void properties();
    void addItems(FlatCAMObj* obj);
    App* app_;
};

}  // namespace flatcam::gui
