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
    bool machinist{false};  ///< `machinist_setting`: sin él se controlan las Z (corregido; ver CncJob::toolGcode).
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
    bool machinist{false};  ///< `machinist_setting`: sin él se controlan las Z (corregido, sección 14).
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

/**
 * @brief Los parámetros de `CNCjob.generate_from_excellon_by_tool` sin la interfaz (`use_ui = False`), como los arma el
 * comando Tcl `drillcncjob`: valen para todas las herramientas.
 */
struct ExcellonByToolSettings {
    std::string preprocessor{"default"};  ///< `pp_excellon_name` (si no existe, falla como el `KeyError` del original).
    std::string units{"MM"};              ///< `"MM"` o `"IN"`.
    int coordsDecimals{4};                ///< Decimales de las coordenadas.
    int decimals{4};                      ///< Decimales generales.
    int frDecimals{2};                    ///< Decimales de los avances.
    int stepsPerCircle{64};               ///< Pasos por circunferencia.
    PyNumber zCut;                        ///< `z_cut` (`float(drillz)`).
    PyNumber zMove;                       ///< `z_move` (Z de viaje).
    PyNumber feedrate;                    ///< `feedrate` (el comando le pone el avance Z).
    PyNumber zFeedrate;                   ///< `z_feedrate`.
    PyNumber feedrateRapid;               ///< `feedrate_rapid`.
    std::optional<PyNumber> spindleSpeed; ///< `spindlespeed` (`None` sin el argumento).
    std::string spindleDir{"CW"};         ///< `spindledir`.
    bool dwell{false};                    ///< `dwell`.
    PyNumber dwellTime;                   ///< `dwelltime`.
    bool multidepth{false};               ///< `multidepth`.
    PyNumber depthPerCut;                 ///< `z_depthpercut`.
    bool toolchange{false};               ///< `toolchange`.
    PyNumber zToolchange;                 ///< `z_toolchange`.
    std::string xyToolchange;             ///< `xy_toolchange`, el texto ("x, y" o vacío).
    std::optional<PyNumber> startZ;       ///< `startz`.
    PyNumber endZ;                        ///< `endz`.
    std::string xyEnd;                    ///< `xy_end`, el texto.
    std::string optType{"B"};             ///< `excellon_optimization_type`: "M", "B" (OR-Tools: el port usa su optimizador), "T" u otro.
    bool machinist{false};                ///< `machinist_setting`: sin él se controla la Z de corte (`check_zcut`).
    PyNumber zPdepth{-0.02};              ///< Profundidad de la sonda.
    PyNumber feedrateProbe{75};           ///< Avance de la sonda.
    std::optional<BoundingBox> bounds;    ///< Límites del Excellon (`options['xmin'...]`).
    std::vector<ExclusionArea> exclusionAreas;  ///< `app.exc_areas`.
};

/**
 * @brief Parámetros de @ref CncJob::generateFromGeometry2 (`generate_from_geometry_2`, el G-code de una Geometría de una
 * sola geometría que crea el comando Tcl `cncjob`), ya resueltos como los deja `GeometryObject.generatecncjob`.
 *
 * Los números son los que recibe el original; los controles de las Z (signo, cero) los hace el generador.
 */
