/**
 * @file ProjectIO.h
 * @brief Objetos del proyecto desde y hacia un `.FlatPrj` de FlatCAM (port de `open_project`/`save_project` y de los
 * `from_dict`/`to_dict` de cada objeto).
 */
#pragma once

#include <QStringList>
#include <memory>
#include <vector>

#include "io/ProjectFile.h"

namespace flatcam::gui {

class FlatCAMObj;

/**
 * @brief Convierte entre los objetos del port y los diccionarios que guarda FlatCAM en sus proyectos.
 *
 * Al abrir se descarta lo que el port no usa y pesa mucho (el G-code interpretado `gcode_parsed` y las copias de la
 * geometría de los CNC Job y de las Geometrías multiherramienta): un proyecto de 1,7 GB se abre con lo justo.
 */
class ProjectIO {
public:
    /** @brief Qué se descarta al leer un proyecto. @return La función para @ref ProjectFile::read. */
    static JsonSkipFn skipFn();

    /**
     * @brief Crea un objeto a partir de su diccionario (`new_object(kind, name, from_dict)`).
     * @param d Diccionario del objeto.
     * @param[out] warning Motivo si no se pudo (tipo no soportado o datos dañados).
     * @return El objeto, o `nullptr`.
     */
    static std::unique_ptr<FlatCAMObj> objectFrom(const JsonValue& d, QString& warning);
};

}  // namespace flatcam::gui
