/**
 * @file CncJob.h
 * @brief Generación del G-code de un trabajo de taladrado (port de `CNCjob.excellon_tool_gcode_gen`).
 */
#pragma once

#include <array>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "cam/EndpointStorage.h"
#include "cam/ExclusionAreas.h"
#include "cam/Preprocessor.h"
#include "parsers/Excellon.h"

namespace flatcam {

/** @brief Orden en que se visitan los agujeros de cada herramienta. */
enum class DrillOrder {
    FileOrder,   ///< El del archivo Excellon, sin optimizar (`excellon_optimization_type = 'No'`).
    Optimized,   ///< Recorrido corto calculado por @ref PathOptimizer.
    RTree        ///< Solo trabajos de geometría: el `'R'` del original (siempre la línea de extremo más cercano, invirtiéndola si hace falta).
};

/**
 * @brief Parámetros de taladrado de una herramienta (los `tools_drill_*` de FlatCAM).
 *
 * Los números son @ref PyNumber porque el G-code los imprime con `str()` y el resultado depende de si eran
 * enteros o decimales.
 */
struct DrillToolSettings {
    PyNumber cutZ;                        ///< Z de corte (`tools_drill_cutz`).
    PyNumber feedrateZ;                   ///< Avance Z (`tools_drill_feedrate_z`).
    PyNumber feedrateRapid;               ///< Avance de rápidos (`tools_drill_feedrate_rapid`).
    PyNumber travelZ;                     ///< Z de viaje (`tools_drill_travelz`).
    PyNumber depthPerPass;                ///< Profundidad por pasada (`tools_drill_depthperpass`).
    PyNumber offsetZ;                     ///< Offset Z (`tools_drill_offset`).
    bool multidepth{false};               ///< Profundidad múltiple (`tools_drill_multidepth`).
    bool dwell{false};                    ///< Espera tras encender el husillo (`tools_drill_dwell`).
    PyNumber dwellTime;                   ///< Tiempo de espera (`tools_drill_dwelltime`).
    bool fPlunge{false};                  ///< `tools_drill_f_plunge`.
    bool fRetract{false};                 ///< `tools_drill_f_retract`.
    std::optional<PyNumber> spindleSpeed; ///< Velocidad del husillo (`tools_drill_spindlespeed`).
    std::string spindleDir{"CW"};         ///< Sentido del husillo (`tools_drill_spindledir`).
    PyNumber toolchangeZ;                 ///< Z de cambio de herramienta (`tools_drill_toolchangez`).
    std::optional<std::array<double, 2>> toolchangeXY;  ///< X,Y de cambio (`tools_drill_toolchangexy`).
    std::optional<PyNumber> startZ;       ///< Z inicial (`tools_drill_startz`).
    PyNumber endZ;                        ///< Z final (`tools_drill_endz`).
    std::optional<std::array<double, 2>> endXY;         ///< X,Y final (`tools_drill_endxy`).
    PyNumber zPdepth{-0.02};              ///< Profundidad de la sonda (`tools_drill_z_pdepth`, Toolchange_Probe_MACH3).
    PyNumber feedrateProbe{75};           ///< Avance de la sonda (`tools_drill_feedrate_probe`).
};

/** @brief Todo lo necesario para generar el G-code de un Excellon. */
struct DrillJobSettings {
    std::string preprocessor{"default"};  ///< Postprocesador (`tools_drill_ppname_e`).
    GcodeFlavor flavor{GcodeFlavor::FlatCAM8_994};  ///< Versión cuyo G-code se reproduce (solo cambia `GRBL_laser`).
    std::vector<ExclusionArea> exclusionAreas;  ///< Áreas que esquivan los viajes entre agujeros (`app.exc_areas`).
    std::string units{"MM"};              ///< `"MM"` o `"IN"`.
    int coordsDecimals{4};                ///< Decimales de las coordenadas.
    int decimals{4};                      ///< Decimales generales.
    int frDecimals{2};                    ///< Decimales de los avances.
    int stepsPerCircle{64};               ///< Pasos por circunferencia (solo se imprime en la cabecera).
    DrillOrder order{DrillOrder::Optimized};  ///< Orden de los agujeros.
    /// Casilla "Cambio de herram" (`tools_drill_toolchange`). En `default` el cambio se emite igual; solo cambia la
    /// cabecera, que con `true` imprime `Z Toolchange` y `X,Y Toolchange`.
    bool toolchange{true};
    /// Orden en que se taladran las herramientas (`sel_tools` de ToolDrilling: según "Orden de la Herram"); vacío =
    /// por número de herramienta.
    std::vector<int> toolOrder;
    std::map<int, DrillToolSettings> tools;   ///< Parámetros por número de herramienta.
};

/** @brief Parámetros de corte de una herramienta de geometría (las claves de `tools[n]['data']` de FlatCAM). */
struct GeometryToolSettings {
    double toolDia{0.0};                   ///< Diámetro de la herramienta (`tooldia`).
    std::string offsetKind{"Path"};        ///< `"Path"` (sobre la línea), `"in"`, `"out"` o `"custom"` (`offset`).
    double offsetValue{0.0};               ///< Offset si @ref offsetKind es `"custom"` (`offset_value`).
    double cutZ{0.0};                      ///< Z de corte (`cutz`).
    bool multidepth{false};                ///< Profundidad múltiple (`multidepth`).
    double depthPerPass{0.0};              ///< Profundidad por pasada (`depthperpass`).
    double travelZ{0.0};                   ///< Z de viaje (`travelz`).
    double feedrate{0.0};                  ///< Avance X-Y (`feedrate`).
    double feedrateZ{0.0};                 ///< Avance Z (`feedrate_z`).
    double feedrateRapid{0.0};             ///< Avance de rápidos (`feedrate_rapid`).
    double spindleSpeed{0.0};              ///< Velocidad del husillo o potencia del láser (`spindlespeed`).
    std::string spindleDir{"CW"};          ///< `"CW"` o `"CCW"` (`spindledir`).
    bool dwell{false};                     ///< Espera tras encender el husillo (`dwell`).
    double dwellTime{0.0};                 ///< Tiempo de espera (`dwelltime`).
    std::optional<double> startZ;          ///< Z inicial (`startz`).
    double endZ{0.0};                      ///< Z final (`endz`).
    std::optional<std::array<double, 2>> endXY;         ///< X,Y final (`endxy`).
    double toolchangeZ{0.0};               ///< Z de cambio de herramienta (`toolchangez`).
    std::optional<std::array<double, 2>> toolchangeXY;  ///< X,Y de cambio de herramienta (`toolchangexy`).
    bool extracut{false};                  ///< Corte extra al cerrar anillos (`extracut`).
    double extracutLength{0.1};            ///< Largo del corte extra (`extracut_length`); 0 usa el modo de tres puntos.
    double tolerance{0.0};                 ///< Si es mayor que 0 se simplifica cada línea con esa tolerancia.
    double segX{0.0};                      ///< Largo máximo de los tramos en X al segmentar (`segx`); 0 no segmenta.
    double segY{0.0};                      ///< Largo máximo de los tramos en Y al segmentar (`segy`); 0 no segmenta.
};

/** @brief Todo lo necesario para generar el G-code de una geometría con una herramienta. */
struct GeometryJobSettings {
    std::string preprocessor{"default"};   ///< Postprocesador (`ppname_g`).
    std::string units{"MM"};               ///< `"MM"` o `"IN"`.
    int coordsDecimals{4};                 ///< Decimales de las coordenadas.
    int decimals{4};                       ///< Decimales generales.
    int frDecimals{2};                     ///< Decimales de los avances.
    int stepsPerCircle{64};                ///< Pasos por circunferencia (solo se imprime en la cabecera).
    DrillOrder order{DrillOrder::Optimized};  ///< Orden de los trazados.
    bool toolchange{true};                 ///< Emitir el cambio de herramienta antes de empezar.
    int tool{1};                           ///< Número de herramienta.
    GeometryToolSettings toolSettings;     ///< Parámetros de corte.
    GcodeFlavor flavor{GcodeFlavor::FlatCAM8_994};  ///< Versión de FlatCAM cuyo G-code se reproduce (predeterminada: 8.994).
    /// Límites que imprime la cabecera (`options['xmin']`...). Si falta se usan los de la geometría de origen.
    std::optional<BoundingBox> headerBounds;
    /// Profundidad de la sonda (Toolchange_Probe_MACH3): `mtool_gen_cncjob` usa la preferencia `geometry_z_pdepth`.
    double zPdepth{-0.02};
    /// Avance de la sonda: preferencia `geometry_feedrate_probe`.
    double feedrateProbe{75.0};
    std::vector<ExclusionArea> exclusionAreas;  ///< Áreas que esquivan los viajes al inicio de cada trazado.
};

/** @brief Resumen de una herramienta de un Excellon para la cabecera del postprocesador. */
struct DrillToolSummary {
    int id{0};       ///< Número de herramienta.
    double dia{0.0}; ///< Diámetro.
    int drills{0};   ///< Cantidad de agujeros.
    int slots{0};    ///< Cantidad de ranuras.
};

/** @brief Una herramienta de un trabajo de geometría con varias herramientas: su geometría y sus parámetros. */
struct GeometryToolJob {
    ShapeList geometry;            ///< Figuras que recorre esta herramienta (`tools[uid]['solid_geometry']`).
    GeometryJobSettings settings;  ///< Parámetros (`tools[uid]['data']`); `tool` es el número que imprime el cambio.
};

/** @brief Datos de la cabecera de comentarios del archivo de G-code (`gcode_header`). */
struct GcodeHeaderInfo {
    std::string version{"8.994"};          ///< Versión de FlatCAM que se declara.
    std::string versionDate{"2020/11/7"};  ///< Fecha de esa versión.
    std::string name;                      ///< Nombre del objeto CNC.
    std::string type{"Geometry"};          ///< Texto de `Type: G-code from <type>`.
    std::string units{"MM"};               ///< Unidades.
    std::string createdOn;                 ///< Fecha de creación, ya formateada.
};

/**
 * @brief Trabajo CNC: genera y guarda el G-code de un Excellon (taladrado con cambio de herramienta).
 *
 * Reproduce `CNCjob.excellon_tool_gcode_gen` de FlatCAM 2024.4 con `toolchange = True` y los datos de la
 * interfaz: para cada herramienta emite el avance Z, el cambio de herramienta, el husillo y, por cada agujero,
 * el movimiento rápido, la bajada, la subida a Z0 y la elevación; la última herramienta cierra el programa.
 * Solo se taladran agujeros: si el Excellon trae ranuras se avisa con un mensaje de registro, igual que el original,
 * que las deja para la herramienta de fresado.
 *
 * @code
 * flatcam::Excellon ex(opt);
 * ex.parseFile("taladros.drl");
 * flatcam::DrillJobSettings s;      // completar s.tools con los tools_drill_* de cada herramienta
 * flatcam::CncJob job(s);
 * job.generateFromExcellon(ex);
 * std::string gcode = job.exportGcode(header);
 * @endcode
 */
class CncJob {
public:
    /**
     * @brief Crea un trabajo.
     * @param settings Parámetros de taladrado.
     */
    explicit CncJob(DrillJobSettings settings = {});

