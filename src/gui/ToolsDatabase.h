/**
 * @file ToolsDatabase.h
 * @brief Base de datos de herramientas (`tools_db.FlatDB` de `appDatabase.py`): el archivo, las herramientas nuevas, la
 * búsqueda por diámetro al agregar una herramienta y cómo se aplican sus datos.
 */
#pragma once

#include <QString>
#include <functional>
#include <optional>

#include "io/JsonValue.h"

namespace flatcam::gui {

struct ClearingTool;

/**
 * @brief El archivo de la base (un objeto JSON: `"1": {...}, "2": {...}`, cada uno con `name`, `tooldia`, `offset`,
 * `offset_value`, `type`, `tool_type` y `data`).
 *
 * `data['tool_target']` es el índice del combo "Objetivo": 0 General, 1 Fresado, 2 Perforación, 3 Aislamiento, 4 Pintura,
 * 5 NCC, 6 Corte.
 */
class ToolsDatabase {
public:
    /** @brief Ruta del archivo (`tools_database_path`). @param dataPath Carpeta de datos. @return La ruta. */
    static QString fileName(const QString& dataPath);

    /** @brief Crea el archivo vacío (`{}`) si no existe, como al arrancar el original. @param dataPath Carpeta de datos. */
    static void ensureFile(const QString& dataPath);

    /**
     * @brief Lee la base.
     * @param path Ruta.
     * @param[out] db La base.
     * @param[out] error "Could not load the file." o "Failed to parse Tools DB file." (en inglés: se traducen al mostrarlos).
     * @return `true` si se pudo leer.
     */
    static bool load(const QString& path, JsonValue& db, QString& error);

    /**
     * @brief Guarda la base (`json.dump(..., indent=2)`, en el orden de las herramientas).
     * @param path Ruta.
     * @param db La base.
     * @return `true` si se pudo escribir.
     */
    static bool save(const QString& path, const JsonValue& db);

    /**
     * @brief Herramienta nueva de la base (`ToolsDB2.on_tool_add`): `new_tool_<n>` con los valores de fábrica.
     * @param db La base (para numerar el nombre).
     * @return La herramienta.
     */
    static JsonValue newTool(const JsonValue& db);

    /** @brief Resultado de buscar una herramienta por diámetro. */
    enum class Lookup {
        NotLoaded,   ///< No se pudo leer el archivo.
        ParseError,  ///< El archivo no es JSON.
        NotFound,    ///< Ninguna herramienta de ese objetivo tiene ese diámetro (ni lo cubre su tolerancia).
        Multiple,    ///< Más de una.
        Found        ///< Una sola.
    };

    /** @brief La búsqueda y la herramienta encontrada. */
    struct LookupResult {
        Lookup status{Lookup::NotFound};  ///< Resultado.
        JsonValue tool;                   ///< La herramienta (si @ref Lookup::Found).
        std::optional<double> updatedDia; ///< Diámetro de la base si se encontró por la tolerancia.
    };

    /**
     * @brief Busca por diámetro como `on_tool_add` de las herramientas: la del mismo diámetro (redondeado), o la que lo
     * cubre con su tolerancia `[tol_min, tol_max]`, entre las del objetivo que acepta @p targetMatches.
     * @param path Ruta del archivo.
     * @param targetMatches Si la herramienta sirve, según su `data['tool_target']`. El original compara con un número en
     *        Aislamiento y Fresado, y con un texto en NCC, Pintura y Corte (como se guardan números, ahí nunca encuentra).
     * @param truncatedDia Diámetro redondeado a 4 decimales.
     * @return El resultado.
     */
    static LookupResult lookup(const QString& path, const std::function<bool(const JsonValue&)>& targetMatches,
                               double truncatedDia);

    /**
     * @brief Aplica una herramienta de la base a una de Paint, NCC o Isolation: diámetro, desplazamiento, tipo, forma y los
     * datos (los de fresado y los de @p prefix; con @p prefix vacío, todos).
     * @param tool Herramienta de la base.
     * @param t Herramienta del panel.
     * @param prefix Prefijo de los datos propios que se copian (`tools_iso`), o vacío para copiar todos.
     */
    static void applyTo(const JsonValue& tool, ClearingTool& t, const std::string& prefix = {});
};

}  // namespace flatcam::gui