struct Geometry2Settings {
    std::string preprocessor{"default"};     ///< Postprocesador (`pp_geometry_name`; vacío es `default`).
    std::string units{"MM"};                 ///< `"MM"` o `"IN"`.
    int coordsDecimals{4};                   ///< Decimales de las coordenadas.
    int decimals{4};                         ///< Decimales generales (`current_tooldia`).
    int frDecimals{2};                       ///< Decimales de los avances.
    int stepsPerCircle{64};                  ///< Pasos por circunferencia (cabecera).
    double toolDia{0.0};                     ///< Diámetro (`tooldia`).
    PyNumber toolDiaOption;                  ///< `options['tool_dia']` tal como llegó (cabecera).
    double offset{0.0};                      ///< Desplazamiento (`offset`): engrosa cada figura con esquinas en inglete.
    double tolerance{0.0};                   ///< Tolerancia de simplificación (`global_tolerance / 20`).
    double cutZ{0.0};                        ///< Z de corte.
    double travelZ{0.0};                     ///< Z de viaje.
    double feedrate{0.0};                    ///< Avance X-Y.
    double feedrateZ{0.0};                   ///< Avance Z.
    double feedrateRapid{0.0};               ///< Avance de rápidos.
    std::optional<int> spindleSpeed;         ///< `int(spindlespeed)`, o nada con 0.
    std::string spindleDir{"CW"};            ///< Sentido del husillo (`spindledir`, siempre "CW" desde `generatecncjob`).
    bool dwell{false};                       ///< Espera tras encender el husillo.
    double dwellTime{0.0};                   ///< Tiempo de espera.
    bool multidepth{false};                  ///< Profundidad múltiple.
    double depthPerPass{0.0};                ///< Profundidad por pasada (0 = toda la Z de corte).
    bool toolchange{false};                  ///< Cambio de herramienta al empezar.
    double toolchangeZ{0.0};                 ///< Z de cambio.
    std::optional<std::array<double, 2>> toolchangeXY;  ///< X,Y de cambio.
    bool extracut{false};                    ///< Corte extra en los anillos.
    double extracutLength{0.1};              ///< Largo del corte extra.
    std::optional<double> startZ;            ///< Z inicial.
    double endZ{0.0};                        ///< Z final.
    std::optional<std::array<double, 2>> endXY;  ///< X,Y final.
    double segX{0.0};                        ///< Segmentación en X (`segx`).
    double segY{0.0};                        ///< Segmentación en Y (`segy`).
    bool fPlunge{false};                     ///< `geometry_f_plunge`.
    double zPdepth{-0.02};                   ///< Profundidad de la sonda (`z_pdepth` del objeto).
    double feedrateProbe{75.0};              ///< Avance de la sonda (`feedrate_probe` del objeto).
    bool machinist{false};                   ///< `machinist_setting`: sin los controles de las Z.
    std::optional<BoundingBox> headerBounds; ///< `options['xmin']`... del objeto de origen.
    std::string geometryName;                ///< Nombre de la Geometría (mensaje de la Z de corte en cero).
    std::string jobName;                     ///< Nombre del CNC Job (mensaje de la Z de viaje en cero).
    std::vector<ExclusionArea> exclusionAreas;  ///< Áreas de exclusión de la aplicación (`app.exc_areas`).
};

/**
 * @brief Un aviso del generador para la aplicación (`app.inform.emit`): `[nivel] ` + texto + agregado + texto2 +
 * agregado2, con los textos en inglés (la aplicación los traduce).
 */
struct CncMessage {
    std::string level;    ///< `WARNING`, `ERROR_NOTCL`, `ERROR`... (vacío: sin nivel).
    std::string text;     ///< Texto del original (sin traducir).
    std::string suffix;   ///< Lo que va después, tal cual (`": " + nombre`), o vacío.
    std::string text2;    ///< Segundo texto para traducir, o vacío.
    std::string suffix2;  ///< Lo que va al final, tal cual.
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
     * @brief `generate_from_geometry_2`: el G-code de una Geometría de una sola geometría (comando Tcl `cncjob`).
     *
     * Como el original: recorre los trazados desde (0, 0) eligiendo siempre el extremo más cercano (invierte la línea si
     * es el final) y sigue desde el final del trazado recorrido; los controles de la Z de corte (positiva se invierte,
     * cero sin láser cancela) y de viaje (negativa se invierte, cero cancela) sí se aplican; la cabecera no tiene variante
     * de versión (la de `GRBL_laser` es la del fuente). Deja el cuerpo en @ref gcode y el comienzo en @ref startGcode.
     * @param solid `solid_geometry` de la Geometría.
     * @param s Parámetros.
     * @param messages Los avisos para la aplicación, en orden.
     * @return `false` si el original devuelve `'fail'`.
     */
    bool generateFromGeometry2(const ShapeList& solid, const Geometry2Settings& s, std::vector<CncMessage>& messages);