    /**
     * @brief Genera el G-code de todas las herramientas con agujeros del Excellon.
     * @param excellon Excellon ya leído (y transformado, por ejemplo reflejado).
     * @return `true` si se generó; `false` si falló (ver @ref lastError).
     */
    bool generateFromExcellon(const Excellon& excellon);

    /**
     * @brief Genera el G-code que sigue las líneas de una geometría con una herramienta (`geometry_tool_gcode_gen`).
     *
     * Cada línea o contorno se recorre con un movimiento rápido hasta su inicio, la bajada a Z de corte y los
     * movimientos lineales. Si la herramienta tiene offset (`in`, `out` o `custom`) las figuras se desplazan antes.
     * Soporta profundidad múltiple (aritmética decimal exacta de 9 decimales, como el original), corte extra en
     * anillos cerrados, puntos sueltos (un agujero en cada uno), simplificación por tolerancia y segmentación de
     * líneas largas.
     * @param geometry Figuras a recorrer (líneas, anillos o polígonos; los polígonos se recorren por sus contornos).
     * @param settings Parámetros del trabajo.
     * @return `true` si se generó; `false` si falló (ver @ref lastError).
     */
    bool generateFromGeometry(const ShapeList& geometry, const GeometryJobSettings& settings);

    /**
     * @brief Genera el G-code de varias herramientas de una Geometría en un solo trabajo (`mtool_gen_cncjob` con
     * `geometry_tool_gcode_gen(..., is_first, is_last, toolchange=True)`).
     *
     * Cada herramienta emite su cambio de herramienta y sus trazados con sus propios parámetros; el código de inicio sale
     * de la primera y el apagado, la subida y el código de cierre solo van después de la última. Con una sola herramienta
     * es lo mismo que @ref generateFromGeometry.
     * @param tools Herramientas, en el orden de la tabla.
     * @return `true` si se generó; `false` si falló (ver @ref lastError).
     */
    bool generateFromGeometryTools(const std::vector<GeometryToolJob>& tools);

