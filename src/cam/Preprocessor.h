/**
 * @file Preprocessor.h
 * @brief Postprocesadores de G-code (port de `appPreProcessor.py` y de `preprocessors/` de FlatCAM).
 */
#pragma once

#include <array>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "cam/PyNumber.h"

namespace flatcam {

/** @brief Herramienta en uso de un trabajo de taladrado: `[tool, diámetro, cantidad de agujeros, cantidad de ranuras]`. */
struct ToolInUse {
    int tool{0};              ///< Número de herramienta.
    PyNumber tooldia;         ///< Diámetro.
    int drills{0};            ///< Cantidad de agujeros.
    int slots{0};             ///< Cantidad de ranuras.
};

/** @brief Parámetros de una herramienta de taladrado que se imprimen en la cabecera (`exc_tools[n]`). */
struct ExcToolInfo {
    int tool{0};              ///< Número de herramienta.
    PyNumber tooldia;         ///< Diámetro (`tooldia`).
    PyNumber feedrateZ;       ///< Avance Z (`tools_drill_feedrate_z`).
    PyNumber feedrateRapid;   ///< Avance de rápidos (`tools_drill_feedrate_rapid`).
    PyNumber cutZ;            ///< Z de corte (`tools_drill_cutz`).
    PyNumber offsetZ;         ///< Offset Z (`tools_drill_offset`).
    PyNumber depthPerPass;    ///< Profundidad por pasada (`tools_drill_depthperpass`).
    PyNumber travelZ;         ///< Z de viaje (`tools_drill_travelz`).
};

/** @brief Opciones del objeto CNC que usan los postprocesadores (`p['options']`). */
struct PreprocessorOptions {
    PyNumber xmin;                          ///< Límite izquierdo de la geometría.
    PyNumber ymin;                          ///< Límite inferior de la geometría.
    PyNumber xmax;                          ///< Límite derecho de la geometría.
    PyNumber ymax;                          ///< Límite superior de la geometría.
    std::string type{"Geometry"};           ///< `"Geometry"`, `"Excellon"` o `"Excellon Geometry"`.
    PyNumber toolDia;                       ///< Diámetro de herramienta (`tool_dia`).
    std::vector<ToolInUse> toolsInUse;      ///< Herramientas en uso (`Tools_in_use`).
};

/**
 * @brief Todo lo que un postprocesador puede necesitar (el diccionario `p` del original).
 *
 * Reúne los atributos del trabajo CNC (`CNCjob.postdata`) más los argumentos puntuales de cada llamada
 * (`x`, `y`, `toolchangexy`). Los números que se imprimen con `str()` son @ref PyNumber para conservar si eran
 * enteros o decimales.
 */
struct PreprocessorParams {
    std::string units{"MM"};                ///< `"MM"` o `"IN"`.
    int coordsDecimals{4};                  ///< Decimales de las coordenadas (`cncjob_coords_decimals`).
    int decimals{4};                        ///< Decimales generales de la aplicación.
    int frDecimals{2};                      ///< Decimales de los avances (`cncjob_fr_decimals`).

    PyNumber feedrate;                      ///< Avance de trabajo.
    PyNumber zFeedrate;                     ///< Avance en Z.
    PyNumber feedrateRapid;                 ///< Avance de rápidos.
    PyNumber zCut;                          ///< Z de corte.
    bool multidepth{false};                 ///< Profundidad múltiple.
    PyNumber zDepthPerCut;                  ///< Profundidad por pasada.
    PyNumber zMove;                         ///< Z de viaje.
    bool useUi{false};                      ///< `true` si los datos vienen de la interfaz (taladro por herramienta).
    bool toolchange{false};                 ///< Hay cambio de herramienta.
    PyNumber zToolchange;                   ///< Z de cambio de herramienta.
    std::optional<std::array<PyNumber, 2>> xyToolchange;  ///< X,Y de cambio de herramienta.
    std::optional<PyNumber> startZ;         ///< Z inicial (`None` si no hay).
    PyNumber zEnd;                          ///< Z final.
    std::optional<std::array<PyNumber, 2>> xyEnd;         ///< X,Y final.
    int stepsPerCircle{64};                 ///< Pasos por circunferencia.
    std::string ppExcellonName{"default"};  ///< Nombre del postprocesador de Excellon.
    std::string ppGeometryName{"default"};  ///< Nombre del postprocesador de geometría.
    std::optional<PyNumber> spindleSpeed;   ///< Velocidad del husillo (`None` si no hay).
    std::string spindleDir{"CW"};           ///< `"CW"` o `"CCW"`.
    PyNumber dwellTime;                     ///< Tiempo de espera.
    int tool{0};                            ///< Herramienta actual.
    PyNumber toolC;                         ///< Diámetro de la herramienta actual.
    bool fPlunge{false};                    ///< Plunge rápido tras el cambio de herramienta.
    double x{0.0};                          ///< X de la posición actual.
    double y{0.0};                          ///< Y de la posición actual.

