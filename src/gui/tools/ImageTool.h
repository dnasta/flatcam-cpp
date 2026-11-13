/**
 * @file ImageTool.h
 * @brief Importar imagen (port de `appTools/ToolImage.py`, Archivo -> Importar -> Imagen) sobre `ImageImport`: los
 * píxeles oscuros (o con algún color por debajo de su umbral) como un Gerber o una Geometría.
 */
#pragma once

#include "gui/generated/ImageToolUIBase.h"
#include "gui/tools/ToolInfo.h"
#include "parsers/ImageImport.h"

namespace flatcam::gui {

class App;
class FlatCAMObj;

/**
 * @brief Las bandas de una imagen como las lee rasterio: el índice en las de paleta (y las de 1 bit), una sola banda en
 * las grises, las tres primeras en color y los valores de 16 bits en las de 16 bits.
 * @param filename Archivo. @param ok Si se pudo leer. @return Las bandas.
 */
ImageBands readImageBands(const QString& filename, bool* ok = nullptr);

/** @brief El panel de Importar imagen. */
class ImageTool : public ImageToolUIBase {
    Q_OBJECT
public:
    /** @brief Crea el panel. @param app La aplicación. @param parent Padre. */
    explicit ImageTool(App* app, QWidget* parent = nullptr);
    /** @brief Nombre y pestaña. @return Los textos. */
    static ToolInfo info();
    /** @brief `set_tool_ui`: 96 dpi, blanco y negro, umbrales 250. */
    void setToolUi();
    /** @brief `import_image` con los valores del panel. @param filename Archivo. @return El objeto, o nulo. */
    FlatCAMObj* importImage(const QString& filename);

private:
    void onImageType(const QString& v);
    App* app_;
};

}  // namespace flatcam::gui
