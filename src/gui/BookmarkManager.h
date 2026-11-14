/**
 * @file BookmarkManager.h
 * @brief Los marcadores de Ayuda (port de `Bookmark.py` y de `install_bookmarks` / `on_bookmarks_manager` /
 * `on_backup_site` de `app_Main.py`): el submenú Ayuda -> Marcadores con los de `global_bookmarks` y la pestaña
 * "Administrador de marcadores" para agregarlos, quitarlos, ordenarlos, exportarlos e importarlos.
 */
#pragma once

#include <QWidget>
#include <utility>
#include <vector>

namespace flatcam::gui {

class App;
class FCButton;
class FCEntry;
class FCTable;

/** @brief Un marcador: título y enlace. */
using Bookmark = std::pair<QString, QString>;
/** @brief `bm_dict`: clave (un número como texto) y marcador, en el orden del diccionario de Python. */
using BookmarkDict = std::vector<std::pair<QString, Bookmark>>;

/** @brief La pestaña "Administrador de marcadores" (`BookmarkManager`). */
class BookmarkManager : public QWidget {
    Q_OBJECT
public:
    /** @brief Arma la pestaña con una copia de los marcadores. @param app La aplicación. @param storage `global_bookmarks`. */
    BookmarkManager(App* app, BookmarkDict storage);

    FCTable* table_widget{nullptr};   ///< La tabla (#, Título, Enlace).
    FCEntry* title_entry{nullptr};    ///< Título del marcador nuevo.
    FCEntry* link_entry{nullptr};     ///< Enlace del marcador nuevo.
    FCButton* add_entry_btn{nullptr};     ///< "Añadir entrada".
    FCButton* remove_entry_btn{nullptr};  ///< "Remover entrada".
    FCButton* export_list_btn{nullptr};   ///< "Exportar lista".
    FCButton* import_list_btn{nullptr};   ///< "Importar lista".

    /** @brief Los marcadores de la pestaña (`bm_dict`). @return El diccionario. */
    const BookmarkDict& bookmarks() const { return bm_; }
    /**
     * @brief `on_add_entry`: agrega un marcador (con los campos si no se dan).
     * @param title Título. @param link Enlace. @param fromArgs Si vienen dados (si no, los de los campos).
     * @param indexError Se pone en `true` si el original falla con `IndexError` (un enlace vacío).
     * @return `false` si falló (`'fail'`).
     */
    bool addEntry(const QString& title = {}, const QString& link = {}, bool fromArgs = false, bool* indexError = nullptr);
    /** @brief `on_remove_entry`: quita los marcadores de las filas elegidas (salvo FlatCAM y el sitio de respaldo). */
    void removeEntry();
    /** @brief `on_export_bookmarks`. @param filename Archivo (vacío: se pregunta). */
    void exportBookmarks(const QString& filename = {});
    /** @brief `on_import_bookmarks`. @param filename Archivo (vacío: se pregunta). */
    void importBookmarks(const QString& filename = {});
    /**
     * @brief `rebuild_actions`: los marcadores en el orden de la tabla, y el menú de nuevo.
     * @return `false` si una fila no tiene el enlace (el original falla con `AttributeError`).
     */
    bool rebuildActions();

private:
    void buildUi();
    void markRows();
    App* app_;
    BookmarkDict bm_;
};

}  // namespace flatcam::gui
