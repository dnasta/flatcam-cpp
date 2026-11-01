/**
 * @file ToolPanels.h
 * @brief Paneles de las herramientas en la pestaña Herramienta (port de la parte de interfaz de `ToolNCC`,
 * `ToolPaint`); incluye también Perforación y Corte.
 */
#pragma once

#include "gui/generated/NccToolUIBase.h"
#include "gui/generated/PaintToolUIBase.h"
#include "gui/tools/CutOutTool.h"
#include "gui/tools/DrillingTool.h"
#include "gui/tools/ToolInfo.h"

namespace flatcam::gui {

/** @brief Panel de la limpieza de cobre (`NonCopperClear`). */
class NccTool : public NccToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel con los valores de fábrica. @param parent Widget padre. */
    explicit NccTool(QWidget* parent = nullptr) : NccToolUIBase(parent) { setupUi(); }
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
};

/** @brief Panel de pintura (`ToolPaint`). */
class PaintTool : public PaintToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel con los valores de fábrica. @param parent Widget padre. */
    explicit PaintTool(QWidget* parent = nullptr) : PaintToolUIBase(parent) { setupUi(); }
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
};

}  // namespace flatcam::gui
