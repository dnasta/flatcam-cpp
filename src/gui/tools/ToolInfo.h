/**
 * @file ToolInfo.h
 * @brief Nombre de una herramienta y título de su pestaña.
 */
#pragma once

#include <QString>

namespace flatcam::gui {

/** @brief Datos comunes de una herramienta: su nombre (`toolName`) y el título de la pestaña al abrirla. */
struct ToolInfo {
    QString toolName;  ///< Nombre de la herramienta (`toolName`), que también es el nombre del panel.
    QString tabText;   ///< Texto de la pestaña Herramienta (`notebook.setTabText(2, ...)`).
};

/**
 * @brief Postprocesadores que lista FlatCAM en sus combos (`app.preprocessors`): `default` primero (así lo reordena
 * `App.__init__`) y después el resto en el orden en que Windows lista la carpeta `preprocessors` (sin distinguir
 * mayúsculas).
 * @param includeHpgl `false` en Perforación: el original saltea `hpgl`, que es solo para geometrías.
 * @return Los nombres.
 */
QStringList preprocessorNames(bool includeHpgl = true);

}  // namespace flatcam::gui
