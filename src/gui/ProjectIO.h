/**
 * @file ProjectIO.h
 * @brief Objetos del proyecto desde y hacia un `.FlatPrj` de FlatCAM (port de `open_project`/`save_project` y de los
 * `from_dict`/`to_dict` de cada objeto).
 */
#pragma once

#include <QStringList>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include "core/Types.h"
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
     * Desvío consciente (pedido por el usuario): el original no guarda el código de inicio de los CNC Job (`gc_start`)
     * y al reabrir el proyecto los exporta sin él; acá se reconstruye con el postprocesador y los parámetros guardados.
     * @param d Diccionario del objeto.
     * @param[out] warning Motivo si no se pudo (tipo no soportado o datos dañados).
     * @param boundsOf Límites de otro objeto del proyecto por su nombre (el Excellon de origen de un taladrado).
     * @return El objeto, o `nullptr`.
     */
    static std::unique_ptr<FlatCAMObj> objectFrom(
        const JsonValue& d, QString& warning,
        const std::function<std::optional<BoundingBox>(const QString&)>& boundsOf = {});

    /**
     * @brief Código de inicio de un CNC Job guardado en un proyecto (ver @ref objectFrom).
     * @param d Diccionario del CNC Job.
     * @param sourceBounds Límites del Excellon de origen, si es un taladrado y se encontró.
     * @return El código, o vacío si no se pudo reconstruir.
     */
    static std::string rebuildStartGcode(const JsonValue& d, const std::optional<BoundingBox>& sourceBounds);

    /** @brief Opciones de fábrica de FlatCAM 2024.4 (`factory_defaults`, de `resources/factory_defaults.json`). @return Las opciones. */
    static const JsonValue& factoryDefaults();

    /**
     * @brief Diccionario de un objeto como lo guarda el original (`to_dict` con sus `ser_attrs`).
     *
     * Parte de lo que trajo el objeto si vino de un proyecto (así se conservan las claves que el port no usa) y si no,
     * de las opciones de la aplicación como `AppObject.new_object`. El G-code interpretado (`gcode_parsed`) se arma con
     * el algoritmo de `gcode_parse`, para que el FlatCAM en Python pueda dibujar el CNC Job al abrir el proyecto.
     * @param obj Objeto.
     * @param appOptions Opciones de la aplicación (las del proyecto abierto o las de fábrica).
     * @return El diccionario.
     */
    static JsonValue toDict(const FlatCAMObj& obj, const JsonValue& appOptions);

    /**
     * @brief El documento completo del proyecto (`save_project`): `objs`, `options` y `version`.
     * @param objs Objetos, en el orden del árbol.
     * @param appOptions Opciones de la aplicación.
     * @return El documento.
     */
    static JsonValue projectDict(const std::vector<FlatCAMObj*>& objs, const JsonValue& appOptions);

    /**
     * @brief `gcode_parse` del original (sin arcos): trazados separados en cada cambio de Z, con su tipo
     * (`T`/`C` viaje o corte, `F`/`S` rápido o avance) y, en un taladrado, el círculo de cada agujero.
     * @param gcode G-code de una herramienta.
     * @param laser Si el postprocesador es de láser (M3/M5 hacen de Z).
     * @param drillDia Diámetro de los agujeros si es un taladrado; 0 si no.
     * @return La lista `[{"geom": ..., "kind": [..]}]`.
     */
    static JsonValue gcodeParse(const std::string& gcode, bool laser, double drillDia);
};

}  // namespace flatcam::gui