    PreprocessorOptions options;            ///< Opciones del objeto CNC.
    std::vector<ExcToolInfo> excTools;      ///< Herramientas de taladrado (`exc_tools`), en orden.
    std::vector<ExcToolInfo> excCncTools;   ///< Herramientas del trabajo (`exc_cnc_tools`), en orden.
};

/**
 * @brief Versión de FlatCAM cuyo G-code se reproduce.
 *
 * El G-code de referencia del usuario salió de FlatCAM 8.994, y el código fuente que se porta es el de la 2024.4. Casi
 * todo coincide byte por byte, pero el postprocesador `GRBL_laser` y el orden de las primeras líneas de un trabajo sin
 * cambio de herramienta cambiaron entre una y otra. Esta opción elige cuál de las dos se genera.
 */
enum class GcodeFlavor {
    FlatCAM2024_4,  ///< Igual que FlatCAM 2024.4 (el código fuente de referencia).
    FlatCAM8_994    ///< Igual que el G-code generado por FlatCAM 8.994 del usuario (**predeterminada**, por decisión del usuario).
};

/**
 * @brief Interfaz de un postprocesador: genera cada tipo de línea de G-code para un controlador concreto.
 *
 * Cada método recibe los parámetros y devuelve el texto de la línea (sin el salto de línea final), igual que los
 * de `PreProc` en Python.
 */
class Preprocessor {
public:
    virtual ~Preprocessor() = default;

    /** @brief Nombre con el que se elige el postprocesador. @return El nombre. */
    virtual std::string name() const = 0;

