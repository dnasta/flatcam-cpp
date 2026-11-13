/**
 * @file Calibration.h
 * @brief Calibración (port de las cuentas de `appTools/ToolCalibration.py`): el G-code de verificación de los cuatro
 * puntos, los factores de escala e inclinación a partir de lo medido y los puntos escalados o inclinados.
 */
#pragma once

#include <array>
#include <optional>
#include <string>
#include <vector>

#include "core/Types.h"

namespace flatcam {

/** @brief Los cuatro puntos (`click_points`): abajo a la izquierda, abajo a la derecha, arriba a la izquierda y arriba
 *  a la derecha. */
using CalibrationPoints = std::array<Point2D, 4>;

/** @brief Los parámetros del G-code de verificación. */
struct CalibrationGcodeParams {
    std::string units{"MM"};           ///< `app.defaults['units']` en mayúsculas.
    double travelZ{2.0};               ///< Z de viaje.
    double toolchangeZ{15.0};          ///< Z de cambio de herramienta.
    double verificationZ{0.1};         ///< Z de verificación.
    std::vector<double> toolchangeXy;  ///< X,Y de cambio (vacío: no hay).
    bool zeroZ{false};                 ///< "Z cero" (`zeroz_cb`).
    std::string secondPoint{"tl"};     ///< `tl` o `br`.
    int decimals{4};                   ///< `app.decimals`.
    std::string version;               ///< Versión de FlatCAM.
    std::string versionDate;           ///< Su fecha.
    std::string timeStr;               ///< "Created on".
    std::string name;                  ///< "Verification GCode for FlatCAM Calibration Tool", traducido.
};

/** @brief `gcode_header` + `generate_verification_gcode`. @param pts Los puntos. @param p Parámetros.
 *  @return El G-code. */
std::string calibrationGcode(const CalibrationPoints& pts, const CalibrationGcodeParams& p);

/** @brief Lo que `calculate_factors` escribe en los campos (vacío: no los toca). */
struct CalibrationFactors {
    std::optional<double> scaleX;  ///< Escala en X.
    std::optional<double> scaleY;  ///< Escala en Y.
    std::optional<double> skewX;   ///< Inclinación en X (grados).
    std::optional<double> skewY;   ///< Inclinación en Y (grados).
    bool failed{false};            ///< Una división por cero cortó la cuenta (lo anterior queda escrito).
};

/**
 * @brief `calculate_factors`.
 * @param pts Los puntos.
 * @param topLeftDx Delta medido en X del punto de arriba a la izquierda (vacío: el campo no es un número).
 * @param topLeftDy Ídem en Y. @param botRightDx Delta en X del de abajo a la derecha. @param botRightDy Ídem en Y.
 * @param decimals `app.decimals`.
 * @return Los factores.
 */
CalibrationFactors calibrationFactors(const CalibrationPoints& pts, std::optional<double> topLeftDx,
                                      std::optional<double> topLeftDy, std::optional<double> botRightDx,
                                      std::optional<double> botRightDy, int decimals);

/** @brief `on_scale_button`: los puntos escalados desde el primero. @param pts Puntos. @param sx Factor en X.
 *  @param sy Factor en Y. @return Los puntos nuevos. */
CalibrationPoints calibrationScale(const CalibrationPoints& pts, double sx, double sy);

/** @brief `on_skew_button`: los puntos inclinados desde el primero. @param pts Puntos. @param ax Grados en X.
 *  @param ay Grados en Y. @return Los puntos nuevos. */
CalibrationPoints calibrationSkew(const CalibrationPoints& pts, double ax, double ay);

/** @brief El valor redondeado como el original (lo escribe con @p decimals cifras y lo vuelve a leer).
 *  @param v Valor. @param decimals Decimales. @return El valor redondeado. */
double calibrationRound(double v, int decimals);

}  // namespace flatcam
