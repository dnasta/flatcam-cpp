/**
 * @file Isolation.h
 * @brief Aislamiento de pistas: recorridos alrededor del cobre a una distancia dada (port de `ToolIsolation.py`).
 */
#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "core/Shape.h"

namespace flatcam {

/** @brief Qué contornos del aislamiento se conservan (`tools_iso_isotype`). */
enum class IsolationType {
    Exterior = 0,  ///< Solo los contornos exteriores (`ext`).
    Interior = 1,  ///< Solo los agujeros (`int`).
    Full = 2       ///< Exteriores y agujeros (`full`, el valor por defecto).
};

/** @brief Sentido del fresado (`tools_iso_milling_type`). */
enum class MillingDirection {
    Climb,        ///< En concordancia (`cl`, el valor por defecto): se invierte el contorno exterior.
    Conventional  ///< Convencional (`cv`): el contorno queda como lo da el buffer.
};

/** @brief Orden en que se usan las herramientas en el mecanizado de resto (`tools_iso_order`). */
enum class IsolationToolOrder {
    Default,  ///< El orden de la tabla (`no`).
    Forward,  ///< De la más chica a la más grande (`fwd`).
    Reverse   ///< De la más grande a la más chica (`rev`, el valor por defecto).
};

/** @brief Una herramienta de la tabla de aislamiento con sus parámetros propios. */
struct IsolationTool {
    double toolDia{0.1};                              ///< Diámetro (`tooldia`).
    int passes{1};                                    ///< Cantidad de pasadas (`tools_iso_passes`).
    double overlap{0.1};                              ///< Solape entre pasadas, de 0 a 1 (`tools_iso_overlap` / 100).
    MillingDirection milling{MillingDirection::Climb};  ///< Sentido del fresado.
    IsolationType type{IsolationType::Full};          ///< Contornos que se conservan.
    bool follow{false};                               ///< Seguir las líneas centrales del Gerber (`tools_iso_follow`).
};

/** @brief Opciones comunes a todas las herramientas (la parte de abajo del panel de FlatCAM). */
struct IsolationOptions {
    bool combine{true};      ///< Juntar todas las pasadas de una herramienta en una geometría (`tools_iso_combine_passes`).
    bool rest{false};        ///< Mecanizado de resto (`tools_iso_rest`); solo cuenta si @ref combine es `true`.
    bool forcedRest{true};   ///< En el resto, descartar los polígonos cuyos agujeros no se pueden aislar (`tools_iso_force`).
    IsolationToolOrder order{IsolationToolOrder::Reverse};  ///< Orden de las herramientas en el resto.
    int stepsPerCircle{64};  ///< Segmentos por cuarto de círculo del buffer (`gerber_circle_steps`).
    bool negativeDia{false}; ///< Aislar hacia adentro (FlatCAM lo usa al seleccionar agujeros de un polígono).
    bool useExcept{false};   ///< Restar @ref exceptGeometry de los recorridos (`tools_iso_isoexcept`).
    ShapeList exceptGeometry;  ///< Geometría que se resta (la del objeto elegido en el combo, p. ej. un Excellon).
    ShapeList limitedArea;     ///< Si no está vacía, los recorridos se recortan a esta zona (selección por área).
    int decimals{4};           ///< Decimales con que se redondean los diámetros al ordenar y al nombrar.
};

/** @brief Recorridos de una herramienta: todas sus pasadas juntas o una sola pasada. */
struct IsolationPathSet {
    std::size_t toolIndex{0};  ///< Índice de la herramienta en el vector recibido.
    int pass{-1};              ///< Pasada (desde 0) si las pasadas van separadas; -1 si van combinadas.
    ShapeList geometry;        ///< Polígonos, anillos o líneas (lo que va a la `solid_geometry` del objeto).
};

/** @brief Resultado del aislamiento. */
struct IsolationOutcome {
    std::vector<IsolationPathSet> sets;  ///< Una entrada por objeto Geometry que crearía FlatCAM, en su orden.
    ShapeList notIsolated;               ///< En el resto: polígonos que ninguna herramienta pudo aislar.
};

/**
 * @brief Aislamiento de un Gerber con una o varias herramientas (port de `ToolIsolation`).
 *
 * Cubre los tres modos del original: pasadas separadas (un objeto por pasada), pasadas combinadas por herramienta
 * (`combined_normal`) y mecanizado de resto (`combined_rest`), con aislamiento exterior, interior o completo, fresado
 * en concordancia o convencional, seguimiento de líneas, resta de otro objeto y recorte a un área. La selección
 * interactiva (por área, por polígono o por objeto de referencia) queda para la GUI: acá se recibe ya resuelta como
 * @ref IsolationOptions::limitedArea o como la geometría a aislar.
 *
 * Desvíos conscientes respecto del original (que en esos casos falla o pierde figuras):
 * - Con pasadas separadas y un Gerber de un solo polígono el original no puede iterar el resultado; acá se devuelve.
 * - En el resto, si el buffer de un polígono da varias partes (solo con @ref IsolationOptions::negativeDia) se
 *   conservan todas; el original las descarta (al invertir) o falla (sin invertir).
 * - No está portada la verificación de diámetro seguro (`tools_iso_check_valid`), que solo informa.
 */
class Isolation {
public:
    /**
     * @brief Distancia de la pasada @p pass: `tooldia * ((2 i + 1) / 2.0000001) - i * overlap * tooldia`.
     * @param toolDia Diámetro de la herramienta.
     * @param pass Pasada, desde 0.
     * @param overlap Solape, de 0 a 1.
     * @param negative Si es `true` cambia el signo.
     * @return Distancia del buffer.
     */
    static double passOffset(double toolDia, int pass, double overlap, bool negative);