    /**
     * @brief `CNCjob.generate_from_excellon_by_tool` con `tools = "all"`, `order = 'fwd'`, `is_first = False` y
     * `use_ui = False` (el comando Tcl `drillcncjob`): el G-code queda en @ref gcode y el de inicio vacío.
     *
     * Como el original: las herramientas van por diámetro creciente; con cambio de herramienta cada una con su `T`, y
     * sin él todos los agujeros juntos con la herramienta 1; el orden "T" es el vecino más cercano de
     * `optimized_travelling_salesman`, que **repite el primer agujero**; "B" y "M" (OR-Tools) usan @ref PathOptimizer
     * (desvío); con otro orden falla al indexar (`list indices must be integers or slices, not tuple`). Sin
     * `machinist`, `check_zcut` pasa a negativa una Z positiva, falla con cero y **deja `None` una Z negativa**: entonces
     * falla al sumarle el desplazamiento (`unsupported operand type(s) for +: 'int' and 'NoneType'`).
     * @param ex El Excellon.
     * @param s Los parámetros.
     * @param messages Los avisos para la aplicación, en orden.
     * @return `false` si falla: @ref lastError es `"fail"` si el original devuelve `'fail'` y, si no, el texto de la
     * excepción de Python.
     */
    bool generateFromExcellonByTool(const Excellon& ex, const ExcellonByToolSettings& s, std::vector<CncMessage>& messages);

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
     * @brief Los avisos de cada herramienta del último @ref generateFromGeometryTools (los `app.inform.emit` de
     * `geometry_tool_gcode_gen`: el comienzo con el diámetro y, en la última, la cantidad de trazados).
     * @return Uno por herramienta, en orden.
     */
    const std::vector<std::vector<CncMessage>>& toolMessages() const { return toolMessages_; }
    /**
     * @brief Distancia recorrida (`travel_distance`): en un taladrado, los viajes entre agujeros (desde el X,Y de
     * cambio de herramienta) y hasta el X,Y final (`measured_distance` de `ToolDrilling`); en una geometría, los viajes
     * entre trazados y hasta (0, 0) más el largo de los cortes (por la cantidad de pasadas con profundidad múltiple).
     * @return La distancia, en las unidades del trabajo.
     */
    double travelDistance() const { return travelDistance_; }
    /**
     * @brief Tiempo estimado (`routing_time`), en minutos: en un taladrado, las bajadas y subidas a cero con el avance
     * Z y las subidas y los viajes con el avance de rápidos (de la última herramienta); en una geometría, el largo de
     * los cortes dividido por el avance de cada herramienta.
     * @return El tiempo.
     */
    double routingTime() const { return routingTime_; }

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

    /**
     * @brief Cierra las cuentas de un taladrado (`ToolDrilling.on_cnc_button_click`): el viaje hasta el X,Y final y
     * el tiempo estimado. @param last Parámetros de la última herramienta. @param lastPoint Último agujero.
     */
    void finishDrillStats(const DrillToolSettings& last, const std::array<double, 2>& lastPoint);
    /** @brief Genera el G-code de una herramienta. @return `false` si falla. */
    bool toolGcode(int tool, const std::vector<Point2D>& points, const Excellon& ex, std::array<double, 2>& firstPoint,
                   bool isFirst, bool isLast, std::string& tGcode, std::string& startGcode);

    DrillJobSettings settings_;
    std::string startGcode_;
    std::string gcode_;
    std::vector<std::string> toolGcodes_;
    std::string lastError_;
    std::vector<std::vector<CncMessage>> toolMessages_;  ///< Avisos de cada herramienta (trabajos de geometría).
    double travelDistance_{0.0};        ///< `travel_distance`.
    double routingTime_{0.0};           ///< `routing_time`.
    double measuredDistance_{0.0};      ///< `measured_distance` (taladrado).
    double measuredDown_{0.0};          ///< `measured_down_distance`.
    double measuredUpToZero_{0.0};      ///< `measured_up_to_zero_distance`.
    double measuredLift_{0.0};          ///< `measured_lift_distance`.
};

}  // namespace flatcam