    /**
     * @brief Restaura un trabajo guardado en un proyecto: su G-code tal como quedó (no se vuelve a generar).
     * @param startGcode Código de inicio (en los proyectos de FlatCAM no se guarda: queda vacío).
     * @param toolGcodes G-code de cada herramienta, en orden.
     */
    void restore(std::string startGcode, std::vector<std::string> toolGcodes);

    /**
     * @brief Código de inicio de un trabajo de geometría (lo que genera @ref generateFromGeometry para la primera
     * herramienta), sin generar el resto: sirve para reconstruirlo al abrir un proyecto.
     * @param settings Parámetros de la primera herramienta (con @ref GeometryJobSettings::headerBounds).
     * @return El texto, terminado en salto de línea (vacío si el postprocesador no existe).
     */
    static std::string geometryStartGcode(const GeometryJobSettings& settings);

    /**
     * @brief Código de inicio de un taladrado (lo que genera @ref generateFromExcellon), sin generar el resto.
     * @param settings Parámetros (`tools` con los de cada herramienta).
     * @param firstTool Primera herramienta del trabajo.
     * @param tools Herramientas del Excellon con su diámetro y sus cantidades.
     * @param bounds Límites del Excellon (`options['xmin']`...).
     * @return El texto, terminado en salto de línea (vacío si el postprocesador no existe).
     */
    static std::string drillStartGcode(const DrillJobSettings& settings, int firstTool,
                                       const std::vector<DrillToolSummary>& tools,
                                       const std::optional<BoundingBox>& bounds);

