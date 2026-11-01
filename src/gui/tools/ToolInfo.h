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
 * @brief Postprocesadores que lista FlatCAM en sus combos (`app.preprocessors` menos `hpgl`), en el orden en que los
 * carga de la carpeta `preprocessors` en Windows.
 * @return Los nombres.
 */
QStringList preprocessorNames();

}  // namespace flatcam::gui
