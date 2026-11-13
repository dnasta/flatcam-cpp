/**
 * @file PcbWizardTool.h
 * @brief Asistente de PCB (port de `appTools/ToolPcbWizard.py`, Archivo -> Importar): un Excellon de PcbWizard
 * con la tabla de herramientas de su `.INF`.
 */
#pragma once

#include <string>
#include <vector>

#include "gui/generated/PcbWizardToolUIBase.h"
#include "gui/tools/ToolInfo.h"
#include "tools/PcbWizard.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/** @brief El panel del Asistente de PCB. */
class PcbWizardTool : public PcbWizardToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app La aplicación. @param parent Padre. */
    explicit PcbWizardTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y pestaña. @return Los textos. */
    static ToolInfo info();
    /** @brief `set_tool_ui`: pulgadas, LZ, formato 2:4 y nada cargado. */
    void setToolUi();
    /** @brief `load_excellon` + `on_file_loaded`. @param filename Archivo. */
    void loadExcellon(const QString& filename);
    /** @brief `load_inf` + `on_file_loaded`. @param filename Archivo. @return `false` si no trae herramientas. */
    bool loadInf(const QString& filename);
    /** @brief `on_import_excellon`. @return El Excellon creado, o nulo. */
    FlatCAMObj* importExcellon();
    /** @brief El texto armado (`modified_excellon_file`). @return El texto. */
    const std::string& mergedText() const { return merged_; }
    /** @brief "Created on" que se usa al armar el texto (para los tests). @param t Texto fijo; vacío: la hora. */
    void setTimeOverride(const QString& t) { timeOverride_ = t; }

private:
    void buildUi();
    void fileLoaded(bool inf, const QString& filename);
    App* app_;
    PcbWizardInf inf_;                    ///< `tools_from_inf`.
    std::vector<std::string> excLines_;   ///< `exc_file_content`.
    std::string merged_;                  ///< `modified_excellon_file`.
    bool mergedRead_{false};              ///< El StringIO ya se leyó una vez (queda al final).
    QString outname_{"file"};
    QString timeOverride_;
    bool excellonLoaded_{false};
    bool infLoaded_{false};
    bool processFinished_{false};
};

}  // namespace flatcam::gui