    /** @brief Cabecera del programa. @param p Parámetros. @return El texto. */
    virtual std::string startCode(const PreprocessorParams& p) const = 0;
    /** @brief Subida a la Z inicial. @param p Parámetros. @return El texto. */
    virtual std::string startzCode(const PreprocessorParams& p) const = 0;
    /** @brief Subida a la Z de viaje. @param p Parámetros. @return El texto. */
    virtual std::string liftCode(const PreprocessorParams& p) const = 0;
    /** @brief Bajada a la Z de corte. @param p Parámetros. @return El texto. */
    virtual std::string downCode(const PreprocessorParams& p) const = 0;
    /** @brief Cambio de herramienta. @param p Parámetros. @return El texto. */
    virtual std::string toolchangeCode(const PreprocessorParams& p) const = 0;
    /** @brief Subida hasta Z = 0. @param p Parámetros. @return El texto. */
    virtual std::string upToZeroCode(const PreprocessorParams& p) const = 0;
    /** @brief Coordenadas X e Y de la posición actual. @param p Parámetros. @return El texto. */
    virtual std::string positionCode(const PreprocessorParams& p) const = 0;
    /** @brief Movimiento rápido. @param p Parámetros. @return El texto. */
    virtual std::string rapidCode(const PreprocessorParams& p) const = 0;
    /** @brief Movimiento lineal. @param p Parámetros. @return El texto. */
    virtual std::string linearCode(const PreprocessorParams& p) const = 0;
    /** @brief Final del programa. @param p Parámetros. @return El texto. */
    virtual std::string endCode(const PreprocessorParams& p) const = 0;
    /** @brief Avance de trabajo. @param p Parámetros. @return El texto. */
    virtual std::string feedrateCode(const PreprocessorParams& p) const = 0;
    /** @brief Avance en Z. @param p Parámetros. @return El texto. */
    virtual std::string zFeedrateCode(const PreprocessorParams& p) const = 0;
    /** @brief Encendido del husillo. @param p Parámetros. @return El texto. */
    virtual std::string spindleCode(const PreprocessorParams& p) const = 0;
    /** @brief Espera. @param p Parámetros. @return El texto. */
    virtual std::string dwellCode(const PreprocessorParams& p) const = 0;
    /** @brief Apagado del husillo. @param p Parámetros. @return El texto. */
    virtual std::string spindleStopCode(const PreprocessorParams& p) const = 0;
};

/**
 * @brief Postprocesador `default`: Mach3 y compatibles (`preprocessors/default.py`).
 */
class DefaultPreprocessor : public Preprocessor {
public:
    std::string name() const override { return "default"; }
    std::string startCode(const PreprocessorParams& p) const override;
    std::string startzCode(const PreprocessorParams& p) const override;
    std::string liftCode(const PreprocessorParams& p) const override;
    std::string downCode(const PreprocessorParams& p) const override;
    std::string toolchangeCode(const PreprocessorParams& p) const override;
    std::string upToZeroCode(const PreprocessorParams& p) const override;
    std::string positionCode(const PreprocessorParams& p) const override;
    std::string rapidCode(const PreprocessorParams& p) const override;
    std::string linearCode(const PreprocessorParams& p) const override;
    std::string endCode(const PreprocessorParams& p) const override;
    std::string feedrateCode(const PreprocessorParams& p) const override;
    std::string zFeedrateCode(const PreprocessorParams& p) const override;
    std::string spindleCode(const PreprocessorParams& p) const override;
    std::string dwellCode(const PreprocessorParams& p) const override;
    std::string spindleStopCode(const PreprocessorParams& p) const override;
};

/**
 * @brief Postprocesador `GRBL_laser`: GRBL con un láser en el conector del husillo (`preprocessors/GRBL_laser.py`).
 *
 * No genera movimientos en Z: subir y bajar equivale a apagar y encender el láser (`M5` / `M03 S..`).
 */
class GrblLaserPreprocessor : public Preprocessor {
public:
    /**
     * @brief Crea el postprocesador.
     * @param flavor Con @ref GcodeFlavor::FlatCAM8_994 la cabecera lleva `G00 Z0` y el cierre no lleva `G00 Z..`.
     */
    explicit GrblLaserPreprocessor(GcodeFlavor flavor = GcodeFlavor::FlatCAM8_994) : flavor_(flavor) {}

    std::string name() const override { return "GRBL_laser"; }
    std::string startCode(const PreprocessorParams& p) const override;
    std::string startzCode(const PreprocessorParams& p) const override;
    std::string liftCode(const PreprocessorParams& p) const override;
    std::string downCode(const PreprocessorParams& p) const override;
    std::string toolchangeCode(const PreprocessorParams& p) const override;
    std::string upToZeroCode(const PreprocessorParams& p) const override;
    std::string positionCode(const PreprocessorParams& p) const override;
    std::string rapidCode(const PreprocessorParams& p) const override;
    std::string linearCode(const PreprocessorParams& p) const override;
    std::string endCode(const PreprocessorParams& p) const override;
    std::string feedrateCode(const PreprocessorParams& p) const override;
    std::string zFeedrateCode(const PreprocessorParams& p) const override;
    std::string spindleCode(const PreprocessorParams& p) const override;
    std::string dwellCode(const PreprocessorParams& p) const override;
    std::string spindleStopCode(const PreprocessorParams& p) const override;

private:
    GcodeFlavor flavor_;
};

/**
 * @brief Busca un postprocesador por nombre.
 * @param name `"default"` o `"GRBL_laser"`.
 * @param flavor Versión de FlatCAM cuyo G-code se reproduce (`default` es igual en las dos).
 * @return El postprocesador, o `nullptr` si no existe.
 */
std::unique_ptr<Preprocessor> makePreprocessor(const std::string& name,
                                               GcodeFlavor flavor = GcodeFlavor::FlatCAM8_994);

}  // namespace flatcam