    /** @brief Código de inicio (cabecera del postprocesador), terminado en salto de línea. @return El texto. */
    const std::string& startGcode() const { return startGcode_; }

    /** @brief G-code de las herramientas, sin cabecera ni código de inicio. @return El texto. */
    const std::string& gcode() const { return gcode_; }

    /** @brief G-code de cada herramienta por separado, en orden. @return Los textos. */
    const std::vector<std::string>& toolGcodes() const { return toolGcodes_; }

    /** @brief Mensaje del último error. @return El mensaje, vacío si no hubo. */
    const std::string& lastError() const { return lastError_; }

    /**
     * @brief Cabecera de comentarios del archivo (`gcode_header`).
     * @param info Datos de la cabecera.
     * @return El texto, terminado en una línea en blanco.
     */
    static std::string gcodeHeader(const GcodeHeaderInfo& info);

    /**
     * @brief Texto completo del archivo de G-code (`export_gcode` sin preámbulo ni posámbulo).
     * @param info Datos de la cabecera.
     * @return Cabecera, código de inicio y G-code, con saltos de línea `\n`.
     */
    std::string exportGcode(const GcodeHeaderInfo& info) const;

private:
    /** @brief Parámetros del postprocesador de un trabajo de geometría. */
    static PreprocessorParams geometryParams(const GeometryJobSettings& js, const ShapeList& geometry);
    /** @brief Parámetros del postprocesador de una herramienta de taladrado. */
    static PreprocessorParams drillParams(const DrillJobSettings& s, int tool, const std::vector<DrillToolSummary>& tools,
                                          const std::optional<BoundingBox>& bounds);

    /** @brief G-code de una herramienta de geometría (`geometry_tool_gcode_gen`). @return `false` si falla. */
    bool geometryToolGcode(const ShapeList& geometry, const GeometryJobSettings& js, bool isFirst, bool isLast,
                           std::string& tGcode, std::string& startGcode);

    /** @brief Genera el G-code de una herramienta. @return `false` si falla. */
    bool toolGcode(int tool, const std::vector<Point2D>& points, const Excellon& ex, std::array<double, 2>& firstPoint,
                   bool isFirst, bool isLast, std::string& tGcode, std::string& startGcode);

    DrillJobSettings settings_;
    std::string startGcode_;
    std::string gcode_;
    std::vector<std::string> toolGcodes_;
    std::string lastError_;
};

}  // namespace flatcam
