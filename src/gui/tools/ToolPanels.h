/**
 * @file ToolPanels.h
 * @brief Paneles de las herramientas en la pestaña Herramienta (port de la parte de interfaz de `ToolNCC`,
 * `ToolPaint`, `ToolDrilling` y `ToolCutOut`).
 */
#pragma once

#include "gui/generated/CutOutToolUIBase.h"
#include "gui/generated/DrillingToolUIBase.h"
#include "gui/generated/NccToolUIBase.h"
#include "gui/generated/PaintToolUIBase.h"

namespace flatcam::gui {

/** @brief Datos comunes de una herramienta: su nombre (`toolName`) y el título de la pestaña al abrirla. */
struct ToolInfo {
    QString toolName;  ///< Nombre de la herramienta (`toolName`), que también es el nombre del panel.
    QString tabText;   ///< Texto de la pestaña Herramienta (`notebook.setTabText(2, ...)`).
};

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

/** @brief Panel de perforación (`ToolDrilling`). */
class DrillingTool : public DrillingToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel con los valores de fábrica. @param parent Widget padre. */
    explicit DrillingTool(QWidget* parent = nullptr) : DrillingToolUIBase(parent) { setupUi(); }
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
};

/** @brief Panel de corte del contorno (`CutOut`). */
class CutOutTool : public CutOutToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel con los valores de fábrica. @param parent Widget padre. */
    explicit CutOutTool(QWidget* parent = nullptr) : CutOutToolUIBase(parent) { setupUi(); }
    /** @brief Nombre y título de la pestaña. @return Los datos. */
    static ToolInfo info();
};

}  // namespace flatcam::gui
