/**
 * @file FilmTool.h
 * @brief Film (port de `appTools/ToolFilm.py`, Alt+L) sobre `Film`: la película positiva o negativa de un Gerber o una
 * Geometría dentro de la caja de otro objeto, con escala, inclinación, espejo y perforado, en SVG, PNG o PDF.
 */
#pragma once

#include <QString>

#include "gui/generated/FilmToolUIBase.h"
#include "gui/tools/ToolInfo.h"
#include "tools/Film.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/** @brief El panel de Film. */
class FilmTool : public FilmToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app La aplicación. @param parent Padre. */
    explicit FilmTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y pestaña. @return Los textos. */
    static ToolInfo info();
    /** @brief `set_tool_ui`. */
    void setToolUi();
    /** @brief `on_film_creation`. @return El archivo escrito (vacío si no se escribió). */
    QString createFilm();
    /** @brief El archivo que devuelve el diálogo de guardar (para los tests). @param f Archivo; vacío: el diálogo. */
    void setSaveFileOverride(const QString& f) { saveOverride_ = f; }
    /** @brief El último SVG armado. @return El texto. */
    const std::string& lastSvg() const { return lastSvg_; }

private:
    void onFilmType(const QString& v);
    void onFileType(const QString& v);
    void onPunchSource(const QString& v);
    void fillCombo(FCComboBox* combo, const QString& type, bool last);
    void updateSections();
    QString positiveNormal(const QString& name, const QString& boxname, double factor, const QString& ftype);
    QString positivePunched(const QString& name, const QString& boxname, double factor, const QString& ftype);
    QString negative(const QString& name, const QString& boxname, double factor, const QString& ftype);
    QString askFile(const QString& caption, const QString& name, const QString& ftype);
    QString exportFilm(bool negative, const QString& objName, const QString& boxName, const QString& filename,
                       FilmOptions o, const QString& ftype);
    FilmOptions transforms() const;
    App* app_;
    QString saveOverride_;
    std::string lastSvg_;
};

}  // namespace flatcam::gui
