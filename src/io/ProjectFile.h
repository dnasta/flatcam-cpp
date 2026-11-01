/**
 * @file ProjectFile.h
 * @brief Lectura y escritura de proyectos de FlatCAM (`.FlatPrj`): JSON, comprimido con XZ/LZMA o no.
 */
#pragma once

#include <functional>
#include <string>
#include <vector>

#include "io/JsonValue.h"

namespace flatcam {

/**
 * @brief Decide si un valor se descarta al leer, sin guardarlo en memoria.
 *
 * Recibe la ruta de claves hasta el valor (las listas aportan `"[]"`) y el objeto que lo contiene, con los miembros
 * ya leídos (las claves de los proyectos vienen ordenadas, así que por ejemplo `kind` ya está cuando llega
 * `solid_geometry`). Devuelve `true` para saltearlo: queda como nulo.
 */
using JsonSkipFn = std::function<bool(const std::vector<std::string>& path, const JsonValue& parent)>;

/**
 * @brief Lee y escribe los archivos de proyecto de FlatCAM.
 *
 * El original guarda `json.dumps(d, default=to_dict, indent=2, sort_keys=True)`, comprimido con `lzma.open` (formato
 * XZ) si `global_save_compressed` está activo (el valor por defecto). Un proyecto puede pesar más de un gigabyte sin
 * comprimir (el NCC por láser guarda tres copias de su geometría), por eso la lectura es en streaming y permite
 * descartar lo que no hace falta sin guardarlo. Las figuras van como `{"__class__": "Shply", "__inst__": "<WKT>"}`.
 */
class ProjectFile {
public:
    /**
     * @brief Lee un proyecto (comprimido o no; se reconoce por el encabezado XZ).
     * @param path Ruta.
     * @param[out] root El documento.
     * @param[out] error Motivo si falla.
     * @param skip Valores a descartar (opcional).
     * @return `true` si se pudo leer.
     */
    static bool read(const std::string& path, JsonValue& root, std::string& error, const JsonSkipFn& skip = {});

    /**
     * @brief Interpreta un texto JSON.
     * @param text Texto.
     * @param[out] root El documento.
     * @param[out] error Motivo si falla.
     * @param skip Valores a descartar (opcional).
     * @return `true` si es JSON válido.
     */
    static bool parse(const std::string& text, JsonValue& root, std::string& error, const JsonSkipFn& skip = {});

    /**
     * @brief Texto JSON como `json.dumps(value, indent=2, sort_keys=True)` de Python (claves ordenadas, `ensure_ascii`).
     * @param value Documento.
     * @return El texto.
     */
    static std::string dump(const JsonValue& value);

    /**
     * @brief Guarda un proyecto.
     * @param path Ruta.
     * @param root Documento.
     * @param compressed Comprimir con XZ (`global_save_compressed`).
     * @param level Nivel de compresión 0 a 9 (`global_compression_level`).
     * @param[out] error Motivo si falla.
     * @return `true` si se pudo escribir.
     */
    static bool write(const std::string& path, const JsonValue& root, bool compressed, int level, std::string& error);
};

}  // namespace flatcam