    /**
     * @brief Contornos a una distancia del cobre (`Geometry.isolation_geometry`).
     *
     * Agranda cada figura @p offset (esquinas redondeadas), une todo y conserva los contornos pedidos.
     * @param geometry Figuras a aislar.
     * @param offset Distancia; 0 no agranda.
     * @param type Contornos que se conservan.
     * @param stepsPerCircle Segmentos por cuarto de círculo.
     * @return Polígonos (si @p type es Full) o anillos.
     * @throws GeometryError si GEOS falla.
     */
    static ShapeList isolationGeometry(const ShapeList& geometry, double offset, IsolationType type,
                                       int stepsPerCircle);

    /**
     * @brief Envolvente de una pasada con el sentido de fresado aplicado (`generate_envelope`).
     *
     * Con @p invert (fresado en concordancia) cada polígono se rehace con el contorno exterior al revés y cada anillo
     * se convierte en un polígono con sus vértices al revés. Con @p follow devuelve @p geometry sin cambios.
     * @param geometry Figuras a aislar (o líneas centrales si @p follow).
     * @param offset Distancia de la pasada.
     * @param invert Invertir el sentido.
     * @param type Contornos que se conservan.
     * @param follow Seguir las líneas en vez de agrandar.
     * @param stepsPerCircle Segmentos por cuarto de círculo.
     * @return La envolvente.
     * @throws GeometryError si GEOS falla.
     */
    static ShapeList generateEnvelope(const ShapeList& geometry, double offset, bool invert, IsolationType type,
                                      bool follow, int stepsPerCircle);

    /**
     * @brief Resta una geometría de los recorridos (`area_subtraction`).
     *
     * Los polígonos se recorren por sus anillos; las líneas y anillos se restan tal cual; el resto se descarta.
     * @param geometry Recorridos.
     * @param subtractor Geometría a restar (se une antes).
     * @return Líneas resultantes, sin las vacías.
     * @throws GeometryError si GEOS falla.
     */
    static ShapeList areaSubtraction(const ShapeList& geometry, const ShapeList& subtractor);

    /**
     * @brief Recorta los recorridos a una zona (`area_intersection`).
     * @param geometry Recorridos.
     * @param area Zona (se une antes).
     * @return Líneas resultantes, sin las vacías.
     * @throws GeometryError si GEOS falla.
     */
    static ShapeList areaIntersection(const ShapeList& geometry, const ShapeList& area);

    /**
     * @brief Aislamiento de resto de una herramienta (`generate_rest_geometry`).
     *
     * Una pasada de un polígono vale solo si su buffer al doble de la distancia no toca a ningún otro polígono. Los
     * buffers usan 16 segmentos por cuarto de círculo y la distancia `(2 i + 1) / 2.0`, como el original.
     * @param geometry Polígonos que faltan aislar.
     * @param toolDia Diámetro de la herramienta.
     * @param passes Cantidad de pasadas.
     * @param overlap Solape, de 0 a 1.
     * @param invert Invertir el sentido (fresado en concordancia).
     * @param type Contornos que se conservan.
     * @param negativeDia Aislar hacia adentro.
     * @param forcedRest Descartar los polígonos con más de un agujero si alguno no se pudo aislar.
     * @param[out] notIsolated Polígonos que esta herramienta no pudo aislar.
     * @return Anillos de los recorridos.
     * @throws GeometryError si GEOS falla.
     */
    static ShapeList generateRestGeometry(const ShapeList& geometry, double toolDia, int passes, double overlap,
                                          bool invert, IsolationType type, bool negativeDia, bool forcedRest,
                                          ShapeList& notIsolated);

    /**
     * @brief Geometría a aislar con la selección "objeto de referencia": el cobre menos la referencia.
     * @param solidGeometry Cobre del Gerber.
     * @param reference Geometría del objeto de referencia.
     * @return Los polígonos de la diferencia.
     * @throws GeometryError si GEOS falla.
     */
    static ShapeList referenceGeometry(const ShapeList& solidGeometry, const ShapeList& reference);

    /**
     * @brief Nombre del objeto que crea FlatCAM con pasadas separadas (`<base>_<diámetro>[_ext|_int]_iso[N]`).
     * @param baseName Nombre del Gerber.
     * @param toolDia Diámetro de la herramienta.
     * @param type Contornos que se conservan.
     * @param pass Pasada, desde 0.
     * @param passes Cantidad total de pasadas (con una sola no lleva número).
     * @param decimals Decimales del diámetro.
     * @return El nombre.
     */
    static std::string objectName(const std::string& baseName, double toolDia, IsolationType type, int pass,
                                  int passes, int decimals = 4);

    /**
     * @brief Aísla un Gerber con todas las herramientas (`isolate`, `combined_normal` y `combined_rest`).
     * @param solidGeometry Cobre del Gerber.
     * @param followGeometry Líneas centrales del Gerber (para las herramientas con @ref IsolationTool::follow).
     * @param tools Herramientas en el orden de la tabla.
     * @param options Opciones comunes.
     * @param[out] outcome Recorridos de cada objeto que se crearía.
     * @param[out] error Mensaje si falla.
     * @param geometry Si no es nulo, figuras a aislar en lugar del cobre (selección por polígono o por referencia).
     * @return `true` si se generó al menos un recorrido.
     */
    static bool isolate(const ShapeList& solidGeometry, const ShapeList& followGeometry,
                        const std::vector<IsolationTool>& tools, const IsolationOptions& options,
                        IsolationOutcome& outcome, std::string& error, const ShapeList* geometry = nullptr);
};

}  // namespace flatcam
